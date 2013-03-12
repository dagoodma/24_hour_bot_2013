/*
 * File:   main.c
 * Author: dagoodma
 *
 * Created on February 21, 2012, 7:46 PM
 */

//#define USE_MAIN

//#define DEBUG


//#define DEBUG_STATES // slows and blinks LEDs
//#define DISABLE_AVOID
//#define DEBUG_VERBOSE

//#define TAPE_CALIBRATE 1

//#define TARGET_USETIME


#ifdef USE_MAIN
#include <xc.h>
#include "serial.h"
#include "PORTS.h"
#include "timers.h"
#include "Drive.h"
#include "TapeSensor.h"
#include "IR.h"
#include "Bumper.h"
#include "AD.h"
#include "Util.h"
#include "LED.h"
//#include <stdio.h>


/*******************************************************************************
 * #DEFINES                                                                    *
 ******************************************************************************/
#define TIMER_START 6
#define TIMER_MOVE 7
#define TIMER_AVOID 8
#define TIMER_FIND 9
#define TIMER_FOLLOW 10
#define TIMER_CALIBRATE 11
#define TIMER_RETURN 12
#define TIMER_CHARGE 13
#define TIMER_OBSTACLE 14
#define MASTER_TIMER 15

#define MASTER_TIMEOUT 11000


// All delay times in (ms)
#define START_TAPE_DELAY        2500
#define ATTACK_REVERSE_DELAY    2000
#define AVOID_ARC_DELAY         2000


/*******************************************************************************
 * VARIABLES                                                                   *
 ******************************************************************************/

//---------- Top state variables ------------
static enum { search, attack, avoidTape} topState;

//-------- Search state variables -----------

static enum { searchState_rightIR, searchState_leftIR } searchState;
static enum { searchEvent_none, searchEvent_bothFound, searchEvent_rightFound,
    searchEvent_rightLost } searchEvent;

//-------- Attack state variables -----------

static enum { attackState_transition, attackState_charge, attackState_reverse } attackState;
static enum { attackEvent_none, attackEvent_lostTarget, attackEvent_hitTape,
    attackEvent_hitObject, attackEvent_reverseDone} attackEvent;


//-------- AvoidTape state variables -----------

static enum { avoidState_transition, avoidState_front, avoidState_back } avoidTapeState;
static enum { avoidEvent_none, avoidEvent_resolved, avoidEvent_backTape,
    avoidEvent_frontTape } avoidTapeEvent;



/*******************************************************************************
 * FUNCTION  PROTOTYPES                                                        *
 ******************************************************************************/
void doTopSM();

void doSearchSM();
void doAttackSM();
void doAvoidTapeSM();

void checkSearchEvents();
void checkAttackEvents();
void checkAvoidTapeEvents();

void startSearchSM();
void startAttackSM();
void startAvoidTapeSM();


void startMasterSM();
void doMasterSM();


/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/

// ----------------------------------------------------------------------------
// ------------------------------- SearchSM -----------------------------------
// ----------------------------------------------------------------------------
void checkSearchEvents() {
    searchEvent = searchEvent_none;

    switch(searchState) {
        case searchState_rightIR:
            if (!IR_LeftTriggered() && IR_RightTriggered())
                searchEvent = searchEvent_rightFound;
            else if (IR_LeftTriggered() && IR_RightTriggered())
                searchEvent = searchEvent_bothFound;
            break;
        case searchState_leftIR:
            if (!IR_RightTriggered())
                searchEvent = searchEvent_rightLost;
            else if (IR_LeftTriggered() && IR_RightTriggered())
                searchEvent = searchEvent_bothfound;
            break;
        default:
            break;
    }
}

void doSearchSM() {
    checkSearchEvents();

    switch (searchState) {
        case searchState_rightIR:
            if (searchEvent == searchEvent_rightFound) {
                Drive_Turn(pivot,right,HALF_SPEED);
                searchState = searchState_leftIR;
            }
            else if (searchEvent == searchEvent_bothFound) {
                Drive_Stop();
                // Transitions out
            }
            break;
        case searchState_leftIR:
            if (searchEvent == searchEvent_rightLost) {
                Drive_Turn(pivot,left,HALF_SPEED);
                searchState = searchState_rightIR;
            }
            else if (searchEvent == searchEvent_bothFound) {
                Drive_Stop();
                // Transitions out
            }
            break;
        default:
            break;
    }
}

// ----------------------------------------------------------------------------
// ------------------------------- AttackSM -----------------------------------
// ----------------------------------------------------------------------------

void checkAttackEvents() {
    attackEvent = attackEvent_none;

    switch (attackState) {
        case attackState_transition:
            if (!IR_RightTriggered() || !IR_LeftTriggered())
                attackEvent = attackEvent_lostTarget;
            else if (Tape_AnyTriggered())
                attackEvent = attackEvent_hitTape;
            else if (Bumper_AnyFrontTriggered())
                attackEvent = attackEvent_hitObject;
            break;
        case attackState_charge:
            break;
        case attackState_reverse:
            if (IsTimerExpired(TIMER_REVERSE))
                attackEvent = attackEvent_reverseDone;
            else if (Tape_AnyTriggered())
                attackEvent = attackEvent_hitTape;
            break;
        default:
            break;
    }
}

void doAttackSM() {
    checkAttackEvents();

    switch (attackState) {
        case attackState_transition:
            if (attackEvent == attackEvent_none) {
                attackState = attackState_charge;
                Drive_Forward(FULL_SPEED);
            }
            else if (attackEvent == attackEvent_lostTarget) {
                Drive_Stop();
                // Transition out
            }
            else if (attackEvent == attackEvent_hitTape) {
                Drive_Stop();
                // Transition out
            }
            else if (attackEvent == attackEvent_hitObject) {
                InitTimer(TIMER_REVERSE, ATTACK_REVERSE_DELAY);
                Drive_Reverse(HALF_SPEED);
            }
            break;
        case attackState_charge:
            attackState = attackState_transition;
            break;
        case attackState_reverse:
            if (attackEvent == attackEvent_reverseDone) {
                attackState = attackState_transition;
                Drive_Stop();
            }
            else if (attackEvent == attackEvent_hitTape) {
                Drive_Stop();
                // Transition out
            }
            break;
        default:
            break;
    }
}

// ----------------------------------------------------------------------------
// ----------------------------- AvoidTapeSM ----------------------------------
// ----------------------------------------------------------------------------

void checkAvoidTapeEvents() {
    avoidEvent = avoidEvent_none;

    switch (avoidState) {
        case avoidState_transition:
            if (Tape_AnyFrontTriggered())
                avoidEvent = avoidEvent_frontTape;
            else if (Tape_AnyBackTriggered())
                avoidEvent = avoidEvent_backTape;
            break;
        case avoidState_front:
            if (Tape_AnyBackTriggered())
                avoidEvent = avoidEvent_backTape;
            else if (IsTimerExpired(TIMER_TAPE))
                avoidEvent = avoidEvent_resolved;
            break;
        case avoidState_back:
            if (Tape_AnyFrontTriggered())
                avoidEvent = avoidEvent_frontTape;
            else if (IsTimerExpired(TIMER_TAPE))
                avoidEvent = avoidEvent_resolved;
            break;
        default:
            break;
    }

}

void doAvoidTapeSM() {
    checkAvoidTapeEvents();

    switch (avoidState) {
        case avoidState_transition:
            if (avoidEvent == avoidEvent_backTape)
                avoidState = avoidState_back;
            else if (avoidEvent == avoidEvent_frontTape)
                avoidState = avoidState_front;

            InitTimer(TAPE_TIMER, AVOID_ARC_DELAY);
            break;
        case avoidState_front:
            if (avoidEvent == avoidEvent_backTape) {
                InitTimer(TAPE_TIMER, AVOID_ARC_DELAY);
                avoidState = avoidState_back;
            }
            else if (avoidEvent == avoidEvent_arcDone) {
                // Transition out
            }
            break;
        case avoidState_back:
            if (avoidEvent == avoidEvent_frontTape) {
                InitTimer(TAPE_TIMER, AVOID_ARC_DELAY);
                avoidState = avoidState_front;
            }
            else if (avoidEvent == avoidEvent_arcDone) {
                // Transition out
            }
            break;
        default:
            break;
    }

}



// ----------------------- Main state machine handler -----------------


void startMasterSM() {
    // ---------------------------- Initialization ----------------------------
    SERIAL_Init();
    TIMERS_Init();

    // Initialize interrupts
    INTEnableSystemMultiVectoredInt();
    time = GetTime();

    //int adPins = TAPE_LEFT | TAPE_CENTER | TAPE_RIGHT | TAPE_BACK |
    //     TAPE_ARMFRONT | TAPE_ARMLEFT | TAPE_ARMRIGHT | BAT_VOLTAGE | IR_PINS;

    //AD_Init(adPins);
    // Initialize modules
    IR_Init();
    Tape_Init();
    Drive_Init();
    Bumper_Init();
    Baller_Init();

    startSearchSM();

    InitTimer(TIMER_START, START_TAPE_DELAY);
}


void startSearchSM() {
    searchState = searchState_rightIR;

    //Baller_Start();
    Drive_Turn(pivot,left,HALF_SPEED);
}

void startAttackSM() {
    attackState = attackState_transition;
}

void startAvoidTapeSM() {
    avoidState = avoidState_transition;
}

/*
 * @Function doTopSM
 * @remark
 * @date 2012.3.6 08:30 */
void doTopSM() {
    switch (topState) {
        case search:
            doSearchSM();
            if (searchEvent == searchEvent_bothFound)
                startAttackSM();
            break;
        case attack:
            doAttackSM();
            if (attackEvent == attackEvent_lostTarget)
                startSearchSM();
            if (attackEvent == attackEvent_hitTape)
                startAvoidTapeSM();
            break;
        case avoidTape:
            doAvoidTapeSM();
            if (avoidEvent == avoidEvent_resolved)
                startSearchSM();
            break;
        default:
            break;
    }
}

/*******************************************************************************
 * ENTRY POINT                                                                 *
 ******************************************************************************/

int main(void) {

    // ------------------------------- Main Loop -------------------------------
    while (1) {
        // Handle updates and module state machines
        Tape_HandleSM();
        Drive_Update();
        Bumper_Update();
        IR_Update();
        
        doTopSM();
    }

    exit:
    Tape_End();
    Drive_Stop();
    Bumper_End();
    Gate_End();
    IR_End();

    return 0;
} // end of main()

// ---------------------- EOF ----------------------
#endif

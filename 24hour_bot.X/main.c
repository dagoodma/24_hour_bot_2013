/*
 * File:   main.c
 * Author: dagoodma
 *
 * Created on February 21, 2012, 7:46 PM
 */

#define USE_MAIN
//#define USE_TRACK_TEST

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
#include <stdint.h>
#include "pwm.h"
#include "Shooter.h"
//#include <stdio.h>


/*******************************************************************************
 * #DEFINES                                                                    *
 ******************************************************************************/

#define SEARCH_SPEED            2//LOWEST_SPEED
#define FAST_SEARCH_SPEED       5//100 //MIN_SPEED

// All delay times in (ms)
#define START_TAPE_DELAY        2500
#define ATTACK_REVERSE_DELAY    1000
#define AVOID_ARC_DELAY         2000



#define DEBUG_VERBOSE
#ifdef DEBUG_VERBOSE
    #define dbprintf(...) printf(__VA_ARGS__)
#else
    #define dbprintf(...)
#endif


#define FREQUENCY_PWM 250


/*******************************************************************************
 * VARIABLES                                                                   *
 ******************************************************************************/

//---------- Top state variables ------------
static enum { search, attack, avoidTape} topState;

//-------- Search state variables -----------

static enum { searchState_rightIR, searchState_leftIR, searchState_fastSearch } searchState;
static enum { searchEvent_none, searchEvent_bothFound, searchEvent_rightFound,
    searchEvent_leftLost, searchEvent_leftFound, searchEvent_rightLost,
    searchEvent_anyFound, searchEvent_bothLost } searchEvent;

//-------- Attack state variables -----------

static enum { attackState_transition, attackState_charge, attackState_reverse } attackState;
static enum { attackEvent_none, attackEvent_lostTarget, attackEvent_hitTape,
    attackEvent_hitObject, attackEvent_reverseDone} attackEvent;


//-------- AvoidTape state variables -----------

static enum { avoidState_transition, avoidState_resolveFront,
        avoidState_resolveBack } avoidTapeState;
static enum { avoidEvent_none, avoidEvent_resolved, avoidEvent_backTape,
    avoidEvent_frontTape } avoidTapeEvent;

uint32_t time = 0;

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
        case searchState_fastSearch:
            if (IR_LeftTriggered() || IR_RightTriggered())
                searchEvent = searchEvent_anyFound;
            break;
        case searchState_leftIR:
            if (IR_LeftTriggered())
                searchEvent = searchEvent_leftFound;
            else if (IR_LeftTriggered() && IR_RightTriggered())
                searchEvent = searchEvent_bothFound;
            else if (!IR_LeftTriggered() && !IR_RightTriggered())
                searchEvent = searchEvent_bothLost;
            break;
        case searchState_rightIR:
            // TODO add tagert moves right exception
            if (!IR_LeftTriggered())
                searchEvent = searchEvent_leftLost;
            else if (IR_LeftTriggered() && IR_RightTriggered())
                searchEvent = searchEvent_bothFound;
            else if (!IR_LeftTriggered() && !IR_RightTriggered())
                searchEvent = searchEvent_bothLost;
            break;
        default:
            break;
    }
}

void doSearchSM() {
    checkSearchEvents();

    switch (searchState) {
        // search fast right
        case searchState_fastSearch:
            if (searchEvent == searchEvent_anyFound) {
                searchState = searchState_rightIR;
                Drive_Pivot(left,SEARCH_SPEED);
            }
            else {
                Drive_Pivot(right, FAST_SEARCH_SPEED);
            }
            break;

        case searchState_leftIR:
            if (searchEvent == searchEvent_leftFound) {
                Drive_Pivot(right,SEARCH_SPEED);
                searchState = searchState_rightIR;
            }
            else if (searchEvent == searchEvent_bothFound) {
                Drive_Stop();
                // Transitions out
            }
            else if (searchEvent == searchEvent_bothLost) {
                searchState = searchState_fastSearch;
                Drive_Pivot(right, FAST_SEARCH_SPEED);
            }
            break;
        case searchState_rightIR:
            if (searchEvent == searchEvent_leftLost) {
                Drive_Pivot(left,SEARCH_SPEED);
                searchState = searchState_leftIR;
            }
            else if (searchEvent == searchEvent_bothFound) {
                Drive_Stop();
                // Transitions out
            }
            else if (searchEvent == searchEvent_bothLost) {
                searchState = searchState_fastSearch;
                Drive_Pivot(right, FAST_SEARCH_SPEED);
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
            if (Tape_AnyTriggered())
                attackEvent = attackEvent_hitTape;
            else if (IsTimerExpired(TIMER_ATTACK))
                attackEvent = attackEvent_reverseDone;
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
                Drive_Forward(HALF_SPEED);
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
                attackState = attackState_reverse;
                InitTimer(TIMER_ATTACK, ATTACK_REVERSE_DELAY);
                Drive_Reverse(LOW_SPEED);
            }
            break;
        case attackState_charge:
            attackState = attackState_transition;
            break;
        case attackState_reverse:
            // TODO Add 5 times counter
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
// TODO NOW remove this state machine
void checkAvoidTapeEvents() {
    avoidTapeEvent = avoidEvent_none;

    switch (avoidTapeState) {
        case avoidState_transition:
            if (Tape_AnyFrontTriggered())
                avoidTapeEvent = avoidEvent_frontTape;
            else if (Tape_AnyBackTriggered())
                avoidTapeEvent = avoidEvent_backTape;
            break;
        case avoidState_resolveFront:
            if (Tape_AnyBackTriggered())
                avoidTapeEvent = avoidEvent_backTape;
            else if (IsTimerExpired(TIMER_TAPEAVOID))
                avoidTapeEvent = avoidEvent_resolved;
            break;
        case avoidState_resolveBack:
            if (Tape_AnyFrontTriggered())
                avoidTapeEvent = avoidEvent_frontTape;
            else if (IsTimerExpired(TIMER_TAPEAVOID))
                avoidTapeEvent = avoidEvent_resolved;
            break;
        default:
            break;
    }
}

void doAvoidTapeSM() {
    checkAvoidTapeEvents();
    // TODO consider hit tape, stop and find them,
    //  since they're assumed to be in bounds
    switch (avoidTapeState) {
        case avoidState_transition:
            if (avoidTapeEvent == avoidEvent_backTape)
                avoidTapeState = avoidState_resolveBack;
            else if (avoidTapeEvent == avoidEvent_frontTape)
                avoidTapeState = avoidState_resolveFront;

            InitTimer(TIMER_TAPEAVOID, AVOID_ARC_DELAY);
            break;
        case avoidState_resolveBack:
            if (avoidTapeEvent == avoidEvent_frontTape) {
                InitTimer(TIMER_TAPEAVOID, AVOID_ARC_DELAY);
                avoidTapeState = avoidState_resolveFront;
            }
            else if (avoidTapeEvent == avoidEvent_resolved) {
                Drive_Stop();
                // Transition out
            }
            else {
                Drive_Forward(MIN_SPEED);
            }
            break;
        case avoidState_resolveFront:
            if (avoidTapeEvent == avoidEvent_backTape) {
                InitTimer(TIMER_TAPEAVOID, AVOID_ARC_DELAY);
                avoidTapeState = avoidState_resolveBack;
            }
            else if (avoidTapeEvent == avoidEvent_resolved) {
                Drive_Stop();
                // Transition out
            }
            else {
                Drive_Reverse(MIN_SPEED);
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

    int adPins = TAPE_FRONTLEFT | TAPE_FRONTRIGHT | TAPE_BACKRIGHT
        | TAPE_BACKLEFT | BAT_VOLTAGE | IR_PINS;
    AD_Init(adPins);

    int pwmPins = MOTOR_A_PWM |  MOTOR_B_PWM; //| DC_MOTOR_PWM;
    PWM_Init(pwmPins, FREQUENCY_PWM);

    // Initialize modules
    IR_Init();
    //Tape_Init();
    Drive_Init();
    //Bumper_Init();
#ifdef USE_BALLER
    Baller_Init();
#endif
    Shooter_Init();

    startSearchSM();

    InitTimer(TIMER_START, START_TAPE_DELAY);
}


void startSearchSM() {
    topState = search;
    searchState = searchState_fastSearch;
    dbprintf("Started in state: search\n");
}

void startAttackSM() {
    topState = attack;
    attackState = attackState_transition;
    Drive_Stop();
    wait();
#ifdef USE_BALLER
    Baller_Start();
#endif
    dbprintf("Started in state: attack\n");
}

void startAvoidTapeSM() {
    topState = avoidTape;
    avoidTapeState = avoidState_transition;
    dbprintf("Started in state: avoidTape\n");
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
                startSearchSM();
                //startAvoidTapeSM();
            break;
        case avoidTape:
            doAvoidTapeSM();
            if (avoidTapeEvent == avoidEvent_resolved)
                startSearchSM();
            break;
        default:
            break;
    }
}

/*******************************************************************************
 * ENTRY POINT                                                                 *
 ******************************************************************************/

#ifndef USE_TRACK_TEST
int main(void) {
    startMasterSM();
    // ------------------------------- Main Loop -------------------------------
    while (1) {
        // Handle updates and module state machines
        //Tape_HandleSM();
        Drive_Update();
        //Bumper_Update();
        IR_Update();
        
        doTopSM();
    }

    exit:
    //Tape_End();
    Drive_Stop();
    //Bumper_End();
    IR_End();

    return 0;
} // end of main()

#else
// Tracking test harness
int main(void) {
    startMasterSM();
    dbprintf("Initialized master state machine.\n");
    // ------------------------------- Main Loop -------------------------------
    while (1) {
        // Handle updates and module state machines
        //Tape_HandleSM();
        Drive_Update();
        //Bumper_Update();
        IR_Update();
        Shooter_doSM();

        //doTopSM();
        doSearchSM();
        if (searchEvent == searchEvent_bothFound)
            return SUCCESS;
            //startSearchSM();
    }
}

#endif

// ---------------------- EOF ----------------------
#endif

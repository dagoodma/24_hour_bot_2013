/*
 * File:   main.c
 * Author: dagoodma
 *
 * Created on February 21, 2012, 7:46 PM
 */

#define USE_MAIN
#define USE_TRACK_TEST

//#define DEBUG

#ifdef USE_MAIN
#include <xc.h>
#include <stdint.h>
#include "serial.h"
#include "PORTS.h"
#include "timers.h"
#include "Drive.h"
#include "IR.h"
#include "Bumper.h"
#include "AD.h"
#include "Util.h"
#include "LED.h"
#include "pwm.h"
#include "Shooter.h"
//#include <stdio.h>


/*******************************************************************************
 * #DEFINES                                                                    *
 ******************************************************************************/

#define SLOW_SEARCH_SPEED       LOW_SPEED
#define SEARCH_SPEED            HALF_SPEED

// All delay times in (ms)
#define ATTACK_REVERSE_DELAY    1000

#define SEARCH_OVERSHOOT_DELAY  550



//#define DEBUG_VERBOSE
#ifdef DEBUG_VERBOSE
    #define dbprintf(...)   do { printf(__VA_ARGS__); wait(); } while(0)
#else
    #define dbprintf(...)
#endif


#define FREQUENCY_PWM 250

/*******************************************************************************
 * VARIABLES                                                                   *
 ******************************************************************************/

//---------- Top state variables ------------
static enum { search, attack } topState;

//-------- Search state variables -----------

static enum { searchState_rightIR, searchState_leftIR, searchState_bothIR,
searchState_transition } searchState;
static enum { searchEvent_none, searchEvent_bothFound, searchEvent_rightFound,
    searchEvent_leftFound, searchEvent_anyLost, searchEvent_searchLeft,
        searchEvent_searchRight, searchEvent_bothConfirmed } searchEvent;

//-------- Attack state variables -----------

static enum { attackState_transition, attackState_shoot, attackState_charge, attackState_reverse } attackState;
static enum { attackEvent_none, attackEvent_lostTarget, attackEvent_hitObject,
    attackEvent_reverseDone} attackEvent;

uint32_t time = 0;

/*******************************************************************************
 * FUNCTION  PROTOTYPES                                                        *
 ******************************************************************************/
void doTopSM();

void doSearchSM();
void doAttackSM();

void checkSearchEvents();
void checkAttackEvents();

void startSearchSM();
void startAttackSM();


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
        case searchState_transition:
            if (!IR_RightTriggered())
                searchEvent = searchEvent_searchRight;
            else
                searchEvent = searchEvent_searchLeft;
            break;
        case searchState_leftIR:
            if (IR_LeftTriggered() && IR_RightTriggered())
                searchEvent = searchEvent_bothFound;
            else if (IR_LeftTriggered() && !IR_RightTriggered())
                searchEvent = searchEvent_leftFound;
            break;
        case searchState_rightIR:
            if (IR_LeftTriggered() && IR_RightTriggered())
                searchEvent = searchEvent_bothFound;
            else if (!IR_LeftTriggered() && IR_RightTriggered())
                searchEvent = searchEvent_rightFound;
            break;
        case searchState_bothIR:
            if (IsTimerExpired(TIMER_SEARCH) && (!IR_LeftTriggered()
                    || !IR_RightTriggered()))
                searchEvent = searchEvent_anyLost;
            else if (IsTimerExpired(TIMER_SEARCH) && IR_LeftTriggered()
                    && IR_RightTriggered())
                searchEvent = searchEvent_bothConfirmed;
    }
}

void doSearchSM() {
    checkSearchEvents();

    
    switch (searchState) {
        case searchState_transition:
            if (searchEvent == searchEvent_searchRight) {
                Drive_Pivot(right,SEARCH_SPEED);
                searchState = searchState_rightIR;
            }
            else {
                Drive_Pivot(left,SEARCH_SPEED);
                searchState = searchState_leftIR;
            }
            break;
        case searchState_leftIR:
            if (searchEvent == searchEvent_leftFound) {
                Drive_Pivot(right,SEARCH_SPEED);
                searchState = searchState_rightIR;
            }
            else if (searchEvent == searchEvent_bothFound) {
                Drive_Pivot(right,SLOW_SEARCH_SPEED);
                InitTimer(TIMER_SEARCH, SEARCH_OVERSHOOT_DELAY);
                searchState = searchState_bothIR;
            }
            break;
        case searchState_rightIR:
            if (searchEvent == searchEvent_rightFound) {
                Drive_Pivot(left,SEARCH_SPEED);
                searchState = searchState_leftIR;
            }
            else if (searchEvent == searchEvent_bothFound) {
                Drive_Pivot(left,SLOW_SEARCH_SPEED);
                InitTimer(TIMER_SEARCH, SEARCH_OVERSHOOT_DELAY);
                searchState = searchState_bothIR;
            }
            break;
        case searchState_bothIR:
            if (searchEvent == searchEvent_bothConfirmed) {
                Drive_Stop();
                // transition out
            }
            else if (searchEvent == searchEvent_anyLost) {
                Drive_Stop();
                searchState = searchState_transition;
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
            else if (Bumper_AnyFrontTriggered())
                attackEvent = attackEvent_hitObject;
            break;
        case attackState_charge:
            break;
        case attackState_reverse:
            if (IsTimerExpired(TIMER_ATTACK))
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
                if (Shooter_hasAmmo()) {
                    attackState = attackState_shoot;
                    Shooter_startShooting();
                }
                else {
                    attackState = attackState_charge;
                    Drive_Forward(HALF_SPEED);
                }
            }
            else if (attackEvent == attackEvent_lostTarget) {
                Drive_Stop();
                // Transition out
            }
            else if (attackEvent == attackEvent_hitObject) {
                attackState = attackState_reverse;
                InitTimer(TIMER_ATTACK, ATTACK_REVERSE_DELAY);
                Drive_Reverse(LOW_SPEED);
            }
            break;
        case attackState_shoot:
            attackState = attackState_transition;
            break;
        case attackState_charge:
            attackState = attackState_transition;
            break;
        case attackState_reverse:
            // reverses after rammng
            // TODO Add 5 times counter
            if (attackEvent == attackEvent_reverseDone) {
                attackState = attackState_transition;
                Drive_Stop();
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

    int adPins = BAT_VOLTAGE | IR_PINS;
    AD_Init(adPins);

    int pwmPins = MOTOR_A_PWM |  MOTOR_B_PWM | DC_MOTOR_PWM;
    PWM_Init(pwmPins, FREQUENCY_PWM);

    // Initialize modules
    IR_Init();
    //Tape_Init();
    Drive_Init();
    //Bumper_Init();
    Shooter_Init();

    startSearchSM();
}


void startSearchSM() {
    topState = search;
    Drive_Stop();
    searchState = searchState_transition;

    dbprintf("Started in state: search\n");
}

void startAttackSM() {
    topState = attack;
    attackState = attackState_transition;
    Drive_Stop();
    dbprintf("Started in state: attack\n");
}

/*
 * @Function doTopSM
 * @remark
 * @date 2012.3.6 08:30 */
void doTopSM() {
    switch (topState) {
        case search:
            doSearchSM();
            if (searchEvent == searchEvent_bothConfirmed)
                startAttackSM();
            break;
        case attack:
            doAttackSM();
            if (attackEvent == attackEvent_lostTarget)
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
        Drive_Update();
        //Bumper_Update();
        IR_Update();
        Shooter_doSM();
        
        doTopSM();
    }

    exit:
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
        Drive_Update();
        //Bumper_Update();
        IR_Update();
        Shooter_doSM();

        doSearchSM();
        if (searchEvent == searchEvent_bothFound)
            startSearchSM();
    }
}

#endif

// ---------------------- EOF ----------------------
#endif

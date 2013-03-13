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
#define FAST_SEARCH_SPEED       HALF_SPEED

// All delay times in (ms)
#define ATTACK_REVERSE_DELAY    1000
#define AVOID_ARC_DELAY         2000


//#define DEBUG_VERBOSE
#ifdef DEBUG_VERBOSE
    #define dbprintf(...)   do { printf(__VA_ARGS__); wait(); } while(0)
#else
    #define dbprintf(...)
#endif


#define FREQUENCY_PWM 250

#define SEARCH_LOST_DELAY   1000


/*******************************************************************************
 * VARIABLES                                                                   *
 ******************************************************************************/

//---------- Top state variables ------------
static enum { search, attack } topState;

//-------- Search state variables -----------

static enum { searchState_rightIR, searchState_leftIR, searchState_fastSearch,
searchState_transition } searchState;
static enum { searchEvent_none, searchEvent_bothFound, searchEvent_rightFound,
    searchEvent_leftFound, searchEvent_anyFound, searchEvent_bothLost,
searchEvent_leftLost, searchEvent_rightLost } searchEvent;

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
#define USE_SIMPLE_SEARCH
#ifndef USE_SIMPLE_SEARCH
void checkSearchEvents() {
    searchEvent = searchEvent_none;

    switch(searchState) {
        case searchState_fastSearch:
            if (IR_LeftTriggered() || IR_RightTriggered())
                searchEvent = searchEvent_anyFound;
            break;
        case searchState_leftIR:
            if (IR_LeftTriggered() && IR_RightTriggered())
                searchEvent = searchEvent_bothFound;
            else if (IR_LeftTriggered())
                searchEvent = searchEvent_leftFound;
            else if (IsTimerExpired(TIMER_SEARCH)
                    && !IR_LeftTriggered() && !IR_RightTriggered())
                searchEvent = searchEvent_bothLost;
            break;
        case searchState_rightIR:
            // TODO add tagert moves right exception
            if (IR_LeftTriggered() && IR_RightTriggered())
                searchEvent = searchEvent_bothFound;
            if (IR_RightTriggered())
                searchEvent = searchEvent_rightFound;
            else if (IsTimerExpired(TIMER_SEARCH)
                    && !IR_LeftTriggered() && !IR_RightTriggered())
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
            // turn right quickly
            if (searchEvent == searchEvent_anyFound) {
                dbprintf("Search: Fast search found IR (%x)\n",searchState);
                searchState = searchState_leftIR;
                InitTimer(TIMER_SEARCH,SEARCH_LOST_DELAY);
                Drive_Pivot(left,SLOW_SEARCH_SPEED);
            }
            else {
                Drive_Pivot(right, FAST_SEARCH_SPEED);
            }
            break;

        case searchState_leftIR:
            // turn left slowly till left sensor hits
            if (searchEvent == searchEvent_leftFound) {
                dbprintf("Search: Left IR triggered (%x)\n",searchState);
                InitTimer(TIMER_SEARCH,SEARCH_LOST_DELAY);
                Drive_Pivot(right,SLOW_SEARCH_SPEED);
                searchState = searchState_rightIR;
            }
            else if (searchEvent == searchEvent_bothFound) {
                dbprintf("Search: Triggered both IR (%x)\n",searchState);
                Drive_Stop();
                // Transitions out
            }
            else if (searchEvent == searchEvent_bothLost) {
                dbprintf("Search: Both IR lost (%x)\n",searchState);
                searchState = searchState_fastSearch;
            }
            break;
        case searchState_rightIR:
            if (searchEvent == searchEvent_rightFound) {
                dbprintf("Search: Right IR triggered (%x)\n",searchState);
                InitTimer(TIMER_SEARCH,SEARCH_LOST_DELAY);
                Drive_Pivot(left,SLOW_SEARCH_SPEED);
                searchState = searchState_leftIR;
            }
            else if (searchEvent == searchEvent_bothFound) {
                dbprintf("Search: Triggered both IR (%x)\n",searchState);
                Drive_Stop();
                // Transitions out
            }
            else if (searchEvent == searchEvent_bothLost) {
                dbprintf("Search: Both IR lost (%x)\n",searchState);
            }
            break;
        default:
            break;
    }
}
#else
void checkSearchEvents() {
    searchEvent = searchEvent_none;

    switch(searchState) {
        case searchState_transition:
            if (IR_LeftTriggered() && IR_RightTriggered())
                searchEvent = searchEvent_bothFound;
            else if (!IR_RightTriggered())
                searchEvent = searchEvent_rightLost;
            else if (!IR_LeftTriggered())
                searchEvent = searchtLost;
        case searchState_leftIR:
            if (IR_LeftTriggered() && IR_RightTriggered())
                searchEvent = searchEvent_bothFound;
            else if (IR_LeftTriggered())
                searchEvent = searchEvent_leftFound;
            break;
        case searchState_rightIR:
            if (IR_LeftTriggered() && IR_RightTriggered())
                searchEvent = searchEvent_bothFound;
            else if (IR_LeftTriggered())
                searchEvent = searchEvent_rightFound;
            break;
        default:
            break;
    }
}

void doSearchSM() {
    checkSearchEvents();

    switch (searchState) {
        case searchState_transition:
            if (searchEvent == searchEvent_rightLost) {
                Drive_Pivot(right,SLOW_SEARCH_SPEED);
                searchState = searchState_rightIR;
            }
            else if (searchEvent == searchEvent_bothFound) {
                Drive_Stop();
                // Transitions out
            }
            else if (searchEvent == searchEvent_leftLost) {

                Drive_Pivot(left,SLOW_SEARCH_SPEED);
                searchState = searchState_leftIR;
            }
            else {
                Drive_Stop();
            }
            break;

        case searchState_leftIR:
            if (searchEvent == searchEvent_leftFound) {
                Drive_Pivot(left,SLOW_SEARCH_SPEED);
                searchState = searchState_rightIR;
            }
            else if (searchEvent == searchEvent_bothFound) {
                Drive_Stop();
                // Transitions out
            }
            else {
                Drive_Pivot(left,SLOW_SEARCH_SPEED);
            }
            break;
        case searchState_rightIR:
            if (searchEvent == searchEvent_rightFound) {
                Drive_Pivot(left,SLOW_SEARCH_SPEED);
                searchState = searchState_leftIR;
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
#endif
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
#ifndef USE_SIMPLE_SEARCH
    searchState = searchState_fastSearch;
#else
    searchState = searchState_transition;
#endif
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
            if (searchEvent == searchEvent_bothFound)
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

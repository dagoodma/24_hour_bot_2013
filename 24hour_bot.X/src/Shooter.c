
#include <xc.h>
#include <plib.h>
#include "RCServo.h"
//#include "bits.h"
#include <stdio.h>
#include "ports.h"
#include "serial.h"
#include "timers.h"
//#include "Util.h"
#include "pwm.h"
#include "Shooter.h"

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
//#define GATE_TEST
#define DEBUG_VERBOSE
#ifdef DEBUG_VERBOSE
    #define dbprintf(...) printf(__VA_ARGS__)
#else
    #define dbprintf(...)
#endif

#define SERVO_TOP       RC_PORTZ08
#define SERVO_BOTTOM    RC_PORTZ09


#define MINPULSE 1000
#define MAXPULSE 2000

// Positions for open and close
#define SERVO_TOP_CLOSE_POSITION 1400
#define SERVO_TOP_OPEN_POSITION 2000
#define SERVO_BOTTOM_CLOSE_POSITION 1000
#define SERVO_BOTTOM_OPEN_POSITION 2000

#define SPOOLUP_DELAY     4000
#define WAIT_DELAY      1500
#define LOAD_DELAY      1000
#define RELEASE_DELAY   1500

#define START_AMMO      6

#define MOTOR_ON_CYCLE  470

/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/

void openTopServo();
void closeTopServo();
void openBottomServo();
void closeBottomServo();


char ammoCount = START_AMMO;
BOOL shooting = FALSE;
enum { idle, shootWait, shootLoad, shootRelease } state;
/*******************************************************************************
 * PUBLIC FUNCTIONS                                                           *
 ******************************************************************************/

void Shooter_Init() {
    SetDutyCycle(DC_MOTOR_PWM, 0);
    RC_Init(SERVO_TOP | SERVO_BOTTOM);
    closeTopServo();
    closeBottomServo();
    state = idle;
}

void Shooter_doSM() {
    switch (state) {
        case idle:
            // Do nothing
            break;
        case shootWait:
            // We are waiting for the DC motors to spin up
            if (IsTimerExpired(TIMER_SHOOT)) {
                state = shootLoad;
                InitTimer(TIMER_SHOOT,LOAD_DELAY);
                openTopServo();
                dbprintf("\nLoading ball.\n");
            }
            break;
        case shootLoad:
            // We are loading with the top servo
            if (IsTimerExpired(TIMER_SHOOT)) {
                state = shootRelease;
                closeTopServo();
                wait();
                openBottomServo();
                InitTimer(TIMER_SHOOT,RELEASE_DELAY);
                dbprintf("Releasing ball.\n");
            }
            break;
        case shootRelease:
            // We are waiting for ball to release
            if (IsTimerExpired(TIMER_SHOOT)) {
                ammoCount -= 1;
                closeBottomServo();
                if (Shooter_hasAmmo()) {
                    state = shootWait;
                    InitTimer(TIMER_SHOOT,WAIT_DELAY);
                }
                else {
                    Shooter_stopShooting();
                }
                dbprintf("Ball shot (%d).\n", ammoCount);
            }
            break;
        default:
            break;
    }
}

void Shooter_startShooting() {
    if (Shooter_hasAmmo() && !Shooter_isShooting()) {
        state = shootWait;
        
        InitTimer(TIMER_SHOOT,SPOOLUP_DELAY);

        SetDutyCycle(DC_MOTOR_PWM, MOTOR_ON_CYCLE);
    }
}

void Shooter_stopShooting() {
    state = idle;
    closeBottomServo();
    closeTopServo();
    SetDutyCycle(DC_MOTOR_PWM, 0);

    RC_End();
}

char Shooter_hasAmmo() {
    return ammoCount > 0;
}

char Shooter_isShooting() {
    return state != idle;
}



void openTopServo() {
    RC_SetPulseTime(SERVO_TOP, SERVO_TOP_OPEN_POSITION);
}

void closeTopServo() {
    RC_SetPulseTime(SERVO_TOP, SERVO_TOP_CLOSE_POSITION);
}

void openBottomServo() {
    RC_SetPulseTime(SERVO_BOTTOM, SERVO_BOTTOM_OPEN_POSITION);
}

void closeBottomServo() {
    RC_SetPulseTime(SERVO_BOTTOM, SERVO_BOTTOM_CLOSE_POSITION);
}


//#define SHOOTER_TEST
#ifdef SHOOTER_TEST


#define FREQUENCY_PWM 250
//#define SERVO_LIMIT_TEST
int main() {
    SERIAL_Init();
    INTEnableSystemMultiVectoredInt();
    TIMERS_Init();

    PWM_Init(DC_MOTOR_PWM, PWM_1KHZ);

    Shooter_Init();
    printf("\nHello World shooter");
    wait();

#ifndef SERVO_LIMIT_TEST
    Shooter_startShooting();

    while (Shooter_hasAmmo()) {
        Shooter_doSM();
    }

#else
    int i;
    while (1) {
        for (i = 0; i<= 3; i++) {
            openTopServo();
            dbprintf("Top opened.\n");
            wait();
            closeTopServo();
            dbprintf("Top closed.\n");
            wait();
        }
        for (i = 0; i<= 3; i++) {
            openBottomServo();
            dbprintf("Bottom opened.\n");
            wait();
            closeBottomServo();
            dbprintf("Bottom closed.\n");
            wait();
        }
    }
#endif

    return TRUE;
}

#endif


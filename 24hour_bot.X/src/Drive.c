/*
 * File:   Drive.c
 * Author: dagoodma jurjohns
 *
 * Created on March 2, 2012
 */

#include <xc.h>
#include "serial.h"
#include "PORTS.h"
#include "pwm.h"
#include "Drive.h"
#include "AD.h"
#include "LED.h"
#include "Util.h"
#include "timers.h"

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
//#define DRIVE_TEST
//#define USE_BATPWM
#define DEBUG_VERBOSE
#ifdef DEBUG_VERBOSE
    #define dbprintf(...) printf(__VA_ARGS__)
#else
    #define dbprintf(...)
#endif

#define MOTOR_A_DIR         PORTY09_LAT
#define MOTOR_A_DIR_TRIS    PORTY09_TRIS
#define MOTOR_B_DIR         PORTY11_LAT
#define MOTOR_B_DIR_TRIS    PORTY11_TRIS

#define LEFT 0
#define RIGHT 1
#define BAT_MAX 713 // (1023 * Vbat) / (10 * 3.3) => Vbat = 9.8 V
#define UPDATE_DELAY 100


/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
enum motor {B, A};
enum direction {FORWARD, REVERSE};

static unsigned char motorPWM[] = {MOTOR_A_PWM, MOTOR_B_PWM};
static unsigned int motorPWMValue[] = {0, 0};
static unsigned char motorSpeed[] = { 0, 0 };

/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/
static void SetDirection(int motor, int direction);

/*************************************

******************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/
static void SetDirection(int motor, int direction) {

    //if (motor == A) {
        MOTOR_A_DIR = !direction;
    //}
    //else {
        MOTOR_B_DIR = !direction;
    //}
}

static char SetSpeed(int motor, unsigned int speed) {
    // TODO add error checking
    motorSpeed[motor] =  speed;
    //printf("\nPWM should be = %f", speed * 80.0);
 /*
    unsigned int pwm = 0;
    if (speed > 0) {
#ifdef USE_BATPWM
        int battery = (float)ReadADPin(BAT_VOLTAGE);
#else
        int battery = 0;
#endif
        float pwm_modifier = 100.0 * ((1.0 - 1.0*(battery/BAT_MAX)) * 2.15);
        printf("\nMultiplier =%f", max((int)pwm_modifier, 0));
        pwm = (unsigned int)(speed * 90.0 + max((int)pwm_modifier, 0));
        //pwm = speed;
    }
    motorPWMValue[motor] = pwm;
    //printf("\nPWM is=%i", pwm);
    pwm = min(pwm, 1000);
    pwm = max(pwm, 0);
*/
    SetDutyCycle(motorPWM[motor], speed);
}

void SetMotor(int motor, int direction, unsigned int speed) {
    SetDirection(motor, direction);
    SetSpeed(motor, speed);
}


/*******************************************************************************
 * PUBLIC FUNCTIONS                                                           *
 ******************************************************************************/

char Drive_Init(void) {
    // Set motor direction outputs
    MOTOR_A_DIR_TRIS = 0;
    MOTOR_B_DIR_TRIS = 0;

    Drive_Stop();
    InitTimer(TIMER_DRIVE,UPDATE_DELAY);

    return SUCCESS;
}

char Drive_Update(void) {
    /*
    if (IsTimerExpired(TIMER_DRIVE)) {
        SetSpeed(A, motorSpeed[A]);
        SetSpeed(B, motorSpeed[B]);
        InitTimer(TIMER_DRIVE, UPDATE_DELAY);
    }
     */
    return SUCCESS;
}

Drive_Pivot(enum turnDir dir, unsigned int speed) {
    switch (dir) {
        case right:
            SetMotor(B, FORWARD, 0);
            SetMotor(A, FORWARD, speed);
            break;
        case left:
            SetMotor(B, FORWARD, speed);
            SetMotor(A, FORWARD, 0);
            break;
        default:
            break;
    }
}


Drive_Arc(enum turnDir dir, unsigned int speed) {
    switch (dir) {
        case right:
            SetMotor(A, FORWARD, speed);
            SetMotor(B, REVERSE, speed);
            break;
        case left:
            SetMotor(A, REVERSE, speed);
            SetMotor(B, FORWARD, 0);
            break;
        default:
            break;
    }
}

char Drive_Turn(enum turnType type, enum turnDir dir, unsigned int speed) {
    static char previousTurn;
    switch(type){
        case pivot:
            switch(dir){
                case right:
                    SetMotor(A, REVERSE, speed);
                    SetMotor(B, FORWARD, speed);
                    previousTurn = RIGHT;
                    break;
                case left:
                    SetMotor(A, FORWARD, speed);
                    SetMotor(B, REVERSE, speed);
                    previousTurn = LEFT;
                    break;
                case same:
                    if(previousTurn == LEFT)
                        Drive_Turn(pivot, left, speed);
                    else if(previousTurn == RIGHT)
                        Drive_Turn(pivot, right, speed);
                    break;
            case opposite:	
                    if(previousTurn == LEFT)
                        Drive_Turn(pivot, right, speed);
                    else if(previousTurn == RIGHT)
                        Drive_Turn(pivot, left, speed);
                    break;
            }
            break;
        case rightAng:			//leaving for now, should prob. be a timed pivot and only take L/R
            switch(dir){
                case right:
                    previousTurn = RIGHT;
                    break;
                case left:
                    previousTurn = LEFT;
                    break;
                case same:
                    if(previousTurn == LEFT)
                        Drive_Turn(rightAng, left, speed);
                    else if(previousTurn == RIGHT)
                        Drive_Turn(rightAng, right, speed);
                    break;
                case opposite:
                    if(previousTurn == LEFT)
                        Drive_Turn(rightAng, right, speed);
                    else if(previousTurn == RIGHT)
                        Drive_Turn(rightAng, left, speed);
                    break;
            }
            break;
        case soft:	
            switch(dir){
                case right:
                    SetMotor(A, FORWARD, speed/2);
                    SetMotor(B, FORWARD, speed);

                    previousTurn = RIGHT;
                    break;
                case left:
                    SetMotor(A, FORWARD, speed);
                    SetMotor(B, FORWARD, speed/2);

                    previousTurn = LEFT;
                    break;
                case same:
                    if(previousTurn == LEFT)
                        Drive_Turn(soft, left, speed);
                    else if(previousTurn == RIGHT)
                        Drive_Turn(soft, right, speed);
                    break;
                case opposite:
                      if(previousTurn == LEFT)
                        Drive_Turn(soft, right, speed);
                      else if(previousTurn == RIGHT)
                        Drive_Turn(soft, left, speed);
                    break;
            }
            break;
        case hard:
            switch(dir){
                case right:
                    SetMotor(A, FORWARD, speed);
                    SetMotor(B, FORWARD, speed/3);
                    previousTurn = RIGHT;
                    break;
                case left:
                    SetMotor(A, FORWARD, speed/3);
                    SetMotor(B, FORWARD, speed);
                    previousTurn = LEFT;
                    break;
                case same:
                     if(previousTurn == LEFT)
                        Drive_Turn(hard, left, speed);
                    else if(previousTurn == RIGHT)
                        Drive_Turn(hard, right, speed);
                     break;
                case opposite:
                     if(previousTurn == LEFT)
                        Drive_Turn(hard, right, speed);
                      else if(previousTurn == RIGHT)
                        Drive_Turn(hard, left, speed);
                    break;
            }
            break;
        default:
            break;
    }
    return SUCCESS;
}

char Drive_Forward(unsigned int speed){
    SetMotor(A, FORWARD, speed);
    SetMotor(B, FORWARD, speed);
    return SUCCESS;
}

char Drive_Reverse(unsigned int speed){
    SetMotor(A, REVERSE, speed);
    SetMotor(B, REVERSE, speed);

    return SUCCESS;
}

char Drive_Stop(void) {
    SetMotor(A, FORWARD, 0);
    SetMotor(B, FORWARD, 0);

    return SUCCESS;
}

/*******************************************************************************
 * TEST HARNESS                                                                *
 ******************************************************************************/
//#define DRIVE_TEST
#ifdef DRIVE_TEST

#define DELAY() for(i=0;i < NOPCOUNT; i++) __asm("nop")
#define NOPCOUNT 990000

char main(){
    int i;
    SERIAL_Init();
    INTEnableSystemMultiVectoredInt();
    Drive_Init();
    dbprintf("\nHello World");
    AD_Init(BAT_VOLTAGE);
    LED_Init(LED_BANK3);
    LED_OffBank(LED_BANK3, 0xf);
    wait();
    int pwm;

    while(1) {
        //Drive_Stop();
        wait();
        //Drive_Forward(MIN_SPEED);
        printf("\nFORWARD! speed=%u pwm=%u", motorSpeed[A], motorPWMValue[A]);
        //for (i = 0; i < 100; i++)
        //    wait();
        Drive_Stop();
        Drive_Pivot(left, 1);
        for (i = 0; i < 1; i++)
            wait();
        Drive_Stop();
        Drive_Pivot(right, 1);
        for (i = 0; i < 1; i++)
            wait();
        Drive_Stop();
        for (i = 0; i < 1; i++)
            wait();
    }
}

#endif

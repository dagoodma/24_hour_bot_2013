/*
 * File:   IR.c
 * Author: dagoodma, hahernan, jurjohns
 *
 * Infrared beacon sensor module. Switches between the main and angled
 * beacon sensors to aquire readings from both.
 *
 */

#include <xc.h>
#include <stdio.h>
#include "serial.h"
#include "timers.h"
#include "PORTS.h"
#include "AD.H"
#include "IR.h"
#include "LED.h"


#define USE_LEDS
//#define USE_SAMPLING
//#define IR_TEST

#define DEBUG_VERBOSE
#ifdef DEBUG_VERBOSE
#define dbprintf(...) printf(__VA_ARGS__)
#else
#define dbprintf(...)
#endif
#define TIMER_IR 4
#define UPDATE_DELAY 1


#define IR_LEFT AD_PORTW6
#define IR_RIGHT AD_PORTW7

/*
#define LEFT_HIGH_THRESHOLD     835
#define LEFT_LOW_THRESHOLD      635
#define RIGHT_HIGH_THRESHOLD    815
#define RIGHT_LOW_THRESHOLD     715
 */
#define LEFT_HIGH_THRESHOLD     795
#define LEFT_LOW_THRESHOLD      635
#define RIGHT_HIGH_THRESHOLD    778
#define RIGHT_LOW_THRESHOLD     715
 

#define MAXCOUNT 1


/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
static unsigned char timesCounted = 0;

enum { IR_LEFT_I, IR_RIGHT_I };

static unsigned int irState[] = { 0, 0 };
static unsigned int irThreshold[] = { LEFT_HIGH_THRESHOLD, RIGHT_HIGH_THRESHOLD };
static unsigned char irCounter[] = { 0, 0};

/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/
char IsLeftTriggered(void);
char IsRightTriggered(void);
void UpdateCounters();

/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/
/**
 * @Function: UpdateCounters
 * @remark Updates the IR sensor counts by comparing them with hysteretic thresholds.
 * @date 2012.3.5 12:47 */
void UpdateCounters() {
    char result = IsLeftTriggered();
    irCounter[IR_LEFT_I] += result;
    // Check angle sensor
    result = IsRightTriggered();
    irCounter[IR_RIGHT_I]+= result;

    //printf("\nMain counter=%d, angle=%d",irCounter[IR_MAIN_I], irCounter[IR_ANGLE_I]);

    timesCounted++;

    if (timesCounted >= MAXCOUNT) {
        result = irCounter[IR_LEFT_I] >= MAXCOUNT;
        irState[IR_LEFT_I] = result;

        if (result)
            irThreshold[IR_LEFT_I] = LEFT_LOW_THRESHOLD;
        else
            irThreshold[IR_LEFT_I] = LEFT_HIGH_THRESHOLD;

        result = irCounter[IR_RIGHT_I] >= MAXCOUNT;
        irState[IR_RIGHT_I] = result;

        if (result)
            irThreshold[IR_RIGHT_I] = RIGHT_LOW_THRESHOLD;
        else
            irThreshold[IR_RIGHT_I] = RIGHT_HIGH_THRESHOLD;

        // Clear counters
        timesCounted = 0;
        irCounter[IR_LEFT_I] = 0;
        irCounter[IR_RIGHT_I] = 0;
    }
}

/**
 * @Function: IsRightTriggered
 * @return TRUE or FALSE 
 * @remark When angled beacon is high.
 * @date 2012.3.5 12:47 */
char IsRightTriggered() {
    unsigned int val = ReadADPin(IR_RIGHT);
    char result = OFF;
    //dbprintf("\nAngle=%d", val);
    if (val > irThreshold[IR_RIGHT_I]) {
#ifndef USE_SAMPLING
        irThreshold[IR_RIGHT_I] = RIGHT_LOW_THRESHOLD;
#endif
        result = ON;
    }
    if (val < irThreshold[IR_RIGHT_I]) {
        #ifndef USE_SAMPLING
        irThreshold[IR_RIGHT_I] = RIGHT_HIGH_THRESHOLD;
        #endif
        result = OFF;
    }

    #ifdef USE_LEDS
    if (result)
        LED_OnBank(LED_BANK1, 0x8);
    else
        LED_OffBank(LED_BANK1, 0x8);
    #endif
    return result;
}


char IsLeftTriggered() {
    unsigned int val = ReadADPin(IR_LEFT);
    char result = OFF;
    //dbprintf("\nMain=%d", val);
    if (val > irThreshold[IR_LEFT_I]) {
        #ifndef USE_SAMPLING
        irThreshold[IR_LEFT_I] = LEFT_LOW_THRESHOLD;
#endif
        result =  ON;
    }
    if (val < irThreshold[IR_LEFT_I]) {
        #ifndef USE_SAMPLING
        irThreshold[IR_LEFT_I] = LEFT_HIGH_THRESHOLD;
        #endif
        result = OFF;
    }

    #ifdef USE_LEDS
    if (result)
        LED_OnBank(LED_BANK3, 0x8);
    else
        LED_OffBank(LED_BANK3, 0x8);
    #endif

    return result;
}

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                           *
 ******************************************************************************/
char IR_Init() {
    //dbprintf("\nInitializing the IR Sensor Module.");

    #ifdef USE_LEDS
    LED_Init(LED_BANK3 |LED_BANK1);
    LED_OffBank(LED_BANK3, 0xF);
    LED_OffBank(LED_BANK1, 0xF);

    #endif

    InitTimer(TIMER_IR, UPDATE_DELAY);

    return SUCCESS;
}

char IR_Update() {
    // Check main sensor
#ifdef USE_SAMPLING
    if (IsTimerExpired(TIMER_IR)) {
        /*
        char result = IsMainTriggered();
        #ifdef USE_LEDS
        if (result)
            LED_OnBank(LED_BANK3, 0x1);
        else
            LED_OffBank(LED_BANK3, 0x1);
        #endif
        irState[IR_MAIN_I] = result;

        // Check angle sensor
        result = IsAngleTriggered();
        #ifdef USE_LEDS
        if (result)
            LED_OnBank(LED_BANK3, 0x8);
        else
            LED_OffBank(LED_BANK3, 0x8);
        #endif
         irState[IR_ANGLE_I] = result;
         */
        UpdateCounters();
        InitTimer(TIMER_IR, UPDATE_DELAY);
     }
#endif

     return TRUE;
}

char IR_LeftTriggered() {
#ifdef USE_SAMPLING
    IR_Update();
    return irState[IR_MAIN_I];
#else
    return IsLeftTriggered();
#endif
}

char IR_RightTriggered() {
    #ifdef USE_SAMPLING
    IR_Update();
    return irState[IR_ANGLE_I];
#else 
    return IsRightTriggered();
    #endif
}

unsigned int IR_LeftReading() {
    return ReadADPin(IR_LEFT);
}

unsigned int IR_RightReading() {
    return ReadADPin(IR_RIGHT);
}

char IR_End() {
    AD_End();
    return SUCCESS;
}


/*******************************************************************************
 * TEST HARNESS                                                                *
 ******************************************************************************/
#ifdef IR_TEST
#ifndef DEBUG_VERBOSE
#define DEBUG_VERBOSE
#endif

#define NOPCOUNT 990000
#define DELAY() for(i=0;i < NOPCOUNT; i++) __asm("nop")

int main(void) {

    SERIAL_Init();
    TIMERS_Init();
    char i, j = 0;
    int k, l = 0;
    int time = GetTime();
    INTEnableSystemMultiVectoredInt();
    AD_Init(IR_PINS);
    IR_Init();

    while (1) {
        k = ReadADPin(IR_LEFT);
        l = ReadADPin(IR_RIGHT);
		char leftTrig = '_';
		char rightTrig = '_';
		if (IR_LeftTriggered())
			leftTrig = 'x';
		if (IR_RightTriggered())
			rightTrig = 'x';
			
        //if (time > GetTime() + 500) {
            if (IsTransmitEmpty()) {
                printf("\n %cLeft : %d \n %cRight : %d",leftTrig, k, rightTrig, l);
            }
                wait();
            //time = GetTime();
        //}
        //while (!IsTransmitEmpty()); // bad, this is blocking code
    }
    return 0;
}
#endif

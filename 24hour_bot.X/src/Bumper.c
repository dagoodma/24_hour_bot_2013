/*
 * File:   TapeSensor.c
 * Author: dagoodma
 *
 * Taking 3 samples, at 
 *
 */

#include <xc.h>
#include "serial.h"
#include "timers.h"
#include "PORTS.h"
#include "LED.h"

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
//#define USE_LEDS
//#define BUMP_TEST

//#define DEBUG_VERBOSE
#ifdef DEBUG_VERBOSE
    #define dbprintf(...) printf(__VA_ARGS__)
#else
    #define dbprintf(...)
#endif

#define TIMER_BUMPER 2
#define UPDATE_DELAY 50 // ms

#define BUMPERCOUNT 4

#define BUMPER_FRONTRIGHT_BIT       PORTZ04_BIT // _RE4
#define BUMPER_FRONTRIGHT_TRIS      PORTZ04_TRIS
#define BUMPER_FRONTLEFT_BIT        PORTZ03_BIT // _RE4
#define BUMPER_FRONTLEFT_TRIS       PORTZ03_TRIS
#define BUMPER_BACKRIGHT_BIT        PORTZ05_BIT
#define BUMPER_BACKRIGHT_TRIS       PORTZ05_TRIS
#define BUMPER_BACKLEFT_BIT         PORTZ06_BIT
#define BUMPER_BACKLEFT_TRIS        PORTZ06_TRIS


#define MAXCOUNT 4
#define SHIFTCOUNT 1


/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/


enum bumperIndex {BUMPER_FRONTRIGHT_I, BUMPER_FRONTLEFT_I, BUMPER_BACKRIGHT_I,
    BUMPER_BACKLEFT_I };
static unsigned char bumperCounter[] = {0, 0, 0, 0};
static unsigned int bumperPort[] = {0, 0, 0, 0};
static char bumperState[] = { 0, 0, 0, 0};
static unsigned int timesCounted = 0;


/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/

static void UpdateBumperCounters();
static char ReadBumpers();
static void DebugLED(unsigned int i);

/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/

static void UpdateBumperCounters() {
    int i;
    ReadBumpers();
    timesCounted++;
    // Iterate over bumpers and adjust saturating counters

    for (i = 0; i < BUMPERCOUNT; i++) {
        bumperCounter[i] += bumperPort[i];

        if (timesCounted >= MAXCOUNT) {
            bumperCounter[i] >>= SHIFTCOUNT;
            bumperState[i] = bumperCounter[i] >= 1;
            DebugLED(i);
            bumperCounter[i] = 0;
        }
    }

    if (timesCounted >= MAXCOUNT)
        timesCounted = 0;
}

static void DebugLED(unsigned int i) {
    #ifdef USE_LEDS

        switch (i) {
            case BUMPER_FRONTLEFT_I:
                if (bumperState[i])
                    LED_OnBank(LED_BANK1, 0x8);
                else
                    LED_OffBank(LED_BANK1, 0x8);
                break;
            case BUMPER_FRONTRIGHT_I:
                if (bumperState[i])
                    LED_OnBank(LED_BANK1, 0x4);
                else
                    LED_OffBank(LED_BANK1, 0x4);
                break;
            case BUMPER_BACKLEFT_I:
                if (bumperState[i])
                    LED_OnBank(LED_BANK1, 0x2);
                else
                    LED_OffBank(LED_BANK1, 0x2);
                break;
            case BUMPER_BACKRIGHT_I:
                if (bumperState[i])
                    LED_OnBank(LED_BANK1, 0x1);
                else
                    LED_OffBank(LED_BANK1, 0x1);
                break;
        } // switch
   #endif
}

static char ReadBumpers() {
    bumperPort[BUMPER_FRONTRIGHT_I] = !BUMPER_FRONTRIGHT_BIT;
    bumperPort[BUMPER_FRONTLEFT_I] = !BUMPER_FRONTLEFT_BIT;
    bumperPort[BUMPER_BACKRIGHT_I] = !BUMPER_BACKRIGHT_BIT;
    bumperPort[BUMPER_BACKLEFT_I] = !BUMPER_BACKLEFT_BIT;
    return SUCCESS;
}

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                           *
 ******************************************************************************/
char Bumper_Init() {
    //dbprintf("\nInitializing the Bumper Sensor Module.");

    InitTimer(TIMER_BUMPER, UPDATE_DELAY);

    // Define inputs
    BUMPER_FRONTRIGHT_TRIS = 1;
    BUMPER_FRONTLEFT_TRIS = 1;
    BUMPER_BACKRIGHT_TRIS = 1;
    BUMPER_BACKLEFT_TRIS = 1;

#ifdef USE_LEDS
    LED_Init(LED_BANK1);
    LED_OffBank(LED_BANK1, 0xF);

#endif


    //dbprintf("\nBumper sensors initialized (%d)", BUMPERCOUNT);

    return SUCCESS;

}

char Bumper_Update() {
    if (IsTimerExpired(TIMER_BUMPER)) {
        UpdateBumperCounters();
        InitTimer(TIMER_BUMPER, UPDATE_DELAY);
    }
    return SUCCESS;
}

char Bumper_FrontLeftTriggered() {
    return bumperState[BUMPER_FRONTLEFT_I];
}

char Bumper_FrontRightTriggered() {
    return bumperState[BUMPER_FRONTRIGHT_I];
}

char Bumper_BackRightTriggered() {
    return bumperState[BUMPER_BACKRIGHT_I];
}

char Bumper_BackLeftTriggered() {
    return bumperState[BUMPER_BACKLEFT_I];
}

char Bumper_AnyTriggered() {
    return Bumper_FrontLeftTriggered()
        || Bumper_FrontRightTriggered()
        || Bumper_BackRightTriggered()
        || Bumper_BackLeftTriggered();
}

char Bumper_AnyFrontTriggered() {
    return Bumper_FrontLeftTriggered()
        || Bumper_FrontRightTriggered();
}

char Bumper_AnyBackTriggered() {
    return Bumper_BackRightTriggered()
        || Bumper_BackLeftTriggered();
}

char Bumper_End() {
    StopTimer(TIMER_BUMPER);
    return SUCCESS;
}

/*******************************************************************************
 * TEST HARNESS                                                                *
 ******************************************************************************/
#ifdef BUMP_TEST
#ifndef DEBUG_VERBOSE
#define DEBUG_VERBOSE
#endif

#define NOPCOUNT 990000
#define DELAY() for(i=0;i < NOPCOUNT; i++) __asm("nop")

int main(void) {
    SERIAL_Init();
    TIMERS_Init();
    int i = 0;

    INTEnableSystemMultiVectoredInt();

    Bumper_Init();

    // Test routine (above goes in init)
    printf("\nHello tester!");
    DELAY();

    while(1) {
        Bumper_Update();

        printf("\nStates: Front-left=(%x), Front-right=(%x), Back-left=(%x), Back-right=(%x)",
            Bumper_FrontLeftTriggered(), Bumper_FrontRightTriggered(),
            Bumper_BackRightTriggered(), Bumper_BackLeftTriggered());
               
            while (!IsTransmitEmpty()); // bad, this is blocking code
    } // end of loop

    Bumper_End();
    return 0;
} // test harness
#endif


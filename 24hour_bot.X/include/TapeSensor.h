/*
 * File:   TapeSensor.h
 * Author: dagoodma, hahernan, jurjohns
 *
 * Public interface for the TapeSensor module. Tape sensors ports are
 * initialized externally by the caller using AD_Init.
 *
 * Created on February 25, 2012, 7:50 PM
 */

#ifndef TapeSensor_H
#define TapeSensor_H

#include "PORTS.h"

/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/

#ifndef SUCCESS
#define SUCCESS 0
#define ERROR -1
#endif

#ifndef TRUE
#define FALSE 0
#define TRUE 1
#endif

enum sensorIndex { TAPE_FRONTLEFT_I, TAPE_FRONTRIGHT_I, TAPE_BACKRIGHT_I,
    TAPE_BACKLEFT_I };
//--------------- Photodetectors --------------

#define TAPE_FRONTLEFT  AD_PORTV5
#define TAPE_FRONTRIGHT AD_PORTV6
#define TAPE_BACKLEFT   AD_PORTV7
#define TAPE_BACKRIGHT  AD_PORTV8

/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/


/**
 * @Function: Tape_Init
 * @return SUCCESS or ERROR
 * @remark Initializes the tape sensors.
 * @date 2012.2.28 08:34 */
char Tape_Init();


/**
 * @Function: Tape_HandleSM
 * @return SUCCESS or ERROR
 * @remark Handles the tape sensor's state machine. This should be
 *         called before handling any other state machines.
 * @date 2012.2.27 05:42 */
char Tape_HandleSM();


/**
 * @Function: Tape_End
 * @return SUCCESS or ERROR
 * @remark Used to end use of the tape sensors.
 * @date 2012.2.29 12:59 */
char Tape_End();


// ********************* Tape Sensor Accessors *************************

char Tape_FrontLeftTriggered();

char Tape_FrontRightTriggered();

char Tape_BackRightTriggered();

char Tape_BackLeftTriggered();

char Tape_AnyTriggered();

char Tape_AnyBackTriggered();

char Tape_AnyFrontTriggered();


void Tape_SetOnTapeThreshold(unsigned int index);
void Tape_SetOffTapeThreshold(unsigned int index);

#endif

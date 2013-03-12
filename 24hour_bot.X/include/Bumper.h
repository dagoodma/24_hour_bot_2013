/*
 * File:   Bumper.h
 * Author: dagoodma, hahernan, jurjohns
 *
 * Public interface for the Bumper module. Once initialzed, the
 * Bumper_Update
 * 
 *
 * Created on February 25, 2012, 7:50 PM
 */

#ifndef Bumper_H
#define Bumper_H

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


/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/

/**
 * Function: Bumper_Init
 * @return SUCCESS or ERROR
 * @remark Initializes the bumper sensor state machine.
 */
char Bumper_Init();

/**
 * Function: Bumper_Update
 * @return SUCCESS or ERROR
 * @remark Checks the bumpers for a hit. This must be called at least
 *         three times before a hit will be detected.
 */
char Bumper_Update();

char Bumper_FrontLeftTriggered();

char Bumper_FrontRightTriggered();

char Bumper_BackRightTriggered();

char Bumper_BackLeftTriggered();

char Bumper_AnyTriggered();

char Bumper_AnyFrontTriggered();

char Bumper_AnyBackTriggered();

/**
 * Function: Bumper_End
 * @return SUCCESS or ERROR
 * @remark Stops the bumper module.
 */
char Bumper_End();

#endif
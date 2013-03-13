/*
 * File:   Drive.h
 * Author: dagoodma, hahernan, jurjohns
 *
 * Public interface for the Drive module.
 *
 * Created on February 25, 2012, 7:50 PM
 */

#ifndef Drive_H
#define Drive_H

#include "ports.h"
#include "pwm.h"

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

#define MOTOR_B_PWM         PWM_PORTY12
#define MOTOR_A_PWM         PWM_PORTY10

#define HALF_SPEED      370
#define FULL_SPEED      1000
#define LOW_SPEED       300
#define MIN_SPEED       325

enum turnType{pivot, rightAng, soft, hard};
enum turnDir{right, left, same, opposite};


/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/

/**
 * Function: Drive_Init()
 * @return SUCCESS or ERROR
 * @remark Initializes the two motors for driving
 */
char Drive_Init(void);

/**
 * @Function: Drive_Update()
 * @return SUCCESS or ERROR
 * @remark Updates the battery and adjusts the speed accordingly
 * @date  */
char Drive_Update(void);

/**
 * @Function: Drive_Forward()
 * @param speed of driving forward 0(slowest) 10(fastest)
 * @return SUCCESS or ERROR
 * @remark Drives the motors straight and forward
 * @date  */
char Drive_Forward(unsigned int speed);

/**
 * @Function: Drive_Reverse()
 * @param speed of driving forward 0(slowest) 10(fastest)
 * @return TRUE or FALSE value
 * @remark Drives the motors straight and reverse
 * @date */
char Drive_Reverse(unsigned int speed);

/**
 * @Function: Drive_Stop()
 * @return TRUE or FALSE value
 * @remark Stops the motors
 * @date */
char Drive_Stop(void);



void Drive_Pivot(enum turnDir dir, unsigned int speed);
#endif

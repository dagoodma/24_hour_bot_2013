/* 
 * File:   Shooter.h
 * Author: David
 *
 * Created on March 12, 2013, 5:47 PM
 */

#ifndef SHOOTER_H
#define	SHOOTER_H
#include "pwm.h"

#define DC_MOTOR_PWM    PWM_PORTY04

void Shooter_Init();

void Shooter_doSM();

void Shooter_startShooting();

void Shooter_stopShooting();

char Shooter_hasAmmo();


#endif	/* SHOOTER_H */


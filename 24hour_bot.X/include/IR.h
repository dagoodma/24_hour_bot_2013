/*
 * File:   IR.h
 * Author: dagoodma
 *
 * barebones
 *
 */
 
#ifndef IR_H
#define IR_H

#define ON 1
#define OFF 0

#define IR_PINS AD_PORTW6 | AD_PORTW7

char IR_Update();
char IR_Init(void);
char IR_AngleTriggered(void);
char IR_MainTriggered(void);
char IR_End(void);

unsigned int IR_MainReading();
unsigned int IR_AngleReading();

#endif
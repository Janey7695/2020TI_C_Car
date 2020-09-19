/*
 * MotorCTL.h
 *
 *  Created on: 2020Äê9ÔÂ19ÈÕ
 *      Author: Lenovo
 */

#ifndef MOTORCTL_H_
#define MOTORCTL_H_

#define int8 volatile unsigned char
#define int16 volatile unsigned int

void Left_Ahead();
void Right_Ahead();
void Left_Stop();
void Right_Stop();
void Left_Back();
void Right_Back();
void Speed_1test();
void Speed_2test();
void Speed_3test();
void Left_Ahead_Speed(int8 Perstop,int8 Perahead,int8 time);
void Right_Ahead_Speed(int8 Perstop,int8 Perahead,int8 time);

#endif /* MOTORCTL_H_ */

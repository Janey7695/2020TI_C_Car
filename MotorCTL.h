/*
 * MotorCTL.h
 *
 *  Created on: 2020��9��19��
 *      Author: Lenovo
 */

#ifndef MOTORCTL_H_
#define MOTORCTL_H_

#include <config.h>

void Go_ahead(unsigned char Duty);
void Turn(unsigned char lefP,unsigned char rightP,unsigned char Duty,unsigned int Ms);
void Stop();
#endif /* MOTORCTL_H_ */

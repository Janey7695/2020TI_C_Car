/*
 * sys.h
 *
 *  Created on: 2020��9��19��
 *      Author: Lenovo
 */

#ifndef SYS_H_
#define SYS_H_

#include <msp430.h>

void Delay_us(int us);
void Delay_ms(int ms);
void Clock_Init();
void Gpio_Init();
void Uart_Init();
void printf(unsigned char* str);

#endif /* SYS_H_ */

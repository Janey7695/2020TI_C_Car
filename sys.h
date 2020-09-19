/*
 * sys.h
 *
 *  Created on: 2020Äê9ÔÂ19ÈÕ
 *      Author: Lenovo
 */

#ifndef SYS_H_
#define SYS_H_

#include <msp430.h>

void Delay_us(int us);
void Delay_ms(int ms);
void Clock_Init();
void Gpio_Init();

#endif /* SYS_H_ */

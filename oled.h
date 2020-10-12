/*
 * oled.h
 *
 *  Created on: 2020Äê10ÔÂ11ÈÕ
 *      Author: Lenovo
 */

#ifndef OLED_H_
#define OLED_H_

#include <msp430.h>

#define uchar volatile unsigned char
#define uint volatile unsigned int

#define OLED_CMD 0
#define OLED_DATA 1

#define SCL_L() P8OUT&=~BIT6
#define SCL_H() P8OUT|=BIT6

#define SDIN_L() P8OUT&=~BIT7
#define SDIN_H() P8OUT|=BIT7

#define RST_NE() P9OUT&=~BIT0
#define RST_E() P9OUT|=BIT0

#define DC_NE() P9OUT&=~BIT1
#define DC_E() P9OUT|=BIT1

#define CS_NE() P9OUT&=~BIT5
#define CS_E() P9OUT|=BIT5

void Delay_us(int us);
void Delay_ms(int ms);

void OLED_WR_Byte(uchar dat,uchar mode);
void OLED_Display_On();
void OLED_Display_Off();
void OLED_Init();
void OLED_Clear();
void OLED_Set_Pos(uchar x,uchar y);
void OLED_ShowChar(uchar x,uchar y,uchar CHAR,uchar fontsize);
void OLED_ShowStr(uchar x,uchar y,uchar*chr,uchar fontsize);
void OLED_DrawBMP(uchar x0,uchar y0,uchar x1,uchar y1,uchar BMP[]);

#endif /* OLED_H_ */

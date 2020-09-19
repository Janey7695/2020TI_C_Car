/*
 * sys.c
 *
 *  Created on: 2020年9月19日
 *      Author: Lenovo
 */
#include <sys.h>

/*
 * 硬件精确延时
 */
#define CPU_F ((double)8000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))


void Delay_ms(int ms)
{
    while(ms--)
    {
        delay_ms(1);
    }

}

void Delay_us(int us)
{
    while(us--)
    {
        delay_us(1);
    }
}

/*
 * 系统的时钟初始化
 * MCLK -> 8MHz
 *
 */
void Clock_Init()
{
    CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
    CSCTL1 = DCOFSEL_6;                       // Set DCO to 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;  // Set SMCLK = MCLK = DCO
                                              // ACLK = VLOCLK
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers to 1
    CSCTL0_H = 0;                             // Lock CS registers
}

/*
 * 初始化GPIO P8.4-P8.7 设置为输出模式
 * P8.4 = 1 并且 P8.5 = 0 -> 右轮正转
 * P8.6 = 1 并且 P8.7 = 0 -> 左轮正转
 */
void Gpio_Init()
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5; //退出LPM5模式
    P1DIR|=BIT0;
    P8DIR|=BIT6+BIT7+BIT4+BIT5;
}




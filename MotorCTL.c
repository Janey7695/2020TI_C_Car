/*
 * MotorCTL.c
 *
 *  Created on: 2020年9月19日
 *      Author: Lenovo
 */

#include <msp430.h>
#include <MotorCTL.h>
#include <sys.h>


void Turn(unsigned char lefP,unsigned char rightP,unsigned int Ms)
{
    TA1CCR0 = 50-1;                          // PWM Period
    TA1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TA1CCR1 = rightP;                             // CCR1 PWM duty cycle
    TA1CCTL2 = OUTMOD_7;                      // CCR2 reset/set
    TA1CCR2 = lefP;                             // CCR2 PWM duty cycle
    TA1CTL = TASSEL__ACLK | MC__UP | TACLR;   // ACLK, up mode, clear TAR   Delay_us(150);
    Delay_ms(Ms);
    Speed_3test();
}

//不同前进速度
void Speed_1test()
{
    TA1CCR0 = 50;                          // PWM Period
            TA1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
            TA1CCR1 = 25;                             // CCR1 PWM duty cycle
            TA1CCTL2 = OUTMOD_7;                      // CCR2 reset/set
            TA1CCR2 = 25;                             // CCR2 PWM duty cycle
            TA1CTL = TASSEL__ACLK | MC__UP | TACLR;   // ACLK, up mode, clear TAR   Delay_us(150);
            P3OUT|=BIT6;
}
void Speed_2test()
{
    TA1CCR0 = 50-1;                          // PWM Period
        TA1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
        TA1CCR1 = 12;                             // CCR1 PWM duty cycle
        TA1CCTL2 = OUTMOD_7;                      // CCR2 reset/set
        TA1CCR2 = 45;                             // CCR2 PWM duty cycle
        TA1CTL = TASSEL__ACLK | MC__UP | TACLR;   // ACLK, up mode, clear TAR   Delay_us(150);

}
void Speed_3test()
{
    P3OUT&=~BIT6;
    TA1CCR0 = 50-1;                          // PWM Period
    TA1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TA1CCR1 = 12;                             // CCR1 PWM duty cycle
    TA1CCTL2 = OUTMOD_7;                      // CCR2 reset/set
    TA1CCR2 = 12;                             // CCR2 PWM duty cycle
    TA1CTL = TASSEL__ACLK | MC__UP | TACLR;   // ACLK, up mode, clear TAR
}

void Stop()
{
    TA1CCR0 = 50-1;                          // PWM Period
    TA1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TA1CCR1 = 0;                             // CCR1 PWM duty cycle
    TA1CCTL2 = OUTMOD_7;                      // CCR2 reset/set
    TA1CCR2 = 0;                             // CCR2 PWM duty cycle
    TA1CTL = TASSEL__ACLK | MC__UP | TACLR;   // ACLK, up mode, clear TAR
    //TA1CTL = TASSEL__ACLK | MC__STOP | TACLR;   // ACLK, up mode, clear TAR
    P3OUT&=~BIT3;
    P1OUT&=~BIT3;
    P2OUT|=BIT6;
    Delay_ms(90);
    P2OUT&=~BIT6;
    while(1);
}





/*
 * MotorCTL.c
 *
 *  Created on: 2020年9月19日
 *      Author: Lenovo
 */

#include <msp430.h>
#include <MotorCTL.h>
#include <sys.h>

/*
 * 控制小车转弯
 * 参数：unsigned char lefP 左轮的占空比（取值0-49->对应0%-100%）
 *     unsigned char rightP 右轮的占空比（取值0-49->对应0%-100%）
 *     unsigned char Duty 转弯完之后小车继续前行时，前进的占空比（取值0-49->对应0%-100%）
 *     unsigned int Ms 过弯时间（单位ms）
 */

void Turn(unsigned char lefP,unsigned char rightP,unsigned char Duty,unsigned int Ms)
{
    if(Duty>50)
    {
        Duty=49;
    }
    TA1CCR0 = 50-1;                          // PWM Period
    TA1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TA1CCR1 = rightP;                             // CCR1 PWM duty cycle
    TA1CCTL2 = OUTMOD_7;                      // CCR2 reset/set
    TA1CCR2 = lefP;                             // CCR2 PWM duty cycle
    TA1CTL = TASSEL__ACLK | MC__UP | TACLR;   // ACLK, up mode, clear TAR   Delay_us(150);
    Delay_ms(Ms);
    Go_ahead(Duty);
}
/*
 * 小车直行，通过调节占空比来调制前行速度
 * 参数：unsigned char Duty 占空比（取值0-49->对应0%-100%）
 */

void Go_ahead(unsigned char Duty)
{
    P3OUT&=~BIT6;
    TA1CCR0 = 50-1;                          // PWM Period
    TA1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TA1CCR1 = Duty;                             // CCR1 PWM duty cycle
    TA1CCTL2 = OUTMOD_7;                      // CCR2 reset/set
    TA1CCR2 = Duty;                             // CCR2 PWM duty cycle
    TA1CTL = TASSEL__ACLK | MC__UP | TACLR;   // ACLK, up mode, clear TAR
}

//停车函数
void Stop()
{
    //关闭总中断，防止计时器，计数器继续进行，浪费芯片资源
    _DINT();
    TA1CCR0 = 50-1;                          // PWM Period
    TA1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TA1CCR1 = 0;                             // CCR1 PWM duty cycle
    TA1CCTL2 = OUTMOD_7;                      // CCR2 reset/set
    TA1CCR2 = 0;                             // CCR2 PWM duty cycle
    TA1CTL = TASSEL__ACLK | MC__UP | TACLR;   // ACLK, up mode, clear TAR
    //TA1CTL = TASSEL__ACLK | MC__STOP | TACLR;   // ACLK, up mode, clear TAR
    P3OUT&=~BIT3;
    P1OUT&=~BIT3;

    //蜂鸣器鸣90ms
    P9OUT|=BIT4;

    Delay_ms(90);
    P9OUT&=~BIT4;
    //



    while(1);
}





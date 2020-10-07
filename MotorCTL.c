/*
 * MotorCTL.c
 *
 *  Created on: 2020年9月19日
 *      Author: Lenovo
 */

#include <msp430.h>
#include <MotorCTL.h>
#include <sys.h>
// 左边正转
void Left_Ahead()
{
    P8OUT&=~BIT7;
    P8OUT|=BIT6;
}
//右边正转
void Right_Ahead()
{
    P8OUT&=~BIT5;
    P8OUT|=BIT4;
}
//左边停止
void Left_Stop()
{
    P8OUT&=0x3f;
}
//右边停止
void Right_Stop()
{
    P8OUT&=0xcf;
}
//左边后退
void Left_Back()
{
    P8OUT&=~BIT6;
    P8OUT|=BIT7;
}
//右边后退
void Right_Back()
{
    P8OUT&=~BIT4;
    P8OUT|=BIT5;
}

//不同前进速度
void Speed_1test()
{
    int8 i;
    for(i=0;i<200;i++)
    {

        Left_Stop();
        Delay_us(100);
        Left_Ahead();
        Delay_us(100);
    }
}
void Speed_2test()
{
    int8 i;
    for(i=0;i<200;i++)
    {

        Left_Stop();
        Delay_us(50);
        Left_Ahead();
        Delay_us(150);
    }
}
void Speed_3test()
{
    TA1CCR0 = 50;                          // PWM Period
    TA1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TA1CCR1 = 40;                             // CCR1 PWM duty cycle
    TA1CCTL2 = OUTMOD_7;                      // CCR2 reset/set
    TA1CCR2 = 40;                             // CCR2 PWM duty cycle
    TA1CTL = TASSEL__ACLK | MC__UP | TACLR;   // ACLK, up mode, clear TAR
}
/*
 * 通过设置占空比模拟PWM输出控制电机转速
 * 参数：Perstop :取值0-200
 *     Perahead : 取值0-200
 *     Perstop+Perahead 需要等于200
 *     time : 每份time约为50ms
 */
void Left_Ahead_Speed(int8 Perstop,int8 Perahead,int8 time)
{
        int8 i;
        while(time--)
        for(i=0;i<200;i++)
        {

            Left_Stop();
            Delay_us(Perstop);
            Left_Ahead();
            Delay_us(Perahead);
        }
}
void Right_Ahead_Speed(int8 Perstop,int8 Perahead,int8 time)
{
        int8 i;
        while(time--)
        for(i=0;i<200;i++)
        {

            Right_Stop();
            Delay_us(Perstop);
            Right_Ahead();
            Delay_us(Perahead);
        }
}


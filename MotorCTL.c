/*
 * MotorCTL.c
 *
 *  Created on: 2020��9��19��
 *      Author: Lenovo
 */

#include <msp430.h>
#include <MotorCTL.h>
#include <sys.h>
// �����ת
void Left_Ahead()
{
    P8OUT&=~BIT7;
    P8OUT|=BIT6;
}
//�ұ���ת
void Right_Ahead()
{
    P8OUT&=~BIT5;
    P8OUT|=BIT4;
}
//���ֹͣ
void Left_Stop()
{
    P8OUT&=0x3f;
}
//�ұ�ֹͣ
void Right_Stop()
{
    P8OUT&=0xcf;
}
//��ߺ���
void Left_Back()
{
    P8OUT&=~BIT6;
    P8OUT|=BIT7;
}
//�ұߺ���
void Right_Back()
{
    P8OUT&=~BIT4;
    P8OUT|=BIT5;
}

//��ͬǰ���ٶ�
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
 * ͨ������ռ�ձ�ģ��PWM������Ƶ��ת��
 * ������Perstop :ȡֵ0-200
 *     Perahead : ȡֵ0-200
 *     Perstop+Perahead ��Ҫ����200
 *     time : ÿ��timeԼΪ50ms
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


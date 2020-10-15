/*
 * MotorCTL.c
 *
 *  Created on: 2020��9��19��
 *      Author: Lenovo
 */

#include <msp430.h>
#include <MotorCTL.h>
#include <sys.h>

/*
 * ����С��ת��
 * ������unsigned char lefP ���ֵ�ռ�ձȣ�ȡֵ0-49->��Ӧ0%-100%��
 *     unsigned char rightP ���ֵ�ռ�ձȣ�ȡֵ0-49->��Ӧ0%-100%��
 *     unsigned char Duty ת����֮��С������ǰ��ʱ��ǰ����ռ�ձȣ�ȡֵ0-49->��Ӧ0%-100%��
 *     unsigned int Ms ����ʱ�䣨��λms��
 */

void Turn(unsigned char lefP,unsigned char rightP,unsigned char Duty,unsigned int Ms)
{
    if(Duty>100)
    {
        Duty=99;
    }
    TA1CCR0 = 100-1;                          // PWM Period
    TA1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TA1CCR1 = rightP;                             // CCR1 PWM duty cycle
    TA1CCTL2 = OUTMOD_7;                      // CCR2 reset/set
    TA1CCR2 = lefP;                             // CCR2 PWM duty cycle
    TA1CTL = TASSEL__ACLK | MC__UP | TACLR;   // ACLK, up mode, clear TAR   Delay_us(150);
    Delay_ms(Ms);
    Go_ahead(90);
    Delay_ms(5);
    Go_ahead(Duty);
}
/*
 * С��ֱ�У�ͨ������ռ�ձ�������ǰ���ٶ�
 * ������unsigned char Duty ռ�ձȣ�ȡֵ0-49->��Ӧ0%-100%��
 */

void Go_ahead(unsigned char Duty)
{
    if(Duty>100)
       {
           Duty=99;
       }
    P3OUT&=~BIT6;
    TA1CCR0 = 100-1;                          // PWM Period
    TA1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TA1CCR1 = Duty;                             // CCR1 PWM duty cycle
    TA1CCTL2 = OUTMOD_7;                      // CCR2 reset/set
    TA1CCR2 = Duty;                             // CCR2 PWM duty cycle
    TA1CTL = TASSEL__ACLK | MC__UP | TACLR;   // ACLK, up mode, clear TAR
}

//ͣ������
void Stop()
{
    //�ر����жϣ���ֹ��ʱ�����������������У��˷�оƬ��Դ
    _DINT();
    TA1CCR0 = 100-1;                          // PWM Period
    TA1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TA1CCR1 = 0;                             // CCR1 PWM duty cycle
    TA1CCTL2 = OUTMOD_7;                      // CCR2 reset/set
    TA1CCR2 = 0;                             // CCR2 PWM duty cycle
    TA1CTL = TASSEL__ACLK | MC__UP | TACLR;   // ACLK, up mode, clear TAR
    //TA1CTL = TASSEL__ACLK | MC__STOP | TACLR;   // ACLK, up mode, clear TAR
    P3OUT&=~BIT3;
    P1OUT&=~BIT3;

    //��������90ms
    P9OUT|=BIT4;

    Delay_ms(90);
    P9OUT&=~BIT4;
    //



    while(1);
}





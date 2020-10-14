#include <msp430.h> 
#include <MotorCTL.h>
#include <sys.h>
#include <oled.h>
#include <PID.h>
/**
 * main.c
 */

int8 rightflag=0; //�Һ���2��Ӧ��־
int8 Bigrightflag=0; //�Һ���3��Ӧ��־
int8 teth=0; //������һ��ʱ������ת����1/5Ȧ�ĸ���
int8 teth1; //����teth��ֵ����ֹteth������һ������ʱ���жϴ�ϣ�����cpuȡֵ����
int8 num[6]; //�����洢��ǰʱ������ת����1/5Ȧ�ĸ���ת���ɵ��ַ���
int8 num2[3]; //�����洢��ǰ��Dutyռ�ձ�
int8 cesuFlag=0; //���ٱ�־
int8 Duty=14; //ռ�ձ� ��ʼֵĬ����14
int8 ChoiceTime=10; //ѡ��ʻ��1m��ʱ�䣨����ֵ10-21��
int8 Vset=2;//���ݼ���ó��� ֻ����һ��ʱ������ת����1/5Ȧ�ĸ�������Vset��ֵ��С����ʱ���ٶȲŽӽ�Ԥ��ֵ

/*
 * ���ٺ���
 *
 */
void AdjustV()
{

    if(teth1<Vset)
    {
        Duty+=4*PID(Vset,teth1,0.5);
        if(Duty>50)
            Duty=49;
    }
    if(teth1>Vset)
    {
        Duty-=PID(Vset,teth1,0.5);
        if(Duty==0)
            Duty=0;
    }
}

/*
 * Change num to Char
 * ��int������ת��Ϊchar�ͣ�������oled����ʾ
 */

void Cn2C()
{
    num[0]='-';
    num[1]='>';
    num[2]='r';
    num[3]='=';
    num[4]=teth1+48;
    num2[0]=Duty/10+48;
    num2[1]=Duty%10+48;
}

int main(void)
{
    int8 Menu=0;
    Gpio_Init();//I/O�ڳ�ʼ��
    Clock_Init();//ʱ�ӳ�ʼ��
    Uart_Init();//����ͨ�ų�ʼ��
    OLED_Init();//OLED��ʼ��
    OLED_Clear();//oled����

    /*
     * ʱ��ѡ��
     * UP/DOWM:�������ϵ��µ�
     * OK:ȷ�ϼ�
     *
     */
    while(!Menu)
    {
        if(ChoiceTime!=21)
        {
            OLED_ShowStr(0,4,"Time:",16);
            OLED_ShowChar(40,4,ChoiceTime/10+48,16);
            OLED_ShowChar(48,4,ChoiceTime%10+48,16);
            OLED_ShowChar(56,4,' ',16);
            OLED_ShowChar(64,4,' ',16);
        }
        if(ChoiceTime==21)
        {
            OLED_ShowStr(0,4,"Time:Fast!",16);
        }

        if(!(P4IN&BIT7))
        {
            Delay_ms(100);
            if(!(P4IN&BIT7))
            {
                Menu=1;
            }
        }
        if(!(P2IN&BIT4))
        {
            Delay_ms(100);
            if(!(P2IN&BIT4))
            {
                ChoiceTime+=1;
                if(ChoiceTime>21)
                    ChoiceTime=10;
            }
        }
        if(!(P2IN&BIT5))
        {
            Delay_ms(100);
            if(!(P2IN&BIT5))
            {
                ChoiceTime-=1;
                if(ChoiceTime<10)
                    ChoiceTime=21;
            }
        }
        OLED_ShowStr(0,3,"Hit OK to continue!",8);
        OLED_ShowStr(0,0,"Hit UP/DOWM to Choice Time",8);
        while((!(P8IN&BIT4))&&(!(P8IN&BIT5))&&(!(P1IN&BIT4))&&(!(P1IN&BIT7)))
        {
            OLED_ShowStr(0,4,"Stop way3!",16);
            Stop();
        }
    }

    //������TimerA0��ʼ��
    switch(ChoiceTime)
    {
    case 10:
        TimerA0_Init(VsetTime10);
        break;
    case 11:
            TimerA0_Init(VsetTime11);
            break;
    case 12:
            TimerA0_Init(VsetTime12);
            break;
    case 13:
            TimerA0_Init(VsetTime13);
            break;
    case 14:
            TimerA0_Init(VsetTime14);
            break;
    case 15:
            TimerA0_Init(VsetTime15);
            break;
    case 16:
            TimerA0_Init(VsetTime16);
            break;
    case 17:
            TimerA0_Init(VsetTime17);
            break;
    case 18:
            TimerA0_Init(VsetTime18);
            break;
    case 19:
            TimerA0_Init(VsetTime19);
            break;
    case 20:
            TimerA0_Init(VsetTime20);
            break;
    case 21:
    {
        Vset=10;
        Duty=35;
        TimerA0_Init(VsetTimeFast);
        break;
    }

    }
    OLED_Clear();
    Go_ahead(Duty);//ǰ��
    P1OUT|=BIT0;//led ��

    /*
     * ��ʼ����ѭ��ѭ��
     */
    while(1)
    {
        while((!(P8IN&BIT4))&&(!(P8IN&BIT5))&&(!(P1IN&BIT4))&&(!(P1IN&BIT7)))
        {
            OLED_ShowStr(0,4,"Stop way3!",16);
            Stop();
        }
        //�����һ������һ����ͬʱ��⵽�ڿ飬��ͣ��
        while((!(P8IN&BIT4))&&(!(P8IN&BIT5)))
        {
            OLED_ShowStr(0,4,"Stop way1!",16);
            Stop();
        }
        //�����2���ⴥ���ڿ飬С��ת��־��rightflag������1����ʼС��ת
        while(rightflag)
        {
            OLED_ShowStr(0,2,"Turn Brigh!",16);
            P3OUT|=BIT6;
            Turn(Duty+18,30,Duty,210);
            rightflag--;
        }
        //�����3���ⴥ���ڿ飬����ת��־��Bigrightflag������1����ʼ����ת
        while(Bigrightflag)
        {
            OLED_ShowStr(0,2,"Turn BBigh!",16);
            P3OUT|=BIT6;
            Turn(Duty+25,30,Duty,350);
            Bigrightflag--;

        }
        //�����1�����⵽�ڿ飬����ֱ���ϵ�С�ͽ�����ת
        while(!(P8IN&BIT4))
        {
            //�������1�ռ�⵽�ڿ鲻�ã���1Ҳ��⵽�ˣ�����Ϊ������ֹͣ��־
            if(!(P8IN&BIT5))
            {
                OLED_ShowStr(0,4,"Stop way2!",16);
                Stop();
            }
            else
            {
                OLED_ShowStr(0,2,"Turn right!",16);
                Turn(40,3,Duty,50);
            }
        }
        //�����1�����⵽�ڿ飬����ֱ���ϵ�С�ͽ�����ת
        while(!(P8IN&BIT5))
        {
            //�������1�ռ�⵽�ڿ鲻�ã���1Ҳ��⵽�ˣ�����Ϊ������ֹͣ��־
            if(!(P8IN&BIT4))
            {
                OLED_ShowStr(0,4,"Stop way2!",16);
                Stop();
            }
            else
            {
                OLED_ShowStr(0,2,"Turn left! ",16);
                Turn(3,40,Duty,50);
            }
        }
        OLED_ShowStr(0,2,"Go  ahead! ",16);

        //������ٱ�־���ж���1����������ٺ�����
        if(cesuFlag==1)
        {
            OLED_ShowStr(0,2,"Adjust !   ",16);
            teth1=teth; //����teth��ֵ
            AdjustV(); //����
            Go_ahead(Duty);//ǰ��
            Cn2C();
            OLED_ShowStr(0,0,"Duty:",16);
            OLED_ShowStr(40,0,num2,16);
            OLED_ShowStr(0,6,num,16);
            teth=0;
            cesuFlag=0;//������ٱ�־
        }

    }

}
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer0_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    cesuFlag=1;
}

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
  switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
//      while(!(UCA0IFG&UCTXIFG));
//      flag=UCA0RXBUF;
      break;
    case USCI_UART_UCTXIFG:
        break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
  }
}



#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
    switch(__even_in_range(P1IV, P1IV_P1IFG7))
    {
        case P1IV_NONE : break;
        case P1IV_P1IFG0 : break;
        case P1IV_P1IFG1 : break;
        case P1IV_P1IFG2 : break;
        case P1IV_P1IFG3 : break;
        case P1IV_P1IFG4 :
        {
            Bigrightflag+=1;
            P1IFG &= ~BIT4;
            break;
        }

        case P1IV_P1IFG5 :
        {
            teth+=1;
            P1IFG &= ~BIT5;
            break;
        }
        case P1IV_P1IFG6 : break;
        case P1IV_P1IFG7 :
            {
                rightflag+=1;
                P1IFG &= ~BIT7;                           // Clear P1.1 IFG
                break;
            }
    }
}



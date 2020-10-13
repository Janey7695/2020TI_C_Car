#include <msp430.h> 
#include <MotorCTL.h>
#include <sys.h>
#include <oled.h>
#include <PID.h>
/**
 * main.c
 */

int8 rightflag=0;
int8 Bigrightflag=0;
int8 teth=0;
int8 teth1;
int8 num[6];
int8 num2[3];
int8 cesuFlag=0;
int8 Duty=17;
int8 ChoiceTime=10;
void AdjustV()
{

    if(teth1<Vset)
    {
        Duty+=Pway(Vset,teth1);
        if(Duty>50)
            Duty=49;
    }
    if(teth1>Vset)
    {
        Duty-=Pway(Vset,teth1);
        if(Duty==0)
            Duty=0;
    }
}

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
    int8 Okk=0;
    Gpio_Init();//I/O口初始化
    Clock_Init();//时钟初始化
    Uart_Init();//串口通信初始化
    OLED_Init();//OLED初始化
    OLED_Clear();
    while(!Okk)
    {
        OLED_ShowStr(0,2,"Time:",16);
        OLED_ShowChar(40,2,ChoiceTime/10+48,16);
        OLED_ShowChar(48,2,ChoiceTime%10+48,16);
        if(!(P4IN&BIT7))
        {
            Delay_ms(50);
            if(!(P4IN&BIT7))
            {
                Okk=1;
            }
        }
        if(!(P2IN&BIT4))
        {
            Delay_ms(50);
            if(!(P2IN&BIT4))
            {
                ChoiceTime+=1;
                if(ChoiceTime>20)
                    ChoiceTime=20;
            }
        }
        if(!(P2IN&BIT5))
        {
            Delay_ms(50);
            if(!(P2IN&BIT5))
            {
                ChoiceTime-=1;
                if(ChoiceTime<10)
                    ChoiceTime=10;
            }
        }
        OLED_ShowStr(0,0,"Hit OK to Con",16);
    }

    //计数器TimerA0初始化
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
    }
    OLED_Clear();
//    OLED_ShowStr(0,2,"Init Finish",16);
//    OLED_Clear();
    Go_ahead(Duty);//前行
    while(1)
    {
        while((!(P8IN&BIT4))&&(!(P8IN&BIT5)))
        {
            OLED_ShowStr(0,4,"Stop way1!",16);
            Stop();
        }
        while(rightflag)
        {
            OLED_ShowStr(0,2,"Turn Brigh!",16);
            P3OUT|=BIT6;
            Turn(Duty+18,30,Duty,210);
            rightflag--;
        }
        while(Bigrightflag)
        {
            OLED_ShowStr(0,2,"Turn BBigh!",16);
            P3OUT|=BIT6;
            Turn(Duty+25,30,Duty,350);
            Bigrightflag--;

        }
        while(!(P8IN&BIT4))//右红外检测到嘿块 小车右转1.3s
        {
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
        while(!(P8IN&BIT5))//左红外检测到嘿块 小车左转100ms
        {
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
        //printf("go straight!",1);
        P1OUT|=BIT0;//led 亮
        if(cesuFlag==1)
        {
            OLED_ShowStr(0,2,"Adjust !   ",16);
            teth1=teth;
            AdjustV();
            Go_ahead(Duty);//前行
            Cn2C();
            OLED_ShowStr(0,0,"Duty:",16);
            OLED_ShowStr(40,0,num2,16);
            OLED_ShowStr(0,6,num,16);
            teth=0;
            cesuFlag=0;
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
            P1IFG &= ~BIT7;
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
                P1IFG &= ~BIT1;                           // Clear P1.1 IFG
                break;
            }
    }
}



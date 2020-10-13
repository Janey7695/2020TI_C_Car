#include <msp430.h> 
#include <MotorCTL.h>
#include <sys.h>
#include <oled.h>
/**
 * main.c
 */

int8 rightflag=0;
int8 Bigrightflag=0;
int8 teth=0;
int16 tethtimelen;
int8 num[6];

//void ChangeNum2Char(int16 Num)
//{
//
//    num[0]=Num/10000+'0';
//    num[1]=Num%10000/1000+'0';
//    num[2]=Num%10000%1000/100+'0';
//    num[3]=Num%10000%1000%100/10+'0';
//    num[4]=Num%10000%1000%100%10+'0';
//}
int main(void)
{
    Gpio_Init();//I/O口初始化
    Clock_Init();//时钟初始化
    Uart_Init();//串口通信初始化
    OLED_Init();
    //TimerA0_Init();
    //printf("Initialization Finish!",1);
    OLED_Clear();
    OLED_ShowStr(0,0,"Init Finish",16);
    OLED_ShowChar(0,6,teth+'0',16);
    Go_ahead();//前行
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
            // printf("turn right!",1);
            //Turn(15,35,200);
            P3OUT|=BIT6;
            Turn(44,30,167);
            rightflag--;
        }
        while(Bigrightflag)
        {
            OLED_ShowStr(0,2,"Turn BBigh!",16);
            // printf("turn right!",1);
            //Turn(20,40,200);
            P3OUT|=BIT6;
            Turn(40,30,300);
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
                // printf("turn right!",1);
                Turn(30,3,50);
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
                //  printf("turn left!",1);
                Turn(3,30,50);
            }
        }
        OLED_ShowStr(0,2,"Go  ahead! ",16);
        //printf("go straight!",1);
        P1OUT|=BIT0;//led 亮

    }

}
//#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
//#pragma vector = TIMER0_A0_VECTOR
//__interrupt void Timer0_A0_ISR (void)
//#elif defined(__GNUC__)
//void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer0_A0_ISR (void)
//#else
//#error Compiler not supported!
//#endif
//{
//  P1OUT ^= BIT0;
////  TA0CCR0 += 50000;                         // Add Offset to TA0CCR0
//}

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
//
//#pragma vector = PORT2_VECTOR
//__interrupt void PORT2_ISR(void)
//{
//    switch(__even_in_range(P2IV, P2IV_P2IFG7))
//    {
//        case P2IV_NONE : break;
//        case P2IV_P2IFG0 : break;
//        case P2IV_P2IFG1 : break;
//        case P2IV_P2IFG2 : break;
//
//        //UP
//        case P2IV_P2IFG3 :
//        {
//            if(!CHO)
//            {
//                MODE+=1;
//                if(MODE==3)
//                    MODE=1;
//            }
//            P2IFG &= ~BIT3;
//            break;
//        }
//
//        //DOWN
//        case P2IV_P2IFG4 :
//        {
//            if(CHO!=0)
//            {
//                MODE-=1;
//                if(MODE==0)
//                    MODE=2;
//
//            }
//            P2IFG &= ~BIT4;
//            break;
//        }
//
//        // OK
//        case P2IV_P2IFG5 :
//        {
//            if(CHO!=0)
//            {
//                OK=1;
//                CHO=1;
//            }
//            P1OUT|=BIT0;
//            P2IFG &= ~BIT5;
//            break;
//        }
//        case P2IV_P2IFG6 : break;
//        case P2IV_P2IFG7 : break;
//    }
//}

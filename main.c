#include <msp430.h> 
#include <MotorCTL.h>
#include <sys.h>
/**
 * main.c
 */

//int8 flag;
//int16 time[2]; //计数器计算的从发射波到接收波的时间值
//int8 next;
//int16 S; //超声波与障碍物之间的距离
//
//unsigned char DataFlash[5];//字符缓存
//
///*
// * 结束计时后计算距离
// * TODO:有问题 需要处理 S永远都是1.78
// */
//void HCSR04_Count()
//{
//
//    S=(time[1]-time[0])/58;
//
//        DataFlash[0]=S%1000/100+'0';
//        DataFlash[1]='.';
//        DataFlash[2]=S%1000%100/10+'0';
//        DataFlash[3]=S%1000%100%10+'0';
//        DataFlash[4]='M';
//        printf("Distance is :",0);
//        printf(DataFlash,1);
//        TA1CCR1=0;
//        TA1CCTL1|=CCIE;
//}
//
///*
// * 启动HCSR04模块 发射8个40Khz的方波
// */
//void StartHCSR04()
//{
//    printf("begin to count",1);
//    P2OUT|=BIT7;
//    Delay_us(15);
//    P2OUT&=~BIT7;
//    while(next<2);
//    printf("over count",1);
//    TA1CCTL1&=~CCIE;
//    HCSR04_Count();
//}



int main(void)
{


    Gpio_Init();//I/O口初始化
    Clock_Init();//时钟初始化
//    TimerA_Init();//计时器初始化
    Uart_Init();//串口通信初始化
    printf("Initialization Finish!",1);
    Speed_3test();
	while(1){
//	    StartHCSR04();//发出波
	    P1OUT|=BIT0;//小灯
        printf("go ahead now",1);
        Delay_ms(1000);//延时1s
        P1OUT&=~BIT0;//小灯灭
        Delay_ms(1000);//延时1s
	}

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

//#pragma vector = TIMER1_A1_VECTOR
//__interrupt void Timer1_A1_ISR (void)
//{
//    time[next++]=TA1CCR1;
//    TA1CCTL1&=~CCIFG;
//}


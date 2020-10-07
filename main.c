#include <msp430.h> 
#include <MotorCTL.h>
#include <sys.h>
/**
 * main.c
 */

//int8 flag;
//int16 time[2]; //����������Ĵӷ��䲨�����ղ���ʱ��ֵ
//int8 next;
//int16 S; //���������ϰ���֮��ľ���
//
//unsigned char DataFlash[5];//�ַ�����
//
///*
// * ������ʱ��������
// * TODO:������ ��Ҫ���� S��Զ����1.78
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
// * ����HCSR04ģ�� ����8��40Khz�ķ���
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


    Gpio_Init();//I/O�ڳ�ʼ��
    Clock_Init();//ʱ�ӳ�ʼ��
//    TimerA_Init();//��ʱ����ʼ��
    Uart_Init();//����ͨ�ų�ʼ��
    printf("Initialization Finish!",1);
    Speed_3test();
	while(1){
//	    StartHCSR04();//������
	    P1OUT|=BIT0;//С��
        printf("go ahead now",1);
        Delay_ms(1000);//��ʱ1s
        P1OUT&=~BIT0;//С����
        Delay_ms(1000);//��ʱ1s
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


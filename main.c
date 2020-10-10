#include <msp430.h> 
#include <MotorCTL.h>
#include <sys.h>
/**
 * main.c
 */




int main(void)
{
    Gpio_Init();//I/O�ڳ�ʼ��
    Clock_Init();//ʱ�ӳ�ʼ��
    Uart_Init();//����ͨ�ų�ʼ��
    printf("Initialization Finish!",1);
    Speed_3test();//ǰ��
  	while(1){
  	    while(!(P8IN&BIT4))//�Һ����⵽�ٿ� С����ת100ms
  	    {
  	        printf("turn right!",1);
  	        Turn(45,15,100);
  	    }
  	    while(!(P8IN&BIT5))//������⵽�ٿ� С����ת100ms
  	    {
  	      printf("turn left!",1);
  	        Turn(15,45,100);
  	    }
  	    printf("go straight!",1);
	    P1OUT|=BIT0;//led ��
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



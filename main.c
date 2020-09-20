#include <msp430.h> 
#include <MotorCTL.h>
#include <sys.h>
/**
 * main.c
 */

int8 flag;

int main(void)
{
    int8 j;
    Clock_Init();
    Gpio_Init();
    Uart_Init();
    printf("Initialization Finish!");
	while(1){
	    while(flag=='1')
	    {
	        P1OUT|=BIT0;
            Left_Ahead();
            printf("go ahead now");
            Delay_ms(1000);
            P1OUT&=~BIT0;
            printf("slowdown now");
            Left_Ahead_Speed(50,150,20);
	    }


	}


}
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
      while(!(UCA0IFG&UCTXIFG));
      flag=UCA0RXBUF;
      break;
    case USCI_UART_UCTXIFG:
        break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
  }
}


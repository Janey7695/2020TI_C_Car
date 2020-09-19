#include <msp430.h> 
#include <MotorCTL.h>
#include <sys.h>
/**
 * main.c
 */

/*
 * 计时器初始化
 */
//void Timer_Init()
//{
//    TA0CCTL0 = CCIE;
//    TA0CCR0 = 50000;
//    TA0CTL = TASSEL_1 + MC_1 + TACLR;
//    _EINT();
//}



int main(void)
{
    int8 j;
    Clock_Init();
    Gpio_Init();
//    Timer_Init();
	while(1){
	    P1OUT|=BIT0;
	    Left_Ahead();
	    Delay_ms(1000);
	    P1OUT&=~BIT0;
	    Left_Ahead_Speed(50,150,20);
//	    Delay_ms(1000);
	}


}

//#pragma vector = TIMER0_A0_VECTOR
//
//__interrupt void TIMER0_Intr(void)
//{
//    if(flag%2==0)
//       Left_Ahead();
//    else
//        Left_Back();
//    flag+=1;
//}

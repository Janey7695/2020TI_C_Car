#include <msp430.h> 
#include <MotorCTL.h>
#include <sys.h>
/**
 * main.c
 */

int8 flag;
int16 time; //计数器计算的从发射波到接收波的时间值
int16 S; //超声波与障碍物之间的距离
unsigned char DataFlash[5];//字符缓存
/*
 * 启动HCSR04模块 发射8个40Khz的方波
 */
void StartHCSR04()
{
    P2OUT|=BIT7;
    Delay_us(15);
    P2OUT&=~BIT7;
}

/*
 * 结束计时后计算距离
 * TODO:有问题 需要处理 S永远都是9.36
 */
void HCSR04_Count()
{
    time=TA0R;
    TA0R=0;
    S=time*2/10;
    if(flag==1)
    {
        flag=0;
        printf("-.--M");
    }
    else
    {
        flag=0;
        DataFlash[0]=S%1000/100+'0';
        DataFlash[1]='.';
        DataFlash[2]=S%1000%100/10+'0';
        DataFlash[3]=S%1000%100%10+'0';
        DataFlash[4]='M';
        printf(DataFlash);
    }
}

int main(void)
{

    Clock_Init();//时钟初始化
    Gpio_Init();//I/O口初始化
    Uart_Init();//串口通信初始化
    TimerA_Init();//计时器初始化
    printf("Initialization Finish!");
	while(1){
	    StartHCSR04();//发出波
	    while(!(P2IN&BIT6));
	    TA0CTL = TASSEL__SMCLK | MC__CONTINOUS|ID__8;//开始计时
	    printf("begin to count");
	    while(P2IN&BIT6);
	    TA0CTL = TASSEL__SMCLK | MC__STOP|ID__8;//结束计时
	    printf("over count");
	    HCSR04_Count();//计算距离
	        P1OUT|=BIT0;//小灯亮
            Left_Ahead();//左轮前转
            printf("go ahead now");
            Delay_ms(1000);//延时1s
            P1OUT&=~BIT0;//小灯灭
            printf("slowdown now");
            Left_Ahead_Speed(50,150,20);//左轮减速 持续大约1s
	}


}

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
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

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
{
    flag=1;
}


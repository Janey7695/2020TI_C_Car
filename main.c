#include <msp430.h> 
#include <MotorCTL.h>
#include <sys.h>
/**
 * main.c
 */

int8 flag;
int16 time; //����������Ĵӷ��䲨�����ղ���ʱ��ֵ
int16 S; //���������ϰ���֮��ľ���
unsigned char DataFlash[5];//�ַ�����
/*
 * ����HCSR04ģ�� ����8��40Khz�ķ���
 */
void StartHCSR04()
{
    P2OUT|=BIT7;
    Delay_us(15);
    P2OUT&=~BIT7;
}

/*
 * ������ʱ��������
 * TODO:������ ��Ҫ���� S��Զ����9.36
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

    Clock_Init();//ʱ�ӳ�ʼ��
    Gpio_Init();//I/O�ڳ�ʼ��
    Uart_Init();//����ͨ�ų�ʼ��
    TimerA_Init();//��ʱ����ʼ��
    printf("Initialization Finish!");
	while(1){
	    StartHCSR04();//������
	    while(!(P2IN&BIT6));
	    TA0CTL = TASSEL__SMCLK | MC__CONTINOUS|ID__8;//��ʼ��ʱ
	    printf("begin to count");
	    while(P2IN&BIT6);
	    TA0CTL = TASSEL__SMCLK | MC__STOP|ID__8;//������ʱ
	    printf("over count");
	    HCSR04_Count();//�������
	        P1OUT|=BIT0;//С����
            Left_Ahead();//����ǰת
            printf("go ahead now");
            Delay_ms(1000);//��ʱ1s
            P1OUT&=~BIT0;//С����
            printf("slowdown now");
            Left_Ahead_Speed(50,150,20);//���ּ��� ������Լ1s
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


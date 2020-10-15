/*
 * sys.c
 *
 *  Created on: 2020��9��19��
 *      Author: Lenovo
 */
#include <sys.h>

/*
 * Ӳ����ȷ��ʱ
 */
#define CPU_F ((double)8000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))


void Delay_ms(int ms)
{
    while(ms--)
    {
        delay_ms(1);
    }

}

void Delay_us(int us)
{
    while(us--)
    {
        delay_us(1);
    }
}

/*
 * ϵͳ��ʱ�ӳ�ʼ��
 * MCLK -> 8MHz
 *
 */
void Clock_Init()
{
//    CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
//    CSCTL1 = DCOFSEL_3 | DCORSEL;             // Set DCO to 8MHz
//    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;  // Set SMCLK = MCLK = DCO
//                                              // ACLK = VLOCLK
//    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers to 1
//    CSCTL0_H = 0;                             // Lock CS registers


    CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
    CSCTL1 = DCOFSEL_6;                       // Set DCO to 8MHz
    CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK; // Set ACLK = XT1; MCLK = DCO
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
    CSCTL4 &= ~LFXTOFF;
    do
    {
      CSCTL5 &= ~LFXTOFFG;                    // Clear XT1 fault flag
      SFRIFG1 &= ~OFIFG;
    }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
    CSCTL0_H = 0;                             // Lock CS registers
}

/*
 *
 * ʹ��PWM���������Ƴ���
 *
 * P1.3 ->���ֵ�PWMOUT
 * P3.3 ->���ֵ�PWMOUT
 *���32khz�ľ���
 */
void Gpio_Init()
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P2SEL0 |= BIT0 | BIT1;                    // ����P2.0��ΪTxD P2.1ΪRxD
    P2SEL1 &= ~(BIT0 | BIT1);

    //P1.3 ->���� P3.3 ->����
    P1DIR |=  BIT3;                     // P1.3  output ��
    P1SEL0 |=  BIT3;                    //  P1.3 options select
    P3DIR |=  BIT3;                     //  P3.3 output ��
    P3SEL0 &=~BIT3;                    // P3.3 options select
    P3SEL1 |= BIT3;                    // P3.3 options select

    P3DIR |=BIT0|BIT6;  //������

    PJSEL0 |= BIT4 | BIT5; // ��Ӿ���
//    P3OUT&=~BIT0;
//    P3OUT&=~BIT6;

    PM5CTL0 &= ~LOCKLPM5; //�˳�LPM5ģʽ

    P1DIR|=BIT0; //led��
    P8DIR&=~BIT4; //����ƽ�����
    P8DIR&=~BIT5; //����ƽ�����
    P8DIR|=BIT6+BIT7; //P8.6 ->D0 SCL P8.7 ->D1 SDIN
    P9DIR|=BIT0+BIT1+BIT5; //P9.0 -> rst P9.1 ->DC P9.5 ->CS


    P9DIR|=BIT4; //������
    P9OUT&=~BIT4;

    P1DIR&=~BIT4;   //��������Ҷ�
    P1IES |= BIT4;                             // P1.1 Hi/Lo edge
    P1IFG = 0;                                // Clear all P1 interrupt flags
    P1IE |= BIT4;                              // P1.1 interrupt enabled
    P1DIR&=~BIT7; //�����������
    P1IES |= BIT7;                             // P1.1 Hi/Lo edge
    P1IFG = 0;                                // Clear all P1 interrupt flags
    P1IE |= BIT7;                              // P1.1 interrupt enabled
    P1DIR&=~BIT5; //�������
    P1IES |= BIT5;                             // P1.1 Hi/Lo edge
    P1IFG = 0;                                // Clear all P1 interrupt flags
    P1IE |= BIT5;                              // P1.1 interrupt enabled

    P2DIR&=~BIT5; // UP
    P2OUT|=BIT5;                             // Pull-up resistor on P1.1
    P2REN|= BIT5;                             // Select pull-up mode for P1.1

    P2DIR&=~BIT4; // DOWN
    P2OUT|=BIT4;                             // Pull-up resistor on P1.1
    P2REN|= BIT4;

    P4DIR&=~BIT7; // OK
    P4OUT|=BIT7;                             // Pull-up resistor on P1.1
    P4REN|= BIT7;

      _EINT(); //�ж�������

}

void TimerA0_Init(int16 VsetTime)
{
    TA0CCTL0 = CCIE;                          // TACCR0 interrupt enabled
    TA0CCR0=VsetTime;
    TA0CTL = TASSEL__ACLK | MC__UP;   // SMCLK, continuous mode
}
/*
 * ���ڳ�ʼ��
 * ���ò�����Ϊ9600
 *
 */
void Uart_Init()
{
    // Configure USCI_A0 for UART mode
    UCA0CTLW0 = UCSWRST;                      // Put eUSCI in reset
    UCA0CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK
    // Baud Rate calculation
    // 8000000/(16*9600) = 52.083
    // Fractional portion = 0.083
    // User's Guide Table 21-4: UCBRSx = 0x04
    // UCBRFx = int ( (52.083-52)*16) = 1
    UCA0BR0 = 52;                             // 8000000/16/9600
    UCA0BR1 = 0x00;
    UCA0MCTLW |= UCOS16 | UCBRF_1 | 0x4900;
    UCA0CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
    UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

    __bis_SR_register(GIE);       //  interrupts enabled
}


/*
 * ���ڴ�ӡ����
 *
 * �򴮿ڷ�������
 * ��ent=1,���ӡ�س� ����
 */
void printf(unsigned char str[],unsigned char ent)
{
    unsigned char i=0;
    while((str[i])!='\0')
    {
        while(!(UCA0IFG&UCTXIFG));
        UCA0TXBUF=str[i];
        i++;
    }
    if(ent==1)
    {
        while(!(UCA0IFG&UCTXIFG));
        UCA0TXBUF=13;
        while(!(UCA0IFG&UCTXIFG));
        UCA0TXBUF=10;
    }

}





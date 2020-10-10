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
    P1DIR |=  BIT3;                     // P1.3  output
    P1SEL0 |=  BIT3;                    //  P1.3 options select
    P3DIR |=  BIT3;                     //  P3.3 output
    P3SEL0 &=~BIT3;                    // P3.3 options select
    P3SEL1 |= BIT3;                    // P3.3 options select
    PJSEL0 |= BIT4 | BIT5;
    P3DIR |=BIT0|BIT6;
    P3OUT&=~BIT0;
    P3OUT&=~BIT6;
    PM5CTL0 &= ~LOCKLPM5; //�˳�LPM5ģʽ
    P1DIR|=BIT0; //led��
    P8DIR&=~BIT4; //����ƽ�����
    P8DIR&=~BIT5; //����ƽ�����
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




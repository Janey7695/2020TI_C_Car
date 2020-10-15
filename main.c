#include <msp430.h> 
#include <MotorCTL.h>
#include <sys.h>
#include <oled.h>
#include <PID.h>
#include <oledbmp.h>
/**
 * main.c
 */

int8 rightflag=0; //右红外2感应标志
int8 Bigrightflag=0; //右红外3感应标志
int8 teth=0; //码盘在一定时间间隔内转过的1/5圈的个数
int8 teth1; //复制teth的值，防止teth在做下一步运算时被中断打断，导致cpu取值紊乱
int8 num[6]; //用来存储当前时间间隔内转过的1/5圈的个数转换成的字符串
int8 num2[3]; //用来存储当前的Duty占空比
int8 cesuFlag=0; //调速标志
int8 Duty=30; //占空比 起始值默认是13
int8 ChoiceTime=10; //选择驶过1m的时间（可能值10-21）
int8 Vset=2;//根据计算得出的 只有在一定时间间隔内转过的1/5圈的个数等于Vset的值，小车此时的速度才接近预设值
int8 flag1;

/*
 * 调速函数
 *
 */

void AdjustV()
{

    if(teth1<Vset)
    {
        Duty+=2*PID(Vset,teth1,1);
        if(Duty>90)
            Duty=90;
    }
    if(teth1>Vset)
    {
        Duty-=2*PID(Vset,teth1,1);
        if(Duty==0)
            Duty=0;
    }
}

/*
 * Change num to Char
 * 将int型数据转换为char型，方便在oled上显示
 */

void Cn2C()
{
    num[0]='-';
    num[1]='>';
    num[2]='r';
    num[3]='=';
    num[4]=teth1+48;
    num2[0]=Duty/10+48;
    num2[1]=Duty%10+48;
}

int main(void)
{
    int8 Menu=0;
    Gpio_Init();//I/O口初始化
    Clock_Init();//时钟初始化
    Uart_Init();//串口通信初始化
    OLED_Init();//OLED初始化
    OLED_Clear();//oled清屏

    /*
     * 时间选择
     * UP/DOWM:参数的上调下调
     * OK:确认键
     *
     */
    while(!Menu)
    {
        if(ChoiceTime!=21)
        {
            OLED_ShowStr(0,4,"Time:",16);
            OLED_ShowChar(40,4,ChoiceTime/10+48,16);
            OLED_ShowChar(48,4,ChoiceTime%10+48,16);
            OLED_ShowChar(56,4,' ',16);
            OLED_ShowChar(64,4,' ',16);
        }
        if(ChoiceTime==21)
        {
            OLED_ShowStr(0,4,"Time:Fast!",16);
        }

        if(!(P4IN&BIT7))
        {
            Delay_ms(100);
            if(!(P4IN&BIT7))
            {
                Menu=1;
            }
        }
        if(!(P2IN&BIT4))
        {
            Delay_ms(100);
            if(!(P2IN&BIT4))
            {
                ChoiceTime+=1;
                if(ChoiceTime>21)
                    ChoiceTime=10;
            }
        }
        if(!(P2IN&BIT5))
        {
            Delay_ms(100);
            if(!(P2IN&BIT5))
            {
                ChoiceTime-=1;
                if(ChoiceTime<10)
                    ChoiceTime=21;
            }
        }
        OLED_ShowStr(0,3,"Hit OK to continue!",8);
        OLED_ShowStr(0,0,"Hit UP/DOWM to Choice Time",8);
        while((!(P8IN&BIT4))&&(!(P8IN&BIT5))&&(!(P1IN&BIT4))&&(!(P1IN&BIT7)))
        {
            OLED_ShowStr(0,4,"Stop way3!",16);
            Stop();
        }
    }

    //计数器TimerA0初始化
    switch(ChoiceTime)
    {
    case 10:
        TimerA0_Init(VsetTime10);
        break;
    case 11:
            TimerA0_Init(VsetTime11);
            break;
    case 12:
            TimerA0_Init(VsetTime12);
            break;
    case 13:
            TimerA0_Init(VsetTime13);
            break;
    case 14:
            TimerA0_Init(VsetTime14);
            break;
    case 15:
            TimerA0_Init(VsetTime15);
            break;
    case 16:
            TimerA0_Init(VsetTime16);
            break;
    case 17:
            TimerA0_Init(VsetTime17);
            break;
    case 18:
            TimerA0_Init(VsetTime18);
            break;
    case 19:
            TimerA0_Init(VsetTime19);
            break;
    case 20:
            TimerA0_Init(VsetTime20);
            break;
    case 21:
    {
        Vset=12;
        Duty=80;
        TimerA0_Init(VsetTimeFast);
        break;
    }

    }
    OLED_Clear();
    rightflag=0; //右红外2感应标志
    Bigrightflag=0; //右红外3感应标志
    P1OUT|=BIT0;//led 亮
    OLED_ShowStr(24,4,"Wait!...",16);
    Delay_ms(500);
    Go_ahead(Duty);//前行
    /*
     * 开始进入循迹循环
     */
    while(1)
    {
        while((!(P8IN&BIT4))&&(!(P8IN&BIT5))&&(!(P1IN&BIT4))&&(!(P1IN&BIT7)))
        {
            OLED_ShowStr(0,4,"Stop way3!",16);
            Stop();
        }
        //如果左一红外右一红外同时检测到黑块，则停车
        while((!(P8IN&BIT4))&&(!(P8IN&BIT5)))
        {
            OLED_ShowStr(0,4,"Stop way1!",16);
            Stop();
        }
        //如果右2红外触碰黑块，小右转标志（rightflag）被置1，开始小右转
        while(rightflag)
        {
            rightflag=0;
            //OLED_DrawBMP(0,0,128,7,RightBmp);
            OLED_ShowStr(0,2,"Turn Brigh!",16);
            P3OUT|=BIT6;
            Turn(70,35,Duty,100);
            flag1=1;
        }
        //如果右3红外触碰黑块，大右转标志（Bigrightflag）被置1，开始大右转
        while(Bigrightflag)
        {
           // OLED_DrawBMP(0,0,128,7,RightBmp);
            Bigrightflag=0;
            OLED_ShowStr(0,2,"Turn BBigh!",16);
            P3OUT|=BIT6;
            Turn(80,30,Duty,150);
            flag1=1;

        }
        //如果右1红外检测到黑块，进行直线上的小型矫正旋转
        while(!(P8IN&BIT4))
        {
            //如果在右1刚检测到黑块不久，左1也检测到了，则视为触碰了停止标志
            if(!(P8IN&BIT5))
            {
                OLED_ShowStr(0,4,"Stop way2!",16);
                Stop();
            }
            else
            {
               // OLED_DrawBMP(0,0,128,7,RightBmp);
                OLED_ShowStr(0,2,"Turn right!",16);
                Turn(80,20,Duty,40);
            }
        }
        //如果左1红外检测到黑块，进行直线上的小型矫正旋转
        while(!(P8IN&BIT5))
        {
            //如果在右1刚检测到黑块不久，左1也检测到了，则视为触碰了停止标志
            if(!(P8IN&BIT4))
            {
                OLED_ShowStr(0,4,"Stop way2!",16);
                Stop();
            }
            else
            {
                //OLED_DrawBMP(0,0,128,7,LeftBmp);
                OLED_ShowStr(0,2,"Turn left! ",16);
                Turn(20,80,Duty,40);
            }
        }
        OLED_ShowStr(0,2,"Go  ahead! ",16);
      //  OLED_DrawBMP(0,0,128,7,AheadBmp);
        //如果调速标志被中断置1，则会进入调速函数中
        if(cesuFlag==1)
        {
            if(flag1==1)
            {
                cesuFlag=0;//清除调速标志
            }
            else
            {
                OLED_ShowStr(16,2,"Ad",16);
                teth1=teth; //复制teth的值
                AdjustV(); //调速
                Go_ahead(Duty);//前行
                Cn2C();
                OLED_ShowStr(0,0,"Duty:",16);
                OLED_ShowStr(48,0,num2,16);
                OLED_ShowStr(0,6,num,16);
                teth=0;
                cesuFlag=0;//清除调速标志
            }

        }

    }

}
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer0_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    cesuFlag=1;
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
            Bigrightflag=1;
            P1IFG &= ~BIT4;
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
                rightflag=1;
                P1IFG &= ~BIT7;                           // Clear P1.1 IFG
                break;
            }
    }
}



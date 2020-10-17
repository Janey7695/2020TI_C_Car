# 2020TI_C题小车
——By Fang linghai、Li fusheng、Xu jinyuan
## 文件概要
1. main.c  
程序的主函数，在其内完成小车控制
2. MotorCTL.c  
电机占空比控制模块
3. sys.c  
系统初始化模块，包含了2个硬件延时、时钟初始化设置、GPIO初始化设置、串口初始化设置、以及一个串口printf函数用来向上位机发送程序运行情况。
4. config.h  
对计时器CCR0填充值的宏定义，以及对标识符的重定义。
5. PID.c  
内含一个极其简略的P算法
6. oled.c oledfont.h oledbmp.h  
oled的显示模块驱动
## 整体思路
1. 小车移动  
通过msp430的TimerA1计数器的UP模式(频率选用外接37.768KHz)，输出一定的PWM波控制电机占空比，进而控制速度。
2. 循迹  
通过红外对管对黑块的检测进而改变2边电机占空比，形成差速，实现转弯，4个红外对管，中间2个用来直线上的矫正，右边2个用来过弯。
3. 速度自适应
通过车轮上的码盘配合一个红外对管，实时测量小车当前的速度，并通过PID稳定速度为预设速度。
## 主要函数说明
### MotroCTL.c
1. `void Go_ahead(unsigned char Duty);`

* 功能：改变TimerA1中CCR0和CCR1的值，进而改变输出PWM波的占空比，进而改变电机转速。
* 参数：  
a. `unsigned char Duty` 无符号`char`型，占空比数值，取值（0-99）。
2. `void Turn(unsigned char lefP,unsigned char rightP,unsigned char Duty,unsigned int Ms);`

* 功能：改变TimerA1中CCR0和CCR1的值，进而改变输出PWM波的占空比，进而使电机产生差速，并转弯一定的时间，转完后会在函数内部执行一次`Go_ahead()`函数，保证小车转完弯后继续直行。
* 参数：  
a. `unsigned char lefP` 无符号`char`型，左轮占空比数值，取值（0-99）。  
b. `unsigned char rightP` 无符号`char`型，右轮占空比数值，取值（0-99）。  
c. `unsigned char Duty` 无符号`char`型，占空比数值，取值（0-99）。  
d. `unsigned int Ms` 无符号`int`型 表示每次检测到黑块，小车转弯的时间，单位ms。
3. `void Stop();`

* 功能：停止运行，蜂鸣器响一声，关闭所有的中断，避免继续运行。
* 参数：  
无
### sys.c
1. `void Delay_us(int us);`  `void Delay_ms(int ms);`  
* 功能：硬件延时函数
* 参数：  

2. `void Clock_Init();`  
* 功能：时钟初始化。
* 说明：改初始化将单片机主频设置为8M，Aclk设置为32.768kHz.
3. `void Gpio_Init();`  
* 功能：IO口初始化。
* 说明：  
|  
P1.3 ---->小车左轮组（1输出口）  
P3.0 ---->小车左轮组（0输出口）  
P3.3 ---->小车右轮组（1输出口）  
P3.6 ---->小车右轮组（0输出口）  
|  
P2.0 ----> TxD  
P2.1 ----> RxD  
|  
P8.5 ----> 左红外  
P8.4 ----> 右红外  
P1.4 ----> 右红外三  
P1.5 ----> 测速码盘  
P1.7 ----> 右红外二  
|  
P4.7 ----> OK按键  
P2.4 ----> UP 按键  
P2.5 ----> DOWM 按键  
|  
P9.4 ----> 蜂鸣器  
|  
P8.6 ----> OLED-D0(SCL)  
P8.7 ----> OLED-D1(SDIN)  
P9.0 ----> OLED-RST  
P9.1 ----> OLED-DC  
P9.5 ----> OLED-CS  
|  
4. `void Uart_Init();`  
* 功能：串口通信的初始化，将波特率设置为9600.
5. `void printf(unsigned char str[],unsigned char ent);`  
* 功能：向上位机发送字符串。
* 参数：  
a. `unsigned char str[]` 无符号型字符串，是需要传输的信息  
b. `unsigned char ent` 是否需要回车换行标识，若为1则换行，若为0则不换行。 
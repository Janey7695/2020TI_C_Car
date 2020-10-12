/*
 * oled.c
 *
 *  Created on: 2020年10月11日
 *      Author: Lenovo
 */

#include <oled.h>
#include <sys.h>
#include <oledfont.h>


//void Delay_ms(int ms)
//{
//    while(ms--)
//    {
//        delay_ms(1);
//    }
//
//}

//void Delay_us(int us)
//{
//    while(us--)
//    {
//        delay_us(1);
//    }
//}

void OLED_WR_Byte(uchar dat,uchar mode)
{
    uchar i;
    if(mode)
        DC_E();
    else
        DC_NE();

    CS_NE();

    for(i=0;i<8;i++)
    {
        SCL_L();
        if(dat&0x80)
        {
            SDIN_H();
        }
        else
        {
            SDIN_L();
        }
        SCL_H();
        dat<<=1;
    }

    CS_E();
    DC_E();
}

void OLED_Set_Pos(uchar x,uchar y)
{
    OLED_WR_Byte(0xb0+y,OLED_CMD);
    OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
    OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD);
}

void OLED_Display_On(){
    OLED_WR_Byte(0x8d,OLED_CMD);
    OLED_WR_Byte(0x14,OLED_CMD);
    OLED_WR_Byte(0xaf,OLED_CMD);
}

void OLED_Display_Off(){
    OLED_WR_Byte(0x8d,OLED_CMD);
    OLED_WR_Byte(0x14,OLED_CMD);
    OLED_WR_Byte(0xae,OLED_CMD);
}

void OLED_Clear(){
    uchar i,n;
    for(i=0;i<8;i++){
        OLED_WR_Byte(0xb0+i,OLED_CMD);
        OLED_WR_Byte(0x00,OLED_CMD);
        OLED_WR_Byte(0x10+i,OLED_CMD);
        for(n=0;n<128;n++)
            OLED_WR_Byte(0,OLED_DATA);
    }
}

void OLED_ShowChar(uchar x,uchar y,uchar CHAR,uchar fontsize){
    uchar i,j;
    j=CHAR-32;
    if(fontsize==16)
    {
        OLED_Set_Pos(x,y);
        for(i=0;i<8;i++)
        {
            OLED_WR_Byte(CHAR16x8[j*16+i],OLED_DATA);
        }
        OLED_Set_Pos(x,y+1);
        for(i=0;i<8;i++)
        {
            OLED_WR_Byte(CHAR16x8[j*16+i+8],OLED_DATA);
        }
    }
    else
    {
        OLED_Set_Pos(x,y);
        for(i=0;i<6;i++)
        {
            OLED_WR_Byte(CHAR8x6[j][i],OLED_DATA);
        }
    }
}

void OLED_ShowStr(uchar x,uchar y,uchar*chr,uchar fontsize){
    uchar j=0,spacing;

    if(fontsize==16) spacing=8;
    else spacing=6;

    while(chr[j]!='\0')
    {
        OLED_ShowChar(x,y,chr[j],fontsize);
        x+=spacing;
        if(x>120)
        {
            x=0;
            y+=2;
        }
        j++;
    }
}

void OLED_DrawBMP(uchar x0,uchar y0,uchar x1,uchar y1,uchar BMP[])
{
    uint j=0;
    uchar x,y;
    if(y1%8==0)
        y=y1/8;
    else
        y=y1/8+1;
    for(y=y0;y<y1;y++)
    {
        OLED_Set_Pos(x0,y);
        for(x=x0;x<x1;x++)
        {
            OLED_WR_Byte(BMP[j++],OLED_DATA);
        }
    }
}

void OLED_Init(){
    RST_E();
    Delay_ms(100);
    RST_NE();
    Delay_ms(100);
    RST_E();
    OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
    OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
    OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
    OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
    OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
    OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
    OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
    OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
    OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
    OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset   Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WR_Byte(0x00,OLED_CMD);//-not offset
    OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
    OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
    OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
    OLED_WR_Byte(0x12,OLED_CMD);
    OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
    OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
    OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WR_Byte(0x02,OLED_CMD);//
    OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
    OLED_WR_Byte(0x14,OLED_CMD);
    OLED_WR_Byte(0xA4,OLED_CMD);
    OLED_WR_Byte(0xA6,OLED_CMD);
    OLED_WR_Byte(0xAF,OLED_CMD);

    OLED_WR_Byte(0xAF,OLED_CMD);
    OLED_Clear();
    OLED_Set_Pos(0,0);
}

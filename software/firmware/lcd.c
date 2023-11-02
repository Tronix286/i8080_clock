//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STC12C5A60S2,晶振30M  单片机工作电压3.3V
//QDtech-LCD液晶驱动 for C51
//xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtft.com
//淘宝网站：http://qdtech.taobao.com
//wiki技术网站：http://www.lcdwiki.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-23594567 
//手机:15989313508（冯工） 
//邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
//技术支持QQ:3002773612  3002778157
//技术交流QQ群:324828016
//创建日期:2018/7/7
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2018-2028
//All rights reserved
//********************************************************************************
//=========================================电源接线================================================//
//5V接DC 5V电源
//GND接地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为16位并口
//液晶屏模块             C51单片机
// DB0~DB7       接       P00~P07        //数据低8位
// DB8~DB15      接       P20~P27        //数据高8位
//=======================================液晶屏控制线接线==========================================//
//液晶屏模块             C51单片机
// LCD_CS        接        P13           //片选控制信号
// LCD_RST       接        P33           //复位信号
// LCD_RS        接        P12           //数据/命令选择控制信号
// LCD_WR        接        P11           //写控制信号
// LCD_RD        接        P10           //读控制信号
//=========================================触摸屏接线=========================================//
//STC89C52RC单片机ROM太小，无法运行带触摸功能的程序，所以触摸屏不需要接线
//不使用触摸或者模块本身不带触摸，则可不连接
//触摸屏使用的数据总线类型为SPI
//触摸屏模块             C51单片机
//  T_CLK        接        P36          //触摸屏SPI时钟信号
//  T_CS         接        P37          //触摸屏片选控制信号
//  T_DIN        接        P34          //触摸屏SPI写信号
//  T_DO         接        P35          //触摸屏SPI读信号
//  T_IRQ        接        P40          //触摸屏响应检查信号，如单片机无P4组，请自行更改其他可用IO并修改代码定义
//**************************************************************************************************/	
#include <stdlib.h>
#include "lcd.h"
#include "main.h"

//LCD的画笔颜色和背景色	   
u16 POINT_COLOR=0x0000;	//画笔颜色
u16 BACK_COLOR=0xFFFF;  //背景色 
//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

/*void LCD_write(u8 HVAL,u8 LVAL)
{
	LCD_CS = 0;
	LCD_WR = 0;
	LCD_DataPortH = HVAL;
	LCD_DataPortL = LVAL;
	LCD_WR = 1;
	LCD_CS = 1;
}

u16 LCD_read(void)
{
	u16 d;
	LCD_CS = 0;
	LCD_RD = 0;
	delay_us(1); //delay 1 us
	d = LCD_DataPortH;
	d = (d<<8)|LCD_DataPortL;
	LCD_RD = 1;
  LCD_CS = 1;
	return d;
}
*/
/*****************************************************************************
 * @name       :void LCD_WR_REG(u16 Reg)	
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_REG(u16 Reg) __z88dk_fastcall
{	
#asm
  ld hl,4
  add hl,sp              ; skip over return address on stack

	#ifdef LCD_PIO
	mvi a,LCD_RS		; RS low
	out PIO_M
	mvi a,LCD_CS		; CS low
	out PIO_M
	#endif

//	mov a,l
	ld a,(hl)
	#ifdef LCD_PIO
	out PIO_A
	mvi a,LCD_WR		; WR low
	out PIO_M
	mvi a,LCD_WR | 1	; WR high
	out PIO_M
	mvi a,LCD_CS | 1	; CS high
	out PIO_M
	#else
	out LCD_CMD_PORT
	#endif

#endasm
} 

/*****************************************************************************
 * @name       :void LCD_WR_DATA(u16 Data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_DATA(u16 Data) __z88dk_fastcall
{
#asm
  ld hl,4
  add hl,sp              ; skip over return address on stack

	#ifdef LCD_PIO
	mvi a,LCD_RS | 1	; RS high
	out PIO_M
	mvi a,LCD_CS		; CS low
	out PIO_M
	#endif

//	mov a,l
	ld a,(hl)
	#ifdef LCD_PIO
	out PIO_A

	mvi a,LCD_WR		; WR low
	out PIO_M
	mvi a,LCD_WR | 1	; WR high
	out PIO_M
	mvi a,LCD_CS | 1	; CS high
	out PIO_M
	#else
	out LCD_DATA_PORT
	#endif
#endasm
}

/*****************************************************************************
 * @name       :u16 LCD_RD_DATA(void)
 * @date       :2018-11-13 
 * @function   :Read an 16-bit value from the LCD screen
 * @parameters :None
 * @retvalue   :read value
******************************************************************************/
u16 LCD_RD_DATA(void) __z88dk_fastcall
{
#asm
	in LCD_DATA_PORT	; dummy data
	nop

	in LCD_DATA_PORT
	mov h,a
	mvi l,0
#endasm
}

/*****************************************************************************
 * @name       :void LCD_WR_DATA_16Bit(u16 Data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/	 
void LCD_WR_DATA_16Bit(u16 Data) __z88dk_fastcall
{
#asm
  ld hl,4
  add hl,sp              ; skip over return address on stack

	#ifdef LCD_PIO
	mvi a,LCD_RS | 1	; RS high
	out PIO_M
	mvi a,LCD_CS		; CS low
	out PIO_M
	#endif

//	mov a,h
	inc hl
	ld a,(hl)
	#ifdef LCD_PIO
	out PIO_A

	mvi a,LCD_WR		; WR low
	out PIO_M
	mvi a,LCD_WR | 1	; WR high
	out PIO_M
	mvi a,LCD_CS | 1	; CS high
	out PIO_M

	mvi a,LCD_CS		; CS low
	out PIO_M
	#else
	out LCD_DATA_PORT
	#endif

//	mov a,l
	dec hl
	ld a,(hl)
	#ifdef LCD_PIO
	out PIO_A

	mvi a,LCD_WR		; WR low
	out PIO_M
	mvi a,LCD_WR | 1	; WR high
	out PIO_M
	mvi a,LCD_CS | 1	; CS high
	out PIO_M
	#else
	out LCD_DATA_PORT
	#endif
#endasm
}

/*****************************************************************************
 * @name       :u16 Lcd_ReadData_16Bit(void)
 * @date       :2018-11-13 
 * @function   :Read an 16-bit value from the LCD screen
 * @parameters :None
 * @retvalue   :read value
******************************************************************************/	
u16 Lcd_RD_DATA_16Bit(void) __z88dk_fastcall
{
#asm
	in LCD_DATA_PORT	; dummy data
	nop

	in LCD_DATA_PORT
	mov h,a
	in LCD_DATA_PORT
	mov l,a
#endasm
}

/*****************************************************************************
 * @name       :void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
 * @date       :2018-08-09 
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{
  LCD_WR_REG(LCD_Reg);
	LCD_WR_DATA(LCD_RegValue);
}

/*****************************************************************************
 * @name       :u16 LCD_ReadReg(u16 LCD_Reg)
 * @date       :2018-11-13 
 * @function   :read value from specially registers
 * @parameters :LCD_Reg:Register address
 * @retvalue   :read value
******************************************************************************/
void LCD_ReadReg(u16 LCD_Reg,u8 *Rval,int n)
{
	LCD_WR_REG(LCD_Reg);
	while(n--)
	{		
		*(Rval++) = LCD_RD_DATA();
	}
}

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09 
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_WriteRAM_Prepare(void)
{
 	LCD_WR_REG(lcddev.wramcmd);	  
}

/*****************************************************************************
 * @name       :void LCD_ReadRAM_Prepare(void)
 * @date       :2018-11-13 
 * @function   :Read GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void LCD_ReadRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.rramcmd);
}

/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/	
void LCD_Clear(u16 Color)
{
	u16 i,j;
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);	
/*
    for(i=0;i<lcddev.width;i++)
	 {
	  for (j=0;j<lcddev.height;j++)
	   {
        	 LCD_WR_DATA_16Bit(Color);
	   }

	  }
*/
	#asm
	push d

	ld hl,8
	add hl,sp
	ld b,(hl)
	inc hl
	ld c,(hl)
	;out 20h

	lxi d,38400  ; (320x480/4)
do_clr:
	mov a,c
	out LCD_DATA_PORT
	mov a,b
	out LCD_DATA_PORT
	mov a,c
	out LCD_DATA_PORT
	mov a,b
	out LCD_DATA_PORT
	mov a,c
	out LCD_DATA_PORT
	mov a,b
	out LCD_DATA_PORT
	mov a,c
	out LCD_DATA_PORT
	mov a,b
	out LCD_DATA_PORT
	dcx d
	mov     a,d         ;Copy D to A    (so as to compare DE with zero)
        ora     e           ;A = A | E      (are both D and E zero?)
        jnz     do_clr      ;Jump to 'do_clr:' if the zero-flag is not set. 

	pop d
	#endasm
}

/*****************************************************************************
 * @name       :void LCD_DrawPoint(u16 x,u16 y)
 * @date       :2018-08-09 
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/	
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetWindows(x,y,x,y);//设置光标位置 
	LCD_WR_DATA_16Bit(POINT_COLOR); 	    
} 	 


/*****************************************************************************
 * @name       :u16 LCD_ReadPoint(u16 x,u16 y)
 * @date       :2018-11-13 
 * @function   :Read a pixel color value at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :the read color value
******************************************************************************/	
u16 LCD_ReadPoint(u16 x,u16 y)
{
	u16 color;
	if(x>=lcddev.width||y>=lcddev.height)
	{
		return 0;	//超过了范围,直接返回	
	}
	LCD_SetCursor(x,y);//设置光标位置 
	LCD_ReadRAM_Prepare();
	
	color = Lcd_RD_DATA_16Bit();
	
	return color;
}

/*****************************************************************************
 * @name       :void LCD_Set_GPIO(void)
 * @date       :2018-11-13 
 * @function   :Set the gpio to push-pull mode
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_Delay(void)
{
    int i,j;
    for (i=0;i<255;i++)
       for (j=0;j<255;j++)
         #asm
               nop
	 #endasm
}

/*****************************************************************************
 * @name       :void LCD_Reset(void)
 * @date       :2018-08-09 
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_Reset(void)
{

/*    msleep(120);
    #asm
	mvi a,LCD_RESET
	out PIO_M
    #endasm   
    msleep(180);
    #asm
	mvi a,LCD_RESET | 1
	out PIO_M
    #endasm
    msleep(100);
*/

/*    #asm
	mvi a,LCD_RS		; RS low
	out PIO_M
	mvi a,LCD_CS		; CS low
	out PIO_M

	mvi a,0x00
	out PIO_A
	
	mvi a,LCD_WR		; WR low
	out PIO_M
	mvi a,LCD_WR | 1	; WR high
	out PIO_M

	mvi a,LCD_WR		; WR low
	out PIO_M
	mvi a,LCD_WR | 1	; WR high
	out PIO_M

	mvi a,LCD_WR		; WR low
	out PIO_M
	mvi a,LCD_WR | 1	; WR high
	out PIO_M

	mvi a,LCD_CS | 1	; CS high
	out PIO_M
    #endasm    
*/
//    LCD_Delay();

}

/*****************************************************************************
 * @name       :void LCD_Init(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 	 
void LCD_Init(void)
{
#if 0
	LCD_Reset(); //初始化之前复位
    LCD_WR_REG(0xF1);
    LCD_WR_DATA(0x36);
    LCD_WR_DATA(0x04);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x3C);
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x8F);


    LCD_WR_REG(0xF2);
    LCD_WR_DATA(0x18);
    LCD_WR_DATA(0xA3);
    LCD_WR_DATA(0x12);
    LCD_WR_DATA(0x02);
    LCD_WR_DATA(0xb2);
    LCD_WR_DATA(0x12);
    LCD_WR_DATA(0xFF);
    LCD_WR_DATA(0x10);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xF8);
    LCD_WR_DATA(0x21);
    LCD_WR_DATA(0x04);

    LCD_WR_REG(0xF9);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x08);  

    LCD_WR_REG(0xC0);
    LCD_WR_DATA(0x0f); //13
    LCD_WR_DATA(0x0f); //10

    LCD_WR_REG(0xC1);
    LCD_WR_DATA(0x42); //43

    LCD_WR_REG(0xC2);
    LCD_WR_DATA(0x22);

    LCD_WR_REG(0xC5);
    LCD_WR_DATA(0x01); //00
    LCD_WR_DATA(0x29); //4D
    LCD_WR_DATA(0x80);

    LCD_WR_REG(0xB6);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x02); //42
    LCD_WR_DATA(0x3b);

    LCD_WR_REG(0xB1);
    LCD_WR_DATA(0xB0); //C0
    LCD_WR_DATA(0x11);

    LCD_WR_REG(0xB4);
    LCD_WR_DATA(0x02); //01

    LCD_WR_REG(0xE0);
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x18);
    LCD_WR_DATA(0x15);
    LCD_WR_DATA(0x09);
    LCD_WR_DATA(0x0B);
    LCD_WR_DATA(0x04);
    LCD_WR_DATA(0x49);
    LCD_WR_DATA(0x64);
    LCD_WR_DATA(0x3D);
    LCD_WR_DATA(0x08);
    LCD_WR_DATA(0x15);
    LCD_WR_DATA(0x06);
    LCD_WR_DATA(0x12);
    LCD_WR_DATA(0x07);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xE1);
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x38);
    LCD_WR_DATA(0x35);
    LCD_WR_DATA(0x0a);
    LCD_WR_DATA(0x0c);
    LCD_WR_DATA(0x03);
    LCD_WR_DATA(0x4A);
    LCD_WR_DATA(0x42);
    LCD_WR_DATA(0x36);
    LCD_WR_DATA(0x04);
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x03);
    LCD_WR_DATA(0x1F);
    LCD_WR_DATA(0x1B);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0x20);                     // display inversion OFF
  
    LCD_WR_REG(0x36);      // MEMORY_ACCESS_CONTROL (orientation stuff)
    LCD_WR_DATA(0x48);
     
    LCD_WR_REG(0x3A);      // COLMOD_PIXEL_FORMAT_SET
    LCD_WR_DATA(0x55);     // 16 bit pixel 

    LCD_WR_REG(0x13); // Nomal Displaymode
    
    LCD_WR_REG(0x11);                     // sleep out
    msleep(150);
     
    LCD_WR_REG(0x29);                     // display on
    msleep(150);

#endif
#if 0
	// WORKED
	LCD_Reset(); //初始化之前复位
    LCD_WR_REG(0xF9);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x08);
    LCD_WR_REG(0xC0);
    LCD_WR_DATA(0x19);
    LCD_WR_DATA(0x1A);
    LCD_WR_REG(0xC1);
    LCD_WR_DATA(0x45);
    LCD_WR_DATA(0X00);
    LCD_WR_REG(0xC2);
    LCD_WR_DATA(0x33);
    LCD_WR_REG(0xC5);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x28);
    LCD_WR_REG(0xB1);
    LCD_WR_DATA(0x90);
    LCD_WR_DATA(0x11);
    LCD_WR_REG(0xB4);
    LCD_WR_DATA(0x02);
    LCD_WR_REG(0xB6);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x42);
    LCD_WR_DATA(0x3B);
    LCD_WR_REG(0xB7);
    LCD_WR_DATA(0x07);
    LCD_WR_REG(0xE0);
    LCD_WR_DATA(0x1F);
    LCD_WR_DATA(0x25);
    LCD_WR_DATA(0x22);
    LCD_WR_DATA(0x0B);
    LCD_WR_DATA(0x06);
    LCD_WR_DATA(0x0A);
    LCD_WR_DATA(0x4E);
    LCD_WR_DATA(0xC6);
    LCD_WR_DATA(0x39);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_REG(0xE1);
    LCD_WR_DATA(0x1F);
    LCD_WR_DATA(0x3F);
    LCD_WR_DATA(0x3F);
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x1F);
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x46);
    LCD_WR_DATA(0x49);
    LCD_WR_DATA(0x31);
    LCD_WR_DATA(0x05);
    LCD_WR_DATA(0x09);
    LCD_WR_DATA(0x03);
    LCD_WR_DATA(0x1C);
    LCD_WR_DATA(0x1A);
    LCD_WR_DATA(0x00);
    LCD_WR_REG(0xF1);
    LCD_WR_DATA(0x36);
    LCD_WR_DATA(0x04);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x3C);
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0xA4);
    LCD_WR_DATA(0x02);
    LCD_WR_REG(0xF2);
    LCD_WR_DATA(0x18);
    LCD_WR_DATA(0xA3);
    LCD_WR_DATA(0x12);
    LCD_WR_DATA(0x02);
    LCD_WR_DATA(0x32);
    LCD_WR_DATA(0x12);
    LCD_WR_DATA(0xFF);
    LCD_WR_DATA(0x32);
    LCD_WR_DATA(0x00);
    LCD_WR_REG(0xF4);
    LCD_WR_DATA(0x40);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x08);
    LCD_WR_DATA(0x91);
    LCD_WR_DATA(0x04);
    LCD_WR_REG(0xF8);
    LCD_WR_DATA(0x21);
    LCD_WR_DATA(0x04);
    LCD_WR_REG(0x36);
    LCD_WR_DATA(0x48);
    LCD_WR_REG(0x3A);
    LCD_WR_DATA(0x55);    
    LCD_WR_REG(0x11);    //Exit Sleep
	msleep(150);
    LCD_WR_REG(0x29);    //Display on#endif
#endif
#if 0
	LCD_Reset(); //初始化之前复位

    LCD_WR_REG(0x01); // SW reset
    msleep(120);

    LCD_WR_REG(0x11); // Sleep out, also SW reset
    msleep(120);

    LCD_WR_REG(0x3A);
      LCD_WR_DATA(0x55);           // 16 bit colour interface

    LCD_WR_REG(0xC0); //                          1100.0000 Power Control 1
    LCD_WR_DATA(0x0E);    //                          0001.0111   ... VRH1
    LCD_WR_DATA(0x0E);    //                          0001.0101   ... VRH2
    LCD_WR_REG(0xC1); //                          1100.0001 Power Control 2
    LCD_WR_DATA(0x41);    //                          0100.0001   . SAP BT
    LCD_WR_DATA(0x00);    //                          0000.0000   ..... VC
    LCD_WR_REG(0xC2); //                          1100.0010 Power Control 3
    LCD_WR_DATA(0x55);    //     nb. was 0x44         0101.0101   . DCA1 . DCA0

    LCD_WR_REG(0xC5);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xE0);
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x1F);
    LCD_WR_DATA(0x1C);
    LCD_WR_DATA(0x0C);
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x08);
    LCD_WR_DATA(0x48);
    LCD_WR_DATA(0x98);
    LCD_WR_DATA(0x37);
    LCD_WR_DATA(0x0A);
    LCD_WR_DATA(0x13);
    LCD_WR_DATA(0x04);
    LCD_WR_DATA(0x11);
    LCD_WR_DATA(0x0D);
    LCD_WR_DATA(0x00);
 
    LCD_WR_REG(0xE1);
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x32);
    LCD_WR_DATA(0x2E);
    LCD_WR_DATA(0x0B);
    LCD_WR_DATA(0x0D);
    LCD_WR_DATA(0x05);
    LCD_WR_DATA(0x47);
    LCD_WR_DATA(0x75);
    LCD_WR_DATA(0x37);
    LCD_WR_DATA(0x06);
    LCD_WR_DATA(0x10);
    LCD_WR_DATA(0x03);
    LCD_WR_DATA(0x24);
    LCD_WR_DATA(0x20);
    LCD_WR_DATA(0x00);
 
      LCD_WR_REG(0x20);
 
    LCD_WR_REG(0x36);
    LCD_WR_DATA(0x48);

    LCD_WR_REG(0x29);                     // display on
	msleep(150);
#endif
#if 1

	LCD_Reset(); //初始化之前复位
//	LCD_WR_REG(0X00);
//	LCD_WR_REG(0X00);
//	LCD_WR_REG(0X00);

	LCD_WR_REG(0XF1);
	LCD_WR_DATA(0x36);
	LCD_WR_DATA(0x04);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x3C);
	LCD_WR_DATA(0X0F);
	LCD_WR_DATA(0x8F);
	LCD_WR_REG(0XF2);
	LCD_WR_DATA(0x18);
	LCD_WR_DATA(0xA3);
	LCD_WR_DATA(0x12);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0XB2);
	LCD_WR_DATA(0x12);
	LCD_WR_DATA(0xFF);
	LCD_WR_DATA(0x10);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0XF8);
	LCD_WR_DATA(0x21);
	LCD_WR_DATA(0x04);
	LCD_WR_REG(0XF9);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x08);
	LCD_WR_REG(0x36);
	LCD_WR_DATA(0x08);
	LCD_WR_REG(0xB4);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0xC1);
	LCD_WR_DATA(0x41);
	LCD_WR_REG(0xC5);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x91);
	LCD_WR_DATA(0x80);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0xE0);
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0x1F);
	LCD_WR_DATA(0x1C);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x48);
	LCD_WR_DATA(0x98);
	LCD_WR_DATA(0x37);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x13);
	LCD_WR_DATA(0x04);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x0D);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0xE1);
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0x32);
	LCD_WR_DATA(0x2E);
	LCD_WR_DATA(0x0B);
	LCD_WR_DATA(0x0D);
	LCD_WR_DATA(0x05);
	LCD_WR_DATA(0x47);
	LCD_WR_DATA(0x75);
	LCD_WR_DATA(0x37);
	LCD_WR_DATA(0x06);
	LCD_WR_DATA(0x10);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0x24);
	LCD_WR_DATA(0x20);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0x3A);
	LCD_WR_DATA(0x55);
	LCD_WR_REG(0x11);
	LCD_WR_REG(0x36);
	LCD_WR_DATA(0x28);
	msleep(150);
	//LCD_Delay();
	LCD_WR_REG(0x29);
#endif

	//设置LCD属性参数
	LCD_direction(3);//设置LCD显示方向 
//	LCD_BL=1;//点亮背光	 

}
 
/*****************************************************************************
 * @name       :void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
 * @date       :2018-08-09 
 * @function   :Setting LCD display window
 * @parameters :xStar:the bebinning x coordinate of the LCD display window
								yStar:the bebinning y coordinate of the LCD display window
								xEnd:the endning x coordinate of the LCD display window
								yEnd:the endning y coordinate of the LCD display window
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(xStar>>8);
	LCD_WR_DATA(0x00FF&xStar);		
	LCD_WR_DATA(xEnd>>8);
	LCD_WR_DATA(0x00FF&xEnd);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(yStar>>8);
	LCD_WR_DATA(0x00FF&yStar);		
	LCD_WR_DATA(yEnd>>8);
	LCD_WR_DATA(0x00FF&yEnd);	

	LCD_WriteRAM_Prepare();	//开始写入GRAM				
}   

/*****************************************************************************
 * @name       :void LCD_SetCursor(u16 Xpos, u16 Ypos)
 * @date       :2018-08-09 
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
								Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/ 
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_SetWindows(Xpos,Ypos,Xpos,Ypos);	
} 

/*****************************************************************************
 * @name       :void LCD_direction(u8 direction)
 * @date       :2018-08-09 
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
													2-180 degree
													3-270 degree
 * @retvalue   :None
******************************************************************************/ 
void LCD_direction(u8 direction)
{ 
			lcddev.setxcmd=0x2A;
			lcddev.setycmd=0x2B;
			lcddev.wramcmd=0x2C;
			lcddev.rramcmd=0x2E;
	switch(direction){		  
		case 0:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;		
			LCD_WriteReg(0x36,(1<<6)|(1<<3));//0 degree MY=0,MX=0,MV=0,ML=0,BGR=1,MH=0
		break;
		case 1:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x36,(1<<3)|(1<<4)|(1<<5));//90 degree MY=0,MX=1,MV=1,ML=1,BGR=1,MH=0
		break;
		case 2:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;	
			LCD_WriteReg(0x36,(1<<3)|(1<<7));//180 degree MY=1,MX=1,MV=0,ML=0,BGR=1,MH=0
		break;
		case 3:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			LCD_WriteReg(0x36,(1<<3)|(1<<5)|(1<<6)|(1<<7));//270 degree MY=1,MX=0,MV=1,ML=0,BGR=1,MH=0
		break;	
		default:break;
	}		
	
}	 

/*****************************************************************************
 * @name       :u16 LCD_Read_ID(void)
 * @date       :2018-11-13 
 * @function   :Read ID
 * @parameters :None
 * @retvalue   :ID value
******************************************************************************/ 
u16 LCD_Read_ID(void)
{
	u8 val[4] = {0};
	LCD_ReadReg(0xD3,val,4);
	return (val[2]<<8)|val[3];
}




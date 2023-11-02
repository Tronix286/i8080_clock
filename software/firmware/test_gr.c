#include <stdlib.h>
#include "lcd.h"
#include "gui.h"
#include "cust_fnt.h"
#include "i2c.h"
#include <stdio.h>
#include <string.h>
#include "pic.h"
#include "onewire.h"
#include "bme280.h"

uint8_t buf[40];
uint8_t key;
uint8_t y[4]={20,40,115,50};
//uint8_t buff
uint16_t col=0;

void wait_key()
{
		#asm
		   mvi a,PIO_C_IN
		   out PIO_M
		   in PIO_C
		   sta _key
		#endasm
		while ((key & 2))
		{
		#asm
		   mvi a,PIO_C_IN
		   out PIO_M
		   in PIO_C
		   sta _key
		#endasm
		}
		while (!(key & 2))
		{
		#asm
		   mvi a,PIO_C_IN
		   out PIO_M
		   in PIO_C
		   sta _key
		#endasm
		}
}

void    IntegerBezier(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t x3,uint16_t y3,uint16_t x4,uint16_t y4)
{
        uint16_t     x12,y12,x23,y23,x34,y34,x123,y123,x234,y234,x1234,y1234;

	memset(buf,0,40);
	sprintf((char *)buf,"deep=%d; %d %d %d %d %d %d %d %d", col,x1,y1,x2,y2,x3,y3,x4,y4);
	LCD_ShowString(50,300,16,buf,0);

	wait_key();
	col++;

        // if the distance between points (x1,y1) and (x4,y4) is < 16 //
        // plot the line (x1,y1,x4,y4)
        if      (abs(x4-x1)+abs(y4-y1) < 16)
        {
                //LCD_DrawLine(x1,320-y1,x4,320-y4);      // Draw line from (x1,y1) -> (x4,y4)
		drawLine(x1,y1,x4,y4,GREEN);
		col--;
                return;                 // Return up recursion tree
        }

        // Calculate all the mid-points of the lines //
        x12 = (x1+x2)/2;                // The compiler will probably turn //
        y12 = (y1+y2)/2;                //  these into shifts, since they  //
        x23 = (x2+x3)/2;                //  are integer variables          //
        y23 = (y2+y3)/2;                //  (if the C compiler's decent)   //
        x34 = (x3+x4)/2;                // Are there any decent C compiler's //
        y34 = (y3+y4)/2;                //  left on the PC???              //
        x123 = (x12+x23)/2;             // Best C compiler I ever used was //
        y123 = (y12+y23)/2;             //  Acorn C for RISC OS (for ARM 600) //
        x234 = (x23+x34)/2;             //  Best RISC processor available  //
        y234 = (y23+y34)/2;             //  (Ooops, here come the flames!) //
        x1234 = (x123+x234)/2;
        y1234 = (y123+y234)/2;
        IntegerBezier(x1,x2,x12,y12,x123,y123,x1234,y1234);
        IntegerBezier(x1234,y1234,x234,y234,x34,y34,x4,y4);

}

void cubic_hermite()
{
   unsigned char j;
    unsigned char l = 0;
    for(j = 0; j < 16; j++)
    {
        // (global) unsigned char y[4] = {0};
/*
        y[0] = y[1];
        y[1] = y[2];
        y[2] = y[3];
        //y[3] = randchar(); // Wrapper, limits standard rand to [0,255]
        // Debug for overshoot worst case. (y[] should alternate 2xMAX, 2xMIN)
        y[3] = y[0]; 

        //further limit our starting points to prevent overshoot
        if (y[3] > (255-16)){y[3]=(255-16);}
        if (y[3] < 12){y[3]=12;}
*/

        unsigned char k;
        const unsigned char c0 = 64; // amount of fixed point shift.
        for(k = c0; k < c0*2; k = k+(c0/16)) {
            signed int A1 = (((c0 - k) * y[0] + k * y[1]) / c0);
            signed int A2 = ((((c0*2) - k) * y[1] + (k - c0) * y[2]) / c0);
            signed int A3 = ((((c0*3) - k) * y[2] + (k - (c0*2)) * y[3]) / c0);

            signed int B1 = ((((c0*2) - k) / 2 * A1 + k / 2 * A2) / c0);
            signed int B2 = ((((c0*3) - k) / 2 * A2 + (k - c0) / 2 * A3) / c0);

            // (global) unsigned char buff[256] = {0};
            //buff[l] = ((((c0*2) - k) * B1 + (k - c0) * B2) + (c0*16))/ c0;
	    LCD_DrawPoint(l,320-((((c0*2) - k) * B1 + (k - c0) * B2) + (c0*16))/ c0);
            l++;
        }
    }
}

void main()
{
	LCD_Init();
//	POINT_COLOR = WHITE;
	while(1)
	{
		LCD_Clear(BLACK);

		//cubic_hermite();
		gui_circle(10,20,WHITE,2,0);
		gui_circle(40,25,WHITE,2,0);
		gui_circle(60,60,WHITE,2,0);
		gui_circle(90,25,WHITE,2,0);
		
                IntegerBezier(10,20, 40,25, 60,60, 90,25);

		wait_key();
	}
}
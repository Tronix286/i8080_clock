//#pragma printf = "%f %02ld %02lu %04lu"

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
#include "math.h"

u8 buf[40] = {0};
uint8_t scrachpad[10] = {0x87,1,0x4b,0x46,0x7f,0xff,0x0c,0x10,0xb7};
//uint8_t scrachpad[10] = {0x35,0xff,0xff,0xff,0x7f,0xff,0x0c,0x10,0x03}; //-12.7
uint8_t key;
uint16_t temp;

int32_t temperature;
uint32_t pressure, humidity;

unsigned char timeb[7] = {0x12,0x33,0x15,0x01,0x01,0x01,0x23};
unsigned char time_bcd[7];

float tmp_float;

void myftoa(x,f,str)
double x;       /* the number to be converted */
int f;          /* number of digits to follow decimal point */
char *str;      /* output string */
{
        double scale;           /* scale factor */
        int i,                          /* copy of f, and # digits before decimal point */
                d;                              /* a digit */

        if( x < 0.0 ) {
                *str++ = '-' ;
                x = -x ;
        }
        i = f ;
        scale = 2.0 ;
        while ( i-- )
                scale *= 10.0 ;
        x += 1.0 / scale ;
        /* count places before decimal & scale the number */
        i = 0 ;
        scale = 1.0 ;
        while ( x >= scale ) {
                scale *= 10.0 ;
                i++ ;
        }
	if ( i == 0 )
	    *str++ = '0';

        while ( i-- ) {
                /* output digits before decimal */
                scale = floor(0.5 + scale * 0.1 ) ;
                d = ( x / scale ) ;
                *str++ = d + '0' ;
                x -= (double)d * scale ;
        }
        if ( f <= 0 ) {
                *str = 0;
                return ;
        }
        *str++ = '.' ;
        while ( f-- ) {
                /* output digits after decimal */
                x *= 10.0 ;
                d = x;
                *str++ = d + '0' ;
                x -= d ;
        }
        *str = 0;
}

void main()
{

	i2c_init();
	ds_start_conversion();
	LCD_Init();

/*
	if (BME280_begin(MODE_NORMAL) == 0)
	{
		LCD_ShowString(50,15,16,"BME280 INIT ERROR!",0);
  		key = BME280_Read8(BME280_REG_CHIPID);
		memset(buf,0,40);
		sprintf((char *)buf,"addr = %02x , ID = %02x",BME280_I2C_ADDRESS,key);
		LCD_ShowString(50,15+17,16,buf,0);
		BME280_I2C_ADDRESS = 0xEE;
  		key = BME280_Read8(BME280_REG_CHIPID);
		memset(buf,0,40);
		sprintf((char *)buf,"addr = %02x , ID = %02x",BME280_I2C_ADDRESS,key);
		LCD_ShowString(50,15+17+17,16,buf,0);
		BME280_I2C_ADDRESS = 0xEC;
		msleep(1500);
	}
*/
	while(1)
	{
		LCD_Clear(BLACK);
   // Read temperature (in hundredths C), pressure (in Pa)
    // and humidity (in 1024 steps RH%) values from BME280 sensor
//    BME280_readTemperature(&temperature);  // read temperature
//    BME280_readHumidity(&humidity);        // read humidity
//    BME280_readPressure(&pressure);        // read pressure

  temperature = 2516;
  humidity = 65233;
  pressure = 123734;

		memset(buf,0,40);
		sprintf((char *)buf,"%02ld.%02ld C raw=%ld", temperature / 100, temperature % 100,temperature);
		LCD_ShowString(50,15,16,buf,0);
		memset(buf,0,40);
		sprintf((char *)buf,"%02lu.%02lu %%", humidity / 1024, ((humidity * 100)/1024) % 100);
		LCD_ShowString(50,15+17,16,buf,0);
		memset(buf,0,40);
		sprintf((char *)buf,"%04lu.%02lu hPa", pressure/100, pressure % 100);
		LCD_ShowString(50,15+17+17,16,buf,0);

		tmp_float = (float)(pressure/100)+(float)(pressure % 100)/100;

		memset(buf,0,40);
//		sprintf((char *)buf,"float=%f",1.25f);
		myftoa(tmp_float,2,buf);
		LCD_ShowString(50,15+17+17+17,16,buf,0);

		memset(buf,0,40);
		temp = ds_convert_temp();
		sprintf((char *)buf,"temp=%d.%d",(int8_t)(temp >> 8), temp & 0xf);
		LCD_ShowString(50,32+17+17+17,16,buf,0);

		if (temp & 0x8000) 
		 tmp_float = (float)(int8_t)(temp >> 8)-(float)(temp & 0xf)/10;
		else
		 tmp_float = (float)(int8_t)(temp >> 8)+(float)(temp & 0xf)/10;

		myftoa(tmp_float,2,buf);
		LCD_ShowString(50,15+17+17+17+17+17,16,buf,0);

		tmp_float = 12.5f;
		key = floor(tmp_float);
		memset(buf,0,40);
		sprintf((char *)buf,"%d",key);
		LCD_ShowString(50,15+17+17+17+17+17+17,16,buf,0);

/*		rtc_read_time(timeb);


	   	time_bcd[0] = timeb[2] & 0x3F; //hours
	   	time_bcd[1] = timeb[1] & 0x7F; //min
	   	time_bcd[2] = timeb[0] & 0x7F; //sec

		time_bcd[3] = timeb[3] & 0x3F; //days
		time_bcd[4] = timeb[4] & 0x07; //week days
		time_bcd[5] = timeb[5] & 0x1F; //month
		time_bcd[6] = timeb[6]; //years

		memset(buf,0,40);
		sprintf((char *)buf,"%x %x %x %x %x %x %x",time_bcd[0],time_bcd[1],time_bcd[2],time_bcd[3],time_bcd[4],time_bcd[5],time_bcd[6]);
		LCD_ShowString(50,15+17+17+17,16,buf,0);
*/
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

	}
/*
	while(1)
	{
		ds_start_conversion();
		LCD_Clear(BLACK);
		//ds_read_scrach(scrachpad);

		memset(buf,0,40);
		sprintf((char *)buf,"id=%x",ds_read_famcode());
		LCD_ShowString(50,15,16,buf,0);
		memset(buf,0,40);
		//settings_mode = ds_read_famcode();
		sprintf((char *)buf,"%x %x %x %x %x %x %x %x %x",scrachpad[0],scrachpad[1],scrachpad[2],scrachpad[3],scrachpad[4],scrachpad[5],scrachpad[6],scrachpad[7],scrachpad[8]);
		LCD_ShowString(50,32,16,buf,0);
		memset(buf,0,40);
		sprintf((char *)buf,"crc=%x",ds_calc_crc8());
		LCD_ShowString(50,32+17,16,buf,0);
		memset(buf,0,40);
		if (ds_calc_crc8() == scrachpad[8])
		{
		LCD_ShowString(50,32+17+17,16,"CRC OK",0);
		}
		else
		LCD_ShowString(50,32+17+17,16,"CRC ERROR!",0);
		memset(buf,0,40);
		temp = ds_convert_temp();
		sprintf((char *)buf,"temp=%d.%d",(int8_t)(temp >> 8), temp & 0xf);
		LCD_ShowString(50,32+17+17+17,16,buf,0);
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
	}

*/
}
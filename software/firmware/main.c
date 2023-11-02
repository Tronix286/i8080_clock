#include <stdlib.h>
#include "lcd.h"
#include "gui.h"
#include "test.h"
#include "cust_fnt.h"
#include "i2c.h"
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "pic.h"
#include "onewire.h"
#include "bme280.h"
#include "forecast.h"
#include "math.h"

//#define DBG 1

//uint32_t TProgmemRGBPalette16 = {0x3CB371, 0x7B68EE, 0x32CD32, 0x4169E1, 0x800080, 0xFF0000, 0xFF6347, 0xFFA500};

// Воскресенье Понедельник Вторник Среда Четверг Пятница Суббота
char DayOfWeek[][15] = {"Corkqfrfn}f", "Ponfefl}nik", "Csoqnik", "Rqfea","Xfscfqd","P!sniwa","Rtbbosa"};

extern TFTSTCustomFontData tftstFont_arialRegular_120;
extern TFTSTCustomFontData tftstFont_arialRegular_48;
extern TFTSTCustomFontData tftstFont_arialRegular_32;
u8 str_buf[20] = {0};	// string buffer
unsigned char timeb[7] = {0x12,0x33,0x15,0x01,0x01,0x01,0x23};
unsigned char time_bcd[7];
char lcd_time[5];
char prev_lcd_time[5];
char lcd_date[17] = {"\1DD\1.\1MM\1.\00120YY\0"};
unsigned char prev_lcd_date[5] = {0xFF,0xFF,0xFF,0xFF};
unsigned char old_sec;
uint8_t scrachpad[10] = {0x87,1,0x4b,0x46,0x7f,0xff,0x0c,0x10,0xb7}; //+24.4
//uint8_t scrachpad[10] = {0x35,0xff,0xff,0xff,0x7f,0xff,0x0c,0x10,0x03}; //-12.7
uint32_t total_seconds=0;     // global seconds counter
uint32_t btn3_time;	// button3 time
uint8_t settings_mode= 0; // button3 was entered

uint8_t key;

uint16_t curr_temp;	// current temperature
uint16_t old_temp;

int32_t temperature;
uint32_t pressure, humidity, old_pressure, old_humidity;

float tmp_float;
uint8_t read_sensors_mutex = 0;
uint8_t update_weather_mutex = 0;

uint8_t bcd_to_decimal(uint8_t x) {
    return x - 6 * (x >> 4);
}

uint8_t BCDDEC(uint8_t val) __z88dk_fastcall
{
   #asm
	mov a,l
	adi 99h
	daa
	mov l,a
   #endasm
}

uint8_t BCDINC(uint8_t val) __z88dk_fastcall
{ 
   #asm
	mov a,l
	adi 1h
	daa
	mov l,a
   #endasm
} 

uint8_t readkey() __z88dk_fastcall
{
	#asm
	mvi a,PIO_C_IN
	out PIO_M
	in PIO_C
	;sta _key
	mov l,a
	#endasm	
}


void drawHours(uint16_t col)
{
	tftstDrawCharWithFont(&tftstFont_arialRegular_120, 90, 0, lcd_time[0], col, CLOCK_BG);
	tftstDrawCharWithFont(&tftstFont_arialRegular_120, 90+61, 0, lcd_time[1], col, CLOCK_BG);
}

void drawMinutes(uint16_t col)
{
	tftstDrawCharWithFont(&tftstFont_arialRegular_120, 90+61+61+41, 0, lcd_time[3], col, CLOCK_BG);
	tftstDrawCharWithFont(&tftstFont_arialRegular_120, 90+61+61+41+61+1, 0, lcd_time[4], col, CLOCK_BG);
}

void drawDayOfWeek(uint16_t col)
{
	tftstDrawTextWithFont(&tftstFont_arialRegular_32, 20, 125, DayOfWeek[time_bcd[4]], col, CLOCK_BG);
}

void drawDate()
{
	tftstDrawTextWithFontMulti(&tftstFont_arialRegular_32, 320, 125, lcd_date, CLOCK_FG, CLOCK_SET, CLOCK_BG);
}

void readBMEsensor()
{
    	BME280_readTemperature(&temperature);  // read temperature
    	BME280_readHumidity(&humidity);        // read humidity
    	BME280_readPressure(&pressure);        // read pressure
}

void drawHumidity()
{
	memset(str_buf,0,20);
	sprintf((char *)str_buf,"%02lu.%lu%%", humidity / 1024, ((humidity * 10)/1024) % 10);
	tftstDrawTextWithFont(&tftstFont_arialRegular_48, 257-(tftstTextWithFontLength(&tftstFont_arialRegular_48,str_buf)/2)-20, 230, str_buf, CLOCK_FG, 0x18E3);
}

void drawPressure()
{
	memset(str_buf,0,20);
	//sprintf((char *)str_buf,"%04lu.%02lu", pressure/100, pressure % 100);
	sprintf((char *)str_buf,"%lu", (pressure*75)/10000);
	tftstDrawTextWithFont(&tftstFont_arialRegular_48, 398-(tftstTextWithFontLength(&tftstFont_arialRegular_48,str_buf)/2), 230, str_buf, CLOCK_FG, 0x18E3);
}

void drawWeather()
{
  uint8_t w_cast;

//		myftoa(getCast(),2,str_buf);
//		LCD_ShowString(20,60,16,str_buf,1);

	LCD_Fill(400,20,465,58,CLOCK_BG);
	memset(str_buf,0,20);
	w_cast = round(getCast());
	sprintf((char *)str_buf,"%d", w_cast);
	tftstDrawTextWithFont(&tftstFont_arialRegular_32, 400, 20, str_buf, CLOCK_FG, CLOCK_BG);
	switch (w_cast)
	{
	   case 0:
	   case 1:
		tftstDrawImg(gImage_sun, 20,20, CLOCK_FG, CLOCK_BG);
		break;
	   case 2:
	   case	3:
		tftstDrawImg(gImage_cloudy, 20,20, CLOCK_FG, CLOCK_BG);
		break;
	   case 4:
	   case	5:
		tftstDrawImg(gImage_cld_rain, 20,20, CLOCK_FG, CLOCK_BG);
		break;
	   case 6:
	   case	7:
		tftstDrawImg(gImage_rainy, 20,20, CLOCK_FG, CLOCK_BG);
		break;
	   case 8:
	   case	9:
		tftstDrawImg(gImage_storm, 20,20, CLOCK_FG, CLOCK_BG);
		break;
	   default:
		tftstDrawImg(gImage_storm, 20,20, CLOCK_FG, CLOCK_BG);
		break;
	}
}

void main ()
{
	i2c_init();
	rtc_init();

	ds_start_conversion();
	BME280_begin(MODE_NORMAL);

	LCD_Init();

	LCD_Clear(SCR_BG);

	fillRoundRect(10,10,460,160,15,CLOCK_BG);

//	fillRoundRect(10,180,225,130,15,0x18E3); //	0x18A4
//	fillRoundRect(245,180,225,130,15,0x18E3); //	0x18A4

	fillRoundRect(10,180,146,130,15,0x18E3); //	0x18A4
	fillRoundRect(167,180,146,130,15,0x18E3); //	0x18A4
	fillRoundRect(324,180,146,130,15,0x18E3); //	0x18A4

	tftstDrawImg(gImage_temperature, 74,190, CLOCK_FG, 0x18E3);
	tftstDrawImg(gImage_humidity,    225,190, CLOCK_FG, 0x18E3);
	tftstDrawImg(gImage_presure,     387,190, CLOCK_FG, 0x18E3);

//!!!
	ds_read_scrach(scrachpad);

	memset(str_buf,0,20);
	curr_temp = ds_convert_temp();
	old_temp = curr_temp;
	sprintf((char *)str_buf,(curr_temp & 0x8000) ? "%d.%d" : "+%d.%d",(int8_t)(curr_temp >> 8), curr_temp & 0xf);
	tftstDrawTextWithFont(&tftstFont_arialRegular_48, 68-(tftstTextWithFontLength(&tftstFont_arialRegular_48,str_buf)/2)+10, 230, str_buf, CLOCK_FG, 0x18E3);

	readBMEsensor();

//humidity = 65233;
//!!!
//pressure = 123734;
	old_humidity = humidity;
	old_pressure = pressure;

	drawHumidity();

	drawPressure();

	// установить высоту над уровнем моря (Москва 255м)
  	setH(255);

	// если есть RTC - можно установить месяц 1-12
	rtc_read_time(timeb);
	setMonth(bcd_to_decimal(timeb[5] & 0x1F));

//!!!
//memset(str_buf,0,20);
//sprintf((char *)str_buf,"%d",bcd_to_decimal(timeb[5] & 0x1F));
//LCD_ShowString(20,80,16,str_buf,0);

	if (curr_temp & 0x8000) 
		tmp_float = (float)(int8_t)(curr_temp >> 8)-(float)(curr_temp & 0xf)/10;
	else
		tmp_float = (float)(int8_t)(curr_temp >> 8)+(float)(curr_temp & 0xf)/10;

	addP(pressure,tmp_float);

//!!!
//myftoa(tmp_float,2,str_buf);
//LCD_ShowString(20,100,16,str_buf,0);

	drawWeather();

	while (1)
	{
		rtc_read_time(timeb);


	   	time_bcd[0] = timeb[2] & 0x3F; //hours
	   	time_bcd[1] = timeb[1] & 0x7F; //min
	   	time_bcd[2] = timeb[0] & 0x7F; //sec

		time_bcd[3] = timeb[3] & 0x3F; //days
		time_bcd[4] = timeb[4] & 0x07; //week days
		time_bcd[5] = timeb[5] & 0x1F; //month
		time_bcd[6] = timeb[6]; //years


		lcd_time[0] = ((time_bcd[0] & 0xF0) >>4) + 0x30;
		lcd_time[1] = (time_bcd[0]&0x0F) + 0x30;
		lcd_time[2] = ':';
		lcd_time[3] = ((time_bcd[1] & 0xF0) >>4) + 0x30;
		lcd_time[4] = (time_bcd[1]&0x0F) + 0x30;
		

		lcd_date[1] = ((time_bcd[3] & 0xF0) >>4) + 0x30;
		lcd_date[2] = (time_bcd[3]&0x0F) + 0x30;

		lcd_date[6] = ((time_bcd[5] & 0xF0) >>4) + 0x30;
		lcd_date[7] = (time_bcd[5]&0x0F) + 0x30;

		lcd_date[13] = ((time_bcd[6] & 0xF0) >>4) + 0x30;
		lcd_date[14] = (time_bcd[6]&0x0F) + 0x30;

		//max width 60, so 61+61+41+61+61=285. (480-285)/2=97

		// dispatcher
		if (time_bcd[2] == 0x10) 	// 10 sec
		   ds_start_conversion();

		if ((time_bcd[2] == 0x20) && (read_sensors_mutex == 0))	// 20 sec
		{
			read_sensors_mutex = 1;
			ds_read_scrach(scrachpad);
			if (ds_calc_crc8() == scrachpad[8])
			{
			   curr_temp = ds_convert_temp();
			   if (old_temp != curr_temp)
			   {
			   	memset(str_buf,0,20);
			   	sprintf((char *)str_buf,(curr_temp & 0x8000) ? "%d.%d" : "+%d.%d",(int8_t)(curr_temp >> 8), curr_temp & 0xf);
			   	LCD_Fill(12,225,155,292,0x18E3);
			   	tftstDrawTextWithFont(&tftstFont_arialRegular_48, 68-(tftstTextWithFontLength(&tftstFont_arialRegular_48,str_buf)/2)+10, 230, str_buf, CLOCK_FG, 0x18E3);
				old_temp = curr_temp;
			   }
			}
			readBMEsensor();
			if (humidity != old_humidity)
			{
			   	LCD_Fill(169,225,310,292,0x18E3);
				drawHumidity();
				old_humidity = humidity;
			}
			if (pressure != old_pressure)
			{
			   	LCD_Fill(326,225,468,292,0x18E3);
				drawPressure();
				old_pressure = pressure;
			}
		}

		if (time_bcd[2] == 0x21) 	// 21 sec
		   read_sensors_mutex = 0;

		if ( ((time_bcd[1] == 0x30) || (time_bcd[1] == 0x00)) && update_weather_mutex == 0) // every 30 min
		{
			update_weather_mutex = 1;
			if (curr_temp & 0x8000) 
			  tmp_float = (float)(int8_t)(curr_temp >> 8)-(float)(curr_temp & 0xf)/10;
			else
			  tmp_float = (float)(int8_t)(curr_temp >> 8)+(float)(curr_temp & 0xf)/10;

			addP(pressure,tmp_float);
			drawWeather();
		}
		
		if ((time_bcd[1] == 0x31) || (time_bcd[1] == 0x01))
		   update_weather_mutex = 0;

		if (prev_lcd_date[1] != time_bcd[4])
		{
			LCD_Fill(20,132,210,162,CLOCK_BG);
			drawDayOfWeek((settings_mode ==3) ? CLOCK_SET : CLOCK_FG);
			prev_lcd_date[1] = time_bcd[4];
		}

		if ((prev_lcd_date[0] != time_bcd[3]) || (prev_lcd_date[2] != time_bcd[5]) || (prev_lcd_date[3] != time_bcd[6]))
		{
			LCD_Fill(320,132,467,158,CLOCK_BG);
			drawDate();
			prev_lcd_date[0] = time_bcd[3];
			prev_lcd_date[2] = time_bcd[5];
			prev_lcd_date[3] = time_bcd[6];
		}


		if ((prev_lcd_time[0] != lcd_time[0]))
		{
		    #ifdef DBG
		    LCD_Fill(90,93,97+60,183,GREEN);
		    #else
		    LCD_Fill(90,33,97+60,123,CLOCK_BG);
		    #endif
		    tftstDrawCharWithFont(&tftstFont_arialRegular_120, 90, 0, lcd_time[0], (settings_mode ==1) ? CLOCK_SET : CLOCK_FG, CLOCK_BG);
		    prev_lcd_time[0] = lcd_time[0];
	 	}

		if (prev_lcd_time[1] != lcd_time[1])
		{
		    #ifdef DBG
		    LCD_Fill(90+61,93,97+61+61,183,GREEN);
		    #else
		    LCD_Fill(90+61,33,97+61+61,123,CLOCK_BG);
		    #endif
		    tftstDrawCharWithFont(&tftstFont_arialRegular_120, 90+61, 0, lcd_time[1], (settings_mode ==1) ? CLOCK_SET : CLOCK_FG, CLOCK_BG);
		    prev_lcd_time[1] = lcd_time[1];
	 	}
		if (old_sec != time_bcd[2])
		{
		    total_seconds++;
		    if (time_bcd[2] % 2 ==0)
		    #ifdef DBG
		    LCD_Fill(90+61+61+1,93,97+61+61+40,183,BLUE);
		    #else
		    LCD_Fill(90+61+61+1,33,90+61+61+40,123,CLOCK_BG);
		    #endif
		    else
		    tftstDrawCharWithFont(&tftstFont_arialRegular_120, 90+61+61+1, 0, lcd_time[2], CLOCK_FG, CLOCK_BG);
		    old_sec = time_bcd[2];
	 	}

		if (prev_lcd_time[3] != lcd_time[3])
		{
		    #ifdef DBG
		    LCD_Fill(90+61+61+41,43,97+61+61+41+61,183,RED);
		    #else
		    LCD_Fill(90+61+61+41,33,97+61+61+41+61,123,CLOCK_BG);
		    #endif
		    tftstDrawCharWithFont(&tftstFont_arialRegular_120, 90+61+61+41, 0, lcd_time[3], (settings_mode ==2) ? CLOCK_SET : CLOCK_FG, CLOCK_BG);
		    prev_lcd_time[3] = lcd_time[3];
	 	}

		if (prev_lcd_time[4] != lcd_time[4])
		{
		    #ifdef DBG
		    LCD_Fill(90+61+61+41+61+1,93,97+61+61+41+61+61+1,183,CYAN);
		    #else
		    LCD_Fill(90+61+61+41+61+1,33,97+61+61+41+61+61+1,123,CLOCK_BG);
		    #endif
		    tftstDrawCharWithFont(&tftstFont_arialRegular_120, 90+61+61+41+61+1, 0, lcd_time[4], (settings_mode ==2) ? CLOCK_SET : CLOCK_FG, CLOCK_BG);
		    prev_lcd_time[4] = lcd_time[4];
	 	}


		key = readkey();

		if (!(key & 8))		// button 3 (settings)
		{
		 t_delay(1);
		 if (!(readkey() & 8))
	 	  {
		   while (!(readkey() & 8));
		   btn3_time = total_seconds;
		   if (settings_mode == 0)
		   {
		     settings_mode = 1;
		   } else
		   {
		     if (settings_mode == 1) {
			drawHours(CLOCK_FG);
		     } else
		     if (settings_mode == 2) {
			drawMinutes(CLOCK_FG);
		     } else
		     if (settings_mode == 3) {
			drawDayOfWeek(CLOCK_FG);
		     } else
		     if (settings_mode == 4) {
			lcd_date[0] = 1;
			drawDate();
		     } else
		     if (settings_mode == 5) {
			lcd_date[5] = 1;
			drawDate();
		     } else
		     if (settings_mode == 6) {
			lcd_date[10] = 1;
			drawDate();
		     }
		     settings_mode++;
		     if (settings_mode > 6) settings_mode = 1;
		   }

		   if (settings_mode == 1) {
		    drawHours(CLOCK_SET);
		   } else
		   if (settings_mode == 2) {
		    drawMinutes(CLOCK_SET);
		   } else
		   if (settings_mode == 3) {
		    drawDayOfWeek(CLOCK_SET);
		   } else
		   if (settings_mode == 4) {
			lcd_date[0] = 2;
			drawDate();
		   } else
		   if (settings_mode == 5) {
			lcd_date[5] = 2;
			drawDate();
		   } else
		   if (settings_mode == 6) {
			lcd_date[10] = 2;
			drawDate();
		   }
		
		  }
		}

		if (settings_mode != 0)
		{
		if (!(key & 4))		// button 2 (minus)
		{
		 t_delay(1);
		 if (!(readkey() & 4))
	 	  {
		   while (!(readkey() & 4));
		   btn3_time = total_seconds;
		   switch (settings_mode)
		   {
			case 1:	//hr
	    	   		if (time_bcd[0] > 0) 
				  time_bcd[0] = BCDDEC(time_bcd[0]);
	    	   		else
	       	   		  time_bcd[0] = 0x23;
				break;
			case 2:	//min
	    	   		if (time_bcd[1] > 0) 
				  time_bcd[1] = BCDDEC(time_bcd[1]);
	    	   		else
	       	   		  time_bcd[1] = 0x59;
				break;
			case 3:	//week days
	    	   		if (time_bcd[4] > 0) 
				  time_bcd[4]--;
	    	   		else
	       	   		  time_bcd[4] = 0x06;
				break;
			case 4:	//days
	    	   		if (time_bcd[3] > 1) 
				  time_bcd[3] = BCDDEC(time_bcd[3]);
	    	   		else
	       	   		  time_bcd[3] = 0x31;
				break;
			case 5:	//month
	    	   		if (time_bcd[5] > 1) 
				  time_bcd[5] = BCDDEC(time_bcd[5]);
	    	   		else
	       	   		  time_bcd[5] = 0x12;
				break;
			case 6:	//years
	    	   		if (time_bcd[6] > 1) 
				  time_bcd[6] = BCDDEC(time_bcd[6]);
	    	   		else
	       	   		  time_bcd[6] = 0x99;
		//memset(buf,0,10);
		//sprintf((char *)buf,"tbcd 0x%02x,prev 0x%02x",time_bcd[6],prev_lcd_date[3]);
		//LCD_ShowString(50,15,16,buf,0);
				break;
		   }
	    	   rtc_read_time(timeb);
	           timeb[2] = time_bcd[0];
	    	   timeb[1] = time_bcd[1]; 
		   timeb[3] = time_bcd[3];
		   timeb[4] = time_bcd[4];
		   timeb[5] = time_bcd[5];
		   timeb[6] = time_bcd[6];
	    	   rtc_write_time(timeb);
		  }
		} else
		if (!(key & 2))		// button 1 (plus)
		{
		 t_delay(1);
		 if (!(readkey() & 2))
	 	  {
		   //Show_Str(120,25,WHITE,BLACK,"HOUR BTN",16,0);		
		   while (!(readkey() & 2));
		   btn3_time = total_seconds;
		   switch (settings_mode)
		   {
			case 1:	//hr
	    	   		if (time_bcd[0] < 0x23) 
				  time_bcd[0] = BCDINC(time_bcd[0]);
	    	   		else
	       	   		  time_bcd[0] = 0;
				break;
			case 2:	//min
	    	   		if (time_bcd[1] < 0x59) 
				  time_bcd[1] = BCDINC(time_bcd[1]);
	    	   		else
	       	   		  time_bcd[1] = 0;
				break;
			case 3:	//week days
	    	   		if (time_bcd[4] < 6) 
				  time_bcd[4]++;
	    	   		else
	       	   		  time_bcd[4] = 0x00;
				break;
			case 4:	//days
	    	   		if (time_bcd[3] < 0x31) 
				  time_bcd[3] = BCDINC(time_bcd[3]);
	    	   		else
	       	   		  time_bcd[3] = 1;
				break;
			case 5:	//month
	    	   		if (time_bcd[5] < 0x12) 
				  time_bcd[5] = BCDINC(time_bcd[5]);
	    	   		else
	       	   		  time_bcd[5] = 1;
				break;
			case 6:	//years
	    	   		if (time_bcd[6] < 0x99) 
				  time_bcd[6] = BCDINC(time_bcd[6]);
	    	   		else
	       	   		  time_bcd[6] = 1;
				break;
		   }
	    	   rtc_read_time(timeb);
	    	   timeb[2] = time_bcd[0];
	    	   timeb[1] = time_bcd[1]; 
		   timeb[3] = time_bcd[3];
		   timeb[4] = time_bcd[4];
		   timeb[5] = time_bcd[5];
		   timeb[6] = time_bcd[6];
	           rtc_write_time(timeb);
		  }
		}
		//sleep(1);
		}

		if ((total_seconds - btn3_time) > 5) 
		{
		  if (settings_mode == 1) {
		    drawHours(CLOCK_FG);
		  } else
		  if (settings_mode == 2) {
		    drawMinutes(CLOCK_FG);
		  } else
		  if (settings_mode == 3)
                  {
		    drawDayOfWeek(CLOCK_FG);
		  } else
		  if ((settings_mode == 4) || (settings_mode == 5) || (settings_mode == 6))
		  {
		    lcd_date[0] = 1;
		    lcd_date[5] = 1;
		    lcd_date[10] = 1;
		    drawDate();
		  }
		  settings_mode = 0;
		}

	} // main loop
}


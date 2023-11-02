/*
 * Copyright (c) 2009, Zilogic Systems <code@zilogic.com>
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose with or without fee is hereby granted, provided
 * that the above copyright notice and this permission notice appear
 * in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "i2c.h"
#include "main.h"

#ifdef EMU
__sfr __at 0x30 pcf1;
__sfr __at 0x31 pcf2;
__sfr __at 0x32 pcf3;
__sfr __at 0x33 pcf4;
__sfr __at 0x34 pcf5;
__sfr __at 0x35 pcf6;
__sfr __at 0x36 pcf7;
#endif

#define SCL_bit	4h		// SCL port PC.4
#define SDA_bit	0h		// SDA port PC.0

uint8_t SDA;

void sclset() __naked
{
	#asm
	 ; SCL HI without changing SDA     	
	mvi a,00001001b	; set bit
	out PIO_M
        ret
	#endasm
}

void sclclr() __naked
{
	#asm
	  ; SCL LO without changing SDA       	
	mvi a,00001000b	; clear bit
	out PIO_M
        ret
	#endasm
}

void sdaset() __naked
{
	#asm
	; SDA HI without changing SCL
	mvi a,00000001b	; set bit
	out PIO_M
        ret
	#endasm
}
void sdaclr() __naked
{
	#asm
	 ; SDA LO without changing SCL   	
	mvi a,00000000b	; clear bit
	out PIO_M
        ret
	#endasm
}

// I2C functions
void i2c_init() __naked
{
	#asm
	mvi A,PIO_C_IN
	out PIO_M
	mvi a,00001001b	; set bit
	out PIO_M
	ret
	#endasm
}


/* START condition */
void i2c_start(void)
{
	#asm
	mvi a,PIO_C_OUT
	out PIO_M		
	#endasm
	sdaset();
	sclset();
	sdaclr();
	sclclr();
	sdaset();
	
}

/* Issuing a STOP condition */

void i2c_stop(void)
{
	#asm
	mvi a,PIO_C_OUT
	out PIO_M		
	#endasm
        sdaclr();
        sclset();
        sdaset();

}

/* Clock pulse generation. The function returns data or acknowledgment
   bit */

uint8_t i2c_clock_read(void)
{
	#asm
	call _sclset
	in PIO_C			; SDA (RX data bit) is in A.0
	ani 1
	sta _SDA
	call _sclclr
	#endasm
	return(SDA);
}

void i2c_clock(void)
{
	sclset();
	sclclr();
}

/* Writing a byte to a slave, with most significant bit first. The
   function returns acknowledgment bit. */

uint8_t i2c_write(uchar byte)
{
	uchar mask = 0x80;
	uint8_t t_ret;

	while (mask) {
		if (byte & mask)
			sdaset();
		else
			sdaclr();
		i2c_clock();
		mask >>= 1;
	}
	sdaset();
		#asm
		mvi a,PIO_C_IN       		; SDA - in, SCL - out
		out PIO_M   		;
		#endasm
	t_ret = i2c_clock_read();
		#asm
		mvi a,PIO_C_OUT       		; SDA - out, SCL - out
		out PIO_M   		;
		#endasm
	return (t_ret);
}

/* Reading byte from a slave, with most significant bit
   first. The parameter indicates, whether to acknowledge
   (1) or not (0) */

uchar i2c_read(uint8_t acknowledgment)
{
	uchar mask = 0x80, byte = 0x00;

		#asm
		mvi a,PIO_C_IN       		; SDA - in, SCL - out
		out PIO_M   		;
		#endasm

	while (mask) {
		if (i2c_clock_read())
			byte |= mask;
		mask >>= 1;	/* next bit to receive */
	}

		#asm
		mvi a,PIO_C_OUT       		; SDA - out, SCL - out
		out PIO_M   		;
		#endasm
	if (acknowledgment) {
		sdaclr();
		i2c_clock();
		sdaset();
	} else {
		sdaset();
		i2c_clock();
	}
	return (byte);
}

void rtc_init()
{
   	i2c_start();
	if (!i2c_write(PCF8563))
		if (!i2c_write(0x0D))
		if (!i2c_write(0x00))
	i2c_stop();
}

/* PCF8563 RTC functions */
void rtc_read_time(unsigned char timebuf[])
{
	i2c_start();
	if (!i2c_write(PCF8563))
		if (!i2c_write(0x02))
			i2c_start();
	if (!i2c_write(PCF8563 | 0x01)) {
		timebuf[0] = i2c_read(1); //sec
		timebuf[1] = i2c_read(1); //min
		timebuf[2] = i2c_read(1); //hr
		timebuf[3] = i2c_read(1); //days
		timebuf[4] = i2c_read(1); //weekdays
		timebuf[5] = i2c_read(1); //month
		timebuf[6] = i2c_read(0); //years
	}
	i2c_stop();
#ifdef EMU
		timebuf[0] = pcf1; //sec
		timebuf[1] = pcf2; //min
		timebuf[2] = pcf3; //hr
		timebuf[3] = pcf4; //days
		timebuf[4] = pcf5; //weekdays
		timebuf[5] = pcf6; //month
		timebuf[6] = pcf7; //years
#endif
}

void rtc_write_time(const char *time)
{
	/* set time */
	if (!rtc_write(0x02, time[0]))	/* set seconds */
		goto FAIL;

	if (!rtc_write(0x03, time[1]))	/* set minutes */
		goto FAIL;

	if (!rtc_write(0x04, time[2])) 	/* set hours */
		goto FAIL;

	if (!rtc_write(0x05, time[3])) 	//days
		goto FAIL;
	if (!rtc_write(0x06, time[4])) 	//weekdays
		goto FAIL;
	if (!rtc_write(0x07, time[5])) 	//month
		goto FAIL;
	if (!rtc_write(0x08, time[6])) 	//years
		goto FAIL;

	return;
 FAIL:
#ifdef EMU
		pcf1 = time[0]; //sec
		pcf2 = time[1]; //min
		pcf3 = time[2]; //hr
		pcf4 = time[3]; //days
		pcf5 = time[4]; //wdays
		pcf6 = time[5]; //month
		pcf7 = time[6]; //years
#endif
	return;
}

unsigned char rtc_read(unsigned char addr)
{
	unsigned char status = 0;
	i2c_start();
	if (!i2c_write(PCF8563))
		if (!i2c_write(addr))
			i2c_start();
	if (!i2c_write(PCF8563 | 0x01))
		status = i2c_read(0);
	i2c_stop();
	return status;
}

unsigned char rtc_write(unsigned char addr, unsigned char value)
{
	unsigned char status = 0;
	i2c_start();
	if (!i2c_write(PCF8563))
		if (!i2c_write(addr))
			if (!i2c_write(value))
				status = 1;
	i2c_stop();
	return status;
}

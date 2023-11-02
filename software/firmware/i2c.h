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

#ifndef _I2C_H
#define _I2C_H

#include <stdlib.h>

/* I2C functions */
void i2c_init();
void i2c_start(void);
void i2c_stop(void);
void i2c_clock(void);
uint8_t i2c_clock_read(void);
uint8_t i2c_write(unsigned char byte);
unsigned char i2c_read(uint8_t acknowledgment);

/* RTC functions */
unsigned char rtc_read(unsigned char addr);
unsigned char rtc_write(unsigned char addr, unsigned char value);
void rtc_read_time(unsigned char timebuf[]);
void rtc_write_time(const char *);
void rtc_init();

#define  uchar   unsigned char
#define  PCF8951 0x90		/* slave address of DTOA data direction
				 * bit = 0  */
#define  PCF8563 0xA2
#define  NO_ACK  0		/* lack of acknowledgment */
#define  ACK     1		/* acknowledgment */

#endif				/* _I2C_H  */

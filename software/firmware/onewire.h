#ifndef _ONEWIRE_H
#define _ONEWIRE_H

#include "main.h"
#include <stdlib.h>

#define OW_Bit 0

//void ow_init(void);
//void ow_write(uint8_t data);
//uint8_t ow_read();
unsigned char ds_read_famcode(void);
void ds_start_conversion(void);
void ds_read_scrach(unsigned char * buff);
unsigned char ds_calc_crc8();
uint16_t ds_convert_temp();
#endif
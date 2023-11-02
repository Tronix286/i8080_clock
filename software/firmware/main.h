#ifndef __MAIN_H
#define __MAIN_H 	   

//#define EMU 1

#define PIO_A		00h		// 8255 base address 	  (port A)
#define PIO_B		01h		// 8255 base address + 1 (port B)
#define PIO_C		02h		// 8255 base address + 2 (fport C)
#define PIO_M		03h		// 8255 control register

#define PIO_A_OUT	82h
#define PIO_C_OUT       82h
#define PIO_C_IN	83h

#define PIO_B_IN	83h
#define PIO_B_OUT	81h

// Pallete
#define SCR_BG 0x3186 //0x528A	
#define CLOCK_BG 0x0366 //0x3D8E
#define CLOCK_FG 0xD7FF //0xFF5F
#define CLOCK_SET 0xF58B


#endif

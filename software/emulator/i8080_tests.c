// This file uses the 8080 emulator to run the test suite (roms in cpu_tests
// directory). It uses a simple array as memory.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "i8080.h"
#include <assert.h>

//#define SDL_MAIN_HANDLED 1
//#define NO_STDIO_REDIRECT
#include "SDL.h"

// memory callbacks
#define MEMORY_SIZE 0x10000

// 3,5MHz
#define CLOCK_SPEED 3555555LU
#define W            480
#define H            320

enum ILI_CMD {
	CMD_UNK,
	START_XY,
	END_XY,
	PREP_WR
};

SDL_Surface *screen;

static uint8_t loc_time[7] = {0};

typedef unsigned short pixel;
pixel *screenPixels;

static uint16_t wnd_sx,wnd_sy,wnd_ex,wnd_ey;
static uint16_t dx,dy;
static uint16_t color;

static uint32_t current_time = 0;
static uint32_t last_time = 0;
static uint32_t dt = 0;

static uint8_t* memory = NULL;
static bool test_finished = 0;

static uint32_t cur_ili_cmd = CMD_UNK;
static uint8_t prev_ili_data = 0;
static uint32_t data_index = 0;

static uint8_t button = 0xff;

static long nb_instructions = 0;
static long long nb_cyc = 0;

static time_t rawtime;
static struct tm * timeinfo;

static uint8_t rb(void* userdata, uint16_t addr) {
  return memory[addr];
}

static void wb(void* userdata, uint16_t addr, uint8_t val) {
  if (addr < 0x8000)
	printf("Write to EEPROM!!\r\n");
  else
  	memory[addr] = val;
}

uint8_t DecimalToBCD (uint8_t decimalByte)
{
  return (((decimalByte / 10) << 4) | (decimalByte % 10));
}

static inline int bcd_decimal(uint8_t hex)
{
  return ((hex / 16 * 10) + (hex % 16));
}       

// Function to be called after a certain time

Uint32 t_callback(Uint32 interval, void * param) 
{
	if (loc_time[0] < 59) loc_time[0]++;
	else 
	{
	  loc_time[0] = 0;
	  if (loc_time[1] < 59) loc_time[1]++;
	  else
	  {
		loc_time[1] = 0;
		if (loc_time[2] < 23) loc_time[2]++;
		else
		{
		    loc_time[2] = 0;
		}
  	  }
	}
	
    return interval;
}


static uint8_t port_in(void* userdata, uint8_t port) {
  uint8_t ret;
  ret = 0xff;

  if (port == 0x30) ret = DecimalToBCD(loc_time[0]);
  else
  if (port == 0x31) ret = DecimalToBCD(loc_time[1]);
  else
  if (port == 0x32) ret = DecimalToBCD(loc_time[2]);
  else
  if (port == 0x33) ret = DecimalToBCD(loc_time[3]);
  else
  if (port == 0x34) ret = DecimalToBCD(loc_time[4]);
  else
  if (port == 0x35) ret = DecimalToBCD(loc_time[5]);
  else
  if (port == 0x36){ ret = DecimalToBCD(loc_time[6]);
//	printf("read year = %x, bcd = %x\r\n",loc_time[6],DecimalToBCD(loc_time[6]));
  }
  else
  if (port == 0x02) ret = button;

  return ret;
}

static void port_out(void* userdata, uint8_t port, uint8_t value) {
  i8080* const c = (i8080*) userdata;


  time_t rawtime;
  struct tm * timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  if (port == 0x10) {   // CMD
	switch (value)
	{
		case 0x2a: cur_ili_cmd = START_XY;
			   data_index = 0;
				break;
		case 0x2b: cur_ili_cmd = END_XY;
			   data_index = 0;
				break;
		case 0x2c: cur_ili_cmd = PREP_WR;
			   data_index = 0;
			   dx = wnd_sx;
			   dy = wnd_sy;
				break;
		default: cur_ili_cmd = CMD_UNK;
			 data_index = 0; 
			break;
	}
	//printf("CMD  =  %x\r\n",value);
  } else
  if (port == 0x11) {
	switch (cur_ili_cmd)
	{
		case START_XY:
			data_index++;
			if (data_index == 1) wnd_sx = value << 8;
			else if (data_index == 2) wnd_sx = wnd_sx | value;
			else if (data_index == 3) wnd_ex = value << 8;
			else if (data_index == 4) 
 				{
					wnd_ex = wnd_ex | value;
					cur_ili_cmd = CMD_UNK;
					//printf("startx = %u, endx = %u\r\n",wnd_sx,wnd_ex);
				}
			break;
		case END_XY:
			data_index++;
			if (data_index == 1) wnd_sy = value << 8;
			else if (data_index == 2) wnd_sy = wnd_sy | value;
			else if (data_index == 3) wnd_ey = value << 8;
			else if (data_index == 4) 
 				{
					wnd_ey = wnd_ey | value;
					cur_ili_cmd = CMD_UNK;
					//printf("starty = %u, endy = %u\r\n",wnd_sy,wnd_ey);
				}
			break;
		case PREP_WR:
			data_index++;
			if (data_index % 2==0)
			{
			  color = color | value;
			  int idx;
			  idx = (dx+W*dy);
			  if (idx < W*H) 
				screenPixels[idx] = color;
			  dx++;
			  if (dx > wnd_ex) {
				dx = wnd_sx;
				dy++;
			  }
			}
			else
				color = value << 8;;
			break;
	}
//	printf("DATA =  %x\r\n",value);
  } else 
  if (port == 0x31) {
	loc_time[1] = bcd_decimal(value);
  } else
  if (port == 0x32) {
	loc_time[2] = bcd_decimal(value);
  } else
  if (port == 0x33) {
	loc_time[3] = bcd_decimal(value);
  } else
  if (port == 0x34) {
	loc_time[4] = bcd_decimal(value);
  } else
  if (port == 0x35) {
	loc_time[5] = bcd_decimal(value);
  } else
  if (port == 0x36) {
	loc_time[6] = bcd_decimal(value);
	printf("set year: %d, bcdval = %x\r\n",value,bcd_decimal(value));
  } else
  if (port == 0x20) {
  	i8080_debug_output(c, true);

          uint16_t Addr;
	  uint8_t I,J;
/*
	  //Addr = ((c->h << 8) | c->l) - 1;
	  Addr = c->sp;
          printf("\n");
          for(J=0;J<16;J++)
          {
            printf("%04X: ",Addr);
            for(I=0;I<16;I++,Addr++)
              printf("%02X ",memory[Addr]);
            printf(" | ");Addr-=16;
            for(I=0;I<16;I++,Addr++)
              printf("%c",isprint(memory[Addr])? memory[Addr]:'.');
            printf("\n");
          }
*/
  } else
  if (port == 0x21) {
	nb_instructions = 0;
	nb_cyc = c->cyc;
  } else
  if (port == 0x22) {
        long long diff = c->cyc - nb_cyc;
  	printf("\n*** %lu instructions executed on %lld cycles\n\n",
               nb_instructions, diff);
  }
/*else
  if (port == 0) {
    test_finished = 1;
  } else if (port == 1) {
    uint8_t operation = c->c;

    if (operation == 2) { // print a character stored in E
      printf("%c", c->e);
    } else if (operation == 9) { // print from memory at (DE) until '$' char
      uint16_t addr = (c->d << 8) | c->e;
      do {
        printf("%c", rb(c, addr++));
      } while (rb(c, addr) != '$');
    }
  }
*/
}

static inline int load_file(const char* filename, uint16_t addr) {
  FILE* f = fopen(filename, "rb");
  if (f == NULL) {
    fprintf(stderr, "error: can't open file '%s'.\n", filename);
    return 1;
  }

  // file size check:
  fseek(f, 0, SEEK_END);
  size_t file_size = ftell(f);
  rewind(f);

  if (file_size + addr >= MEMORY_SIZE) {
    fprintf(stderr, "error: file %s can't fit in memory.\n", filename);
    return 1;
  }

  // copying the bytes in memory:
  size_t result = fread(&memory[addr], sizeof(uint8_t), file_size, f);
  if (result != file_size) {
    fprintf(stderr, "error: while reading file '%s'\n", filename);
    return 1;
  }

  fclose(f);
  return 0;
}

static inline void run_test(
    i8080* const c, const char* filename, unsigned long cyc_expected) {

	SDL_Event event;

  i8080_init(c);
  c->userdata = c;
  c->read_byte = rb;
  c->write_byte = wb;
  c->port_in = port_in;
  c->port_out = port_out;
  memset(memory, 0, MEMORY_SIZE);

  if (load_file(filename, 0x00) != 0) {
    return;
  }
  printf("*** TEST: %s\n", filename);

  c->pc = 0x00;

  int count = 0;
  test_finished = 0;
  while (!test_finished) {

    // uncomment following line to have a debug output of machine state
    // warning: will output multiple GB of data for the whole test suite
    // i8080_debug_output(c, false);

    //i8080_step(c);
  current_time = SDL_GetTicks();
  dt = current_time - last_time;

    while (count < (int)(dt * CLOCK_SPEED / 1000)) {
    	int cyc = c->cyc;
    	i8080_step(c);
	nb_instructions += 1;
    	int elapsed = c->cyc - cyc;
    	count += elapsed;
    }

    if (c->sp < 0x9000) i8080_debug_output(c, true);
    count = 0;
    last_time = current_time;
		SDL_UpdateRect(screen, 0, 0, 0, 0);
		while (SDL_PollEvent(&event)) 
		{	
			if (event.type == SDL_QUIT) {test_finished=1;}
					
			if (event.type == SDL_KEYDOWN || event.type == SDL_JOYBUTTONDOWN) {
						
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					test_finished=1;
				} 
				if (event.key.keysym.sym == SDLK_1) {
					button=button & 0xFD;
				} 
				if (event.key.keysym.sym == SDLK_2) {
					button=button & 0xFB;
				} 
				if (event.key.keysym.sym == SDLK_3) {
					button=button & 0xF7;
				} 
			}
			if (event.type == SDL_KEYUP || event.type == SDL_JOYBUTTONUP) {
						
				if (event.key.keysym.sym == SDLK_1) {
					button=button | 2;
				} 
				if (event.key.keysym.sym == SDLK_2) {
					button=button | 4;
				} 
				if (event.key.keysym.sym == SDLK_3) {
					button=button | 8;
				} 
			}
		}

  }

}

int main(int argc, char *argv[]) {
  memory = malloc(MEMORY_SIZE);
  if (memory == NULL) {
    return 1;
  }

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

  	time(&rawtime);
  	timeinfo = localtime(&rawtime);
	loc_time[0] = timeinfo->tm_sec;
	loc_time[1] = timeinfo->tm_min;
	loc_time[2] = timeinfo->tm_hour;
	loc_time[3] = timeinfo->tm_mday;
	loc_time[4] = timeinfo->tm_wday;
	loc_time[5] = timeinfo->tm_mon+1;
	loc_time[6] = (timeinfo->tm_year+1900)-2000;
	printf("ym_info.tm_year = %d, year = %d\r\n",timeinfo->tm_year,loc_time[6]);

	SDL_Event event;
	screen = SDL_SetVideoMode(W, H, 2*8, SDL_SWSURFACE);
	if ( screen == NULL )
	{
		fprintf ( stderr , "Error\n" );
		exit (1);
	}
	SDL_WM_SetCaption("8080 clock emu v1.0 (c) Tronix 2023", "8080_clock_emu");

	SDL_TimerID timerID = SDL_AddTimer(1000, t_callback, NULL);

	screenPixels = (pixel *)screen->pixels;
	for (int i=0 ; i<H*W ; i++ )
	{
		if (i%2==0)screenPixels[i] = 0xFFFF;
		else screenPixels[i] = 0x0000;
	}

  i8080 cpu;
  run_test(&cpu, "../full.bin", 23803381171LU);

  free(memory);
  SDL_Quit();

  return 0;
}

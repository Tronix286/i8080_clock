#include "main.h"
#include "onewire.h"

extern unsigned char scrachpad[10];

void ow_init() __z88dk_fastcall
{
	#asm
	mvi a,PIO_B_IN
	out PIO_M

	mvi a,PIO_B_OUT
	out PIO_M

	mvi a,OW_Bit	; clear bit
	out PIO_B

	mvi b,47*3
w_lp1:	dcr b
	jnz w_lp1

	mvi a,PIO_B_IN
	out PIO_M

	mvi b,23*3
w_lp2:	dcr b
	jnz w_lp2

	in PIO_B
	ani 1

	mvi b,20*3
w_lp6:	dcr b
	jnz w_lp6
	#endasm
}

void ow_write(unsigned char data) __z88dk_fastcall
{
	#asm
	push d
	mov c,l

        mvi      b,8

ow_pb1:  

	mov a,c
	rrc                    ;B[7] => CY
	mov c,a

	jnc ow_put_low
	mvi a,PIO_B_IN
	out PIO_M
	mvi a,PIO_B_OUT	; SDA + SCL output
	out PIO_M	;
	mvi a,OW_Bit	; clear bit
	out PIO_B
	mvi a,PIO_B_IN
	out PIO_M

	jmp ow_pnxt
ow_put_low:
	mvi a,PIO_B_IN
	out PIO_M
	mvi a,PIO_B_OUT	; SDA + SCL output
	out PIO_M	;
	mvi a,OW_Bit	; clear bit
	out PIO_B

	mvi d,5
w_lp4:	dcr d
	jnz w_lp4
ow_pnxt:
	mvi d,18
w_lp3:	dcr d
	jnz w_lp3


	dcr b
        jnz    ow_pb1

	mvi a,PIO_B_IN
	out PIO_M
	pop d
	#endasm
}

unsigned char ow_read() __z88dk_fastcall
{
	#asm
	push d
		mvi b,8
ow_gb1:    
	mvi a,PIO_B_OUT       		
	out PIO_M   		;

	mvi a,PIO_B_IN
	out PIO_M

;	nop
;	nop
;	nop
;	nop

	in PIO_B			; SDA (RX data bit) is in A.0

	mvi d,18
w_lp5:	dcr d
	jnz w_lp5

	rrc					; move RX data bit to CY
	mov a,c
	rar                    	; Shift CY into C
	mov c,a
	dcr b
        jnz    ow_gb1
        mov l,c             		; Return RX Byte in L
	pop d
	#endasm
}

unsigned char ds_calc_crc8() __z88dk_fastcall
{
	#asm
	push d
	mvi h,8		; scrach len
	lxi d,_scrachpad ; DE = scrachpad
	mvi c,0		; c = crc

nxt_bt_loop:
	ldax d
	inx d
	mvi l,8

rot_bt_loop:
	push psw	; save inbyte
	xra c
	ani 1
	mov b,a		; b = (crc ^ inbyte) & 0x01;
	
	mov a,c
	rar
	mov c,a		; c = crc >> 1

	mov a,b
	cpi 1
	jnz dont_xor
	mov a,c
	xri 08Ch
	mov c,a
dont_xor:
	pop psw		; restore inbyte
	rar		; inbyte >> 1
	
	dcr l
	jnz rot_bt_loop
	dcr h
	jnz nxt_bt_loop
	mov l,c		; return CRC
	pop d
	#endasm
}

uint16_t ds_convert_temp() __z88dk_fastcall
{
	#asm
	push d

	lda _scrachpad+1
	ani 080h
	jnz sign_minus
	mvi d,00h	; "+"
	jmp do_calc_temp
sign_minus:
	mvi d,080h	; "-"	
	lda _scrachpad
	ani 0fh
	jnz subst_2
	lda _scrachpad
	sui 1
	jmp store_pad
subst_2:
	lda _scrachpad
	sui 2
store_pad:
	cma
	sta _scrachpad
	lda _scrachpad+1
	cma
	sta _scrachpad+1
do_calc_temp:

	lda _scrachpad
			;           ;       // cycles
			;shl ax, 1  ; *2    // 2
			;mov bx, ax ; *2    // 4
			;shl ax, 1  ; *4    // 6
			;shl ax, 1  ; *8    // 8
			;add ax, bx ; *10   // 10

	ani 00001111b
	ral
	mov b,a
	ral
	ral
	add b

	ani 11110000b	; shift 4 right
	rar
	rar
	rar
	rar
	mov l,a
	;sta local_temp_l
	
	lda _scrachpad
	rar
	rar
	rar
	rar
	ani 00001111b
	mov b, a
	lda _scrachpad+1
	ral
	ral
	ral
	ral
	ani 11110000b
	ora b
	;sta local_temp_h	

	mov h,a
	mov a,d
	cpi 080h
	jnz done_conv_temp
	mov a,h
	sui 1
	cma
	mov h,a
done_conv_temp:
	pop d
	#endasm
}

unsigned char ds_read_famcode(void)
{
    unsigned char ret=0;
    ow_init();
    ow_write(0x33);
    ret = ow_read();
    return ret;
}

void ds_start_conversion(void)
{
    ow_init();
    ow_write(0xcc);
    ow_write(0x44);
}

void ds_read_scrach(unsigned char * buff)
{
    unsigned char n;
    ow_init();
    ow_write(0xcc);
    ow_write(0xbe);
    for (n=0; n<9; n++)     // read 9 bytes but, use only one byte
    {
       buff[n]=ow_read();  // read DS1820
    }
}

//-----------------------------------------
// Read temperature
//-----------------------------------------
/*
unsigned char ReadTemp(unsigned char * buff)
{
	unsigned char n;
   unsigned char fam;
	   
    Init_DS1820();   
    WriteByte(0xcc);  // skip ROM
    WriteByte(0x44);  // perform temperature conversion
    //while (ReadByte()==0xff); // wait for conversion complete	
   t_delay(120);
    Init_DS1820();
    WriteByte(0xcc);  // skip ROM
    WriteByte(0xbe);  // read the result
    
    for (n=0; n<9; n++)     // read 9 bytes but, use only one byte
    {
       buff[n]=ReadByte();  // read DS1820
    }
	EA=1;
    return fam;
}


*/
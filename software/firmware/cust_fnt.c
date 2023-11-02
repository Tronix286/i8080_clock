/* 
 * File: tftst.h      
 * Author: funStackLabs
 * Comments: ST7735 1.8" SPI TFT screens PIC C driver
 * 
 */

#include "cust_fnt.h"
#include "lcd.h"
#include "math.h"
#include "string.h"
#include "gui.h"

uint16_t __fgColor = 0;
uint16_t __bgColor = 0;
uint16_t __blend[16];

void tfstPrepareBlend(uint16_t color1, uint16_t color2) {
    if (__fgColor == color1 && __bgColor == color2) {
        return;
    }
/*
    uint8_t color1Red = color1 >> 11;
    uint8_t color1Green = (color1 >> 5) & 0b00111111;
    uint8_t color1Blue = color1 & 0b00011111;

    uint8_t color2Red = color2 >> 11;
    uint8_t color2Green = (color2 >> 5) & 0b00111111;
    uint8_t color2Blue = color2 & 0b00011111;

    for (uint8_t i = 0; i < 16; i++) {
        float alpha = i / 15.0f;
        float iAlpha = 1 - alpha;
        uint8_t blendRed = (color1Red * alpha) + (color2Red * iAlpha);
        uint8_t blendGreen = (color1Green * alpha) + (color2Green * iAlpha);
        uint8_t blendBlue = (color1Blue * alpha) + (color2Blue * iAlpha);
        __blend[i] = (blendRed << 11) + (blendGreen << 5) + blendBlue;
    }
*/
#if 1
    for (uint8_t i = 0; i < 16; i++) {

    // Alpha converted from [0..255] to [0..31]
    uint8_t alpha = i*2;

    // Converts  0000000000000000rrrrrggggggbbbbb
    //     into  00000gggggg00000rrrrr000000bbbbb
    // with mask 00000111111000001111100000011111
    // This is useful because it makes space for a parallel fixed-point multiply
    uint32_t bg = ((uint32_t)color2 | ((uint32_t)color2 << 16)) & 0b00000111111000001111100000011111;
    uint32_t fg = ((uint32_t)color1 | ((uint32_t)color1 << 16)) & 0b00000111111000001111100000011111;

    // This implements the linear interpolation formula: result = bg * (1.0 - alpha) + fg * alpha
    // This can be factorized into: result = bg + (fg - bg) * alpha
    // alpha is in Q1.5 format, so 0.0 is represented by 0, and 1.0 is represented by 32
    uint32_t result = (fg - bg) * alpha; // parallel fixed-point multiply of all components
    result >>= 5;
    result += bg;
    result &= 0b00000111111000001111100000011111; // mask out fractional parts
    __blend[i] = (uint16_t)((result >> 16) | result); // contract result

    }
#endif

    __fgColor = color1;
    __bgColor = color2;
}

void tftstCharHelper(TFTSTCustomFontCharData *charData) __z88dk_fastcall
{
	#asm
	lxi b,3
	dad b		; HL+3 (charData.size)

	mov c,m
	inx h
	mov b,m		; BC = charData.size
	inx h		; HL+5 (chardata)

	mov e,m
	inx h
	mov d,m
	XCHG		; DE -> HL
			; HL = charData.compressedData[]

do_ex_char:

	push b

        mvi b,0
	mov a,m
	ani 00001111b
	rlc		; * 2
	mov c,a		; BC = aplha

	push h

	lxi h,___blend  ; HL = __blend[]
	dad b

	mov c,m		; BC = __blend[alpha]
	inx h
	mov b,m

	pop h

	mov a,m
	rar
	rar
	rar
	rar
	ani 00001111b
	mov d,a		; D = count
	inx h
	
do_ex_chr_cnt:
	mov a,b
	out LCD_DATA_PORT
	mov a,c
	out LCD_DATA_PORT
	dcr d
	jnz do_ex_chr_cnt

	pop b

	dcx b
	mov     a,b         
        ora     c           
        jnz     do_ex_char

	#endasm
}

void tftstDrawCharWithFont(TFTSTCustomFontData *font, uint16_t x, uint16_t y, uint8_t c, uint16_t color, uint16_t bg) {
    tfstPrepareBlend(color, bg);

    TFTSTCustomFontCharData charData;
    charData = font->charData[c - 32];


    if (charData.left >= 0){
        LCD_SetWindows(x + charData.left, y + charData.top, x + charData.left + charData.width - 1, lcddev.height);
    } else {
        LCD_SetWindows(x + charData.left, y + charData.top, x + charData.left + charData.left + charData.width - 1, lcddev.height);
    }
/*
    for (int16_t i = 0; i < charData.size; i++) {
        uint8_t count = charData.compressedData[i] >> 4;
        uint8_t alpha = charData.compressedData[i] & 15;
        for (uint8_t j = 0; j < count; j++) {
            LCD_WR_DATA_16Bit(__blend[alpha]);
        }
    }
*/
    tftstCharHelper(charData);
}

void tftstDrawImg(char *img, uint16_t x, uint16_t y, uint16_t color, uint16_t bg) {
    int i;
    tfstPrepareBlend(color, bg);
    LCD_SetWindows(x, y, x + 32 - 1, y + 32 - 1);
    for(i=0;i<(32*32)/2;i++)
	{	
        	uint8_t col1 = (*(img+i) >> 4) & 15;
        	uint8_t col2 = *(img+i) & 15;
            	LCD_WR_DATA_16Bit(__blend[col1]);
            	LCD_WR_DATA_16Bit(__blend[col2]);
	}	
}

void tftstDrawTextWithFont(TFTSTCustomFontData *font, uint16_t x, uint16_t y, char *_text, uint16_t color, uint16_t bg) {
    uint16_t cursor_x, cursor_y;
    uint16_t textsize, i;
    cursor_x = x, cursor_y = y;
    textsize = strlen(_text);
    for (i = 0; i < textsize; i++) {
        TFTSTCustomFontCharData charData;
	charData = font->charData[_text[i] - 32];
        uint8_t charWidth = charData.width + charData.left;
        if (((cursor_x + charWidth) > lcddev.width)) {
            cursor_x = 0;
            cursor_y = cursor_y + font->size + 2;
            if (cursor_y > lcddev.height) cursor_y = lcddev.height;
            if (_text[i] == 0x20) goto _skip;
        }
        tftstDrawCharWithFont(font, cursor_x, cursor_y, _text[i], color, bg);

        cursor_x = cursor_x + charWidth;
        if (cursor_x > lcddev.width) cursor_x = lcddev.width;
_skip:
        ;
    }
}

void tftstDrawTextWithFontMulti(TFTSTCustomFontData *font, uint16_t x, uint16_t y, char *_text, uint16_t color1, uint16_t color2, uint16_t bg) {
    uint16_t cursor_x, cursor_y;
    uint16_t textsize, i;
    cursor_x = x, cursor_y = y;
    uint16_t color=color1;
    textsize = strlen(_text);
    for (i = 0; i < textsize; i++) {
        TFTSTCustomFontCharData charData;
	if (_text[i] == 1) {color = color1; goto _skip;}
	if (_text[i] == 2) {color = color2; goto _skip;}
        if (_text[i] == 0x20) goto _skip;
	charData = font->charData[_text[i] - 32];
        uint8_t charWidth = charData.width + charData.left;
        if (((cursor_x + charWidth) > lcddev.width)) {
            cursor_x = 0;
            cursor_y = cursor_y + font->size + 2;
            if (cursor_y > lcddev.height) cursor_y = lcddev.height;
        }
        tftstDrawCharWithFont(font, cursor_x, cursor_y, _text[i], color, bg);

        cursor_x = cursor_x + charWidth;
        if (cursor_x > lcddev.width) cursor_x = lcddev.width;
_skip:
        ;
    }
}

// return string length in pixels with unique font
uint16_t tftstTextWithFontLength(TFTSTCustomFontData *font, char *_text) {
    uint16_t txtlen,textsize,i;
    textsize = strlen(_text);
    txtlen = 0;
    for (i = 0; i < textsize; i++) {
	txtlen = txtlen +(font->charData[_text[i] - 32].width) + (font->charData[_text[i] - 32].left);
    }
   return txtlen;
}

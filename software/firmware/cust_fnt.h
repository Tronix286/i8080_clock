#ifndef TFTST_CUSTOM_FONTS_H
#define	TFTST_CUSTOM_FONTS_H
#include "types.h"


typedef struct TFTSTCustomFontCharData {
    int8_t left;
    int8_t top;
    uint8_t width;
    uint16_t size;
    uint8_t *compressedData;
} TFTSTCustomFontCharData;

typedef struct TFTSTCustomFontData {
    uint8_t size;
    TFTSTCustomFontCharData *charData;
} TFTSTCustomFontData;

void tfstPrepareBlend(uint16_t color1, uint16_t color2);
void tftstDrawCharWithFont(TFTSTCustomFontData *font, uint16_t x, uint16_t y, uint8_t c, uint16_t color, uint16_t bg);
void tftstDrawTextWithFont(TFTSTCustomFontData *font, uint16_t x, uint16_t y, char *_text, uint16_t color, uint16_t bg);
uint16_t tftstTextWithFontLength(TFTSTCustomFontData *font, char *_text);
void tftstDrawTextWithFontMulti(TFTSTCustomFontData *font, uint16_t x, uint16_t y, char *_text, uint16_t color1, uint16_t color2, uint16_t bg);
void tftstDrawImg(char *img, uint16_t x, uint16_t y, uint16_t color, uint16_t bg);
#endif	/* TFTST_CUSTOM_FONTS_H */
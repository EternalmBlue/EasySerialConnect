#include "main.h"

#define Line0 0
#define Line1 24
#define Line2 48
#define Line3 72
#define Line4 96
#define Line5 120
#define Line6 144
#define Line7 168
#define Line8 192
#define Line9 216

void lcdWriteRam(uint8_t RS,uint16_t value);
void lcdInit(void);
void lcdClearScreen(uint16_t color);
void lcdDisplayLine(uint8_t *String,uint8_t Line);
void lcdDisplaychar(uint8_t ch,uint8_t Line,uint16_t yPos);
void lcdDrawChar(uint16_t *ch,uint8_t Line,uint16_t yPos);
void lcdSetCursor(uint8_t xPos,uint16_t yPos);
void lcdWritePrepar(void);
void lcdWriteValue(uint16_t address,uint16_t value);
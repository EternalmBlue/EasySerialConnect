#include "main.h"
#include "lcd.h"
#include "fonts.h"

uint16_t backColor = 0x0000;
uint16_t textColor = 0xffff;

void lcdWriteRam(uint8_t RS,uint16_t value)
{
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,0);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,1);
    if (RS == 0)
    {
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,0);
    }else
    {
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,1);
    }
    GPIOC->ODR = value;
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,0);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,1);
}

void lcdInit(void)
{
    lcdWriteValue(0x00,0x0001);
    lcdWriteValue(0x03,0x1018);
    lcdWriteValue(0x07,0x0173);
    lcdWriteValue(0x10,0x1090);
    lcdWriteValue(0x11,0x0227);
    lcdWriteValue(0x12,0x001d);
    lcdWriteValue(0x60,0x2700);
    lcdWriteValue(0x61,0x0001);
    HAL_Delay(250);
}

void lcdClearScreen(uint16_t color)
{
    lcdSetCursor(0x00,0x00);
    lcdWritePrepar();
    for (int i = 0; i < 240*320; i++)
    {
        lcdWriteRam(1,color);
    }
}
void lcdDisplayLine(uint8_t *String,uint8_t Line)
{
    uint8_t max = 20;
    uint8_t i = 0;
    uint16_t yPos = 319;
    while (*String != 0 && i < max)
    {
        lcdDisplaychar(*String,Line,yPos);
        yPos -= 16;
        String++;
        i++;
    }
}
void lcdDisplaychar(uint8_t ch,uint8_t Line,uint16_t yPos)
{
    uint8_t ascii = ch - 32;
    lcdDrawChar(&ASCII_Table[ascii*24],Line,yPos);
}
void lcdDrawChar(uint16_t *ch,uint8_t Line,uint16_t yPos)
{
    for (uint8_t i = 0; i < 24; i++)
    {
        lcdSetCursor(Line,yPos);
        lcdWritePrepar();
        for (uint8_t j = 0; j < 16; j++)
        {
            if ((ch[i] & (1 << j)) == 0)
            {
                lcdWriteRam(1,backColor);
            }else
            {
                lcdWriteRam(1,textColor);
            }
        }
        Line++;
    }
    
}
void lcdSetCursor(uint8_t xPos,uint16_t yPos)
{
    lcdWriteValue(0x20,xPos);
    lcdWriteValue(0x21,yPos);
}

void lcdWritePrepar(void)
{
    lcdWriteRam(0,0x22);
}

void lcdWriteValue(uint16_t address,uint16_t value)
{
    lcdWriteRam(0,address);
    lcdWriteRam(1,value);
}
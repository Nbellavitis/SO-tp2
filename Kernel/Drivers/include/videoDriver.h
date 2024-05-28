#ifndef VIDEODRIVER_H
#define VIDEODRIVER_H

#include <stdint.h>
#define FONT_SIZE 1
#define FONT_BIG 2


void drawChar(uint32_t hexColor, char character);

void drawRectangle(uint32_t color, uint32_t x, uint32_t y, uint32_t height, uint32_t width);

void putPixel(uint32_t hexColor, uint32_t x, uint32_t y);

void drawWord(uint32_t hexColor,char * str);

void drawWordLen(uint32_t hexColor,char * str,int len);

void drawBuffer(uint32_t hexColor);

void setFontSize(uint32_t size);

void clear();

void newLine();

uint16_t getWidth();

uint16_t getHeight();

void drawWordAt(uint32_t hexColor,char * str,uint32_t posX,uint32_t posY);
#endif
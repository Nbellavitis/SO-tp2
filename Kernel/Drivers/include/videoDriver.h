#ifndef VIDEODRIVER_H
#define VIDEODRIVER_H

#include <stdint.h>
#define FONT_SIZE 2


void drawChar(uint32_t hexColor, char character);

void drawRectangle(uint32_t color, uint32_t x, uint32_t y, uint32_t height, uint32_t width);

void putPixel(uint32_t hexColor, uint32_t x, uint32_t y);

void drawWord(uint32_t hexColor,char * str);

void drawBuffer(uint32_t hexColor);
#endif
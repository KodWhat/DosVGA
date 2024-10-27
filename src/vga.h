#ifndef __INCLUDE_VGA_H__
#define __INCLUDE_VGA_H__

#include "typedef.h"

#define VGA_WIDTH   320
#define VGA_HEIGHT  200
#define VGA_COLORS  256

typedef struct
{
    byte* vector;
    int bytesPerChar;
    short charWidth;
    short charHeight;
} VGA_CharGenInfo;

void VGA_SetTextMode();

void VGA_SetVga256Mode();

void VGA_DrawPixel(short x, short y, byte color);

void VGA_DrawCharacter(char character, short x, short y, byte color);

void VGA_PrintText(char* string, short x, short y, byte color);

#endif

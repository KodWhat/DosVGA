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
} dosvga_char_gen_info;

void dosvga_set_text_mode();

void dosvga_set_vga_256_colors_mode();

void dosvga_draw_pixel(short x, short y, byte color);

void dosvga_draw_character(char character, short x, short y, byte color);

void dosvga_print_text(char* string, short x, short y, byte color);

#endif

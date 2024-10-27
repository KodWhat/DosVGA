#include "vga.h"
#include "typedef.h"

#include <stdlib.h>
#include <stdio.h>
#include <i86.h>

#define BIOS_INT10 0x10
#define BIOS_INT10_SET_VIDEO_MODE_SVC 0x00
#define BIOS_INT10_CHAR_GEN_SVC 0x11
#define BIOS_INT10_CHAR_GEN_SVC_GET_INFO 0x30

#define BIOS_INT10_CHAR_GEN_SVC_GET_INFO_VEC43H 0x01
#define BIOS_INT10_CHAR_GEN_SVC_GET_INFO_8x16 0x06

static byte* p_vga_display = (byte*)0xA0000000L;
static dosvga_char_gen_info p_char_gen_info;

static void p_dosvga_ensure_initialized();
static void p_dosvga_set_mode(byte mode);
static void p_dosvga_init_char_gen_info();
static void p_dosvga_draw_character_byte(byte charByte, short x, short y, byte color);

void dosvga_set_text_mode()
{
    p_dosvga_set_mode(0x03); // Default text mode
}

void dosvga_set_vga_256_colors_mode()
{
    p_dosvga_set_mode(0x13); // VGA 256 colors
}

void dosvga_draw_pixel(short x, short y, byte color)
{
    // Safe position
    if(x >= VGA_WIDTH || y >= VGA_HEIGHT)
    {
        return;
    }
    
    p_vga_display[VGA_WIDTH * y + x] = color;
}

void dosvga_draw_character(char character, short x, short y, byte color)
{
    byte* charStart;
    byte line;
    
    p_dosvga_ensure_initialized();

    charStart = (byte*)((p_char_gen_info.vector) + character * p_char_gen_info.bytesPerChar);

    for(line = 0; line < p_char_gen_info.bytesPerChar; line++)
    {
        p_dosvga_draw_character_byte(charStart[line], x, y + line, color);
    }
}

void dosvga_print_text(char* string, short x, short y, byte color)
{
    int index = 0;
    char c = string[index];
    
    p_dosvga_ensure_initialized();
    
    while(c != '\0')
    {
        dosvga_draw_character(string[index], x + index * p_char_gen_info.charWidth, y, color);
        
        index++;
        c = string[index];
    }
}

// Private functions
static void p_dosvga_ensure_initialized()
{
    if(p_char_gen_info.vector == NULL)
    {
        p_dosvga_init_char_gen_info();
    }
}

static void p_dosvga_set_mode(byte mode)
{
    union REGS regs;

    regs.h.ah = BIOS_INT10_SET_VIDEO_MODE_SVC; // mode set
    regs.h.al = mode;
    int86(BIOS_INT10, &regs, &regs); // Call interupt and set graphics mode
}

static void p_dosvga_init_char_gen_info()
{
    union REGPACK regs;

    regs.h.ah = BIOS_INT10_CHAR_GEN_SVC;
    regs.h.al = BIOS_INT10_CHAR_GEN_SVC_GET_INFO;
    regs.h.bh = BIOS_INT10_CHAR_GEN_SVC_GET_INFO_VEC43H;

    intr(BIOS_INT10, &regs);
    
    p_char_gen_info.vector = (byte*) MK_FP(regs.w.es, regs.w.bp);
    p_char_gen_info.bytesPerChar = 8;
    p_char_gen_info.charWidth = 8;
    p_char_gen_info.charHeight = 8;
}

static void p_dosvga_draw_character_byte(byte charByte, short x, short y, byte color)
{
    byte mask = 0x80;

    short xBit = x;

    while(mask !=0)
    {
        if((mask & charByte) != 0)
        {
            dosvga_draw_pixel(xBit, y, color);
        }
        
        mask = mask >> 1;
        xBit++;
    }
}

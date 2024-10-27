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

static byte* PRIV_VgaDisplay = (byte*)0xA0000000L;
static VGA_CharGenInfo PRIV_CharGenInfo;

static void PRIV_EnsureInitialized();
static void PRIV_SetMode(byte mode);
static void PRIV_InitCharGenInfo();
static void PRIV_DrawCharacterByte(byte charByte, short x, short y, byte color);

void VGA_SetTextMode()
{
    PRIV_SetMode(0x03); // Default text mode
}

void VGA_SetVga256Mode()
{
    PRIV_SetMode(0x13); // VGA 256 colors
}

void VGA_DrawPixel(short x, short y, byte color)
{
    // Safe position
    if(x >= VGA_WIDTH || y >= VGA_HEIGHT)
    {
        return;
    }
    
    PRIV_VgaDisplay[VGA_WIDTH * y + x] = color;
}

void VGA_DrawCharacter(char character, short x, short y, byte color)
{
    byte* charStart;
    byte line;
    
    PRIV_EnsureInitialized();

    charStart = (byte*)((PRIV_CharGenInfo.vector) + character * PRIV_CharGenInfo.bytesPerChar);

    for(line = 0; line < PRIV_CharGenInfo.bytesPerChar; line++)
    {
        PRIV_DrawCharacterByte(charStart[line], x, y + line, color);
    }
}

void VGA_PrintText(char* string, short x, short y, byte color)
{
    int index = 0;
    char c = string[index];
    
    PRIV_EnsureInitialized();
    
    while(c != '\0')
    {
        VGA_DrawCharacter(string[index], x + index * PRIV_CharGenInfo.charWidth, y, color);
        
        index++;
        c = string[index];
    }
}

// Private functions
static void PRIV_EnsureInitialized()
{
    if(PRIV_CharGenInfo.vector == NULL)
    {
        PRIV_InitCharGenInfo();
    }
}

static void PRIV_SetMode(byte mode)
{
    union REGS regs;

    regs.h.ah = BIOS_INT10_SET_VIDEO_MODE_SVC; // mode set
    regs.h.al = mode;
    int86(BIOS_INT10, &regs, &regs); // Call interupt and set graphics mode
}

static void PRIV_InitCharGenInfo()
{
    union REGPACK regs;

    regs.h.ah = BIOS_INT10_CHAR_GEN_SVC;
    regs.h.al = BIOS_INT10_CHAR_GEN_SVC_GET_INFO;
    regs.h.bh = BIOS_INT10_CHAR_GEN_SVC_GET_INFO_VEC43H;

    intr(BIOS_INT10, &regs);
    
    PRIV_CharGenInfo.vector = (byte*) MK_FP(regs.w.es, regs.w.bp);
    PRIV_CharGenInfo.bytesPerChar = 8;
    PRIV_CharGenInfo.charWidth = 8;
    PRIV_CharGenInfo.charHeight = 8;
}

static void PRIV_DrawCharacterByte(byte charByte, short x, short y, byte color)
{
    byte mask = 0x80;

    short xBit = x;

    while(mask !=0)
    {
        if((mask & charByte) != 0)
        {
            VGA_DrawPixel(xBit, y, color);
        }
        
        mask = mask >> 1;
        xBit++;
    }
}

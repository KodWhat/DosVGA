#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#include "vga.h"
#include "typedef.h"

#define KEY_ESCAPE 0x1B
#define KEY_ENTER 0x0D

void print_all_chars();

void main()
{
    char kc = 0;

    VGA_SetVga256Mode();

    //while(kc != KEY_ENTER)
    //{
    //    kc = getch();
    //}

    VGA_PrintText("Hello world! This is a very long text that goes off screen", 17, 17, 12);
    VGA_PrintText("Hello world! This is a very long text th", 18, 18, 10);
    VGA_PrintText("Hello world! This is a very long text th", 19, 19, 9);
    VGA_PrintText("Hello world! This is a very long text th", 20, 20, 15);
    VGA_PrintText("Hello world!", 30, 30, 15);
    
    while(kc != KEY_ESCAPE)
    {
        kc = getch();
    }

    VGA_SetTextMode();

    printf("Program exit");
}

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#include "vga.h"
#include "typedef.h"

#define KEY_ESCAPE 0x1B
#define KEY_ENTER 0x0D

void main()
{
    char kc = 0;

    dosvga_set_vga_256_colors_mode();

    dosvga_print_text("Hello world! This is a very long text that goes off screen", 17, 17, 12);
    dosvga_print_text("Hello world! This is a very long text th", 18, 18, 10);
    dosvga_print_text("Hello world! This is a very long text th", 19, 19, 9);
    dosvga_print_text("Hello world! This is a very long text th", 20, 20, 15);
    dosvga_print_text("Hello world!", 30, 30, 15);
    
    while(kc != KEY_ESCAPE)
    {
        kc = getch();
    }

    dosvga_set_text_mode();

    printf("Program exit");
}

#ifndef VGA_BASIC_H
#define VGA_BASIC_H
#include "port.h"

typedef
enum char_color {
    black = 0,
    blue = 1,
    green = 2,
    cyan = 3,
    red = 4,
    magenta = 5,
    brown = 6,
    light_grey = 7,
    dark_grey = 8,
    light_blue = 9,
    light_green = 10,
    light_cyan = 11,
    light_red = 12,
    light_magenta = 13,
    yellow  = 14,   
    white = 15
} vga_color_t;

void vga_putc(char,vga_color_t,vga_color_t);
void kputc_color(char,vga_color_t,vga_color_t);
void kputc(char);
void clear_screen();
void mov_cursor_by_XY();
void kputs_color(char *,vga_color_t,vga_color_t);
void kputs(char *);
void vga_init();
#endif
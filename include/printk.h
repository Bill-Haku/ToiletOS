#ifndef PRINTK_H
#define PRINTK_H
#include "string.h"
#include "vga_basic.h"
#include "vargs.h"

void insert_str(char *inserted_str,char *inserting_str,uint32_t offset);
void printk(char *input_str,...);
void printbasic(char *format_str,char *m);
void printk_color(char *input_str,vga_color_t back,vga_color_t fore,...);
void put_str(char* message);
void put_char(uint8_t char_ascii);
void put_int(uint32_t num);
void printkDebug();

#endif
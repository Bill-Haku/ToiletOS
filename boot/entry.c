#include "stdint.h"

void kern_entry(){
    char *input = (uint8_t *)0xB8000;
    char color = (0 << 4) | (15 & 0x0F);

    *input++ = 'T'; *input++ = color;
    *input++ = 'o'; *input++ = color;
    *input++ = 'i'; *input++ = color;
    *input++ = 'l'; *input++ = color;
    *input++ = 'e'; *input++ = color;
    *input++ = 't'; *input++ = color;
    *input++ = 'O'; *input++ = color;
    *input++ = 'S'; *input++ = color;
    *input++ = '!'; *input++ = color;
    
    while(1)
    	asm volatile ("hlt");
}
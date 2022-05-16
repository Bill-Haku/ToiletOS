#ifndef PORT_H
#define PORT_H
#include "types.h"

void outb(uint16_t , uint8_t );
uint8_t inb(uint16_t );
uint16_t inw(uint16_t );

#endif
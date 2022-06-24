#include "printk.h"
#include "init.h"

void main(void) {
	printkDebug();
	init_all();
	asm volatile("sti");
	while(1);
	return 0;
}
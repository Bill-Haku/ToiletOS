#include "printk.h"
#include "init.h"
#include "debug.h"

void main(void) {
	printkDebug();
	init_all();
	// asm volatile("sti");
	ASSERT(1 == 2);
	while(1);
	return 0;
}
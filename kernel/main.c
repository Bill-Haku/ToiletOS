#include "printk.h"
#include "init.h"
#include "debug.h"

void main(void) {
	printkDebug();
	init_all();
	// asm volatile("sti");
	printk("Toilet OS Start Complete!\n");
	printk_color("Hello!\n",blue, red);
	printk_color("Welcome to Toilet OS where every App can shits!\n",blue, red);
	while(1);
	return 0;
}
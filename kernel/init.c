#include "init.h"
#include "printk.h"
#include "timer.h"
#include "memory.h"

void init_all() {
    put_str("initing_all start...\n");
    // init interrupt
    idt_init();
    // init timer PIT8253
    timer_init();
    // init memory
    mem_init();

    put_str("init_all done.\n");
}
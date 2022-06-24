# include "init.h"
# include "printk.h"
# include "timer.h"

void init_all() {
    put_str("init_all.\n");
    // init interrupt
    idt_init();
    // init timer PIT8253
    timer_init();
}
#include "init.h"
#include "printk.h"
#include "timer.h"
#include "memory.h"
#include "thread/thread.h"
#include "../device/console.h"
#include "../device/keyboard.h"
#include "../user/tss.h"

void init_all() {
    printk("initing_all start...\n");
    // init interrupt
    idt_init();
    // init timer PIT8253
    timer_init();
    // init memory
    mem_init();
    // init thread
    thread_init();
    // init console
    console_init();
    // init keyboard
    keyboard_init();
    // init tss
    tss_init();

    printk("init_all done.\n");
}
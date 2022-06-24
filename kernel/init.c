# include "init.h"
# include "printk.h"

void init_all() {
    put_str("init_all.\n");
    idt_init();
}
#ifndef INTERRUPT_H
#define INTERRUPT_H

# include "stdint.h"

typedef void* intr_handler;

void idt_init(void);

/**
 * 中断状态.
 */ 
enum intr_status {
    INTR_OFF,
    INTR_ON
};

enum intr_status intr_get_status(void);
enum intr_status intr_set_status(enum intr_status);
enum intr_status intr_enable(void);
enum intr_status intr_disable(void);

#endif
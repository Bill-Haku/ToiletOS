# ifndef TSS_H
# define TSS_H

#include "thread/thread.h"

void update_tss_esp(struct task_struct* pthread);
void tss_init();

# endif
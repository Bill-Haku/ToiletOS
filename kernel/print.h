# ifndef KERNEL_PRINT_H
# define KERNEL_PRINT_H

# include "stdint.h"

void aput_char(uint8_t char_asci);

void set_cursor(uint32_t pos);

# endif
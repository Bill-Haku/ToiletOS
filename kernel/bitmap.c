#include "bitmap.h"
#include "stdint.h"
#include "string.h"
#include "printk.h"
#include "interrupt.h"
#include "debug.h"

// btmp init
void bitmap_init(struct bitmap* btmp) {
    memset(btmp->bits, 0, btmp->btmp_bytes_len);
}

// check if bit_idx == 1 return true
bool bitmap_test(struct bitmap* btmp, uint32_t bit_idx) {
    uint32_t byte_idx = bit_idx / 8;
    uint32_t bit_odd = bit_idx % 8;
    return (btmp->bits[byte_idx] & (BITMAP_MASK << bit_odd));
}

// apply continuous cnt bits in bitmap, return start index if true and -1 if failed
int bitmap_scan(struct bitmap* btmp, uint32_t cnt) {
    uint32_t idx_byte = 0;
    // first compare byte by byte
    while((0xff == btmp->bits[idx_byte]) && (idx_byte < btmp->btmp_bytes_len)) {
        // 1 means it is allocated, if it is 0xff, it means there is no spare bit and find in the next byte
        idx_byte++;
    }

    ASSERT(idx_byte < btmp->btmp_bytes_len);
    if (idx_byte == btmp->btmp_bytes_len) { //cannot find enough space in memory pool
        return -1;
    }

    // if find a spare bit in the range of bitmap array
    // check in the byte bit by bit and return the index of the spare bit
    int idx_bit = 0;
    // compare with btmp->bits[idx_byte] this byte bit by bit
    while((uint8_t)(BITMAP_MASK << idx_bit) & btmp->bits[idx_byte]) {
        idx_bit++;
    }

    int bit_idx_start = idx_byte * 8 + idx_bit; // index of the spare bit in the bitmap
    if (cnt == 1) {
        return bit_idx_start;
    }

    uint32_t bit_left = (btmp->btmp_bytes_len - bit_idx_start) * 8 - idx_bit; // record how many bits can be judged
    uint32_t next_bit = bit_idx_start + 1;
    uint32_t count = 1;

    bit_idx_start = -1;
    while (bit_left-- > 0) {
        if (!(bitmap_scan_test(btmp, next_bit))) { // if next_bit == 0
            count++;
        } else {
            count = 0;
        }
        if (count == cnt) { // if find continuous cnt spare bits
            bit_idx_start = next_bit - cnt + 1;
            break;
        }
        next_bit++;
    }
    return bit_idx_start;
}

// set the bit_idx of bitmap btmp to value
void bitmap_set(struct bitmap* btmp, uint32_t bit_idx, int8_t value) {
    ASSERT((value == 0) || (value == 1));
    uint32_t byte_idx = bit_idx / 8;
    uint32_t bit_odd = bit_idx % 8;

    if (value) {
        btmp->bits[byte_idx] |= (BITMAP_MASK << bit_odd);
    } else {
        btmp->bits[byte_idx] &= ~(BITMAP_MASK << bit_odd);
    }
}
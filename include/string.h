#ifndef STRING_H
#define STRING_H

#include "types.h"

void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len);

void memset(void *dest, uint8_t val, uint32_t len);

void bzero(void *dest, uint32_t len);

int strcmp(const char *str1, const char *str2);

char *strcpy(char *dest, const char *src);

char *strcat(char *dest, const char *src);

int strlen(const char *src);

char *uintTostring(uint32_t num);

char *strrevers(char *str);

char *num2hexstr(int number,int need0x);

#endif

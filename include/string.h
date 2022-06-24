#ifndef STRING_H
#define STRING_H

#include "types.h"

void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len);

void memset(void *dest, uint8_t val, uint32_t len);

int memcmp(const void* a_, const void* b_, uint32_t size);

void bzero(void *dest, uint32_t len);

int strcmp(const char *str1, const char *str2);

char *strcpy(char *dest, const char *src);

char *strcat(char *dest, const char *src);

int strlen(const char *src);

char *strchr(const char *str, const uint8_t ch);

char *strrchr(const char *str, const uint8_t ch);

uint32_t strchrs(const char* str, uint8_t ch);

char *uintTostring(uint32_t num);

char *strrevers(char *str);

char *num2hexstr(int number,int need0x);

#endif

#ifndef TYPES_H
#define TYPES_H

#ifndef NULL
    #define NULL 0
#endif

#ifndef True
    #define True  1
#endif
#ifndef False
    #define False  0
#endif

#ifndef BOOL
    typedef int bool;
#endif

typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed int int32_t;
typedef signed long long int int64_t;

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;

#endif
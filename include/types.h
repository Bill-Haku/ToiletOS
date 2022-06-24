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

typedef unsigned int   uint32_t;
typedef          int   int32_t;
typedef unsigned short uint16_t;
typedef          short int16_t;
typedef unsigned char  uint8_t;
typedef          char  int8_t;
typedef int bool;
#endif
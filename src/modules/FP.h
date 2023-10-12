#ifndef _FP_H_
#define _FP_H_

#include <stdint.h>

#define MANTISSA_LOW        0
#define MANTISSA_HIGH       1
#define EXPONENT            2
#define FP_SIZE             3
#define _16_BIT_SIGNED_MAX  32767
#define MAX_STRING_LENGTH   16

typedef uint16_t * Float;

Float FP_createFromString( const char * decimalStr );
void FP_delete( Float floatNumber );
char* FP_toString( const Float value );

#endif 

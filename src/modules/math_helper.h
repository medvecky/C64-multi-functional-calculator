#ifndef MATH_HELPER_H
#define MATH_HELPER_H

#include <stdint.h>

#define MAX_ARGUMENT_LENGTH 12

typedef uint8_t FP[5];

void FP_arg1ToFp( void );
void FP_arg2ToFp( void );
void FP_printResult( void );
void FP_arg1ToFac( void );
void FP_add( void );
void FP_facToResult( void );
void FP_printRaw( FP fp );
void FP_subst( void );
void FP_mult( void );
void FP_div( void );
void FP_pwr( void );
void FP_FACtoARG( void );
void FP_arg2ToFac( void );

#endif
#ifndef _FP_H_
#define _FP_H_

#include <stdint.h>

#define EXPONENT                0
#define MANTISSA_4              1
#define MANTISSA_3              2
#define MANTISSA_2              3
#define MANTISSA_1              4
#define FP_SIZE                 5
#define MAX_E_STRING_LENGTH     16 
#define MAX_NUMBER_SIZE_BITS    32
#define MANTISSA_TRESHOLD       100000000

typedef uint8_t * Float;

int8_t FP_createFromString( const char * decimalStr, Float floatNumber );
void FP_delete( Float floatNumber );
int8_t FP_toString( const Float value, char * resultString );

static int determineSign( const char * str );
static int16_t calculateExponent( const char * str );
static int16_t parseExponent( const char * str );
static int isScientific( const char *str ); 
static int8_t parseMantissaInt( const char * str, uint32_t * mantissa );
static int8_t addDigitToMantissa( uint8_t currentDigit, uint32_t * mantissa );
static uint8_t countTrailingZeros( const char * str );
static void removeTrailingZeros( const char * str, int16_t * mantissaHigh, uint16_t * mantissaLow );
static uint8_t countBinaryDigits( uint32_t mantissa ); 
static uint8_t calculateMBFPositiveExponent( uint32_t mantissa ); 
static void createMBFInt( uint32_t mantissa, int isNegative, uint8_t * ma4, uint8_t * ma3, uint8_t * ma2, uint8_t * ma1 );
static void mbfMantissaShiftRight( uint8_t * ma4, uint8_t * ma3, uint8_t * ma2, uint8_t * ma1, uint8_t shiftCount );
static void createDecimalString( uint16_t mantissaHigh, uint16_t mantissaLow, int8_t isNegative, char * resultString  );

#endif 

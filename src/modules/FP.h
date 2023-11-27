#ifndef _FP_H_
#define _FP_H_

#include <stdint.h>

#define EXPONENT                0
#define MANTISSA_4              1
#define MANTISSA_3              2
#define MANTISSA_2              3
#define MANTISSA_1              4
#define FP_SIZE                 5
#define MAX_STRING_LENGTH       10
#define MAX_E_STRING_LENGTH     16 
#define DIGIT_TRESHOLD_HIGH     0x7FFF
#define MBF_BIAS                0x80 
#define SIGN_BIT_MASK_SET       0x8000
#define SIGN_BIT_MASK_CLEAR     0x7FFF          

typedef uint8_t * Float;

int8_t FP_createFromString( const char * decimalStr, Float floatNumber );
void FP_delete( Float floatNumber );
int8_t FP_toString( const Float value, char * resultString );

static int determineSign( const char * str );
static int16_t calculateExponent( const char * str );
static int16_t parseExponent( const char * str );
static int isScientific( const char *str ); 
static int8_t parseMantissaInt( const char * str, int16_t * mantissaHigh, uint16_t * mantissaLow );
static int8_t parseMantissaFraction( const char * str, int16_t * mantissaHighP, uint16_t * mantissaLowP );
static int8_t addDigitToMantissa( uint8_t currentDigit, int16_t * mantissaHigh, uint16_t * mantissaLow );
static uint8_t countTrailingZeros( const char * str );
static void removeTrailingZeros( const char * str, int16_t * mantissaHigh, uint16_t * mantissaLow );
static uint8_t countBinaryDigits( uint16_t mantissaHigh, uint16_t mantissaLow ); 
static uint8_t calculateMBFPositiveExponent( int16_t mantissaIntHigh, uint16_t mantissaIntLow ); 
static void createMBFInt( int16_t mantissaIntHigh, uint16_t mantissaIntLow, int isNegative, uint8_t * ma4, uint8_t * ma3, uint8_t * ma2, uint8_t * ma1 );

#endif 

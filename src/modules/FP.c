#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <conio.h>

#include "FP.h"

int8_t FP_createFromString( const char * decimalStr, Float floatNumber )
{
    uint32_t mantissa = 0;
    uint8_t exponentMBF = 0;
    uint8_t ma4 = 0;
    uint8_t ma3 = 0;
    uint8_t ma2 = 0;
    uint8_t ma1 = 0;
    uint16_t mantissaLowMBF = 0;

    int16_t exponent = 0;
    
    bool isNegative = determineSign( decimalStr );
    bool scientific = isScientific( decimalStr);
    
    if ( scientific )
    {
        return EXIT_FAILURE;
    }
    else
    {
        exponent = calculateExponent( decimalStr );
        
        if ( parseMantissaInt( decimalStr, &mantissa ) == EXIT_FAILURE )
        {
            return EXIT_FAILURE;
        }
    }
    
    if ( exponent >= 0 )
    {
        exponentMBF = calculateMBFPositiveExponent( mantissa );
    }
    else
    {
        return EXIT_FAILURE;
    }
    
    createMBFInt( mantissa, isNegative, &ma4, &ma3, &ma2, &ma1 );

    floatNumber[ EXPONENT ] = exponentMBF;
    floatNumber[ MANTISSA_4 ] = ma4;
    floatNumber[ MANTISSA_3 ] = ma3;
    floatNumber[ MANTISSA_2 ] = ma2;
    floatNumber[ MANTISSA_1 ] = ma1;
 
    return EXIT_SUCCESS;
}

void FP_delete( Float floatNumber )
{
    free( floatNumber );
}

int8_t FP_toString( const Float value, char * resultString )
{
    uint8_t ma4 = value[ MANTISSA_4 ];
    uint8_t ma3 = value[ MANTISSA_3 ];
    uint8_t ma2 = value[ MANTISSA_2 ];
    uint8_t ma1 = value[ MANTISSA_1 ];
    uint8_t shiftCount = MAX_NUMBER_SIZE_BITS - ( value[ EXPONENT ] - 128 );
    int16_t mantissaHigh = 0;
    uint16_t mantissaLow = 0;
    bool isNegative = ma4 & 0x80;
    ma4 |= 0x80;

    mbfMantissaShiftRight( &ma4, &ma3, &ma2, &ma1, shiftCount );

    mantissaLow = ma2;
    mantissaLow = mantissaLow << 8;
    mantissaLow += ma1;

    mantissaHigh = ma4;
    mantissaHigh = mantissaHigh << 8;
    mantissaHigh += ma3;

    createDecimalString( mantissaHigh, mantissaLow, isNegative, resultString );

    return EXIT_SUCCESS;
}

static int determineSign( const char * str ) 
{
    if ( str[ 0 ] == '-' ) 
    {
        return true;
    } 
    else 
    {
        return false;
    }
}

static int8_t parseMantissaInt( const char * str, uint32_t * mantissa )
{
    uint8_t index = 0;
    bool isCarry = false;
    uint8_t currentDigit = 0;
    uint8_t decimalStrLen = strlen( str );

    if ( str[ index ] == '-' )
    {
        index++;
    }

    while ( str[ index ] >= '0' && str[ index ] <= '9'  && str[ index ] != '.' )
    {
        currentDigit = str[ index ] - '0';

        if ( str[ index ] == '.' )
        {
            index++;
            continue;
        } 
        
        if ( addDigitToMantissa( currentDigit, mantissa ) == EXIT_FAILURE ) 
        {
            return EXIT_FAILURE;
        }

       if ( ++index >= decimalStrLen - 1 ) break;
    }

    return EXIT_SUCCESS;
}

static int16_t calculateExponent( const char * str ) 
{
    int16_t exponent = 0;
    uint8_t index = 0, decimalFound = 0, firstDigitFound = 0;

    // Handle possible sign
    if ( str[ index ] == '-' || str[ index ] == '+' ) 
    {
        index++;
    }

    // Count digits before decimal point for numbers >= 1
    while ( str[ index ] != '\0' && str[ index ] != '.' ) 
    {
        if ( str[ index ] != '0' && str[ index ] != '+' && str[ index ] != '-' ) 
        {
            firstDigitFound = 1;
        }
        
        if ( firstDigitFound )  
        {

            exponent++;
        }
        
        index++;
    }

    if ( str[ index ] == '.' ) 
    {
        decimalFound = 1;
    }

    // Adjust for numbers >= 1 (since exponent is one less than the number of digits)
    if ( exponent > 0 && decimalFound ) 
    {
        exponent--;
        if ( exponent == 0 ) return exponent;
    }
    else if ( exponent > 0 )
    {
        exponent -= 2;
    }

    // Handle numbers < 1
    if ( decimalFound && exponent == 0 ) 
    {
        exponent = -1;
        index++; // Move past decimal point
        while ( str[ index ] == '0' ) 
        {
            exponent--;
            index++;
        }
    }

    return exponent;
}



static int isScientific( const char * str ) 
{
    uint8_t index = 0;

    while ( str[ index ] != '\0' ) 
    {
        if ( str[ index ] == 'e' || str[ index ] == 'E' ) 
        {
            return true;
        }
        
        index++;
    }

    return false;
}

static int8_t addDigitToMantissa( uint8_t currentDigit, uint32_t * mantissa )
{
    if ( ( *mantissa / MANTISSA_TRESHOLD ) > 0 )
    {
        return EXIT_FAILURE;
    }
   
    *mantissa = *mantissa * 10 + currentDigit;

    return EXIT_SUCCESS;
}


static uint8_t countBinaryDigits( uint32_t mantissa ) 
{
    uint8_t count = 0;

    if ( mantissa > 0) 
    {
        // Count digits in mantissaHigh
        while ( mantissa > 0 ) 
        {
            mantissa /= 2;
            count++;
        }
    } 

    return count;
}

static uint8_t calculateMBFPositiveExponent( uint32_t mantissa )
{
    uint8_t result = 0;

    if ( mantissa != 0 )
    { 
        result = countBinaryDigits( mantissa );
        result += 128;
    }

    return result;
}

static void createMBFInt( uint32_t mantissa, int isNegative, uint8_t * ma4, uint8_t * ma3, uint8_t * ma2, uint8_t * ma1 )
{
    uint16_t mantissaHigh = 0;
    uint16_t mantissaLow = 0;

    if ( mantissa == 0 )
    {
        *ma4 = 0;
        *ma3 = 0;
        *ma2 = 0;
        *ma1 = 0;

        return; 
    } 

    mantissaLow = mantissa;
    mantissa >>= 16;
    mantissaHigh = mantissa; 

    *ma4 = ( uint8_t )( mantissaHigh >> 8 );
    *ma3 = ( uint8_t )( mantissaHigh & 0x00FF );
    *ma2 = ( uint8_t )( mantissaLow >> 8 );
    *ma1 = ( uint8_t )( mantissaLow & 0x00FF ); 

    while ( ( *ma4 & 0x80 ) == 0 && ( *ma4 != 0 || *ma3 != 0 || *ma2 != 0 || *ma1 != 0 ) ) 
    {
        *ma4 <<= 1;
        if ( *ma3 & 0x80 ) 
        {
            *ma4 |= 0x01;
        }

        *ma3 <<= 1;
        if ( *ma2 & 0x80 ) 
        {
            *ma3 |= 0x01;
        }

        *ma2 <<= 1;
        if ( *ma1 & 0x80 ) 
        {
            *ma2 |= 0x01;
        }

        *ma1 <<= 1;
    }

    *ma4 &= 0x7F;
    if ( isNegative ) *ma4 |= 0x80; 
}

static void mbfMantissaShiftRight( uint8_t * ma4, uint8_t * ma3, uint8_t * ma2, uint8_t * ma1, uint8_t shiftCount ) 
{
    uint8_t index = 0;

    for ( index = 0; index < shiftCount; index++ ) 
    {
        *ma1 >>= 1;
        if ( *ma2 & 0x01 ) 
        {
            *ma1 |= 0x80;
        }

        *ma2 >>= 1;
        if ( *ma3 & 0x01 ) 
        {
            *ma2 |= 0x80;
        }

        *ma3 >>= 1;
        if ( *ma4 & 0x01 ) 
        {
            *ma3 |= 0x80;
        }

        *ma4 >>= 1;
    }
}

static void createDecimalString( uint16_t mantissaHigh, uint16_t mantissaLow, int8_t isNegative, char * resultString  )
{
    uint32_t result = 0;

    if ( mantissaHigh > 0 )
    {
        result = mantissaHigh;
        result <<= 16;
        result |= mantissaLow;
        
        if ( isNegative )
        {
            snprintf( resultString, MAX_E_STRING_LENGTH, "-%ld", result  );
        }
        else
        {
            snprintf( resultString, MAX_E_STRING_LENGTH, "%ld", result  );
        }
    }
    else
    {
        if ( isNegative )
        {
            snprintf( resultString, MAX_E_STRING_LENGTH, "-%d", mantissaLow  );
        }
        else
        {
            snprintf( resultString, MAX_E_STRING_LENGTH, "%d", mantissaLow  );
        }
    }
}
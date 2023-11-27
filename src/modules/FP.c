#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <conio.h>

#include "FP.h"

int8_t FP_createFromString( const char * decimalStr, Float floatNumber )
{
    uint16_t mantissaLowInt = 0;
    int16_t mantissaHighInt = 0;
    uint16_t mantissaLowFraction = 0;
    int16_t mantissaHighFraction = 0;
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
        
        if ( parseMantissaInt( decimalStr, &mantissaHighInt, &mantissaLowInt ) == EXIT_FAILURE )
        {
            return EXIT_FAILURE;
        }

        // if ( parseMantissaFraction( decimalStr, &mantissaHighFraction, &mantissaLowFraction ) == EXIT_FAILURE )
        // {
        //     return EXIT_FAILURE;
        // }
    }

    if ( exponent >= 0 )
    {
        exponentMBF = calculateMBFPositiveExponent( mantissaHighInt, mantissaLowInt );
    }
    else
    {
        return EXIT_FAILURE;
    }
    
    printf( "1 [%d][%d]\n", mantissaHighInt, mantissaLowInt );
    printf( "2 [%04X][%04x]\n", mantissaHighInt, mantissaLowInt );

    printf( "IsNegative[%d]\n", isNegative );

    createMBFInt( mantissaHighInt, mantissaLowInt, isNegative, &ma4, &ma3, &ma2, &ma1 );

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
    return EXIT_FAILURE;
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

static int8_t parseMantissaInt( const char * str, int16_t * mantissaHighP, uint16_t * mantissaLowP )
{
    uint8_t index = 0;
    bool isCarry = false;
    uint8_t currentDigit = 0;
    int16_t mantissaHigh = 0;
    uint16_t mantissaLow = 0;
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
        
        if ( addDigitToMantissa( currentDigit, &mantissaHigh, &mantissaLow ) == EXIT_FAILURE ) 
        {
            return EXIT_FAILURE;
        }

       if ( ++index >= decimalStrLen - 1 ) break;
    }
    
    *mantissaHighP = mantissaHigh; 
    *mantissaLowP = mantissaLow;

    return EXIT_SUCCESS;
}

static int8_t parseMantissaFraction( const char * str, int16_t * mantissaHighP, uint16_t * mantissaLowP ) 
{
    uint8_t index = 0;
    bool isCarry = false;
    uint8_t currentDigit = 0;
    int16_t mantissaHigh = 0;
    uint16_t mantissaLow = 0;

    // Find the decimal point
    while ( str[ index ] != '\0' && str[ index ] != '.') 
    {
        index++;
    }
    
    if ( str[ index ] == '.' ) 
    {
        index++; // Move past the decimal point
    }

    while ( str[ index ] >= '0' && str[ index ] <= '9' ) 
    {
        currentDigit = str[ index ] - '0';
        
        if ( addDigitToMantissa( currentDigit, &mantissaHigh, &mantissaLow ) == EXIT_FAILURE ) 
        {
            return EXIT_FAILURE;
        }

        index++;
    }

    *mantissaHighP = mantissaHigh;
    *mantissaLowP = mantissaLow;

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

static int16_t parseExponent( const char * str ) 
{
    int16_t exponent = 0;
    int8_t sign = 1; // 1 for positive, -1 for negative
    uint8_t index = 0;

    // Find 'e' or 'E'
    while ( str[ index ] && str[ index ] != 'e' && str[ index ] != 'E' ) 
    {
        index++;
    }

    // If found, parse the exponent
    if ( str[ index ] == 'e' || str[ index ] == 'E' ) 
    {
        index++; // Move past 'e' or 'E'

        // Handle potential sign
        if ( str[ index ] == '-' ) 
        {
            sign = -1;
            index++;
        } 
        else if ( str[ index ] == '+' ) 
        {
            index++;
        }

        // Parse the number
        while ( str[ index ] >= '0' && str[ index ] <= '9' ) 
        {
            exponent = exponent * 10 + ( str[ index ] - '0' );
            index++;
        }

        exponent *= sign;
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

static int8_t addDigitToMantissa( uint8_t currentDigit, int16_t * mantissaHigh, uint16_t * mantissaLow )
{
    uint16_t oldLow = *mantissaLow;
    bool isMultiplied = false;
    *mantissaLow *= 10;

    // Check for overflow during multiplication
    if ( *mantissaLow / 10 != oldLow ) 
    {
        oldLow >>= 8;
        oldLow *= 10;
        oldLow = oldLow & 0xFF00;
        oldLow >>= 8;
        *mantissaHigh = *mantissaHigh * 10 + oldLow;
        isMultiplied = true;
    }

    oldLow = *mantissaLow;
    *mantissaLow += currentDigit;

    // Check for overflow during addition
    if ( *mantissaLow < oldLow ) 
    {
        ( *mantissaHigh )++;
    }

    if ( !isMultiplied )
    {
        *mantissaHigh *= 10;
    }

    // Check if mantissaHigh itself overflows
    if ( *mantissaHigh > INT16_MAX / 10 ) 
    {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

// static uint8_t countTrailingZeros( const char * str ) 
// {
//     uint8_t count = 0;
//     uint8_t i = 0;
//     uint8_t length = strlen( str ) - 1;

//     char reversedStr[ MAX_STRING_LENGTH ] = { '\0' };
    
//     for ( i = 0; i < length; i++ ) 
//     {
//         reversedStr[ i ] = str[ length - 1 - i ];
//     }

//     reversedStr[ length + 1 ] = '\0';
    
//     for ( i = 0; i < length; i++ ) 
//     {
//         if ( reversedStr[ i ] == '0' ) 
//         {
//             count++;
//         } 
//         else 
//         {
//             break;  
//         }
//     }

//     return count;
// }

// static void removeTrailingZeros( const char * str, int16_t * mantissaHigh, uint16_t * mantissaLow )
// {
//     uint8_t numberOfZeros = countTrailingZeros( str );
//     uint8_t index = 0;
//     uint8_t reminder = 0;

//     for ( ; index < numberOfZeros; index++ )
//     {
//         reminder = 0;
        
//         if ( *mantissaHigh != 0 )
//         {
//             reminder = *mantissaHigh % 10;
//             *mantissaHigh /= 10;
//             *mantissaLow /= 10;
//             *mantissaLow += reminder * DIGIT_PATTERN;
//         }
//         else
//         {
//             *mantissaLow /= 10;
//         }
//     }
// }

static uint8_t countBinaryDigits( uint16_t mantissaHigh, uint16_t mantissaLow ) 
{
    uint8_t count = 0;

    if ( mantissaHigh > 0) 
    {
        // Count digits in mantissaHigh
        while ( mantissaHigh > 0 ) 
        {
            mantissaHigh /= 2;
            count++;
        }
        // Add the bits of the lower part
        count += 16; // As mantissaHigh is non-zero, mantissaLow contributes 16 bits
    } 
    else 
    {
        // Count digits in mantissaLow if mantissaHigh is zero
        while ( mantissaLow > 0 ) 
        {
            mantissaLow /= 2;
            count++;
        }
    }

    return count;
}

static uint8_t calculateMBFPositiveExponent( int16_t mantissaIntHigh, uint16_t mantissaIntLow )
{
    uint8_t result = 0;

    if ( mantissaIntHigh != 0 || mantissaIntLow != 0 )
    { 
        result = countBinaryDigits( mantissaIntHigh, mantissaIntLow );
        result += 128;
    }

    return result;
}

static void createMBFInt( int16_t mantissaIntHigh, uint16_t mantissaIntLow, int isNegative, uint8_t * ma4, uint8_t * ma3, uint8_t * ma2, uint8_t * ma1 )
{
    printf( "3 [%04X][%04x]\n", mantissaIntHigh, mantissaIntLow );

    if ( mantissaIntHigh == 0 && mantissaIntLow == 0 )
    {
        *ma4 = 0;
        *ma3 = 0;
        *ma2 = 0;
        *ma1 = 0;

        return; 
    } 

    *ma4 = ( uint8_t )( mantissaIntHigh >> 8 );
    *ma3 = ( uint8_t )( mantissaIntHigh & 0x00FF );
    *ma2 = ( uint8_t )( mantissaIntLow >> 8 );
    *ma1 = ( uint8_t )( mantissaIntLow & 0x00FF ); 

    printf( "[%02X][%02X][%02X][%02X]\n", *ma4,*ma3, *ma2, *ma1 );

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
    printf( "2 isNegative[%d]\n", isNegative);
    if ( isNegative ) *ma4 |= 0x80; 
}
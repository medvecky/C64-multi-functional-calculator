#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "FP.h"

Float FP_createFromString( const char * decimalStr )
{
    Float result = ( uint16_t * ) calloc( FP_SIZE, sizeof( uint16_t ) );
    uint16_t mantissaLow = 0;
    uint16_t mantissaHigh = 0;
    uint16_t exponent = 0;
    int mantissaPosition = 0;
    char pointIndex = 0;
    char index = 0;
    char isNegative = 0;
    char isCarry = 0;
    char decimalStrLen = strlen( decimalStr );
    char numberOfTrailingZeros = 0;

    const char * charPtr = strchr( decimalStr, '.' );
    pointIndex = ( int )( charPtr - decimalStr );
    
    numberOfTrailingZeros = countTrailingZeros( decimalStr );
    
    if ( charPtr )
    {
        mantissaPosition = pointIndex - decimalStrLen + 2;
    } 

    mantissaPosition += numberOfTrailingZeros;
    charPtr = decimalStr;

    if ( *charPtr == '-' )
    {
        isNegative = 1;
        charPtr++;
    }

    while ( ( *charPtr >= '0' && *charPtr <= '9' ) || *charPtr == '.' )
    {
        int curcentDigit = ( *charPtr - '0' );
        
        char isMantissaLowOverflowed = ( ( mantissaLow / 10000 ) >= 1 ) || 
                ( ( _16_BIT_UNSIGNED_MAX - mantissaLow ) < curcentDigit );
        
        char isMantissaOverflowed = ( ( ( mantissaHigh * 10 ) + mantissaLow / 10000 ) > _16_BIT_SIGNED_MAX ) || 
                 ( ( mantissaLow % 10000 ) >= (_16_BIT_UNSIGNED_MAX_P ) ) ||
                ( ( _16_BIT_UNSIGNED_MAX - ( mantissaLow % 10000 ) * 10) < curcentDigit ); 

        if ( *charPtr == '.' )
        { 
            charPtr++;
            continue;
        }

        if ( !isCarry )
        {
            if ( isMantissaLowOverflowed )
            {
                isCarry = 1;
                mantissaHigh =  mantissaLow / 10000;
                mantissaLow = ( mantissaLow % 10000 ) * 10 + curcentDigit; 
            }
            else
            {
                mantissaLow = mantissaLow * 10 + curcentDigit;
            }
        }
        else
        {
            if ( isMantissaOverflowed )
            {
                result[ MANTISSA_HIGH ] = 0xFFFF;
                result[ MANTISSA_LOW ] = 0xFFFF; 
                result[ EXPONENT ] = 0xFFFF;

                return result;
            }
            
            mantissaHigh = ( mantissaHigh * 10 ) + mantissaLow / 10000;
            mantissaLow = ( mantissaLow % 10000 ) * 10 + curcentDigit; 
        }    

        charPtr++;
        
        if ( ++index >= decimalStrLen ) break;
    }

    if ( numberOfTrailingZeros > 0 )
    {
        for ( index = 0; index < numberOfTrailingZeros; index++ )
        {
            char remainder = 0;
            
            if ( mantissaHigh > 0 )
            {
                remainder = mantissaHigh % 10;
                mantissaHigh /= 10;
            }

            mantissaLow /= 10;

            if ( remainder > 0 )
            {
                mantissaLow += remainder * 10000;
            }
        }
    }

    if ( isNegative )
    {
        mantissaHigh |= 0x8000;
    }

    result[ EXPONENT ] = mantissaPosition;
    result[ MANTISSA_HIGH ] = mantissaHigh;
    result[ MANTISSA_LOW ] = mantissaLow; 

    return result;
}

void FP_delete( Float floatNumber )
{
    free( floatNumber );
}

char* FP_toString( const Float value )
{
    char isNegative = value[ MANTISSA_HIGH ] >> 15;
    uint16_t exponent = value[ EXPONENT ];
    uint16_t mantissaHigh = value[ MANTISSA_HIGH ];
    uint16_t mantissaLow = value[ MANTISSA_LOW ];
    char pointPosition = 0;
    char mantissaStrLen = 0;
    char wholePartLen = 0;
    char fractionPartLen = 0;
    char isEUsed = 0;
    char eCorection = 0;

    char mantissaStr[ MAX_STRING_LENGTH ] =  { '\0' };
    char decimalStrWholePart[ MAX_STRING_LENGTH ] = { '\0' };
    char decimalStrFractionPart[ MAX_STRING_LENGTH ] = { '\0' };
    char resultString[ MAX_STRING_LENGTH ] = { '\0' };
    
    if ( isNegative )
    {
        mantissaHigh &= 0x7FFF;
        
        if (mantissaHigh > 0)
        { 
            snprintf( mantissaStr, sizeof( mantissaStr ), "-%u%u", mantissaHigh, mantissaLow );
        }
        else
        {
            snprintf( mantissaStr, sizeof( mantissaStr ), "-%u", mantissaLow );
        }
        
        mantissaStrLen = strlen( mantissaStr ) - 1;
    }
    else
    {
        if (mantissaHigh > 0)
        { 
            snprintf( mantissaStr, sizeof( mantissaStr ), "%u%u", mantissaHigh, mantissaLow );
        }
        else
        {
            snprintf( mantissaStr, sizeof( mantissaStr ), "%u", mantissaLow );
        }

         mantissaStrLen = strlen( mantissaStr );
    }

    mantissaStrLen = strlen( mantissaStr );
    pointPosition = mantissaStrLen + exponent;
    
    if ( pointPosition > mantissaStrLen )
    {
        if ( isNegative )
        {
            eCorection = mantissaStrLen - 1;
            wholePartLen = 2;
            pointPosition = 2;
            isEUsed = 1;
        }
        else
        {
            eCorection = mantissaStrLen;
            wholePartLen = 1;
            pointPosition = 1;
            isEUsed = 1;
        }
    }
    else if ( pointPosition == mantissaStrLen )
    {
        wholePartLen = mantissaStrLen;
    }
    else
    {
        wholePartLen = pointPosition;
    }
    
    strncpy( decimalStrWholePart, mantissaStr, wholePartLen );
    fractionPartLen = mantissaStrLen - wholePartLen;
    strncpy( decimalStrFractionPart, mantissaStr + pointPosition, fractionPartLen );
    
    if ( fractionPartLen > 0 && !isEUsed)
    {
        snprintf(
            resultString, 
            sizeof( mantissaStr ), 
            "%s.%s", 
            decimalStrWholePart, 
            decimalStrFractionPart );
    }
    else if ( isEUsed )
    {
        snprintf( 
            resultString, 
            sizeof( mantissaStr ), 
            "%s.%se%d", 
            decimalStrWholePart, 
            decimalStrFractionPart, 
            exponent + eCorection - 1 );
    }
    else
    {
        snprintf( 
            resultString, 
            sizeof( mantissaStr ), 
            "%s", 
            decimalStrWholePart );
    }
    
    return strdup( resultString );
}

static int countTrailingZeros( const char * str ) 
{
    char count = 0;
    char i = 0;
    char length = strlen( str ) - 1;

    char reversedStr[ MAX_STRING_LENGTH ] = { '\0' };
    
    for ( i = 0; i < length; i++ ) 
    {
        reversedStr[ i ] = str[ length - 1 - i ];
    }

    reversedStr[ length + 1 ] = '\0';
    
    for ( i = 0; i < length; i++ ) 
    {
        if ( reversedStr[ i ] == '0' ) 
        {
            count++;
        } 
        else 
        {
            break;  
        }
    }

    return count;
}
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
    char index = 0;
    char isNegative = 0;
    char isCarry = 0;
    char decimalStrLen = strlen( decimalStr );

    const char * charPtr = strchr( decimalStr, '.' );
    index = ( int )( charPtr - decimalStr );
    
    if ( charPtr )
    {
        mantissaPosition = index - decimalStrLen + 2;
    } 

    charPtr = decimalStr;

    if ( *charPtr == '-' )
    {
        isNegative = 1;
        charPtr++;
    }

    while ( ( *charPtr >= '0' && *charPtr <= '9' ) || *charPtr == '.' )
    {
        if ( *charPtr == '.' ) charPtr++;

        if ( !isCarry && mantissaHigh <= _16_BIT_SIGNED_MAX )
        {
            if ( (mantissaHigh * 10 + ( *charPtr - '0' )) > _16_BIT_SIGNED_MAX )
            {
                isCarry = 1;
                mantissaLow = ( *charPtr - '0' );
            }
            else
            {
                mantissaHigh = mantissaHigh * 10 + ( *charPtr - '0' );
            }
            
            charPtr++;
        }
        else
        {
            mantissaLow = mantissaLow * 10 + ( *charPtr - '0' );
            charPtr++;
        }    
    }

    if (isNegative)
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

    char mantissaStr[ MAX_STRING_LENGTH ] =  { '\0' };
    char decimalStrWholePart[ MAX_STRING_LENGTH ] = { '\0' };
    char decimalStrFractionPart[ MAX_STRING_LENGTH ] = { '\0' };
    char resultString[ MAX_STRING_LENGTH ] = { '\0' };
    
    if ( isNegative )
    {
        mantissaHigh &= 0x7FFF;
        
        if (mantissaLow > 0)
        { 
            snprintf( mantissaStr, sizeof( mantissaStr ), "-%d%d", mantissaHigh, mantissaLow );
        }
        else
        {
            snprintf( mantissaStr, sizeof( mantissaStr ), "-%d", mantissaHigh );
        }
        
        mantissaStrLen = strlen( mantissaStr ) - 1;
    }
    else
    {
        if (mantissaLow > 0)
        { 
            snprintf( mantissaStr, sizeof( mantissaStr ), "%d%d", mantissaHigh, mantissaLow );
        }
        else
        {
            snprintf( mantissaStr, sizeof( mantissaStr ), "%d", mantissaHigh );
        }

         mantissaStrLen = strlen( mantissaStr );
    }

    mantissaStrLen = strlen( mantissaStr );
    pointPosition = mantissaStrLen + exponent;
    
    if ( pointPosition == mantissaStrLen )
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
    
    if ( fractionPartLen > 0 )
    {
        snprintf(   resultString, sizeof( mantissaStr ), "%s.%s", decimalStrWholePart, decimalStrFractionPart );
    }
    else
    {
        snprintf(   resultString, sizeof( mantissaStr ), "%s", decimalStrWholePart );
    }
    

    return strdup( resultString );
}
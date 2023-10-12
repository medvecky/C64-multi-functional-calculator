#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "modules/system_helper.h"
#include "modules/FP.h"

#define MAX_NUMBER_LENGTH 20

int main ( void )
{   
   Float floatNumber;
   char inputStriung[ MAX_NUMBER_LENGTH ] = { '\0' };
   char * outputString = NULL;

   setUpScreen();

   fgets( inputStriung, MAX_NUMBER_LENGTH, stdin );
   floatNumber = FP_createFromString( inputStriung );

   printf( 
      "MantissaL=0x%04X MantissaH=0x%04X Exponent=0x%04X\n",
      floatNumber[ MANTISSA_LOW ], 
      floatNumber[ MANTISSA_HIGH ], 
      floatNumber[ EXPONENT ] );
   printf( 
      "MantissaL=%d MantissaH=%d Exponent=%d\n",
      floatNumber [ MANTISSA_LOW ], 
      floatNumber[ MANTISSA_HIGH ],
      floatNumber[ EXPONENT ] );

   puts( "FP_toString result:" );
   outputString = FP_toString( floatNumber );
   puts( outputString );
   
   cgetc();  

   FP_delete( floatNumber );
   
   if ( outputString ) free( outputString );

   resetDefaultScreen();
   
   return EXIT_SUCCESS;
}

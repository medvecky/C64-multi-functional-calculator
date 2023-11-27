#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "modules/system_helper.h"
#include "modules/FP.h"

#define MAX_NUMBER_LENGTH 20

int main ( void )
{   
   Float floatNumber1 = ( uint8_t * ) calloc( FP_SIZE, sizeof( uint8_t ) );

   if ( floatNumber1 == NULL )
   {
      puts( "Failed to alloc memory" );
      return EXIT_FAILURE;
   }

   char inputString[ MAX_NUMBER_LENGTH ] = { '\0' };

   setUpScreen();

   fgets( inputString, MAX_NUMBER_LENGTH, stdin );
   if ( FP_createFromString( inputString, floatNumber1 ) == EXIT_FAILURE )
   {
      puts( "FP creating failed!" );
      return EXIT_FAILURE;
   }

   printf( 
      "[%02X][%02X][%02X][%02X][%02X]\n",
      floatNumber1[ EXPONENT ],
      floatNumber1[ MANTISSA_4 ],
      floatNumber1[ MANTISSA_3 ],
      floatNumber1[ MANTISSA_2 ],
      floatNumber1[ MANTISSA_1 ]);

   cgetc();  

   FP_delete( floatNumber1 );

   resetDefaultScreen();
   
   return EXIT_SUCCESS;
}

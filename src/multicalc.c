#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "modules/system_helper.h"
#include "modules/FP.h"

int main ( void )
{   
   Float floatNumber1 = ( uint8_t * ) calloc( FP_SIZE, sizeof( uint8_t ) );
   char inputString[ MAX_E_STRING_LENGTH ] = { '\0' };
   char outputstring[ MAX_E_STRING_LENGTH ] = { '\0' };

   if ( floatNumber1 == NULL )
   {
      puts( "Failed to alloc memory" );
      return EXIT_FAILURE;
   }

   setUpScreen();

   fgets( inputString, MAX_E_STRING_LENGTH, stdin );
   if ( FP_createFromString( inputString, floatNumber1 ) == EXIT_FAILURE )
   {
      puts( "FP creating failed!" );
      return EXIT_FAILURE;
   }

   printf( 
      "FP: [%02X][%02X][%02X][%02X][%02X]\n",
      floatNumber1[ EXPONENT ],
      floatNumber1[ MANTISSA_4 ],
      floatNumber1[ MANTISSA_3 ],
      floatNumber1[ MANTISSA_2 ],
      floatNumber1[ MANTISSA_1 ]);

   FP_toString( floatNumber1, outputstring );   
   printf( "toString result: %s\n", outputstring );

   cgetc();  

   FP_delete( floatNumber1 );

   resetDefaultScreen();
   
   return EXIT_SUCCESS;
}

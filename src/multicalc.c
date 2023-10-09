#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "modules/system_helper.h"

int main ( void )
{   
   setUpScreen();

   puts("Super cool calculator programm.");
   cgetc();  

   resetDefaultScreen();
   
   return EXIT_SUCCESS;
}

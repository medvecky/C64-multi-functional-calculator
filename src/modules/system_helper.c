#include <c64.h> 
#include <conio.h>

unsigned char defaultBGColor = 0;
unsigned char defaultBorderColor = 0;
unsigned char defaultTextColor = 0;

void setUpScreen( void )
{
   clrscr();
   defaultBGColor = bgcolor( COLOR_BLACK );
   defaultBorderColor = bordercolor( COLOR_BLACK );
   defaultTextColor = textcolor( COLOR_GREEN );
}

void resetDefaultScreen( void )
{
   clrscr();
   bgcolor( defaultBGColor );
   bordercolor( defaultBorderColor );
   textcolor( defaultTextColor );
   * ( char *  ) 0xD018 = 0x15;
}
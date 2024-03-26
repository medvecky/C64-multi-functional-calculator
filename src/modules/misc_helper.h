#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LENGTH 80

void header( void );
void handleArgumentString( char * argumentString );
void handleNumber( char * token );
void handleOperator( char * token );
void handleAddition( void );
void showResult( void );
void handleSubtraction( void );
void handleMultiplication( void );
void handleDivision( void );
void handlePower( void );
void getUserInput( char * argumentString );

#include <stdint.h>
#include <stdlib.h>

#include "modules/system_helper.h"
#include "modules/misc_helper.h"
#include "modules/adt_stack.h"

StackNodePtr stackPtr = NULL;


int main ( void )
{

    char argumentString[ MAX_INPUT_LENGTH ];
    char quitFlag = ' ';
    
    setUpScreen();
    
    header();

    getUserInput( argumentString );
    
    quitFlag = argumentString[ 0 ];

    while ( quitFlag != 'q' )
    {
        handleArgumentString( argumentString );
        
        getUserInput( argumentString );

        quitFlag = argumentString[ 0 ];
    } 

    resetDefaultScreen();

    return EXIT_SUCCESS;
}
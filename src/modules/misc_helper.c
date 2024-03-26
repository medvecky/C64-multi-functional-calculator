#include "misc_helper.h"
#include "math_helper.h"  
#include "system_helper.h"
#include "adt_stack.h" 

extern char arg1[ MAX_ARGUMENT_LENGTH ];
extern uint8_t argLength;
extern StackNodePtr stackPtr;
extern FP arg1Fp;
extern FP arg2Fp;
extern FP resultFp;

void header( void )
{
    cputsxy( 13, 0, "CALCULATOR." );
    gotoxy( 0, 2 );
    puts( "Enter an expression for RPN calculation" );
    puts( "or 'q' for exit" );
    puts( "" );
    puts( "" );
}

void handleArgumentString( char * argumentString )
{
    char * token;
    token = strtok( argumentString, " " ); 

    while ( token != NULL ) 
    {
        if ( isdigit( ( unsigned char )token[ 0 ] ) ) 
        {
            handleNumber( token );
        } 
        else 
        {
            if ( ( unsigned char )token[ 0 ] == '-' && isdigit( ( unsigned char )token[ 1 ] ) )
            {
                handleNumber( token );
            } 
            else 
            {
                handleOperator( token );
            }
        }
        
        token = strtok( NULL, " " );
    }

    printf( "" );

    if ( Stack_isEmpty( stackPtr ) )
    {
        puts( "Stack is empty" );
    }
    else
    {
        showResult();
    }
}

void handleNumber( char * token )
{
    if ( strlen( token ) >= sizeof( arg1 ) ) 
    {
        puts( "Number is too long" );
        return;
    }
    
    strncpy( arg1, token, sizeof( arg1 ) - 1 );
    arg1[ sizeof( arg1 ) - 1 ] = '\0';

    argLength = strlen( arg1 );

    enableBasicRom();
    FP_arg1ToFp();
    disableBasicRom();

    Stack_push( &stackPtr, arg1Fp );
}

void handleOperator( char * token )
{
    char operator = token[ 0 ];

    if ( Stack_isEmpty( stackPtr ) )
    {
        puts( "Stack is empty" );
    }
    else
    {
        switch ( operator )
        {
            case '+' :
                handleAddition();
                Stack_push( &stackPtr, resultFp );       
                break;

            case '-' :
                handleSubtraction();
                Stack_push( &stackPtr, resultFp );
                break;

            case '/' :
                handleDivision();
                Stack_push( &stackPtr, resultFp );
                break;

            case '*' :
                handleMultiplication();
                Stack_push( &stackPtr, resultFp );
                break;

            case 0x5e :  
                handlePower();
                Stack_push( &stackPtr, resultFp );
                break; 
       
            default:
                puts( "Invalid operator" );
                break;
        }
    } 
}

void handleAddition( void )
{
    
    Stack_pop( &stackPtr, &arg1Fp );
    Stack_pop( &stackPtr, &arg2Fp );  
    
    enableBasicRom();
    FP_add();
    FP_facToResult();  
    disableBasicRom();
}

void showResult( void )
{
    Stack_pop( &stackPtr, &resultFp );
    enableBasicRom();
    FP_printResult();
    disableBasicRom();
    puts( "" );
}

void handleSubtraction( void )
{
    Stack_pop( &stackPtr, &arg1Fp );
    Stack_pop( &stackPtr, &arg2Fp );  
    
    enableBasicRom();
    FP_subst();
    FP_facToResult();  
    disableBasicRom();
}

void handleMultiplication( void )
{
    Stack_pop( &stackPtr, &arg1Fp );
    Stack_pop( &stackPtr, &arg2Fp );  
    
    enableBasicRom();
    FP_mult();
    FP_facToResult();  
    disableBasicRom();
}

void handleDivision( void )
{
    Stack_pop( &stackPtr, &arg1Fp );
    Stack_pop( &stackPtr, &arg2Fp );  
    
    enableBasicRom();
    FP_div();
    FP_facToResult();  
    disableBasicRom();
}

void handlePower( void )
{
    Stack_pop( &stackPtr, &arg1Fp );
    Stack_pop( &stackPtr, &arg2Fp );  
    
    enableBasicRom();
    FP_pwr();
    FP_facToResult();  
    disableBasicRom();
}

void getUserInput( char * argumentString )
{
    uint8_t len = 0;
    printf( "> " );
    
    if ( fgets( argumentString, MAX_INPUT_LENGTH, stdin ) == NULL ) 
    {
        puts( "Error reading input" );
        cgetc();
        resetDefaultScreen();
        exit( EXIT_FAILURE );
    }

    len = strlen( argumentString );
    
    if ( len > 0 && argumentString[ len - 1 ] == '\n' ) 
    {
        argumentString[ --len ] = '\0';   
    }      
}

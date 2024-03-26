#include <string.h>

#include "adt_stack.h"
#include "math_helper.h"

void Stack_push( StackNodePtr * topPtr, FP value )
{
    StackNodePtr newPtr;

    newPtr = malloc( sizeof( StackNode ) );
    
    if ( newPtr != NULL )
    {
        memcpy( newPtr->data, value, sizeof( FP ) );
        newPtr->nextPtr = *topPtr;
        *topPtr = newPtr;
    } 
    else 
    {
        FP_printRaw( value );
        puts( "not inserted. No memory available.\n" );
    } 
}  

void Stack_print( StackNodePtr currentPtr )
{
    if ( currentPtr == NULL )
    {
        puts( "The stack is empty" );
    } 
    else
    {
        puts( "The stack is: " );

        while ( currentPtr != NULL ) 
        {
            printf( "--> " );
            FP_printRaw( currentPtr->data );
            currentPtr = currentPtr->nextPtr;
        } 

        puts( "NULL\n" );
    } 
} 

void Stack_pop( StackNodePtr * topPtr, FP * popValue )
{
    StackNodePtr tempPtr;

    if ( Stack_isEmpty( *topPtr ) ) 
    {
        puts( "Stack is empty. Cannot pop value." );
        return;
    }

    tempPtr = *topPtr;
    memcpy( *popValue, ( *topPtr )->data, sizeof( FP ) );
    *topPtr = ( *topPtr )->nextPtr;
    free( tempPtr );
} 

bool Stack_isEmpty( StackNodePtr topPtr )
{
    return topPtr == NULL;
} 

void Stack_getTop( StackNodePtr topPtr, FP * topValue)
{
    if ( Stack_isEmpty( topPtr ) ) 
    {
        puts( "Stack is empty. Cannot pop value." );
        return;
    }

    memcpy( *topValue, topPtr->data, sizeof( FP ) );
} 

void Stack_free( StackNodePtr * topPtr )
{
    StackNodePtr tempPtr = *topPtr;
    StackNodePtr nextPtr;
    while ( tempPtr != NULL )
    {
        nextPtr = ( *tempPtr ).nextPtr;
        free( tempPtr );
        tempPtr = nextPtr;
    }
}


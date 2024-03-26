#ifndef ATD_STACK_H
#define ATD_STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "math_helper.h"

struct stackNode
{
    FP data;
    struct stackNode *nextPtr; 
};

typedef struct stackNode StackNode;
typedef StackNode *StackNodePtr;

void Stack_push( StackNodePtr * topPtr, FP value );
void Stack_print( StackNodePtr currentPtr );
void Stack_pop( StackNodePtr * topPtr, FP * popValue );
bool Stack_isEmpty( StackNodePtr topPtr );
void Stack_getTop( StackNodePtr topPtr, FP * topValue );
void Stack_free( StackNodePtr * topPtr );

#endif
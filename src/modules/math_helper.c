#include <stdio.h>

#include "math_helper.h"

uint8_t argLength = 0;
FP resultFp = { 0, 0, 0, 0, 0 };
FP arg1Fp = { 0, 0, 0, 0, 0 };
FP arg2Fp = { 0, 0, 0, 0, 0 };
char arg1[ MAX_ARGUMENT_LENGTH ];
char arg2[ MAX_ARGUMENT_LENGTH ];

void FP_arg1ToFp( void )
{
    __asm__ ( "lda  #<%v", arg1 );
    __asm__ ( "sta  $22" );
    __asm__ ( "lda  #>%v", arg1 );
    __asm__ ( "sta  $23" );
    __asm__ ( "lda  #<%v", argLength );
    __asm__ ( "jsr  $b7b5" );
    
    __asm__ ( "ldx  #<%v", arg1Fp );
    __asm__ ( "ldy  #>%v", arg1Fp );
    __asm__ ( "jsr  $bbd4" );
}

void FP_arg2ToFp( void )
{
    __asm__ ( "lda  #<%v", arg2 );
    __asm__ ( "sta  $22" );
    __asm__ ( "lda  #>%v", arg2 );
    __asm__ ( "sta  $23" );
    __asm__ ( "lda  #<%v", argLength );
    __asm__ ( "jsr  $b7b5" );
    
    __asm__ ( "ldx  #<%v", arg2Fp );
    __asm__ ( "ldy  #>%v", arg2Fp );
    __asm__ ( "jsr  $bbd4" );
}

void FP_printResult( void )
{
    __asm__ ( "lda  #<%v", resultFp );
    __asm__ ( "ldy  #>%v", resultFp );
    __asm__ ( "jsr  $bba2" );
    __asm__ ( "jsr  $bddd" );
    __asm__ ( "jsr  $ab1e" );
}

void FP_arg1ToFac( void )
{
    __asm__ ( "lda  #<%v", arg1Fp );
    __asm__ ( "ldy  #>%v", arg1Fp );
    __asm__ ( "jsr  $bba2" );
}

void FP_arg2ToFac( void )
{
    __asm__ ( "lda  #<%v", arg2Fp );
    __asm__ ( "ldy  #>%v", arg2Fp );
    __asm__ ( "jsr  $bba2" );
}

void FP_add( void )
{
    FP_arg1ToFac();
    __asm__ ( "lda  #<%v", arg2Fp );
    __asm__ ( "ldy  #>%v", arg2Fp );
    __asm__ ( "jsr  $b867" );

}

void FP_facToResult( void )
{
    __asm__ ( "ldx  #<%v", resultFp );
    __asm__ ( "ldy  #>%v", resultFp );
    __asm__ ( "jsr  $bbd4" );
}

void FP_printRaw( FP fp )
{
    printf( "0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", fp[ 0 ], fp[ 1 ], fp[ 2 ], fp[ 3 ], fp[ 4 ] );
}

void FP_subst( void )
{
    FP_arg1ToFac();
    __asm__ ( "lda  #<%v", arg2Fp );
    __asm__ ( "ldy  #>%v", arg2Fp );
    __asm__ ( "jsr  $b850" );
}

void FP_mult( void )
{
    FP_arg1ToFac();
    __asm__ ( "lda  #<%v", arg2Fp );
    __asm__ ( "ldy  #>%v", arg2Fp );
    __asm__ ( "jsr  $ba28" );
}

void FP_div( void )
{
    FP_arg1ToFac();
    __asm__ ( "lda  #<%v", arg2Fp );
    __asm__ ( "ldy  #>%v", arg2Fp );
    __asm__ ( "jsr  $bb0f" );
}

void FP_pwr( void )
{
    FP_arg2ToFac();
    FP_FACtoARG();
    FP_arg1ToFac();
    __asm__ ( "jsr  $bf7b" );
} 

void FP_FACtoARG( void )
{
     __asm__ ( "jsr  $bc0f" );
}
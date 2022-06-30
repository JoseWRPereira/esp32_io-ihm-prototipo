#include <stdio.h>
#include "driver/gpio.h"
#include "keyboard.h"
#include "serialIO_keyboard.h"

#define NUM_COL         4
#define NUM_LIN         4

#define L0_MASK         0x01
// #define C0_MASK         0x10
#define C0_MASK         0x01

unsigned char teclIn = 0;
unsigned char teclOut = 0;

void keyboard_init( void )
{
    keyb_serialIO_init( &teclIn, &teclOut, 1 );
}

const char tecladoMatriz[4][4] = {  {'1','2','3','A'},
                                    {'4','5','6','B'},
                                    {'7','8','9','C'},
                                    {'*','0','#','D'} };

unsigned char keyboard( void )
{
    unsigned char aux = 0;
    unsigned char l,c;
    
    for( l=0; l<NUM_LIN; l++ )
    {
        teclOut = (unsigned char)(L0_MASK << l);
        keyb_serialIO_scan();
        keyb_serialIO_scan();
        keyb_serialIO_scan();
        for( c=0; c<NUM_COL; c++ )
        {
//            keyb_serialIO_scan();
            if( teclIn & (C0_MASK<<c) )
                aux = tecladoMatriz[c][l];
        }
    }   
    return( aux );
}
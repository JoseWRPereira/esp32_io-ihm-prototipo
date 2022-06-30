#include <stdio.h>
#include "driver/gpio.h"
#include "serialIO_keyboard.h"


// #define SIO_KEYB_CK_PIN      4
// #define SIO_KEYB_DO_PIN      16
// #define SIO_KEYB_LD_PIN      2
// #define SIO_KEYB_DI_PIN      15

#define SIO_KEYB_CK_PIN      12
#define SIO_KEYB_DO_PIN      27
#define SIO_KEYB_LD_PIN      14
#define SIO_KEYB_DI_PIN      13

#define SIO_KEYB_LD( x )     gpio_set_level( SIO_KEYB_LD_PIN, x )
#define SIO_KEYB_DO( x )     gpio_set_level( SIO_KEYB_DO_PIN, x )
#define SIO_KEYB_CK( x )     gpio_set_level( SIO_KEYB_CK_PIN, x )
#define SIO_KEYB_DI()        gpio_get_level( SIO_KEYB_DI_PIN )

unsigned char * keyb_ptrSerialIn;
unsigned char * keyb_ptrSerialOut;
unsigned char   keyb_lenSerialIO;


void keyb_serialIOload( void )
{
    SIO_KEYB_LD( 0 );
    SIO_KEYB_LD( 1 );
}

unsigned char keyb_serialIObyteShift( unsigned char dataIn )
{
    unsigned char dataBit;
    unsigned char dataOut;

    dataBit = 0x80;
    dataOut = 0;

    while( dataBit )
    {
        SIO_KEYB_CK( 0 );
        if( dataIn & dataBit )
            SIO_KEYB_DO( 1 );
        else
            SIO_KEYB_DO( 0 );

        if( SIO_KEYB_DI() )
            dataOut |= dataBit;
        SIO_KEYB_CK( 1 );
        dataBit >>= 1;
    }
    SIO_KEYB_CK( 0 );
    return( dataOut );
}

//***************** Interface Serial IO Scan
void keyb_serialIO_scan( void )
{
    unsigned char i,j;
    j = keyb_lenSerialIO - 1;
    keyb_serialIOload();
    for( i=0; i<keyb_lenSerialIO; i++ )
    {
        keyb_ptrSerialIn[i] = keyb_serialIObyteShift( keyb_ptrSerialOut[j-i]);
    }
    keyb_serialIOload();
}

//***************** Inicialiação de interface com SerialIO
void keyb_serialIO_init( unsigned char * ptrIn, unsigned char * ptrOut, unsigned char length )
{
    keyb_ptrSerialIn = ptrIn;
    keyb_ptrSerialOut = ptrOut;
    keyb_lenSerialIO = length;
// ************************** Serial IO Shift/Load
    gpio_reset_pin( SIO_KEYB_LD_PIN );
    gpio_set_direction( SIO_KEYB_LD_PIN, GPIO_MODE_OUTPUT );

// ************************** Serial IO Clock
    gpio_reset_pin( SIO_KEYB_CK_PIN );
    gpio_set_direction( SIO_KEYB_CK_PIN, GPIO_MODE_OUTPUT );

// ************************** Serial IO Output
    gpio_reset_pin( SIO_KEYB_DO_PIN );
    gpio_set_direction( SIO_KEYB_DO_PIN, GPIO_MODE_OUTPUT );

// ************************** Serial IO Input
    gpio_reset_pin( SIO_KEYB_DI_PIN );
    gpio_set_direction( SIO_KEYB_DI_PIN, GPIO_MODE_INPUT );
    
//***************** Inicialização dos pinos de interface com os CIs
    SIO_KEYB_CK( 0 );
    SIO_KEYB_DO( 0 );
    SIO_KEYB_LD( 1 );
}

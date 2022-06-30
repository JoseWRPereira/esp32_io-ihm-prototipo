/*
 * File:   serialIO.c
 * Author: curtaeletronica
 *
 * Created on 30 de Setembro de 2019, 19:36
 * 
 * Objetivo: 
 *      Camada de Abstração de Hardware:
 *          Interface com o CIs 74595 e 74165.
 * 
 * Pinos    |nº     |Conexão
 * RE0      |8      | SHLD : Shift / Load
 * RE1      |9      | CLK : Clock
 * RE2      |10     | OUT : Output
 * RE3      |1      | IN : Input
 */

#include <stdio.h>
#include "driver/gpio.h"
#include "serialIO.h"

// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// void sio_delay( unsigned int t )
// {
//     vTaskDelay( t / portTICK_PERIOD_MS);
//     // vTaskDelay( pdMS_TO_TICKS( t ));
// }

// #define SIO_CK_PIN      17
// #define SIO_DO_PIN      18
// #define SIO_LD_PIN      5
#define SIO_DI_PIN      15

#define SIO_CK_PIN      22
#define SIO_DO_PIN      21
#define SIO_LD_PIN      19

#define SIO_LD( x )     gpio_set_level( SIO_LD_PIN, x )
#define SIO_DO( x )     gpio_set_level( SIO_DO_PIN, x )
#define SIO_CK( x )     gpio_set_level( SIO_CK_PIN, x )
#define SIO_DI()        gpio_get_level( SIO_DI_PIN )

unsigned char * ptrSerialIn;
unsigned char * ptrSerialOut;
unsigned char   lenSerialIO;


void serialIOload( void )
{
    SIO_LD( 0 );
    SIO_LD( 1 );
}

unsigned char serialIObyteShift( unsigned char dataIn )
{
    unsigned char dataBit;
    unsigned char dataOut;

    dataBit = 0x80;
    dataOut = 0;

    while( dataBit )
    {
        SIO_CK( 0 );
        if( dataIn & dataBit )
            SIO_DO( 1 );
        else
            SIO_DO( 0 );

        if( SIO_DI() )
            dataOut |= dataBit;
        SIO_CK( 1 );
        dataBit >>= 1;
    }
    SIO_CK( 0 );
    return( dataOut );
}

//***************** Interface Serial IO Scan
void serialIO_scan( void )
{
    unsigned char i,j;
    j = lenSerialIO - 1;
    serialIOload();
    for( i=0; i<lenSerialIO; i++ )
    {
        ptrSerialIn[i] = serialIObyteShift( ptrSerialOut[j-i]);
    }
    serialIOload();
}

//***************** Inicialiação de interface com SerialIO
void serialIO_init( unsigned char * ptrIn, unsigned char * ptrOut, unsigned char length )
{
    ptrSerialIn = ptrIn;
    ptrSerialOut = ptrOut;
    lenSerialIO = length;
// ************************** Serial IO Shift/Load
    gpio_reset_pin( SIO_LD_PIN );
    gpio_set_direction( SIO_LD_PIN, GPIO_MODE_OUTPUT );

// ************************** Serial IO Clock
    gpio_reset_pin( SIO_CK_PIN );
    gpio_set_direction( SIO_CK_PIN, GPIO_MODE_OUTPUT );

// ************************** Serial IO Output
    gpio_reset_pin( SIO_DO_PIN );
    gpio_set_direction( SIO_DO_PIN, GPIO_MODE_OUTPUT );

// ************************** Serial IO Input
    gpio_reset_pin( SIO_DI_PIN );
    gpio_set_direction( SIO_DI_PIN, GPIO_MODE_INPUT );
    
//***************** Inicialização dos pinos de interface com os CIs
    SIO_CK( 0 );
    SIO_DO( 0 );
    SIO_LD( 1 );
}

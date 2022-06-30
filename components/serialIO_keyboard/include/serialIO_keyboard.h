#ifndef SERIALIO_KEYBOARD_H
#define	SERIALIO_KEYBOARD_H

void keyb_serialIO_init( unsigned char * ptrIn, unsigned char * ptrOut, unsigned char length );
void keyb_serialIO_scan( void );

#endif

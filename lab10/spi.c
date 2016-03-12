#include <avr/io.h>
#include <util/delay.h>
#include "integer.h"

void init_spi(void){
    DDRB = ( 1 << PB7 ) | ( 1 << PB5 ) | ( 1 << PB4 );
    SPCR = ( 1 << SPE ) | ( 1 << MSTR) | ( 1 << SPR1); //SPI Enable+Master+f=f_osc/64
    SPSR = 0;
}

void dly_100us(void){
    _delay_us(100);
}

BYTE rcv_spi(void){
    BYTE data;

    SPDR = 0xff;
    while ( ! ( SPSR & ( 1 << SPIF ) ) );
    data = SPDR;

    return data;
}

BYTE xmit_spi(BYTE data){
    SPDR = data;
    while ( ! ( SPSR & ( 1 << SPIF ) ) );
    data = SPDR;

    return data;
}



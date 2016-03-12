#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd_lib.h"
#define SS PB4
#define DD_MOSI PB5
#define DD_MISO PB6
#define DD_SCK PB7
#define SET_SS PORTB|=(1<<SS)
#define CLR_SS PORTB&=~(1<<SS)
#define DDR_SPI DDRB

void SPI_MasterInit(void)
{
/* Set MOSI and SCK output, all others input */
DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK)|(1<<SS);
/* Enable SPI, Master, set clock rate fck/16 */
SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

uint8_t SPI_MasterTransmit(char cData)
{
/* Start transmission */
SPDR = cData;
/* Wait for transmission complete */
while(!(SPSR & (1<<SPIF)));
return SPDR;
	
}



int main(void){
	SPI_MasterInit();
	lcdinit();
	_delay_ms(1000);
	goto_xy(0,0);
	//lcd_text("hello world");
	
	
	

	while(1){
	CLR_SS;
	SPI_MasterTransmit(1);
	uint16_t tbyte, lbyte;
	tbyte=SPI_MasterTransmit((1<<7)|(1<<5));
	lbyte=SPI_MasterTransmit(0);
	SET_SS;
	uint32_t data, temp1, temp2;
	tbyte=(tbyte& 0x0F)<<8;
	data = tbyte + lbyte;
	temp1=(data*1221)/10000;
	temp2=(data*1221)%10000;
	lcd_int(temp1);
	//goto_xy(4,0);
	lcd_text(",");
	lcd_int(temp2);
	lcd_text(" C");
	_delay_ms(1000);
	lcd_clr();
	}
	
	return 0;
}
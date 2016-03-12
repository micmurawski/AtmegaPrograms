#include <avr/io.h>
#include <avr/interrupt.h>
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1 //71
#include <util/delay.h>
volatile uint8_t znak=0;

void USART_Init(unsigned int baud){
/* Set baud rate */
UBRRH = (unsigned char)(baud>>8);
UBRRL = (unsigned char)baud;
/* Enable receiver and transmitter */
UCSRB = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
/* Set frame format: 8data, 2stop bit */
UCSRC = (1<<URSEL)|(0<<USBS)|(3<<UCSZ0)|(0<<UMSEL);
//UCSRC = (3<<UCSZ0);
}

void USART_Transmit( unsigned char data )
{
/* Wait for empty transmit buffer */
while ( !( UCSRA & (1<<UDRE)) )
;
/* Put data into buffer, sends the data */
UDR = data;
}

unsigned char USART_Receive( void )
{
/* Wait for data to be received */
while ( !(UCSRA & (1<<RXC)) )
;
/* Get and return received data from buffer */
return UDR;
}




void send_text(char *text)
{
uint8_t i=0;
while(1){
if(text[i]==0x0000){
break;
}
else{
USART_Transmit(text[i]);
i++;
}

}
}

ISR (USART_RXC_vect){
znak=USART_Receive();

}

ISR(INT0_vect){
		switch_led();
		_delay_ms(500);
	}

void switch_led(void){
if(PINC & (1<<PC0)){

send_text("led on");
USART_Transmit('\n');
PORTC=(0<<PC0);

}
else {

PORTC=(1<<PC0);
send_text("led of");
USART_Transmit('\n');



}
}

int main(void){
DDRC=0x01;
PORTC=0x01;
DDRD&=~(1<<PD2);
GICR|=(1<<INT0);
MCUCR|=(1<<ISC01);
MCUCR&=~(1<<ISC00);
sei();
USART_Init(MYUBRR);





while(1){
if(znak=='a'){
switch_led();
znak=0;
}
}

return 0;

}
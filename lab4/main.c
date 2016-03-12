#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t anode=0;
volatile uint8_t digit[4]={8,9,9,9};

ISR(TIMER1_COMPA_vect){

digit[0]++;
if(digit[0]>9)
{
digit[0]=0;
digit[1]++;
if(digit[1]>9){
digit[1]=0;
digit[2]++;
if(digit[2]>9){
digit[2]=0;
digit[3]++;
if(digit[3]>9){
digit[3]=0;
}

}
}
}
}

ISR(TIMER0_COMP_vect){
anode++;
if(anode>3) anode=0;
PORTA=~(1<<anode);
if(digit[anode]==0)PORTC=0b11000000;
if(digit[anode]==1)PORTC=0b11111001;
if(digit[anode]==2)PORTC=0b10100100;
if(digit[anode]==3)PORTC=0b10110000;
if(digit[anode]==4)PORTC=0b10011001;
if(digit[anode]==5)PORTC=0b10010010;
if(digit[anode]==6)PORTC=0b10000010;
if(digit[anode]==7)PORTC=0b11111000;
if(digit[anode]==8)PORTC=0b10000000;
if(digit[anode]==9)PORTC=0b10010000;



}

int main(void){


DDRA=0XFF;
DDRC=0xff;
PORTC=0x00;


TCCR0|=(1<<WGM01);
TCCR0|=(1<<CS02)|(1<<CS00);
OCR0=20;
TIMSK|=(1<<OCIE0);

TCCR1B|=(1<<WGM12)|(1<<CS12)|(1<<CS10);
OCR1A=10800;

TIMSK|=(1<<OCIE1A);
sei();


while(1){
}
return 0;
}
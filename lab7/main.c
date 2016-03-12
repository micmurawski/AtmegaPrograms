#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


int main(void){
TCCR1B=(1<<WGM13)|(1<<WGM12)|(1<<CS11);
TCCR1A=(1<<WGM11)|(0<<WGM10)|(1<<COM1A1)|(1<<COM1B1);
TCCR0|=(1<<WGM01)|(1<<WGM00)|(1<<COM01)|(1<<COM00)|(1<<CS01);
ICR1=255; //27647
//1244,16
//2903,04
DDRD|=(1<<PD5)|(1<<PD4);
DDRB|=(1<<PB3);

while(1){

for(uint8_t i=0;i<255;i++){
OCR0=0;
OCR1B=0+i;
OCR1A=255-i;
_delay_ms(7);
}
for(uint8_t i=0;i<255;i++){
OCR0=0;
OCR1B=255-i;
OCR1A=0+i;
_delay_ms(7);
}
for(uint8_t i=0;i<255;i++){
OCR0=0+i;
OCR1B=0;
OCR1A=0;
_delay_ms(7);
}



}
return 0;
}
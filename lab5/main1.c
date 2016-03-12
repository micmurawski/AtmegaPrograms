#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#define LED1 (1<<PD4)
#define LED2 (1<<PD5)

volatile int8_t kierunek=1;
int main(void){
DDRD|=LED1|LED2;
TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);
TCCR1B|=(1<<WGM13)|(1<<WGM12)|(CS10);
ICR1=10000000;
OCR1A=5000000;
TCCR0|=(1<<CS01);
TIMSK|=(1<<TOIE0);
sei();
for(;;)
//TCCR0|=(1<<WGM01)|(1<<WGM00)|(1<<COM01)|(1<<COM00)|(1<<CS01);


return 0;
}
ISR(TIMER0_OVF_vect){
	if(OCR1A==10000000 || OCR1A==0) kierunek*=-1;
	OCR1A+=kierunek;
	OCR1B-=kierunek;
}
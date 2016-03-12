#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd_lib.h"
#define CLK 11059200
#define F_UP !(PINC& (1<<6))
#define F_DOWN !(PINC& (1<<5))
#define DC_UP !(PINC& (1<<4))
#define DC_DOWN !(PINC& (1<<3))
#define SWEEP !(PINC& (1<<2))


volatile int16_t freq=1;
volatile int16_t dc=50;
volatile int8_t sweep=0;

int main(void){
DDRC&=~(1<<PC6);

//TCCR1A|=(1<<WGM11)|(1<<COM1A1)|(1<<COM1B1);
//TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS10);

TCCR1B=(1<<WGM13)|(1<<WGM12)|(1<<CS12);

TCCR1A=(1<<WGM11)|(0<<WGM10)|(1<<COM1A1)|(1<<COM1B1);


TCCR0|=(1<<WGM01);
TCCR0|=(1<<CS02)|(1<<CS00);
OCR0=215;
TIMSK|=(1<<OCIE0);

//TCCR0|=(1<<WGM01)|(1<<WGM00)|(1<<COM01)|(1<<COM00)|(1<<CS01);
//1244,16
//2903,04

ICR1=2160;
OCR1A=((uint32_t)dc*ICR1)/100;
DDRD|=(1<<PD5)|(1<<PD4);
DDRB|=(1<<PB3);

lcdinit();
_delay_ms(1000);
goto_xy(0,0);


while(1){
sei();

if(sweep==1){

if(freq<200){
freq+=1;
ICR1=21600/freq;
OCR1A=((uint32_t)dc*ICR1)/100;
_delay_ms(20);
}
else if(freq==200){
sweep=0;
freq=1;
ICR1=21600/freq;
OCR1A=((uint32_t)dc*ICR1)/100;

}

}

lcd_text("frequency: ");
lcd_int(freq);
lcd_text(" Hz");
goto_xy(0,1);
lcd_text("duty cycle: ");
lcd_int(dc);
lcd_text("%");
_delay_ms(20);
lcd_clr();




}

return 0;
}



ISR(TIMER0_COMP_vect){

if(F_UP && freq<200){
freq+=1;
ICR1=21600/freq;
OCR1A=((uint32_t)dc*ICR1)/100;
}

if(F_DOWN && freq>1){
freq-=1;
ICR1=21600/freq;
OCR1A=((uint32_t)dc*ICR1)/100;
}


if(DC_UP && dc<100){
dc+=1;
OCR1A=((uint32_t)dc*ICR1)/100;
}

if(DC_DOWN && dc>0){
dc-=1;
OCR1A=((uint32_t)dc*ICR1)/100;
}

if(SWEEP){
sweep=1;
freq=1;
}


}
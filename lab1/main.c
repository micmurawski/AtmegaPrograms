#include <avr/io.h>
#include <util/delay.h>
#define KEY_DOWN !(PIND& (1<<6))

int main(void){

	uint8_t i=0;
	
	DDRA=0XFF;
	DDRC|=0x0F;
	DDRD&=~(1<<PD6);
	PORTA=0xFF;
	PORTC=0x03;
	
	while(1){
	
	
		if((i==0) && (KEY_DOWN))
	{		i=1;
		while(1){
		_delay_ms(10);
		PORTC = (PORTC<<1 | PORTC>>3); //prawo
		if((i==1) && (KEY_DOWN)) break;
		}
	}
	else if((i!=0) && !(KEY_DOWN)){
		i++;
		while(1){
		_delay_ms(10);
		PORTC = (PORTC>>1 | PORTC<<3); //lewo
		if((i!=1) && (KEY_DOWN)) break;
		}
		
	}
					
	}
			


		return 0;	
	
}

//	while(1){
//zad2
//	
//	PORTA=0xFF;
//	if(KEY_DOWN){
//		PORTA=~(1<<PA0);
//	}
//	else{
//		
//	}

// zad1	
//	for(uint8_t i=0;i<8;i++){		
//		_delay_ms(500);
//		PORTA=~(1<<i);		
//	}
	
	
//	}



// zad3	
//	if((i==0) && (KEY_DOWN))
//	{
//		i=1;
//		//PORTA^=0xFF;
//		PORTA^=(1<<PA0);
//	}
//	else if((i!=0) && !(KEY_DOWN)){
//		i++;
//	}
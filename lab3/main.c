#include <avr/io.h>
#include <avr/interrupt.h>

//define KEY_DOWN !(PIND& (1<<2))

volatile uint8_t z=0;

	ISR(INT0_vect){
		z=z+1;
	}	
	

int main(void){
	
	DDRA=0XFF;
	
	DDRD&=~(1<<PD2);
	
	MCUCR|=(1<<ISC01);
	MCUCR&=~(1<<ISC00);
	
	GICR|=(1<<INT0);
	//PORTA=0xFF;
	
	sei();
	


	
	while(1){

	PORTA=~z;
	
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
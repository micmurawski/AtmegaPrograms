#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd_lib.h"

int main(void){
	
	lcdinit();
	goto_xy(0,0);
	lcd_text("hello");
	//lcd_text();
	//lcd_text();
	//lcd_int(10);
	while(1);
	return 0;
}



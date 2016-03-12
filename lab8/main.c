#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd_lib.h"
#define SCL 40000
#define SLA_W 0b11010000
#define SLA_R 0b11010001
#define SECOND 0x70

void start(void){
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}

void stop(void){
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}




uint8_t read_byte(void){
	//TWDR = SLA_W;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	
	return TWDR;
}


void write_byte(uint8_t DATA){
	TWDR = DATA;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}


void i2c_send(uint8_t addr, uint8_t data){
	start();
	write_byte(SLA_W);
	write_byte(addr);
	write_byte(data);
	stop();
}

uint8_t i2c_recive(uint8_t addr){
	start();
	write_byte(SLA_W);
	write_byte(addr);
	start();
	write_byte(SLA_R);
	uint8_t temp=read_byte();
	stop();
	return temp;
}

int main(void){
uint8_t day,mounth,year,hours,minutes,seconds;

TWBR=3;
TWSR|=(1<<TWPS1)|(1<<TWPS0);


i2c_send(0x00,0);
i2c_send(0x02,0b00001010);
lcdinit();
_delay_ms(100);
goto_xy(0,0);

while(1){

seconds=i2c_recive(0x00);
minutes=i2c_recive(0x01);
hours=i2c_recive(0x02);
day=i2c_recive(0x04);
mounth=i2c_recive(0x05);
year=i2c_recive(0x06);

lcd_int((hours&0x70)>>4);
lcd_int((hours&0x0F));
lcd_text(":");
lcd_int((minutes&0x70)>>4);
lcd_int((minutes&0x0F));
lcd_text(":");
lcd_int((seconds&0x70)>>4);
lcd_int((seconds&0x0F));
goto_xy(0,1);

lcd_int((day&0x70)>>4);
lcd_int((day&0x0F));
lcd_text(".");
lcd_int((mounth&0x70)>>4);
lcd_int((mounth&0x0F));
lcd_text(".");
lcd_text("20");
lcd_int((year&0x70)>>4);
lcd_int((year&0x0F));
lcd_text("r");
_delay_ms(500);
lcd_clr();


}
return 0;
}
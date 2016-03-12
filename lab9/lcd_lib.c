#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define LCD_PORT_DDR DDRA
#define LCD_PORT PORTA
#define LCD_RS 0
#define LCD_E 1
#define LCD_D4 2
#define LCD_D5 3
#define LCD_D6 4
#define LCD_D7 5

#define SET_RS LCD_PORT|=(1<<LCD_RS)
#define SET_E LCD_PORT|=(1<<LCD_E)

#define CLR_RS LCD_PORT&=~(1<<LCD_RS)
#define CLR_E LCD_PORT&=~(1<<LCD_E)

static inline void lcd_send_half_byte(uint8_t data){
	SET_E;
	if(data & (1<<0)) LCD_PORT|=(1<<LCD_D4); else LCD_PORT&=~(1<<LCD_D4);
	if(data & (1<<1)) LCD_PORT|=(1<<LCD_D5); else LCD_PORT&=~(1<<LCD_D5);
	if(data & (1<<2)) LCD_PORT|=(1<<LCD_D6); else LCD_PORT&=~(1<<LCD_D6);
	if(data & (1<<3)) LCD_PORT|=(1<<LCD_D7); else LCD_PORT&=~(1<<LCD_D7);
	CLR_E;
}

void lcd_write_byte(uint8_t data){
	lcd_send_half_byte(data>>4);
	_delay_us(120);
	lcd_send_half_byte(data);
	_delay_us(120);
	
}

void lcd_write_cmd(uint8_t cmd){
	CLR_RS;
	lcd_write_byte(cmd);
}

void lcd_write_data(uint8_t data){
	SET_RS;
	lcd_write_byte(data);
}

void lcdinit(void){
	LCD_PORT_DDR|=(1<<LCD_RS);
	LCD_PORT_DDR|=(1<<LCD_E);
	LCD_PORT_DDR|=(1<<LCD_D4)|(1<<LCD_D5)|(1<<LCD_D6)|(1<<LCD_D7);
	LCD_PORT&=~(1<<LCD_RS);
	LCD_PORT&=~(1<<LCD_E);
	LCD_PORT=0x00;
	_delay_ms(45);
	lcd_send_half_byte(3);
	_delay_ms(4.1);
	lcd_send_half_byte(3);
	_delay_us(100);
	lcd_send_half_byte(3);
	_delay_us(100);
	lcd_send_half_byte(0x02);
	_delay_ms(3);
	lcd_write_cmd(0x28);
	_delay_ms(3);
	lcd_write_cmd(1<<3);
	_delay_ms(3);
	lcd_write_cmd(1<<0);
	_delay_ms(3);
	lcd_write_cmd(3<<1);
	_delay_ms(3);
	lcd_write_cmd(0x0F);
	_delay_ms(1);
	
}

void lcd_clr(void){
	lcd_write_cmd(1<<0);
	_delay_ms(5);
}

void lcd_cursor_return(void){
	lcd_write_cmd(1<<1);
	_delay_ms(5);
}

void lcd_text(char* str){
	while(*str) lcd_write_data(*str++);
}

void lcd_int(int16_t z){
	char buf[10];
	lcd_text(itoa(z,buf,10));
}
void goto_xy(uint8_t x, uint8_t y){
	uint8_t addr=0;
	if(y==0) addr=0x00+x;
	if(y==1) addr=0x40+x;
	lcd_write_cmd((1<<7)|addr);
}
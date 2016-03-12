#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define LCD_PORT_DDR DDRD
#define LCD_PORT PORTD
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
	_delay_ms(120);
	lcd_send_half_byte(data);
	_delay_ms(120);
	
}

void lcd_write_cmd(uint8_t cmd){
	CLR_RS;
	lcd_write_byte(cmd);
	
}

void lcd_write_data(uint8_t data){
	SET_RS;
	lcd_write_byte(data);
}
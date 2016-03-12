#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd_lib.h"
#include "diskio.h"
#include "pff.h"
#define DS_OUT DDRB|=(1<<PB3) 
#define DS_LOW PORTB&=~(1<<PB3) 
#define DS_IN DDRB&=~(1<<PB3) 
#define KEY_DOWN !(PIND& (1<<6))

FATFS fs;
BYTE buffer[512];
WORD br;
FRESULT result;

void end(void){
	_delay_ms(2000);
	goto_xy(0,0);
	pf_mount(NULL);
	lcd_text("DEVICE UNMOUNTED");

}


uint8_t init(void){
	DS_LOW;
	DS_OUT;
	_delay_us(480);
	DS_IN;
	_delay_us(60);
	uint8_t state = PINB&(1<<PB3);
	_delay_us(420);
	
	return state;
}

void write_bit(bit){
	if(bit==0){
		DS_LOW;
		DS_OUT;
		_delay_us(60);
		DS_IN;
		uint8_t state = PINB&(1<<PB3);
	}else if(bit==1){
		DS_LOW;
		DS_OUT;
		_delay_us(5);
		DS_IN;
		_delay_us(55);
	
	}
}

uint8_t read_bit(void){
	DS_OUT;
	DS_LOW;
	
	_delay_us(1);
	DS_IN;
	_delay_us(14);
	uint8_t state = PINB&(1<<PB3);
	_delay_us(45);
	if(state) return 1; else return 0;

}


void write_byte(uint8_t data){
	for(uint8_t i=0;i<8;i++){
		if(data&(1<<i)) write_bit(1); else write_bit(0);
	}

}

uint8_t read_byte(void){
	uint8_t data=0;
	for(uint8_t i=0;i<8;i++){
		if(read_bit())data|=(1<<i);
	}
	return data;
}

int main(void){
	DDRD&=~(1<<PD6);
	lcdinit();
	_delay_ms(500);
	goto_xy(0,0);
	result = pf_mount(&fs);
	if(result==FR_OK) lcd_text("DEVICE MOUNTED");
	if(result==FR_NO_FILESYSTEM) lcd_text("No vaild FAT partition on file disck");
	if(result==FR_DISK_ERR) lcd_text("Error in the disck function");
	if(result==FR_NOT_READY) lcd_text("Could not be initialized due to a disk error or no medium");
	_delay_ms(1000);
	result= pf_open("/info/opis.txt");
	_delay_ms(2000);
	
	if(result){
		goto_xy(0,0);
		lcd_text("Error opening file");
		end();
	}
	
	result=pf_read(buffer,15,&br);
	if(br!=15){
		goto_xy(0,0);
		lcd_text("Error reading  file");
		end();
	}
	
	goto_xy(0,0);
	lcd_text(buffer);
	_delay_ms(5000);
	lcd_clr();
	
	result= pf_open("/data/dane.txt");
	_delay_ms(2000);
	
	if(result){
		goto_xy(0,0);
		lcd_text("Error opening file");
		end();
	}
	
	result=pf_read(buffer,1000,&br);
	
	
	goto_xy(0,0);
	lcd_text(buffer);
	_delay_ms(5000);
	lcd_clr();
	
	

	while(1){
		
		if(!init()){
			
			write_byte(0xCC); //SKIP ROM
			write_byte(0x44); //CONVERT T
			while(!read_bit());
			init();
			write_byte(0xCC);
			write_byte(0xBE);  //READ SCRATCHPAD
			uint16_t LSB = read_byte();
			uint16_t MSB = read_byte();
			
			init();
			uint16_t T = (LSB>>4)+((MSB&(0x07))<<4);
			uint16_t T2 = (LSB&0x0F)*625;
			
			goto_xy(0,0);
			lcd_text("Temp ");
			lcd_int(T);
			lcd_text(".");
			if((LSB&0x0F)==1) lcd_int(0);
			lcd_int(T2);
			//lcd_text(" C");
			_delay_ms(500);
			
			
			uint8_t slen;
			sprintf(buffer,"Temp %d.%d \n",T,T2);
			slen = strlen(buffer);
			result = pf_write(buffer,slen, &br);
			if(br!=slen){
				goto_xy(0,0);
				lcd_text("Error writting file");
				end();
			}
			
			if(KEY_DOWN){
			result=pf_write(0,0,&br);
			end();
			lcd_clr();
			goto_xy(0,0);
			lcd_text("KEY DOWN");
			_delay_ms(1000);
			break;
			}
			//while(1);
			
		}else if(init()){ 
		goto_xy(0,0);
		lcd_text("brak czujnika");
		_delay_ms(500);
		}
	
	}

	return 0;
}
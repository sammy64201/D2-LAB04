/*
 * D2-Lab-04.c
 *
 * Created: 2/18/2025 4:34:26 PM
 * Author : samue
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "I2C/I2C.h"
#include "LCD/LCD.h"

uint8_t direccion;
uint8_t temp;
uint8_t bufferI2C;
uint8_t valorI2C;
// DEFINICION DE SLAVES 
#define slave1 0x30
#define slave2 0x29
uint8_t pot1;
uint8_t contador;
float volt1;
char buffer[16];
char buffer_c[16];



void setup(){
	init_LCD();
	I2C_Master_Init(100000,1);
	
	LCD_CURSOR(0, 0);
	LCD_STR("S1    S2");
	
}




int main(void)
{
	setup();
	
	
    
    while (1) 
    {
		
		
		// MUESTREO EN LCD
		volt1 = (pot1 / 255.0) * 5.0;
		LCD_CURSOR(0, 1);
		dtostrf(volt1, 4, 2, buffer);
		LCD_STR(buffer);
		LCD_STR("    ");
		// MUESTREO DE CONTADOR EN LCD
		LCD_CURSOR(6, 1);
		itoa(contador, buffer_c, 10);
		LCD_STR(buffer_c);
		LCD_STR("    ");
		
		
		PORTB |=(1<<PORTB5);
		I2C_Master_Start();
		// SLAVE 1
		bufferI2C = slave1 << 1 & 0b11111110;
		
		temp = I2C_Master_Write(bufferI2C);
		if(temp != 1){
			I2C_Master_Stop();
			} else{
			I2C_Master_Write('R');
			I2C_Master_Stop();
		}
		_delay_ms(500);
		PORTB &=~(1<<PORTB5);
		I2C_Master_Start();
		
		//leo
		bufferI2C = slave1 << 1 | 0b00000001;
		temp = I2C_Master_Write(bufferI2C);
		if(temp != 1){
			I2C_Master_Stop();
			}else{
			TWCR = (1<<TWINT)|(1<<TWEN);
			while(!(TWCR & (1<<TWINT)));
			
			//pot1 = TWDR;
			valorI2C = TWDR;
			pot1 = valorI2C;
			
			I2C_Master_Stop();
			//pot1 = valorI2C;
		}
		
		// PIDO SLAVE 2
		PORTB |=(1<<PORTB5);
		I2C_Master_Start();
		// SLAVE 1
		bufferI2C = slave2 << 1 & 0b11111110;
		temp = I2C_Master_Write(bufferI2C);
		if(temp != 1){
			I2C_Master_Stop();
			} else{
			I2C_Master_Write('R');
			I2C_Master_Stop();
		}
		_delay_ms(500);
		PORTB &=~(1<<PORTB5);
		I2C_Master_Start();
		
		// LEO SLAVE 2
		bufferI2C = slave2 << 1 | 0b00000001;
		temp = I2C_Master_Write(bufferI2C);
		if(temp != 1){
			I2C_Master_Stop();
			}else{
			TWCR = (1<<TWINT)|(1<<TWEN);
			while(!(TWCR & (1<<TWINT)));
			
			//pot1 = TWDR;
			valorI2C = TWDR;
			contador = valorI2C;
			
			I2C_Master_Stop();
			//pot1 = valorI2C;
		}
		
		
    }
}


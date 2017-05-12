/*
 * pir_prototype.c
 *
 * Created: 5/8/2017 5:01:05 PM
 *  Author: Ahmux
 *
 * PIR sensor on PINB1
 * LDR sensor on PINC0
 * RELAY OUTPUT on PIND4
 */ 




//****************************************************
// Target: ATMEGA48
// Code by virgoptrex (c) 2008
// Read ADC from ATMEGA48 datasheet for more understanding on ADC operation
// Note: while porting this code to atmega128 you may need to take into account ADFR (free running) in ADCSRA.
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define LTHRES 700

void adc_init()
{
	// AREF = AVcc
	ADMUX = (1<<REFS0);
	// ADC Enable and prescaler of 128
	// 16000000/128 = 125000
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}


uint16_t adc_read(uint8_t ch)
{
	// select the corresponding channel 0~7
	// ANDing with '7' will always keep the value
	// of 'ch' between 0 and 7
	ch &= 0b00011111; // AND operation with 7
	ADMUX |= ch;
	// start single convertion
	// write '1' to ADSC
	ADCSRA |= (1<<ADSC);
	// wait for convertion to complete
	// ADSC becomes '0' again
	// till then, run loop continuously
	while(ADCSRA & (1<<ADSC));
	return (ADC);
}

int main (void)
{
	adc_init();
	
    DDRB = 0x01;   // PORTB0 will be an OUTPUT (LED), PORTB7-1 will be INPUTS(PIR signal PORTB1)
	DDRD = 0b11110000;  // PORD 4-7 will be an OUTPUT, Output will be on PD4
	int x=0;

	uint16_t adc_result0;
//	char int_buffer[10];
	
	while(1)
	{
		adc_result0 = adc_read(0); // read adc value at PA0
		
		// condition for led to glow
		if (adc_result0 < LTHRES){
		PORTB = 0x00;
		PORTD = 0b00000000;}
		else{
			if (PINB & (1<<PINB1)){
				PORTB = 0x01;
				x = 1 ;
			}
			}
			
		if (x == 1){
			PORTD = 0b00010000;
			x=0;
		}
	
	}
}
//***************************************************************
//---------------------------------------------------------------












/*
#include <avr/io.h>

#include <avr/interrupt.h>
#include <stdio.h>

#define LUX 700  //sensitivity of LDR when a shadow dropped on it

// LDR connected to pin26
void adc_init()
{
	// AREF = AVcc
	ADMUX = (1<<REFS0);
	// ADC Enable and prescaler of 128
	// 16000000/128 = 125000
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

uint16_t adc_read(uint8_t ch)
{
	// select the corresponding channel 0~7
	// ANDing with '7' will always keep the value of 'ch' between 0 and 7
	ch &= 0b00011111; // AND operation with PIND4
	ADMUX |= ch;
	// start single conversion
	// write '1' to ADSC
	ADCSRA |= (1<<ADSC);
	// wait for conversion to complete
	// ADSC becomes '0' again
	// till then, run loop continuously
	while(ADCSRA & (1<<ADSC));
	return (ADC);
}


int main (void)
{
	DDRB = 0x00;   // PORTB all pins are INPUTS (PIR signal PORTB1)
	DDRD = 0b11110000;  // PORD 4-7 will be an OUTPUT, Output will be on PD4
	// DDRD = DDRD | B11111100; it sets pins 2 to 7 as outputs without changing the value of pins 0 & 1, which are RX & TX
	adc_init();
	
	uint16_t adc_result0;
	//	char int_buffer[10];
	int x = 0;
	
	while(1)
	{
		if (adc_result0 < LUX){
			PORTD = 0b10000000;
			if (PINB & (1<<PINB1)){
				x = 1 ;
			}
		}
		if (x == 1){
			PORTD = 0b00000000;
			}else{
			PORTD = 0b00010000;
		}
		
		/*
		adc_result0 = adc_read(0); // read adc value at PC0 (LDR sensor)
		while (PINB & (1<<PINB1)){     //check if PINB1 is 1 or 0 >>> PIR sensor
			PORTD = 0b10000000;
			if (adc_result0 < LUX){
				PORTD = 0b00000000;
				}else{
				PORTD = 0b00010000;
			}
*/
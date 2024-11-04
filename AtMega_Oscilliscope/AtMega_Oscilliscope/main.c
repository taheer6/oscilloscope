/*
 * AtMega_Oscilliscope.c
 *
 * Created: 2024-10-29 2:19:54 PM
 * Author : khant
 */ 

#define F_CPU 16000000UL // CPU @ 16000 MHz

#include <avr/io.h>
#include <util/delay.h>	

#define USART_BAUDRATE 9600 // Set Baud rate
#define BAUD_PRESCALER ((F_CPU / (USART_BAUDRATE * 16UL)) -1)

// Setting data, parity and stop bits and transmit and receive to initialize uart module for atm328p
// No parity bits used as prioritizing speed of transmission over accuracy 
void USART_INIT(void) 
{
	//Set Baud Rate
	UBRR0H = (uint8_t)(BAUD_PRESCALER >> 8); // High Byte
	UBRR0L = (uint8_t)BAUD_PRESCALER;
	
	// Frame Format: 8 data bits no parity bit, 1 stop bit
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	// Enable transmit and receive
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
	
}

void USART_TRANSMIT(char data)
{
	while(!(UCSR0A & (1 << UDRE0))); // check if transmit buffer is empty
	UDR0 = data; // put data into buffer for transmission
}


// Configuring ADC 
void ADC_INIT(void)
{
	//set reference voltage to 5V and select A0
	ADMUX = (1 << REFS0);
	
	// Set ADC Prescaler to 128 for 16 MHz clock
	ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	ADCSRA |= (1 << ADEN); //Enabling ADC

}

uint16_t ADC_READ(void)
{
	ADMUX &= 0xF0; // clear bits to make sure on A0
	
	//start conversion
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC)); // waits for conversion
	return ADC; // returns 10 bit conversion
}


// Transmitting ADC
void Transmit_ADC_Value(uint16_t adc_value)
{
	// Send the ADC value over UART
	USART_TRANSMIT((adc_value >> 8) & 0xFF) //high byte
	USART_TRANSMIT(adc_value & 0xFF) // low byte
}

int main (void)
{
	USART_INIT(); // Initialize USART communication
	ADC_INIT(); //Initialize ADC
	int count = 10;
	
    //while (count > 0) 
    //{
		//test
	//	USART_TRANSMIT('A');  // Send character 'A' repeatedly
	//	_delay_ms(1000);  // 1-second delay
	//	count--;
    //}
	// infinite loop standard in embedded programming -- usually no exit conditions as micro controllers 
	// have no OS to handle what happens after it stops
	while (1)
	{
		uint16_t adc_value = ADC_Read(); // read value
		Transmit_ADC_Value(adc_value); // transmit ADC value over UART
		_delay_ms(10) // delay to manage data rate
	}
}


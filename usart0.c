#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t buff, bp = 0, newqnh = 0;

uint8_t qnh[4]={"1013"};

void init_USART0()
{
    UBRR0H = 0;
    UBRR0L = 103;          
   
    UCSR0B |= (1<<RXEN0)|(1<<TXEN0); 
    
    UCSR0C = 0x06;
   
}

void USART0_transmit(uint8_t data)
{
    while (!(UCSR0A & (1<<UDRE0)))  
        ;
    UDR0 = data;                    
}


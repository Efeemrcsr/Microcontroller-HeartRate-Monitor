#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t buff, bp = 0, newqnh = 0;

uint8_t qnh[4]={"1013"};

void init_USART0()
{
    UBRR0H = 0;
    UBRR0L = 103;           // 9600 at 16MHz
    //UBRR1H = 0;
    //UCSR0B |= (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);   // RX, TX, RXI enable
    UCSR0B |= (1<<RXEN0)|(1<<TXEN0); // RX, TX enable
    //UCSR0B |= (1<<RXEN0)|(1<<RXCIE0);   // RX, RXI enable
    //UCSR1C |= (1<<UCSZ11)|(1<<UCSZ10);  // 8 bits DATA
    UCSR0C = 0x06;
    //DDRD |= (1<<3);
}

// Blocking transmit of one byte: wait for the data register to be empty,
// then load it. Used to send the PPG samples as ASCII to the Serial Plotter.
void USART0_transmit(uint8_t data)
{
    while (!(UCSR0A & (1<<UDRE0)))  // wait until UDR0 is ready for new data
        ;
    UDR0 = data;                    // transmit
}


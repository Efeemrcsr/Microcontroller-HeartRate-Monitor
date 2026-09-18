#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "display.h"

volatile uint8_t lcdptr=0, lcdcmd=0xC0, lcdoff=0;
unsigned char LCDline[32]={"MAX30102                        "};
//                          0123456789ABCDEF0123456789ABCDEF

void init_display()
{
    DDRC  |= 0xC7;
    PORTC = 0x80;	// power on
    _delay_ms(2000);
	PORTC = 0xC0;	// backlight on
    
    PORTC &= 0xF8;
	_delay_ms(100);
	
	DDRA = 0xFF;
	
    PORTA = 0x38;
    PORTC |= 0x04;
    PORTC &= 0xFB;
    _delay_ms(2);
    PORTA = 0x0C;
    PORTC |= 0x04;
    PORTC &= 0xFB;
    _delay_ms(2);
    PORTA = 0x01;
    PORTC |= 0x04;
    PORTC &= 0xFB;  
    _delay_ms(2);
    
    TCCR0B |= (1<<CS02)|(1<<CS00);  // prescaler 1024
    TCNT0 = 0;                      // initialize counter
    TIMSK0 |= (1 << TOIE0);	        // enable overflow interrupt 
}

ISR(TIMER0_OVF_vect)
{
    if(lcdptr==16) {
        PORTC &= 0xFE;
        PORTA = lcdcmd;
        lcdptr = 0;
        lcdcmd ^= 0x40;
        lcdoff ^= 0x10;        
    }
    else {
        PORTC |= 0x01;
        PORTA = LCDline[lcdoff+lcdptr];
        PORTC |= 0x10;
        lcdptr++;
    }
    PORTC |= 0x04;
    PORTC &= 0xFB;
}


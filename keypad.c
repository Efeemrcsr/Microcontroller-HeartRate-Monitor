#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t pressed, keycode;

void init_keypad()
{
    PCICR |= (1<<PCIE0);
    PCMSK0 |= (1<<PCINT0) ; // enable PCINT0
    pressed = 0;
}

uint8_t getkey(uint8_t* key)
{
	if(pressed) {
		*key = keycode;
		pressed = 0;
		return(1);
	}
	else {
		return(0);
	}
	
}

ISR(PCINT0_vect)
{
	if((PINB&0x01)==0x01) {
		keycode = PINL&0x0F;
		pressed = 1;
	}
}

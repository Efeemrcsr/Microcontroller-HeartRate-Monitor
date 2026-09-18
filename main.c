//EE308 TERM PROJECT
//Group 11
//22608 - Efe Emir Cesur
//33952 - Arda Taşpınar
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "display.h"
#include "max30102.h"
#include "i2c.h"
#include "usart0.h"
#include "keypad.h"

int main(void)
{
    init_display();
    init_I2C();
    init_USART0();
    init_keypad();
    
    sei();
    
    _delay_ms(1000);
    MAX30102init();
    _delay_ms(1000);

    
    while(1){
        MAX30102read();
    } 
}


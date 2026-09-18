#include <avr/io.h>

void init_I2C()
{
    TWSR |= (1<<TWPS0);  // prescaler set to 4
    TWBR = 198;          // 16e6 / (16+2*198*4) = 10 kBit/s
    TWCR |= (1<<TWEN);   // enable I2C
}
void I2Cstart() {TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); while (!(TWCR & (1<<TWINT)));}
void I2Csend(uint8_t c) {TWDR = c; TWCR = (1<<TWINT)|(1<<TWEN); while (!(TWCR & (1<<TWINT)));}
void I2Cstop() {TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);}
uint8_t I2CreadACK()  {TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN); while (!(TWCR & (1<<TWINT))); return TWDR;}
uint8_t I2CreadNACK() {TWCR = (1<<TWINT)|(0<<TWEA)|(1<<TWEN); while (!(TWCR & (1<<TWINT))); return TWDR;}



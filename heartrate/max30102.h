#include <avr/io.h>

#define	I2CaddrW 0xAE
#define I2CaddrR 0xAF

#define IE1         0x02
#define FIFO_DATA   0x07
#define ModeControl 0x09
#define SpO2_Config 0x0A
#define LED_PA_1    0x0C
#define LED_PA_2    0x0D

void MAX30102init(void);
void MAX30102read(void);

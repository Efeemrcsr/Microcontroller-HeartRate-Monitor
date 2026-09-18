
#include "max30102.h"
#include "i2c.h"
#include "usart0.h"


#define IntStatus1  0x00   
#define FIFOconfig  0x08   
#define PPG_RDY     0x40   



static void MAX30102_writeReg(uint8_t reg, uint8_t value)
{
    I2Cstart();
    I2Csend(I2CaddrW);        
    I2Csend(reg);               
    I2Csend(value);            
    I2Cstop();
}

static uint8_t MAX30102_readReg(uint8_t reg)
{
    uint8_t value;

    I2Cstart();
    I2Csend(I2CaddrW);         
    I2Csend(reg);               
    I2Cstart();                
    I2Csend(I2CaddrR);        
    value = I2CreadNACK();      
    I2Cstop();

    return value;
}



static void USART0_putdec(uint32_t v)
{
    char    buf[10];
    uint8_t n = 0;

    do {                                       
        buf[n++] = (char)('0' + (v % 10U));
        v /= 10U;
    } while (v);

    while (n) {                                
        USART0_transmit((uint8_t)buf[--n]);    
    }

    USART0_transmit('\r');                     
    USART0_transmit('\n');                    
}



void MAX30102init(void)
{
  
    MAX30102_writeReg(ModeControl, 0x40);         
    while (MAX30102_readReg(ModeControl) & 0x40)    
        ;

   
    MAX30102_writeReg(FIFOconfig, 0x50);  

    
    MAX30102_writeReg(SpO2_Config, 0x27);

    
    MAX30102_writeReg(LED_PA_1, 0x10);


    MAX30102_writeReg(IE1, 0x40);     

  
    MAX30102_writeReg(ModeControl, 0x02); 

    
    (void)MAX30102_readReg(IntStatus1);
}

void MAX30102read(void)
{
    
    if (MAX30102_readReg(IntStatus1) & PPG_RDY) {
        uint8_t  b2, b1, b0;
        uint32_t sample;

       
        I2Cstart();
        I2Csend(I2CaddrW);
        I2Csend(FIFO_DATA);
        I2Cstart();                
        I2Csend(I2CaddrR);
        b2 = I2CreadACK();      
        b1 = I2CreadACK();         
        b0 = I2CreadNACK();         
        I2Cstop();

        sample  = ((uint32_t)b2 << 16) | ((uint32_t)b1 << 8) | (uint32_t)b0;
        sample &= 0x3FFFFUL;       

        USART0_putdec(sample);    
    }
}

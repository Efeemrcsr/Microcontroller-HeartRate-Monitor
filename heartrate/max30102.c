/*
 * max30102.c
 *
 * Minimal driver to run the MAX30102 in Heart-Rate (HR) mode and stream the
 * raw photoplethysmogram (PPG) samples to the Arduino IDE Serial Plotter.
 *
 * Only the settings that are actually needed for HR operation are written:
 *   - operating mode      -> Heart-Rate mode (RED LED only)
 *   - RED LED brightness  -> LED1 pulse amplitude
 *   - sample format       -> 18-bit, 100 sps, averaged by 4 (-> 25 sps out)
 * Samples are read from the FIFO whenever the PPG_RDY flag is set and are
 * sent over USART0 as plain decimal numbers (one per line) for the plotter.
 *
 * The I2C transfers use the routines from i2c.c and the register names from
 * max30102.h.
 */

#include "max30102.h"
#include "i2c.h"
#include "usart0.h"

/* Registers / bit masks used in addition to the ones already in max30102.h */
#define IntStatus1  0x00   /* Interrupt Status 1 register             */
#define FIFOconfig  0x08   /* FIFO Configuration register             */
#define PPG_RDY     0x40   /* "new FIFO data ready" flag (bit 6)      */

/* ---------- low level register access via the provided I2C routines ------- */

static void MAX30102_writeReg(uint8_t reg, uint8_t value)
{
    I2Cstart();
    I2Csend(I2CaddrW);          /* slave address + write */
    I2Csend(reg);               /* register address      */
    I2Csend(value);             /* data byte             */
    I2Cstop();
}

static uint8_t MAX30102_readReg(uint8_t reg)
{
    uint8_t value;

    I2Cstart();
    I2Csend(I2CaddrW);          /* slave address + write */
    I2Csend(reg);               /* register address      */
    I2Cstart();                 /* repeated start        */
    I2Csend(I2CaddrR);          /* slave address + read  */
    value = I2CreadNACK();      /* single byte then NACK */
    I2Cstop();

    return value;
}

/* ---------- send an unsigned value as ASCII for the Serial Plotter -------- */

static void USART0_putdec(uint32_t v)
{
    char    buf[10];
    uint8_t n = 0;

    do {                                        /* build digits in reverse  */
        buf[n++] = (char)('0' + (v % 10U));
        v /= 10U;
    } while (v);

    while (n) {                                 /* output most significant  */
        USART0_transmit((uint8_t)buf[--n]);     /* digit first              */
    }

    USART0_transmit('\r');                      /* line terminator expected */
    USART0_transmit('\n');                      /* by the Serial Plotter    */
}

/* -------------------------------- public API ------------------------------ */

void MAX30102init(void)
{
    /* 1) Soft reset so every register starts from a known POR state.       */
    MAX30102_writeReg(ModeControl, 0x40);           /* RESET = 1            */
    while (MAX30102_readReg(ModeControl) & 0x40)     /* wait until it clears */
        ;

    /* 2) FIFO: average 4 samples per stored sample, enable roll-over.      */
    MAX30102_writeReg(FIFOconfig, 0x50);   /* SMP_AVE=010, FIFO_ROLLOVER_EN */

    /* 3) ADC range 4096 nA, 100 sps, 411 us pulse width (18-bit data).     */
    MAX30102_writeReg(SpO2_Config, 0x27);  /* ADC_RGE=01 SR=001 LED_PW=11   */

    /* 4) RED LED (LED1) drive current ~7.2 mA - tune for the subject.      */
    MAX30102_writeReg(LED_PA_1, 0x24);

    /* 5) Enable the "new FIFO data ready" (PPG_RDY) interrupt source.      */
    MAX30102_writeReg(IE1, 0x40);          /* PPG_RDY_EN = 1                */

    /* 6) Enter Heart-Rate mode (RED LED only) - this starts acquisition.   */
    MAX30102_writeReg(ModeControl, 0x02);  /* MODE = 010                    */

    /* 7) Clear any flag left from power-up (e.g. PWR_RDY) for a clean start */
    (void)MAX30102_readReg(IntStatus1);
}

void MAX30102read(void)
{
    /* Take a sample only when the sensor signals that new data is ready.   */
    if (MAX30102_readReg(IntStatus1) & PPG_RDY) {
        uint8_t  b2, b1, b0;
        uint32_t sample;

        /* In HR mode one sample = one channel = 3 bytes, sent MSB first.   */
        I2Cstart();
        I2Csend(I2CaddrW);
        I2Csend(FIFO_DATA);
        I2Cstart();                 /* repeated start */
        I2Csend(I2CaddrR);
        b2 = I2CreadACK();          /* FIFO_DATA[23:16] */
        b1 = I2CreadACK();          /* FIFO_DATA[15:8]  */
        b0 = I2CreadNACK();         /* FIFO_DATA[7:0]   */
        I2Cstop();

        sample  = ((uint32_t)b2 << 16) | ((uint32_t)b1 << 8) | (uint32_t)b0;
        sample &= 0x3FFFFUL;        /* keep the 18 valid (left-justified) bits */

        USART0_putdec(sample);      /* stream it to the Serial Plotter */
    }
}

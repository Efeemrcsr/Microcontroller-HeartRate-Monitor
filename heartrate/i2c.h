void init_I2C(void);
void I2Cstart(void);
void I2Csend(uint8_t c);
void I2Cstop(void);
uint8_t I2CreadACK(void);
uint8_t I2CreadNACK(void);

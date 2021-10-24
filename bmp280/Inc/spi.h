#ifndef _SPI_H_
#define _SPI_H_
#include<stdint.h>
#define BV(x) ((uint8_t)(1 << x))

#define SET_bit(REG, BIT) ((REG) |= BV(BIT))

#define CLEAR_bit(REG, BIT) ((REG) &= ~ ((uint8_t)(1 << BIT)))

#define READ_bit(REG, BIT) ((REG>>BIT) & ((uint8_t)(0x01)))


#define onebyte 1
#define timeout HAL_MAX_DELAY
#define READ(REG)  (CLEAR_BIT(REG,7))
#define WRITE(REG) (REG)
// switch between I2c and spi 
void write_reg(uint8_t reg_addr, uint8_t val);

// reading the value of the register 
uint8_t  read_reg(uint8_t reg_addr );


void read_data(uint8_t reg_addr, uint8_t *data , uint8_t len);

#endif

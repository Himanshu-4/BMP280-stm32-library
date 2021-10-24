#include "spi.h"
#include "main.h"

static uint8_t addr_reg;

extern SPI_HandleTypeDef hspi5;

extern GPIO_TypeDef* port_bmp;
extern uint16_t pin_bmp;

static void write_pin(uint8_t out)
{
    HAL_GPIO_WritePin(port_bmp, pin_bmp, out);
}
void write_reg(uint8_t reg_addr, uint8_t val)
{
    addr_reg = WRITE(reg_addr);
    #ifdef USE_SPI
    write_pin(RESET);
    HAL_SPI_Transmit(&hspi5 , &addr_reg , onebyte, timeout);
    HAL_SPI_Transmit(&hspi5, &val, onebyte, timeout);
    write_pin(SET);
    #else

    #endif
}

uint8_t read_reg(uint8_t reg_addr )
{
  uint8_t data;
  addr_reg = READ(reg_addr);
    #ifdef USE_SPI
    write_pin(RESET);
     HAL_SPI_Transmit(&hspi5 , &addr_reg , onebyte, timeout);
     HAL_SPI_Receive(&hspi5 , &data, onebyte, timeout);
    write_pin(SET);
    #else

    #endif
    return data;
}

void read_data(uint8_t reg_addr, uint8_t *data, uint8_t len)
{
    addr_reg = READ(reg_addr);
   #ifdef USE_SPI
   write_pin(RESET);
   HAL_SPI_Transmit(&hspi5 , &addr_reg , onebyte, timeout);
   HAL_SPI_Receive(&hspi5 , data, len , timeout);
   write_pin(SET);
   #else

   #endif
}

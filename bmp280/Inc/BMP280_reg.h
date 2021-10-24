////////////////////////////////////////////////
///////  all the sensor register defination are present here
////////////////////////////////////////////////////////////

#ifndef _BMP280_REG_H_
#define _BMP280_REG_H_

// the default i2c address for the chip
#define I2C_addr 0x77

  #define BMP280_T1 = 0x88
  #define BMP280_T2 = 0x8A
  #define BMP280_T3 = 0x8C
  #define BMP280_P1 = 0x8E
  #define BMP280_P2 = 0x90
  #define BMP280_P3 = 0x92
  #define BMP280_P4 = 0x94
  #define BMP280_P5 = 0x96
  #define BMP280_P6 = 0x98
  #define BMP280_P7 = 0x9A
  #define BMP280_P8 = 0x9C
  #define BMP280_P9 = 0x9E
/******
 * @name ID reg
 * @def this holds the id of the BMP280 which is 0x58
 */
#define ID 0xD0

#define CHIP_ID 0x58
/*****
 * @name reset reg
 * @def if you write 0xB6 to this register then it performs a soft reset 
 */
#define RESET 0xE0

#define reset_value 0xB6

/******
 * @name STATUS reg 
 * @def contains two bit which indicate the status of the device 
 */
#define STATUS 0xF3

/*****
 * @name ctrl meas 
 * @def this reg set the data acquisition options of the device 
 */
#define CTRL_MEAS 0xF4

/*******
 * @name config reg 
 * @def set the rate , interface and filter option 
 */
#define CONFIG 0xF5

/*******
 * @name pressure data
 * @def contains the raw pressure data 
 */
#define PRESS_MSB 0xF7
#define PRESS_LSB 0xF8
#define PRESS_XLSB 0xF9

/*******
 * @name temp data
 * @def contains the raw temperature data 
 */
#define TEMP_MSB 0xFA
#define TEMP_LSB 0xFB
#define TEMP_XLSB 0xFC



#endif
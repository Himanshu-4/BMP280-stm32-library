#ifndef _BMP280_H_
#define _BMP280_H_

#include "BMP280_reg.h"
#include "spi.h"
#include <stdint.h>



 typedef struct {
  uint16_t dig_T1; /**< dig_T1 cal register. */
  int16_t dig_T2;  /**<  dig_T2 cal register. */
  int16_t dig_T3;  /**< dig_T3 cal register. */

  uint16_t dig_P1; /**< dig_P1 cal register. */
  int16_t dig_P2;  /**< dig_P2 cal register. */
  int16_t dig_P3;  /**< dig_P3 cal register. */
  int16_t dig_P4;  /**< dig_P4 cal register. */
  int16_t dig_P5;  /**< dig_P5 cal register. */
  int16_t dig_P6;  /**< dig_P6 cal register. */
  int16_t dig_P7;  /**< dig_P7 cal register. */
  int16_t dig_P8;  /**< dig_P8 cal register. */
  int16_t dig_P9;  /**< dig_P9 cal register. */
} bmp280_calib_data;


  /** Oversampling rate for the sensor. */
  typedef enum _sensor_sampling {
    /** No over-sampling. */
    SAMPLING_NONE = 0x00,
    /** 1x over-sampling. */
    SAMPLING_X1 = 0x01,
    /** 2x over-sampling. */
    SAMPLING_X2 = 0x02,
    /** 4x over-sampling. */
    SAMPLING_X4 = 0x03,
    /** 8x over-sampling. */
    SAMPLING_X8 = 0x04,
    /** 16x over-sampling. */
    SAMPLING_X16 = 0x05
  }sensor_sampling;

/** Operating mode for the sensor. */
  typedef enum _sensor_mode {
    /** Sleep mode. */
    MODE_SLEEP = 0x00,
    /** Forced mode. */
    MODE_FORCED = 0x01,
    /** Normal mode. */
    MODE_NORMAL = 0x03,
  }sensor_mode;

   /** Filtering level for sensor data. */
  typedef enum _sensor_filter {
    /** No filtering. */
    FILTER_OFF = 0x00,
    /** 2x filtering. */
    FILTER_X2 = 0x01,
    /** 4x filtering. */
    FILTER_X4 = 0x02,
    /** 8x filtering. */
    FILTER_X8 = 0x03,
    /** 16x filtering. */
    FILTER_X16 = 0x04
  }sensor_filter;

 /** Standby duration in ms */
  typedef enum _standby_duration {
    /** 1 ms standby. */
    STANDBY_MS_1 = 0x00,
    /** 62.5 ms standby. */
    STANDBY_MS_63 = 0x01,
    /** 125 ms standby. */
    STANDBY_MS_125 = 0x02,
    /** 250 ms standby. */
    STANDBY_MS_250 = 0x03,
    /** 500 ms standby. */
    STANDBY_MS_500 = 0x04,
    /** 1000 ms standby. */
    STANDBY_MS_1000 = 0x05,
    /** 2000 ms standby. */
    STANDBY_MS_2000 = 0x06,
    /** 4000 ms standby. */
    STANDBY_MS_4000 = 0x07
  }standby_duration;

// hold the configuration value for the BMP280 sensor 
typedef struct _CONFIG
{
  standby_duration st;
  sensor_filter filter;

}config;

typedef struct _CTRL_SAMP
{
     sensor_sampling  pressure_sampling;
     sensor_sampling  temp_sampling;
     sensor_mode  mode;

}ctrl_samp;


typedef struct  _bmp_typedef 
{
 config * bmp_config;
 ctrl_samp * control;
 bmp280_calib_data * data;
}bmp_typedef;

typedef enum _profile_typedef 
{
  Handheld_device_low_power =1,
  Handheld_device_dynamic   ,
  Weather_monitoring ,
  Elevator,
  Drop_detection,
  Indoor_navigaton
}profile_typedef;
/**************************************************************************
 * ******************************************************************
 *             function section **********************************/

/********
 * @name BMP_init
 * @def initialize the BMP sensor with user settings 
 * @param  BMP_typedef 
 */

void BMP_init(bmp_typedef *bmp);
/*********
 * @name get_temperature 
 * @def used to get the temperature based on the user seetings
 * @param bmp_typedef 
 */ 
float get_temp(bmp_typedef *bmp );

/********
 * @name get prressure 
 * @def used to get the pressure based on the user seeting 
 * @param bmp_typedef
 */
float get_pressure(bmp_typedef *bmp );
/***********
 * this function is used to set different profile based on that profile 
 * different settings are made to accomplish that task
 * please refer to datasheet for more information on profile 
 * @param p 
 * @param bmp_typedef
 */
void set_profile(profile_typedef p , bmp_typedef *bmp );




#endif

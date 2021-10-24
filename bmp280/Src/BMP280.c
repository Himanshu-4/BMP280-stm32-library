#include "BMP280.h"

static int32_t t_fine;
//initialize the BMP280  
void BMP_init(bmp_typedef *bmp)
{
    uint8_t conf_val =0;

   if(CHIP_ID != read_reg(ID))return;

    // so first of all Reset  the bmp280 sensor 
  write_reg(RESET, reset_value);

  // set the oversampling parameter  of the sensor
  conf_val = ((bmp->control->temp_sampling << 5) | (bmp->control->pressure_sampling << 2) | (bmp->control->mode));

  write_reg(CTRL_MEAS, conf_val);
  
  // now set the IIR filter time constant and the standby duration in normal mode 
  conf_val =0;

  conf_val = ((bmp->bmp_config->st<<5) | (bmp->bmp_config->filter<<2) | (0x00));
  
  write_reg(CONFIG , conf_val);

 // now read the calibration value 
 uint8_t arr[24] ={0};
  
  read_data(0x88, arr, 24);

  bmp->data->dig_T1 = (uint16_t)  (arr[0]<<8  | arr[1]  );
  bmp->data->dig_T2 = (int16_t)   (arr[2]<<8  | arr[3]  );
  bmp->data->dig_T3 = (int16_t)   (arr[4]<<8  | arr[5]  );
  bmp->data->dig_P1 = (uint16_t)  (arr[6]<<8  | arr[7]  );
  bmp->data->dig_P2 = (int16_t)   (arr[8]<<8  | arr[9]  );
  bmp->data->dig_P3 = (int16_t)   (arr[10]<<8 | arr[11] );
  bmp->data->dig_P4 = (int16_t)   (arr[12]<<8 | arr[13] );
  bmp->data->dig_P5 = (int16_t)   (arr[14]<<8 | arr[15] );
  bmp->data->dig_P6 = (int16_t)   (arr[16]<<8 | arr[17] );
  bmp->data->dig_P7 = (int16_t)   (arr[18]<<8 | arr[19] );
  bmp->data->dig_P8 = (int16_t)   (arr[20]<<8 | arr[21] );
  bmp->data->dig_P9 = (int16_t)   (arr[22]<<8 | arr[23] );





 }


 float get_pressure(bmp_typedef *bmp)
 {
  // first read the raw presssure from the sensor
  int64_t p,  var1 ,  var2;
  uint8_t arr[3];
  int32_t adc_P;
  
  // first read the temperature data for the presuure calculations 
  get_temp(bmp);

  read_data(PRESS_MSB, arr, 3);

  adc_P = (int32_t)( (arr[0]<<16) | (arr[1] << 8) | arr[3]);
  adc_P >> 4;

  // these API are taken from BOSCH sensoetech it is highly recommended by them 
  var1 = ((int64_t)t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)bmp->data->dig_P6;
  var2 = var2 + ((var1 * (int64_t)bmp->data->dig_P5) << 17);
  var2 = var2 + (((int64_t)bmp->data->dig_P4) << 35);
  var1 = ((var1 * var1 * (int64_t)bmp->data->dig_P3) >> 8) +
         ((var1 * (int64_t)bmp->data->dig_P2) << 12);
  var1 =
      (((((int64_t)1) << 47) + var1)) * ((int64_t)bmp->data->dig_P1) >> 33;

  if (var1 == 0) {
    return 0; // avoid exception caused by division by zero
  }
  p = 1048576 - adc_P;
  p = (((p << 31) - var2) * 3125) / var1;
  var1 = (((int64_t)bmp->data->dig_P9) * (p >> 13) * (p >> 13)) >> 25;
  var2 = (((int64_t)bmp->data->dig_P8) * p) >> 19;

  p = ((p + var1 + var2) >> 8) + (((int64_t)bmp->data->dig_P7) << 4);
  return (float)p / 256;
       
 }

 float get_temp(bmp_typedef *bmp)
 {
  // first read the raw data from the sensor 
  int32_t var1 , var2;
  uint8_t arr[3];
  int32_t adc_T;
  read_data(TEMP_MSB, arr, 3);

 adc_T = (int32_t)( (arr[0]<<16) | (arr[1] << 8) | arr[3]);
 adc_T >>=4;

var1 = ((((adc_T >> 3) - ((int32_t)bmp->data->dig_T1 << 1))) *
          ((int32_t)bmp->data->dig_T2)) >>
         11;

  var2 = (((((adc_T >> 4) - ((int32_t)bmp->data->dig_T1)) *
            ((adc_T >> 4) - ((int32_t)bmp->data->dig_T1))) >>
           12) *
          ((int32_t)bmp->data->dig_T3)) >>
         14;

t_fine =  var1 + var2;
 
 float temperature = (t_fine * 5 +128 )>>8;

 return  temperature /100;
 }

 void set_profile(profile_typedef p , bmp_typedef *bmp)
 {
    
  // automatically set the parameter based on the profile

  if(p == Handheld_device_low_power )
  {
    /**********
     * mode = normal 
     * pressure oversampling = 16 
     * temp overs. = 2 
     * IIR filter coff. = 4 
     * standby timimng = 63ms
     */
    bmp->control->mode = MODE_NORMAL;
    bmp->control->pressure_sampling = SAMPLING_X16;
    bmp->control->temp_sampling = SAMPLING_X4;

    bmp->bmp_config->filter = FILTER_X4;
    bmp->bmp_config->st =  STANDBY_MS_63;

  }  

  else if(p == Handheld_device_dynamic)
  {
       /**********
     * mode = normal 
     * pressure oversampling = 4
     * temp overs. = 1
     * IIR filter coff. = 16 
     * standby timimng = 0.5ms
     */
    bmp->control->mode = MODE_NORMAL;
    bmp->control->pressure_sampling = SAMPLING_X4;
    bmp->control->temp_sampling = SAMPLING_X1;

    bmp->bmp_config->filter = FILTER_X16;
    bmp->bmp_config->st =  STANDBY_MS_1;

  }

  else if(p == Weather_monitoring )
  {
       /**********
     * mode = forced 
     * pressure oversampling = 1 
     * temp overs. = 1
     * IIR filter coff. = off 
     * standby timimng = 1 min
     */
    bmp->control->mode = MODE_FORCED;
    bmp->control->pressure_sampling = SAMPLING_X1;
    bmp->control->temp_sampling = SAMPLING_X1;

    bmp->bmp_config->filter = FILTER_OFF;
    bmp->bmp_config->st =  STANDBY_MS_4000;

  }
  else if (p == Elevator )
  {
       /**********
     * mode = normal 
     * pressure oversampling = 4
     * temp overs. = 1
     * IIR filter coff. = 4 
     * standby timimng = 125ms
     */
    bmp->control->mode = MODE_NORMAL;
    bmp->control->pressure_sampling = SAMPLING_X4;
    bmp->control->temp_sampling = SAMPLING_X1;

    bmp->bmp_config->filter = FILTER_X4;
    bmp->bmp_config->st =  STANDBY_MS_125;
  }
 
 else if (p == Drop_detection)
 {
      /**********
     * mode = normal 
     * pressure oversampling = 4 
     * temp overs. = 1
     * IIR filter coff. = off 
     * standby timimng = 0.5ms
     */
    bmp->control->mode = MODE_NORMAL;
    bmp->control->pressure_sampling = SAMPLING_X4;
    bmp->control->temp_sampling = SAMPLING_X1;

    bmp->bmp_config->filter = FILTER_OFF;
    bmp->bmp_config->st =  STANDBY_MS_500;
 }
 else if (p == Indoor_navigaton)
 {
      /**********
     * mode = normal 
     * pressure oversampling = 16
     * temp overs. = 2 
     * IIR filter coff. = 16
     * standby timimng = 500
     */
    bmp->control->mode = MODE_NORMAL;
    bmp->control->pressure_sampling = SAMPLING_X16;
    bmp->control->temp_sampling = SAMPLING_X2;

    bmp->bmp_config->filter = FILTER_X16;
    bmp->bmp_config->st =  STANDBY_MS_500;
 }

// call the init function 
 BMP_init(bmp);

 }

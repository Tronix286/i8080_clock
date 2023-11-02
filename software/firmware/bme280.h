#ifndef __BME280_H
#define __BME280_H		

#define BME280_CHIP_ID        0x60

#define BME280_REG_DIG_T1     0x88
#define BME280_REG_DIG_T2     0x8A
#define BME280_REG_DIG_T3     0x8C

#define BME280_REG_DIG_P1     0x8E
#define BME280_REG_DIG_P2     0x90
#define BME280_REG_DIG_P3     0x92
#define BME280_REG_DIG_P4     0x94
#define BME280_REG_DIG_P5     0x96
#define BME280_REG_DIG_P6     0x98
#define BME280_REG_DIG_P7     0x9A
#define BME280_REG_DIG_P8     0x9C
#define BME280_REG_DIG_P9     0x9E

#define BME280_REG_DIG_H1     0xA1
#define BME280_REG_DIG_H2     0xE1
#define BME280_REG_DIG_H3     0xE3
#define BME280_REG_DIG_H4     0xE4
#define BME280_REG_DIG_H5     0xE5
#define BME280_REG_DIG_H6     0xE7

#define BME280_REG_CHIPID     0xD0
#define BME280_REG_SOFTRESET  0xE0

#define BME280_REG_CTRLHUM    0xF2
#define BME280_REG_STATUS     0xF3
#define BME280_REG_CONTROL    0xF4
#define BME280_REG_CONFIG     0xF5
#define BME280_REG_PRESS_MSB  0xF7

// BME280 sensor modes, register ctrl_meas mode[1:0]
enum bme280_mode
{
  MODE_SLEEP  = 0x00,  // sleep mode
  MODE_FORCED = 0x01,  // forced mode
  MODE_NORMAL = 0x03   // normal mode
} ;

// oversampling setting. osrs_h[2:0], osrs_t[2:0], osrs_p[2:0]
enum bme280_sampling
{
  SAMPLING_SKIPPED = 0x00,  //skipped, output set to 0x80000 (0x8000 for humidity)
  SAMPLING_X1      = 0x01,  // oversampling x1
  SAMPLING_X2      = 0x02,  // oversampling x2
  SAMPLING_X4      = 0x03,  // oversampling x4
  SAMPLING_X8      = 0x04,  // oversampling x8
  SAMPLING_X16     = 0x05   // oversampling x16
} ;

// filter setting filter[2:0]
enum bme280_filter
{
  FILTER_OFF = 0x00,  // filter off
  FILTER_2   = 0x01,  // filter coefficient = 2
  FILTER_4   = 0x02,  // filter coefficient = 4
  FILTER_8   = 0x03,  // filter coefficient = 8
  FILTER_16  = 0x04   // filter coefficient = 16
} ;

// standby (inactive) time in ms (used in normal mode), t_sb[2:0]
enum standby_time
{
  STANDBY_0_5   =  0x00,  // standby time = 0.5 ms
  STANDBY_62_5  =  0x01,  // standby time = 62.5 ms
  STANDBY_125   =  0x02,  // standby time = 125 ms
  STANDBY_250   =  0x03,  // standby time = 250 ms
  STANDBY_500   =  0x04,  // standby time = 500 ms
  STANDBY_1000  =  0x05,  // standby time = 1000 ms
  STANDBY_10    =  0x06,  // standby time = 10 ms
  STANDBY_20    =  0x07   // standby time = 20 ms
} ;

typedef struct
{
  uint16_t dig_T1;
  int16_t  dig_T2;
  int16_t  dig_T3;
  uint16_t dig_P1;
  int16_t  dig_P2;
  int16_t  dig_P3;
  int16_t  dig_P4;
  int16_t  dig_P5;
  int16_t  dig_P6;
  int16_t  dig_P7;
  int16_t  dig_P8;
  int16_t  dig_P9;

  uint8_t  dig_H1;
  int16_t  dig_H2;
  uint8_t  dig_H3;
  int16_t  dig_H4;
  int16_t  dig_H5;
  int8_t   dig_H6;
} BME280calib;

extern BME280calib BME280_calib;
extern uint8_t  BME280_I2C_ADDRESS;

uint8_t BME280_Read8(uint8_t reg_addr);
uint8_t BME280_begin(uint8_t mode);
void BME280_Update();
uint8_t BME280_readTemperature(int32_t *temp);
uint8_t BME280_readHumidity(uint32_t *humi);
uint8_t BME280_readPressure(uint32_t *pres);
#endif

#ifndef BMP280_H
#define BMP280_H

#include "stm32f4xx.h"

#define BMP280_I2C_ADDR 0x76

#define BMP280_CHIP_ID_REG 0xD0
#define BMP280_RESET_REG 0xE0
#define BMP280_STATUS_REG 0xF3
#define BMP280_CTRL_MEAS_REG 0xF4
#define BMP280_CONFIG_REG 0xF5

#define BMP280_PRESS_MSB_REG 0xF7
#define BMP280_PRESS_LSB_REG 0xF8
#define BMP280_PRESS_XLSB_REG 0xF9

#define BMP280_TEMP_MSB_REG 0xFA
#define BMP280_TEMP_LSB_REG 0xFB
#define BMP280_TEMP_XLSB_REG 0xFC

#define BMP280_DIG_T1_REG 0x88
#define BMP280_DIG_T2_REG 0x8A
#define BMP280_DIG_T3_REG 0x8C

#define BMP280_DIG_P1_REG 0x8E
#define BMP280_DIG_P2_REG 0x90
#define BMP280_DIG_P3_REG 0x92
#define BMP280_DIG_P4_REG 0x94
#define BMP280_DIG_P5_REG 0x96
#define BMP280_DIG_P6_REG 0x98
#define BMP280_DIG_P7_REG 0x9A
#define BMP280_DIG_P8_REG 0x9C
#define BMP280_DIG_P9_REG 0x9E

typedef struct
{
	uint16_t dig_T1;

	int16_t dig_T2;
	int16_t dig_T3;

	uint16_t dig_P1;

	int16_t dig_P2;
	int16_t dig_P3;
	int16_t dig_P4;
	int16_t dig_P5;
	int16_t dig_P6;
	int16_t dig_P7;
	int16_t dig_P8;
	int16_t dig_P9;
}BMP280_CalibData_t;

uint8_t bmp280_read_chip_id(void);
uint8_t bmp280_read_calib_data(uint8_t *calib);
uint8_t bmp280_write_reg(uint8_t address, uint8_t data);
uint8_t bmp280_init(void);
uint8_t bmp280_read_reg(uint8_t adddress, uint8_t *data);
uint8_t bmp280_read_regs(uint8_t reg, uint8_t *buffer, uint8_t len);
int32_t bmp280_read_raw_temperature(void);
float bmp280_get_temperature(void);
int32_t bmp280_read_raw_pressure(void);
float bmp280_get_pressure(void);

#endif









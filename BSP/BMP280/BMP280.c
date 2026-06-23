#include "BMP280.h"
#include "i2c.h"

static BMP280_CalibData_t bmp280_calib_data;
static int32_t t_fine;
static void bmp280_parse_calib_data(uint8_t *calib, BMP280_CalibData_t *bmp280_calib_data);

uint8_t bmp280_read_chip_id(void)
{
	uint8_t id;
	i2c1_read_register(BMP280_I2C_ADDR, BMP280_CHIP_ID_REG, &id);
	return id;
}

uint8_t bmp280_read_calib_data(uint8_t *calib)
{
	if(!(i2c1_read_multi(BMP280_I2C_ADDR, BMP280_DIG_T1_REG, calib, 24)))
	{
		return 0;
	}
	return 1;
}

static void bmp280_parse_calib_data(uint8_t *calib, BMP280_CalibData_t *bmp280_calib_data)
{
	bmp280_calib_data->dig_T1 = (uint16_t)((calib[1] << 8) | calib[0]);
	bmp280_calib_data->dig_T2 = (int16_t)((calib[3] << 8) | calib[2]);
	bmp280_calib_data->dig_T3 = (int16_t)((calib[5] << 8) | calib[4]);

	bmp280_calib_data->dig_P1 = (uint16_t)((calib[7] << 8) | calib[6]);
	bmp280_calib_data->dig_P2 = (int16_t)((calib[9] << 8) | calib[8]);
	bmp280_calib_data->dig_P3 = (int16_t)((calib[11] << 8) | calib[10]);
	bmp280_calib_data->dig_P4 = (int16_t)((calib[13] << 8) | calib[12]);
	bmp280_calib_data->dig_P5 = (int16_t)((calib[15] << 8) | calib[14]);
	bmp280_calib_data->dig_P6 = (int16_t)((calib[17] << 8) | calib[16]);
	bmp280_calib_data->dig_P7 = (int16_t)((calib[19] << 8) | calib[18]);
	bmp280_calib_data->dig_P8 = (int16_t)((calib[21] << 8) | calib[20]);
	bmp280_calib_data->dig_P9 = (int16_t)((calib[23] << 8) | calib[22]);
}

uint8_t bmp280_write_reg(uint8_t address, uint8_t data)
{
	if(!(i2c1_write_register(BMP280_I2C_ADDR, address, data)))
	{
		return 0;
	}
	return 1;
}


uint8_t bmp280_read_reg(uint8_t address, uint8_t *data)
{
	if(!(i2c1_read_register(BMP280_I2C_ADDR, address, data)))
	{
		return 0;
	}
	return 1;
}
uint8_t bmp280_read_regs(uint8_t reg, uint8_t *buffer, uint8_t len)
{
	if(!(i2c1_read_multi(BMP280_I2C_ADDR, reg, buffer, len)))
	{
		return 0;
	}
	return 1;
}

uint8_t bmp280_init(void)
{
	uint8_t calib[24];
	if(bmp280_read_chip_id() != 0x58)
	{
		return 0;
	}
	if(!(bmp280_write_reg(BMP280_CTRL_MEAS_REG,0x27)))
		return 0;
	if(!(bmp280_read_calib_data(calib)))
		return 0;
	bmp280_parse_calib_data(calib, &bmp280_calib_data);
	return 1;
}

int32_t bmp280_read_raw_temperature(void)
{
	uint8_t data[3];
	bmp280_read_regs(BMP280_TEMP_MSB_REG, data, 3);
	return ((int32_t)data[0] << 12) | ((int32_t)data[1] << 4) | ((int32_t)data[2] >> 4);
}

float bmp280_get_temperature(void)
{
	int32_t adc_T;
	int32_t var1, var2;
	int32_t T;
	adc_T = bmp280_read_raw_temperature();
	var1 = ((((adc_T >> 3) - ((int32_t)bmp280_calib_data.dig_T1 << 1))) * ((int32_t)bmp280_calib_data.dig_T2)) >> 11;
	var2 = (((((adc_T >> 4) - ((int32_t)bmp280_calib_data.dig_T1)) * ((adc_T >> 4) - ((int32_t)bmp280_calib_data.dig_T1))) >> 12) * ((int32_t)bmp280_calib_data.dig_T3)) >> 14;
	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) >> 8;
	return T / 100.0f;
}

int32_t bmp280_read_raw_pressure(void)
{
    uint8_t data[3];

    bmp280_read_regs(BMP280_PRESS_MSB_REG, data, 3);
    return ((int32_t)data[0] << 12) | ((int32_t)data[1] << 4)  | ((int32_t)data[2] >> 4);
}

float bmp280_get_pressure(void)
{
    int32_t adc_P;
    int64_t var1, var2;
    int64_t P;

    adc_P = bmp280_read_raw_pressure();
    bmp280_get_temperature();
    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)bmp280_calib_data.dig_P6;
    var2 = var2 + ((var1 * (int64_t)bmp280_calib_data.dig_P5) << 17);
    var2 = var2 + (((int64_t)bmp280_calib_data.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)bmp280_calib_data.dig_P3) >> 8) + ((var1 * (int64_t)bmp280_calib_data.dig_P2) << 12);
    var1 = ((((int64_t)1 << 47) + var1) * (int64_t)bmp280_calib_data.dig_P1) >> 33;
    if (var1 == 0)
    {
        return 0;
    }
    P = 1048576 - (int64_t)adc_P;
    P = (((P << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)bmp280_calib_data.dig_P9) * (P >> 13) * (P >> 13)) >> 25;
    var2 = (((int64_t)bmp280_calib_data.dig_P8) * P) >> 19;
    P = ((P + var1 + var2) >> 8) + (((int64_t)bmp280_calib_data.dig_P7) << 4);
    return P / 256.0f;
}






























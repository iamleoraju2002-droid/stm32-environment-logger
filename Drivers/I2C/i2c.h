#ifndef I2C_H
#define I2C_H


#include "stm32f4xx.h"

#define I2C_WRITE 0
#define I2C_READ 1
#define I2C_TIMEOUT 100000U

void i2c1_scan(void);
void i2c1_init(void);
uint8_t i2c1_start(void);
uint8_t i2c1_restart(void);
void i2c1_stop(void);
uint8_t i2c1_send_address(uint8_t address, uint8_t rw);
uint8_t i2c1_write_byte(uint8_t data);
uint8_t i2c1_read_byte_ack(uint8_t *data);
uint8_t i2c1_read_byte_nack(uint8_t *data);
uint8_t i2c1_write_register(uint8_t slave_addr, uint8_t reg_addr, uint8_t data);
uint8_t i2c1_read_register(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data);
uint8_t i2c1_read_multi(uint8_t slave_addr, uint8_t reg_addr, uint8_t *buffer, uint8_t len);
uint8_t i2c1_write_multi(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);
#endif

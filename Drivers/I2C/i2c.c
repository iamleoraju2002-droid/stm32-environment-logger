#include "i2c.h"
#include "usart.h"

static volatile uint32_t temp;

void i2c1_scan(void)
{
    uint8_t addr;

    usart2_write_string("Scanning I2C bus...\r\n");

    for(addr = 0; addr < 128; addr++)
    {
        i2c1_start();

        if(i2c1_send_address(addr, I2C_WRITE))
        {
            usart2_write_string("Device found at: ");
            usart2_write_hex(addr);
            usart2_write_string("\r\n");
        }

        i2c1_stop();
    }

    usart2_write_string("Scan complete.\r\n");
}
void i2c1_init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

	GPIOB->MODER &= ~(GPIO_MODER_MODE8_Msk | GPIO_MODER_MODE9_Msk);
	GPIOB->MODER |= ((2<<GPIO_MODER_MODE8_Pos) | (2<<GPIO_MODER_MODE9_Pos));

	GPIOB->OTYPER |= GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9;
	GPIOB->OSPEEDR |= (3<<GPIO_OSPEEDR_OSPEED8_Pos) | (3<<GPIO_OSPEEDR_OSPEED9_Pos);
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD8_Msk | GPIO_PUPDR_PUPD9_Msk);
	GPIOB->AFR[1] |= (4<<GPIO_AFRH_AFSEL8_Pos) | (4<<GPIO_AFRH_AFSEL9_Pos);

	I2C1->CR1 |= I2C_CR1_SWRST;
	I2C1->CR1 &= ~I2C_CR1_SWRST;

	I2C1->CR2 = 42;
	I2C1->CCR = 210;
	I2C1->TRISE = 43;

	I2C1->CR1 |= I2C_CR1_PE;
}

uint8_t i2c1_start(void)
{
	uint32_t timeout = I2C_TIMEOUT;
	while(I2C1->SR2 & I2C_SR2_BUSY)
	{
		if(--timeout == 0)
		{
			return 0;
		}
	}
	I2C1->CR1 |= I2C_CR1_START;
	timeout = I2C_TIMEOUT;
	while(!(I2C1->SR1 & I2C_SR1_SB))
	{
		if(--timeout == 0)
		{
			return 0;
		}
	}
	return 1;
}

uint8_t i2c1_restart(void)
{
	uint32_t timeout = I2C_TIMEOUT;
	I2C1->CR1 |= I2C_CR1_START;
	while(!(I2C1->SR1 & I2C_SR1_SB))
	{
		if(--timeout == 0)
		{
			return 0;
		}
	}
	return 1;
}

void i2c1_stop(void)
{
	I2C1->CR1 |= I2C_CR1_STOP;
}

uint8_t i2c1_send_address(uint8_t address, uint8_t rw)
{
	volatile uint32_t dummy;
	uint32_t timeout = I2C_TIMEOUT;
	I2C1->SR1 &= ~I2C_SR1_AF;
	I2C1->DR = (address<<1) | rw;
	while(--timeout)
	{
		uint32_t sr1 = I2C1->SR1;
		if(sr1 & I2C_SR1_AF)
		{
			I2C1->SR1 &= ~I2C_SR1_AF;
			return 0;
		}
		if(sr1 & I2C_SR1_ADDR)
				{
					dummy = I2C1->SR1;
					dummy = I2C1->SR2;
					return 1;
				}
	}
	return 0;
}

uint8_t i2c1_write_byte(uint8_t data)
{
	uint32_t timeout = I2C_TIMEOUT;
	while(!(I2C1->SR1 & I2C_SR1_TXE))
	{
		if(--timeout == 0)
		{
			return 0;
		}
	}
	I2C1->DR = data;
	timeout = I2C_TIMEOUT;
	while(!(I2C1->SR1 & I2C_SR1_BTF))
	{
		if(--timeout == 0)
		{
			return 0;
		}
	}
	return 1;
}

uint8_t i2c1_read_byte_ack(uint8_t *data)
{
	uint32_t timeout = I2C_TIMEOUT;
	I2C1->CR1 |= I2C_CR1_ACK;
	while(!(I2C1->SR1 & I2C_SR1_RXNE))
	{
		if(--timeout == 0)
		{
			return 0;
		}
	}
	*data = I2C1->DR;
	return 1;
}

uint8_t i2c1_read_byte_nack(uint8_t *data)
{
	uint32_t timeout = I2C_TIMEOUT;
	I2C1->CR1 &= ~I2C_CR1_ACK;
	I2C1->CR1 |= I2C_CR1_STOP;
	while(!(I2C1->SR1 & I2C_SR1_RXNE))
	{
		if(--timeout == 0)
		{
			return 0;
		}
	}
	*data = I2C1->DR;
	return 1;
}

uint8_t i2c1_write_register(uint8_t slave_addr, uint8_t reg_addr, uint8_t data)
{
	if(!(i2c1_start()))
	{
		i2c1_stop();
		return 0;
	}
	if(!(i2c1_send_address(slave_addr, I2C_WRITE)))
	{
		i2c1_stop();
		return 0;
	}
	if(!(i2c1_write_byte(reg_addr)))
	{
		i2c1_stop();
		return 0;
	}
	if(!(i2c1_write_byte(data)))
	{
		i2c1_stop();
		return 0;
	}
	i2c1_stop();
	return 1;
}

uint8_t i2c1_read_register(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data)
{
	if(!(i2c1_start()))
	{
		i2c1_stop();
		return 0;
	}
	if(!(i2c1_send_address(slave_addr, I2C_WRITE)))
	{
		i2c1_stop();
		return 0;
	}
	if(!(i2c1_write_byte(reg_addr)))
	{
		i2c1_stop();
		return 0;
	}
	if(!(i2c1_restart()))
	{
		i2c1_stop();
		return 0;
	}
	if(!(i2c1_send_address(slave_addr, I2C_READ)))
	{
		i2c1_stop();
		return 0;
	}
	if(!(i2c1_read_byte_nack(data)))
	{
		i2c1_stop();
		return 0;
	}
	return 1;
}

uint8_t i2c1_read_multi(uint8_t slave_addr, uint8_t reg_addr, uint8_t *buffer, uint8_t len)
{
	if(!(i2c1_start()))
	{
		i2c1_stop();
		return 0;
	}
	if(!(i2c1_send_address(slave_addr, I2C_WRITE)))
	{
		i2c1_stop();
		return 0;
	}
	if(!(i2c1_write_byte(reg_addr)))
	{
		i2c1_stop();
		return 0;
	}
	if(!(i2c1_restart()))
	{
		i2c1_stop();
		return 0;
	}
	if(!(i2c1_send_address(slave_addr, I2C_READ)))
	{
		i2c1_stop();
		return 0;
	}
	for(uint8_t i = 0; i < len; i++)
	{
		if(len == 0)
		{
			i2c1_stop();
			return 0;
		}
		else if(i == (len-1))
		{
			if(!(i2c1_read_byte_nack(&buffer[i])))
			{
				i2c1_stop();
				return 0;
			}
		}
		else
		{
			if(!(i2c1_read_byte_ack(&buffer[i])))
			{
				i2c1_stop();
				return 0;
			}
		}
	}
	return 1;
}

uint8_t i2c1_write_multi(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
	if(!(i2c1_start()))
		return 0;
	if(!(i2c1_send_address(slave_addr, I2C_WRITE)))
	{
		i2c1_stop();
		return 0;
	}
	if(!(i2c1_write_byte(reg_addr)))
	{
		i2c1_stop();
		return 0;
	}
	for (uint16_t i = 0; i < len; i++)
	{
		if(!(i2c1_write_byte(data[i])))
		{
			i2c1_stop();
			return 0;
		}
	}
	i2c1_stop();
	return 1;
}














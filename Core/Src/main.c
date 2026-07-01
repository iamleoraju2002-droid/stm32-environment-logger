
#include "main.h"
#include "rcc.h"
#include "delay.h"
#include "gpio.h"
#include "systick.h"
#include "usart.h"
#include "ringbuffer.h"
#include "i2c.h"
#include "BMP280.h"
#include "SSD1306.h"
#include "FONTS.h"
#include "EEPROM.h"
#include "LOGGER.h"
#include "tim.h"
#include "nvic.h"
#include "command.h"

int16_t int_temp;
uint32_t int_pressure;



int main(void)
{

	rcc_init();
	SystemCoreClockUpdate();
	systick_init();
	gpio_init();
	usart2_init();
	i2c1_init();

	ssd1306_init();
	bmp280_init();
	logger_init();
	timer2_init(5000);
	nvic_init();

	for(uint16_t i=0; i<5; i++)
	{
		bmp280_get_temperature();
		bmp280_get_pressure();
		delay_ms(50);
	}

	timer2_start();
	command_terminal();
	command_packet_t packet;
	uint8_t logging = LOGGING_OFF;


  while (1)
  {
	  command_parser();
	  if(command_available())
	  {
		  if(command_process(&packet))
		  {
			  command_execute(&packet);
		  }
		  command_clear();
	  }
	  if(timer_flag == 1)
	  {
		  timer_flag = 0;

		  float temp = bmp280_get_temperature();
		  float pressure = bmp280_get_pressure();
		  int_temp = (int16_t)(temp * 100);
		  int_pressure = (uint32_t)(pressure * 100);
		  if(logger_is_enabled())
		  {
			  logger_save_record(int_temp, int_pressure);
			  logging = LOGGING_ON;
		  }
		  else
		  {
			  logging = LOGGING_OFF;
		  }
		  display_main_screen(int_temp, int_pressure, logging);
	  }


  }
}


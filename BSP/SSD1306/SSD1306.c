#include "SSD1306.h"
#include "i2c.h"
#include "FONTS.h"
#include "stdio.h"

uint8_t ssd1306_buffer[1024];


void ssd1306_clear_all(void)
{
	for(uint8_t page = 0; page < 8; page++)
	{
		ssd1306_send_command(0xB0 + page);
		ssd1306_send_command(0x00);
		ssd1306_send_command(0x10);
		for(uint16_t col = 0; col < 128; col++)
		{
			ssd1306_send_data(0x00);
		}
	}
}

void ssd1306_fill_all(void)
{
	for (uint8_t page = 0; page < 8; page++)
	{
		ssd1306_send_command(0xB0 + page);
		ssd1306_send_command(0x00);
		ssd1306_send_command(0x10);
		for(uint16_t col = 0; col < 128; col++)
		{
			ssd1306_send_data(0xFF);
		}
	}
}

uint8_t ssd1306_set_page(uint8_t page)
{
	if(!(ssd1306_send_command(0xB0 + page)))
		return 0;
	return 1;
}
uint8_t ssd1306_set_column(uint8_t column)
{
	if(!(ssd1306_send_command(0x00 | (column & 0x0F))))
		return 0;
	if(!(ssd1306_send_command(0x10 | ((column >> 4) & 0x0F))))
		return 0;
	return 1;
}

uint8_t ssd1306_send_command(uint8_t cmd)
{
	if(!(i2c1_write_register(SSD1306_I2C_ADDR, 0x00, cmd)))
	{
		i2c1_stop();
		return 0;
	}
	return 1;
}

void ssd1306_init(void)
{
    ssd1306_send_command(0xAE);  // Display OFF

    ssd1306_send_command(0x20);  // Addressing mode
    ssd1306_send_command(0x02);  // Page Addressing

    ssd1306_send_command(0x40);  // Start line

    ssd1306_send_command(0xA1);  // Segment remap

    ssd1306_send_command(0xC8);  // COM scan direction

    ssd1306_send_command(0xA8);  // Multiplex ratio
    ssd1306_send_command(0x3F);  // 64 rows

    ssd1306_send_command(0xD3);  // Display offset
    ssd1306_send_command(0x00);

    ssd1306_send_command(0xD5);  // Clock divide
    ssd1306_send_command(0x80);

    ssd1306_send_command(0xD9);  // Precharge
    ssd1306_send_command(0xF1);

    ssd1306_send_command(0xDA);  // COM pins
    ssd1306_send_command(0x12);

    ssd1306_send_command(0x81);  // Contrast
    ssd1306_send_command(0xCF);

    ssd1306_send_command(0xA4);  // Display RAM

    ssd1306_send_command(0xA6);  // Normal display

    ssd1306_send_command(0x8D);  // Charge pump
    ssd1306_send_command(0x14);

    ssd1306_send_command(0xAF);  // Display ON
}

uint8_t ssd1306_send_data(uint8_t data)
{
	if(!(i2c1_write_register(SSD1306_I2C_ADDR, 0x40, data)))
	{
		i2c1_stop();
		return 0;
	}
	return 1;

}

uint8_t ssd1306_send_multi_data(uint8_t *data, uint16_t len)
{
	if(!(i2c1_write_multi(SSD1306_I2C_ADDR, 0x40, data, len)))
		return 0;
	return 1;
}

uint8_t ssd1306_update_screen(void)
{
	for(uint8_t page = 0; page < 8; page++)
	{
		if(!(ssd1306_set_page(page)))
			return 0;
		if(!(ssd1306_set_column(0)))
			return 0;
		if(!(ssd1306_send_multi_data(&ssd1306_buffer[page * 128], 128)))
			return 0;
	}
	return 1;
}

uint8_t ssd1306_draw_pixel(uint8_t x, uint8_t y)
{
	if(x >= 128 || y >= 64)
		return 0;
	ssd1306_buffer[x + (y/8) * 128] |= (1 << (y % 8));
	return 1;
}


uint8_t ssd1306_clear_pixel(uint8_t x, uint8_t y)
{
	if(x >= 128 || y >= 64)
		return 0;
	ssd1306_buffer[x + (y/8) * 128] &= ~(1 << (y % 8));
	return 1;
}

void ssd1306_clear_buffer(void)
{
	for(uint16_t i = 0; i < 1024; i++)
	{
		ssd1306_buffer[i] = 0;
	}
}

uint8_t ssd1306_draw_hline(uint8_t x1, uint8_t x2, uint8_t y)
{
	for(uint8_t x = x1; x <= x2; x++)
	{
		if(!(ssd1306_draw_pixel(x, y)))
			return 0;
	}
	return 1;
}
uint8_t ssd1306_draw_vline(uint8_t x, uint8_t y1, uint8_t y2)
{
	for(uint8_t y = y1; y <= y2; y++)
	{
		if(!(ssd1306_draw_pixel(x, y)))
			return 0;
	}
	return 1;
}

uint8_t ssd1306_draw_rect(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2)
{
	for(uint8_t x = x1; x <= x2; x++)
	{
		if(!(ssd1306_draw_pixel(x,y1)))
			return 0;
	}
	for(uint8_t x = x1; x <= x2; x++)
	{
		if(!(ssd1306_draw_pixel(x,y2)))
			return 0;
	}
	for(uint8_t y = y1; y <= y2; y++)
	{
		if(!(ssd1306_draw_pixel(x1,y)))
			return 0;
	}
	for(uint8_t y = y1; y <= y2; y++)
	{
		if(!(ssd1306_draw_pixel(x2,y)))
			return 0;
	}
	return 1;
}

uint8_t ssd1306_draw_char (uint8_t x, uint8_t page, char ch)
{
	uint8_t i;
	const uint8_t *glyph;
	if(ch ==' ')
		glyph = font5x7[0];
	else if(ch >= '0' && ch <= '9')
		glyph = font5x7[1 + (ch - '0')];
	else if(ch >= 'A' && ch <= 'Z')
		glyph = font5x7[ 11 + (ch - 'A')];
	else if(ch == ':')
		glyph = font5x7[37];
	else if(ch == '.')
		glyph = font5x7[38];
	else if(ch == '-')
		glyph = font5x7[39];
	else
		return 0;

	for(i = 0; i < 5; i++)
	{
		ssd1306_buffer[page * 128 + x +i] = glyph[i];
	}
	ssd1306_buffer[page * 128 + x + 5] = 0x00;
	return 1;
}

uint8_t ssd1306_draw_string (uint8_t x, uint8_t page, char *str)
{
	while(*str)
	{
		if(!(ssd1306_draw_char(x, page, *str)))
			return 0;
		x += 6;
		str++;
	}
	return 1;
}

void display_main_screen(int16_t temp, int32_t pressure, uint8_t logging)
{

	char temp_str[16];
	char press_str[16];

	sprintf(temp_str, "%d.%02d", temp/100, temp%100);
	sprintf(press_str, "%lu.%02lu", pressure/100, pressure%100);

	ssd1306_clear_buffer();

	ssd1306_draw_rect(0,127,0,63);

	ssd1306_draw_string(36,1,"LOGGER");

	ssd1306_draw_string(6,3,"TEMP: ");
	ssd1306_draw_string(48,3,temp_str);

	ssd1306_draw_string(6,4,"PRESS: ");
	ssd1306_draw_string(48,4,press_str);

	ssd1306_draw_string(6,6,"LOG: ");

	if(logging)
		ssd1306_draw_string(48,6,"ON");
	else
		ssd1306_draw_string(48,6,"OFF");
	ssd1306_update_screen();
}

















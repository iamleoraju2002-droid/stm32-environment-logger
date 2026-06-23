#ifndef SSD_H
#define SSD_H

#include "stm32f4xx.h"

#define SSD1306_I2C_ADDR 0x3C

void ssd1306_clear_all(void);
void ssd1306_fill_all(void);
void ssd1306_init(void);
uint8_t ssd1306_set_page(uint8_t page);
uint8_t ssd1306_set_column(uint8_t column);
uint8_t ssd1306_send_command(uint8_t cmd);
uint8_t ssd1306_send_data(uint8_t data);
uint8_t ssd1306_send_multi_data(uint8_t *data, uint16_t len);
uint8_t ssd1306_update_screen(void);
uint8_t ssd1306_draw_pixel(uint8_t x, uint8_t y);
uint8_t ssd1306_clear_pixel(uint8_t x, uint8_t y);
void ssd1306_clear_buffer(void);
uint8_t ssd1306_draw_hline(uint8_t x1, uint8_t x2, uint8_t y);
uint8_t ssd1306_draw_vline(uint8_t x, uint8_t y1, uint8_t y2);
uint8_t ssd1306_draw_rect(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2);
uint8_t ssd1306_draw_char (uint8_t x, uint8_t page, char ch);
uint8_t ssd1306_draw_string (uint8_t x, uint8_t page, char *str);
void display_main_screen(int16_t temp, int32_t pressure, uint8_t logging);








#endif

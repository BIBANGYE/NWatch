#ifndef __BSP_LCD_H
#define __BSP_LCD_H

#include "include.h"

#define SOFTWARE_SPI 1
#define hardWARE_SPI 0

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

#define X_WIDTH 128
#define Y_WIDTH 64

extern uint8_t OLED_GRAM[1024];

/*
 *  oled (ssd1306) 四线spi引脚定义
 *  D0 -- SCLK（上升沿有效）
 *  D1 -- SDIN
 *  DC -- 数据/命令
 *       -# 0    commamd
 *       -# 1    data
 *  cs -- 片选（低电平有效）
 *  RES-- 复位引脚
 */
#define SCLK_PIN    GPIO_PIN_12  //D0
#define SCLK_PORT   GPIOD
#define RCC_SCLK_ENABLE()   __HAL_RCC_GPIOD_CLK_ENABLE()

#define SDIN_PIN    GPIO_PIN_13   // D1
#define SDIN_PORT   GPIOD
#define RCC_SDIN_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()

#define RES_PIN     GPIO_PIN_10
#define RES_PORT    GPIOD
#define RCC_RES_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()

#define DC_PIN      GPIO_PIN_11
#define DC_PORT     GPIOD
#define RCC_DC_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()

#define CS_PIN      GPIO_PIN_8
#define CS_PORT     GPIOD
#define RCC_CS_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()



#define CS_LOW()        HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET)
#define CS_HIGH()       HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET)
#define LCD_RST_CLR()   HAL_GPIO_WritePin(RES_PORT, RES_PIN, GPIO_PIN_RESET)
#define LCD_RST_SET()   HAL_GPIO_WritePin(RES_PORT, RES_PIN, GPIO_PIN_SET)
#define LCD_DC_CLR()    HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_RESET)
#define LCD_DC_SET()    HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_SET)

#define LCD_SCL_CLR()   HAL_GPIO_WritePin(SCLK_PORT, SCLK_PIN, GPIO_PIN_RESET)
#define LCD_SCL_SET()   HAL_GPIO_WritePin(SCLK_PORT, SCLK_PIN, GPIO_PIN_SET)

#define LCD_SDA_CLR()   HAL_GPIO_WritePin(SDIN_PORT, SDIN_PIN, GPIO_PIN_RESET)
#define LCD_SDA_SET()   HAL_GPIO_WritePin(SDIN_PORT, SDIN_PIN, GPIO_PIN_SET)
        

void oled_init(void);
void oled_delay_ms(uint32_t ms);
void OLED_WR_Byte(uint8_t byte,uint8_t cmd);
void OLED_ColorTurn(uint8_t color_format);
void OLED_DisplayTurn(uint8_t i);
void oled_color_filp(uint8_t color_format);
void oled_display_direction(uint8_t direction);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void oled_flush(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode);
void OLED_DrawCircle(uint8_t x,uint8_t y,uint8_t r);
    
void LCD_PutPixel(uint8_t x, uint8_t y);

void LCD_Set_Pos(uint8_t x, uint8_t y);
void lcd_Fill(uint8_t bmp_data);
void Draw_BMP(uint8_t x, uint8_t y, const uint8_t *bmp);
void LCD_CLS_y(char y);
#endif


/**
 ******************************************************************************
 * @file  oled�����ļ�
 * @brief
 * @details
 * @author
 * @date  2022.05.16
 * @version
 * @copyright Copyright (c)
 ******************************************************************************
 * @attention
 *
 * @par �޸���־:
 * <table>
 * <tr><th>ʱ��               <th>�汾      <th>����    <th>���¼�¼  </tr>
 * <tr><td>2022.05.16         <td>v1.0      <td>bby     <td>�����ļ�  </tr>
 * </table>
 ******************************************************************************
 */

#include "bsp_lcd.h"


uint8_t OLED_GRAM[1024];

uint8_t oledBuffer[FRAME_BUFFER_SIZE];	     //ģ��oled�Ļ�����

static void oled_GPIO_init(void);
static void oled_rcc_init(void);

static void oled_rcc_init(void)
{
    #if SOFTWARE_SPI /* ���SPI */
    RCC_SCLK_ENABLE();
    RCC_SDIN_ENABLE();
    RCC_RES_ENABLE();
    RCC_DC_ENABLE();
    RCC_CS_ENABLE();
    #elif hardWARE_SPI /* Ӳ��SPI */

    #endif
}

static void oled_GPIO_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    #if SOFTWARE_SPI /* ���SPI */
    oled_rcc_init();

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pin = RES_PIN;
    HAL_GPIO_Init(RES_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = SCLK_PIN;
    HAL_GPIO_Init(SCLK_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = SDIN_PIN;
    HAL_GPIO_Init(SDIN_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = DC_PIN;
    HAL_GPIO_Init(DC_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = CS_PIN;
    HAL_GPIO_Init(CS_PORT, &GPIO_InitStruct);
    #elif hardWARE_SPI /* Ӳ��SPI */

    #endif
}

void oled_delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}

//��ɫ��ת����
void OLED_ColorTurn(uint8_t color_format)
{
    if(color_format == 0)
    {
        OLED_WR_Byte(0xA6, OLED_CMD); //������ʾ
    }

    if(color_format == 1)
    {
        OLED_WR_Byte(0xA7, OLED_CMD); //��ɫ��ʾ
    }

    OLED_WR_Byte(0xAF, OLED_CMD); //������ʾ
}

//��Ļ��ת180��
void oled_display_direction(uint8_t direction)
{
    if(direction == 0)
    {
        OLED_WR_Byte(0xC8, OLED_CMD); //������ʾ
        OLED_WR_Byte(0xA1, OLED_CMD);
    }

    if(direction == 1)
    {
        OLED_WR_Byte(0xC0, OLED_CMD); //��ת��ʾ
        OLED_WR_Byte(0xA0, OLED_CMD);
    }
}

void OLED_WR_Byte(uint8_t byte, uint8_t cmd)
{
    uint8_t i = 8;

    (cmd == OLED_DATA) ? LCD_DC_SET() : LCD_DC_CLR();
    LCD_SCL_CLR();

    while(i--)
    {
        if(byte & 0x80)
        {
            LCD_SDA_SET();
        }
        else
        {
            LCD_SDA_CLR();
        }

        LCD_SCL_SET();
        LCD_SCL_CLR();
        byte <<= 1;
    }
}

//����OLED��ʾ
void OLED_DisplayTurn(uint8_t i)
{
    if(i == 0)
    {
        OLED_WR_Byte(0xC8, OLED_CMD); //������ʾ
        OLED_WR_Byte(0xA1, OLED_CMD);
    }

    if(i == 1)
    {
        OLED_WR_Byte(0xC0, OLED_CMD); //��ת��ʾ
        OLED_WR_Byte(0xA0, OLED_CMD);
    }
}

//�ر�OLED��ʾ
void OLED_DisPlay_Off(void)
{
    OLED_WR_Byte(0x8D, OLED_CMD); //��ɱ�ʹ��
    OLED_WR_Byte(0x10, OLED_CMD); //�رյ�ɱ�
    OLED_WR_Byte(0xAE, OLED_CMD); //�ر���Ļ
}

//�����Դ浽OLED
void OLED_Refresh(void)
{
    oled_flush();
}

//��������
void OLED_Clear(void)
{
    memset(&oledBuffer, 0x00, FRAME_BUFFER_SIZE);

    OLED_Refresh();//������ʾ
}

//����
//x:0~127
//y:0~63
//t:1 ��� 0,���
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t)
{
    uint8_t pos, bx, temp = 0;

    if(x > 127 || y > 63)return; //������Χ��.

    pos = y / 8;
    bx = y % 8;
    temp = 1 << (bx);

    if(t) OLED_GRAM[128 * pos + x] |= temp; // ������Ч�������Ŵ�� ����Ϊy ������Ϊx
    else OLED_GRAM[128 * pos + x] &= ~temp;
}

//����
//x1,y1:�������
//x2,y2:��������
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t mode)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; //������������
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;

    if(delta_x > 0)incx = 1; //���õ�������
    else if(delta_x == 0)incx = 0; //��ֱ��
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if(delta_y > 0)incy = 1;
    else if(delta_y == 0)incy = 0; //ˮƽ��
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if( delta_x > delta_y)distance = delta_x; //ѡȡ��������������
    else distance = delta_y;

    for(t = 0; t <= distance + 1; t++ ) //�������
    {
        OLED_DrawPoint(uRow, uCol, 1); //����
        xerr += delta_x ;
        yerr += delta_y ;

        if(xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }

        if(yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}

//x,y:Բ������
//r:Բ�İ뾶
void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r)
{
    int a, b, num;
    a = 0;
    b = r;

    while(2 * b * b >= r * r)
    {
        OLED_DrawPoint(x + a, y - b, 1);
        OLED_DrawPoint(x - a, y - b, 1);
        OLED_DrawPoint(x - a, y + b, 1);
        OLED_DrawPoint(x + a, y + b, 1);

        OLED_DrawPoint(x + b, y + a, 1);
        OLED_DrawPoint(x + b, y - a, 1);
        OLED_DrawPoint(x - b, y - a, 1);
        OLED_DrawPoint(x - b, y + a, 1);

        a++;
        num = (a * a + b * b) - r * r; //���㻭�ĵ���Բ�ĵľ���

        if(num > 0)
        {
            b--;
            a--;
        }
    }
}


void oled_init(void)
{
    oled_GPIO_init();

    CS_LOW();

    LCD_SCL_SET();

    LCD_RST_CLR();
    oled_delay_ms(50);
    LCD_RST_SET();

    OLED_WR_Byte(0xae, OLED_CMD);
    OLED_WR_Byte(0x00, OLED_CMD);
    OLED_WR_Byte(0x10, OLED_CMD);
    OLED_WR_Byte(0x40, OLED_CMD);
    OLED_WR_Byte(0x81, OLED_CMD);
    OLED_WR_Byte(0xcf, OLED_CMD);
    OLED_WR_Byte(0xa1, OLED_CMD);
    OLED_WR_Byte(0xc8, OLED_CMD);
    OLED_WR_Byte(0xa6, OLED_CMD);
    OLED_WR_Byte(0xa8, OLED_CMD);
    OLED_WR_Byte(0x3f, OLED_CMD);
    OLED_WR_Byte(0xd3, OLED_CMD);
    OLED_WR_Byte(0x00, OLED_CMD);
    OLED_WR_Byte(0xd5, OLED_CMD);
    OLED_WR_Byte(0x80, OLED_CMD);
    OLED_WR_Byte(0xd9, OLED_CMD);
    OLED_WR_Byte(0xf1, OLED_CMD);
    OLED_WR_Byte(0xda, OLED_CMD);
    OLED_WR_Byte(0x12, OLED_CMD);
    OLED_WR_Byte(0xdb, OLED_CMD);
    OLED_WR_Byte(0x40, OLED_CMD);
    OLED_WR_Byte(0x20, OLED_CMD);
    OLED_WR_Byte(0x02, OLED_CMD);
    OLED_WR_Byte(0x8d, OLED_CMD);
    OLED_WR_Byte(0x14, OLED_CMD);
    OLED_WR_Byte(0xa4, OLED_CMD);
    OLED_WR_Byte(0xa6, OLED_CMD);
    OLED_WR_Byte(0xaf, OLED_CMD);

    lcd_Fill(0x00);  //��ʼ����
    LCD_Set_Pos(0, 0);
}

void LCD_PutPixel(uint8_t x, uint8_t y)
{
    uint8_t data1;  //data1��ǰ�������

    data1 = 0x01 << (y % 8);
    OLED_WR_Byte(0xb0 + (y >> 3), OLED_CMD);
    OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
    OLED_WR_Byte((x & 0x0f) | 0x00, OLED_CMD);
    OLED_WR_Byte(data1, OLED_DATA);
}



void lcd_Fill(uint8_t bmp_data)
{
    uint8_t y, x;

    for(y = 0; y < 8; y++)
    {
        OLED_WR_Byte(0xb0 + y, OLED_CMD);
        OLED_WR_Byte(0x01, OLED_CMD);
        OLED_WR_Byte(0x10, OLED_CMD);

        for(x = 0; x < X_WIDTH; x++)
            OLED_WR_Byte(bmp_data, OLED_DATA);
    }
}

void LCD_Set_Pos(uint8_t x, uint8_t y)
{
    OLED_WR_Byte(0xb0 + (y >> 3), OLED_CMD);
    OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
    OLED_WR_Byte((x & 0x0f) | 0x01, OLED_CMD);
}


void Draw_BMP(uint8_t x, uint8_t y, const uint8_t *bmp)
{
    uint8_t wm = 0;
    uint8_t adder = 0;

    LCD_Set_Pos(x, y);

    for(wm = 0; wm < 16; wm++)
    {
        OLED_WR_Byte(*(bmp + adder), OLED_DATA);
        adder += 1;
    }

    LCD_Set_Pos(x, y + 8);

    for(wm = 0; wm < 16; wm++)
    {
        OLED_WR_Byte(*(bmp + adder), OLED_DATA);
        adder += 1;
    }
}

//���һ��0-63
void LCD_CLS_y(char y)
{
    uint8_t x;

    OLED_WR_Byte(0xb0 + (y >> 3), OLED_CMD);
    OLED_WR_Byte(0x01, OLED_CMD);
    OLED_WR_Byte(0x10, OLED_CMD);

    for(x = 0; x < X_WIDTH; x++)
    {
        OLED_WR_Byte(0, OLED_DATA);
    }
}

void oled_flush(void)
{
    uint8_t i, n;

    for(i = 0; i < 8; i++)
    {
        OLED_WR_Byte(0xb0 + i, OLED_CMD); //��������ʼ��ַ
        OLED_WR_Byte(0x00, OLED_CMD);  //���õ�����ʼ��ַ
        OLED_WR_Byte(0x10, OLED_CMD);  //���ø�����ʼ��ַ

        for(n = 0; n < 128; n++)
            OLED_WR_Byte(oledBuffer[128 * i + n], OLED_DATA);
    }

    memset(&oledBuffer, 0x00, FRAME_BUFFER_SIZE);
}


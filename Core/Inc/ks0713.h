
#ifndef _KS0713_H
#define _KS0713_H
#include <stdint.h>

#define KS0713_WIDTH		128
#define KS0713_HEIGHT		64

#define KS0713_DISP_OFF					(0xAE)
#define KS0713_DISP_ON					(0xAF)
#define KS0713_ADC_SELECT_NORMAL		(0xA0)
#define KS0713_ADC_SELECT_REVERSE		(0xA1)
#define KS0713_DISP_MODE_NORMAL			(0xA6)
#define KS0713_DISP_MODE_REVERSE		(0xA7)
#define KS0713_ENTIRE_DISP_NORMAL		(0xA4)
#define KS0713_ENTIRE_DISP_ON			(0xA5)
#define KS0713_LCD_BIAS					(0xA2)
#define KS0713_SHL_SELECT_NORMAL		(0xC0)
#define KS0713_SHL_SELECT_REVERSE		(0xC8)
#define KS0713_POWER_CTRL_VC			(0x2C)
#define KS0713_POWER_CTRL_VR			(0x2E)
#define KS0713_POWER_CTRL_VF			(0x2F)
#define KS0713_REG_RES_SEL				(0x25)
#define KS0713_SET_REF_VOLTAGE			(0x81)
#define KS0713_SET_REF_VOLT_REG			(0x18)
#define KS0713_CONTRAST 				(0x25)
#define KS0713_DISPLAY_LINE				(0x40)
#define KS0713_SET_PAGE_ADDR			(0xB0)
#define KS0713_SET_COL_ADDR_MSB			(0x10)
#define KS0713_SET_COL_ADDR_LSB			(0x00)
#define KS0713_SET_MOD_READ				(0xE0)
#define KS0713_UNSET_MOD_READ			(0xEE)
#define KS0713_RESET					(0xE2)
#define KS0713_STATIC_IND_MODE			(0xAC)





void LCD_IO_Init(void);
void LCD_Delay(uint32_t Delay);

void ks0713_init (void);
void ks0713_WriteData(uint16_t DataValue);
void ks0713_WriteCommand( uint8_t CommandValue);
void ks0713_WriteBytePixel (int8_t x, int8_t y, uint8_t pixel);
void ks0713_WritePixel (int8_t x, int8_t y);



void SPI_Write(uint16_t Value);
void SPI_Error(void);


void LCD_TestPixel(void);
void LCD_Clear(void);
void LCD_Fill(void);
void LCD_DrawHLine(int8_t y1, int8_t x1, int8_t x2);
void LCD_DrawVLine(int8_t x, int8_t y1, int8_t y2);
void LCD_DrawRectangle(int8_t x1, int8_t y1, int8_t x2, int8_t y2);
void LCD_DisplayPicture(void);
void LCD_DisplayPicture2 (void);
void LCD_WriteFont(int num, uint8_t x, uint8_t y);
void LCD_ShowString(uint8_t *str, uint16_t x,uint16_t y);
uint8_t  LCD_DisplayChar(uint8_t str);
void LCD_Gotoxy(uint8_t x, uint8_t y);
void LCD_RandomPixel (void);
void LCD_DemoMode (void);
void LCD_ReverseMode(void);
void LCD_NormalMode(void);






#endif //

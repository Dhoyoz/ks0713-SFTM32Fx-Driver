#include <string.h>
#include "stm32f4xx_hal.h"
#include "main.h"
#include "ks0713.h"
#include "lcd_font_7x8.h"
#include "logo.h"
#include "logocupra64.h"

#define KS0713_CHAR_WIDTH 		7
#define KS0713_CHAR_HEIGHT 		8
#define KS0713_CONTRAST_MIN		25
#define KS0713_CONTRAST_MAX		60
#define SPI_TIMEOUT_MAX  		0x1000


extern SPI_HandleTypeDef hspi1;

void ks0713_init ()
{
	LCD_IO_Init();
	LCD_Delay (50);
	ks0713_WriteCommand( KS0713_RESET);						// reset
	LCD_Delay (50);
	ks0713_WriteCommand( KS0713_ADC_SELECT_NORMAL);			// ADC select: normal
	ks0713_WriteCommand( KS0713_SHL_SELECT_NORMAL);			// SHL select:
	ks0713_WriteCommand( KS0713_LCD_BIAS);					// LCD bias select
	ks0713_WriteCommand( KS0713_POWER_CTRL_VC);				// Voltage converter ON - VC
	LCD_Delay (1);
	ks0713_WriteCommand( KS0713_POWER_CTRL_VR);				// Voltage regulator ON - VR
	LCD_Delay (1);
	ks0713_WriteCommand( KS0713_POWER_CTRL_VF);				// Voltage follower ON - VF
	ks0713_WriteCommand( KS0713_REG_RES_SEL);				// Regulator resistor select 0x20..0x27
	ks0713_WriteCommand( KS0713_SET_REF_VOLTAGE);			// Reference voltage register set
	ks0713_WriteCommand( KS0713_DISPLAY_LINE);				// initial display line 0x40..0x7f
	ks0713_WriteCommand( KS0713_DISP_ON);					// display on
	ks0713_WriteCommand( KS0713_ENTIRE_DISP_NORMAL);		// Reverse display On/Off
	LCD_Delay (50);
	ks0713_WriteCommand( KS0713_SET_REF_VOLTAGE);			// Reference voltage 0..63
	ks0713_WriteCommand( KS0713_CONTRAST);
	LCD_Clear();
}

/*
 * ******************************************************************************************************
 *
 * ******************************************************************************************************
 */
void LCD_DemoMode (void){
	LCD_ShowString ("KS0713 Driver", 15,16);
	LCD_DrawHLine (9,0,127);
	LCD_ShowString ("Hello World", 20,32);
	LCD_DrawHLine (42,0,127);
	LCD_ShowString ("LasCosasdeDani",10 ,55);
	LCD_DrawRectangle (0,0,127,63);
	LCD_Delay (5000);
	LCD_Clear();
	LCD_DrawRectangle (0,0,127,63);
	LCD_Delay (1500);
	LCD_DrawRectangle (8,8,119,55);
	LCD_Delay (1500);
	LCD_DrawRectangle (16,16,111,47);
	LCD_Delay (1500);
	LCD_DrawRectangle (24,24,103,39);
	LCD_Delay (3000);
	LCD_Clear();
	LCD_DisplayPicture ();
	LCD_Delay (5000);
    LCD_Clear();
    LCD_DisplayPicture2 ();
    LCD_ShowString ("Image ",70,8);
    LCD_ShowString ("64x64px ",70,24);

	for (uint8_t a = 4; a > 0; a--){
		LCD_WriteFont (a,90 ,48);
		LCD_Delay (750);
	}

	LCD_RandomPixel ();
	LCD_Clear();

	for (uint8_t a = 0; a<8; a++){
		LCD_Fill();
		LCD_Delay(100);
		LCD_Clear();
		LCD_Delay (100);
	}

	LCD_TestPixel ();
	LCD_Clear();

}

/*
 * ******************************************************************************************************
 *
 * ******************************************************************************************************
 */
void LCD_DisplayPicture2 (void){
	uint8_t pixel = 0;
	uint8_t lo_col, hi_col, x1;
	uint16_t index;
	x1 = 10;

	  lo_col= (x1&0x0f);
	  hi_col= ((x1>>4)&0x0f)|0x10;

  for(int x=0; x<8; x++){
	  ks0713_WriteCommand(KS0713_SET_PAGE_ADDR + x);   //y, page address y=1-0-1-1-y3-y2-y1-y0, 1-page with 8-rows
	  ks0713_WriteCommand(hi_col);
	  ks0713_WriteCommand(lo_col);

    for(int i=0; i<64; i++){

    	pixel = logocupra[index];
    	//pixel = picture[index];
    	ks0713_WriteData(pixel);
    	index++;
      }
  }
}




/*
 * ******************************************************************************************************
 *
 * ******************************************************************************************************
 */
void LCD_RandomPixel (void){
	uint8_t x;
	uint8_t y;
	uint8_t byte;
	for (uint16_t a = 0 ; a<10000; a++ ){
	x = random();
	y = random();
	byte = random();
	while (x > 127) x=x/5;
	while (y > 63) y=y/4;
	while (y > 255) y=y/3;
	//ks0713_WritePixel (x, y);
	ks0713_WriteBytePixel (x,y, byte);
	LCD_Delay (1);
	//LCD_Clear();
	}
}



/*
 * ******************************************************************************************************
 *
 * ******************************************************************************************************
 */
void LCD_TestPixel(void){
  for(int x=0; x<8; x++){
	  ks0713_WriteCommand(KS0713_SET_PAGE_ADDR + x);
	  ks0713_WriteCommand(KS0713_SET_COL_ADDR_MSB);
	  ks0713_WriteCommand(KS0713_SET_COL_ADDR_LSB);

    for(int i=0; i<128; i++){
    	ks0713_WriteData(0xff);
    	LCD_Delay(1);
      }
  }
  LCD_Delay (1500);
  for(int x=0; x<8; x++){
	  ks0713_WriteCommand(KS0713_SET_PAGE_ADDR + x);
	  ks0713_WriteCommand(KS0713_SET_COL_ADDR_MSB);
	  ks0713_WriteCommand(KS0713_SET_COL_ADDR_LSB);

    for(int i=0; i<128; i++){
    	ks0713_WriteData(0x00);
    	LCD_Delay(1);
      }
  }

}

/*
 * ******************************************************************************************************
 *
 * ******************************************************************************************************
 */
void LCD_Clear(void){
  for(int x=0; x<8; x++){
	  ks0713_WriteCommand(KS0713_SET_PAGE_ADDR + x);
	  ks0713_WriteCommand(KS0713_SET_COL_ADDR_MSB);
	  ks0713_WriteCommand(KS0713_SET_COL_ADDR_LSB);
    for(int i=0; i<128; i++){
    	ks0713_WriteData(0x0);
      }
  }
}



/*
 * ******************************************************************************************************
 *
 * ******************************************************************************************************
 */
void LCD_Fill(void){
  for(int x=0; x<8; x++){
	  ks0713_WriteCommand(KS0713_SET_PAGE_ADDR + x);
	  ks0713_WriteCommand(KS0713_SET_COL_ADDR_MSB);
	  ks0713_WriteCommand(KS0713_SET_COL_ADDR_LSB);
    for(int i=0; i<128; i++){
    	ks0713_WriteData(0xff);
      }
  }
}



/*
 * ******************************************************************************************************
 *
 * ******************************************************************************************************
 */
void LCD_DrawHLine(int8_t y1, int8_t x1, int8_t x2){
	uint8_t delta_x;
	delta_x=x2-x1;
	for(int a=0; a <=delta_x ;a++ )
	{
		ks0713_WritePixel(x1++,y1);

	}
}

/*
 * ******************************************************************************************************
 *
 * ******************************************************************************************************
 */
void LCD_DrawVLine(int8_t x, int8_t y1, int8_t y2){
	uint64_t Columna=0;
	uint8_t delta_y;
	delta_y=y2-y1;
	uint8_t PixelBlock [8] = {0};

	for (int a=0; a<delta_y; a++){
		Columna = Columna << 1;
		Columna++;
	}
	Columna = Columna << y1;

	for (int a=0; a<8; a++){
		PixelBlock[a] = Columna;
		Columna = Columna >> 8;
	}
	for (int a=1; a<9; a++){
		if (PixelBlock[a-1] != 0){
		ks0713_WriteBytePixel(x,((a-1)*8),PixelBlock[a-1]);
		}
	}
}



/*
 * ******************************************************************************************************
 *
 * ******************************************************************************************************
 */
void LCD_DrawRectangle(int8_t x1, int8_t y1, int8_t x2, int8_t y2){
	LCD_DrawHLine(y1,x1,x2);
	LCD_DrawHLine(y2,x1,x2);
	LCD_DrawVLine(x1,y1,y2);
	LCD_DrawVLine(x2,y1,y2);
}



/*
 * ******************************************************************************************************
 *
 * ******************************************************************************************************
 */
void LCD_DisplayPicture(void){
	uint8_t pixel = 0;
	uint16_t index;
  for(int x=0; x<8; x++){
	  ks0713_WriteCommand(KS0713_SET_PAGE_ADDR + x);
	  ks0713_WriteCommand(KS0713_SET_COL_ADDR_MSB);
	  ks0713_WriteCommand(KS0713_SET_COL_ADDR_LSB);
    for(int i=0; i<128; i++){
    	pixel = gImage_logo[index];
    	ks0713_WriteData(pixel);
    	index++;
      }
  }
}



/*
 * ******************************************************************************************************
 *
 * ******************************************************************************************************
 */
void ks0713_WritePixel (int8_t x, int8_t y){
	LCD_Gotoxy(x,y);
	uint8_t pixel;
	if (y <= 7){
		if (y==7) pixel = 0x80;
		if (y==6) pixel = 0x40;
		if (y==5) pixel = 0x20;
		if (y==4) pixel = 0x10;
		if (y==3) pixel = 0x08;
		if (y==2) pixel = 0x04;
		if (y==1) pixel = 0x02;
		if (y==0) pixel = 0x01;
	}else{
		y=y/8;
		if (y==8) pixel = 0x80;
		if (y==7) pixel = 0x40;
		if (y==6) pixel = 0x20;
		if (y==5) pixel = 0x10;
		if (y==4) pixel = 0x08;
		if (y==3) pixel = 0x04;
		if (y==2) pixel = 0x02;
		if (y==1) pixel = 0x01;
	}
	ks0713_WriteData(pixel);
}



/*
 * ******************************************************************************************************
 *
 * ******************************************************************************************************
 */
void ks0713_WriteBytePixel (int8_t x, int8_t y, uint8_t pixel){
	LCD_Gotoxy(x,y);
	ks0713_WriteData(pixel);
}



/*
 * ******************************************************************************************************
 *
 * ******************************************************************************************************
 */
void LCD_WriteFont(int num, uint8_t x, uint8_t y){
	uint8_t pixel;
	LCD_Gotoxy(x,y);
  for(int i=0; i<7; i++){
	  pixel = font_7x8[num][i];
	  ks0713_WriteData(pixel);
    }
}



/*
 * ******************************************************************************************************
 *
 * ******************************************************************************************************
 */
void LCD_ShowString(uint8_t *str, uint16_t x,uint16_t y){
	uint8_t len = 0;
	len = strlen(str);
		for(int num=0; num<len; num++){
			LCD_WriteFont ( LCD_DisplayChar(str[num]), x, y);
			x=x+8;
    }
}



/*
 * ******************************************************************************************************
 *
 * ******************************************************************************************************
 */
uint8_t  LCD_DisplayChar(uint8_t str){

    switch(str){
      case '0': return (0); break;
      case '1': return (1); break;
      case '2': return (2); break;
      case '3': return (3); break;
      case '4': return (4); break;
      case '5': return (5); break;
      case '6': return (6); break;
      case '7': return (7); break;
      case '8': return (8); break;
      case '9': return (9); break;
      case 'a': return (10); break;
      case 'b': return (11); break;
      case 'c': return (12); break;
      case 'd': return (13); break;
      case 'e': return (14); break;
      case 'f': return (15); break;
      case 'g': return (16); break;
      case 'h': return (17); break;
      case 'i': return (18); break;
      case 'j': return (19); break;
      case 'k': return (20); break;
      case 'l': return (21); break;
      case 'm': return (22); break;
      case 'n': return (23); break;
      case 'o': return (24); break;
      case 'p': return (25); break;
      case 'q': return (26); break;
      case 'r': return (27); break;
      case 's': return (28); break;
      case 't': return (29); break;
      case 'u': return (30); break;
      case 'v': return (31); break;
      case 'w': return (32); break;
      case 'x': return (33); break;
      case 'y': return (34); break;
      case 'z': return (35); break;
      case 'A': return (36); break;
      case 'B': return (37); break;
      case 'C': return (38); break;
      case 'D': return (39); break;
      case 'E': return (40); break;
      case 'F': return (41); break;
      case 'G': return (42); break;
      case 'H': return (43); break;
      case 'I': return (44); break;
      case 'J': return (45); break;
      case 'K': return (46); break;
      case 'L': return (47); break;
      case 'M': return (48); break;
      case 'N': return (49); break;
      case 'O': return (50); break;
      case 'P': return (51); break;
      case 'Q': return (52); break;
      case 'R': return (53); break;
      case 'S': return (54); break;
      case 'T': return (55); break;
      case 'U': return (56); break;
      case 'V': return (57); break;
      case 'W': return (58); break;
      case 'X': return (59); break;
      case 'Y': return (60); break;
      case 'Z': return (61); break;
      case '!': return (62); break;
      case '"': return (63); break;
      case '#': return (64); break;
      case '$': return (65); break;
      case '%': return (66); break;
      case '&': return (67); break;
      case '\'': return (68); break;
      case '(': return (69); break;
      case ')': return (70); break;
      case '*': return (71); break;
      case '+': return (72); break;
      case ',': return (73); break;
      case '-': return (74); break;
      case '/': return (75); break;
      case ':': return (76); break;
      case ';': return (77); break;
      case '<': return (78); break;
      case '=': return (79); break;
      case '>': return (80); break;
      case '?': return (81); break;
      case '@': return (82); break;
      case '{': return (83); break;
      case '|': return (84); break;
      case '}': return (85); break;
      case '~': return (86); break;
      case ' ': return (87); break;
      case '.': return (88); break;
      case '^': return (89); break;
      case '_': return (90); break;
      case '`': return (91); break;
      case '[': return (92); break;
      case '\\': return (93); break;
      case ']': return (94); break;
      default: break;
  }
}



/*
 * ******************************************************************************************************
 *
 * ******************************************************************************************************
 */
void LCD_ReverseMode(void){
	ks0713_WriteCommand(KS0713_DISP_MODE_REVERSE);
}


/*
 * ******************************************************************************************************
 *
 * ******************************************************************************************************
 */
void LCD_NormalMode(void){
	ks0713_WriteCommand(KS0713_DISP_MODE_NORMAL);
}


/*
 * ******************************************************************************************************
 *
 * ******************************************************************************************************
 */
void ks0713_WriteData(uint16_t DataValue){
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCD_SPI_CS_GPIO_Port, LCD_SPI_CS_Pin, GPIO_PIN_RESET);
	SPI_Write(DataValue);
	HAL_GPIO_WritePin(LCD_SPI_CS_GPIO_Port, LCD_SPI_CS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);
}

/*
 * ******************************************************************************************************
 *
 * ******************************************************************************************************
 */
void ks0713_WriteCommand( uint8_t CommandValue){
    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_SPI_CS_GPIO_Port, LCD_SPI_CS_Pin, GPIO_PIN_RESET);
    SPI_Write(CommandValue);
    HAL_GPIO_WritePin(LCD_SPI_CS_GPIO_Port, LCD_SPI_CS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET);

}

/*
 * ******************************************************************************************************
 *
 * ******************************************************************************************************
 */
void LCD_Gotoxy(uint8_t x, uint8_t y){
  char hi_col,lo_col;
  uint8_t PageADDR = 0 ;
  lo_col= (x&0x0f);
  hi_col= ((x>>4)&0x0f)|0x10;
  PageADDR=(y/8);
  ks0713_WriteCommand(hi_col);
  ks0713_WriteCommand(lo_col);
  ks0713_WriteCommand(KS0713_SET_PAGE_ADDR + PageADDR);
  }


/*
 * ******************************************************************************************************
 *
 * ******************************************************************************************************
 */
void SPI_Write(uint16_t Value){
    HAL_StatusTypeDef status = HAL_OK;
    status = HAL_SPI_Transmit(&hspi1, (uint8_t*) &Value, 1, SPI_TIMEOUT_MAX);
    if(status != HAL_OK)
    {
      SPI_Error();
    }
  }




/*
 * ******************************************************************************************************
 *
 * ******************************************************************************************************
 */
void SPI_Error(void){
    HAL_SPI_DeInit(&hspi1);
    HAL_SPI_Init(&hspi1);
}



  /*
   * ******************************************************************************************************
   *
   * ******************************************************************************************************
   */
void LCD_IO_Init(void){
    /*Reset the control line */
    LCD_Delay(15);
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
    LCD_Delay(15);
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCD_SPI_CS_GPIO_Port, LCD_SPI_CS_Pin, GPIO_PIN_SET);
}

  /*
   * ******************************************************************************************************
   *
   * ******************************************************************************************************
   */
void LCD_Delay(uint32_t Delay){
    HAL_Delay(Delay);
  }

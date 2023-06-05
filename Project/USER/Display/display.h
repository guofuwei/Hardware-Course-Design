/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DISPLAY_H
#define __DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
typedef struct{
	uint16_t x_cur;
	uint16_t y_cur;
}CURSOR;


uint8_t private_cursor_check(void);

void display_string(char* pFont, uint8_t size,uint16_t fColor, uint16_t bColor);
void display_string_center(char* pFont, uint8_t size,uint16_t fColor, uint16_t bColor);
void display_string_custom(char* pFont, uint8_t size,uint16_t fColor, uint16_t bColor,uint16_t x,uint16_t y);
void clear_screen(void);

#ifdef __cplusplus
}
#endif

#endif

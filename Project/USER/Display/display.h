/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DISPLAY_H
#define __DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

#define SCREEN_HEAD_HEIGHT 16 // 显示标题的高度
//#define HEAD_FCOLOR BLACK
//#define HEAD_BCOLOR CYAN

typedef struct{
	uint16_t x_cur;
	uint16_t y_cur;
}CURSOR;

// 内部调用函数
uint8_t private_cursor_check(void);

// 基础显示函数
void display_string(char* pFont, uint8_t size,uint16_t fColor, uint16_t bColor);
void display_string_center(char* pFont, uint8_t size,uint16_t fColor, uint16_t bColor);
void display_string_custom(char* pFont, uint8_t size,uint16_t fColor, uint16_t bColor,uint16_t x,uint16_t y);
void set_head_string(char* pFont,uint16_t fColor, uint16_t bColor);
void clear_screen_all(void);
void clear_screen_head( uint16_t bColor);
void clear_screen_content(void);

// 下面的函数用于菜单绘制
void draw_main_menu(void);
void draw_txt_menu(void);
void draw_pic_menu(void);
void draw_music_menu(void);

void clear_select(void);
void draw_select(void);

// 具体的内容显示
void show_txt_content(void);
void show_pic_content(void);

#ifdef __cplusplus
}
#endif

#endif

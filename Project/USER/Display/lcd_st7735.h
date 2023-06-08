#ifndef __LCD_H
#define __LCD_H

/***********************************************************************************************************************************
    ** 显示屏信号引脚请在main.h中按规范定义
    ** GND         电源地
    ** VCC         3.3V。不要接5V, 模块不带LDO，5V很易烧屏
    ** SCK         SPI的SCK引脚，H/L，模拟SPI要看协议时序
    ** MOSI        SPI的MOSI引脚，H=1/L=0
    ** RST         屏的复位引脚，EN/DIS
    ** DC          命令COM/数据DAT 选择引脚
    ** CS          SPI的片选引脚，EN/DIS
    ** BKL         控制背光灯-BackLight，ON/OFF。
    **  
    ** 测试硬件    Nucleo_STM32F401RE + 1.8寸彩屏模块(ST7735)
************************************************************************************************************************************/
#include "main.h" 
#include "gpio.h"
#include "spi.h"
#include "w25qxx.h"
#include "stdio.h"        // C标准库头文件: 输入输出函数；getchar()、putchar()、scanf()、printf()、gets()、puts()、sprintf()
#include "stdlib.h"       // C标准库头文件: 通用工具函数：malloc()、calloc()、realloc()、free()、system()、atoi()、atol()、rand()、srand()、exit()
#include "stdbool.h"      // C标准库头文件: 定义布尔类型: bool、true、false
#include "string.h"       // C标准库头文件: 字符数组常用：strcpy()、strncpy()、strcmp()、strlen()、strnset()

// GPIO控制屏幕简单硬件部分的宏操作函数在main.h，

// 屏幕参数
#define LCD_WIDTH         130              // 设置屏幕横向像素大小，此参数适用128*nnn屏_1.44寸  
//#define LCD_HIGH          130              // 实际像素要比标示值大1~2像素，High130适合高128屏_1.44寸  
#define LCD_HIGH          162              // High162适合128*160屏_1.80寸
#define LCD_DIR           1                // 四种显示方向，参数：1、2、3、4

/*****************************************************************************
 ** 颜色定义
*************************************************************************** */
#define BLUE                          (uint16_t)0x001F	   //蓝色 0x001F
#define GREEN                         (uint16_t)0x07E0	   //绿色 
#define RED                           (uint16_t)0xF800	   //红色 0xF800
#define CYAN                          (uint16_t)0x07FF	   //蓝绿色，青色
#define MAGENTA                       (uint16_t)0xF81F	   //红紫色，洋红色
#define YELLOW                        (uint16_t)0xFFE0	   //黄色 
#define LIGHTBLUE                     (uint16_t)0x841F
#define LIGHTGREEN                    (uint16_t)0x87F0
#define LIGHTRED                      (uint16_t)0xFC10
#define LIGHTCYAN                     (uint16_t)0x87FF
#define LIGHTMAGENTA                  (uint16_t)0xFC1F
#define LIGHTYELLOW                   (uint16_t)0xFFF0
#define DARKBLUE                      (uint16_t)0x0010
#define DARKGREEN                     (uint16_t)0x0400
#define DARKRED                       (uint16_t)0x8000
#define DARKCYAN                      (uint16_t)0x0410
#define DARKMAGENTA                   (uint16_t)0x8010
#define DARKYELLOW                    (uint16_t)0x8400
#define WHITE                         (uint16_t)0xFFFF	   //白色
#define LIGHTGRAY                     (uint16_t)0xD69A
#define GRAY                          (uint16_t)0x8410	   //灰色 
#define DARKGRAY                      (uint16_t)0x4208
#define BLACK                         (uint16_t)0x0000	   //黑色 
#define BROWN                         (uint16_t)0xA145
#define ORANGE                        (uint16_t)0xFD20


#define GOODBLUE                      (uint16_t)0x646BD9
/*****************************************************************************
 ** 声明  全局函数； 对应lcd_st7735.c中的函数主体
****************************************************************************/
void LCD_Init(short BG);
//LCD驱动ST7735初始化，屏幕参数在本文件顶部修改，主要是像素尺寸、显示方向

void LCD_Fill(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t color);
// 填充单色
void LCD_Line(uint16_t sX, uint16_t sY, uint16_t eX, uint16_t eY, uint16_t color);
// 画线
void LCD_Circle(uint16_t x0,uint16_t y0,uint8_t r, uint16_t color);
// 画圆

void LCD_Image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *image);

void drawAscii(uint16_t x,uint16_t y,uint8_t num, uint8_t size,uint16_t fColor, uint16_t bColor);
// 大量Ascii字符数据及几个特定汉字16*16字库数据以及一副LOG图数据在font.h文件中，适合固定需要输出

void drawString(uint16_t x,uint16_t y, char* pFont, uint8_t size,uint16_t fColor, uint16_t bColor);
// 任意字符、汉字显示：需要数据在存储器（比如魔女开发板有W25Q128带字库文件，size最大32）
int LCD_String(uint16_t x,uint16_t y,char* pFont, uint8_t size,uint16_t fColor, uint16_t bColor);
void LED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1,uint16_t fColor, uint16_t bColor);
	
// void LCD_Chinese32ForFile(uint32_t x, uint32_t y, uint8_t index, uint16_t fColor, uint16_t bColor);
// void LCD_Chinese16ForFile(uint32_t x, uint32_t y, uint8_t index, uint16_t fColor, uint16_t bColor);
// 特定汉字32*32字库数据没有放入font.h，故此函数无法使用


void drawPoint(uint16_t x, uint16_t y, uint16_t color);

#endif  
 




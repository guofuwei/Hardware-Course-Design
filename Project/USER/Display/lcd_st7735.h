#ifndef __LCD_H
#define __LCD_H

/***********************************************************************************************************************************
    ** ��ʾ���ź���������main.h�а��淶����
    ** GND         ��Դ��
    ** VCC         3.3V����Ҫ��5V, ģ�鲻��LDO��5V��������
    ** SCK         SPI��SCK���ţ�H/L��ģ��SPIҪ��Э��ʱ��
    ** MOSI        SPI��MOSI���ţ�H=1/L=0
    ** RST         ���ĸ�λ���ţ�EN/DIS
    ** DC          ����COM/����DAT ѡ������
    ** CS          SPI��Ƭѡ���ţ�EN/DIS
    ** BKL         ���Ʊ����-BackLight��ON/OFF��
    **  
    ** ����Ӳ��    Nucleo_STM32F401RE + 1.8�����ģ��(ST7735)
************************************************************************************************************************************/
#include "main.h" 
#include "gpio.h"
#include "spi.h"
#include "w25qxx.h"
#include "stdio.h"        // C��׼��ͷ�ļ�: �������������getchar()��putchar()��scanf()��printf()��gets()��puts()��sprintf()
#include "stdlib.h"       // C��׼��ͷ�ļ�: ͨ�ù��ߺ�����malloc()��calloc()��realloc()��free()��system()��atoi()��atol()��rand()��srand()��exit()
#include "stdbool.h"      // C��׼��ͷ�ļ�: ���岼������: bool��true��false
#include "string.h"       // C��׼��ͷ�ļ�: �ַ����鳣�ã�strcpy()��strncpy()��strcmp()��strlen()��strnset()

// GPIO������Ļ��Ӳ�����ֵĺ����������main.h��

// ��Ļ����
#define LCD_WIDTH         130              // ������Ļ�������ش�С���˲�������128*nnn��_1.44��  
//#define LCD_HIGH          130              // ʵ������Ҫ�ȱ�ʾֵ��1~2���أ�High130�ʺϸ�128��_1.44��  
#define LCD_HIGH          162              // High162�ʺ�128*160��_1.80��
#define LCD_DIR           1                // ������ʾ���򣬲�����1��2��3��4

/*****************************************************************************
 ** ��ɫ����
*************************************************************************** */
#define BLUE                          (uint16_t)0x001F	   //��ɫ 0x001F
#define GREEN                         (uint16_t)0x07E0	   //��ɫ 
#define RED                           (uint16_t)0xF800	   //��ɫ 0xF800
#define CYAN                          (uint16_t)0x07FF	   //����ɫ����ɫ
#define MAGENTA                       (uint16_t)0xF81F	   //����ɫ�����ɫ
#define YELLOW                        (uint16_t)0xFFE0	   //��ɫ 
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
#define WHITE                         (uint16_t)0xFFFF	   //��ɫ
#define LIGHTGRAY                     (uint16_t)0xD69A
#define GRAY                          (uint16_t)0x8410	   //��ɫ 
#define DARKGRAY                      (uint16_t)0x4208
#define BLACK                         (uint16_t)0x0000	   //��ɫ 
#define BROWN                         (uint16_t)0xA145
#define ORANGE                        (uint16_t)0xFD20


#define GOODBLUE                      (uint16_t)0x646BD9
/*****************************************************************************
 ** ����  ȫ�ֺ����� ��Ӧlcd_st7735.c�еĺ�������
****************************************************************************/
void LCD_Init(short BG);
//LCD����ST7735��ʼ������Ļ�����ڱ��ļ������޸ģ���Ҫ�����سߴ硢��ʾ����

void LCD_Fill(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t color);
// ��䵥ɫ
void LCD_Line(uint16_t sX, uint16_t sY, uint16_t eX, uint16_t eY, uint16_t color);
// ����
void LCD_Circle(uint16_t x0,uint16_t y0,uint8_t r, uint16_t color);
// ��Բ

void LCD_Image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *image);

void drawAscii(uint16_t x,uint16_t y,uint8_t num, uint8_t size,uint16_t fColor, uint16_t bColor);
// ����Ascii�ַ����ݼ������ض�����16*16�ֿ������Լ�һ��LOGͼ������font.h�ļ��У��ʺϹ̶���Ҫ���

void drawString(uint16_t x,uint16_t y, char* pFont, uint8_t size,uint16_t fColor, uint16_t bColor);
// �����ַ���������ʾ����Ҫ�����ڴ洢��������ħŮ��������W25Q128���ֿ��ļ���size���32��
int LCD_String(uint16_t x,uint16_t y,char* pFont, uint8_t size,uint16_t fColor, uint16_t bColor);
void LED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1,uint16_t fColor, uint16_t bColor);
	
// void LCD_Chinese32ForFile(uint32_t x, uint32_t y, uint8_t index, uint16_t fColor, uint16_t bColor);
// void LCD_Chinese16ForFile(uint32_t x, uint32_t y, uint8_t index, uint16_t fColor, uint16_t bColor);
// �ض�����32*32�ֿ�����û�з���font.h���ʴ˺����޷�ʹ��


void drawPoint(uint16_t x, uint16_t y, uint16_t color);

#endif  
 




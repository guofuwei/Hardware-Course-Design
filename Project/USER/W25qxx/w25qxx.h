#ifndef __W25QX_H
#define __W25QX_H
/*===============================================================================================================
    *   本工程使用的是魔女开发板，板载的SPI FLASH已保存汉字字库~
    *   W25Q128 存储位置解释
    *  【起始地址】   【结束地址】           【 用 途 】
    *   0x00A00000  -  0x01000000        重要：已保存汉字字库数据，不要写些区域地址    
    *   0x00002000  -  0x00A00000        用户自定义
    *   0x00001000  -  0x00002000        建议用作：参数配置数据区 备份
    *   0x00000000  -  0x00001000        建议用作：参数配置数据区 
 ================================================================================================================                                                                                                  
 ===============================================================================================================*/  
#include "stm32f1xx_hal.h"
#include "main.h"
#include "spi.h"
#include "stdio.h"        // C标准库头文件: 输入输出函数；getchar()、putchar()、scanf()、printf()、gets()、puts()、sprintf()
#include "stdlib.h"       // C标准库头文件: 通用工具函数：malloc()、calloc()、realloc()、free()、system()、atoi()、atol()、rand()、srand()、exit()
#include "stdbool.h"      // C标准库头文件: 定义布尔类型: bool、true、false
#include "string.h"       // C标准库头文件: 字符数组常用：strcpy()、strncpy()、strcmp()、strlen()、strnset()

// 移植参数  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++          
/* 以下定义有用，但被转移到main.h集中
#define   W25QX_NSS_HIGH			HAL_GPIO_WritePin( W25QXX_NSS_GPIOx,W25QXX_NSS_PIN,GPIO_PIN_SET) 
#define   W25QX_NSS_LOW				HAL_GPIO_WritePin( W25QXX_NSS_GPIOx,W25QXX_NSS_PIN,GPIO_PIN_RESET) 
#define   W25QXX_SPIx          SPI1          // SPIx
#define   W25QXX_SPI_CLK_EN    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;  // SPI时钟

//  以下定义使用CubeMX及硬件SPI后无需用户处理
#define   W25QXX_NSS_GPIOx     GPIOA         // NSS
#define   W25QXX_NSS_PIN       GPIO_PIN_2    // STD库中是Pin，HAL库用PIN ！！

#define   W25QXX_CLK_GPIOx     GPIOA         // CLK
#define   W25QXX_CLK_PIN       GPIO_PIN_5

#define   W25QXX_MISO_GPIOx    GPIOA         // MISO
#define   W25QXX_MISO_PIN      GPIO_PIN_6

#define   W25QXX_MOSI_GPIOx    GPIOA         // MOSI
#define   W25QXX_MOSI_PIN      GPIO_PIN_7          
//  ++++++++++++++++++++++++++++++++++++++++++ 
*/

#define   GBK_STORAGE_ADDR     0x00A00000    // GBK字库起始地址,魔女开发板的W25Q128已保存宋体12、16、24、32号字体
 
/*****************************************************************************
 ** 声明全局函数，已封闭成3个对外函数，可完成对其所有存取操作
****************************************************************************/
void W25qx_Init(void);
void W25qxx_ReadBuffer ( uint32_t addr, uint8_t *p, uint16_t num);
void W25qxx_WriteBuffer( uint32_t addr, uint8_t *p, uint16_t num);   

uint16_t W25qxx_StartupTimes(void);                     // 获取有记录的上电启动次数


#endif



#ifndef __W25QX_H
#define __W25QX_H
/*===============================================================================================================
    *   ������ʹ�õ���ħŮ�����壬���ص�SPI FLASH�ѱ��溺���ֿ�~
    *   W25Q128 �洢λ�ý���
    *  ����ʼ��ַ��   ��������ַ��           �� �� ; ��
    *   0x00A00000  -  0x01000000        ��Ҫ���ѱ��溺���ֿ����ݣ���ҪдЩ�����ַ    
    *   0x00002000  -  0x00A00000        �û��Զ���
    *   0x00001000  -  0x00002000        ������������������������ ����
    *   0x00000000  -  0x00001000        ������������������������ 
 ================================================================================================================                                                                                                  
 ===============================================================================================================*/  
#include "stm32f1xx_hal.h"
#include "main.h"
#include "spi.h"
#include "stdio.h"        // C��׼��ͷ�ļ�: �������������getchar()��putchar()��scanf()��printf()��gets()��puts()��sprintf()
#include "stdlib.h"       // C��׼��ͷ�ļ�: ͨ�ù��ߺ�����malloc()��calloc()��realloc()��free()��system()��atoi()��atol()��rand()��srand()��exit()
#include "stdbool.h"      // C��׼��ͷ�ļ�: ���岼������: bool��true��false
#include "string.h"       // C��׼��ͷ�ļ�: �ַ����鳣�ã�strcpy()��strncpy()��strcmp()��strlen()��strnset()

// ��ֲ����  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++          
/* ���¶������ã�����ת�Ƶ�main.h����
#define   W25QX_NSS_HIGH			HAL_GPIO_WritePin( W25QXX_NSS_GPIOx,W25QXX_NSS_PIN,GPIO_PIN_SET) 
#define   W25QX_NSS_LOW				HAL_GPIO_WritePin( W25QXX_NSS_GPIOx,W25QXX_NSS_PIN,GPIO_PIN_RESET) 
#define   W25QXX_SPIx          SPI1          // SPIx
#define   W25QXX_SPI_CLK_EN    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;  // SPIʱ��

//  ���¶���ʹ��CubeMX��Ӳ��SPI�������û�����
#define   W25QXX_NSS_GPIOx     GPIOA         // NSS
#define   W25QXX_NSS_PIN       GPIO_PIN_2    // STD������Pin��HAL����PIN ����

#define   W25QXX_CLK_GPIOx     GPIOA         // CLK
#define   W25QXX_CLK_PIN       GPIO_PIN_5

#define   W25QXX_MISO_GPIOx    GPIOA         // MISO
#define   W25QXX_MISO_PIN      GPIO_PIN_6

#define   W25QXX_MOSI_GPIOx    GPIOA         // MOSI
#define   W25QXX_MOSI_PIN      GPIO_PIN_7          
//  ++++++++++++++++++++++++++++++++++++++++++ 
*/

#define   GBK_STORAGE_ADDR     0x00A00000    // GBK�ֿ���ʼ��ַ,ħŮ�������W25Q128�ѱ�������12��16��24��32������
 
/*****************************************************************************
 ** ����ȫ�ֺ������ѷ�ճ�3�����⺯��������ɶ������д�ȡ����
****************************************************************************/
void W25qx_Init(void);
void W25qxx_ReadBuffer ( uint32_t addr, uint8_t *p, uint16_t num);
void W25qxx_WriteBuffer( uint32_t addr, uint8_t *p, uint16_t num);   

uint16_t W25qxx_StartupTimes(void);                     // ��ȡ�м�¼���ϵ���������


#endif



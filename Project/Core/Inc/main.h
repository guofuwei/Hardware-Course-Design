/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KeyBoard_Col1_Pin GPIO_PIN_0
#define KeyBoard_Col1_GPIO_Port GPIOC
#define KeyBoard_Col2_Pin GPIO_PIN_1
#define KeyBoard_Col2_GPIO_Port GPIOC
#define KeyBoard_Col3_Pin GPIO_PIN_2
#define KeyBoard_Col3_GPIO_Port GPIOC
#define KeyBoard_Col4_Pin GPIO_PIN_3
#define KeyBoard_Col4_GPIO_Port GPIOC
#define KEY1_Pin GPIO_PIN_0
#define KEY1_GPIO_Port GPIOA
#define KEY1_EXTI_IRQn EXTI0_IRQn
#define LCD_A1_Pin GPIO_PIN_1
#define LCD_A1_GPIO_Port GPIOA
#define W25Q_CS_Pin GPIO_PIN_2
#define W25Q_CS_GPIO_Port GPIOA
#define TF_CS_Pin GPIO_PIN_3
#define TF_CS_GPIO_Port GPIOA
#define LCD_A4_Pin GPIO_PIN_4
#define LCD_A4_GPIO_Port GPIOA
#define LCD_C4_Pin GPIO_PIN_4
#define LCD_C4_GPIO_Port GPIOC
#define KEY2_Pin GPIO_PIN_5
#define KEY2_GPIO_Port GPIOC
#define KEY2_EXTI_IRQn EXTI9_5_IRQn
#define LCD_B12_Pin GPIO_PIN_12
#define LCD_B12_GPIO_Port GPIOB
#define LED_RED_Pin GPIO_PIN_8
#define LED_RED_GPIO_Port GPIOA
#define KEY3_Pin GPIO_PIN_15
#define KEY3_GPIO_Port GPIOA
#define KEY3_EXTI_IRQn EXTI15_10_IRQn
#define LED_BLUE_Pin GPIO_PIN_2
#define LED_BLUE_GPIO_Port GPIOD
#define KeyBoard_Row1_Pin GPIO_PIN_7
#define KeyBoard_Row1_GPIO_Port GPIOB
#define KeyBoard_Row2_Pin GPIO_PIN_8
#define KeyBoard_Row2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define huartX            huart1           // ���ڲ�����Ҫ,��LCD��ӦUART���б�ʱ�޸Ĵ˴����ɣ����統ǰ��USART1

#define hspiX             hspi1             // LCD_SPI������Ҫ,��LCD��ӦSPI���б�ʱ�޸Ĵ˴����ɣ����統ǰ��SPI1
#define LCD_SPI_Init      //MX_SPI1_Init()  // LCD_SPI������Ҫ,��LCD��ӦSPI���б�ʱ�޸Ĵ˴����ɣ����統ǰ��SPI1
                                            // �������SPI�����Զ���Ϊ�ղ����������lcd.c
//�������SPI�豸��ͬ��SPI��ʽ������ÿ���豸�л�ʱ����������SPI_Init��ֻҪ�л�NSS��
//�õ�ǰ�豸NSSΪ�ͼ��ɣ���Ҳ�����SPIInit()�������ɲ����޸ĺ�����롣


#define LED_RED_OFF			  HAL_GPIO_WritePin( LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_SET)	    // ����PIN����ߣ�
#define LED_RED_ON				HAL_GPIO_WritePin( LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_RESET)		// ����
#define LED_RED_TOGGLE		HAL_GPIO_TogglePin(LED_RED_GPIO_Port,LED_RED_Pin)					          // ������ת

#define LED_BLUE_OFF			HAL_GPIO_WritePin( LED_BLUE_GPIO_Port,LED_BLUE_Pin,GPIO_PIN_SET)	    // ����PIN����ߣ�
#define LED_BLUE_ON				HAL_GPIO_WritePin( LED_BLUE_GPIO_Port,LED_BLUE_Pin,GPIO_PIN_RESET)		// ����
#define LED_BLUE_TOGGLE		HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port,LED_BLUE_Pin)					          // ������ת


// GPIO����SPI��Ӳ�����ֵĺ���������������𱳹⣬Ƭѡ����λ������/����
// ��Ҫ��Ӳ��˵��ȷ�϶�Ӧ�ܽŸߵ͵�ƽ����Ļ�����壬���統ǰ ����ܽ��ø�1SET=��������

//�ؼ���Ƭѡ�źţ�ÿһ��SlaveSPI��һ��Ƭѡ���͵�ƽѡ��
#define LCD_CS_EN         HAL_GPIO_WritePin( LCD_A4_GPIO_Port, LCD_A4_Pin,  GPIO_PIN_RESET)
#define LCD_CS_DIS        HAL_GPIO_WritePin( LCD_A4_GPIO_Port, LCD_A4_Pin,  GPIO_PIN_SET)
#define W25QX_NSS_HIGH	  HAL_GPIO_WritePin( W25Q_CS_GPIO_Port,W25Q_CS_Pin, GPIO_PIN_SET) 
#define W25QX_NSS_LOW			HAL_GPIO_WritePin( W25Q_CS_GPIO_Port,W25Q_CS_Pin, GPIO_PIN_RESET) 

#define LCD_BKL_ON        HAL_GPIO_WritePin( LCD_A1_GPIO_Port, LCD_A1_Pin,  GPIO_PIN_SET)
#define LCD_BKL_OFF       HAL_GPIO_WritePin( LCD_A1_GPIO_Port, LCD_A1_Pin,  GPIO_PIN_RESET)
#define LCD_BKL_TOGGLE    HAL_GPIO_TogglePin(LCD_A1_GPIO_Port, LCD_A1_Pin )

#define LCD_RST_EN        HAL_GPIO_WritePin( LCD_B12_GPIO_Port, LCD_B12_Pin,  GPIO_PIN_RESET)
#define LCD_RST_DIS       HAL_GPIO_WritePin( LCD_B12_GPIO_Port, LCD_B12_Pin,  GPIO_PIN_SET)
#define LCD_DC_DAT        HAL_GPIO_WritePin( LCD_C4_GPIO_Port,LCD_C4_Pin, GPIO_PIN_SET)
#define LCD_DC_COM        HAL_GPIO_WritePin( LCD_C4_GPIO_Port,LCD_C4_Pin, GPIO_PIN_RESET)



// �Լ���ӵĶ���
typedef enum{
  SYS_OK =0,
  SYS_SD_ERROR
}SYSINFO;  // ϵͳ״̬

typedef enum{
  LOADING_MENU,
  MAIN_MENU,
  TXT_MENU,
  PIC_MENU,
  MUSIC_MENU,
  TXT_PIC_DETAIL_MENU,
  MUSIC_DETAIL_MENU
}MENUINFO;  // �ܹ��Ĳ˵�

typedef enum{
  KEY_ACCEPT,
  KEY_DENY
}KEYSTATUS;

#define MENU_HEIGHT_SIZE 16
#define NAMELIST_NUM  10
#define NAMELIST_MAX_LEN 15

void main_state_machine(uint16_t GPIO_Pin);
void main_menu_handle(uint16_t GPIO_Pin);
void txt_menu_handle(uint16_t GPIO_Pin);
void pic_menu_handle(uint16_t GPIO_Pin);
void music_menu_handle(uint16_t GPIO_Pin);


// �����SCK, MOSI ���ƺ궨�������SPI��ʽ����
//#define LCD_SCK_H         HAL_GPIO_WritePin( LCD_A5_GPIO_Port, LCD_A5_Pin, GPIO_PIN_SET) 
//#define LCD_SCK_L         HAL_GPIO_WritePin( LCD_A5_GPIO_Port, LCD_A5_Pin, GPIO_PIN_RESET) 
//#define LCD_MOSI_H        HAL_GPIO_WritePin( LCD_A7_GPIO_Port, LCD_A7_Pin, GPIO_PIN_SET) 
//#define LCD_MOSI_L        HAL_GPIO_WritePin( LCD_A7_GPIO_Port, LCD_A7_Pin, GPIO_PIN_RESET) 
/***********************************************************************************************************************************
    ** ��ʾ���ź���������main.h�а��淶ת��������
       GND         ��Դ��
       VCC         3.3V����Ҫ��5V, ģ�鲻��LDO��5V��������
    ** SCK         SPI��SCK���ţ�MCU���͵����ڴ����ʱ���ź�,H/L
    ** MOSI        SPI��MOSI����(������SDA��)���Ǵ�MCU�͸������յ�����,H=1/L=0
    ** RST         ���ĸ�λ���ţ�EN/DISable
    ** DC          ��������COM/����DAT ѡ������
    ** CS          SPI��Ƭѡ���ţ�EN/DISable
    ** BKL         ���Ʊ����-BackLight��ON/OFF�������Ѵ�1K���裬����10K�������ߵ�ƽ����
       ����Ӳ��    ħŮF103RE + ħŮ����1.8�����ģ��(ST7735)
       ����SPI����ֻҪ��ST7735���ܽ�˳��һ�£�������϶���ͬ���öŰ�����Ӧת�Ӽ����ô����̡�
       ����SPI���ܽŻ�����Ӧ�������ĸ��ܽ���˳��ͬ������������ת���������޸ĺ��Կ�ֱ��ʹ�á�
************************************************************************************************************************************/

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

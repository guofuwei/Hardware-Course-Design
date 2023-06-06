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
#define LCD_B12_Pin GPIO_PIN_12
#define LCD_B12_GPIO_Port GPIOB
#define A8_Led_Pin GPIO_PIN_8
#define A8_Led_GPIO_Port GPIOA
#define D2_Led_Pin GPIO_PIN_2
#define D2_Led_GPIO_Port GPIOD
#define KeyBoard_Col1_Pin GPIO_PIN_3
#define KeyBoard_Col1_GPIO_Port GPIOB
#define KeyBoard_Col2_Pin GPIO_PIN_4
#define KeyBoard_Col2_GPIO_Port GPIOB
#define KeyBoard_Col3_Pin GPIO_PIN_5
#define KeyBoard_Col3_GPIO_Port GPIOB
#define KeyBoard_Col4_Pin GPIO_PIN_6
#define KeyBoard_Col4_GPIO_Port GPIOB
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


#define R_LED_OFF			  HAL_GPIO_WritePin( A8_Led_GPIO_Port,A8_Led_Pin,GPIO_PIN_SET)	    // ����PIN����ߣ�
#define R_LED_ON				HAL_GPIO_WritePin( A8_Led_GPIO_Port,A8_Led_Pin,GPIO_PIN_RESET)		// ����
#define R_LED_TOGGLE		HAL_GPIO_TogglePin(A8_Led_GPIO_Port,A8_Led_Pin)					          // ������ת

#define B_LED_OFF			  HAL_GPIO_WritePin( D2_Led_GPIO_Port,D2_Led_Pin,GPIO_PIN_SET)	    // ����PIN����ߣ�
#define B_LED_ON				HAL_GPIO_WritePin( D2_Led_GPIO_Port,D2_Led_Pin,GPIO_PIN_RESET)		// ����
#define B_LED_TOGGLE		HAL_GPIO_TogglePin(D2_Led_GPIO_Port,D2_Led_Pin)					          // ������ת

//#define KU_L001_Pin       GPIO_PIN_15       // �ظ����壬�����ǽ�A15_KEY���¶���ΪKU_L001
//#define KU_L001_GPIO_Port GPIOA
//#define KU_L001_EXTI_IRQn EXTI15_10_IRQn    // K ����,U ������������,L001 ���󿴵�3�����أ�


//#define KU_L001_DOWN      GPIO_PIN_RESET    // ���󿴵�3�����ذ���Ϊ��0��ƽ
//#define KU_L001_UP        GPIO_PIN_SET      // ���U �������򲻰���Ϊ�ߵ�ƽ1
//#define KU_L010_DOWN      GPIO_PIN_RESET    //
//#define KU_L010_UP        GPIO_PIN_SET      //  

//#define KU_L010_Pin       GPIO_PIN_5        // �ظ����壬�����ǽ�C5_KEY���¶���ΪKU_L010
//#define KU_L010_GPIO_Port GPIOC             // ���󿴵�2������ 
//#define KU_L010_EXTI_IRQn EXTI9_5_IRQn

//#define KU_L000_Pin       GPIO_PIN_0        // �ظ����壬�����ǽ�A0_KEY���¶���ΪKU_L000
//#define KU_L000_GPIO_Port GPIOA             // ���󿴵�2������ 
//#define KU_L000_EXTI_IRQn EXTI0_IRQn

//#define KU_L011_Pin       GPIO_PIN_3        // �ظ����壬�����ǽ�B3_KEY���¶���ΪKU_L011
//#define KU_L011_GPIO_Port GPIOB             // ���󿴵�2������ 
//#define KU_L011_EXTI_IRQn EXTI3_IRQn

//#define KU_L100_Pin       GPIO_PIN_4        // �ظ����壬�����ǽ�B4_KEY���¶���ΪKU_L100
//#define KU_L100_GPIO_Port GPIOB             // ���󿴵�2������ 
//#define KU_L100_EXTI_IRQn EXTI4_IRQn

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

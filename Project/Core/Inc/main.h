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
#define huartX            huart1           // 串口操作需要,在LCD对应UART口有变时修改此处即可，比如当前是USART1

#define hspiX             hspi1             // LCD_SPI操作需要,在LCD对应SPI口有变时修改此处即可，比如当前是SPI1
#define LCD_SPI_Init      //MX_SPI1_Init()  // LCD_SPI操作需要,在LCD对应SPI口有变时修改此处即可，比如当前是SPI1
                                            // 若是软仿SPI，可以定义为空操作，避免改lcd.c
//当多个从SPI设备用同种SPI方式，不必每个设备切换时都重新设置SPI_Init，只要切换NSS，
//让当前设备NSS为低即可，故也定义空SPIInit()，这样可不必修改后面代码。


#define R_LED_OFF			  HAL_GPIO_WritePin( A8_Led_GPIO_Port,A8_Led_Pin,GPIO_PIN_SET)	    // 灯灭（PIN输出高）
#define R_LED_ON				HAL_GPIO_WritePin( A8_Led_GPIO_Port,A8_Led_Pin,GPIO_PIN_RESET)		// 灯亮
#define R_LED_TOGGLE		HAL_GPIO_TogglePin(A8_Led_GPIO_Port,A8_Led_Pin)					          // 灯亮灭反转

#define B_LED_OFF			  HAL_GPIO_WritePin( D2_Led_GPIO_Port,D2_Led_Pin,GPIO_PIN_SET)	    // 灯灭（PIN输出高）
#define B_LED_ON				HAL_GPIO_WritePin( D2_Led_GPIO_Port,D2_Led_Pin,GPIO_PIN_RESET)		// 灯亮
#define B_LED_TOGGLE		HAL_GPIO_TogglePin(D2_Led_GPIO_Port,D2_Led_Pin)					          // 灯亮灭反转

//#define KU_L001_Pin       GPIO_PIN_15       // 重复定义，等于是将A15_KEY重新定义为KU_L001
//#define KU_L001_GPIO_Port GPIOA
//#define KU_L001_EXTI_IRQn EXTI15_10_IRQn    // K 开关,U 本板必须加上拉,L001 从左看第3个开关，


//#define KU_L001_DOWN      GPIO_PIN_RESET    // 从左看第3个开关按下为低0电平
//#define KU_L001_UP        GPIO_PIN_SET      // 设好U 上拉，则不按下为高电平1
//#define KU_L010_DOWN      GPIO_PIN_RESET    //
//#define KU_L010_UP        GPIO_PIN_SET      //  

//#define KU_L010_Pin       GPIO_PIN_5        // 重复定义，等于是将C5_KEY重新定义为KU_L010
//#define KU_L010_GPIO_Port GPIOC             // 从左看第2个开关 
//#define KU_L010_EXTI_IRQn EXTI9_5_IRQn

//#define KU_L000_Pin       GPIO_PIN_0        // 重复定义，等于是将A0_KEY重新定义为KU_L000
//#define KU_L000_GPIO_Port GPIOA             // 从左看第2个开关 
//#define KU_L000_EXTI_IRQn EXTI0_IRQn

//#define KU_L011_Pin       GPIO_PIN_3        // 重复定义，等于是将B3_KEY重新定义为KU_L011
//#define KU_L011_GPIO_Port GPIOB             // 从左看第2个开关 
//#define KU_L011_EXTI_IRQn EXTI3_IRQn

//#define KU_L100_Pin       GPIO_PIN_4        // 重复定义，等于是将B4_KEY重新定义为KU_L100
//#define KU_L100_GPIO_Port GPIOB             // 从左看第2个开关 
//#define KU_L100_EXTI_IRQn EXTI4_IRQn

// GPIO控制SPI简单硬件部分的宏操作函数，如亮灭背光，片选，复位，命令/数据
// 需要按硬件说明确认对应管脚高低电平对屏幕的意义，比如当前 背光管脚置高1SET=背光亮起

//关键的片选信号，每一个SlaveSPI有一个片选，低电平选中
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


// 下面的SCK, MOSI 控制宏定义在软仿SPI方式才用
//#define LCD_SCK_H         HAL_GPIO_WritePin( LCD_A5_GPIO_Port, LCD_A5_Pin, GPIO_PIN_SET) 
//#define LCD_SCK_L         HAL_GPIO_WritePin( LCD_A5_GPIO_Port, LCD_A5_Pin, GPIO_PIN_RESET) 
//#define LCD_MOSI_H        HAL_GPIO_WritePin( LCD_A7_GPIO_Port, LCD_A7_Pin, GPIO_PIN_SET) 
//#define LCD_MOSI_L        HAL_GPIO_WritePin( LCD_A7_GPIO_Port, LCD_A7_Pin, GPIO_PIN_RESET) 
/***********************************************************************************************************************************
    ** 显示屏信号引脚请在main.h中按规范转换定义名
       GND         电源地
       VCC         3.3V。不要接5V, 模块不带LDO，5V很易烧屏
    ** SCK         SPI的SCK引脚，MCU发送的用于传输的时钟信号,H/L
    ** MOSI        SPI的MOSI引脚(别名：SDA等)，是从MCU送给屏接收的数据,H=1/L=0
    ** RST         屏的复位引脚，EN/DISable
    ** DC          屏的命令COM/数据DAT 选择引脚
    ** CS          SPI的片选引脚，EN/DISable
    ** BKL         控制背光灯-BackLight，ON/OFF（引脚已串1K电阻，且已10K上拉，高电平亮）
       测试硬件    魔女F103RE + 魔女标配1.8寸彩屏模块(ST7735)
       其他SPI屏，只要是ST7735，管脚顺序不一致，但含义肯定相同，用杜邦线相应转接即可用此例程。
       个别SPI屏管脚基本对应，但后四个管脚有顺序不同，在上面引脚转换定义中修改后仍可直插使用。
************************************************************************************************************************************/

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

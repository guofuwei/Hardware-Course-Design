/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "lcd_st7735.h"
#include "w25qxx.h"
#include "spi_sdcard.h"
#include "display.h"
#include "keyboard.h"
#include "malloc.h"
#include "VS1053.h"
#include "piclib.h"
#include "control.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// 用于SD读取的全局变量
char NameList[NAMELIST_NUM][NAMELIST_MAX_LEN] = {"\0"}; // 文件列表缓冲区
FATFS Fs;
FIL File;
UINT Br, Bw;

// 用于按键状态机转移
SYSINFO SysInfo = SYS_OK;         // 系统状态
MENUINFO MenuInfo = LOADING_MENU; // 当前在哪一个菜单
KEYSTATUS KeyStatus = KEY_DENY;
VS1053STATUS Vs1053Status = VS1053_PLAY;
uint8_t SelectIndex = 0;           // 当前选中的index
bool SubMenuOk = true;             // 子菜单是否ok
uint8_t FileNum = 0;               // 扫描出来的文件个数，防止select指示过度
char Buffer[BUFFER_SIZE] = {'\0'}; // 文本读取内容缓冲区
int16_t BufferPos = 0;             // 文本读取内容缓冲区位置
bool IsEndTxt = false;             // 文本是否读取完毕

long VS1053_CURRENTPOS = 0; // 当前播放的位置
extern _vs10xx_obj vsset;   // VS1053的设置

// 用于在主函数中播放
char SongFullName[10 + NAMELIST_MAX_LEN] = {'\0'};
bool IsPlay = false;
bool IsStop = false;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_FATFS_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */

  //  HAL_NVIC_SetPriority(SysTick_IRQn,0,0U);
  //  HAL_NVIC_SetPriority(TIM,1,0U);

  LCD_CS_DIS;      // 尽管从设备基本操作函数中有片选的处理
  W25QX_NSS_HIGH;  // 但仍要注意切换同一SPI从设备的片选CS
  LCD_Init(BLACK); // LCD驱动ST7735先初始化
  W25qx_Init();    // W25qx再初始化（内带字库）
                   // atk_mo1053_init();
  //  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  // 初始化屏幕标题
  set_head_string("系统初始化中", BLACK, GOODBLUE);
  // 启动定时器
  __HAL_TIM_CLEAR_IT(&htim7, TIM_IT_UPDATE);
  HAL_TIM_Base_Start_IT(&htim7);
  // 初始化TF卡
  uint8_t res = sd_init();
  sd_init_handle(res);

  res = f_mount(&Fs, "0", 1);
  f_mount_handle(res);

  res = f_scandir(ROOT_FOLDER);
  f_scandir_handle(res);

  // 初始化画图
  piclib_init();

  printf("你好\n");
  if (res != FR_OK)
  {
    SysInfo = SYS_SD_ERROR;
    display_string_center("系统初始化失败", 16, RED, BLACK);
    display_string_center("按任意键退出", 16, YELLOW, BLACK);
    KeyStatus = KEY_ACCEPT;
  }
  else
  {
    display_string_center("系统初始化成功", 16, GREEN, BLACK);
    display_string_center("按任意键继续", 16, GREEN, BLACK);
    KeyStatus = KEY_ACCEPT;
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if (IsPlay && !IsStop)
    {
      song_play(SongFullName, 0);
			IsPlay = false;
			if( Vs1053Status == VS1053_MOVE)
				IsPlay = true;
      Vs1053Status = VS1053_PLAY;
      
    }
    if (IsPlay && IsStop)
    {
      song_play(SongFullName, VS1053_CURRENTPOS);
			IsPlay = false;
			if( Vs1053Status == VS1053_MOVE)
				IsPlay = true;
      Vs1053Status = VS1053_PLAY;
      
    }
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM7)
  {
    uint8_t res = KEY_SCAN();
    if (res != 0 && MenuInfo == MUSIC_DETAIL_MENU)
    {
      if (res == 1)
      {
        music_run_stop();
      }
      if (res == 2)
      {
        addvolume();
      }
      if (res == 3)
      {
        subvolume();
      }
			if(res==4)
			{
				Vs1053Status = VS1053_MOVE;
				IsStop=1;
				IsPlay=1;				
			}
      printf("res:%d", res);
    }
    else if (res != 0 && MenuInfo == TXT_DETAIL_MENU)
    {
      if (res == 2)
      {
        txt_next_page();
      }
      else if (res == 1)
      {
        txt_prev_page();
      }
    }
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if ((GPIO_Pin == KEY1_Pin || GPIO_Pin == KEY2_Pin || GPIO_Pin == KEY3_Pin) && (KeyStatus == KEY_ACCEPT))
  {
    main_state_machine(GPIO_Pin);
  }
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

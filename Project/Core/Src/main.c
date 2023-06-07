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
char NameList[NAMELIST_NUM][NAMELIST_MAX_LEN]={"\0"};//文件列表缓冲区
FATFS Fs;
FIL File;
UINT Br,Bw;

// 用于按键状态机转移
SYSINFO SysInfo=SYS_OK;     // 系统状态
MENUINFO MenuInfo=LOADING_MENU; // 当前在哪一个菜单
KEYSTATUS KeyStatus=KEY_DENY;
uint8_t SelectIndex=0; // 当前选中的index
bool SubMenuOk=true;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void song_play(char *path)
{
	  FIL *fmp3;
    uint16_t br;
    uint8_t res, rval;
    uint8_t *databuf;
    uint16_t i = 0;

	
	   rval = 0;
		
    fmp3 = (FIL *)mymalloc(SRAMIN, sizeof(FIL));                        /* 申请内存*/
    databuf = (uint8_t *)mymalloc(SRAMIN, 4096);                       /* 开辟4096字节的内存区域 */

    atk_mo1053_reset();
    atk_mo1053_soft_reset();	
	 //sset.mvol = 200; 

    if (databuf == NULL || fmp3 == NULL)rval = 0XFF ;                   /* 内存申请失败 */

    if (rval == 0)
    {
			
        atk_mo1053_restart_play();      
		/* 重启播放 */
        atk_mo1053_set_all(); 
			/* 设置音量等信息 */
        atk_mo1053_reset_decode_time();                                 /* 复位解码时间 */
        //res = exfuns_file_type(pname);       
	/* 得到文件后缀 */
                            
       // if (res == T_FLAC)                                              /* 如果是flac,加载patch */
      //  {
       //     atk_mo1053_load_patch((uint16_t *)vs1053b_patch, VS1053B_PATCHLEN);
      }
//
      res = f_open(fmp3,path, FA_READ);              /* 打开文件 */
			
			if(res==0)
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
			}

        if (res == 0)                                                   /* 打开成功 */
        {
					//atk_mo1053_spi_speed_high();
           // atk_mo1053_spi_speed_high();                                /* 高速 */
					spi1_set_speed(SPI_SPEED_16);

            while (rval == 0)
            {
                res = f_read(fmp3, databuf, 4096, (UINT *)&br);         /* 读出4096个字节 */
                i = 0;

                do                                                      /* 主播放循环 */
                {
                    if (atk_mo1053_send_music_data(databuf + i) == 0)   /* 给VS10XX发送音频数据 */
                    {
                        i += 32;
                    }
                    else
                    {
                       
                     }
                       // audio_msg_show(fmp3->obj.objsize);              /* 显示信息 */
                    
                } while (i < 4096);                                     /* 循环发送4096个字节 */

                if (br != 4096 || res != 0)
                {
                    //rval = KEY0_PRES;
                    break;                                              /* 读完了 */
                }
            }

            f_close(fmp3);
        }
        else
        {
            rval = 0XFF;                                                /* 出现错误 */
        }
    myfree(SRAMIN,databuf);
    myfree(SRAMIN,fmp3);
    //return rval;
}
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
  
	LCD_CS_DIS;     //尽管从设备基本操作函数中有片选的处理  
  W25QX_NSS_HIGH; //但仍要注意切换同一SPI从设备的片选CS
  LCD_Init(BLACK);     //LCD驱动ST7735先初始化
	W25qx_Init();   //W25qx再初始化（内带字库）
  
  // 初始化屏幕标题
	set_head_string("系统初始化中",WHITE,GOODBLUE);
  // 启动定时器
  __HAL_TIM_CLEAR_IT(&htim7,TIM_IT_UPDATE);	
	HAL_TIM_Base_Start_IT(&htim7);
	// 初始化TF卡
	uint8_t res=sd_init();
	sd_init_handle(res);
	
	res = f_mount(&Fs,"0",1);
	f_mount_handle(res);
	
	res=f_scandir(ROOT_FOLDER);
	f_scandir_handle(res);
  
  uint8_t i=0;
  for(i=0;i<NAMELIST_NUM;i++)
  {
    printf("%s\n",NameList[i]);
  }
  
  
  
  if(res!=FR_OK)
  { 
    SysInfo=SYS_SD_ERROR;
    display_string_center("系统初始化失败",16,RED,BLACK);
    display_string_center("按任意键退出",16,YELLOW,BLACK);
    KeyStatus=KEY_ACCEPT;
  }
  else
  {
    display_string_center("系统初始化成功",16,GREEN,BLACK);
    display_string_center("按任意键继续",16,GREEN,BLACK);
    KeyStatus=KEY_ACCEPT;
  }

	
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
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
	if(htim->Instance==TIM7)
	{
      HW_KEY_FUNCTION();
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if((GPIO_Pin==KEY1_Pin||GPIO_Pin==KEY2_Pin||GPIO_Pin==KEY3_Pin)&&(KeyStatus==KEY_ACCEPT))
	{
    main_state_machine(GPIO_Pin);
  }
}
void main_state_machine(uint16_t GPIO_Pin)
{
  if(SysInfo!=SYS_OK)
  {
    set_head_string("系统已退出",YELLOW,BLACK);
    clear_screen_content();
    KeyStatus=KEY_DENY;
  }
  else
  {
    switch(MenuInfo)
    {
      case LOADING_MENU:
        draw_main_menu();
        break;
      case MAIN_MENU:
        main_menu_handle(GPIO_Pin);
        break;
      case TXT_MENU:
        txt_menu_handle(GPIO_Pin);
        break;
      case PIC_MENU:
        pic_menu_handle(GPIO_Pin);
        break;
      case MUSIC_MENU:
        music_menu_handle(GPIO_Pin);
        break;
      case TXT_PIC_DETAIL_MENU:
        break;
      case MUSIC_DETAIL_MENU:
        break;
    }
  }
}

void main_menu_handle(uint16_t GPIO_Pin)
{
  // 向下翻按钮
  if(GPIO_Pin==KEY1_Pin)
  {
    clear_select();
    SelectIndex=SelectIndex+1;
    if(SelectIndex>=4)
    {
       SelectIndex=0;
    }
    draw_select();
  }
  // 确定按钮
  else if(GPIO_Pin==KEY2_Pin)
  {
    switch(SelectIndex)
    {
      case 0:
        draw_txt_menu();
        break;
      case 1:
        draw_pic_menu();
        break;
      case 2:
        draw_music_menu();
        break;
      case 3:
        break;
      default:
        LED_RED_ON;
        HAL_Delay(100);
        LED_RED_OFF;
    }
  }
  // 返回或者取消按钮（在主菜单无用）
  else if(GPIO_Pin==KEY3_Pin)
  {
    LED_BLUE_ON;
    HAL_Delay(100);
    LED_BLUE_OFF;
  }
   else
  {
    LED_RED_ON;
    HAL_Delay(100);
    LED_RED_OFF;
  }
}


void txt_menu_handle(uint16_t GPIO_Pin)
{
  if(GPIO_Pin==KEY1_Pin && SubMenuOk==true)
  {
    clear_select();
    SelectIndex=SelectIndex+1;
    if(SelectIndex>=4)
    {
       SelectIndex=0;
    }
    draw_select();
  }
  else if(GPIO_Pin==KEY2_Pin  && SubMenuOk==true )
  {
  }
  else if(GPIO_Pin==KEY3_Pin)
  {
    draw_main_menu();
  }
  else
  {
    LED_RED_ON;
    HAL_Delay(100);
    LED_RED_OFF;
  }
}


void pic_menu_handle(uint16_t GPIO_Pin)
{
  if(GPIO_Pin==KEY1_Pin && SubMenuOk==true)
  {
    clear_select();
    SelectIndex=SelectIndex+1;
    if(SelectIndex>=4)
    {
       SelectIndex=0;
    }
    draw_select();
  }
  else if(GPIO_Pin==KEY2_Pin && SubMenuOk==true)
  {
  }
  else if(GPIO_Pin==KEY3_Pin)
  {
    draw_main_menu();
  }
  else
  {
    LED_RED_ON;
    HAL_Delay(100);
    LED_RED_OFF;
  }
}


void music_menu_handle(uint16_t GPIO_Pin)
{
  if(GPIO_Pin==KEY1_Pin && SubMenuOk==true)
  {
    clear_select();
    SelectIndex=SelectIndex+1;
    if(SelectIndex>=4)
    {
       SelectIndex=0;
    }
    draw_select();
  }
  else if(GPIO_Pin==KEY2_Pin && SubMenuOk==true)
  {
  }
  else if(GPIO_Pin==KEY3_Pin)
  {
    draw_main_menu();
  }
  else
  {
    LED_RED_ON;
    HAL_Delay(100);
    LED_RED_OFF;
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

#ifdef  USE_FULL_ASSERT
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

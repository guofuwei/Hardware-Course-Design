/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   fatfs.h
  * @brief  Header for fatfs applications
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
#ifndef __fatfs_H
#define __fatfs_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "ff.h"
#include "ff_gen_drv.h"
#include "user_diskio.h" /* defines USER_Driver as external */

/* USER CODE BEGIN Includes */
#include "display.h"
#include "lcd_st7735.h"

/* USER CODE END Includes */

extern uint8_t retUSER; /* Return value for USER */
extern char USERPath[4]; /* USER logical drive path */
extern FATFS USERFatFS; /* File system object for USER logical drive */
extern FIL USERFile; /* File object for USER */

void MX_FATFS_Init(void);

/* USER CODE BEGIN Prototypes */
typedef enum{
  ROOT_FOLDER=0,
  TXT_FOLDER,
  PIC_FOLDER,
  MUSIC_FOLDER
}SCAN_FOLDER;

FRESULT f_scandir(SCAN_FOLDER);
void sd_init_handle(uint8_t res);
void f_mount_handle(uint8_t res);
void f_scandir_handle(uint8_t res);


uint8_t char_upper(uint8_t c);


//f_typetell返回的类型定义
//根据表FILE_TYPE_TBL获得.在exfuns.c里面定义
#define T_BIN		0X00	//bin文件
#define T_LRC		0X10	//lrc文件
#define T_NES		0X20	//nes文件
#define T_TEXT		0X30	//.txt文件
#define T_C			0X31	//.c文件
#define T_H			0X32    //.h文件
#define T_FLAC		0X4C	//flac文件
#define T_BMP		0X50	//bmp文件
#define T_JPG		0X51	//jpg文件
#define T_JPEG		0X52	//jpeg文件		 
#define T_GIF		0X53	//gif文件  

uint8_t f_typetell(char *fname);
/* USER CODE END Prototypes */
#ifdef __cplusplus
}
#endif
#endif /*__fatfs_H */

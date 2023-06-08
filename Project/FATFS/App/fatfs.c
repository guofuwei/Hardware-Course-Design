/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
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
  
  
#include "main.h"
/* USER CODE END Header */
#include "fatfs.h"

uint8_t retUSER;    /* Return value for USER */
char USERPath[4];   /* USER logical drive path */
FATFS USERFatFS;    /* File system object for USER logical drive */
FIL USERFile;       /* File object for USER */

/* USER CODE BEGIN Variables */
extern char NameList[NAMELIST_NUM][NAMELIST_MAX_LEN];
/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the USER driver ###########################*/
  retUSER = FATFS_LinkDriver(&USER_Driver, USERPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return 0;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */
FRESULT f_scandir(SCAN_FOLDER scan_folder)
{
//  uint8_t j=0;
//  for(j=0;j<NAMELIST_MAX_LEN;j++)
//  {
//    if(strcmp(NameList[j],"\0")==0)break;
//    strcpy(NameList[j],"\0");
//  }
  memset(NameList,'\0',sizeof(NameList));
  
  
	FRESULT res;
	DIR dir;
	int i=0;
	FILINFO fno;
  switch(scan_folder)
  {
    case ROOT_FOLDER:
      printf("f_scanf:0:/");
      res = f_opendir(&dir, "0:/");
      break;
    case TXT_FOLDER:
       printf("f_scanf:0:/txt");
      res = f_opendir(&dir, "0:/txt");
      break;
    case PIC_FOLDER:
       printf("f_scanf:0:/pic");
      res = f_opendir(&dir, "0:/pic");
      break;
    case MUSIC_FOLDER:
      printf("f_scanf:0:/music");
      res = f_opendir(&dir, "0:/music");
      break;
    default:
      LED_RED_ON;
      HAL_Delay(100);
      LED_RED_OFF;
      return FR_INVALID_PARAMETER;
  }
  printf("%d",res);
	if (res == FR_OK) 
  {
    for (;;) {
      res = f_readdir(&dir, &fno);                   // Read a directory item 
      if (res != FR_OK || fno.fname[0] == 0) break;  // Break on error or end of dir 
      strcpy(NameList[i++],fno.fname);
    }
    f_closedir(&dir);
  }
	return res;
}

void sd_init_handle(uint8_t res)
{
		switch(res){
//		case 0x01:
//			display_string("SD_MMC初始完成",16,WHITE,BLACK);
//			break;
//		case 0x06:
//			display_string("SD_VCH2初始完成",16,WHITE,BLACK);
//			break;
//		case 0x02:
//			display_string("SD_V1初始完成",16,WHITE,BLACK);
//			break;
//		case 0x04:
//			display_string("SD_V2初始完成",16,WHITE,BLACK);
//			break;
		case 0x00:
			display_string_center("SD初始化成功",16,GREEN,BLACK);
			break;
		default:
			display_string_center("SD初始化失败",16,RED,BLACK);
	}
}
void f_mount_handle(uint8_t res)
{
		switch(res)
	{
		case FR_OK:
			printf("f_mount_handle res:FR_OK");
			break;
		case FR_DISK_ERR:
			printf("f_mount_handle res:FR_DISK_ERR");
			break;
		case FR_INT_ERR:
			printf("f_mount_handle res:FR_INT_ERR");
			break;
		case FR_NOT_READY:
			printf("f_mount_handle res:FR_NOT_READY");
			break;
		case FR_NO_FILE:
			printf("f_mount_handle res:FR_NO_FILE");
			break;
		case FR_NO_PATH:
			printf("f_mount_handle res:FR_NO_PATH");
			break;
		case FR_INVALID_NAME:
			printf("f_mount_handle res:FR_INVALID_NAME");
			break;
		case FR_DENIED:
			printf("f_mount_handle res:FR_DENIED");
			break;
		case FR_EXIST:
			printf("f_mount_handle res:FR_EXIST");
			break;
		case FR_INVALID_OBJECT:
			printf("f_mount_handle res:FR_INVALID_OBJECT");
			break;
		case FR_WRITE_PROTECTED:
			printf("f_mount_handle res:FR_WRITE_PROTECTED");
			break;
		case FR_INVALID_DRIVE:
			printf("f_mount_handle res:FR_INVALID_DRIVE");
			break;
		case FR_NOT_ENABLED:
			printf("f_mount_handle res:FR_NOT_ENABLED");
			break;
		case FR_NO_FILESYSTEM:
			printf("f_mount_handle res:FR_NO_FILESYSTEM");
			break;
		case FR_MKFS_ABORTED:
			printf("f_mount_handle res:FR_MKFS_ABORTED");
			break;
		case FR_TIMEOUT:
			printf("f_mount_handle res:FR_TIMEOUT");
			break;
		case FR_LOCKED:
			printf("f_mount_handle res:FR_LOCKED");
			break;
		case FR_NOT_ENOUGH_CORE:
			printf("f_mount_handle res:FR_NOT_ENOUGH_CORE");
			break;
		case FR_TOO_MANY_OPEN_FILES:
			printf("f_mount_handle res:FR_TOO_MANY_OPEN_FILES");
			break;
		case FR_INVALID_PARAMETER:
			printf("f_mount_handle res:FR_INVALID_PARAMETER");
			break;
		default:
			printf("f_mount_handle res:OTHER_ERROR");
			break;
	}
}

void f_scandir_handle(uint8_t res)
{
	switch(res)
	{
		case FR_OK:
			display_string_center("文件目录扫描完成",16,WHITE,BLACK);
			break;
		case FR_DISK_ERR:
			display_string_center("FR_DISK_ERR",16,RED,BLACK);
			break;
		case FR_INT_ERR:
			display_string_center("FR_INT_ERR",16,RED,BLACK);
			break;
		case FR_NOT_READY:
			display_string_center("FR_NOT_READY",16,RED,BLACK);
			break;
		case FR_NO_FILE:
			display_string_center("FR_NO_FILE",16,RED,BLACK);
			break;
		case FR_NO_PATH:
			display_string_center("FR_NO_PATH",16,RED,BLACK);
		break;
		case FR_INVALID_NAME:
			display_string_center("FR_INVALID_NAME",16,RED,BLACK);
			break;
		case FR_DENIED:
			display_string_center("FR_DENIED",16,RED,BLACK);
			break;
		case FR_EXIST:
			display_string_center("FR_EXIST",16,RED,BLACK);
			break;
		case FR_INVALID_OBJECT:
			display_string_center("FR_INVALID_OBJECT",16,RED,BLACK);
			break;
		case FR_WRITE_PROTECTED:
			display_string_center("FR_WRITE_PROTECTED",16,RED,BLACK);
			break;
		case FR_INVALID_DRIVE:
			display_string_center("FR_INVALID_DRIVE",16,RED,BLACK);
			break;
		case FR_NOT_ENABLED:
			display_string_center("FR_NOT_ENABLED",16,RED,BLACK);
			break;
		case FR_NO_FILESYSTEM:
			display_string_center("FR_NO_FILESYSTEM",16,RED,BLACK);
			break;
		case FR_MKFS_ABORTED:
			display_string_center("FR_MKFS_ABORTED",16,RED,BLACK);
			break;
		case FR_TIMEOUT:
			display_string_center("FR_TIMEOUT",16,RED,BLACK);
			break;
		case FR_LOCKED:
			display_string_center("FR_LOCKED",16,RED,BLACK);
			break;
		case FR_NOT_ENOUGH_CORE:
			display_string_center("FR_NOT_ENOUGH_CORE",16,RED,BLACK);
			break;
		case FR_TOO_MANY_OPEN_FILES:
			display_string_center("FR_TOO_MANY_OPEN_FILES",16,RED,BLACK);
			break;
		case FR_INVALID_PARAMETER:
			display_string_center("FR_INVALID_PARAMETER",16,RED,BLACK);
			break;
		default:
			display_string_center("OTHER_ERROR",16,RED,BLACK);
			break;
	}
}
/* USER CODE END Application */

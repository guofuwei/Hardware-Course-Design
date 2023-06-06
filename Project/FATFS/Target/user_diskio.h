/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    user_diskio.h
 * @brief   This file contains the common defines and functions prototypes for
 *          the user_diskio driver.
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
#ifndef __USER_DISKIO_H
#define __USER_DISKIO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* USER CODE BEGIN 0 */
/**
 ****************************************************************************************************
 * @file        diskio.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-28
 * @brief       FATFS底层(diskio) 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 Mini STM32F103 开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20200428
 * 第一次发布
 *
 ****************************************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "ff.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

#define ISDIO_READ 55  /* Read data form SD iSDIO register */
#define ISDIO_WRITE 56 /* Write data to SD iSDIO register */
#define ISDIO_MRITE 57 /* Masked write data to SD iSDIO register */

/* Exported functions ------------------------------------------------------- */
extern Diskio_drvTypeDef USER_Driver;

/* USER CODE END 0 */

#ifdef __cplusplus
}
#endif

#endif /* __USER_DISKIO_H */

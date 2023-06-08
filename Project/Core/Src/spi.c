/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
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
#include "spi.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

SPI_HandleTypeDef hspi1;

/* SPI1 init function */
void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspInit 0 */

  /* USER CODE END SPI1_MspInit 0 */
    /* SPI1 clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI1_MspInit 1 */

  /* USER CODE END SPI1_MspInit 1 */
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspDeInit 0 */

  /* USER CODE END SPI1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI1_CLK_DISABLE();

    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

  /* USER CODE BEGIN SPI1_MspDeInit 1 */

  /* USER CODE END SPI1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */


/***************************************************************************************************************************/
/**
 * @brief       SPI1��ʼ������
 *   @note      ����ģʽ,8λ����,��ֹӲ��Ƭѡ
 * @param       ��
 * @retval      ��
 */
void spi1_init(void)
{
    SPI1_SPI_CLK_ENABLE();                                             /* SPI1      ʱ��ʹ�� */

    hspi1.Instance = SPI1_SPI;                                /* SPI1 */
    hspi1.Init.Mode = SPI_MODE_MASTER;                        /* ����SPI����ģʽ������Ϊ��ģʽ */
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;              /* ����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ */
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;                  /* ����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ */
    hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;               /* ����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ */
    hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;                    /* ����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ����� */
    hspi1.Init.NSS = SPI_NSS_SOFT;                            /* NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ���� */
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256; /* ���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256 */
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;                   /* ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ */
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;                   /* �ر�TIģʽ */
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;   /* �ر�Ӳ��CRCУ�� */
    hspi1.Init.CRCPolynomial = 7;                             /* CRCֵ����Ķ���ʽ */
    HAL_SPI_Init(&hspi1);                                     /* ��ʼ�� */
    
    __HAL_SPI_ENABLE(&hspi1);                                 /* ʹ��SPI1 */
    
    spi1_read_write_byte(0Xff);                                        /* ��������, ʵ���Ͼ��ǲ���8��ʱ������, �ﵽ���DR������, �Ǳ��� */
}

/**
 * @brief       SPI1�ٶ����ú���
 *   @note      SPI1ʱ��ѡ������APB2, ��PCLK2, Ϊ72Mhz
 *              SPI�ٶ� = PCLK1 / 2^(speed + 1)
 * @param       speed   : SPI1ʱ�ӷ�Ƶϵ��
 * @retval      ��
 */
void spi1_set_speed(uint8_t speed)
{
    speed &= 0X07;                                  /* ���Ʒ�Χ */
    __HAL_SPI_DISABLE(&hspi1);             /* �ر�SPI */
    hspi1.Instance->CR1 &= 0XFFC7;         /* λ3-5���㣬�������ò����� */
    hspi1.Instance->CR1 |= speed << 3;     /* ����SPI�ٶ� */
    __HAL_SPI_ENABLE(&hspi1);              /* ʹ��SPI */
}

/**
 * @brief       SPI1��дһ���ֽ�����
 * @param       txdata  : Ҫ���͵�����(1�ֽ�)
 * @retval      ���յ�������(1�ֽ�)
 */
uint8_t spi1_read_write_byte(uint8_t txdata)
{
    while ((hspi1.Instance->SR & 1 << 1) == 0);   /* �ȴ��������� */
    hspi1.Instance->DR = txdata;                  /* ����һ��byte */
    while ((hspi1.Instance->SR & 1 << 0) == 0);   /* �ȴ�������һ��byte */
    return hspi1.Instance->DR;                    /* �����յ������� */
}


/* USER CODE END 1 */

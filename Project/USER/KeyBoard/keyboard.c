#include "keyboard.h"
#include <stdio.h>


/***
 *��������KEY_SCAN
 *��  �ܣ�����ɨ��
 *����ֵ��1~4����Ӧ1~4�а���λ��
 */
uint8_t KEY_SCAN(void)
{
	if(HAL_GPIO_ReadPin(KeyBoard_Col1_GPIO_Port,KeyBoard_Col1_Pin)==GPIO_PIN_RESET)
	{
    	HAL_Delay(20);
			if(HAL_GPIO_ReadPin(KeyBoard_Col1_GPIO_Port,KeyBoard_Col1_Pin)==GPIO_PIN_RESET)
			{
        while(HAL_GPIO_ReadPin(KeyBoard_Col1_GPIO_Port,KeyBoard_Col1_Pin)==GPIO_PIN_RESET);
				return 1;
			}
	}
	else if(HAL_GPIO_ReadPin(KeyBoard_Col2_GPIO_Port,KeyBoard_Col2_Pin)==GPIO_PIN_RESET)
	{
    HAL_Delay(20);
    if(HAL_GPIO_ReadPin(KeyBoard_Col2_GPIO_Port,KeyBoard_Col2_Pin)==GPIO_PIN_RESET)
    {
      while(HAL_GPIO_ReadPin(KeyBoard_Col2_GPIO_Port,KeyBoard_Col2_Pin)==GPIO_PIN_RESET);
      return 2;
    }
	}
  else if(HAL_GPIO_ReadPin(KeyBoard_Col3_GPIO_Port,KeyBoard_Col3_Pin)==GPIO_PIN_RESET)
  {
    HAL_Delay(20);
    if(HAL_GPIO_ReadPin(KeyBoard_Col3_GPIO_Port,KeyBoard_Col3_Pin)==GPIO_PIN_RESET)
    {
      while(HAL_GPIO_ReadPin(KeyBoard_Col3_GPIO_Port,KeyBoard_Col3_Pin)==GPIO_PIN_RESET);
      return 3;
    }
  }
  else if(HAL_GPIO_ReadPin(KeyBoard_Col4_GPIO_Port,KeyBoard_Col4_Pin)==GPIO_PIN_RESET)
  {
    HAL_Delay(20);
    if(HAL_GPIO_ReadPin(KeyBoard_Col4_GPIO_Port,KeyBoard_Col4_Pin)==GPIO_PIN_RESET)
    {
      while(HAL_GPIO_ReadPin(KeyBoard_Col4_GPIO_Port,KeyBoard_Col4_Pin)==GPIO_PIN_RESET);
      return 4;
    }
  }
  return 0;
}


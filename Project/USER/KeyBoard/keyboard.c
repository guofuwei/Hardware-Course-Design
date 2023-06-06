#include "keyboard.h"
#include <stdio.h>

uint8_t Key_row[1]={0xff};   //保存按键行扫描情况的状态数组





/***
 *函数名：KEY_ROW_SCAN
 *功  能：按键行扫描
 *返回值：1~2，对应1~2行按键位置
 */
char KEY_ROW_SCAN(void)
{
  Key_row[0]=Key_row[0]|0x0f;
	//读出行扫描状态
	if(HAL_GPIO_ReadPin(KeyBoard_Row1_GPIO_Port,KeyBoard_Row1_Pin)==GPIO_PIN_RESET)
	{
		Key_row[0]=Key_row[0]&0x7;
//    printf("row1");
	}
	if(HAL_GPIO_ReadPin(KeyBoard_Row2_GPIO_Port,KeyBoard_Row2_Pin)==GPIO_PIN_RESET)
	{
		Key_row[0]=Key_row[0]&0xb;
//    printf("row2");
	}
	
	if(Key_row[0]!= 0x0f)         //行扫描有变化，判断该列有按键按下
	{
//		HAL_Delay(10);                    //消抖
		if(Key_row[0] != 0x0f)
			{   
//				printf("Key_Row_DATA = 0x%x\r\n",Key_row[0]);
				switch(Key_row[0])
				{
					case 0x07:         //0111 判断为该列第1行的按键按下
							return 1;
					case 0x0b:         //1011 判断为该列第2行的按键按下
							return 2;
					case 0x0d:         //1101 判断为该列第3行的按键按下
							return 3;
					case 0x0e:         //1110 判断为该列第4行的按键按下
							return 4;
					default :
							return 0;
				}
			}
		else return 0;
	}
	else return 0;
}

/***
 *函数名：KEY_SCAN
 *功  能：4*4按键扫描
 *返回值：0~16，对应16个按键
 */
char KEY_SCAN(void)
{    
    char Key_Num=0;       //1-16对应的按键数
    char key_row_num=0;        //行扫描结果记录
    Key_row[0]=0xff;
    
    KEY_CLO1_OUT_HIGH;        
    if( (key_row_num=KEY_ROW_SCAN()) != 0 )
    { 
//        printf("Key_Clo_1\r\n");
        while(KEY_ROW_SCAN() != 0);  //消抖
        Key_Num = 0 + key_row_num;
        
    }
    KEY_CLO1_OUT_LOW;
    
    KEY_CLO2_OUT_HIGH;        
    if( (key_row_num=KEY_ROW_SCAN()) != 0 )
    { 
        
        while(KEY_ROW_SCAN() != 0);
//      printf("Key_Clo_2\r\n");
        Key_Num = 4 + key_row_num;
        
    }
    KEY_CLO2_OUT_LOW;
    
    KEY_CLO3_OUT_HIGH;    
    if( (key_row_num=KEY_ROW_SCAN()) != 0 )
    { 
        while(KEY_ROW_SCAN() != 0);
        Key_Num = 8 + key_row_num;
//        printf("Key_Clo_3\r\n");
    }
    KEY_CLO3_OUT_LOW;
    
    KEY_CLO4_OUT_HIGH;    
    if( (key_row_num=KEY_ROW_SCAN()) != 0 )
    {
        while(KEY_ROW_SCAN() != 0);
        Key_Num = 12 + key_row_num;
//        printf("Key_Clo_4\r\n");
    }
    KEY_CLO3_OUT_LOW;
    
    return Key_Num;	
}

/***
 *函数名：KEY_ROW_SCAN
 *功  能：执行按下按键后的操作
 *返回值：无
 */
void HW_KEY_FUNCTION(void)
{
    char key_confirm;
    key_confirm = KEY_SCAN();
    if( 0 < key_confirm  && key_confirm < 17 )
    {
        printf("Key_NUM = %d \r\n",key_confirm); //按下1-16个按键的操作
        printf("= = = = = = = = = = = \r\n");
    }
}

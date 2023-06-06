#include "keyboard.h"
#include <stdio.h>

uint8_t Key_row[1]={0xff};   //���水����ɨ�������״̬����





/***
 *��������KEY_ROW_SCAN
 *��  �ܣ�������ɨ��
 *����ֵ��1~2����Ӧ1~2�а���λ��
 */
char KEY_ROW_SCAN(void)
{
  Key_row[0]=Key_row[0]|0x0f;
	//������ɨ��״̬
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
	
	if(Key_row[0]!= 0x0f)         //��ɨ���б仯���жϸ����а�������
	{
//		HAL_Delay(10);                    //����
		if(Key_row[0] != 0x0f)
			{   
//				printf("Key_Row_DATA = 0x%x\r\n",Key_row[0]);
				switch(Key_row[0])
				{
					case 0x07:         //0111 �ж�Ϊ���е�1�еİ�������
							return 1;
					case 0x0b:         //1011 �ж�Ϊ���е�2�еİ�������
							return 2;
					case 0x0d:         //1101 �ж�Ϊ���е�3�еİ�������
							return 3;
					case 0x0e:         //1110 �ж�Ϊ���е�4�еİ�������
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
 *��������KEY_SCAN
 *��  �ܣ�4*4����ɨ��
 *����ֵ��0~16����Ӧ16������
 */
char KEY_SCAN(void)
{    
    char Key_Num=0;       //1-16��Ӧ�İ�����
    char key_row_num=0;        //��ɨ������¼
    Key_row[0]=0xff;
    
    KEY_CLO1_OUT_HIGH;        
    if( (key_row_num=KEY_ROW_SCAN()) != 0 )
    { 
//        printf("Key_Clo_1\r\n");
        while(KEY_ROW_SCAN() != 0);  //����
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
 *��������KEY_ROW_SCAN
 *��  �ܣ�ִ�а��°�����Ĳ���
 *����ֵ����
 */
void HW_KEY_FUNCTION(void)
{
    char key_confirm;
    key_confirm = KEY_SCAN();
    if( 0 < key_confirm  && key_confirm < 17 )
    {
        printf("Key_NUM = %d \r\n",key_confirm); //����1-16�������Ĳ���
        printf("= = = = = = = = = = = \r\n");
    }
}

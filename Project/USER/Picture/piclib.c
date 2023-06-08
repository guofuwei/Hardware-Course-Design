#include "piclib.h"
//#include "lcd.h"
#include "lcd_st7735.h"
#include "fatfs.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK STM32F407������
//ͼƬ���� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/15
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//********************************************************************************
//����˵��
//��
//////////////////////////////////////////////////////////////////////////////////

extern struct _LCD              // �ṹ�壺���ڱ�����Ϣ��״̬
{     
  uint8_t  InitOK;
  uint16_t width;        // LCD ����
  uint16_t height;       // LCD �߶�
  uint16_t bColor;       // ����ɫ
}xLCD;

_pic_info picinfo;	 	//ͼƬ��Ϣ
_pic_phy pic_phy;		//ͼƬ��ʾ�����ӿ�	
//////////////////////////////////////////////////////////////////////////
//lcd.hû���ṩ�����ߺ���,��Ҫ�Լ�ʵ��
void piclib_draw_hline(uint16_t x0,uint16_t y0,uint16_t len,uint16_t color)
{
	if((len==0)||(x0>xLCD.width)||(y0>xLCD.height))return;
	LCD_Fill(x0,y0,x0+len-1,y0,color);	
}
//�����ɫ
//x,y:��ʼ����
//width��height�����Ⱥ͸߶ȡ�
//*color����ɫ����
void piclib_fill_color(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t color)
{  
	LCD_Fill(x,y,x+width-1,y+height-1,color);	
}
//////////////////////////////////////////////////////////////////////////
//��ͼ��ʼ��,�ڻ�ͼ֮ǰ,�����ȵ��ô˺���
//ָ������/����
void piclib_init(void)
{
//	pic_phy.read_point=LCD_ReadPoint;  		//���㺯��ʵ��
	pic_phy.draw_point=drawPoint;	//���㺯��ʵ��
	pic_phy.fill=LCD_Fill;					//��亯��ʵ��
	pic_phy.draw_hline=piclib_draw_hline;  	//���ߺ���ʵ��
	pic_phy.fillcolor=piclib_fill_color;  	//��ɫ��亯��ʵ�� 

	picinfo.lcdwidth=xLCD.width;	//�õ�LCD�Ŀ�������
	picinfo.lcdheight=xLCD.height;//�õ�LCD�ĸ߶�����

	picinfo.ImgWidth=0;	//��ʼ������Ϊ0
	picinfo.ImgHeight=0;//��ʼ���߶�Ϊ0
	picinfo.Div_Fac=0;	//��ʼ������ϵ��Ϊ0
	picinfo.S_Height=0;	//��ʼ���趨�ĸ߶�Ϊ0
	picinfo.S_Width=0;	//��ʼ���趨�Ŀ���Ϊ0
	picinfo.S_XOFF=0;	//��ʼ��x���ƫ����Ϊ0
	picinfo.S_YOFF=0;	//��ʼ��y���ƫ����Ϊ0
	picinfo.staticx=0;	//��ʼ����ǰ��ʾ����x����Ϊ0
	picinfo.staticy=0;	//��ʼ����ǰ��ʾ����y����Ϊ0
}
//����ALPHA BLENDING�㷨.
//src:Դ��ɫ
//dst:Ŀ����ɫ
//alpha:͸���̶�(0~32)
//����ֵ:��Ϻ����ɫ.
uint16_t piclib_alpha_blend(uint16_t src,uint16_t dst,uint8_t alpha)
{
	uint32_t src2;
	uint32_t dst2;	 
	//Convert to 32bit |-----GGGGGG-----RRRRR------BBBBB|
	src2=((src<<16)|src)&0x07E0F81F;
	dst2=((dst<<16)|dst)&0x07E0F81F;   
	//Perform blending R:G:B with alpha in range 0..32
	//Note that the reason that alpha may not exceed 32 is that there are only
	//5bits of space between each R:G:B value, any higher value will overflow
	//into the next component and deliver ugly result.
	dst2=((((dst2-src2)*alpha)>>5)+src2)&0x07E0F81F;
	return (dst2>>16)|dst2;  
}
//��ʼ�����ܻ���
//�ڲ�����
void ai_draw_init(void)
{
	float temp,temp1;	   
	temp=(float)picinfo.S_Width/picinfo.ImgWidth;
	temp1=(float)picinfo.S_Height/picinfo.ImgHeight;						 
	if(temp<temp1)temp1=temp;//ȡ��С���Ǹ�	 
	if(temp1>1)temp1=1;	  
	//ʹͼƬ��������������м�
	picinfo.S_XOFF+=(picinfo.S_Width-temp1*picinfo.ImgWidth)/2;
	picinfo.S_YOFF+=(picinfo.S_Height-temp1*picinfo.ImgHeight)/2;
	temp1*=8192;//����8192��	 
	picinfo.Div_Fac=temp1;
	picinfo.staticx=0xffff;
	picinfo.staticy=0xffff;//�ŵ�һ�������ܵ�ֵ����			 										    
}   
//�ж���������Ƿ������ʾ
//(x,y) :����ԭʼ����
//chg   :���ܱ���. 
//����ֵ:0,����Ҫ��ʾ.1,��Ҫ��ʾ
uint8_t is_element_ok(uint16_t x,uint16_t y,uint8_t chg)
{				  
	if(x!=picinfo.staticx||y!=picinfo.staticy)
	{
		if(chg==1)
		{
			picinfo.staticx=x;
			picinfo.staticy=y;
		} 
		return 1;
	}else return 0;
}
//���ܻ�ͼ
//FileName:Ҫ��ʾ��ͼƬ�ļ�  BMP/JPG/JPEG/GIF
//x,y,width,height:���꼰��ʾ����ߴ�
//fast:ʹ��jpeg/jpgСͼƬ(ͼƬ�ߴ�С�ڵ���Һ���ֱ���)���ٽ���,0,��ʹ��;1,ʹ��.
//ͼƬ�ڿ�ʼ�ͽ���������㷶Χ����ʾ
uint8_t ai_load_picfile(char *filename,uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t fast)
{	
  printf("1111");
	uint8_t	res;//����ֵ
	uint8_t temp;	
	if((x+width)>picinfo.lcdwidth)return PIC_WINDOW_ERR;		//x���곬��Χ��.
	if((y+height)>picinfo.lcdheight)return PIC_WINDOW_ERR;		//y���곬��Χ��.  
	//�õ���ʾ�����С	  	 
//	if(width==0||height==0)return PIC_WINDOW_ERR;	//�����趨����
	picinfo.S_Height=height;
	picinfo.S_Width=width;
  
  
  printf("2222");
	//��ʾ������Ч
	if(picinfo.S_Height==0||picinfo.S_Width==0)
	{
		picinfo.S_Height=xLCD.height;
		picinfo.S_Width=xLCD.width;
		return FALSE;   
	}
  
  printf("3333");
	if(pic_phy.fillcolor==NULL)fast=0;//��ɫ��亯��δʵ��,���ܿ�����ʾ
	//��ʾ�Ŀ�ʼ�����
	picinfo.S_YOFF=y;
	picinfo.S_XOFF=x;
  
  printf("4444");
	//�ļ�������		 
  
  

	temp=f_typetell(filename);	//�õ��ļ�������
  
  printf("5555");
	switch(temp)
	{											  
		case T_BMP:
      printf("bmpͼƬ\n");
			res=stdbmp_decode(filename); 				//����bmp	  	  
			break;
		case T_JPG:
		case T_JPEG:
      printf("jpgͼƬ\n");
//			res=jpg_decode(filename,fast);				//����JPG/JPEG	  	  
			break;
		case T_GIF:
      printf("gifͼƬ\n");
//			res=gif_decode(filename,x,y,width,height);	//����gif  	  
			break;
		default:
      printf("error ͼƬ\n");
	 		res=PIC_FORMAT_ERR;  						//��ͼƬ��ʽ!!!  
			break;
	}  											   
	return res;
}
//��̬�����ڴ�
void *pic_memalloc (uint32_t size)			
{
	return (void*)mymalloc(SRAMIN,size);
}
//�ͷ��ڴ�
void pic_memfree (void* mf)		 
{
	myfree(SRAMIN,mf);
}























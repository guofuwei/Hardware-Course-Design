#ifndef __PICLIB_H
#define __PICLIB_H	  		  
#include "sys.h" 
#include "lcd_st7735.h"
#include "malloc.h"
#include "ff.h"
//#include "exfuns.h"
#include "bmp.h"
#include "tjpgd.h"
#include "gif.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//图片解码 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/15
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//********************************************************************************
//升级说明
//无
//////////////////////////////////////////////////////////////////////////////////

#define PIC_FORMAT_ERR		0x27	//格式错误
#define PIC_SIZE_ERR		0x28	//图片尺寸错误
#define PIC_WINDOW_ERR		0x29	//窗口设定错误
#define PIC_MEM_ERR			0x11	//内存错误
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef TRUE
#define TRUE    1
#endif
#ifndef FALSE
#define FALSE   0
#endif 

//图片显示物理层接口  
//在移植的时候,必须由用户自己实现这几个函数
typedef struct 
{
//	uint16_t(*read_point)(uint16_t,uint16_t);				//uint16_t read_point(uint16_t x,uint16_t y)						读点函数
	void(*draw_point)(uint16_t,uint16_t,uint16_t);			//void draw_point(uint16_t x,uint16_t y,uint16_t color)		    画点函数
 	void(*fill)(uint16_t,uint16_t,uint16_t,uint16_t,uint16_t);		///void fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color) 单色填充函数 	 
 	void(*draw_hline)(uint16_t,uint16_t,uint16_t,uint16_t);		//void draw_hline(uint16_t x0,uint16_t y0,uint16_t len,uint16_t color)  画水平线函数	 
 	void(*fillcolor)(uint16_t,uint16_t,uint16_t,uint16_t,uint16_t);	//void piclib_fill_color(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t color) 颜色填充
}_pic_phy; 

extern _pic_phy pic_phy;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//图像信息
typedef struct
{		
	uint16_t lcdwidth;	//LCD的宽度
	uint16_t lcdheight;	//LCD的高度
	uint32_t ImgWidth; 	//图像的实际宽度和高度
	uint32_t ImgHeight;

	uint32_t Div_Fac;  	//缩放系数 (扩大了8192倍的)
	
	uint32_t S_Height; 	//设定的高度和宽度
	uint32_t S_Width;
	
	uint32_t	S_XOFF;	  	//x轴和y轴的偏移量
	uint32_t S_YOFF;

	uint32_t staticx; 	//当前显示到的ｘｙ坐标
	uint32_t staticy;																 	
}_pic_info;
extern _pic_info picinfo;//图像信息
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void piclib_init(void);								//初始化画图
uint16_t piclib_alpha_blend(uint16_t src,uint16_t dst,uint8_t alpha);	//alphablend处理
void ai_draw_init(void);							//初始化智能画图
uint8_t is_element_ok(uint16_t x,uint16_t y,uint8_t chg);				//判断像素是否有效
uint8_t ai_load_picfile(const uint8_t *filename,uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t fast);//智能画图
void *pic_memalloc (uint32_t size);	//pic申请内存
void pic_memfree (void* mf);	//pic释放内存
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif































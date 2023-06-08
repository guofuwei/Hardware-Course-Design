/***********************************************************************************************************************************
    ** MCUDEV 1.8寸彩色显示屏模块引脚说明(ST7735)
    ** GND     电源地
    ** VCC     3.3V。不要接5V, 模块不带LDO，5V很易烧屏
    ** SC      SPI的SCK引脚  
    ** DI      SPI的MOSI引脚 
    ** RST     复位引脚，最好和单片机的复位引脚相接
    ** RS      命令、数据选择引脚
    ** CS      SPI的CS片选引脚
    ** BLK     控制背光LED， 高电平亮。引脚已串1K电阻，且已10K上拉。
    ** 测试硬件   Nucleo-64 STM32F401RE板
    ** 备注    参考了官方公开例程Keil_v5\ARM\PACK\Keil\STM32F4xx_DFP
    **            \2.13.0\Drivers\BSP\Components\st7735\st7735.c
		**  128*160
************************************************************************************************************************************/
#include "lcd_st7735.h"
#include "spi.h"
#include "gpio.h"
#include "stm32f1xx_hal.h"
#include "font.h"

/*****************************************************************************
 ** 本地变量
 *****************************************************************************/
struct _LCD              // 结构体：用于保存信息、状态
{     
  uint8_t  InitOK;
  uint16_t width;        // LCD 宽度
  uint16_t height;       // LCD 高度
  uint16_t bColor;       // 背景色
}xLCD;

/*****************************************************************************
 ** 本地函数，HAL_SPI再封装
 ****************************************************************************/
/**
 * @brief    SPI 发送字节函数（封装HAL库函数，集SPI口唯一定义以便修改）
 * @param    TxData 要发送的字节数据指针
 * @return   0:写入成功,其他:写入失败
 * @备注     如果软件模拟SPI，主要是修改此函数
 */
uint8_t SPI_WriteByte(uint8_t TxData)
{
  return HAL_SPI_Transmit(&hspiX,&TxData,1,500);
}

/*****************************************************************************
 ** 调用HAL_SPI的LCD控制芯片操作基本函数（大多数LCD控制芯片的操作思想类似）
 ****************************************************************************/
/**
 * @brief   写命令到LCD（控制芯片）
 * @param   cmd —— 需要发送的命令=其实就是控制芯片要写数据的寄存器地址
 * @return  none
 */
static void sendOrder(uint8_t cmd)
{
    LCD_CS_EN;             // 片选SPI设备LCD，开始通信
    LCD_DC_COM;            // DAT: 数据， COM: 命令=寄存器地址值
    SPI_WriteByte(cmd);
    LCD_CS_DIS;            // SPI设备片选取消，结束通信
}

/**
 * @brief   写1字节数据到LCD
 * @param   dat —— 需要发送的数据=就是向控制芯片已指定的寄存器地址写入的数据
 * @return  none
 */
static void sendByte(uint8_t dat)
{
    LCD_CS_EN;  
    LCD_DC_DAT;  
    SPI_WriteByte(dat);
    LCD_CS_DIS; 
}

/**
 * @brief   写2字节数据到LCD
 * @param   Data —— 需要发送的2字节数据，主要是颜色数据
 * @return  none
 */
static void sendShort(uint16_t Data)
{
    LCD_CS_EN; 
    LCD_DC_DAT;  
    SPI_WriteByte(Data>>8);  //用HAL_SPI库发送旧颜色数据，高8位先发
	  SPI_WriteByte(Data); 
    LCD_CS_DIS;  
}

/*****************************************************************************
 ** 基于上述基本函数的LCD硬件操作函数--对照芯片的Datasheet才好理解，已与HAL库隔离
 ****************************************************************************/

/*****************************************************************************
*函  数：LCD_Init
*功  能：初始化lcd
*参  数：为了简化理解、便于移植，参数(屏幕像素、显示方向)在h文件里进行修改
*返回值：无 
*备  注：以下适用于1.8寸TFT，控制芯片ST7735
*****************************************************************************/
void LCD_Init(short BG)  
{    
    xLCD.InitOK = 0; 
    xLCD.bColor  = BG;
  // 显示方向像素匹配
    if( (LCD_DIR ==1) || (LCD_DIR ==3))
    {
        xLCD.width  = LCD_WIDTH ;       // 屏宽度像素，超过此值驱动芯片会自动换行，注意：如果屏幕右边有花屏，就加大这个值
        xLCD.height = LCD_HIGH ;        // 屏高度像素， 注意：如果屏幕下面有花屏，就加大这个值
    }
    else
    {
        xLCD.width  = LCD_HIGH;
        xLCD.height = LCD_WIDTH;
    }      

  // 屏幕复位	(复位后必须跟着Init参数，否则白屏）
  LCD_RST_EN;	             // SPI接口复位
  HAL_Delay(20);           // delay 20 ms 
  LCD_RST_DIS;             // SPI接口复位恢复 
  HAL_Delay(20);
  
  //-------------- 以下代码有更换 ---------------------------------
	sendOrder(0x11);         // 退出睡眠模式
	HAL_Delay(5);            
		
	//ST7735R Frame Rate
	sendOrder(0xB1); 
	sendByte(0x01); 
	sendByte(0x2C); 
	sendByte(0x2D); 

	sendOrder(0xB2); 
	sendByte(0x01); 
	sendByte(0x2C); 
	sendByte(0x2D); 

	sendOrder(0xB3); 
	sendByte(0x01); 
	sendByte(0x2C); 
	sendByte(0x2D); 
	sendByte(0x01); 
	sendByte(0x2C); 
	sendByte(0x2D); 
	
	sendOrder(0xB4); // Column inversion 
	sendByte(0x07); 
	
	//ST7735R Power Sequence
	sendOrder(0xC0); 
	sendByte(0xA2); 
	sendByte(0x02); 
	sendByte(0x84); 
	sendOrder(0xC1); 
	sendByte(0xC5); 

	sendOrder(0xC2); 
	sendByte(0x0A); 
	sendByte(0x00); 

	sendOrder(0xC3); 
	sendByte(0x8A); 
	sendByte(0x2A); 
	sendOrder(0xC4); 
	sendByte(0x8A); 
	sendByte(0xEE); 
	
	sendOrder(0xC5);     // VCOM 
	sendByte(0x0E); 
	
	sendOrder(0x36);                   // MX, MY, RGB mode 
	//sendByte(0xC0);                  // 重要：显示方向控制，C0/00/A0/60,  C8/08/A8/68
	if(LCD_DIR == 1)  sendByte(0xC0);  // 重要：显示方向控制，C0/00/A0/60,  C8/08/A8/68
	if(LCD_DIR == 2)  sendByte(0x00);      
    if(LCD_DIR == 3)  sendByte(0xA0);
    if(LCD_DIR == 4)  sendByte(0x60);
	
	//ST7735R Gamma Sequence
	sendOrder(0xe0); 
	sendByte(0x0f); 
	sendByte(0x1a); 
	sendByte(0x0f); 
	sendByte(0x18); 
	sendByte(0x2f); 
	sendByte(0x28); 
	sendByte(0x20); 
	sendByte(0x22); 
	sendByte(0x1f); 
	sendByte(0x1b); 
	sendByte(0x23); 
	sendByte(0x37); 
	sendByte(0x00); 	
	sendByte(0x07); 
	sendByte(0x02); 
	sendByte(0x10); 

	sendOrder(0xe1); 
	sendByte(0x0f); 
	sendByte(0x1b); 
	sendByte(0x0f); 
	sendByte(0x17); 
	sendByte(0x33); 
	sendByte(0x2c); 
	sendByte(0x29); 
	sendByte(0x2e); 
	sendByte(0x30); 
	sendByte(0x30); 
	sendByte(0x39); 
	sendByte(0x3f); 
	sendByte(0x00); 
	sendByte(0x07); 
	sendByte(0x03); 
	sendByte(0x10);  
	
	sendOrder(0x2a);
	sendByte(0x00);
	sendByte(0x00);
	sendByte(0x00);
	sendByte(0x7f);

	sendOrder(0x2b);
	sendByte(0x00);
	sendByte(0x00);
	sendByte(0x00);
	sendByte(0x9f);
	
	sendOrder(0xF0); // Enable test command  
	sendByte(0x01); 
	sendOrder(0xF6); // Disable ram power save mode 
	sendByte(0x00); 
	
	sendOrder(0x3A); // 65k mode 
	sendByte(0x05); 	
	
	sendOrder(0x29); // Display on	

  //-------------------------  代码更换end ----------------------------------
	
  LCD_Fill(1, 1, xLCD.width , xLCD.height , BG );  //调用下方定义的显示操作函数
  LCD_BKL_ON;      // 亮屏
  xLCD.InitOK = 1; 	
}  


/*****************************************************************
 * 函  数：setCursor
 * 功  能：设置显示区域，在此区域写点数据自动换行
 * 参  数：xStart横坐标起点, yStart纵坐标起点
 *         xEnd横坐标线束点，yEnd纵坐标线束点
 * 返回值：无
******************************************************************/
void setCursor(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd)
{		
	sendOrder(0x2A);
	sendByte (0x00);   // 起始位置x高位，因为1.8寸屏是128*160, 不大于255, 直接写0省事
	sendByte (xStart); // 起始位置x低位，值传递时自动舍弃了高8位，也省得运算了
	sendByte (0x00);   // 起始位置y高位
	sendByte (xEnd);   // 起始位置x位位

	sendOrder(0x2B);
	sendByte (0x00);
	sendByte (yStart);
	sendByte (0x00);
	sendByte (yEnd);
	
	sendOrder(0x2c);  // 发送写数据命令
}

/*****************************************************************************
 ** 基于上述基本硬件函数的LCD显示处理函数，与HAL库及硬件都已隔离，移植时基本不必改动
 ****************************************************************************/

/*****************************************************************
 * 函  数：drawPoint
 * 功  能：画一个点
 * 参  数：x坐标，y坐标, 16位颜色值
 * 返回值：无
******************************************************************/
void drawPoint(uint16_t x, uint16_t y, uint16_t color)
{
    setCursor(x, y, x, y);      //设置光标位置 
    sendShort(color);    
}

/******************************************************************
 * 函数名： LCD_Fill
 * 功  能： 在指定区域内填充单个颜色
 *          屏幕坐标从1开始；屏幕左右和下方实际上有多余行列
 * 参  数： uint16_t xStart, yStart  左上角起始坐标
 *          uint16_t xEnd,   yEnd    右下角起始坐标
 *          uint16_t Color           填充的颜色
 *****************************************************************/
void LCD_Fill(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t color)
{      
  LCD_SPI_Init;                                       // 防止SPI参数被其它设备修改了
  uint32_t pixel = (xEnd-xStart+1) * (yEnd-yStart+1); // 填充的像素数量

  setCursor(xStart, yStart, xEnd, yEnd);              // 设定填充范围
  while(pixel-->0)                                    // 发送颜色值
     sendShort(color);                            
}  

/******************************************************************
 * 函数名： LCD_Line
 * 功  能： 画线
 * 参  数： uint16_t x1,y1  起点坐标
 *          uint16_t x2,y2  终点坐标 
 *          uint16_t Color  颜色
 *****************************************************************/
void LCD_Line(uint16_t sX, uint16_t sY, uint16_t eX, uint16_t eY, uint16_t color)
{
	LCD_SPI_Init;                                       // 防止SPI参数被其它设备修改了
	
	uint16_t t; 
	int xerr=0, yerr=0, delta_x, delta_y, distance; 
	int incx,incy,uRow,uCol; 
	delta_x = eX - sX; //计算坐标增量 
	delta_y = eY - sY; 
	uRow = sX; 
	uCol = sY; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		drawPoint(uRow, uCol, color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}

//在指定位置画一个指定大小的圆
//(x,y):中心点；r:半径
void LCD_Circle(uint16_t x0,uint16_t y0,uint8_t r, uint16_t color)
{
  LCD_SPI_Init;                              // 防止SPI参数被其它设备修改了
  int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);                               //判断下个点位置的标志
	while(a<=b)
	{
		drawPoint(x0+a,y0-b, color);             //5
 		drawPoint(x0+b,y0-a, color);             //0           
		drawPoint(x0+b,y0+a, color);             //4               
		drawPoint(x0+a,y0+b, color);             //6 
		drawPoint(x0-a,y0+b, color);             //1       
 		drawPoint(x0-b,y0+a, color);             
		drawPoint(x0-a,y0-b, color);             //2             
  	drawPoint(x0-b,y0-a, color);             //7   
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		}    
	}
} 

/******************************************************************
 * 函数名： LCD_Image
 * 功  能： 在指定区域内填充指定图片数据
 *          图片数据在font.h文件中.只适合少量图片保存
 *          Image2Lcd转换：水平扫描，16位真彩色
 * 参  数： uint16_t x,y     左上角起始坐标
 *          uint16_t width   图片宽度
 *          uint16_t height  图片高度
 *          uint8_t* image   数据缓存地址
 * 备  注： 魔女开发板团队
 *****************************************************************/
void LCD_Image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *image) 
{  
	LCD_SPI_Init;                                 // 防止SPI参数被其它设备修改了
	
	uint16_t colorData=0;
	uint32_t cnt=0;
	
 	for(uint16_t i=0; i<height; i++)              // 一行一行地显示
	{
 		setCursor(x, y+i, x+width, y+height);       // 重新设置光标位置 
		for(uint16_t j=0; j<width; j++)             // 一行中，从左到事，逐个像素处理
		{		    
		    colorData=(image[cnt*2+1]<<8) | image[cnt*2]; 
		    sendShort(colorData);                   // 写入16位颜色数据 
			cnt++;	
		}
	}	  
}  

/******************************************************************
 * 函数名： LCD_Chinese16ForFile
 * 功  能： 显示1个16号汉字,字库数据在font文件中，只适合少量汉字固定输出
 *          PCtoLCD2002取模：阴码+逐列式+顺向+C51格式
 * 参  数： uint16_t x,y     左上角起始坐标
 *          uint8_t  index   字库数据在缓存中的位置
 *          uint16_t fColor  字色
 *          uint16_t bColor  底色
 *****************************************************************/
//void LCD_Chinese16ForFile(uint32_t x, uint32_t y, uint8_t index, uint16_t fColor, uint16_t bColor)	
//{  
//	LCD_SPI_Init;                                        // 防止SPI参数被其它设备修改了
//	
//	const uint8_t *temp=hanzi16 + index*32;              // 获取字体数据在数组中的的起始位置 

//	for(uint8_t j=0;j<16;j++)
//	{
//		setCursor(x,y+j ,x+16, y+j+16);
//		for(uint8_t k=0;k<2;k++)
//		{
//			for(uint8_t i=0;i<8;i++)
//			{ 		     
//			 	if((*temp&(1<<i))!=0)			
//					sendShort(fColor);				
//				else			
//					sendShort(bColor);				 
//			}
//			temp++;
//		}
//	 }
//}	

/******************************************************************
 * 函数名： LCD_Chinese32ForFile
 * 功  能： 显示1个32号汉字,字库数据在font文件中，只适合少量汉字固定输出
 *          PCtoLCD2002取模：阴码+逐列式+顺向+C51格式
 * 参  数： uint16_t x,y     左上角起始坐标
 *          uint8_t  index   字库数据在缓存中的位置
 *          uint16_t fColor  字体颜色
 *          uint16_t bColor  背景颜色
 *****************************************************************/
//void LCD_Chinese32ForFile(uint32_t x, uint32_t y, uint8_t index, uint16_t fColor, uint16_t bColor)	
//{ 
//	LCD_SPI_Init;                                    // 防止SPI参数被其它设备修改了
//	
//	const uint8_t *temp = hanzi32 + index*128;       // 获取字体数据在数组中的的起始位置 

//	for(uint8_t j=0; j<32; j++)                      // 一行一行地向下写
//	{
//		setCursor(x,y+j,x+32, y+j+32);                 // 只要定位了首个坐标，后面发送的数据，就会向后排列，直至收到新的命令
//		for(uint8_t k=0;k<4;k++)
//		{
//			for(uint8_t i=0;i<8;i++)
//			{ 		     
//			 	if((*temp&(1<<i))!=0)				
//					sendShort(fColor);				
//				else
//					sendShort(bColor);				 
//			}
//			temp++;
//		}
//	 }
//}	

/******************************************************************
 * 函数名： drawAscii
 * 功  能： 在指定位置显示一个字符（字符数据已在font.h）
 * 参  数： uint16_t x,y     起始坐标
 *          uint8_t  num     要显示的字符:" "--->"~"
 *          uint8_t  size    字体大小 12/16/24/32
 *          uint16_t fColor  字体颜色
 *          uint16_t bColor  背景颜色
 *****************************************************************/
void drawAscii(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint16_t fColor, uint16_t bColor)
{  			
	LCD_SPI_Init;                                    // 防止SPI参数被其它设备修改了
	
  if(xLCD.InitOK ==0) return;
    
  uint8_t temp;
	uint16_t y0=y;
    
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);	 // 得到字体一个字符对应点阵集所占的字节数	
  num=num-' ';                                     // 得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(uint8_t t=0;t<csize;t++)
	{   
		if(size==12)         temp=asc2_1206[num][t];   // 调用1206字体
		else if(size==16)    temp=asc2_1608[num][t];   // 调用1608字体
		else if(size==24)    temp=asc2_2412[num][t];   // 调用2412字体
		else if(size==32)    temp=asc2_3216[num][t];   // 调用3216字体
		else return;								   // 没有的字库
		
		for(uint8_t t1=0; t1<8; t1++)
		{			    
            if(temp&0x80)   drawPoint (x, y, fColor);  // 字体 画点 
            else            drawPoint (x, y, bColor);  // 背景 画点
            temp<<=1;
			y++;
			if(y>=xLCD.height)    return;		         // 超出屏幕高度(底)
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=xLCD.width) return;              // 超出屏幕宽度(宽)
				break;
			}
		}  	 
	} 
}
//--------------------------从OLED显示中移植的 LCD显示数据值函数
//m^n 次方函数，下面要用
uint32_t LED_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;
	while(n--)
	{
	  result*=m;
	}
	return result;
}

////显示2个数字（原理类似printf %d，用到了上面的次方函数）
////x,y :起点坐标	 
////len :数字的位数
////size:字体大小
void LED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1,uint16_t fColor, uint16_t bColor)
{
	uint8_t t,temp;
	for(t=0;t<len;t++)
	{
		temp=(num/LED_Pow(10,len-t-1))%10;
			if(temp==0)
			{
				drawAscii(x+(size1/2)*t,y,'0',size1,fColor, bColor);
      }
			else 
			{
			  drawAscii(x+(size1/2)*t,y,temp+'0',size1,fColor, bColor);
			}
  }
}
//--------------------------
// code 字符指针开始
// 从字库中查找出字模
// code 字符串的开始地址,GBK码
// mat  数据存放地址 (size/8+((size%8)?1:0))*(size) bytes大小	
// size:字体大小
void GetGBK(uint8_t* code, uint8_t* mat, uint8_t size)
{		    
	uint8_t qh,ql;						  
	uint32_t foffset; 
	uint8_t csize=(size/8+((size%8)?1:0))*(size);    // 计算汉字点阵大小，单位字节数	 
	
  qh=*code;
	ql=*(++code);    
	
	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)    // 非常用汉字，将用填充显示整个位置
	{   	          
	    for(uint8_t i=0; i<csize; i++) *mat++=0x00;  // 填充满格
	    return;                                 // 结束访问
	}     

	if(ql<0x7f)ql-=0x40;                        // 计算要提取的汉字在字库中的偏移位置
	else ql-=0x41;
	qh-=0x81;   
	foffset=((unsigned long)190*qh+ql)*csize;	  // 得到汉字在字库中的偏移位置  		

	switch(size)
	{                                                                             // 按字体的不同，在不同字库读取字体点阵
		case 12:
			W25qxx_ReadBuffer( foffset + GBK_STORAGE_ADDR,            mat, csize);    // 12号字体           
			break;  
		case 16:
      W25qxx_ReadBuffer( foffset + GBK_STORAGE_ADDR+0x0008c460, mat, csize);    // 12号字体
			break;
		case 24:
      W25qxx_ReadBuffer( foffset + GBK_STORAGE_ADDR+0x001474E0, mat, csize);    // 24号字体
			break;
		case 32:
      W25qxx_ReadBuffer( foffset + GBK_STORAGE_ADDR+0x002EC200, mat, csize);    // 32号字体
			break;			
	}     												    
}  

// 显示汉字
void drawGBK( uint16_t x, uint16_t y, uint8_t* font, uint8_t size, uint16_t fColor, uint16_t bColor)
{	
  uint8_t temp;
	uint16_t y0=y;
	uint8_t GBK[128];   
	uint8_t csize=(size/8+((size%8)?1:0))*(size);	// 得到字体一个字符对应点阵集所占的字节数	 
	GetGBK(font, GBK, size);	                    // 得到相应大小的点阵数据
	
	LCD_SPI_Init;                                 // 防止SPI参数被其它设备修改了	
	for(uint8_t t=0; t<csize; t++)
	{   												   
		temp = GBK[t];			                        // 得到GBK点阵数据                          
		for(uint8_t t1=0; t1<8; t1++)
		{
			if(temp&0x80)   drawPoint (x, y, fColor);	
            else            drawPoint (x, y, bColor);            
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}      
}

/******************************************************************
 * 函数名： drawStringEN--drawString（加上了字库读取）
 * 功  能： 从指定位置开始显示一个字符串（已存Font.h），超长会自动换行	
 * 参  数： uint16_t x,y     起始坐标
 *          char     *pStr   要显示的字符串"  "指针
 *          uint8_t  size    字体大小 12/16/24/32
 *          uint16_t fColor  字体颜色
 *          uint16_t bColor  背景颜色
 *****************************************************************/
void drawString(uint16_t x,uint16_t y, char* pFont, uint8_t size,uint16_t fColor, uint16_t bColor)
{
    if(xLCD .InitOK ==0 ) return;
    
    uint16_t xStart = x;
    if( size!=12 && size!=16 && size!=24 && size!=32 )    size=16;    // 字体大小控制
    
    while(*pFont != 0)
    {
        // 位置控制        
        if(x>(xLCD.width-size))       // 如果这一行不够位置，就下一行
        {
            x=xStart;
            y=y+size;
        }
        if(y>(xLCD.height - size))    // 如果到了屏幕底部，就返回，不再输出
            return;                   // 也可改成x=0，y=0 从屏幕左上角输出
        
		    // 判断文字是ASCII还是汉字
        if(*pFont < 127)              // ASCII字符
        {
            drawAscii (x, y, *pFont, size, fColor, bColor);            
            pFont++;
            x+=size/2;            
        }
        else                          // 汉字显示（无字库，跳过，+2因为汉字内码双字节）
        {   
        // 重要: 如果用的不是魔女开发板的字库, 就要修改或注释下面这一行, 这样就不影响ASCII英文字符的输出            
            drawGBK(x, y, (uint8_t *)pFont, size, fColor, bColor);    					
            pFont = pFont+2;          // 下一个要显示的数据在内存中的位置              
            x=x+size;                 // 下一个要显示的数据在屏幕上的X位置    
        }
    }      
}
int LCD_String(uint16_t x,uint16_t y,char* pFont, uint8_t size,uint16_t fColor, uint16_t bColor)
{    
	char* startP=pFont;
	if(xLCD .InitOK ==0 ) return 0;
    
//    uint16_t xStart = x;
    if( size!=12 && size!=16 && size!=24 && size!=32 )    size=24;    // 字体大小控制
    
    while(*pFont != 0)
    {
        // 位置控制        
        if(x>(xLCD.width-size))       // 如果这一行不够位置，就下一行
        {
            x=0;
            y=y+size;
        }
        if(y>(xLCD.height - size))    // 如果到了屏幕底部，就返回，不再输出
            break;                   // 也可改成x=0，y=0 从屏幕左上角输出
        
		    // 判断文字是ASCII还是汉字
        if(*pFont < 127)              // ASCII字符
        {
            drawAscii (x, y, *pFont, size, fColor, bColor);            
            pFont++;
            x+=size/2;            
        }
        else                          // 汉字显示（无字库，跳过，+2因为汉字内码双字节）
        {   
        // 重要: 如果用的不是魔女开发板的字库, 就要修改或注释下面这一行, 这样就不影响ASCII英文字符的输出            
            drawGBK(x, y, (uint8_t *)pFont, size, fColor, bColor);    					
            pFont = pFont+2;          // 下一个要显示的数据在内存中的位置              
            x=x+size;                 // 下一个要显示的数据在屏幕上的X位置    
        }
    }
	  
	return pFont-startP;
}

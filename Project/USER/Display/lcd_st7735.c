/***********************************************************************************************************************************
    ** MCUDEV 1.8���ɫ��ʾ��ģ������˵��(ST7735)
    ** GND     ��Դ��
    ** VCC     3.3V����Ҫ��5V, ģ�鲻��LDO��5V��������
    ** SC      SPI��SCK����  
    ** DI      SPI��MOSI���� 
    ** RST     ��λ���ţ���ú͵�Ƭ���ĸ�λ�������
    ** RS      �������ѡ������
    ** CS      SPI��CSƬѡ����
    ** BLK     ���Ʊ���LED�� �ߵ�ƽ���������Ѵ�1K���裬����10K������
    ** ����Ӳ��   Nucleo-64 STM32F401RE��
    ** ��ע    �ο��˹ٷ���������Keil_v5\ARM\PACK\Keil\STM32F4xx_DFP
    **            \2.13.0\Drivers\BSP\Components\st7735\st7735.c
		**  128*160
************************************************************************************************************************************/
#include "lcd_st7735.h"
#include "spi.h"
#include "gpio.h"
#include "stm32f1xx_hal.h"
#include "font.h"

/*****************************************************************************
 ** ���ر���
 *****************************************************************************/
struct _LCD              // �ṹ�壺���ڱ�����Ϣ��״̬
{     
  uint8_t  InitOK;
  uint16_t width;        // LCD ���
  uint16_t height;       // LCD �߶�
  uint16_t bColor;       // ����ɫ
}xLCD;

/*****************************************************************************
 ** ���غ�����HAL_SPI�ٷ�װ
 ****************************************************************************/
/**
 * @brief    SPI �����ֽں�������װHAL�⺯������SPI��Ψһ�����Ա��޸ģ�
 * @param    TxData Ҫ���͵��ֽ�����ָ��
 * @return   0:д��ɹ�,����:д��ʧ��
 * @��ע     ������ģ��SPI����Ҫ���޸Ĵ˺���
 */
uint8_t SPI_WriteByte(uint8_t TxData)
{
  return HAL_SPI_Transmit(&hspiX,&TxData,1,500);
}

/*****************************************************************************
 ** ����HAL_SPI��LCD����оƬ�������������������LCD����оƬ�Ĳ���˼�����ƣ�
 ****************************************************************************/
/**
 * @brief   д���LCD������оƬ��
 * @param   cmd ���� ��Ҫ���͵�����=��ʵ���ǿ���оƬҪд���ݵļĴ�����ַ
 * @return  none
 */
static void sendOrder(uint8_t cmd)
{
    LCD_CS_EN;             // ƬѡSPI�豸LCD����ʼͨ��
    LCD_DC_COM;            // DAT: ���ݣ� COM: ����=�Ĵ�����ֵַ
    SPI_WriteByte(cmd);
    LCD_CS_DIS;            // SPI�豸Ƭѡȡ��������ͨ��
}

/**
 * @brief   д1�ֽ����ݵ�LCD
 * @param   dat ���� ��Ҫ���͵�����=���������оƬ��ָ���ļĴ�����ַд�������
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
 * @brief   д2�ֽ����ݵ�LCD
 * @param   Data ���� ��Ҫ���͵�2�ֽ����ݣ���Ҫ����ɫ����
 * @return  none
 */
static void sendShort(uint16_t Data)
{
    LCD_CS_EN; 
    LCD_DC_DAT;  
    SPI_WriteByte(Data>>8);  //��HAL_SPI�ⷢ�;���ɫ���ݣ���8λ�ȷ�
	  SPI_WriteByte(Data); 
    LCD_CS_DIS;  
}

/*****************************************************************************
 ** ������������������LCDӲ����������--����оƬ��Datasheet�ź���⣬����HAL�����
 ****************************************************************************/

/*****************************************************************************
*��  ����LCD_Init
*��  �ܣ���ʼ��lcd
*��  ����Ϊ�˼���⡢������ֲ������(��Ļ���ء���ʾ����)��h�ļ�������޸�
*����ֵ���� 
*��  ע������������1.8��TFT������оƬST7735
*****************************************************************************/
void LCD_Init(short BG)  
{    
    xLCD.InitOK = 0; 
    xLCD.bColor  = BG;
  // ��ʾ��������ƥ��
    if( (LCD_DIR ==1) || (LCD_DIR ==3))
    {
        xLCD.width  = LCD_WIDTH ;       // ��������أ�������ֵ����оƬ���Զ����У�ע�⣺�����Ļ�ұ��л������ͼӴ����ֵ
        xLCD.height = LCD_HIGH ;        // ���߶����أ� ע�⣺�����Ļ�����л������ͼӴ����ֵ
    }
    else
    {
        xLCD.width  = LCD_HIGH;
        xLCD.height = LCD_WIDTH;
    }      

  // ��Ļ��λ	(��λ��������Init���������������
  LCD_RST_EN;	             // SPI�ӿڸ�λ
  HAL_Delay(20);           // delay 20 ms 
  LCD_RST_DIS;             // SPI�ӿڸ�λ�ָ� 
  HAL_Delay(20);
  
  //-------------- ���´����и��� ---------------------------------
	sendOrder(0x11);         // �˳�˯��ģʽ
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
	//sendByte(0xC0);                  // ��Ҫ����ʾ������ƣ�C0/00/A0/60,  C8/08/A8/68
	if(LCD_DIR == 1)  sendByte(0xC0);  // ��Ҫ����ʾ������ƣ�C0/00/A0/60,  C8/08/A8/68
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

  //-------------------------  �������end ----------------------------------
	
  LCD_Fill(1, 1, xLCD.width , xLCD.height , BG );  //�����·��������ʾ��������
  LCD_BKL_ON;      // ����
  xLCD.InitOK = 1; 	
}  


/*****************************************************************
 * ��  ����setCursor
 * ��  �ܣ�������ʾ�����ڴ�����д�������Զ�����
 * ��  ����xStart���������, yStart���������
 *         xEnd�����������㣬yEnd������������
 * ����ֵ����
******************************************************************/
void setCursor(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd)
{		
	sendOrder(0x2A);
	sendByte (0x00);   // ��ʼλ��x��λ����Ϊ1.8������128*160, ������255, ֱ��д0ʡ��
	sendByte (xStart); // ��ʼλ��x��λ��ֵ����ʱ�Զ������˸�8λ��Ҳʡ��������
	sendByte (0x00);   // ��ʼλ��y��λ
	sendByte (xEnd);   // ��ʼλ��xλλ

	sendOrder(0x2B);
	sendByte (0x00);
	sendByte (yStart);
	sendByte (0x00);
	sendByte (yEnd);
	
	sendOrder(0x2c);  // ����д��������
}

/*****************************************************************************
 ** ������������Ӳ��������LCD��ʾ����������HAL�⼰Ӳ�����Ѹ��룬��ֲʱ�������ظĶ�
 ****************************************************************************/

/*****************************************************************
 * ��  ����drawPoint
 * ��  �ܣ���һ����
 * ��  ����x���꣬y����, 16λ��ɫֵ
 * ����ֵ����
******************************************************************/
void drawPoint(uint16_t x, uint16_t y, uint16_t color)
{
    setCursor(x, y, x, y);      //���ù��λ�� 
    sendShort(color);    
}

/******************************************************************
 * �������� LCD_Fill
 * ��  �ܣ� ��ָ����������䵥����ɫ
 *          ��Ļ�����1��ʼ����Ļ���Һ��·�ʵ�����ж�������
 * ��  ���� uint16_t xStart, yStart  ���Ͻ���ʼ����
 *          uint16_t xEnd,   yEnd    ���½���ʼ����
 *          uint16_t Color           ������ɫ
 *****************************************************************/
void LCD_Fill(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t color)
{      
  LCD_SPI_Init;                                       // ��ֹSPI�����������豸�޸���
  uint32_t pixel = (xEnd-xStart+1) * (yEnd-yStart+1); // ������������

  setCursor(xStart, yStart, xEnd, yEnd);              // �趨��䷶Χ
  while(pixel-->0)                                    // ������ɫֵ
     sendShort(color);                            
}  

/******************************************************************
 * �������� LCD_Line
 * ��  �ܣ� ����
 * ��  ���� uint16_t x1,y1  �������
 *          uint16_t x2,y2  �յ����� 
 *          uint16_t Color  ��ɫ
 *****************************************************************/
void LCD_Line(uint16_t sX, uint16_t sY, uint16_t eX, uint16_t eY, uint16_t color)
{
	LCD_SPI_Init;                                       // ��ֹSPI�����������豸�޸���
	
	uint16_t t; 
	int xerr=0, yerr=0, delta_x, delta_y, distance; 
	int incx,incy,uRow,uCol; 
	delta_x = eX - sX; //������������ 
	delta_y = eY - sY; 
	uRow = sX; 
	uCol = sY; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		drawPoint(uRow, uCol, color);//���� 
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

//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ㣻r:�뾶
void LCD_Circle(uint16_t x0,uint16_t y0,uint8_t r, uint16_t color)
{
  LCD_SPI_Init;                              // ��ֹSPI�����������豸�޸���
  int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);                               //�ж��¸���λ�õı�־
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
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		}    
	}
} 

/******************************************************************
 * �������� LCD_Image
 * ��  �ܣ� ��ָ�����������ָ��ͼƬ����
 *          ͼƬ������font.h�ļ���.ֻ�ʺ�����ͼƬ����
 *          Image2Lcdת����ˮƽɨ�裬16λ���ɫ
 * ��  ���� uint16_t x,y     ���Ͻ���ʼ����
 *          uint16_t width   ͼƬ���
 *          uint16_t height  ͼƬ�߶�
 *          uint8_t* image   ���ݻ����ַ
 * ��  ע�� ħŮ�������Ŷ�
 *****************************************************************/
void LCD_Image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *image) 
{  
	LCD_SPI_Init;                                 // ��ֹSPI�����������豸�޸���
	
	uint16_t colorData=0;
	uint32_t cnt=0;
	
 	for(uint16_t i=0; i<height; i++)              // һ��һ�е���ʾ
	{
 		setCursor(x, y+i, x+width, y+height);       // �������ù��λ�� 
		for(uint16_t j=0; j<width; j++)             // һ���У������£�������ش���
		{		    
		    colorData=(image[cnt*2+1]<<8) | image[cnt*2]; 
		    sendShort(colorData);                   // д��16λ��ɫ���� 
			cnt++;	
		}
	}	  
}  

/******************************************************************
 * �������� LCD_Chinese16ForFile
 * ��  �ܣ� ��ʾ1��16�ź���,�ֿ�������font�ļ��У�ֻ�ʺ��������̶ֹ����
 *          PCtoLCD2002ȡģ������+����ʽ+˳��+C51��ʽ
 * ��  ���� uint16_t x,y     ���Ͻ���ʼ����
 *          uint8_t  index   �ֿ������ڻ����е�λ��
 *          uint16_t fColor  ��ɫ
 *          uint16_t bColor  ��ɫ
 *****************************************************************/
//void LCD_Chinese16ForFile(uint32_t x, uint32_t y, uint8_t index, uint16_t fColor, uint16_t bColor)	
//{  
//	LCD_SPI_Init;                                        // ��ֹSPI�����������豸�޸���
//	
//	const uint8_t *temp=hanzi16 + index*32;              // ��ȡ���������������еĵ���ʼλ�� 

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
 * �������� LCD_Chinese32ForFile
 * ��  �ܣ� ��ʾ1��32�ź���,�ֿ�������font�ļ��У�ֻ�ʺ��������̶ֹ����
 *          PCtoLCD2002ȡģ������+����ʽ+˳��+C51��ʽ
 * ��  ���� uint16_t x,y     ���Ͻ���ʼ����
 *          uint8_t  index   �ֿ������ڻ����е�λ��
 *          uint16_t fColor  ������ɫ
 *          uint16_t bColor  ������ɫ
 *****************************************************************/
//void LCD_Chinese32ForFile(uint32_t x, uint32_t y, uint8_t index, uint16_t fColor, uint16_t bColor)	
//{ 
//	LCD_SPI_Init;                                    // ��ֹSPI�����������豸�޸���
//	
//	const uint8_t *temp = hanzi32 + index*128;       // ��ȡ���������������еĵ���ʼλ�� 

//	for(uint8_t j=0; j<32; j++)                      // һ��һ�е�����д
//	{
//		setCursor(x,y+j,x+32, y+j+32);                 // ֻҪ��λ���׸����꣬���淢�͵����ݣ��ͻ�������У�ֱ���յ��µ�����
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
 * �������� drawAscii
 * ��  �ܣ� ��ָ��λ����ʾһ���ַ����ַ���������font.h��
 * ��  ���� uint16_t x,y     ��ʼ����
 *          uint8_t  num     Ҫ��ʾ���ַ�:" "--->"~"
 *          uint8_t  size    �����С 12/16/24/32
 *          uint16_t fColor  ������ɫ
 *          uint16_t bColor  ������ɫ
 *****************************************************************/
void drawAscii(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint16_t fColor, uint16_t bColor)
{  			
	LCD_SPI_Init;                                    // ��ֹSPI�����������豸�޸���
	
  if(xLCD.InitOK ==0) return;
    
  uint8_t temp;
	uint16_t y0=y;
    
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);	 // �õ�����һ���ַ���Ӧ������ռ���ֽ���	
  num=num-' ';                                     // �õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
	for(uint8_t t=0;t<csize;t++)
	{   
		if(size==12)         temp=asc2_1206[num][t];   // ����1206����
		else if(size==16)    temp=asc2_1608[num][t];   // ����1608����
		else if(size==24)    temp=asc2_2412[num][t];   // ����2412����
		else if(size==32)    temp=asc2_3216[num][t];   // ����3216����
		else return;								   // û�е��ֿ�
		
		for(uint8_t t1=0; t1<8; t1++)
		{			    
            if(temp&0x80)   drawPoint (x, y, fColor);  // ���� ���� 
            else            drawPoint (x, y, bColor);  // ���� ����
            temp<<=1;
			y++;
			if(y>=xLCD.height)    return;		         // ������Ļ�߶�(��)
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=xLCD.width) return;              // ������Ļ���(��)
				break;
			}
		}  	 
	} 
}
//--------------------------��OLED��ʾ����ֲ�� LCD��ʾ����ֵ����
//m^n �η�����������Ҫ��
uint32_t LED_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;
	while(n--)
	{
	  result*=m;
	}
	return result;
}

////��ʾ2�����֣�ԭ������printf %d���õ�������Ĵη�������
////x,y :�������	 
////len :���ֵ�λ��
////size:�����С
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
// code �ַ�ָ�뿪ʼ
// ���ֿ��в��ҳ���ģ
// code �ַ����Ŀ�ʼ��ַ,GBK��
// mat  ���ݴ�ŵ�ַ (size/8+((size%8)?1:0))*(size) bytes��С	
// size:�����С
void GetGBK(uint8_t* code, uint8_t* mat, uint8_t size)
{		    
	uint8_t qh,ql;						  
	uint32_t foffset; 
	uint8_t csize=(size/8+((size%8)?1:0))*(size);    // ���㺺�ֵ����С����λ�ֽ���	 
	
  qh=*code;
	ql=*(++code);    
	
	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)    // �ǳ��ú��֣����������ʾ����λ��
	{   	          
	    for(uint8_t i=0; i<csize; i++) *mat++=0x00;  // �������
	    return;                                 // ��������
	}     

	if(ql<0x7f)ql-=0x40;                        // ����Ҫ��ȡ�ĺ������ֿ��е�ƫ��λ��
	else ql-=0x41;
	qh-=0x81;   
	foffset=((unsigned long)190*qh+ql)*csize;	  // �õ��������ֿ��е�ƫ��λ��  		

	switch(size)
	{                                                                             // ������Ĳ�ͬ���ڲ�ͬ�ֿ��ȡ�������
		case 12:
			W25qxx_ReadBuffer( foffset + GBK_STORAGE_ADDR,            mat, csize);    // 12������           
			break;  
		case 16:
      W25qxx_ReadBuffer( foffset + GBK_STORAGE_ADDR+0x0008c460, mat, csize);    // 12������
			break;
		case 24:
      W25qxx_ReadBuffer( foffset + GBK_STORAGE_ADDR+0x001474E0, mat, csize);    // 24������
			break;
		case 32:
      W25qxx_ReadBuffer( foffset + GBK_STORAGE_ADDR+0x002EC200, mat, csize);    // 32������
			break;			
	}     												    
}  

// ��ʾ����
void drawGBK( uint16_t x, uint16_t y, uint8_t* font, uint8_t size, uint16_t fColor, uint16_t bColor)
{	
  uint8_t temp;
	uint16_t y0=y;
	uint8_t GBK[128];   
	uint8_t csize=(size/8+((size%8)?1:0))*(size);	// �õ�����һ���ַ���Ӧ������ռ���ֽ���	 
	GetGBK(font, GBK, size);	                    // �õ���Ӧ��С�ĵ�������
	
	LCD_SPI_Init;                                 // ��ֹSPI�����������豸�޸���	
	for(uint8_t t=0; t<csize; t++)
	{   												   
		temp = GBK[t];			                        // �õ�GBK��������                          
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
 * �������� drawStringEN--drawString���������ֿ��ȡ��
 * ��  �ܣ� ��ָ��λ�ÿ�ʼ��ʾһ���ַ������Ѵ�Font.h�����������Զ�����	
 * ��  ���� uint16_t x,y     ��ʼ����
 *          char     *pStr   Ҫ��ʾ���ַ���"  "ָ��
 *          uint8_t  size    �����С 12/16/24/32
 *          uint16_t fColor  ������ɫ
 *          uint16_t bColor  ������ɫ
 *****************************************************************/
void drawString(uint16_t x,uint16_t y, char* pFont, uint8_t size,uint16_t fColor, uint16_t bColor)
{
    if(xLCD .InitOK ==0 ) return;
    
    uint16_t xStart = x;
    if( size!=12 && size!=16 && size!=24 && size!=32 )    size=16;    // �����С����
    
    while(*pFont != 0)
    {
        // λ�ÿ���        
        if(x>(xLCD.width-size))       // �����һ�в���λ�ã�����һ��
        {
            x=xStart;
            y=y+size;
        }
        if(y>(xLCD.height - size))    // ���������Ļ�ײ����ͷ��أ��������
            return;                   // Ҳ�ɸĳ�x=0��y=0 ����Ļ���Ͻ����
        
		    // �ж�������ASCII���Ǻ���
        if(*pFont < 127)              // ASCII�ַ�
        {
            drawAscii (x, y, *pFont, size, fColor, bColor);            
            pFont++;
            x+=size/2;            
        }
        else                          // ������ʾ�����ֿ⣬������+2��Ϊ��������˫�ֽڣ�
        {   
        // ��Ҫ: ����õĲ���ħŮ��������ֿ�, ��Ҫ�޸Ļ�ע��������һ��, �����Ͳ�Ӱ��ASCIIӢ���ַ������            
            drawGBK(x, y, (uint8_t *)pFont, size, fColor, bColor);    					
            pFont = pFont+2;          // ��һ��Ҫ��ʾ���������ڴ��е�λ��              
            x=x+size;                 // ��һ��Ҫ��ʾ����������Ļ�ϵ�Xλ��    
        }
    }      
}
int LCD_String(uint16_t x,uint16_t y,char* pFont, uint8_t size,uint16_t fColor, uint16_t bColor)
{    
	char* startP=pFont;
	if(xLCD .InitOK ==0 ) return 0;
    
//    uint16_t xStart = x;
    if( size!=12 && size!=16 && size!=24 && size!=32 )    size=24;    // �����С����
    
    while(*pFont != 0)
    {
        // λ�ÿ���        
        if(x>(xLCD.width-size))       // �����һ�в���λ�ã�����һ��
        {
            x=0;
            y=y+size;
        }
        if(y>(xLCD.height - size))    // ���������Ļ�ײ����ͷ��أ��������
            break;                   // Ҳ�ɸĳ�x=0��y=0 ����Ļ���Ͻ����
        
		    // �ж�������ASCII���Ǻ���
        if(*pFont < 127)              // ASCII�ַ�
        {
            drawAscii (x, y, *pFont, size, fColor, bColor);            
            pFont++;
            x+=size/2;            
        }
        else                          // ������ʾ�����ֿ⣬������+2��Ϊ��������˫�ֽڣ�
        {   
        // ��Ҫ: ����õĲ���ħŮ��������ֿ�, ��Ҫ�޸Ļ�ע��������һ��, �����Ͳ�Ӱ��ASCIIӢ���ַ������            
            drawGBK(x, y, (uint8_t *)pFont, size, fColor, bColor);    					
            pFont = pFont+2;          // ��һ��Ҫ��ʾ���������ڴ��е�λ��              
            x=x+size;                 // ��һ��Ҫ��ʾ����������Ļ�ϵ�Xλ��    
        }
    }
	  
	return pFont-startP;
}

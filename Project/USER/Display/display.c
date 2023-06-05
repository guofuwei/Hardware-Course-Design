#include "display.h"
#include <string.h>
#include "lcd_st7735.h"

CURSOR Cursor={1,1};

uint8_t private_cursor_check(void)
{
	if(Cursor.x_cur<=0||Cursor.y_cur<=0)
	{
		drawString(1,1,"无效的Cursor值",16,RED,BLACK);
		Cursor.x_cur=1;
		Cursor.y_cur=1;
		return 1;
	}
	return 0;
}

void display_string(char* pFont, uint8_t size,uint16_t fColor, uint16_t bColor)
{
	uint8_t res=private_cursor_check();
	if(res!=0)return;
	drawString(1,Cursor.y_cur,pFont,size,fColor,bColor);
	Cursor.x_cur=size;
	Cursor.y_cur+=size;
}


void display_string_center(char* pFont, uint8_t size,uint16_t fColor, uint16_t bColor)
{
	uint8_t res=private_cursor_check();
	if(res!=0)return;
	uint16_t total_size=strlen(pFont)*size/2;
	drawString(LCD_WIDTH/2-total_size/2,Cursor.y_cur,pFont,size,fColor,bColor);
	Cursor.x_cur=size;
	Cursor.y_cur+=size;
}



void display_string_custom(char* pFont, uint8_t size,uint16_t fColor, uint16_t bColor,uint16_t x,uint16_t y)
{
	uint8_t res=private_cursor_check();
	if(res!=0)return;
	drawString(x,y,pFont,size,fColor,bColor);
	Cursor.x_cur=x+size;
	Cursor.y_cur=y+size;
}


void clear_screen(void)
{
	LCD_Fill(1, 1, LCD_WIDTH ,LCD_HIGH , BLACK ); 
}

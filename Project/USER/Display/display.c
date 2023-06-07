#include "display.h"
#include <string.h>
#include "lcd_st7735.h"
#include "main.h"
#include "fatfs.h"

CURSOR Cursor={1,1+SCREEN_HEAD_HEIGHT};
extern MENUINFO MenuInfo;
extern uint8_t SelectIndex;
extern bool SubMenuOk;
extern char NameList[NAMELIST_NUM][NAMELIST_MAX_LEN];
extern uint8_t FileNum;

uint8_t private_cursor_check(void)
{
	if(Cursor.x_cur<=0||Cursor.y_cur<=0)
	{
		drawString(1,1,"无效的Cursor值",16,RED,BLACK);
		Cursor.x_cur=1;
		Cursor.y_cur=1+SCREEN_HEAD_HEIGHT;
		return 1;
	}
	return 0;
}

void display_string(char* pFont, uint8_t size,uint16_t fColor, uint16_t bColor)
{
	uint8_t res=private_cursor_check();
	if(res!=0)return;
	drawString(1,Cursor.y_cur,pFont,size,fColor,bColor);
	Cursor.y_cur+=size;
}


void display_string_center(char* pFont, uint8_t size,uint16_t fColor, uint16_t bColor)
{
	uint8_t res=private_cursor_check();
	if(res!=0)return;
	uint16_t total_size=strlen(pFont)*size/2;
	drawString(LCD_WIDTH/2-total_size/2,Cursor.y_cur,pFont,size,fColor,bColor);
	Cursor.y_cur+=size;
}

void display_string_custom(char* pFont, uint8_t size,uint16_t fColor, uint16_t bColor,uint16_t x,uint16_t y)
{
	uint8_t res=private_cursor_check();
	if(res!=0)return;
	drawString(x,y,pFont,size,fColor,bColor);
	Cursor.y_cur=y+size;
}

void set_head_string(char* pFont,uint16_t fColor, uint16_t bColor)
{
  uint8_t res=private_cursor_check();
	if(res!=0)return;
  clear_screen_head(bColor);
  uint16_t total_size=strlen(pFont)*SCREEN_HEAD_HEIGHT/2;
	drawString(LCD_WIDTH/2-total_size/2,1,pFont,SCREEN_HEAD_HEIGHT,fColor,bColor);
}

void clear_screen_all(void)
{
	LCD_Fill(1, 1, LCD_WIDTH ,LCD_HIGH , BLACK );
  Cursor.x_cur=1;
  Cursor.y_cur=1+SCREEN_HEAD_HEIGHT;
}

void clear_screen_head( uint16_t bColor)
{
	LCD_Fill(1, 1, LCD_WIDTH, SCREEN_HEAD_HEIGHT, bColor ); 

}

void clear_screen_content(void)
{
	LCD_Fill(1, 1+SCREEN_HEAD_HEIGHT, LCD_WIDTH ,LCD_HIGH , BLACK ); 
  Cursor.x_cur=1;
  Cursor.y_cur=1+SCREEN_HEAD_HEIGHT;
}

void display_margin(uint8_t size)
{
//  drawString(1,Cursor.y_cur,pFont,size,fColor,bColor);
//  LCD_Fill(1,Cursor.y_cur,LCD_WIDTH,Cursor.y_cur+size,BLACK);
  Cursor.y_cur=Cursor.y_cur+size;
}


// 下面的函数用于菜单绘制
void draw_main_menu(void)
{
  // 菜单复位
  MenuInfo=MAIN_MENU;
  SelectIndex=0;
  SubMenuOk=true;
  set_head_string("主菜单",WHITE,GOODBLUE);
  clear_screen_content();
  
  
  display_string("请选择功能:",MENU_HEIGHT_SIZE,WHITE,BLACK);
  display_string_custom("查看文本文件",MENU_HEIGHT_SIZE,WHITE,BLACK,5,Cursor.y_cur);
  display_string_custom("查看图片文件",MENU_HEIGHT_SIZE,WHITE,BLACK,5,Cursor.y_cur);
  display_string_custom("查看音乐文件",MENU_HEIGHT_SIZE,WHITE,BLACK,5,Cursor.y_cur);
  display_string_custom("查看说明",MENU_HEIGHT_SIZE,WHITE,BLACK,5,Cursor.y_cur);
  draw_select();
}

void draw_txt_menu(void)
{
  // 菜单复位
  MenuInfo=TXT_MENU;
  SelectIndex=0;
  SubMenuOk=true;
  set_head_string("文本文件菜单",WHITE,GOODBLUE);
  clear_screen_content();
  
  
  FRESULT res=f_scandir(TXT_FOLDER);
  if(res!=FR_OK)
  {
    f_scandir_handle(res);
    display_string("文件扫描失败",MENU_HEIGHT_SIZE,WHITE,BLACK);
    SubMenuOk=false;
    return;
  }
  display_string("请选择文件:",MENU_HEIGHT_SIZE,WHITE,BLACK);
  uint8_t i=0;
  for(i=0;i<NAMELIST_NUM;i++)
  {
    if(strcmp(NameList[i],"\0")==0)break;
    display_string_custom(NameList[i],MENU_HEIGHT_SIZE,WHITE,BLACK,5,Cursor.y_cur);
  }
  FileNum=i;
  draw_select();
}

void draw_pic_menu(void)
{
  // 菜单复位
  MenuInfo=PIC_MENU;
  SelectIndex=0;
  SubMenuOk=true;
  set_head_string("图片文件菜单",WHITE,GOODBLUE);
  clear_screen_content();
  
  
  FRESULT res=f_scandir(PIC_FOLDER);
  if(res!=FR_OK)
  {
    f_scandir_handle(res);
    display_string("文件扫描失败",MENU_HEIGHT_SIZE,WHITE,BLACK);
    SubMenuOk=false;
    return;
  }
  display_string("请选择文件:",MENU_HEIGHT_SIZE,WHITE,BLACK);
  uint8_t i=0;
  for(i=0;i<NAMELIST_NUM;i++)
  {
    if(strcmp(NameList[i],"\0")==0)break;
    display_string_custom(NameList[i],MENU_HEIGHT_SIZE,WHITE,BLACK,5,Cursor.y_cur);
  }
  FileNum=i;
  draw_select();
}

void draw_music_menu(void)
{
  // 菜单复位
  MenuInfo=MUSIC_MENU;
  SelectIndex=0;
  SubMenuOk=true;
  set_head_string("音乐文件菜单",WHITE,GOODBLUE);
  clear_screen_content();
  
  
  FRESULT res=f_scandir(MUSIC_FOLDER);
  if(res!=FR_OK)
  {
    f_scandir_handle(res);
    display_string("文件扫描失败",MENU_HEIGHT_SIZE,WHITE,BLACK);
    SubMenuOk=false;
    return;
  }
  display_string("请选择文件:",MENU_HEIGHT_SIZE,WHITE,BLACK);
  uint8_t i=0;
  for(i=0;i<NAMELIST_NUM;i++)
  {
    if(strcmp(NameList[i],"\0")==0)break;
    display_string_custom(NameList[i],MENU_HEIGHT_SIZE,WHITE,BLACK,5,Cursor.y_cur);
  }
  FileNum=i;
  draw_select();
}

void clear_select(void)
{
  uint8_t y=1+SCREEN_HEAD_HEIGHT+MENU_HEIGHT_SIZE*(SelectIndex+1);
  LCD_Fill(1,y,3,y+MENU_HEIGHT_SIZE,BLACK);
}


void draw_select(void)
{
  uint8_t y=1+SCREEN_HEAD_HEIGHT+MENU_HEIGHT_SIZE*(SelectIndex+1);
  LCD_Fill(1,y,3,y+MENU_HEIGHT_SIZE,YELLOW);
}

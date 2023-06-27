#include "display.h"
#include <string.h>
#include "lcd_st7735.h"
#include "main.h"
#include "fatfs.h"
#include "VS1053.h"
#include "piclib.h"
#include "control.h"

CURSOR Cursor = {1, 1 + SCREEN_HEAD_HEIGHT};
extern MENUINFO MenuInfo;
extern uint8_t SelectIndex;
extern bool SubMenuOk;
extern char NameList[NAMELIST_NUM][NAMELIST_MAX_LEN];
extern uint8_t FileNum;
extern KEYSTATUS KeyStatus;
extern char Buffer[BUFFER_SIZE];
extern int16_t BufferPos;
extern bool IsEndTxt;

extern char SongFullName[10 + NAMELIST_MAX_LEN]; // 用于音乐播放
extern bool IsPlay;

uint8_t private_cursor_check(void)
{
  if (Cursor.x_cur <= 0 || Cursor.y_cur <= 0)
  {
    drawString(1, 1, "无效的Cursor值", 16, RED, BLACK);
    Cursor.x_cur = 1;
    Cursor.y_cur = 1 + SCREEN_HEAD_HEIGHT;
    return 1;
  }
  return 0;
}

void display_string(char *pFont, uint8_t size, uint16_t fColor, uint16_t bColor)
{
  uint8_t res = private_cursor_check();
  if (res != 0)
    return;
  LCD_String(1, Cursor.y_cur, pFont, size, fColor, bColor);
  Cursor.y_cur += size;
}

void display_string_center(char *pFont, uint8_t size, uint16_t fColor, uint16_t bColor)
{
  uint8_t res = private_cursor_check();
  if (res != 0)
    return;
  uint16_t total_size = strlen(pFont) * size / 2;
  drawString(LCD_WIDTH / 2 - total_size / 2, Cursor.y_cur, pFont, size, fColor, bColor);
  Cursor.y_cur += size;
}

void display_string_custom(char *pFont, uint8_t size, uint16_t fColor, uint16_t bColor, uint16_t x, uint16_t y)
{
  uint8_t res = private_cursor_check();
  if (res != 0)
    return;
  drawString(x, y, pFont, size, fColor, bColor);
  Cursor.y_cur = y + size;
}

void set_head_string(char *pFont, uint16_t fColor, uint16_t bColor)
{
  uint8_t res = private_cursor_check();
  if (res != 0)
    return;
  clear_screen_head(bColor);
  uint16_t total_size = strlen(pFont) * SCREEN_HEAD_HEIGHT / 2;
  drawString(LCD_WIDTH / 2 - total_size / 2, 1, pFont, SCREEN_HEAD_HEIGHT, fColor, bColor);
}

void clear_screen_all(void)
{
  LCD_Fill(1, 1, LCD_WIDTH, LCD_HIGH, BLACK);
  Cursor.x_cur = 1;
  Cursor.y_cur = 1 + SCREEN_HEAD_HEIGHT;
}

void clear_screen_head(uint16_t bColor)
{
  LCD_Fill(1, 1, LCD_WIDTH, SCREEN_HEAD_HEIGHT, bColor);
}

void clear_current_line(uint8_t size)
{
  Cursor.y_cur -= size;
  LCD_Fill(1, Cursor.y_cur, LCD_WIDTH, Cursor.y_cur+size, BLACK);
}

void clear_screen_content(void)
{
  LCD_Fill(1, 1 + SCREEN_HEAD_HEIGHT, LCD_WIDTH, LCD_HIGH, BLACK);
  Cursor.x_cur = 1;
  Cursor.y_cur = 1 + SCREEN_HEAD_HEIGHT;
}

void display_margin(uint8_t size)
{
  //  drawString(1,Cursor.y_cur,pFont,size,fColor,bColor);
  //  LCD_Fill(1,Cursor.y_cur,LCD_WIDTH,Cursor.y_cur+size,BLACK);
  Cursor.y_cur = Cursor.y_cur + size;
}

// 下面的函数用于菜单绘制
void draw_main_menu(void)
{
  // 菜单复位
  KeyStatus = KEY_DENY;
  MenuInfo = MAIN_MENU;
  SelectIndex = 0;
  SubMenuOk = true;
  set_head_string("主菜单", WHITE, GOODBLUE);
  clear_screen_content();

  display_string("请选择功能:", MENU_HEIGHT_SIZE, WHITE, BLACK);
  display_string_custom("查看文本文件", MENU_HEIGHT_SIZE, WHITE, BLACK, 5, Cursor.y_cur);
  display_string_custom("查看图片文件", MENU_HEIGHT_SIZE, WHITE, BLACK, 5, Cursor.y_cur);
  display_string_custom("查看音乐文件", MENU_HEIGHT_SIZE, WHITE, BLACK, 5, Cursor.y_cur);
  display_string_custom("查看说明", MENU_HEIGHT_SIZE, WHITE, BLACK, 5, Cursor.y_cur);
  draw_select();
  KeyStatus = KEY_ACCEPT;
}

void draw_txt_menu(void)
{
  // 菜单复位
  MenuInfo = TXT_MENU;
  SelectIndex = 0;
  SubMenuOk = true;
  set_head_string("文本文件菜单", WHITE, GOODBLUE);
  clear_screen_content();

  //  printf("aaaa");
  FRESULT res = f_scandir(TXT_FOLDER);
  //  printf("bbbbb");
  if (res != FR_OK)
  {
    f_scandir_handle(res);
    printf("cccc");
    display_string("文件扫描失败", MENU_HEIGHT_SIZE, WHITE, BLACK);
    SubMenuOk = false;
    return;
  }
  display_string("请选择文件:", MENU_HEIGHT_SIZE, WHITE, BLACK);
  uint8_t i = 0;
  for (i = 0; i < NAMELIST_NUM; i++)
  {
    if (strcmp(NameList[i], "\0") == 0)
      break;
    display_string_custom(NameList[i], MENU_HEIGHT_SIZE, WHITE, BLACK, 5, Cursor.y_cur);
  }
  FileNum = i;
  draw_select();
  SubMenuOk = true;
}

void draw_pic_menu(void)
{
  // 菜单复位
  MenuInfo = PIC_MENU;
  SelectIndex = 0;
  SubMenuOk = true;
  set_head_string("图片文件菜单", WHITE, GOODBLUE);
  clear_screen_content();

  FRESULT res = f_scandir(PIC_FOLDER);
  if (res != FR_OK)
  {
    f_scandir_handle(res);
    display_string("文件扫描失败", MENU_HEIGHT_SIZE, WHITE, BLACK);
    SubMenuOk = false;
    return;
  }
  display_string("请选择文件:", MENU_HEIGHT_SIZE, WHITE, BLACK);
  uint8_t i = 0;
  for (i = 0; i < NAMELIST_NUM; i++)
  {
    if (strcmp(NameList[i], "\0") == 0)
      break;
    printf("%s\n", NameList[i]);
    display_string_custom(NameList[i], MENU_HEIGHT_SIZE, WHITE, BLACK, 5, Cursor.y_cur);
  }
  FileNum = i;

  printf("draw pic aaaaa\n");
  draw_select();
  SubMenuOk = true;
  printf("SubMenu:%d\n", SubMenuOk);
  printf("draw pic bbbbb\n");
}

void draw_music_menu(void)
{
  // 菜单复位
  MenuInfo = MUSIC_MENU;
  SelectIndex = 0;
  SubMenuOk = true;
  set_head_string("音乐文件菜单", WHITE, GOODBLUE);
  clear_screen_content();

  printf("1111");
  FRESULT res = f_scandir(MUSIC_FOLDER);
  printf("2222");
  if (res != FR_OK)
  {
    f_scandir_handle(res);
    display_string("文件扫描失败", MENU_HEIGHT_SIZE, WHITE, BLACK);
    SubMenuOk = false;
    return;
  }
  display_string("请选择文件:", MENU_HEIGHT_SIZE, WHITE, BLACK);
  uint8_t i = 0;
  for (i = 0; i < NAMELIST_NUM; i++)
  {
    if (strcmp(NameList[i], "\0") == 0)
      break;
    display_string_custom(NameList[i], MENU_HEIGHT_SIZE, WHITE, BLACK, 5, Cursor.y_cur);
  }
  FileNum = i;
  draw_select();
}

void draw_readme_menu(void)
{
  // 菜单复位
  MenuInfo = README_MENU;
  SelectIndex = 0;
  SubMenuOk = true;
  set_head_string("说明文件菜单", WHITE, GOODBLUE);
  clear_screen_content();

  display_string("请选择功能:", MENU_HEIGHT_SIZE, WHITE, BLACK);
  display_string_custom("按键说明", MENU_HEIGHT_SIZE, WHITE, BLACK, 5, Cursor.y_cur);
  display_string_custom("SD卡说明", MENU_HEIGHT_SIZE, WHITE, BLACK, 5, Cursor.y_cur);
  display_string_custom("音乐播放说明", MENU_HEIGHT_SIZE, WHITE, BLACK, 5, Cursor.y_cur);
  display_string_custom("文本查看说明", MENU_HEIGHT_SIZE, WHITE, BLACK, 5, Cursor.y_cur);
  draw_select();
}

void show_txt_content(void)
{
  MenuInfo = TXT_DETAIL_MENU;
  //  printf("SelectIndex:%d",SelectIndex);
  // 获取文件名
  char file_name[NAMELIST_MAX_LEN] = "\0";
  strcpy(file_name, NameList[SelectIndex]);
  char full_name[10 + NAMELIST_MAX_LEN] = "0:/txt/";
  strcat(full_name, file_name);
  //  printf("filename:%s",full_name);

  // 打开文件并读取
  UINT has_read = 0;
  FIL fp;
  // 清空Buffer和BufferPos,IsEndTxt
  memset(Buffer, '\0', sizeof(Buffer));
  BufferPos = 0;
  IsEndTxt = false;
  // 打开并读取文件
  FRESULT res = f_open(&fp, full_name, FA_READ);
  //  printf("FRESULT:%d",res);
  f_read(&fp, Buffer, 1024, &has_read);
  f_close(&fp);

  // 打印提示信息
  printf("hasread:%d\n", has_read);
  printf("%s\nd", Buffer);

  // 显示读取到的内容
  set_head_string(file_name, WHITE, GOODBLUE);
  clear_screen_content();
  if (has_read <= MAX_DISPLAY_TXT_LENGTH)
  {
    display_string(Buffer, 16, WHITE, BLACK);
  }
  else
  {
    char tmp_buffer[MAX_DISPLAY_TXT_LENGTH + 1] = {'\0'}; // 文本读取内容缓冲区
    strncpy(tmp_buffer, Buffer + BufferPos, MAX_DISPLAY_TXT_LENGTH);
    display_string(tmp_buffer, 16, WHITE, BLACK);
  }
}

void show_pic_content(void)
{
  printf("show pic content\n");
  MenuInfo = PIC_DETAIL_MENU;
  // 获取文件名
  char file_name[NAMELIST_MAX_LEN] = "\0";
  strcpy(file_name, NameList[SelectIndex]);
  char full_name[10 + NAMELIST_MAX_LEN] = "0:/pic/";
  strcat(full_name, file_name);

  set_head_string(file_name, WHITE, GOODBLUE);
  clear_screen_content();
  printf("pic 1111\n");
  uint8_t res = ai_load_picfile(full_name, 1, SCREEN_HEAD_HEIGHT + 1, PIC_SIZE, PIC_SIZE, 0);
  if (res == PIC_FORMAT_ERR)
  {
    display_string("图片格式不支持", 16, WHITE, BLACK);
  }
}

void play_song_content(void)
{
  MenuInfo = MUSIC_DETAIL_MENU;

  // 获取文件名
  char file_name[NAMELIST_MAX_LEN] = "\0";
  strcpy(file_name, NameList[SelectIndex]);
  memset(SongFullName, '\0', sizeof(SongFullName));
  strcpy(SongFullName, "0:/music/");
  strcat(SongFullName, file_name);
  //  printf("filename:%s",full_name);

  set_head_string(file_name, WHITE, GOODBLUE);
  clear_screen_content();
  display_string("正在播放音乐...", 16, WHITE, BLACK);

  IsPlay = true;
}

void show_readme_content(void)
{
  MenuInfo = README_DETAIL_MENU;

  switch (SelectIndex)
  {
  case 0:
    set_head_string("按键说明", WHITE, GOODBLUE);
    clear_screen_content();
    display_string("Key1:下一个选项，Key2:确认，Key3:返回或取消", 16, WHITE, BLACK);
    break;
  case 1:
    set_head_string("SD卡说明", WHITE, GOODBLUE);
    clear_screen_content();
    display_string("txt:文本文件目录，pic:图片文件目录，music:音乐文件目录", 16, WHITE, BLACK);
    break;
  case 2:
    set_head_string("音乐播放说明", WHITE, GOODBLUE);
    clear_screen_content();
  display_string("【矩阵键盘】1键:暂停/播放，2键:增大音量，3键:减少音量，4键:播放快进", 16, WHITE, BLACK);
    break;
  case 3:
    set_head_string("文本查看说明", WHITE, GOODBLUE);
    clear_screen_content();
    display_string("【矩阵键盘】1键:向前翻页，2键:向后翻页", 16, WHITE, BLACK);
    break;
  default:
    set_head_string("非法选项", WHITE, GOODBLUE);
    clear_screen_content();
    display_string("无说明", 16, WHITE, BLACK);
  }
}

// 选择指示
void clear_select(void)
{
  uint8_t y = 1 + SCREEN_HEAD_HEIGHT + MENU_HEIGHT_SIZE * (SelectIndex + 1);
  LCD_Fill(1, y, 3, y + MENU_HEIGHT_SIZE, BLACK);
}

void draw_select(void)
{
  uint8_t y = 1 + SCREEN_HEAD_HEIGHT + MENU_HEIGHT_SIZE * (SelectIndex + 1);
  LCD_Fill(1, y, 3, y + MENU_HEIGHT_SIZE, YELLOW);
}

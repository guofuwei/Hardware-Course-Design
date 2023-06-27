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

extern char SongFullName[10 + NAMELIST_MAX_LEN]; // �������ֲ���
extern bool IsPlay;

uint8_t private_cursor_check(void)
{
  if (Cursor.x_cur <= 0 || Cursor.y_cur <= 0)
  {
    drawString(1, 1, "��Ч��Cursorֵ", 16, RED, BLACK);
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

// ����ĺ������ڲ˵�����
void draw_main_menu(void)
{
  // �˵���λ
  KeyStatus = KEY_DENY;
  MenuInfo = MAIN_MENU;
  SelectIndex = 0;
  SubMenuOk = true;
  set_head_string("���˵�", WHITE, GOODBLUE);
  clear_screen_content();

  display_string("��ѡ����:", MENU_HEIGHT_SIZE, WHITE, BLACK);
  display_string_custom("�鿴�ı��ļ�", MENU_HEIGHT_SIZE, WHITE, BLACK, 5, Cursor.y_cur);
  display_string_custom("�鿴ͼƬ�ļ�", MENU_HEIGHT_SIZE, WHITE, BLACK, 5, Cursor.y_cur);
  display_string_custom("�鿴�����ļ�", MENU_HEIGHT_SIZE, WHITE, BLACK, 5, Cursor.y_cur);
  display_string_custom("�鿴˵��", MENU_HEIGHT_SIZE, WHITE, BLACK, 5, Cursor.y_cur);
  draw_select();
  KeyStatus = KEY_ACCEPT;
}

void draw_txt_menu(void)
{
  // �˵���λ
  MenuInfo = TXT_MENU;
  SelectIndex = 0;
  SubMenuOk = true;
  set_head_string("�ı��ļ��˵�", WHITE, GOODBLUE);
  clear_screen_content();

  //  printf("aaaa");
  FRESULT res = f_scandir(TXT_FOLDER);
  //  printf("bbbbb");
  if (res != FR_OK)
  {
    f_scandir_handle(res);
    printf("cccc");
    display_string("�ļ�ɨ��ʧ��", MENU_HEIGHT_SIZE, WHITE, BLACK);
    SubMenuOk = false;
    return;
  }
  display_string("��ѡ���ļ�:", MENU_HEIGHT_SIZE, WHITE, BLACK);
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
  // �˵���λ
  MenuInfo = PIC_MENU;
  SelectIndex = 0;
  SubMenuOk = true;
  set_head_string("ͼƬ�ļ��˵�", WHITE, GOODBLUE);
  clear_screen_content();

  FRESULT res = f_scandir(PIC_FOLDER);
  if (res != FR_OK)
  {
    f_scandir_handle(res);
    display_string("�ļ�ɨ��ʧ��", MENU_HEIGHT_SIZE, WHITE, BLACK);
    SubMenuOk = false;
    return;
  }
  display_string("��ѡ���ļ�:", MENU_HEIGHT_SIZE, WHITE, BLACK);
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
  // �˵���λ
  MenuInfo = MUSIC_MENU;
  SelectIndex = 0;
  SubMenuOk = true;
  set_head_string("�����ļ��˵�", WHITE, GOODBLUE);
  clear_screen_content();

  printf("1111");
  FRESULT res = f_scandir(MUSIC_FOLDER);
  printf("2222");
  if (res != FR_OK)
  {
    f_scandir_handle(res);
    display_string("�ļ�ɨ��ʧ��", MENU_HEIGHT_SIZE, WHITE, BLACK);
    SubMenuOk = false;
    return;
  }
  display_string("��ѡ���ļ�:", MENU_HEIGHT_SIZE, WHITE, BLACK);
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
  // �˵���λ
  MenuInfo = README_MENU;
  SelectIndex = 0;
  SubMenuOk = true;
  set_head_string("˵���ļ��˵�", WHITE, GOODBLUE);
  clear_screen_content();

  display_string("��ѡ����:", MENU_HEIGHT_SIZE, WHITE, BLACK);
  display_string_custom("����˵��", MENU_HEIGHT_SIZE, WHITE, BLACK, 5, Cursor.y_cur);
  display_string_custom("SD��˵��", MENU_HEIGHT_SIZE, WHITE, BLACK, 5, Cursor.y_cur);
  display_string_custom("���ֲ���˵��", MENU_HEIGHT_SIZE, WHITE, BLACK, 5, Cursor.y_cur);
  display_string_custom("�ı��鿴˵��", MENU_HEIGHT_SIZE, WHITE, BLACK, 5, Cursor.y_cur);
  draw_select();
}

void show_txt_content(void)
{
  MenuInfo = TXT_DETAIL_MENU;
  //  printf("SelectIndex:%d",SelectIndex);
  // ��ȡ�ļ���
  char file_name[NAMELIST_MAX_LEN] = "\0";
  strcpy(file_name, NameList[SelectIndex]);
  char full_name[10 + NAMELIST_MAX_LEN] = "0:/txt/";
  strcat(full_name, file_name);
  //  printf("filename:%s",full_name);

  // ���ļ�����ȡ
  UINT has_read = 0;
  FIL fp;
  // ���Buffer��BufferPos,IsEndTxt
  memset(Buffer, '\0', sizeof(Buffer));
  BufferPos = 0;
  IsEndTxt = false;
  // �򿪲���ȡ�ļ�
  FRESULT res = f_open(&fp, full_name, FA_READ);
  //  printf("FRESULT:%d",res);
  f_read(&fp, Buffer, 1024, &has_read);
  f_close(&fp);

  // ��ӡ��ʾ��Ϣ
  printf("hasread:%d\n", has_read);
  printf("%s\nd", Buffer);

  // ��ʾ��ȡ��������
  set_head_string(file_name, WHITE, GOODBLUE);
  clear_screen_content();
  if (has_read <= MAX_DISPLAY_TXT_LENGTH)
  {
    display_string(Buffer, 16, WHITE, BLACK);
  }
  else
  {
    char tmp_buffer[MAX_DISPLAY_TXT_LENGTH + 1] = {'\0'}; // �ı���ȡ���ݻ�����
    strncpy(tmp_buffer, Buffer + BufferPos, MAX_DISPLAY_TXT_LENGTH);
    display_string(tmp_buffer, 16, WHITE, BLACK);
  }
}

void show_pic_content(void)
{
  printf("show pic content\n");
  MenuInfo = PIC_DETAIL_MENU;
  // ��ȡ�ļ���
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
    display_string("ͼƬ��ʽ��֧��", 16, WHITE, BLACK);
  }
}

void play_song_content(void)
{
  MenuInfo = MUSIC_DETAIL_MENU;

  // ��ȡ�ļ���
  char file_name[NAMELIST_MAX_LEN] = "\0";
  strcpy(file_name, NameList[SelectIndex]);
  memset(SongFullName, '\0', sizeof(SongFullName));
  strcpy(SongFullName, "0:/music/");
  strcat(SongFullName, file_name);
  //  printf("filename:%s",full_name);

  set_head_string(file_name, WHITE, GOODBLUE);
  clear_screen_content();
  display_string("���ڲ�������...", 16, WHITE, BLACK);

  IsPlay = true;
}

void show_readme_content(void)
{
  MenuInfo = README_DETAIL_MENU;

  switch (SelectIndex)
  {
  case 0:
    set_head_string("����˵��", WHITE, GOODBLUE);
    clear_screen_content();
    display_string("Key1:��һ��ѡ�Key2:ȷ�ϣ�Key3:���ػ�ȡ��", 16, WHITE, BLACK);
    break;
  case 1:
    set_head_string("SD��˵��", WHITE, GOODBLUE);
    clear_screen_content();
    display_string("txt:�ı��ļ�Ŀ¼��pic:ͼƬ�ļ�Ŀ¼��music:�����ļ�Ŀ¼", 16, WHITE, BLACK);
    break;
  case 2:
    set_head_string("���ֲ���˵��", WHITE, GOODBLUE);
    clear_screen_content();
  display_string("��������̡�1��:��ͣ/���ţ�2��:����������3��:����������4��:���ſ��", 16, WHITE, BLACK);
    break;
  case 3:
    set_head_string("�ı��鿴˵��", WHITE, GOODBLUE);
    clear_screen_content();
    display_string("��������̡�1��:��ǰ��ҳ��2��:���ҳ", 16, WHITE, BLACK);
    break;
  default:
    set_head_string("�Ƿ�ѡ��", WHITE, GOODBLUE);
    clear_screen_content();
    display_string("��˵��", 16, WHITE, BLACK);
  }
}

// ѡ��ָʾ
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

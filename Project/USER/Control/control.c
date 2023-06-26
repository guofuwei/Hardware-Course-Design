#include "control.h"
#include "lcd_st7735.h"
#include "display.h"
#include "vs1053.h"
extern SYSINFO SysInfo;           // ϵͳ״̬
extern MENUINFO MenuInfo;         // ��ǰ����һ���˵�
extern KEYSTATUS KeyStatus;       // ����״̬
extern VS1053STATUS Vs1053Status; // VS1053״̬
extern uint8_t SelectIndex;       // ��ǰѡ�е�index
extern bool SubMenuOk;            // �Ӳ˵��Ƿ�ok
extern uint8_t FileNum;           // ɨ��������ļ���������ֹselectָʾ����
extern char Buffer[BUFFER_SIZE];  // �ı���ȡ���ݻ�����
extern int16_t BufferPos;
extern bool IsEndTxt;

extern long VS1053_CURRENTPOS;

// �������������в���
extern char SongFullName[10 + NAMELIST_MAX_LEN];
extern bool IsPlay;
extern bool IsStop;

// ��״̬������
void main_state_machine(uint16_t gpio_pin)
{
  if (SysInfo != SYS_OK)
  {
    set_head_string("ϵͳ���˳�", YELLOW, BLACK);
    clear_screen_content();
    KeyStatus = KEY_DENY;
  }
  else
  {
    switch (MenuInfo)
    {
    case LOADING_MENU:
      draw_main_menu();
      break;
    case MAIN_MENU:
      main_menu_handle(gpio_pin);
      break;
    case TXT_MENU:
      txt_menu_handle(gpio_pin);
      break;
    case PIC_MENU:
      pic_menu_handle(gpio_pin);
      break;
    case MUSIC_MENU:
      music_menu_handle(gpio_pin);
      break;
    case README_MENU:
      readme_menu_handle(gpio_pin);
      break;
    case TXT_DETAIL_MENU:
      txt_detail_handle(gpio_pin);
      break;
    case PIC_DETAIL_MENU:
      pic_detail_handle(gpio_pin);
      break;
    case MUSIC_DETAIL_MENU:
      printf("music_detail_menu\n");
      music_detail_handle(gpio_pin);
      break;
    case README_DETAIL_MENU:
      readme_detail_handle(gpio_pin);
      break;
    }
  }
}

// ����������
void main_menu_handle(uint16_t gpio_pin)
{
  // ���·���ť
  if (gpio_pin == KEY1_Pin)
  {
    clear_select();
    SelectIndex = SelectIndex + 1;
    if (SelectIndex >= 4)
    {
      SelectIndex = 0;
    }
    draw_select();
  }
  // ȷ����ť
  else if (gpio_pin == KEY2_Pin)
  {
    switch (SelectIndex)
    {
    case 0:
      draw_txt_menu();
      break;
    case 1:
      draw_pic_menu();
      break;
    case 2:
      draw_music_menu();
      break;
    case 3:
      draw_readme_menu();
      break;
    default:
      LED_RED_ON;
      HAL_Delay(100);
      LED_RED_OFF;
    }
  }
  // ���ػ���ȡ����ť�������˵����ã�
  else if (gpio_pin == KEY3_Pin)
  {
    LED_BLUE_ON;
    HAL_Delay(100);
    LED_BLUE_OFF;
  }
  else
  {
    LED_RED_ON;
    HAL_Delay(100);
    LED_RED_OFF;
  }
}

// �ı��Ӳ˵�������
void txt_menu_handle(uint16_t gpio_pin)
{
  if (gpio_pin == KEY1_Pin && SubMenuOk == true)
  {
    clear_select();
    SelectIndex = SelectIndex + 1;
    if (SelectIndex >= FileNum)
    {
      SelectIndex = 0;
    }
    draw_select();
  }
  else if (gpio_pin == KEY2_Pin && SubMenuOk == true)
  {
    //    printf("show txt content");
    show_txt_content();
  }
  else if (gpio_pin == KEY3_Pin)
  {
    draw_main_menu();
  }
  else
  {
    LED_RED_ON;
    HAL_Delay(100);
    LED_RED_OFF;
  }
}
// ͼƬ�Ӳ˵�������
void pic_menu_handle(uint16_t gpio_pin)
{
  printf("pic_menu_handle\n");
  if (gpio_pin == KEY1_Pin && SubMenuOk == true)
  {
    clear_select();
    SelectIndex = SelectIndex + 1;
    if (SelectIndex >= FileNum)
    {
      SelectIndex = 0;
    }
    draw_select();
  }
  else if (gpio_pin == KEY2_Pin && SubMenuOk == true)
  {
    show_pic_content();
  }
  else if (gpio_pin == KEY3_Pin)
  {
    draw_main_menu();
  }
  else
  {
    LED_RED_ON;
    HAL_Delay(100);
    LED_RED_OFF;
  }
}

// �����Ӳ˵�������
void music_menu_handle(uint16_t gpio_pin)
{
  if (gpio_pin == KEY1_Pin && SubMenuOk == true)
  {
    clear_select();
    SelectIndex = SelectIndex + 1;
    if (SelectIndex >= FileNum)
    {
      SelectIndex = 0;
    }
    draw_select();
  }
  else if (gpio_pin == KEY2_Pin && SubMenuOk == true)
  {
    play_song_content();
  }
  else if (gpio_pin == KEY3_Pin)
  {
    draw_main_menu();
  }
  else
  {
    LED_RED_ON;
    HAL_Delay(100);
    LED_RED_OFF;
  }
}

// �ı��ļ����鴦����
void txt_detail_handle(uint16_t gpio_pin)
{
  if (gpio_pin == KEY1_Pin && SubMenuOk == true)
  {
  }
  else if (gpio_pin == KEY2_Pin && SubMenuOk == true)
  {
  }
  else if (gpio_pin == KEY3_Pin)
  {
    draw_txt_menu();
  }
  else
  {
    LED_RED_ON;
    HAL_Delay(100);
    LED_RED_OFF;
  }
}
// ͼƬ�ļ����鴦����
void pic_detail_handle(uint16_t gpio_pin)
{
  if (gpio_pin == KEY1_Pin && SubMenuOk == true)
  {
  }
  else if (gpio_pin == KEY2_Pin && SubMenuOk == true)
  {
  }
  else if (gpio_pin == KEY3_Pin)
  {
    draw_pic_menu();
  }
  else
  {
    LED_RED_ON;
    HAL_Delay(100);
    LED_RED_OFF;
  }
}
//  �����ļ����鴦����
void music_detail_handle(uint16_t gpio_pin)
{
  if (gpio_pin == KEY1_Pin && SubMenuOk == true)
  {
  }
  else if (gpio_pin == KEY2_Pin && SubMenuOk == true)
  {
  }
  else if (gpio_pin == KEY3_Pin)
  {
    //    VS10XX_XCS(1);
    //    VS10XX_XDCS(1);
    draw_music_menu();
    HAL_Delay(100);
    Vs1053Status = VS1053_STOP;
    IsPlay = false;
    IsStop = false;
  }
  else
  {
    LED_RED_ON;
    HAL_Delay(100);
    LED_RED_OFF;
  }
}
// �������ֲ��ź���ͣ
void music_run_stop()
{
  if (!IsStop)
  {
    IsStop = 1;
    Vs1053Status = VS1053_STOP;
    IsPlay = false;
  }
  else
  {
    if (IsPlay)
    {
      IsPlay = false;
      Vs1053Status = VS1053_STOP;
    }
    else
    {
      IsPlay = true;
    }
  }
}
// ˵�����Ӳ˵�������
void readme_menu_handle(uint16_t gpio_pin)
{
  // ���·���ť
  if (gpio_pin == KEY1_Pin)
  {
    clear_select();
    SelectIndex = SelectIndex + 1;
    if (SelectIndex >= 3)
    {
      SelectIndex = 0;
    }
    draw_select();
  }
  // ȷ����ť
  else if (gpio_pin == KEY2_Pin)
  {
    show_readme_content();
  }
  // ���ػ���ȡ����ť�������˵����ã�
  else if (gpio_pin == KEY3_Pin)
  {
    draw_main_menu();
  }
  else
  {
    LED_RED_ON;
    HAL_Delay(100);
    LED_RED_OFF;
  }
}
// ˵�������鴦����
void readme_detail_handle(uint16_t gpio_pin)
{
  if (gpio_pin == KEY1_Pin && SubMenuOk == true)
  {
  }
  else if (gpio_pin == KEY2_Pin && SubMenuOk == true)
  {
  }
  else if (gpio_pin == KEY3_Pin)
  {
    draw_readme_menu();
  }
  else
  {
    LED_RED_ON;
    HAL_Delay(100);
    LED_RED_OFF;
  }
}

// �ı���ҳ����
// ���·�ҳ����
void txt_next_page(void)
{
  BufferPos = BufferPos + MAX_DISPLAY_TXT_LENGTH;

  if (BufferPos >= BUFFER_SIZE)
  {
    char tmp_str[100] = {'\0'};
    sprintf(tmp_str, "�ѳ�������������ǰ������Ϊ%dB", BUFFER_SIZE);
    clear_screen_content();
    display_string(tmp_str, 16, WHITE, BLACK);
    return;
  }
  if (Buffer[BufferPos] == '\0')
  {
    if (IsEndTxt == false)
    {
      IsEndTxt = true;
      clear_screen_content();
      display_string("�ѵ����ļ�ĩβ", 16, WHITE, BLACK);
    }
    else
    {
      BufferPos = BufferPos - MAX_DISPLAY_TXT_LENGTH;
    }
    return;
  }
  char tmp_buffer[MAX_DISPLAY_TXT_LENGTH + 1] = {'\0'}; // �ı���ȡ���ݻ�����
  strncpy(tmp_buffer, Buffer + BufferPos, MAX_DISPLAY_TXT_LENGTH);
  clear_screen_content();
  display_string(tmp_buffer, 16, WHITE, BLACK);
}
// ���Ϸ�ҳ����
void txt_prev_page(void)
{
  BufferPos = BufferPos - MAX_DISPLAY_TXT_LENGTH;

  if (BufferPos < 0)
  {
    BufferPos = BufferPos + MAX_DISPLAY_TXT_LENGTH;
    return;
  }
  char tmp_buffer[MAX_DISPLAY_TXT_LENGTH + 1] = {'\0'}; // �ı���ȡ���ݻ�����
  strncpy(tmp_buffer, Buffer + BufferPos, MAX_DISPLAY_TXT_LENGTH);
  clear_screen_content();
  display_string(tmp_buffer, 16, WHITE, BLACK);
}

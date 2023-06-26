/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONTROL_H
#define __CONTROL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f1xx_hal.h"

  // �Լ���ӵĶ���
  typedef enum
  {
    SYS_OK = 0,
    SYS_SD_ERROR
  } SYSINFO; // ϵͳ״̬

  typedef enum
  {
    LOADING_MENU,
    MAIN_MENU,
    TXT_MENU,
    PIC_MENU,
    MUSIC_MENU,
    README_MENU,
    TXT_DETAIL_MENU,
    PIC_DETAIL_MENU,
    MUSIC_DETAIL_MENU,
    README_DETAIL_MENU
  } MENUINFO; // �˵�����

  typedef enum
  {
    KEY_ACCEPT,
    KEY_DENY
  } KEYSTATUS; // ����״̬

  //  ��״̬��
  //  �˵�����
  void main_state_machine(uint16_t gpio_pin);
  void main_menu_handle(uint16_t gpio_pin);
  void txt_menu_handle(uint16_t gpio_pin);
  void pic_menu_handle(uint16_t gpio_pin);
  void music_menu_handle(uint16_t gpio_pin);
  void readme_menu_handle(uint16_t gpio_pin);

  // ������ʾ����
  void txt_detail_handle(uint16_t gpio_pin);
  void pic_detail_handle(uint16_t gpio_pin);
  void music_detail_handle(uint16_t gpio_pin);
  void readme_detail_handle(uint16_t gpio_pin);

  // ���ֲ��ſ���
  void music_run_stop(void);

  // �ı��Ķ��ķ�ҳ����
  void txt_next_page(void);
  void txt_prev_page(void);

#ifdef __cplusplus
}
#endif

#endif

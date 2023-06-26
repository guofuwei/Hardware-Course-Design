#ifndef __SPI_SDCARD_H
#define __SPI_SDCARD_H

#include "spi.h"
#include "sys.h"


/******************************************************************************************/
/* SD�� Ƭѡ ���� ���� */

#define SD_CS_GPIO_PORT                 GPIOA
#define SD_CS_GPIO_PIN                  GPIO_PIN_3
#define SD_CS_GPIO_CLK_ENABLE()         do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)             /* PA��ʱ��ʹ�� */


/* SD�� SPI �������� �궨��
 * �����ֲ��ʱ��, ������Ҫʵ��: spi1_read_write_byte �� spi1_set_speed
 * ��������������, SD�� SPIģʽ, ��ͨ������������, ʵ�ֶ�SD���Ĳ���.
 */
#define sd_spi_read_write_byte(x)       spi1_read_write_byte(x)             /* SD�� SPI��д���� */
#define sd_spi_speed_low()              spi1_set_speed(SPI_SPEED_256)       /* SD�� SPI����ģʽ */
#define sd_spi_speed_high()             spi1_set_speed(SPI_SPEED_2)         /* SD�� SPI����ģʽ */


/******************************************************************************************/
/* SD_CS �˿ڶ��� */
#define SD_CS(x)   do{ x ? \
                      HAL_GPIO_WritePin(SD_CS_GPIO_PORT, SD_CS_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(SD_CS_GPIO_PORT, SD_CS_GPIO_PIN, GPIO_PIN_RESET); \
                   }while(0)    /* SD_CS */

/******************************************************************************************/
/* SD�� ����ֵ���� */
#define SD_OK           0
#define SD_ERROR        1


/* SD�� ���Ͷ��� */
#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06

/* SD�� ����� */
#define CMD0    (0)             /* GO_IDLE_STATE */
#define CMD1    (1)             /* SEND_OP_COND (MMC) */
#define	ACMD41  (0x80 + 41)     /* SEND_OP_COND (SDC) */
#define CMD8    (8)             /* SEND_IF_COND */
#define CMD9    (9)             /* SEND_CSD */
#define CMD10   (10)            /* SEND_CID */
#define CMD12   (12)            /* STOP_TRANSMISSION */
#define ACMD13  (0x80 + 13)     /* SD_STATUS (SDC) */
#define CMD16   (16)            /* SET_BLOCKLEN */
#define CMD17   (17)            /* READ_SINGLE_BLOCK */
#define CMD18   (18)            /* READ_MULTIPLE_BLOCK */
#define CMD23   (23)            /* SET_BLOCK_COUNT (MMC) */
#define ACMD23  (0x80 + 23)     /* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24   (24)            /* WRITE_BLOCK */
#define CMD25   (25)            /* WRITE_MULTIPLE_BLOCK */
#define CMD32   (32)            /* ERASE_ER_BLK_START */
#define CMD33   (33)            /* ERASE_ER_BLK_END */
#define CMD38   (38)            /* ERASE */
#define CMD55   (55)            /* APP_CMD */
#define CMD58   (58)            /* READ_OCR */


/* SD�������� */
extern uint8_t  sd_type;


/* �������� */
static void sd_spi_init(void);                              /* SD SPI Ӳ����ʼ�� */
static void sd_deselect(void);                              /* SD��ȡ��ѡ�� */
static uint8_t sd_select(void);                             /* SD�� ѡ�� */
static uint8_t sd_wait_ready(void);                         /* �ȴ�SD��׼���� */
static uint8_t sd_get_response(uint8_t response);           /* �ȴ�SD����Ӧ */

static uint8_t sd_send_cmd(uint8_t cmd, uint32_t arg);      /* SD���������� */
static uint8_t sd_send_block(uint8_t *buf, uint8_t cmd);    /* SD������һ�����ݿ� */
static uint8_t sd_receive_data(uint8_t *buf, uint16_t len); /* SD������һ������ */

uint8_t sd_init(void);                                      /* SD ����ʼ�� */
uint32_t sd_get_sector_count(void);                         /* ��ȡSD������������(������) */
uint8_t sd_get_cid(uint8_t *cid_data);                      /* ��ȡSD����CID��Ϣ */
uint8_t sd_get_csd(uint8_t *csd_data);                      /* ��ȡSD����CSD��Ϣ */

uint8_t sd_read_disk(uint8_t *pbuf, uint32_t saddr, uint32_t cnt);  /* ��SD��(fatfs/usb����) */
uint8_t sd_write_disk(uint8_t *pbuf, uint32_t saddr, uint32_t cnt); /* дSD��(fatfs/usb����) */

#endif



























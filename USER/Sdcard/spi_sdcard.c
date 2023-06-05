/**
 ****************************************************************************************************
 * @file        spi_sdcard.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-06-02
 * @brief       SD�� ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� Mini STM32F103 ������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20200602
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "usart.h"
#include "spi_sdcard.h"


uint8_t  sd_type = 0; /* SD�������� */



/**
 * @brief       SD�� SPIӲ�����ʼ��
 *   @note      �ú������ֹSPI1�������������, ͨ�������������
 *              CS������, ��ֹ��������Ӱ��SD��������ͨ��
 * @param       ��
 * @retval      ��
 */
static void sd_spi_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    SD_CS_GPIO_CLK_ENABLE();    /* SD CS�� ʱ��ʹ�� */

    gpio_init_struct.Pin = SD_CS_GPIO_PIN;        
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP; 
    gpio_init_struct.Pull = GPIO_PULLUP; 
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SD_CS_GPIO_PORT, &gpio_init_struct);  /* SD CS ����ģʽ����(�������) */

    /* ����Ӳ������SD��������Ŀ���������� */
    /* ��ֹ��������(NRF/25Q64)��SD������Ӱ�� */
    __HAL_RCC_GPIOA_CLK_ENABLE();     /* PORTAʱ��ʹ�� */
    gpio_init_struct.Pin = GPIO_PIN_2|GPIO_PIN_4;        
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP; /* PA2.4 ���� */
    gpio_init_struct.Pull = GPIO_PULLUP; 
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);  /* SD CS ����ģʽ����(�������) */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);/* PA2.4 ���1 */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);/* PA2.4 ���1 */

    spi1_init();                /* ��ʼ��SPI1 */
    SD_CS(1);                   /* SD����ѡ�� */
}

/**
 * @brief       SD�� ȡ��ѡ��, �ͷ� SPI����
 * @param       ��
 * @retval      ��
 */
static void sd_deselect(void)
{
    SD_CS(1);                       /* ȡ��SD��Ƭѡ */
    sd_spi_read_write_byte(0xff);   /* �ṩ�����8��ʱ�� */
}

/**
 * @brief       SD�� ѡ��, ���ȴ���׼��OK
 * @param       ��
 * @retval      ѡ�н��
 *              SD_OK,      �ɹ�
 *              SD_ERROR,   ʧ��
 */
static uint8_t sd_select(void)
{
    SD_CS(0);

    if (sd_wait_ready() == 0)
    {
        return SD_OK;   /* �ȴ��ɹ� */
    }

    sd_deselect();
    return SD_ERROR;    /* �ȴ�ʧ�� */
}

/**
 * @brief       �ȴ���׼����
 * @param       ��
 * @retval      �ȴ����
 *              SD_OK,      �ɹ�
 *              SD_ERROR,   ʧ��
 */
static uint8_t sd_wait_ready(void)
{
    uint32_t t = 0;

    do
    {
        if (sd_spi_read_write_byte(0XFF) == 0XFF)
        {
            return SD_OK;   /* OK */
        }

        t++;
    } while (t < 0XFFFFFF); /* �ȴ� */

    return SD_ERROR;
}

/**
 * @brief       �ȴ�SD����Ӧ
 * @param       response : �ڴ��õ��Ļ�Ӧֵ
 * @retval      �ȴ����
 *              SD_OK,      �ɹ�
 *              SD_ERROR,   ʧ��
 */
static uint8_t sd_get_response(uint8_t response)
{
    uint16_t count = 0xFFFF;    /* �ȴ����� */

    while ((sd_spi_read_write_byte(0XFF) != response) && count)
    {
        count--;    /* �ȴ��õ�׼ȷ�Ļ�Ӧ */
    }

    if (count == 0)   /* �ȴ���ʱ */
    {
        return SD_ERROR;
    }

    return SD_OK;   /* ��ȷ��Ӧ */
}

/**
 * @brief       ��SD����ȡһ������
 *   @note      ��ȡ���Ȳ���, ��lenָ��
 *              ����һ��Ϊ512�ֽ�
 * @param       buf      : ���ݻ�����
 * @param       len      : Ҫ��ȡ�����ݳ���
 * @retval      ��ȡ���
 *              SD_OK,      �ɹ�
 *              SD_ERROR,   ʧ��
 */
static uint8_t sd_receive_data(uint8_t *buf, uint16_t len)
{
    if (sd_get_response(0xFE))   /* �ȴ�SD������������ʼ����0xFE */
    {
        return SD_ERROR;
    }

    while (len--)   /* ��ʼ�������� */
    {
        *buf = sd_spi_read_write_byte(0xFF);
        buf++;
    }

    /* ������2��αCRC��dummy CRC�� */
    sd_spi_read_write_byte(0xFF);
    sd_spi_read_write_byte(0xFF);

    return SD_OK;   /* ��ȡ�ɹ� */
}

/**
 * @brief       ��SD��д��һ�����ݰ�
 *   @note      д�볤�ȹ̶�Ϊ512�ֽ�!!
 * @param       buf      : ���ݻ�����
 * @param       cmd      : ָ��
* @retval      ���ͽ��
 *              SD_OK,      �ɹ�
 *              SD_ERROR,   ʧ��
 */
static uint8_t sd_send_block(uint8_t *buf, uint8_t cmd)
{
    uint16_t t;

    if (sd_wait_ready())            /* �ȴ�ready */
    {
        return SD_ERROR;
    }

    sd_spi_read_write_byte(cmd);    /* ���� CMD */

    if (cmd != 0XFD)                /* ���ǽ���ָ�� */
    {
        for (t = 0; t < 512; t++)
        {
            sd_spi_read_write_byte(buf[t]); /* �������� */
        }

        sd_spi_read_write_byte(0xFF);       /* ����crc */
        sd_spi_read_write_byte(0xFF);

        t = sd_spi_read_write_byte(0xFF);   /* ������Ӧ */

        if ((t & 0x1F) != 0x05)             /* ���ݰ�û�б����� */
        {
            return SD_ERROR;
        }
    }

    return SD_OK;   /* д��ɹ� */
}

/**
 * @brief       ��SD������һ������
 *   @note      ��ͬ�����CRCֵ�ڸú����ڲ��Զ�ȷ��
 * @param       cmd      : Ҫ���͵�����
 *   @note                 ���λΪ1 ��ʾACMD(Ӧ������)
 *                         ���λΪ0 ��ʾCMD(��ͨ����)
 * @param       arg      : ����Ĳ���
 * @retval      SD�����ص�������Ӧ
 */
static uint8_t sd_send_cmd(uint8_t cmd, uint32_t arg)
{
    uint8_t res;
    uint8_t retry = 0;
    uint8_t crc = 0X01; /* Ĭ�� CRC = ����CRC + ֹͣ */

    if (cmd & 0x80)     /* ACMD����ǰ, ��Ҫ�ȷ���һ�� CMD55 ���� */
    {
        cmd &= 0x7F;                    /* ������λ, ��ȡACMD���� */
        res = sd_send_cmd(CMD55, 0);    /* ����CMD55 */

        if (res > 1)
        {
            return res;
        }
    }

    if (cmd != CMD12)   /* �� cmd ������ ������������ʱ(CMD12), �ȴ���ѡ�гɹ� */
    {
        sd_deselect();  /* ȡ���ϴ�Ƭѡ */

        if (sd_select())
        {
            return 0xFF;/* ѡ��ʧ�� */
        }
    }

    /* ��������� */
    sd_spi_read_write_byte(cmd | 0x40); /* ��ʼ + ���������� */
    sd_spi_read_write_byte(arg >> 24);  /* ����[31 : 24] */
    sd_spi_read_write_byte(arg >> 16);  /* ����[23 : 16] */
    sd_spi_read_write_byte(arg >> 8);   /* ����[15 : 8] */
    sd_spi_read_write_byte(arg);        /* ����[7 : 0] */

    if (cmd == CMD0) crc = 0X95;        /* CMD0 ��CRCֵ�̶�Ϊ 0X95 */

    if (cmd == CMD8) crc = 0X87;        /* CMD8 ��CRCֵ�̶�Ϊ 0X87 */

    sd_spi_read_write_byte(crc);

    if (cmd == CMD12)   /* cmd ���� ������������(CMD12)ʱ */
    {
        sd_spi_read_write_byte(0xff);   /* CMD12 ����һ���ֽ� */
    }


    retry = 10; /* ���Դ��� */

    do          /* �ȴ���Ӧ����ʱ�˳� */
    {
        res = sd_spi_read_write_byte(0xFF);
    } while ((res & 0X80) && retry--);

    return res; /* ����״ֵ̬ */
}

/**
 * @brief       ��ȡSD����CID��Ϣ, ������������Ϣ��
 * @param       cid_data : ���CID���ڴ滺����(����16�ֽ�)
 * @retval      ��ȡ���
 *              SD_OK,      �ɹ�
 *              SD_ERROR,   ʧ��
 */
uint8_t sd_get_cid(uint8_t *cid_data)
{
    uint8_t res;

    res = sd_send_cmd(CMD10, 0);            /* ��CMD10�����CID */

    if (res == 0x00)
    {
        res = sd_receive_data(cid_data, 16);/* ����16���ֽڵ����� */
    }

    sd_deselect();  /* ȡ��Ƭѡ */

    return res;
}

/**
 * @brief       ��ȡSD����CSD��Ϣ�������������ٶ���Ϣ
 * @param       csd_data : ���CSD���ڴ滺����(����16�ֽ�)
 * @retval      ��ȡ���
 *              SD_OK,      �ɹ�
 *              SD_ERROR,   ʧ��
 */
uint8_t sd_get_csd(uint8_t *csd_data)
{
    uint8_t res;
    res = sd_send_cmd(CMD9, 0);             /* ��CMD9�����CSD */

    if (res == 0)
    {
        res = sd_receive_data(csd_data, 16);/* ����16���ֽڵ����� */
    }

    sd_deselect();  /* ȡ��Ƭѡ */

    return res;
}

/**
 * @brief       ��ȡSD������������(������)
 *   @note      ÿ�������ֽ�����Ϊ512, �������512, ���ʼ������ͨ��
 * @param       ��
 * @retval      SD������(������), �����ֽ���Ҫ * 512
 */
uint32_t sd_get_sector_count(void)
{
    uint8_t csd[16];
    uint32_t capacity;
    uint8_t n;
    uint16_t csize;

    if (sd_get_csd(csd) != 0)       /* ȡCSD��Ϣ������ڼ��������0 */
    {
        return 0;                   /* ����0��ʾ��ȡ����ʧ�� */
    }

    /* ���ΪSDHC�����������淽ʽ���� */
    if ((csd[0] & 0xC0) == 0x40)    /* V2.00�Ŀ� */
    {
        csize = csd[9] + ((uint16_t)csd[8] << 8) + ((uint32_t)(csd[7] & 63) << 16) + 1;
        capacity = (uint32_t)csize << 10;       /* �õ������� */
    }
    else     /* V1.XX�Ŀ� / MMC V3�� */
    {
        n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
        csize = (csd[8] >> 6) + ((uint16_t)csd[7] << 2) + ((uint16_t)(csd[6] & 3) << 10) + 1;
        capacity = (uint32_t)csize << (n - 9);  /* �õ������� */
    }

    return capacity;    /* ע�����ﷵ�ص�����������, ����ʵ���������ֽ���, �����ֽ��� �� * 512 */
}

/**
 * @brief       ��ʼ��SD��
 * @param       ��
 * @retval      ��ʼ�����
 *              SD_OK,      �ɹ�
 *              SD_ERROR,   ʧ��
 */
uint8_t sd_init(void)
{
    uint8_t res;        /*  ���SD���ķ���ֵ */
    uint16_t retry;     /*  �������г�ʱ���� */
    uint8_t ocr[4];
    uint16_t i;
    uint8_t cmd;

    sd_spi_init();      /* ��ʼ��IO */
    sd_spi_speed_low(); /* ���õ�����ģʽ */

    for (i = 0; i < 10; i++)
    {
        sd_spi_read_write_byte(0XFF);       /* ��������74������ */
    }

    retry = 20;

    do
    {
        res = sd_send_cmd(CMD0, 0);         /* ����IDLE״̬ */
    } while ((res != 0X01) && retry--);

    sd_type = 0;        /* Ĭ���޿� */

    if (res == 0X01)
    {
        if (sd_send_cmd(CMD8, 0x1AA) == 1)   /* SD V2.0 */
        {
            for (i = 0; i < 4; i++)
            {
                ocr[i] = sd_spi_read_write_byte(0XFF);  /* �õ�R7��32λ��Ӧ */
            }

            if (ocr[2] == 0X01 && ocr[3] == 0XAA)       /* ���Ƿ�֧��2.7~3.6V */
            {
                retry = 1000;

                do
                {
                    res = sd_send_cmd(ACMD41, 1UL << 30);   /* ����ACMD41 */
                } while (res && retry--);

                if (retry && sd_send_cmd(CMD58, 0) == 0)    /* ����SD2.0���汾��ʼ */
                {
                    for (i = 0; i < 4; i++)
                    {
                        ocr[i] = sd_spi_read_write_byte(0XFF);  /* �õ�OCRֵ */
                    }

                    if (ocr[0] & 0x40)          /* ���CCS */
                    {
                        sd_type = SD_TYPE_V2HC; /* V2.0 SDHC�� */
                    }
                    else
                    {
                        sd_type = SD_TYPE_V2;   /* V2.0 �� */
                    }
                }
            }
        }
        else     /* SD V1.x / MMC V3 */
        {
            res = sd_send_cmd(ACMD41, 0);   /* ����ACMD41 */
            retry = 1000;

            if (res <= 1)
            {
                sd_type = SD_TYPE_V1;   /* SD V1�� */
                cmd = ACMD41;           /* ������� ACMD41 */
            }
            else     /* MMC����֧�� ACMD41 ʶ�� */
            {
                sd_type = SD_TYPE_MMC;  /* MMC V3 */
                cmd = CMD1;             /* ������� CMD1 */
            }

            do
            {
                res = sd_send_cmd(cmd, 0);  /* ���� ACMD41 / CMD1 */
            } while (res && retry--);   /* �ȴ��˳�IDLEģʽ */

            if (retry == 0 || sd_send_cmd(CMD16, 512) != 0)
            {
                sd_type = SD_TYPE_ERR;  /* ����Ŀ� */
            }
        }
    }

    sd_deselect();          /* ȡ��Ƭѡ */
    sd_spi_speed_high();    /* ����ģʽ */

    if (sd_type)            /* ��������Ч, ��ʼ����� */
    {
        return SD_OK;
    }

    return res;
}

/**
 * @brief       ��SD��(fatfs/usb����)
 * @param       pbuf  : ���ݻ�����
 * @param       saddr : ������ַ
 * @param       cnt   : ��������
 * @retval      ��ȡ���
 *              SD_OK,      �ɹ�
 *              SD_ERROR,   ʧ��
 */
uint8_t sd_read_disk(uint8_t *pbuf, uint32_t saddr, uint32_t cnt)
{
    uint8_t res;
    long long lsaddr = saddr;

    if (sd_type != SD_TYPE_V2HC)
    {
        lsaddr <<= 9;   /* ת��Ϊ�ֽڵ�ַ */
    }

    if (cnt == 1)
    {
        res = sd_send_cmd(CMD17, lsaddr);       /* ������ */

        if (res == 0)   /* ָ��ͳɹ� */
        {
            res = sd_receive_data(pbuf, 512);   /* ����512���ֽ� */
        }
    }
    else
    {
        res = sd_send_cmd(CMD18, lsaddr);       /* ���������� */

        do
        {
            res = sd_receive_data(pbuf, 512);   /* ����512���ֽ� */
            pbuf += 512;
        } while (--cnt && res == 0);

        sd_send_cmd(CMD12, 0);  /* ����ֹͣ���� */
    }

    sd_deselect();  /* ȡ��Ƭѡ */
    return res;
}

/**
 * @brief       дSD��(fatfs/usb����)
 * @param       pbuf  : ���ݻ�����
 * @param       saddr : ������ַ
 * @param       cnt   : ��������
 * @retval      д����
 *              SD_OK,      �ɹ�
 *              SD_ERROR,   ʧ��
 */
uint8_t sd_write_disk(uint8_t *pbuf, uint32_t saddr, uint32_t cnt)
{
    uint8_t res;
    long long lsaddr = saddr;

    if (sd_type != SD_TYPE_V2HC)
    {
        lsaddr <<= 9;   /* ת��Ϊ�ֽڵ�ַ */
    }

    if (cnt == 1)
    {
        res = sd_send_cmd(CMD24, lsaddr);   /* ������ */

        if (res == 0)   /* ָ��ͳɹ� */
        {
            res = sd_send_block(pbuf, 0xFE);/* д512���ֽ� */
        }
    }
    else
    {
        if (sd_type != SD_TYPE_MMC)
        {
            sd_send_cmd(ACMD23, cnt);       /* ���� ACMD23 ָ�� */
        }

        res = sd_send_cmd(CMD25, lsaddr);   /* ���������� */

        if (res == 0)
        {
            do
            {
                res = sd_send_block(pbuf, 0xFC);    /* д��һ��block, 512���ֽ� */
                pbuf += 512;
            } while (--cnt && res == 0);

            res = sd_send_block(0, 0xFD);   /* д��һ��block, 512���ֽ� */
        }
    }

    sd_deselect();/* ȡ��Ƭѡ */
    return res;
}






















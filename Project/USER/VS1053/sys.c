#include "sys.h"


/**
 * @brief       �����ж�������ƫ�Ƶ�ַ
 * @param       baseaddr: ��ַ
 * @param       offset: ƫ����(������0, ����0X100�ı���)
 * @retval      ��
 */
void sys_nvic_set_vector_table(uint32_t baseaddr, uint32_t offset)
{
    /* ����NVIC��������ƫ�ƼĴ���,VTOR��9λ����,��[8:0]���� */
    SCB->VTOR = baseaddr | (offset & (uint32_t)0xFFFFFE00);
}

/**
 * @brief       ִ��: WFIָ��(ִ�����ָ�����͹���״̬, �ȴ��жϻ���)
 * @param       ��
 * @retval      ��
 */
void sys_wfi_set(void)
{
    __ASM volatile("wfi");
}

/**
 * @brief       �ر������ж�(���ǲ�����fault��NMI�ж�)
 * @param       ��
 * @retval      ��
 */
void sys_intx_disable(void)
{
    __ASM volatile("cpsid i");
}

/**
 * @brief       ���������ж�
 * @param       ��
 * @retval      ��
 */
void sys_intx_enable(void)
{
    __ASM volatile("cpsie i");
}

/**
 * @brief       ����ջ����ַ
 * @note        ���ĺ�X, ����MDK��, ʵ����û�����
 * @param       addr: ջ����ַ
 * @retval      ��
 */
void sys_msr_msp(uint32_t addr)
{
    __set_MSP(addr);  /* ����ջ����ַ */
}

/**
 * @brief       �������ģʽ
 * @param       ��
 * @retval      ��
 */
void sys_standby(void)
{
    __HAL_RCC_PWR_CLK_ENABLE();    /* ʹ�ܵ�Դʱ�� */
    SET_BIT(PWR->CR, PWR_CR_PDDS); /* �������ģʽ */
}

/**
 * @brief       ϵͳ��λ
 * @param       ��
 * @retval      ��
 */
void sys_soft_reset(void)
{
    NVIC_SystemReset();
}

/**
 * @brief       ϵͳʱ�ӳ�ʼ������
 * @param       plln: PLL��Ƶϵ��(PLL��Ƶ), ȡֵ��Χ: 2~16
                �ж�������λ��������ʱ�Ѿ���SystemInit()�г�ʼ��
 * @retval      ��
 */
void sys_stm32_clock_init(uint32_t plln)
{
    HAL_StatusTypeDef ret = HAL_ERROR;
    RCC_OscInitTypeDef rcc_oscinitstructure;
    RCC_ClkInitTypeDef rcc_clkinitstructure;

    rcc_oscinitstructure.OscillatorType = RCC_OSCILLATORTYPE_HSE;       /* �ⲿ����ʱ��ʹ��HSEON */
    rcc_oscinitstructure.HSEState = RCC_HSE_ON;                         /* ��HSE */
    rcc_oscinitstructure.HSEPredivValue = RCC_HSE_PREDIV_DIV1;          /* HSEԤ��Ƶ */
    rcc_oscinitstructure.PLL.PLLState = RCC_PLL_ON;                     /* ��PLL */
    rcc_oscinitstructure.PLL.PLLSource = RCC_PLLSOURCE_HSE;             /* PLLʱ��Դѡ��HSE */
    rcc_oscinitstructure.PLL.PLLMUL = plln;                             /* ��PLL��Ƶ���� */
    ret = HAL_RCC_OscConfig(&rcc_oscinitstructure);                     /* ��ʼ�� */

    if (ret != HAL_OK)
    {
        while (1);                                                      /* ʱ�ӳ�ʼ��72Mʧ�ܣ�֮��ĳ��򽫿����޷�����ִ�У���������������Լ��Ĵ��� */
    }

    /* ѡ��PLL��Ϊϵͳʱ��Դ��������HCLK,PCLK1��PCLK2 */
    rcc_clkinitstructure.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    rcc_clkinitstructure.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;        /* ����ϵͳʱ��ʱ��ԴΪPLL */
    rcc_clkinitstructure.AHBCLKDivider = RCC_SYSCLK_DIV1;               /* AHB��Ƶϵ��Ϊ1 */
    rcc_clkinitstructure.APB1CLKDivider = RCC_HCLK_DIV2;                /* APB1��Ƶϵ��Ϊ2 */
    rcc_clkinitstructure.APB2CLKDivider = RCC_HCLK_DIV1;                /* APB2��Ƶϵ��Ϊ1 */
    ret = HAL_RCC_ClockConfig(&rcc_clkinitstructure, FLASH_LATENCY_2);  /* ͬʱ����FLASH��ʱ����Ϊ2WS��Ҳ����3��CPU���� */

    if (ret != HAL_OK)
    {
        while (1);                                                      /* ʱ�ӳ�ʼ��72Mʧ�ܣ�֮��ĳ��򽫿����޷�����ִ�У���������������Լ��Ĵ��� */
    }
}


/**
 * @brief       GPIOͨ������
 * @param       p_gpiox: GPIOA~GPIOG, GPIOָ��
 * @param       pinx: 0X0000~0XFFFF, ����λ��, ÿ��λ����һ��IO, ��0λ����Px0, ��1λ����Px1, ��������. ����0X0101, ����ͬʱ����Px0��Px8.
 *   @arg       SYS_GPIO_PIN0~SYS_GPIO_PIN15, 1<<0 ~ 1<<15
 *
 * @param       mode: 0~3; ģʽѡ��, ��������:
 *   @arg       SYS_GPIO_MODE_IN,  0, ����ģʽ(ϵͳ��λĬ��״̬)
 *   @arg       SYS_GPIO_MODE_OUT, 1, ���ģʽ
 *   @arg       SYS_GPIO_MODE_AF,  2, ���ù���ģʽ
 *   @arg       SYS_GPIO_MODE_AIN, 3, ģ������ģʽ
 *
 * @param       otype: 0 / 1; �������ѡ��, ��������:
 *   @arg       SYS_GPIO_OTYPE_PP, 0, �������
 *   @arg       SYS_GPIO_OTYPE_OD, 1, ��©���
 *
 * @param       ospeed: 0~2; ����ٶ�, ��������(ע��: ����Ϊ0!!):
 *   @arg       SYS_GPIO_SPEED_LOW,  2, ����
 *   @arg       SYS_GPIO_SPEED_MID,  1, ����
 *   @arg       SYS_GPIO_SPEED_HIGH, 3, ����
 *
 * @param       pupd: 0~3: ����������, ��������:
 *   @arg       SYS_GPIO_PUPD_NONE, 0, ����������
 *   @arg       SYS_GPIO_PUPD_PU,   1, ����
 *   @arg       SYS_GPIO_PUPD_PD,   2, ����
 *   @arg       SYS_GPIO_PUPD_RES,  3, ����
 *
 * @note:       ע��:
 *              1, ������ģʽ(��ͨ����/ģ������)��, otype �� ospeed ������Ч!!
 *              2, �����ģʽ��, pupd ������Ч!!(��©����޷�ʹ���ڲ���������!!)
 * @retval      ��
 */
void sys_gpio_set(GPIO_TypeDef *p_gpiox, uint16_t pinx, uint32_t mode, uint32_t otype, uint32_t ospeed, uint32_t pupd)
{
    uint32_t pinpos = 0, pos = 0, curpin = 0;
    uint32_t config = 0;        /* ���ڱ���ĳһ��IO������(CNF[1:0] + MODE[1:0]),ֻ���������4λ */

    for (pinpos = 0; pinpos < 16; pinpos++)
    {
        pos = 1 << pinpos;      /* һ����λ��� */
        curpin = pinx & pos;    /* ��������Ƿ�Ҫ���� */

        if (curpin == pos)      /* ��Ҫ���� */
        {
            config = 0;         /* bit0~3������Ϊ0, ��CNF[1:0] = 0; MODE[1:0] = 0;  Ĭ����ģ������ģʽ */

            if ((mode == 0X01) || (mode == 0X02))   /* �������ͨ���ģʽ/���ù���ģʽ */
            {
                config = ospeed & 0X03;             /* ����bit0/1 MODE[1:0] = 2/1/3 �ٶȲ��� */
                config |= (otype & 0X01) << 2;      /* ����bit2   CNF[0]    = 0/1   ����/��©��� */
                config |= (mode - 1) << 3;          /* ����bit3   CNF[1]    = 0/1   ��ͨ/������� */
            }
            else if (mode == 0)     /* �������ͨ����ģʽ */
            {
                if (pupd == 0)   /* ����������,����������ģʽ */
                {
                    config = 1 << 2;               /* ����bit2/3 CNF[1:0] = 01   ��������ģʽ */
                }
                else
                {
                    config = 1 << 3;                            /* ����bit2/3 CNF[1:0] = 10   ����������ģʽ */
                    p_gpiox->ODR &= ~(1 << pinpos);             /* ���ԭ�������� */
                    p_gpiox->ODR |= (pupd & 0X01) << pinpos;    /* ����ODR = 0/1 ����/���� */
                }
            }

            /* ����IO��λ�� ����CRL / CRH�Ĵ��� */
            if (pinpos <= 7)
            {
                p_gpiox->CRL &= ~(0X0F << (pinpos * 4));        /* ���ԭ�������� */
                p_gpiox->CRL |= config << (pinpos * 4);         /* ����CNFx[1:0] �� MODEx[1:0], x = pinpos = 0~7 */
            }
            else
            {
                p_gpiox->CRH &= ~(0X0F << ((pinpos - 8) * 4));  /* ���ԭ�������� */
                p_gpiox->CRH |= config << ((pinpos - 8) * 4);   /* ����CNFx[1:0] �� MODEx[1:0], x = pinpos = 8~15 */

            }
        }
    }
}

/**
 * @brief       ����GPIOĳ�����ŵ����״̬
 * @param       p_gpiox: GPIOA~GPIOG, GPIOָ��
 * @param       0X0000~0XFFFF, ����λ��, ÿ��λ����һ��IO, ��0λ����Px0, ��1λ����Px1, ��������. ����0X0101, ����ͬʱ����Px0��Px8.
 *   @arg       SYS_GPIO_PIN0~SYS_GPIO_PIN15, 1<<0 ~ 1<<15
 * @param       status: 0/1, ����״̬(�����λ��Ч), ��������:
 *   @arg       0, ����͵�ƽ
 *   @arg       1, ����ߵ�ƽ
 * @retval      ��
 */
void sys_gpio_pin_set(GPIO_TypeDef *p_gpiox, uint16_t pinx, uint8_t status)
{
    if (status & 0X01)
    {
        p_gpiox->BSRR |= pinx;  /* ����GPIOx��pinxΪ1 */
    }
    else
    {
        p_gpiox->BSRR |= (uint32_t)pinx << 16;  /* ����GPIOx��pinxΪ0 */
    }
}

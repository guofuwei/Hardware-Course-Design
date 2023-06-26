#include "sys.h"


/**
 * @brief       设置中断向量表偏移地址
 * @param       baseaddr: 基址
 * @param       offset: 偏移量(必须是0, 或者0X100的倍数)
 * @retval      无
 */
void sys_nvic_set_vector_table(uint32_t baseaddr, uint32_t offset)
{
    /* 设置NVIC的向量表偏移寄存器,VTOR低9位保留,即[8:0]保留 */
    SCB->VTOR = baseaddr | (offset & (uint32_t)0xFFFFFE00);
}

/**
 * @brief       执行: WFI指令(执行完该指令进入低功耗状态, 等待中断唤醒)
 * @param       无
 * @retval      无
 */
void sys_wfi_set(void)
{
    __ASM volatile("wfi");
}

/**
 * @brief       关闭所有中断(但是不包括fault和NMI中断)
 * @param       无
 * @retval      无
 */
void sys_intx_disable(void)
{
    __ASM volatile("cpsid i");
}

/**
 * @brief       开启所有中断
 * @param       无
 * @retval      无
 */
void sys_intx_enable(void)
{
    __ASM volatile("cpsie i");
}

/**
 * @brief       设置栈顶地址
 * @note        左侧的红X, 属于MDK误报, 实际是没问题的
 * @param       addr: 栈顶地址
 * @retval      无
 */
void sys_msr_msp(uint32_t addr)
{
    __set_MSP(addr);  /* 设置栈顶地址 */
}

/**
 * @brief       进入待机模式
 * @param       无
 * @retval      无
 */
void sys_standby(void)
{
    __HAL_RCC_PWR_CLK_ENABLE();    /* 使能电源时钟 */
    SET_BIT(PWR->CR, PWR_CR_PDDS); /* 进入待机模式 */
}

/**
 * @brief       系统软复位
 * @param       无
 * @retval      无
 */
void sys_soft_reset(void)
{
    NVIC_SystemReset();
}

/**
 * @brief       系统时钟初始化函数
 * @param       plln: PLL倍频系数(PLL倍频), 取值范围: 2~16
                中断向量表位置在启动时已经在SystemInit()中初始化
 * @retval      无
 */
void sys_stm32_clock_init(uint32_t plln)
{
    HAL_StatusTypeDef ret = HAL_ERROR;
    RCC_OscInitTypeDef rcc_oscinitstructure;
    RCC_ClkInitTypeDef rcc_clkinitstructure;

    rcc_oscinitstructure.OscillatorType = RCC_OSCILLATORTYPE_HSE;       /* 外部高速时钟使能HSEON */
    rcc_oscinitstructure.HSEState = RCC_HSE_ON;                         /* 打开HSE */
    rcc_oscinitstructure.HSEPredivValue = RCC_HSE_PREDIV_DIV1;          /* HSE预分频 */
    rcc_oscinitstructure.PLL.PLLState = RCC_PLL_ON;                     /* 打开PLL */
    rcc_oscinitstructure.PLL.PLLSource = RCC_PLLSOURCE_HSE;             /* PLL时钟源选择HSE */
    rcc_oscinitstructure.PLL.PLLMUL = plln;                             /* 主PLL倍频因子 */
    ret = HAL_RCC_OscConfig(&rcc_oscinitstructure);                     /* 初始化 */

    if (ret != HAL_OK)
    {
        while (1);                                                      /* 时钟初始化72M失败，之后的程序将可能无法正常执行，可以在这里加入自己的处理 */
    }

    /* 选中PLL作为系统时钟源并且配置HCLK,PCLK1和PCLK2 */
    rcc_clkinitstructure.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    rcc_clkinitstructure.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;        /* 设置系统时钟时钟源为PLL */
    rcc_clkinitstructure.AHBCLKDivider = RCC_SYSCLK_DIV1;               /* AHB分频系数为1 */
    rcc_clkinitstructure.APB1CLKDivider = RCC_HCLK_DIV2;                /* APB1分频系数为2 */
    rcc_clkinitstructure.APB2CLKDivider = RCC_HCLK_DIV1;                /* APB2分频系数为1 */
    ret = HAL_RCC_ClockConfig(&rcc_clkinitstructure, FLASH_LATENCY_2);  /* 同时设置FLASH延时周期为2WS，也就是3个CPU周期 */

    if (ret != HAL_OK)
    {
        while (1);                                                      /* 时钟初始化72M失败，之后的程序将可能无法正常执行，可以在这里加入自己的处理 */
    }
}


/**
 * @brief       GPIO通用设置
 * @param       p_gpiox: GPIOA~GPIOG, GPIO指针
 * @param       pinx: 0X0000~0XFFFF, 引脚位置, 每个位代表一个IO, 第0位代表Px0, 第1位代表Px1, 依次类推. 比如0X0101, 代表同时设置Px0和Px8.
 *   @arg       SYS_GPIO_PIN0~SYS_GPIO_PIN15, 1<<0 ~ 1<<15
 *
 * @param       mode: 0~3; 模式选择, 设置如下:
 *   @arg       SYS_GPIO_MODE_IN,  0, 输入模式(系统复位默认状态)
 *   @arg       SYS_GPIO_MODE_OUT, 1, 输出模式
 *   @arg       SYS_GPIO_MODE_AF,  2, 复用功能模式
 *   @arg       SYS_GPIO_MODE_AIN, 3, 模拟输入模式
 *
 * @param       otype: 0 / 1; 输出类型选择, 设置如下:
 *   @arg       SYS_GPIO_OTYPE_PP, 0, 推挽输出
 *   @arg       SYS_GPIO_OTYPE_OD, 1, 开漏输出
 *
 * @param       ospeed: 0~2; 输出速度, 设置如下(注意: 不能为0!!):
 *   @arg       SYS_GPIO_SPEED_LOW,  2, 低速
 *   @arg       SYS_GPIO_SPEED_MID,  1, 中速
 *   @arg       SYS_GPIO_SPEED_HIGH, 3, 高速
 *
 * @param       pupd: 0~3: 上下拉设置, 设置如下:
 *   @arg       SYS_GPIO_PUPD_NONE, 0, 不带上下拉
 *   @arg       SYS_GPIO_PUPD_PU,   1, 上拉
 *   @arg       SYS_GPIO_PUPD_PD,   2, 下拉
 *   @arg       SYS_GPIO_PUPD_RES,  3, 保留
 *
 * @note:       注意:
 *              1, 在输入模式(普通输入/模拟输入)下, otype 和 ospeed 参数无效!!
 *              2, 在输出模式下, pupd 参数无效!!(开漏输出无法使用内部上拉电阻!!)
 * @retval      无
 */
void sys_gpio_set(GPIO_TypeDef *p_gpiox, uint16_t pinx, uint32_t mode, uint32_t otype, uint32_t ospeed, uint32_t pupd)
{
    uint32_t pinpos = 0, pos = 0, curpin = 0;
    uint32_t config = 0;        /* 用于保存某一个IO的设置(CNF[1:0] + MODE[1:0]),只用了其最低4位 */

    for (pinpos = 0; pinpos < 16; pinpos++)
    {
        pos = 1 << pinpos;      /* 一个个位检查 */
        curpin = pinx & pos;    /* 检查引脚是否要设置 */

        if (curpin == pos)      /* 需要设置 */
        {
            config = 0;         /* bit0~3都设置为0, 即CNF[1:0] = 0; MODE[1:0] = 0;  默认是模拟输入模式 */

            if ((mode == 0X01) || (mode == 0X02))   /* 如果是普通输出模式/复用功能模式 */
            {
                config = ospeed & 0X03;             /* 设置bit0/1 MODE[1:0] = 2/1/3 速度参数 */
                config |= (otype & 0X01) << 2;      /* 设置bit2   CNF[0]    = 0/1   推挽/开漏输出 */
                config |= (mode - 1) << 3;          /* 设置bit3   CNF[1]    = 0/1   普通/复用输出 */
            }
            else if (mode == 0)     /* 如果是普通输入模式 */
            {
                if (pupd == 0)   /* 不带上下拉,即浮空输入模式 */
                {
                    config = 1 << 2;               /* 设置bit2/3 CNF[1:0] = 01   浮空输入模式 */
                }
                else
                {
                    config = 1 << 3;                            /* 设置bit2/3 CNF[1:0] = 10   上下拉输入模式 */
                    p_gpiox->ODR &= ~(1 << pinpos);             /* 清除原来的设置 */
                    p_gpiox->ODR |= (pupd & 0X01) << pinpos;    /* 设置ODR = 0/1 下拉/上拉 */
                }
            }

            /* 根据IO口位置 设置CRL / CRH寄存器 */
            if (pinpos <= 7)
            {
                p_gpiox->CRL &= ~(0X0F << (pinpos * 4));        /* 清除原来的设置 */
                p_gpiox->CRL |= config << (pinpos * 4);         /* 设置CNFx[1:0] 和 MODEx[1:0], x = pinpos = 0~7 */
            }
            else
            {
                p_gpiox->CRH &= ~(0X0F << ((pinpos - 8) * 4));  /* 清除原来的设置 */
                p_gpiox->CRH |= config << ((pinpos - 8) * 4);   /* 设置CNFx[1:0] 和 MODEx[1:0], x = pinpos = 8~15 */

            }
        }
    }
}

/**
 * @brief       设置GPIO某个引脚的输出状态
 * @param       p_gpiox: GPIOA~GPIOG, GPIO指针
 * @param       0X0000~0XFFFF, 引脚位置, 每个位代表一个IO, 第0位代表Px0, 第1位代表Px1, 依次类推. 比如0X0101, 代表同时设置Px0和Px8.
 *   @arg       SYS_GPIO_PIN0~SYS_GPIO_PIN15, 1<<0 ~ 1<<15
 * @param       status: 0/1, 引脚状态(仅最低位有效), 设置如下:
 *   @arg       0, 输出低电平
 *   @arg       1, 输出高电平
 * @retval      无
 */
void sys_gpio_pin_set(GPIO_TypeDef *p_gpiox, uint16_t pinx, uint8_t status)
{
    if (status & 0X01)
    {
        p_gpiox->BSRR |= pinx;  /* 设置GPIOx的pinx为1 */
    }
    else
    {
        p_gpiox->BSRR |= (uint32_t)pinx << 16;  /* 设置GPIOx的pinx为0 */
    }
}

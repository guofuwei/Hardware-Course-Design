#include "w25qxx.h" 


//W25Q系列芯片型号返回值	   
#define   W25Q80  	0XEF13 	
#define   W25Q16  	0XEF14
#define   W25Q32  	0XEF15
#define   W25Q64 	  0XEF16
#define   W25Q128	  0XEF17
#define   W25Q256   0XEF18
//#define   W25Qxx    65519   // 很多时候重新下载后读出的都是65519
  
  
  
/*****************************************************************************
 ** 本地变量
 *****************************************************************************/
struct 
{
    uint8_t  InitOK;        // 初始化状态   0:失败, 1:成功
    uint8_t  Type[20];      // 型号
    uint16_t StartupTimes;  // 记录启动次数
    uint8_t  GBKStorageOK;  // GBK字库就绪标志, 同时写保护,防止字库被错误写履盖
}xW25Qxx;
 
 
 
/*****************************************************************************
 ** 内部函数
****************************************************************************/
// 5个基本功能
static uint8_t    SPISendByte(uint8_t d);                           // 5_1    字节读写
static void  WriteEnable(void) ;                                    // 5_2    写使能
static void  WaitReady(void) ;                                      // 5_3    等待空闲
static void  EraseSector(uint32_t addr);                            // 5_4    擦扇区
static void  WriteSector(uint32_t addr, uint8_t* p, uint16_t num);  // 5_5    写扇区
// 测试
static void  ReadID(void);

static void  SPIInit(void);
//#define SPIInit()
  
#define W25QXX_SPIx       SPI1            //使用寄存器SPI函数需要此定义，方便修改SPI口。                        
/****************************************************************************** 
  * spi初始化
  * 只要是w25qxx系列，都适用
  * 注意，设备的SPI初始化，必须做成一个单独的函数。因为STM32在设备间切换时，SPI的参数也得重新调换
*/
static void  SPIInit(void)
{     
    W25QXX_SPIx -> CR1  = 0x1<<0;         // CPHA:时钟相位,0x1=在第2个时钟边沿进行数据采样
    W25QXX_SPIx -> CR1 |= 0x1<<1;         // CPOL:时钟极性,0x1=空闲状态时，SCK保持高电平
    W25QXX_SPIx -> CR1 |= 0x1<<2;         // 主从模式:         1 = 主配置
    W25QXX_SPIx -> CR1 |= 0x0<<3;         // 波特率控制[5:3]:  0 = fPCLK /2
    W25QXX_SPIx -> CR1 |= 0x0<<7;         // 帧格式:           0 = 先发送MSB
    W25QXX_SPIx -> CR1 |= 0x1<<9;         // 软件从器件管理 :  1 = 使能软件从器件管理(软件NSS)
    W25QXX_SPIx -> CR1 |= 0x1<<8;         // 内部从器件选择,根据9位设置(失能内部NSS)
    W25QXX_SPIx -> CR1 |= 0x0<<11;        // 数据帧格式,       0 = 8位
    W25QXX_SPIx -> CR1 |= 0x1<<6;         // SPI使能           1 = 使能外设
         
	  HAL_Delay (100);    
}

// 其实就是CubeMX生成的MX_SPI1_Init();已用寄存器方式写好，直接延用即可；
// 也可用MX_SPIx_Inti()函数代替，当多个从SPI设备用同种SPI方式，不必每个
// 设备切换时都重新设置SPI_Init，只要切换NSS，让当前设备NSS为低即可
// 故将其注释掉，并定义空SPIInit()，这样可不必修改下面代码。
 
	
/***************************************************************************** 
  * @Fun    W25Qxx_ReadID
  * @brief  读取芯片型号,用于判断通讯状况         
  */        
static void ReadID()
{   
    uint16_t W25QxxType = 0 ; 	
    // 1: 读取芯片型号, 判断联接状况    
    W25QX_NSS_LOW; 
    SPISendByte(0x90);  // 发送读取ID命令,命令分两分,第一字节是命令,第四字节是0
    SPISendByte(0x00);
    SPISendByte(0x00);
    SPISendByte(0x00);  // 第四字节必节须是 0h      
    W25QxxType  = (SPISendByte(0xFF))<<8;   // uint16_t W25QxxType  在本文件定义,全局
    W25QxxType |= SPISendByte(0xFF);    
	  W25QX_NSS_HIGH;  
    
    xW25Qxx.InitOK  =1;    
    switch (W25QxxType)    {                        
        case W25Q16:            
            sprintf((char*)xW25Qxx.Type, "%s", "W25Q16");               
            break;        
        case W25Q32:
            sprintf((char*)xW25Qxx.Type, "%s", "W25Q32");              
            break;
        case W25Q64:
            sprintf((char*)xW25Qxx.Type, "%s", "W25Q64");              
            break;
    	  case W25Q128:
            sprintf((char*)xW25Qxx.Type, "%s","W25Q128");              
    		    break;
    	  case W25Q256:
            sprintf((char*)xW25Qxx.Type, "%s", "W25Q256");           // 注意:W25Q256的地址是4字节               
    		    break;		
    	  default:             
            sprintf((char*)xW25Qxx.Type, "%s", "Flash设备失败 !!!");              
            xW25Qxx.InitOK =0;
            printf("读取到的错误型号数据：%d\r\n",W25QxxType);
    		    break;
    }        
   
	// 2:读取存储数据, 增加启动次数记录      
    if(xW25Qxx.InitOK  == 1 ) {   
		uint32_t Addr = 0x00;            // 数据地址,  W25Q128最大地址:0X0100 0000
        uint8_t d[4]={0};                 // 数据缓存， 0x0000:标志0xEE, 0x0001:标志0X00,     0x0002:数据高位, 0x0003:数据低位
        uint16_t startFlag  = 0;               // 标志
        uint16_t startNum   = 0;                  // 启动次数
        
        W25qxx_ReadBuffer(  Addr, d, 4);       // 读取4个字节数据
		    startFlag = (d[0]<<8) | d[1];   // 标志
        startNum  = (d[2]<<8) | d[3];   // 启动次数        
        
        if(startFlag!=0xEE00) {         // 没有旧记录	
            startNum=1;            
            d[2]=0;
            d[3]=1;                
        }
        else{
            startNum++;              // 成功读取数据， 次数增加1                    
            d[2]=(uint8_t)(startNum>>8);
            d[3]=(uint8_t)startNum;     
            // d[2]=(uint8_t)0;           // 回复启动次数 = 0, 取消这两行即可，
            // d[3]=(uint8_t)0;           // 编译烧录后，得重新注释，编译再次烧录    
        }
        d[0]=0xEE;
        d[1]=0x00;
		
		// 下载后，f=0xEE00时，来到这里就莫名复位
        W25qxx_WriteBuffer( Addr, d, 4);  
		    xW25Qxx.StartupTimes = startNum; 
        
        //char c[45]=" ";
        printf ("型号:%s ,第%d次用\r\n", xW25Qxx.Type, startNum); 
    }
    else 
    {    // 检测W25Qxx失败	  
        printf("型号错误，不可用!\r"); 
     //printf("尝试复位芯片...\r"); 
		 //System_Reset ();
    }          
}



/******************************************************************************
 * @Function        W25Qxx_Read  全局 4_3
 * @Description     读取数据
 *                                 
 * @Input           uint8_t   *p    读出的数值存放位置    
 *                  uint32_t  addr  读取地址
 *                  uint16_t  num   连续读取的字节数 
**/
void W25qxx_ReadBuffer( uint32_t addr, uint8_t *p, uint16_t num)
{
    SPIInit ();                      // 每次读写前，先重新配置SPI，避免多个设备共用一SPI时的配置不同
    
    W25QX_NSS_LOW ;
    SPISendByte ( 0x03);             // 发送读取命令 03h
    SPISendByte ((uint8_t)(addr>>16));
    SPISendByte ((uint8_t)(addr>>8));
    SPISendByte ((uint8_t)addr);
    
    for(uint32_t i=0;i<num;i++) {
        p[i]=SPISendByte(0xFF);
    }
    W25QX_NSS_HIGH ;    
}



/******************************************************************************
 * @Function        W25Qxx_Write  全局 4_4
 * @Description     读取数据
 *                                 
 * @Input           uint8_t   *p    要写入的数据存储区    
 *                  uint32_t  addr  写入地址         (W25Q128 只用3字节, W25Q256用4字节)
 *                  uint16_t  num   连续写入的字节数  
**/
uint8_t W25QXX_buffer[4096];     // 开辟一段内存空间

void W25qxx_WriteBuffer( uint32_t addr, uint8_t* p, uint16_t num)
{
    // 字库段写保护, 防止字库被错误写入履盖
    if(((addr+num)>0x00A00000) && (xW25Qxx.GBKStorageOK ==1 )) 
    {
        printf("写入字库区，跳过操作!!\r");
        return;
    }
    
    uint32_t  secPos      = addr/4096;         // 扇区地址,第几个扇区
    uint16_t  secOff      = addr%4096;         // 开始地始偏移字节数: 数据在扇区的第几字节存放
    uint16_t  secRemain   = 4096-secOff;       // 扇区剩余空间字节数 ,用于判断够不够存放余下的数据
    uint8_t*  buf = W25QXX_buffer;             // 原子哥代码,为什么不直接使用所声明的数组. (回看前面的疑问, 接触C有15年, 原来没下过工夫) 
    
    SPIInit ();                          // 每次读写前，先重新配置SPI，避免多个设备共用一SPI时的配置不同
    if(num<=secRemain) secRemain=num;  
    while(1) {
        W25qxx_ReadBuffer ( secPos*4096, buf, 4096);        // 读取扇区内容到缓存
        
		EraseSector(secPos );                  // 擦扇区
        for(uint16_t i=0;i<secRemain ;i++)                 // 原始数据写入缓存
            buf[secOff +i]=p[i];
        WriteSector( secPos*4096, buf, 4096);  // 缓存数据写入设备
        
        if(secRemain == num)                 // 已全部写入
            break;                                         
        else {                               // 未写完
            p=p+secRemain ;                  // 原始数据指针偏移
            secPos ++;                       // 新扇区
            secOff =0;                       // 新偏移位,扇区内数据起始地址            
            num=num-secRemain ;              // 剩余未写字节数            
            secRemain = (num>4096)?4096:num; // 计算新扇区写入字节数                  
        }          
    }    
}


/******************************************************************************
 * @Function        25Qxx_StartupTimes  全局 5_5
 * @Description     获取上电启动取次数
 *                                 
 * @Input           
 *                  
 *                 
**/
uint16_t W25qxx_StartupTimes(void)
{
    return xW25Qxx .StartupTimes ;
}

  
// 内部功能函数
// *******************************************************************************************************************************

// 5_1 发送1字节,返回1字节
// SPI通信,只一个动作:向DR写入从设命令值,同步读出数据!写读组合,按从设时序图来. 作为主设,因为收发同步,连接收发送中断也不用开,未验证其它中断对其工作的影响. 

uint8_t  SPISendByte(uint8_t d)
{
	uint8_t retry=0;
	
    while((W25QXX_SPIx ->SR & 2) == 0) {  // 等待发送区为空	
		retry++;
		if(retry>250) return 0;
	}
    W25QXX_SPIx ->DR =d;
	
	retry =0;    
    while((W25QXX_SPIx->SR & 1) == 0){   // 等待接收完数据	
		retry++;
		if(retry>250) return 0;
	}
    return W25QXX_SPIx->DR ;     
}/* 
// 其实就是HAL_SPI_TransmitReceive()，已用寄存器方式写好，直接延用即可
// 也可用HAL函数代替
uint8_t  SPISendByte(uint8_t d)
{ uint8_t Rx;
  HAL_SPI_TransmitReceive(&hspiX,&d,&Rx,1,500);
	return Rx;
}
*/

// 5_2 写使能
void WriteEnable()
{
    W25QX_NSS_LOW ;
    SPISendByte (0x6);          // 命令: Write Enable : 06h
    W25QX_NSS_HIGH ;              
}



// 5_3 等待空闲
void WaitReady()
{    
    W25QX_NSS_LOW ;
	
	SPISendByte (0x05);               // 命令: Read Status Register : 05h
    while(SPISendByte(0xFF) & 1) {}   // 只要发送读状态寄存器指令，芯片就会持续向主机发送最新的状态寄存器内容 ，直到收到通信的停止信号。
   		
	W25QX_NSS_HIGH ;    
} 
         


// 5_4 擦除一个扇区, 每扇区>150ms
void EraseSector(uint32_t addr)
{
    addr=addr*4096;         // 从第几扇区开始
    
    WriteEnable();
    WaitReady();
    // 命令
    W25QX_NSS_LOW ;
    SPISendByte (0x20);   // 命令: Sector Erase(4K) : 20h
    SPISendByte ((uint8_t)(addr>>16));
    SPISendByte ((uint8_t)(addr>>8));
    SPISendByte ((uint8_t)addr);
    W25QX_NSS_HIGH ;	
    
    WaitReady();       
} 



// 5_5 写扇区. 要分页写入
void WriteSector(uint32_t addr, uint8_t *p, uint16_t num)
{
    uint16_t pageRemain = 256;      // 重要，重要，重要：W25Qxx每个页命令最大写入字节数:256字节;    
  
    // 扇区:4096bytes, 缓存页:256bytes, 写扇区要分16次页命令写入     
    for(char i=0;i<16;i++){              
        WriteEnable ();                  // 写使能
        WaitReady ();                    // 等待空闲
        
            W25QX_NSS_LOW ;                     // 低电平,开始
            SPISendByte(0x02);              // 命令: page program : 02h , 每个写页命令最大缓存256字节
            SPISendByte((uint8_t)(addr>>16));    // 地址
            SPISendByte((uint8_t)(addr>> 8));
            SPISendByte ((uint8_t)addr); 
            for(uint16_t i=0;i<pageRemain; i++)      // 发送写入的数据 
                SPISendByte( p[i] );        // 高电平, 结束
            W25QX_NSS_HIGH ;     
        
        WaitReady ();                    // 等待空闲    
      
        p = p + pageRemain;                     // 缓存指针增加一页字节数 
        addr = addr + pageRemain ;              // 写地址增加一页字节数
    }      
}



// 检查字库样本的正确性
void CheckGBKStorage(void)
{
    printf("GBK字库 测试");                   
    uint8_t sub = 0;
    uint8_t f=0 ;                                                            
                                                   
    for(uint32_t i=0; i< 6128640; i=i+1000000)                               
    {
        W25qxx_ReadBuffer(GBK_STORAGE_ADDR +i, &f , 1);               
        sub = sub + f;                                  // 80 , 0, 98, 79, 0, 1, 0
    }      
    xW25Qxx.GBKStorageOK = (sub==146 ? 1 : 0);          // 判断是否有字库,打开地址写保护, 防止字库被错误写入履盖
    if(xW25Qxx.GBKStorageOK==1)
    {
        printf("字库可用\r");
    }
    else
    {
        printf("错，字库不可用!\r");
    }        
}
/***************************************************************************** 
  * @Fun    W25Qxx_Init
  * @brief  字模存储设备  
  */  
void W25qx_Init()
{
    xW25Qxx.InitOK =0;           // 复位初始化成功标志
    xW25Qxx.GBKStorageOK = 1;    // 假设字库存在, 先开启字库地址段写保护标志, 本函数最后检测字库赋真实值

	  //------------------------------------------------------------
    //W25QXX_SPI_CLK_EN ;        // 使能SPI    
    W25QX_NSS_HIGH ;           // W25Qx设备片选线拉高        
    // SPI_GPIO                  --CubeMX使系统初始化时已完成  
    // SPI_通信配置
    SPIInit();                 // spi初始化，独立封装，便于不同设备使用同一spi
	  //---------- 由于LCD屏和W25Q字库存储器共用SPI1，且是同一种SPI工作类型，也无其他干扰，
	  //---------- 所以在系统初始化时调用GPIO_Init()以及MX_SPI1_Init()就够了。
	
    ReadID();                    // 读取芯片型号,以判断通讯是否正常   
    CheckGBKStorage();           // 检查字库 
}



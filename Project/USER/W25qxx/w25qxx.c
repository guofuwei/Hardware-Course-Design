#include "w25qxx.h" 


//W25Qϵ��оƬ�ͺŷ���ֵ	   
#define   W25Q80  	0XEF13 	
#define   W25Q16  	0XEF14
#define   W25Q32  	0XEF15
#define   W25Q64 	  0XEF16
#define   W25Q128	  0XEF17
#define   W25Q256   0XEF18
//#define   W25Qxx    65519   // �ܶ�ʱ���������غ�����Ķ���65519
  
  
  
/*****************************************************************************
 ** ���ر���
 *****************************************************************************/
struct 
{
    uint8_t  InitOK;        // ��ʼ��״̬   0:ʧ��, 1:�ɹ�
    uint8_t  Type[20];      // �ͺ�
    uint16_t StartupTimes;  // ��¼��������
    uint8_t  GBKStorageOK;  // GBK�ֿ������־, ͬʱд����,��ֹ�ֿⱻ����д�ĸ�
}xW25Qxx;
 
 
 
/*****************************************************************************
 ** �ڲ�����
****************************************************************************/
// 5����������
static uint8_t    SPISendByte(uint8_t d);                           // 5_1    �ֽڶ�д
static void  WriteEnable(void) ;                                    // 5_2    дʹ��
static void  WaitReady(void) ;                                      // 5_3    �ȴ�����
static void  EraseSector(uint32_t addr);                            // 5_4    ������
static void  WriteSector(uint32_t addr, uint8_t* p, uint16_t num);  // 5_5    д����
// ����
static void  ReadID(void);

static void  SPIInit(void);
//#define SPIInit()
  
#define W25QXX_SPIx       SPI1            //ʹ�üĴ���SPI������Ҫ�˶��壬�����޸�SPI�ڡ�                        
/****************************************************************************** 
  * spi��ʼ��
  * ֻҪ��w25qxxϵ�У�������
  * ע�⣬�豸��SPI��ʼ������������һ�������ĺ�������ΪSTM32���豸���л�ʱ��SPI�Ĳ���Ҳ�����µ���
*/
static void  SPIInit(void)
{     
    W25QXX_SPIx -> CR1  = 0x1<<0;         // CPHA:ʱ����λ,0x1=�ڵ�2��ʱ�ӱ��ؽ������ݲ���
    W25QXX_SPIx -> CR1 |= 0x1<<1;         // CPOL:ʱ�Ӽ���,0x1=����״̬ʱ��SCK���ָߵ�ƽ
    W25QXX_SPIx -> CR1 |= 0x1<<2;         // ����ģʽ:         1 = ������
    W25QXX_SPIx -> CR1 |= 0x0<<3;         // �����ʿ���[5:3]:  0 = fPCLK /2
    W25QXX_SPIx -> CR1 |= 0x0<<7;         // ֡��ʽ:           0 = �ȷ���MSB
    W25QXX_SPIx -> CR1 |= 0x1<<9;         // ������������� :  1 = ʹ���������������(���NSS)
    W25QXX_SPIx -> CR1 |= 0x1<<8;         // �ڲ�������ѡ��,����9λ����(ʧ���ڲ�NSS)
    W25QXX_SPIx -> CR1 |= 0x0<<11;        // ����֡��ʽ,       0 = 8λ
    W25QXX_SPIx -> CR1 |= 0x1<<6;         // SPIʹ��           1 = ʹ������
         
	  HAL_Delay (100);    
}

// ��ʵ����CubeMX���ɵ�MX_SPI1_Init();���üĴ�����ʽд�ã�ֱ�����ü��ɣ�
// Ҳ����MX_SPIx_Inti()�������棬�������SPI�豸��ͬ��SPI��ʽ������ÿ��
// �豸�л�ʱ����������SPI_Init��ֻҪ�л�NSS���õ�ǰ�豸NSSΪ�ͼ���
// �ʽ���ע�͵����������SPIInit()�������ɲ����޸�������롣
 
	
/***************************************************************************** 
  * @Fun    W25Qxx_ReadID
  * @brief  ��ȡоƬ�ͺ�,�����ж�ͨѶ״��         
  */        
static void ReadID()
{   
    uint16_t W25QxxType = 0 ; 	
    // 1: ��ȡоƬ�ͺ�, �ж�����״��    
    W25QX_NSS_LOW; 
    SPISendByte(0x90);  // ���Ͷ�ȡID����,���������,��һ�ֽ�������,�����ֽ���0
    SPISendByte(0x00);
    SPISendByte(0x00);
    SPISendByte(0x00);  // �����ֽڱؽ����� 0h      
    W25QxxType  = (SPISendByte(0xFF))<<8;   // uint16_t W25QxxType  �ڱ��ļ�����,ȫ��
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
            sprintf((char*)xW25Qxx.Type, "%s", "W25Q256");           // ע��:W25Q256�ĵ�ַ��4�ֽ�               
    		    break;		
    	  default:             
            sprintf((char*)xW25Qxx.Type, "%s", "Flash�豸ʧ�� !!!");              
            xW25Qxx.InitOK =0;
            printf("��ȡ���Ĵ����ͺ����ݣ�%d\r\n",W25QxxType);
    		    break;
    }        
   
	// 2:��ȡ�洢����, ��������������¼      
    if(xW25Qxx.InitOK  == 1 ) {   
		uint32_t Addr = 0x00;            // ���ݵ�ַ,  W25Q128����ַ:0X0100 0000
        uint8_t d[4]={0};                 // ���ݻ��棬 0x0000:��־0xEE, 0x0001:��־0X00,     0x0002:���ݸ�λ, 0x0003:���ݵ�λ
        uint16_t startFlag  = 0;               // ��־
        uint16_t startNum   = 0;                  // ��������
        
        W25qxx_ReadBuffer(  Addr, d, 4);       // ��ȡ4���ֽ�����
		    startFlag = (d[0]<<8) | d[1];   // ��־
        startNum  = (d[2]<<8) | d[3];   // ��������        
        
        if(startFlag!=0xEE00) {         // û�оɼ�¼	
            startNum=1;            
            d[2]=0;
            d[3]=1;                
        }
        else{
            startNum++;              // �ɹ���ȡ���ݣ� ��������1                    
            d[2]=(uint8_t)(startNum>>8);
            d[3]=(uint8_t)startNum;     
            // d[2]=(uint8_t)0;           // �ظ��������� = 0, ȡ�������м��ɣ�
            // d[3]=(uint8_t)0;           // ������¼�󣬵�����ע�ͣ������ٴ���¼    
        }
        d[0]=0xEE;
        d[1]=0x00;
		
		// ���غ�f=0xEE00ʱ�����������Ī����λ
        W25qxx_WriteBuffer( Addr, d, 4);  
		    xW25Qxx.StartupTimes = startNum; 
        
        //char c[45]=" ";
        printf ("�ͺ�:%s ,��%d����\r\n", xW25Qxx.Type, startNum); 
    }
    else 
    {    // ���W25Qxxʧ��	  
        printf("�ͺŴ��󣬲�����!\r"); 
     //printf("���Ը�λоƬ...\r"); 
		 //System_Reset ();
    }          
}



/******************************************************************************
 * @Function        W25Qxx_Read  ȫ�� 4_3
 * @Description     ��ȡ����
 *                                 
 * @Input           uint8_t   *p    ��������ֵ���λ��    
 *                  uint32_t  addr  ��ȡ��ַ
 *                  uint16_t  num   ������ȡ���ֽ��� 
**/
void W25qxx_ReadBuffer( uint32_t addr, uint8_t *p, uint16_t num)
{
    SPIInit ();                      // ÿ�ζ�дǰ������������SPI���������豸����һSPIʱ�����ò�ͬ
    
    W25QX_NSS_LOW ;
    SPISendByte ( 0x03);             // ���Ͷ�ȡ���� 03h
    SPISendByte ((uint8_t)(addr>>16));
    SPISendByte ((uint8_t)(addr>>8));
    SPISendByte ((uint8_t)addr);
    
    for(uint32_t i=0;i<num;i++) {
        p[i]=SPISendByte(0xFF);
    }
    W25QX_NSS_HIGH ;    
}



/******************************************************************************
 * @Function        W25Qxx_Write  ȫ�� 4_4
 * @Description     ��ȡ����
 *                                 
 * @Input           uint8_t   *p    Ҫд������ݴ洢��    
 *                  uint32_t  addr  д���ַ         (W25Q128 ֻ��3�ֽ�, W25Q256��4�ֽ�)
 *                  uint16_t  num   ����д����ֽ���  
**/
uint8_t W25QXX_buffer[4096];     // ����һ���ڴ�ռ�

void W25qxx_WriteBuffer( uint32_t addr, uint8_t* p, uint16_t num)
{
    // �ֿ��д����, ��ֹ�ֿⱻ����д���ĸ�
    if(((addr+num)>0x00A00000) && (xW25Qxx.GBKStorageOK ==1 )) 
    {
        printf("д���ֿ�������������!!\r");
        return;
    }
    
    uint32_t  secPos      = addr/4096;         // ������ַ,�ڼ�������
    uint16_t  secOff      = addr%4096;         // ��ʼ��ʼƫ���ֽ���: �����������ĵڼ��ֽڴ��
    uint16_t  secRemain   = 4096-secOff;       // ����ʣ��ռ��ֽ��� ,�����жϹ�����������µ�����
    uint8_t*  buf = W25QXX_buffer;             // ԭ�Ӹ����,Ϊʲô��ֱ��ʹ��������������. (�ؿ�ǰ�������, �Ӵ�C��15��, ԭ��û�¹�����) 
    
    SPIInit ();                          // ÿ�ζ�дǰ������������SPI���������豸����һSPIʱ�����ò�ͬ
    if(num<=secRemain) secRemain=num;  
    while(1) {
        W25qxx_ReadBuffer ( secPos*4096, buf, 4096);        // ��ȡ�������ݵ�����
        
		EraseSector(secPos );                  // ������
        for(uint16_t i=0;i<secRemain ;i++)                 // ԭʼ����д�뻺��
            buf[secOff +i]=p[i];
        WriteSector( secPos*4096, buf, 4096);  // ��������д���豸
        
        if(secRemain == num)                 // ��ȫ��д��
            break;                                         
        else {                               // δд��
            p=p+secRemain ;                  // ԭʼ����ָ��ƫ��
            secPos ++;                       // ������
            secOff =0;                       // ��ƫ��λ,������������ʼ��ַ            
            num=num-secRemain ;              // ʣ��δд�ֽ���            
            secRemain = (num>4096)?4096:num; // ����������д���ֽ���                  
        }          
    }    
}


/******************************************************************************
 * @Function        25Qxx_StartupTimes  ȫ�� 5_5
 * @Description     ��ȡ�ϵ�����ȡ����
 *                                 
 * @Input           
 *                  
 *                 
**/
uint16_t W25qxx_StartupTimes(void)
{
    return xW25Qxx .StartupTimes ;
}

  
// �ڲ����ܺ���
// *******************************************************************************************************************************

// 5_1 ����1�ֽ�,����1�ֽ�
// SPIͨ��,ֻһ������:��DRд���������ֵ,ͬ����������!д�����,������ʱ��ͼ��. ��Ϊ����,��Ϊ�շ�ͬ��,�����շ����ж�Ҳ���ÿ�,δ��֤�����ж϶��乤����Ӱ��. 

uint8_t  SPISendByte(uint8_t d)
{
	uint8_t retry=0;
	
    while((W25QXX_SPIx ->SR & 2) == 0) {  // �ȴ�������Ϊ��	
		retry++;
		if(retry>250) return 0;
	}
    W25QXX_SPIx ->DR =d;
	
	retry =0;    
    while((W25QXX_SPIx->SR & 1) == 0){   // �ȴ�����������	
		retry++;
		if(retry>250) return 0;
	}
    return W25QXX_SPIx->DR ;     
}/* 
// ��ʵ����HAL_SPI_TransmitReceive()�����üĴ�����ʽд�ã�ֱ�����ü���
// Ҳ����HAL��������
uint8_t  SPISendByte(uint8_t d)
{ uint8_t Rx;
  HAL_SPI_TransmitReceive(&hspiX,&d,&Rx,1,500);
	return Rx;
}
*/

// 5_2 дʹ��
void WriteEnable()
{
    W25QX_NSS_LOW ;
    SPISendByte (0x6);          // ����: Write Enable : 06h
    W25QX_NSS_HIGH ;              
}



// 5_3 �ȴ�����
void WaitReady()
{    
    W25QX_NSS_LOW ;
	
	SPISendByte (0x05);               // ����: Read Status Register : 05h
    while(SPISendByte(0xFF) & 1) {}   // ֻҪ���Ͷ�״̬�Ĵ���ָ�оƬ�ͻ�����������������µ�״̬�Ĵ������� ��ֱ���յ�ͨ�ŵ�ֹͣ�źš�
   		
	W25QX_NSS_HIGH ;    
} 
         


// 5_4 ����һ������, ÿ����>150ms
void EraseSector(uint32_t addr)
{
    addr=addr*4096;         // �ӵڼ�������ʼ
    
    WriteEnable();
    WaitReady();
    // ����
    W25QX_NSS_LOW ;
    SPISendByte (0x20);   // ����: Sector Erase(4K) : 20h
    SPISendByte ((uint8_t)(addr>>16));
    SPISendByte ((uint8_t)(addr>>8));
    SPISendByte ((uint8_t)addr);
    W25QX_NSS_HIGH ;	
    
    WaitReady();       
} 



// 5_5 д����. Ҫ��ҳд��
void WriteSector(uint32_t addr, uint8_t *p, uint16_t num)
{
    uint16_t pageRemain = 256;      // ��Ҫ����Ҫ����Ҫ��W25Qxxÿ��ҳ�������д���ֽ���:256�ֽ�;    
  
    // ����:4096bytes, ����ҳ:256bytes, д����Ҫ��16��ҳ����д��     
    for(char i=0;i<16;i++){              
        WriteEnable ();                  // дʹ��
        WaitReady ();                    // �ȴ�����
        
            W25QX_NSS_LOW ;                     // �͵�ƽ,��ʼ
            SPISendByte(0x02);              // ����: page program : 02h , ÿ��дҳ������󻺴�256�ֽ�
            SPISendByte((uint8_t)(addr>>16));    // ��ַ
            SPISendByte((uint8_t)(addr>> 8));
            SPISendByte ((uint8_t)addr); 
            for(uint16_t i=0;i<pageRemain; i++)      // ����д������� 
                SPISendByte( p[i] );        // �ߵ�ƽ, ����
            W25QX_NSS_HIGH ;     
        
        WaitReady ();                    // �ȴ�����    
      
        p = p + pageRemain;                     // ����ָ������һҳ�ֽ��� 
        addr = addr + pageRemain ;              // д��ַ����һҳ�ֽ���
    }      
}



// ����ֿ���������ȷ��
void CheckGBKStorage(void)
{
    printf("GBK�ֿ� ����");                   
    uint8_t sub = 0;
    uint8_t f=0 ;                                                            
                                                   
    for(uint32_t i=0; i< 6128640; i=i+1000000)                               
    {
        W25qxx_ReadBuffer(GBK_STORAGE_ADDR +i, &f , 1);               
        sub = sub + f;                                  // 80 , 0, 98, 79, 0, 1, 0
    }      
    xW25Qxx.GBKStorageOK = (sub==146 ? 1 : 0);          // �ж��Ƿ����ֿ�,�򿪵�ַд����, ��ֹ�ֿⱻ����д���ĸ�
    if(xW25Qxx.GBKStorageOK==1)
    {
        printf("�ֿ����\r");
    }
    else
    {
        printf("���ֿⲻ����!\r");
    }        
}
/***************************************************************************** 
  * @Fun    W25Qxx_Init
  * @brief  ��ģ�洢�豸  
  */  
void W25qx_Init()
{
    xW25Qxx.InitOK =0;           // ��λ��ʼ���ɹ���־
    xW25Qxx.GBKStorageOK = 1;    // �����ֿ����, �ȿ����ֿ��ַ��д������־, ������������ֿ⸳��ʵֵ

	  //------------------------------------------------------------
    //W25QXX_SPI_CLK_EN ;        // ʹ��SPI    
    W25QX_NSS_HIGH ;           // W25Qx�豸Ƭѡ������        
    // SPI_GPIO                  --CubeMXʹϵͳ��ʼ��ʱ�����  
    // SPI_ͨ������
    SPIInit();                 // spi��ʼ����������װ�����ڲ�ͬ�豸ʹ��ͬһspi
	  //---------- ����LCD����W25Q�ֿ�洢������SPI1������ͬһ��SPI�������ͣ�Ҳ���������ţ�
	  //---------- ������ϵͳ��ʼ��ʱ����GPIO_Init()�Լ�MX_SPI1_Init()�͹��ˡ�
	
    ReadID();                    // ��ȡоƬ�ͺ�,���ж�ͨѶ�Ƿ�����   
    CheckGBKStorage();           // ����ֿ� 
}



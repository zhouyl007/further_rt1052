
#include "bsp_spiflash2.h"


uint16_t W25Q64_TYPE=W25Q64;	//默认是W25Q64


lpspi_master_config_t lpspi1_config; //LPSPI1


static void delay_us(uint32_t us)
{
	uint16_t i;
	for (i = 0; i < us*100; i++);
}


//³õÊ¼»¯SPI3
//baudrate:²¨ÌØÂÊ£¬µ¥Î»Mhz£¬SPI×î´ó120M²¨ÌØÂÊ
static void LPSPI1_Init(uint8_t baudrate)
{
	uint32_t lpspiclk=0;
	uint32_t spi1_baudrate=baudrate*1000000;
	
	//ÉèÖÃLPSPIµÄÊ±ÖÓÔ´ÎªPLL3_PFD0/(1+5)=576Mhz/(1+5)=96MHz,W25Q64Ê±ÖÓ×î¸ß104MHz
	CLOCK_SetMux(kCLOCK_LpspiMux,1);    //USB1 PLL PFD0(576MHz)×÷ÎªLPSPIÊ±ÖÓÔ´
	CLOCK_SetDiv(kCLOCK_LpspiDiv,5);    //·ÖÆµÉèÖÃ£¬·ÖÆµÖµÎª5+1£¬ËùÒÔLPSPI CLK=576/(1+5)=96Mhz
	
	//ÅäÖÃIO×÷ÎªLPSPI3£¬Æ¬Ñ¡²ÉÓÃÈí¼þµÄ£¬ÕâÑùÒ»¸öSPIµÄÍâÉè¾Í²»»áÏÞÖÆÎª4¸ö!
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_00_LPSPI1_SCK,0);                                    
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_02_LPSPI1_SDO,0);                                    
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_03_LPSPI1_SDI,0);                                    
	
	//ÅäÖÃLPSPIÒý½Å¹¦ÄÜ£¬
	//µÍ×ª»»ËÙ¶È,Çý¶¯ÄÜÁ¦ÎªR0/6,ËÙ¶ÈÎª100Mhz£¬¿ªÂ·Êä³ö£¬Ê¹ÄÜpull/keepr
  //Ñ¡Ôñkeeper¹¦ÄÜ£¬ÏÂÀ­100K Ohm£¬¹Ø±ÕHyst
	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_00_LPSPI1_SCK,0x10B0);                              
	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_02_LPSPI1_SDO,0x10B0);                                                                              
	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_03_LPSPI1_SDI,0x10B0);                                
	
	//ÅäÖÃLPSPI3Ö÷Ä£Ê½
	lpspiclk=(CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk)/(5+1));//LPSPI3Ê±ÖÓ
	lpspi1_config.baudRate=spi1_baudrate;               //SPIËÙ¶È
	lpspi1_config.whichPcs=kLPSPI_Pcs1;                 //Æ¬Ñ¡ÐÅºÅ£¬PCS1
	lpspi1_config.pcsActiveHighOrLow=kLPSPI_PcsActiveLow;//Æ¬Ñ¡ÐÅºÅµÍµçÆ½ÓÐÐ§
	lpspi1_config.bitsPerFrame=8;                       //ÉèÖÃSPIµÄÊý¾Ý´óÐ¡:SPI·¢ËÍ½ÓÊÕ8Î»Ö¡½á¹¹
	lpspi1_config.cpol=kLPSPI_ClockPolarityActiveLow;   //´®ÐÐÍ¬²½Ê±ÖÓµÍµçÆ½ÓÐÐ§
	lpspi1_config.cpha=kLPSPI_ClockPhaseSecondEdge;      //´®ÐÐÍ¬²½Ê±ÖÓµÄµÚ¶þ¸öÌø±äÑØ£¨ÉÏÉý»òÏÂ½µ£©Êý¾Ý±»²ÉÑù
	lpspi1_config.direction=kLPSPI_MsbFirst;            //Ö¸¶¨Êý¾Ý´«Êä´ÓMSBÎ»»¹ÊÇLSBÎ»¿ªÊ¼:Êý¾Ý´«Êä´ÓMSBÎ»¿ªÊ¼
	lpspi1_config.pinCfg=kLPSPI_SdiInSdoOut;            //SDIÊäÈëÒý½Å£¬SDOÊä³öÒý½Å
	lpspi1_config.dataOutConfig=kLpspiDataOutRetained;  //Êä³öÊý¾Ý±£Áô
	lpspi1_config.pcsToSckDelayInNanoSec=10;            //Æ¬Ñ¡À­µÍµ½Ê±ÖÓÓÐÐ§Ö®¼äµÄÑÓÊ±Ê±¼ä,µ¥Î»ns
	lpspi1_config.lastSckToPcsDelayInNanoSec=10;        //×îºóÒ»¸öÊ±ÖÓµ½Æ¬Ñ¡À­¸ßÖ®¼äµÄÑÓÊ±£¬µ¥Î»ns
	lpspi1_config.betweenTransferDelayInNanoSec=10;     //Á½´Î´«ÊäÖ®¼äµÄÑÓÊ±£¬µ¥Î»ns
	LPSPI_MasterInit(LPSPI1,&lpspi1_config,lpspiclk);
	LPSPI_Enable(LPSPI1,true);                          //Ê¹ÄÜLPSPI3
}


//LPSPI3 ¶ÁÐ´Ò»¸ö×Ö½Ú
//TxData:ÒªÐ´ÈëµÄ×Ö½Ú
//·µ»ØÖµ:¶ÁÈ¡µ½µÄ×Ö½Ú
uint8_t LPSPI1_ReadWriteByte(uint8_t TxData)
{ 
	uint8_t spirxdata=0;
	uint8_t spitxdata=TxData;
	lpspi_transfer_t spi_tranxfer;
	
	spi_tranxfer.configFlags=kLPSPI_MasterPcs1|kLPSPI_MasterPcsContinuous;     //PCS1
	spi_tranxfer.txData=&spitxdata;                 //Òª·¢ËÍµÄÊý¾Ý
	spi_tranxfer.rxData=&spirxdata;                 //½ÓÊÕµ½µÄÊý¾Ý
	spi_tranxfer.dataSize=1;                        //Êý¾Ý³¤¶È
	LPSPI_MasterTransferBlocking(LPSPI1,&spi_tranxfer);	    //SPI×èÈû·¢ËÍ   					    
	return spirxdata;
}



void bsp_w25q64_init(void)
{ 
	uint8_t temp;
	gpio_pin_config_t spics_config;

	//片选作为普通IO口，软件控制片选，这样一个SPI口就不止4个外设
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_01_GPIO3_IO13,0);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_01_GPIO3_IO13,0x10B0); 

	spics_config.direction=kGPIO_DigitalOutput;	//输出
	spics_config.interruptMode=kGPIO_NoIntmode;	//不使用中断功能
	spics_config.outputLogic=1;					//默认高电平
	GPIO_PinInit(GPIO3,13,&spics_config); 	    //初始化GPIO1_4

	W25Q64_CS(1);                           //SPI FLASH不选中
	LPSPI1_Init(96);                        //SPI波特率96M
	W25Q64_TYPE=W25Q64_ReadID();	        //读取FLASH ID.
	if(W25Q64_TYPE==W25Q256)                //SPI FLASH为W25Q64
	{
		temp=W25Q64_ReadSR(3);              //读取状态寄存器3，判断地址模式
		if((temp&0X01)==0)			        //如果不是4字节地址模式,则进入4字节地址模式
		{
			W25Q64_CS(0); 			        //选中
			LPSPI1_ReadWriteByte(W64_Enable4ByteAddr);//发送进入4字节地址模式指令 
			W25Q64_CS(1); 			        //取消片选             
		}
	}
}  

//读取W25Q64的状态寄存器，W25Q64一共有3个状态寄存器
//状态寄存器1：
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
//状态寄存器2：
//BIT7  6   5   4   3   2   1   0
//SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
//状态寄存器3：
//BIT7      6    5    4   3   2   1   0
//HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
//regno:状态寄存器号，范:1~3
//返回值:状态寄存器值
uint8_t W25Q64_ReadSR(uint8_t regno)   
{  
	uint8_t byte=0,command=0; 
	switch(regno)
	{
		case 1:
				command=W64_ReadStatusReg1;    //读状态寄存器1指令
				break;
		case 2:
				command=W64_ReadStatusReg2;    //读状态寄存器2指令
				break;
		case 3:
				command=W64_ReadStatusReg3;    //读状态寄存器3指令
				break;
		default:
				command=W64_ReadStatusReg1;    
				break;
	}                                       
	W25Q64_CS(0); 			                    //选中
	LPSPI1_ReadWriteByte(command);              //发送读取状态寄存器命令    
	byte=LPSPI1_ReadWriteByte(0Xff);            //读取一个字节  
	W25Q64_CS(1); 			                    //取消选中
	return byte;   
} 

//写W25Q64状态寄存器
void W25Q64_Write_SR(uint8_t regno,uint8_t sr)   
{   
	uint8_t command=0;
	switch(regno)
	{
		case 1:
				command=W64_WriteStatusReg1;    //写状态寄存器1指令
				break;
		case 2:
				command=W64_WriteStatusReg2;    //写状态寄存器2指令
				break;
		case 3:
				command=W64_WriteStatusReg3;    //写状态寄存器3指令
				break;
		default:
				command=W64_WriteStatusReg1;    
				break;
	}     
	W25Q64_CS(0);                               //使能器件 
	LPSPI1_ReadWriteByte(command);              //发送写取状态寄存器命令    
	LPSPI1_ReadWriteByte(sr);                   //写入一个字节   
	W25Q64_CS(1);                               //取消片选      
}  

//W25Q64写使能	
//将WEL置位   
void W25Q64_Write_Enable(void)   
{ 
	W25Q64_CS(0);                               //使能器件  
	LPSPI1_ReadWriteByte(W64_WriteEnable);     //发送写使能  
	W25Q64_CS(1);                               //取消片选     
} 

//W25Q64写禁止	
//将WEL清零  
void W25Q64_Write_Disable(void)   
{ 
	W25Q64_CS(0);                               //使能器件       
	LPSPI1_ReadWriteByte(W64_WriteDisable);    //发送写禁止指令 
	W25Q64_CS(1);                               //使能器件       
} 

//读取芯片ID
//返回值如下:				   
//0XEF13,表示芯片型号为W25Q80  
//0XEF14,表示芯片型号为W25Q16    
//0XEF15,表示芯片型号为W25Q32  
//0XEF16,表示芯片型号为W25Q64 
//0XEF17,表示芯片型号为W25Q128 	  
//0XEF18,表示芯片型号为W25Q64
uint16_t W25Q64_ReadID(void)
{
	uint16_t Temp = 0;	  
    
  W25Q64_CS(0);
	LPSPI1_ReadWriteByte(0x90);//发送读取ID命令	
	LPSPI1_ReadWriteByte(0x00); 
	LPSPI1_ReadWriteByte(0x00); 
	LPSPI1_ReadWriteByte(0x00);
	Temp|=LPSPI1_ReadWriteByte(0xFF)<<8;  
	Temp|=LPSPI1_ReadWriteByte(0xFF);
  W25Q64_CS(1);
	return Temp;
}   		    
//读取SPI FLASH  
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void bsp_w25q64_read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
{ 
 	uint16_t i;   
	W25Q64_CS(0);                               //使能器件    
	LPSPI1_ReadWriteByte(W64_ReadData);        //发送读取命令  
	if(W25Q64_TYPE==W25Q256)                    //如果是W25Q64的话地址为4字节的，要发送最高8位
	{
			LPSPI1_ReadWriteByte((uint8_t)((ReadAddr)>>24));    
	}
	LPSPI1_ReadWriteByte((uint8_t)((ReadAddr)>>16));   //发送24bit地址    
	LPSPI1_ReadWriteByte((uint8_t)((ReadAddr)>>8));   
	LPSPI1_ReadWriteByte((uint8_t)ReadAddr);   
	for(i=0;i<NumByteToRead;i++)
	{ 
		pBuffer[i]=LPSPI1_ReadWriteByte(0XFF);    //循环读数  
	}	
	W25Q64_CS(1);     
}  
//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!	 
void W25Q64_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
 	uint16_t i;  
	W25Q64_Write_Enable();                      //SET WEL 
	W25Q64_CS(0);                               //使能器件      
	LPSPI1_ReadWriteByte(W64_PageProgram);     //发送写页命令   
	if(W25Q64_TYPE==W25Q256)                    //如果是W25Q64的话地址为4字节的，要发送最高8位
	{
		LPSPI1_ReadWriteByte((uint8_t)((WriteAddr)>>24)); 
	}
	LPSPI1_ReadWriteByte((uint8_t)((WriteAddr)>>16)); //发送24bit地址    
	LPSPI1_ReadWriteByte((uint8_t)((WriteAddr)>>8));   
	LPSPI1_ReadWriteByte((uint8_t)WriteAddr);   
	for(i=0;i<NumByteToWrite;i++)LPSPI1_ReadWriteByte(pBuffer[i]);//循环写数  
	W25Q64_CS(1);                            //取消片选 
	W25Q64_Wait_Busy();					    //等待写入结束
} 
//无检验写SPI FLASH 
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void W25Q64_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 			 		 
	uint16_t pageremain;	   
	pageremain=256-WriteAddr%256; //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	   
		W25Q64_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	}    
} 
//写SPI FLASH  
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)						
//NumByteToWrite:要写入的字节数(最大65535)   
uint8_t W25Q64_BUFFER[4096];		
void bsp_w25q64_write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{ 
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    
	uint8_t * W25Q64_BUF;	  
  W25Q64_BUF=W25Q64_BUFFER;	     
 	secpos=WriteAddr/4096;//扇区地址  
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		bsp_w25q64_read(W25Q64_BUF,secpos*4096,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25Q64_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			W25Q64_Erase_Sector(secpos);//擦除这个扇区
			for(i=0;i<secremain;i++)	   //复制
			{
				W25Q64_BUF[i+secoff]=pBuffer[i];	  
			}
			W25Q64_Write_NoCheck(W25Q64_BUF,secpos*4096,4096);//写入整个扇区  

		}else W25Q64_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  //指针偏移
			WriteAddr+=secremain;//写地址偏移	   
		   	NumByteToWrite-=secremain;				//字节数递减
			if(NumByteToWrite>4096)secremain=4096;	//下一个扇区还是写不完
			else secremain=NumByteToWrite;			//下一个扇区可以写完了
		}	 
	} 
}
//擦除整个芯片		  
//等待时间超长...
void W25Q64_Erase_Chip(void)   
{                                   
	W25Q64_Write_Enable();                      //SET WEL 
	W25Q64_Wait_Busy();  
	W25Q64_CS(0);                               //使能器件      
	LPSPI1_ReadWriteByte(W64_ChipErase);       //发送片擦除命令 
	W25Q64_CS(1);                               //取消片选     
	W25Q64_Wait_Busy();   				              //等待芯片擦除结束
}  

//擦除一个扇区
//Dst_Addr:扇区地址 根据实际容量设置
//擦除一个扇区的最少时间:150ms
void W25Q64_Erase_Sector(uint32_t Dst_Addr)
{  
	//监视falsh擦除情况,测试用   
 	//printf("fe:%x\r\n",Dst_Addr);	  
 	Dst_Addr*=4096;
	W25Q64_Write_Enable();                      //SET WEL 	 
	W25Q64_Wait_Busy();  
	W25Q64_CS(0);                               //使能器件     
	LPSPI1_ReadWriteByte(W64_SectorErase);     //发送扇区擦除指令 
	if(W25Q64_TYPE==W25Q256)                    //如果是W25Q64的话地址为4字节的，要发送最高8位
	{
			LPSPI1_ReadWriteByte((uint8_t)((Dst_Addr)>>24)); 
	}
	LPSPI1_ReadWriteByte((uint8_t)((Dst_Addr)>>16));  //发送24bit地址    
	LPSPI1_ReadWriteByte((uint8_t)((Dst_Addr)>>8));   
	LPSPI1_ReadWriteByte((uint8_t)Dst_Addr);     	
	W25Q64_CS(1);                               //取消片选    
	W25Q64_Wait_Busy();   				        //等待擦除完成
}  

//等待空闲
void W25Q64_Wait_Busy(void)
{   
	while((W25Q64_ReadSR(1)&0x01)==0x01);       // 等待BUSY位清空
} 

//进入掉电模式
void W25Q64_PowerDown(void)   
{  
	W25Q64_CS(0);                               //使能器件  
	LPSPI1_ReadWriteByte(W64_PowerDown);       //发送掉电命令   
	W25Q64_CS(1);                               //取消片选    
	delay_us(3);                                //等待TPD  
}   

//唤醒
void W25Q64_WAKEUP(void)   
{ 
	W25Q64_CS(0);                                   //使能器件     
	LPSPI1_ReadWriteByte(W64_ReleasePowerDown);     // send W25X_PowerDown command 0xAB        	      
	W25Q64_CS(1);                                   //取消片选
	delay_us(3);                                     //等待TRES1
}   


uint8_t bsp_test_spiflashwq64(void)
{
	uint16_t iCount =0,jCount = 0;
	uint8_t buf[256] = {0};
	
	//for(jCount=0 ;jCount < 10;jCount++ ) {
	for(iCount=0 ;iCount<256 ;iCount++ ) {
		buf[iCount] = iCount;
	}

	bsp_w25q64_write(buf,333,256); 
		
	memset(buf,0,256);
	
	bsp_w25q64_read(buf,333,256);  
	for(iCount=0 ;iCount<256 ;iCount++ ) {
		if(buf[iCount] != iCount){
			return 1;
		}
	}
		
	//}
	return 0;
}


#include "bsp_uartwifi.h"


static lpuart_config_t lpuart4_config;



#if EN_LPUART4_RX						

uint8_t LPUART4_RX_BUF[LPUART4_REC_LEN];    

uint16_t LPUART4_RX_STA=0;       


void LPUART4_IRQHandler(void)
{
	uint8_t res=0;
	
	if((LPUART4->STAT)&kLPUART_RxDataRegFullFlag) 
	{
		res=LPUART4->DATA;					 		
		LPUART4_RX_BUF[LPUART4_RX_STA&0XFF]= res;
		LPUART4_RX_STA++;
		if( (LPUART4_RX_STA&0xFF) > 200) {
			LPUART4_RX_STA = 0;
		}
	}
	__DSB();			
}
	




#endif	

//»ñÈ¡LPUARTµÄÊ±ÖÓÔ´ÆµÂÊ£¬ÎÒÃÇÇ°ÃæÉèÖÃµÄÊÇ80MHz
//·µ»ØÖµ£ºLPUARTÊ±ÖÓÔ´ÆµÂÊ,¸ù¾ÝÎÒÃÇµÄÖ»ÊÇÒ»°ãÊÇ80Mhz
static uint32_t LPUART_SrcFreqGet(void)
{
	uint32_t freq;

	if(CLOCK_GetMux(kCLOCK_UartMux)==0) 	//LPUARTµÄÊ±ÖÓÔ´Ñ¡ÔñPLL3/6
	{
			freq=(CLOCK_GetPllFreq(kCLOCK_PllUsb1)/6U)/(CLOCK_GetDiv(kCLOCK_UartDiv)+1U);
	}
	else									//LPUARTµÄÊ±ÖÓÔ´Ñ¡ÔñOSC
	{
			freq=CLOCK_GetOscFreq()/(CLOCK_GetDiv(kCLOCK_UartDiv)+1U);
	}
	return freq;
}

//³õÊ¼»¯IO ´®¿Ú1 
//bound:²¨ÌØÂÊ
void bsp_uart4wifi_init(uint32_t bound)
{	
	uint32_t freq=0;							//´®¿ÚµÄÊ±ÖÓÔ´ÆµÂÊ
	
	CLOCK_EnableClock(kCLOCK_Lpuart4);	//Ê¹ÄÜLPUART1Ê±ÖÓ
	
	CLOCK_SetMux(kCLOCK_UartMux,0); 	//ÉèÖÃUARTÊ±ÖÓÔ´ÎªPLL3 80Mhz£¬PLL3/6=480/6=80MHz
  CLOCK_SetDiv(kCLOCK_UartDiv,0); 	//ÉèÖÃUARTÊ±ÖÓ1·ÖÆµ£¬¼´UARTÊ±ÖÓÎª80Mhz
	
	//LPUART1ËùÊ¹ÓÃµÄIO¹¦ÄÜÅäÖÃ£¬¼´£º´ÓALT0~ALT7Ñ¡ÔñºÏÊÊµÄ¹¦ÄÜ¡£
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_00_LPUART4_TX,0U);	//GPIO_AD_B0_12ÉèÖÃÎªLPUART1_TX
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_01_LPUART4_RX,0U);	//GPIO_AD_B0_13ÉèÖÃÎªLPUART1_RX

	//ÅäÖÃIOÒý½ÅGPIO_AD_B0_12ºÍGPIO_AD_B0_13µÄ¹¦ÄÜ
	//µÍ×ª»»ËÙ¶È,Çý¶¯ÄÜÁ¦ÎªR0/6,ËÙ¶ÈÎª100Mhz£¬¹Ø±Õ¿ªÂ·¹¦ÄÜ£¬Ê¹ÄÜpull/keepr
	//Ñ¡Ôñkeeper¹¦ÄÜ£¬ÏÂÀ­100K Ohm£¬¹Ø±ÕHyst
	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_00_LPUART4_TX,0x10B0u); 
	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_01_LPUART4_RX,0x10B0u); 

	freq=LPUART_SrcFreqGet();	
	
	LPUART_GetDefaultConfig(&lpuart4_config); 				//ÏÈÉèÖÃÎªÄ¬ÈÏÅäÖÃ£¬ºóÃæÔÚ¸ù¾ÝÊµ¼ÊÇé¿öÅäÖÃ
	
	//DbgConsole_Init(BOARD_DEBUG_UART_BASEADDR,bound,BOARD_DEBUG_UART_TYPE,freq);
	
	lpuart4_config.baudRate_Bps=bound;						//²¨ÌØÂÊ
	lpuart4_config.dataBitsCount=kLPUART_EightDataBits;		//8Î»
	lpuart4_config.stopBitCount=kLPUART_OneStopBit;			//1Î»Í£Ö¹Î»
	lpuart4_config.parityMode=kLPUART_ParityDisabled;		//ÎÞÆæÅ¼Ð£Ñé
	lpuart4_config.enableRx=true;							//Ê¹ÄÜ½ÓÊÕ
	lpuart4_config.enableTx=true;							//Ê¹ÄÜ·¢ËÍ
	
	LPUART_Init(LPUART4,&lpuart4_config,freq);				//³õÊ¼»¯LPUART1 
    
#if EN_LPUART4_RX									        //ÊÇ·ñÐèÒª¿ªÆôÖÐ¶Ï?	
	LPUART_EnableInterrupts(LPUART4,kLPUART_RxDataRegFullInterruptEnable); //Ê¹ÄÜ½ÓÊÕÖÐ¶Ï
	//RT1052_NVIC_SetPriority(LPUART4_IRQn,5,0);				    //ÇÀÕ¼ÓÅÏÈ¼¶5£¬×ÓÓÅÏÈ¼¶0
	EnableIRQ(LPUART4_IRQn);	                            //Ê¹ÄÜLPUART1ÖÐ¶Ï
#endif
}	


void bsp_uart4_sendbyte(uint8_t _cData)
{
	while((LPUART4->STAT&LPUART_STAT_TDRE_MASK)==0);
	LPUART4->DATA=(uint8_t)_cData;	
}


void bsp_uart4_sendstr(const uint8_t *strP,uint8_t _num)
{
  uint16_t iCount=0;
  for(iCount=0 ;iCount<_num ;iCount++ ){
	  bsp_uart4_sendbyte(strP[iCount]); 
	}  
}








 





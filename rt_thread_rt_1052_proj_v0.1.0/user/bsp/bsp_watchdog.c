
#include "bsp_watchdog.h"

rtwdog_config_t rtwdog_config;	


void bsp_rtwdog_init(u8 src,u8 pscen,u16 toval,u16 win)
{
  rtwdog_clock_prescaler_t prescal;
  
  CLOCK_EnableClock(kCLOCK_Wdog3);			    //Ê¹ÄÜRTWDOG
    
	//³õÊ¼»¯RTWDOG
	if(pscen==0)prescal=kRTWDOG_ClockPrescalerDivide1;	//²»·ÖÆµ
	else if(pscen==1)prescal=kRTWDOG_ClockPrescalerDivide256;//256·ÖÆµ
	
	delay_ms(10);									//ÇÐ»»Ê±ÖÓÇ°ÒªµÈ´ýÒ»¶ÎÊ±¼ä
	RTWDOG_GetDefaultConfig(&rtwdog_config);		//ÏÈÊ¹ÓÃÄ¬ÈÏ²ÎÊýÅäÖÃRTWDOG
	rtwdog_config.testMode=kRTWDOG_UserModeEnabled;	//ÓÃ»§Ä£Ê½
	rtwdog_config.clockSource=(rtwdog_clock_source_t)src;//Ê±ÖÓÔ´Ñ¡Ôñ
	rtwdog_config.prescaler=prescal;				//·ÖÆµ
	rtwdog_config.timeoutValue=toval;			    //³¬Ê±Öµ
	rtwdog_config.enableWindowMode =false;			//¹Ø±Õ´°¿Ú¹¦ÄÜ
	rtwdog_config.enableInterrupt=false;			//¹Ø±ÕÖÐ¶Ï
	rtwdog_config.enableRtwdog=true;				//Ê¹ÄÜ¿´ÃÅ¹·
	rtwdog_config.windowValue=win;                  //´°¿ÚÖµ(Ä¬ÈÏÉÏÃæ¹Ø±ÕÁË´°¿Ú¹¦ÄÜ)
	RTWDOG_Init(RTWDOG,&rtwdog_config);				//³õÊ¼»¯¿´ÃÅ¹·

	RT1052_NVIC_SetPriority(RTWDOG_IRQn,4,0);	    //ÇÀÕ¼ÓÅÏÈ¼¶4£¬×ÓÓÅÏÈ¼¶0
	NVIC_EnableIRQ(RTWDOG_IRQn);				    //Ê¹ÄÜRTWDOGÖÐ¶Ï
}

//RTWDOGÖÐ¶Ï·þÎñº¯Êý
void RTWDOG_IRQHandler(void)
{
    
    if((RTWDOG_GetStatusFlags(RTWDOG)&kRTWDOG_InterruptFlag)==kRTWDOG_InterruptFlag)
	{ 
	
	}	
    RTWDOG_ClearStatusFlags(RTWDOG,kRTWDOG_InterruptFlag); 
}

//Î¹¹·
void bsp_rtwog_feed(void)
{
	RTWDOG_Refresh(RTWDOG);
}

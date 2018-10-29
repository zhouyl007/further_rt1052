
#include "bsp_basetimer.h"
#include "bsp_ts_ft6336_sf.h"
#include "bsp_uartwifi.h"




//初始化QTIMER,TMR定时器的时钟源为IPG_CLK_ROOT=150MHz
//prisrc : 第一时钟源选择
//         0000~0011,通道0~3的输入引脚.
//         0100~0111,通道0~3的输出.可用于级联.
//         1000~1111,IPG_CLK_ROOT时钟的:1,2,4,8,16,32,64,128分频.
//cmp1	 : COMP1寄存器值,0~65535.
//定时时间=cmp1/QTMR1_CLK
//假设prisrc=1111,则QTMR1_CLK=1171875Hz.
//若cmp1=46875,则定时时间= 46875/1171875=0.04秒.
//当使用IPG_CLK_ROOT作为时钟源时,最大延时时间=65535*128/150M=55.9ms
void bsp_qtimer1_ch0int_init(uint8_t prisrc,uint16_t cmp1)
{
//QTIMER1配置结构体
	qtmr_config_t qtimer1_config;

	qtmr_primary_count_source_t qtimer1_source;
	
	uint32_t ipgclk=CLOCK_GetFreq(kCLOCK_IpgClk);            //获取IPG_CLK=150Mhz
	qtimer1_source=(qtmr_primary_count_source_t)prisrc;
	
	QTMR_GetDefaultConfig(&qtimer1_config);             //先设置为默认配置，后面在根据实际情况配置
	
	qtimer1_config.primarySource=qtimer1_source;        //设置第一时钟源
	QTMR_Init(TMR1,kQTMR_Channel_0,&qtimer1_config);    //初始化QTIMER
	QTMR_SetTimerPeriod(TMR1,kQTMR_Channel_0,cmp1);     //设置COMP1匹配值(向上计数用COMP1,向下用COMP2)

	//配置中断
	QTMR_EnableInterrupts(TMR1,kQTMR_Channel_0,kQTMR_CompareInterruptEnable);  //使能通道0的比较中断
	RT1052_NVIC_SetPriority(TMR1_IRQn,6,0);             //抢占优先级6，子优先级0
	EnableIRQ(TMR1_IRQn);                               //是能TMR3中断
			
	//开始通道0
	QTMR_StartTimer(TMR1, kQTMR_Channel_0,kQTMR_PriSrcRiseEdge);    //通道0在primary时钟源的上升沿计数
} 


void bsp_qtimer2_ch0int_init(uint8_t prisrc,uint16_t cmp1)
{
//QTIMER1配置结构体
	qtmr_config_t qtimer2_config;

	qtmr_primary_count_source_t qtimer2_source;
	
	uint32_t ipgclk=CLOCK_GetFreq(kCLOCK_IpgClk);            //获取IPG_CLK=150Mhz
	qtimer2_source=(qtmr_primary_count_source_t)prisrc;
	
	QTMR_GetDefaultConfig(&qtimer2_config);             //先设置为默认配置，后面在根据实际情况配置
	
	qtimer2_config.primarySource=qtimer2_source;        //设置第一时钟源
	QTMR_Init(TMR2,kQTMR_Channel_0,&qtimer2_config);    //初始化QTIMER
	QTMR_SetTimerPeriod(TMR2,kQTMR_Channel_0,cmp1);     //设置COMP1匹配值(向上计数用COMP1,向下用COMP2)

	//配置中断
	QTMR_EnableInterrupts(TMR2,kQTMR_Channel_0,kQTMR_CompareInterruptEnable);  //使能通道0的比较中断
	RT1052_NVIC_SetPriority(TMR2_IRQn,6,0);             //抢占优先级6，子优先级0
	EnableIRQ(TMR2_IRQn);                               //是能TMR3中断
			
	//开始通道0
	QTMR_StartTimer(TMR2, kQTMR_Channel_0,kQTMR_PriSrcRiseEdge);    //通道0在primary时钟源的上升沿计数
} 

//TMR1中断服务函数
void TMR1_IRQHandler(void)
{
	static uint8_t count=0;	    //计数器
	static uint8_t status=0;
	//判断是不是TMR1通道0的比较中断
	if((QTMR_GetStatus(TMR1,kQTMR_Channel_0)&kQTMR_CompareFlag)==kQTMR_CompareFlag)
	{
		FT5X06_OnePiontScan();
		count++;
		if(count==50)       //每进入25次中断服务函数,才执行一次.
		{			
			status++ ;
			rt_pin_write(74 , status%2);
		
			count=0;	        //清零计数器
		}
		QTMR_ClearStatusFlags(TMR1,kQTMR_Channel_0,kQTMR_CompareFlag); //清除中断标志位 
	}
	__DSB();                //数据同步屏蔽指令
}


//TMR2中断服务函数
uint8_t uart4Temp_buf[LPUART4_REC_LEN] = {0};
void TMR2_IRQHandler(void)
{
	static uint8_t Usart_LastLen ;
	uint8_t Usart_NewLen;
	static uint32_t count=0;	    //计数器
	//判断是不是TMR1通道0的比较中断
	if((QTMR_GetStatus(TMR2,kQTMR_Channel_0)&kQTMR_CompareFlag)==kQTMR_CompareFlag) {	
		count++ ;
		Usart_NewLen = (LPUART4_RX_STA&0xFF);

		if( (Usart_LastLen == Usart_NewLen) && (Usart_NewLen > 0)) {                
			// recv data and end
			Usart_NewLen=LPUART4_RX_STA&0xFF;                                     
			bsp_uart4_sendstr(LPUART4_RX_BUF,Usart_NewLen);
			LPUART4_RX_STA = 0;			
		}
		Usart_LastLen = (LPUART4_RX_STA&0xFF);
		if(1000 == count) {
			count = 0;
		}
		QTMR_ClearStatusFlags(TMR2,kQTMR_Channel_0,kQTMR_CompareFlag); //清除中断标志位 
	}
	__DSB();                //数据同步屏蔽指令
}



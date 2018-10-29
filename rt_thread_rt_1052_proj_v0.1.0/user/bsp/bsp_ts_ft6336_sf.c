
#include "bsp_ts_ft6336_sf.h"
#include "bsp_i2c_gpio.h"

#include "GUI.h"

FT5X06_T g_tFT5X06;



static void FT5X06_ReadReg(uint16_t _usRegAddr, uint8_t *_pRegBuf, uint8_t _ucLen);
static void FT5X06_WriteReg(uint16_t _usRegAddr, uint8_t *_pRegBuf, uint8_t _ucLen);
/*
*********************************************************************************************************
*	函 数 名: FT5X06_InitHard
*	功能说明: 配置触摸芯片
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/

static void FT5X06_Delay(void)
{
	uint16_t i;
	for (i = 0; i < 360; i++);
}


void FT5X06_InitHard(void)
{	

		uint8_t write_test =0 ;
		uint8_t intmodedat[1];

		gpio_pin_config_t tpgpio_config;
		
		bsp_InitI2C();
	// RST	GPIO_B1_13/GPIO2_IO29
	// INT	GPIO_B1_15/GPIO2_IO31
		IOMUXC_SetPinMux(IOMUXC_GPIO_B1_13_GPIO2_IO29,0);	 
		IOMUXC_SetPinMux(IOMUXC_GPIO_B1_15_GPIO2_IO31,0);		 
			
		IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_13_GPIO2_IO29,0x10B0);
		IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_15_GPIO2_IO31,0xF080);
	
		tpgpio_config.direction=kGPIO_DigitalOutput;  
		tpgpio_config.interruptMode=kGPIO_NoIntmode;  
		tpgpio_config.outputLogic=1; 				 
		GPIO_PinInit(GPIO2,29,&tpgpio_config); 				// RST				
			
		tpgpio_config.direction=kGPIO_DigitalInput;  
		tpgpio_config.interruptMode=kGPIO_NoIntmode;  
		tpgpio_config.outputLogic=1; 				 
		GPIO_PinInit(GPIO2,31,&tpgpio_config); 				// INT


		GPIO_PinWrite(GPIO2,29,0);
		FT5X06_Delay();
		GPIO_PinWrite(GPIO2,29,1);
		rt_thread_delay(500);	

		FT5X06_ReadID();
		FT5X06_ReadReg(FTS_REG_INT_MODE,intmodedat,1);
		FT5X06_Delay();
		intmodedat[0] = 0;
		FT5X06_WriteReg(FTS_REG_INT_MODE,intmodedat,1);
		FT5X06_ReadReg(FTS_REG_INT_MODE,&write_test,1);
	
		g_tFT5X06.TimerCount = 0;
		g_tFT5X06.Enable = 1;
}

/*
*********************************************************************************************************
*	函 数 名: FT5X06_PenInt
*	功能说明: 判断触摸按下
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t FT5X06_PenInt(void)
{
	if ( GPIO_PinRead(GPIO2, 31) == 0)
	{
		return 1;
	}
	return 0;
}

static void FT5X06_WriteReg(uint16_t _usRegAddr, uint8_t *_pRegBuf, uint8_t _ucLen)
{
	uint8_t i;

	i2c_Start();					/* 总线开始信号 */

	FT5X06_Delay();
	i2c_SendByte(FT5X06_I2C_ADDR);	/* 发送设备地址+写信号 */
	i2c_WaitAck();

	//i2c_SendByte(_usRegAddr >> 8);	/* 地址高8位 */
	//i2c_WaitAck();
	FT5X06_Delay();
	i2c_SendByte(_usRegAddr&0xFF);		/* 地址低8位 */
	i2c_WaitAck();
	FT5X06_Delay();
	for (i = 0; i < _ucLen; i++)
	{
		i2c_SendByte(_pRegBuf[i]);	
	  i2c_WaitAck();
		FT5X06_Delay();
	}
	i2c_Stop();							/* 总线停止信号 */
}


/*
*********************************************************************************************************
*	函 数 名: FT5X06_ReadReg
*	功能说明: 读1个或连续的多个寄存器
*	形    参: _usRegAddr : 寄存器地址
*			  _pRegBuf : 寄存器数据缓冲区
*			 _ucLen : 数据长度
*	返 回 值: 无
*********************************************************************************************************
*/
static void FT5X06_ReadReg(uint16_t _usRegAddr, uint8_t *_pRegBuf, uint8_t _ucLen)
{
	uint8_t i;

    i2c_Start();					/* 总线开始信号 */

    i2c_SendByte(FT5X06_I2C_ADDR);	/* 发送设备地址+写信号 */
	i2c_WaitAck();

    //i2c_SendByte(_usRegAddr >> 8);	/* 地址高8位 */
	//i2c_WaitAck();

    i2c_SendByte(_usRegAddr);		/* 地址低8位 */
	i2c_WaitAck();

	i2c_Start();
    i2c_SendByte(FT5X06_I2C_ADDR + 0x01);	/* 发送设备地址+读信号 */
	i2c_WaitAck();

	for (i = 0; i < _ucLen - 1; i++)
	{
	    _pRegBuf[i] = i2c_ReadByte();	/* 读寄存器数据 */
		i2c_Ack();
	}

	/* 最后一个数据 */
	 _pRegBuf[i] = i2c_ReadByte();		/* 读寄存器数据 */
	i2c_NAck();

    i2c_Stop();							/* 总线停止信号 */
}

/*
*********************************************************************************************************
*	函 数 名: FT5X06_Timer1ms
*	功能说明: 每隔1ms调用1次
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void FT5X06_Timer1ms(void)
{
	g_tFT5X06.TimerCount++;
}

/*
*********************************************************************************************************
*	函 数 名: FT5X06_OnePiontScan
*	功能说明: FT5X06_OnePiontScan函数仅读取了电容触摸屏的一次触摸，在bsp_ts_ft5x06.h文件里面设置参数
*             #define FT5X06_TOUCH_POINTS   1
*             专门用于emWin。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/

GUI_PID_STATE State;

void FT5X06_OnePiontScan(void)
{
	uint8_t buf[CFG_POINT_READ_BUF];
	uint8_t i;
	static uint8_t s_tp_down = 0;
	uint16_t x, y;

	if (g_tFT5X06.Enable == 0)
	{
		return;
	}
	
	if (FT5X06_PenInt() == 0)
	{
		return;
	}
	
	FT5X06_ReadReg(2, buf, 1);	
	
	if( 0 == (buf[0] & 0x0F) ) {  // no touch 
		if (s_tp_down == 1)
		{
			/* State.x?State.y???????,State?????,???????????? */
			s_tp_down = 0;
			State.Pressed = 0;
			GUI_PID_StoreState(&State);	/* ?? */
		}
		return ;
	}
	
	/* 有触摸，读取完整的数据，这里读取了一次 */
	FT5X06_ReadReg(0, buf, CFG_POINT_READ_BUF);
		
	g_tFT5X06.X[0] = (int16_t)(buf[3 ] & 0x0F)<<8 | (int16_t)buf[4 ];
	g_tFT5X06.Y[0] = (int16_t)(buf[5 ] & 0x0F)<<8 | (int16_t)buf[6 ];

	x = g_tFT5X06.X[0];
	y = g_tFT5X06.Y[0];
	/* 判断值域 */
	if (x > 799)
	{
		x = 799;
	}			
	if (y > 1279)
	{
		y = 1279;
	}			

	
	if (s_tp_down == 0)
	{
		s_tp_down = 1;
		State.x = x;
		State.y = y;
		State.Pressed = 1;
		GUI_PID_StoreState(&State);	
	}
	else
	{
		State.x = x;
		State.y = y;
		State.Pressed = 1;
		GUI_PID_StoreState(&State);	
	}

}



/*
*********************************************************************************************************
*	函 数 名: FT5X06_ReadID
*	功能说明: 读芯片ID, 识别是4.3、5.0、7.0寸触摸。
*			实测结果:  4.3寸id = 0x55    5.0寸id = 0x0A  7.0寸id = 0x06
*	形    参: 无
*	返 回 值: 1字节芯片ID
*********************************************************************************************************
*/
uint8_t FT5X06_ReadID(void)
{
	uint8_t id;
	
	FT5X06_ReadReg(FTS_REG_CHIP_ID, &id, 1);
	
	g_tFT5X06.ChipID = id;		/* 保存id */
	return id;
}


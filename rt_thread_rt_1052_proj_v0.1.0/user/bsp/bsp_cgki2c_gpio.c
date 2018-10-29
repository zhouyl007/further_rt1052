/*
*********************************************************************************************************
*
*	模块名称 : I2C总线驱动模块
*	文件名称 : bsp_i2c_gpio.c
*	版    本 : V1.0
*	说    明 : 用gpio模拟i2c总线,  
*
*
*********************************************************************************************************
*/

/*
	应用说明：
	在访问I2C设备前，请先调用 i2c_CheckDevice() 检测I2C设备是否正常，该函数会配置GPIO
*/

#include "bsp_cgki2c_gpio.h"



// SCL  GPIO_AD_B1_00/GPIO1_IO16/
// SDA  GPIO_AD_B1_01/GPIO1_IO17

#define IIC_CGKSCL0    GPIO_PinWrite(GPIO1,16,0)  	  // SCL
#define IIC_CGKSCL1    GPIO_PinWrite(GPIO1,16,1)	  // SCL

#define IIC_CGKSDA0    GPIO_PinWrite(GPIO1,17,0)	  // SDA
#define IIC_CGKSDA1    GPIO_PinWrite(GPIO1,17,1)	  // SDA 
 

#define READ_CGKSDA    GPIO_PinRead(GPIO1, 17)


void bsp_cgkseti2csda_out()
{
	gpio_pin_config_t i2cgpio_config;

	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_01_GPIO1_IO17,0x10B0);   // SDA
	// SCL	GPIO_AD_B1_00/GPIO1_IO16/
	// SDA	GPIO_AD_B1_01/GPIO1_IO17

	i2cgpio_config.direction=kGPIO_DigitalOutput;  
	i2cgpio_config.interruptMode=kGPIO_NoIntmode;  
	i2cgpio_config.outputLogic=1; 				 
	GPIO_PinInit(GPIO1,17,&i2cgpio_config); 				// SDA out
}

void bsp_cgkseti2csda_in()
{
	gpio_pin_config_t i2cgpio_config;

	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_01_GPIO1_IO17,0xF080);
	// SCL	GPIO_AD_B1_00/GPIO1_IO16/
	// SDA	GPIO_AD_B1_01/GPIO1_IO17

	i2cgpio_config.direction=kGPIO_DigitalInput;  
	i2cgpio_config.interruptMode=kGPIO_NoIntmode;  
	i2cgpio_config.outputLogic=1; 				 
	GPIO_PinInit(GPIO1,17,&i2cgpio_config); 				// SDA in
}


/*
*********************************************************************************************************
*	函 数 名: bsp_InitI2C
*	功能说明: 配置I2C总线的GPIO，采用模拟IO的方式实现
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_cgkInitI2C(void)
{
	gpio_pin_config_t i2cgpio_config;
	
	// SCL	GPIO_AD_B1_00/GPIO1_IO16/
	// SDA	GPIO_AD_B1_01/GPIO1_IO17

	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_00_GPIO1_IO16,0);	 
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_01_GPIO1_IO17,0);     
    
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_00_GPIO1_IO16,0x10B0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_01_GPIO1_IO17,0x10B0);

	i2cgpio_config.direction=kGPIO_DigitalOutput;	 
	i2cgpio_config.interruptMode=kGPIO_NoIntmode;	 
	i2cgpio_config.outputLogic=1;					 
	GPIO_PinInit(GPIO1,16,&i2cgpio_config); 	      // SCL        
    
 	i2cgpio_config.direction=kGPIO_DigitalOutput;	 
	i2cgpio_config.interruptMode=kGPIO_NoIntmode;	 
	i2cgpio_config.outputLogic=1;					 
	GPIO_PinInit(GPIO1,17,&i2cgpio_config);         // SDA

	/* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
	i2c_cgkStop();
}


/*
*********************************************************************************************************
*	函 数 名: i2c_Delay
*	功能说明: I2C总线位延迟，最快400KHz
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	uint16_t i;

	for (i = 0; i < 200; i++);//此事件非常重要，关系到能否读到正确的数据
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线启动信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_cgkStart(void)
{
	bsp_cgkseti2csda_out();	 
	IIC_CGKSDA1;		  
	IIC_CGKSCL1;
	i2c_Delay();
	IIC_CGKSDA0;	//	START:when CLK is high,DATA change form high to low 
	i2c_Delay();
	IIC_CGKSCL0;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线停止信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_cgkStop(void)
{
	bsp_cgkseti2csda_out();
	IIC_CGKSCL0;
	IIC_CGKSDA0;	//	STOP:when CLK is high DATA change form low to high
	i2c_Delay();
	IIC_CGKSCL1; 
	IIC_CGKSDA1;
	i2c_Delay();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_SendByte
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参:  _ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_cgkSendByte(uint8_t _ucByte)
{
	uint8_t t;	 
	bsp_cgkseti2csda_out();		
	
	for(t=0;t<8;t++)
	{    
		IIC_CGKSCL0;		
		i2c_Delay();   
		if(_ucByte&0x80) {
			IIC_CGKSDA1;
		}else {
			IIC_CGKSDA0;
		}
		i2c_Delay();   
		IIC_CGKSCL1;
		i2c_Delay(); 
		_ucByte<<=1;	  
	}
	IIC_CGKSCL0;		
	i2c_Delay(); 
}

/*
*********************************************************************************************************
*	函 数 名: i2c_ReadByte
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参:  无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t i2c_cgkReadByte(void)
{	
	uint8_t i;
	uint8_t receive=0;
	IIC_CGKSDA1;
	bsp_cgkseti2csda_in();
	for(i=0;i<8;i++ )
	{
		IIC_CGKSCL1; 
		i2c_Delay();	
		receive<<=1;
		if(READ_CGKSDA)
			receive++; 
		i2c_Delay();		
		IIC_CGKSCL0; 
		i2c_Delay();
	}					 
	return receive;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参:  无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
uint8_t i2c_cgkWaitAck(void)
{
	uint16_t ucErrTime=0;
	bsp_cgkseti2csda_in();
 
	while(READ_CGKSDA)
	{
		ucErrTime++;
		if(ucErrTime>100)
		{
			i2c_cgkStop();
			return 1;
		}
	}
	IIC_CGKSCL1;
	i2c_Delay();	
	IIC_CGKSCL0;	
	i2c_Delay();
	return 0; 
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_cgkAck(void)
{
	bsp_cgkseti2csda_out();
	IIC_CGKSCL0;
	IIC_CGKSDA0;
	i2c_Delay();
	IIC_CGKSCL1;
	i2c_Delay();
	IIC_CGKSCL0;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_cgkNAck(void)
{
	bsp_cgkseti2csda_out();
	IIC_CGKSCL0;
	IIC_CGKSDA1;
	i2c_Delay();
	IIC_CGKSCL1;
	i2c_Delay();
	IIC_CGKSCL0;
}






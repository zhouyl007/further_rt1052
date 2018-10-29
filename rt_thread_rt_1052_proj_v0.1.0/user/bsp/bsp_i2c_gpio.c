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

#include "bsp_i2c_gpio.h"



// SCL  GPIO_AD_B1_07/GPIO1_IO23
// SDA  GPIO_AD_B1_06/GPIO1_IO22

#define IIC_SCL0    GPIO_PinWrite(GPIO1,23,0)  	// SCL
#define IIC_SCL1    GPIO_PinWrite(GPIO1,23,1)	  // SCL

#define IIC_SDA0    GPIO_PinWrite(GPIO1,22,0)	  // SDA
#define IIC_SDA1    GPIO_PinWrite(GPIO1,22,1)	  // SDA 
 

#define READ_SDA    GPIO_PinRead(GPIO1, 22)


void bsp_seti2csda_out()
{
	gpio_pin_config_t i2cgpio_config;

	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_06_GPIO1_IO22,0x10B0);
// SDA	GPIO_AD_B1_06/GPIO1_IO22
	i2cgpio_config.direction=kGPIO_DigitalOutput;  
	i2cgpio_config.interruptMode=kGPIO_NoIntmode;  
	i2cgpio_config.outputLogic=1; 				 
	GPIO_PinInit(GPIO1,22,&i2cgpio_config); 				// SDA out
}

void bsp_seti2csda_in()
{
	gpio_pin_config_t i2cgpio_config;

	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_06_GPIO1_IO22,0xF080);
// SDA	GPIO_AD_B1_06/GPIO1_IO22
	i2cgpio_config.direction=kGPIO_DigitalInput;  
	i2cgpio_config.interruptMode=kGPIO_NoIntmode;  
	i2cgpio_config.outputLogic=1; 				 
	GPIO_PinInit(GPIO1,22,&i2cgpio_config); 				// SDA in
}


/*
*********************************************************************************************************
*	函 数 名: bsp_InitI2C
*	功能说明: 配置I2C总线的GPIO，采用模拟IO的方式实现
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitI2C(void)
{
	gpio_pin_config_t i2cgpio_config;
	
// SCL  GPIO_AD_B1_07/GPIO1_IO23
// SDA  GPIO_AD_B1_06/GPIO1_IO22
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_07_GPIO1_IO23,0);	 
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_06_GPIO1_IO22,0);     
    
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_07_GPIO1_IO23,0x10B0);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_06_GPIO1_IO22,0x10B0);

	i2cgpio_config.direction=kGPIO_DigitalOutput;	 
	i2cgpio_config.interruptMode=kGPIO_NoIntmode;	 
	i2cgpio_config.outputLogic=1;					 
	GPIO_PinInit(GPIO1,23,&i2cgpio_config); 	      // SCL        
    
  i2cgpio_config.direction=kGPIO_DigitalOutput;	 
	i2cgpio_config.interruptMode=kGPIO_NoIntmode;	 
	i2cgpio_config.outputLogic=1;					 
	GPIO_PinInit(GPIO1,22,&i2cgpio_config);         // SDA

	/* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
	i2c_Stop();
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
void i2c_Start(void)
{
	bsp_seti2csda_out();     
	IIC_SDA1;	  	  
	IIC_SCL1;
	i2c_Delay();
 	IIC_SDA0;	//	START:when CLK is high,DATA change form high to low 
	i2c_Delay();
	IIC_SCL0;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线停止信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Stop(void)
{
	bsp_seti2csda_out();
	IIC_SCL0;
	IIC_SDA0;	//	STOP:when CLK is high DATA change form low to high
 	i2c_Delay();
	IIC_SCL1; 
	IIC_SDA1;
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
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t t;   
	bsp_seti2csda_out(); 	    
  
  for(t=0;t<8;t++)
  {    
		IIC_SCL0;		
		i2c_Delay();   
		if(_ucByte&0x80) {
			IIC_SDA1;
		}else {
			IIC_SDA0;
		}
		i2c_Delay();   
		IIC_SCL1;
		i2c_Delay(); 
		_ucByte<<=1; 	  
  }
	IIC_SCL0;		
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
uint8_t i2c_ReadByte(void)
{	
	uint8_t i;
	uint8_t receive=0;
	IIC_SDA1;
	bsp_seti2csda_in();
	for(i=0;i<8;i++ )
	{
		IIC_SCL1; 
		i2c_Delay();	
		receive<<=1;
		if(READ_SDA)
			receive++; 
		i2c_Delay();		
		IIC_SCL0; 
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
uint8_t i2c_WaitAck(void)
{
	uint16_t ucErrTime=0;
	bsp_seti2csda_in();
	i2c_Delay();
 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>1000)
		{
			i2c_Stop();
			return 1;
		}
	}
	IIC_SCL1;
	i2c_Delay();	
	IIC_SCL0;   
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
void i2c_Ack(void)
{
	bsp_seti2csda_out();
	IIC_SCL0;
	IIC_SDA0;
	i2c_Delay();
	IIC_SCL1;
	i2c_Delay();
	IIC_SCL0;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_NAck(void)
{
	bsp_seti2csda_out();
	IIC_SCL0;
	IIC_SDA1;
	i2c_Delay();
	IIC_SCL1;
	i2c_Delay();
	IIC_SCL0;
}



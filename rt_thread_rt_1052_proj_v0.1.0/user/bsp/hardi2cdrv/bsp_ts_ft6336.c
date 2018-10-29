/*
*********************************************************************************************************
*
*	NAME      :    ft6336 touch driver
*	FILENMAME :    bsp_ts_ft6336.c
*   AUTHOR    :    Bearpeak
*	VERSION   :    V1.0
*
*********************************************************************************************************
*/

#include "GUI.h"
#include "bsp_ts_ft6336.h"
#include "bsp_i2cdev_init.h"



FT6336_T g_tFT6336;


static uint8_t ft6336_readreg(uint8_t *_usRegAddr, uint8_t *_pRegBuf, uint8_t _ucLen);
static uint8_t ft6336_writereg(uint8_t *_usRegAddr,uint8_t _pRegDat, uint8_t _ucLen);

/*
*********************************************************************************************************
// file name: ft6336_inithard
// return 0 : open touch device is ok
// return 1 : cgk i2c device is not inits
// return 2 : cgk i2c device addr is not correct
*********************************************************************************************************/
uint8_t ft6336_inithard(void )
{	
	struct rt_i2c_msg msgs[1];
	uint8_t len = 0;
	uint8_t intmodedat;
	uint8_t ft6336_addrcmd[] = {GEST_ID};
  uint8_t ft6336_intmodecmd[] = {FTS_REG_INT_MODE};
	
	msgs[0].flags	 = RT_I2C_WR;
	msgs[0].buf    = ft6336_addrcmd;
	msgs[0].len 	 = 1;
	msgs[0].addr   = FT6336_I2C_ADDR;


 	if(i2c1dev == NULL) {
		rt_kprintf("ft6336 i2c bus is not init .\n");
		g_tFT6336.ucEnable = 0;
   	return 1;
 	}

	len = rt_i2c_transfer((struct rt_i2c_bus_device *)i2c1dev, msgs, 1);
	if (len == 1) {
		g_tFT6336.ucDeviceAddr = FT6336_I2C_ADDR;
    g_tFT6336.ucEnable = 1;  
		g_tFT6336.ucTimerCount = 0;
		rt_kprintf("the ft6336 i2c addr is ok %2x \n", g_tFT6336.ucDeviceAddr);	
		rt_pin_mode(FT6336_INT_PIN, PIN_MODE_INPUT);

    ft6336_readreg(ft6336_intmodecmd,&intmodedat,1);
	  ft6336_writereg(ft6336_intmodecmd,~(intmodedat&0x01),1);
		return 0;
	}
	rt_kprintf("the ft6336 i2c addr is error,please check it addr :%2x \n", FT6336_I2C_ADDR);	

  return 2;
}

/*
*********************************************************************************************************
*	函 数 名: FT6336_PenInt
*	功能说明: 判断触摸按下
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t ft6336_penint(void)
{

	if( rt_pin_read(FT6336_INT_PIN) == 0 ){
		return 1;
	}
	return 0;
}



/*
*********************************************************************************************************
*	NAME      : gt911_ReadReg
*	parameter : dev        : Operating's equipment
*	            _usRegAddr : gt911 reg address
*			    _pRegBuf   : data save buffer
*			    _ucLen     : want to read data len
*	return    : 0          : succeed read reg data
*	            1          : read reg data failed 
*	            2          : touch device is not open 
*********************************************************************************************************
*/
static uint8_t ft6336_readreg(uint8_t *_usRegAddr,uint8_t *_pRegBuf, uint8_t _ucLen)
{
	uint8_t len = 0;

	struct rt_i2c_msg msgs[2];

	if( 1 != g_tFT6336.ucEnable ) {
		return 2;
	}	
	msgs[0].addr   = g_tFT6336.ucDeviceAddr;
	msgs[0].flags  = RT_I2C_WR;
	msgs[0].buf    = _usRegAddr;
	msgs[0].len    = 1;

	msgs[1].addr   = g_tFT6336.ucDeviceAddr;
	msgs[1].flags  = RT_I2C_RD;
	msgs[1].buf    = _pRegBuf;
	msgs[1].len    = _ucLen;	

  len = rt_i2c_transfer((struct rt_i2c_bus_device *)i2c1dev, msgs, 2);
  if (len == 2) {
      // have succeed read data
      return 0;
  }else {
		rt_kprintf("i2c read reg failed \n");
	}
	return 1;   // read data is error 
}


/*
*********************************************************************************************************
*	NAME      : ft6336_writereg
*	parameter : dev        : Operating's equipment
*	            _usRegAddr : gt911 reg address
*			    _pRegDat   : data want send to reg
*			    _ucLen     : want to read data len
*	return    : 0          : succeed write reg data
*	            1          : write reg data failed 
*	            2          : touch device is not open 
*********************************************************************************************************
*/
static uint8_t ft6336_writereg(uint8_t *_usRegAddr,uint8_t _pRegDat, uint8_t _ucLen)
{
	uint8_t len = 0;
	
  struct rt_i2c_msg msgs[1];

	if( 1 != g_tFT6336.ucEnable ) {
		return 2;
	}	
	msgs[0].addr    = g_tFT6336.ucDeviceAddr;
	msgs[0].flags   = RT_I2C_WR;
	msgs[0].buf     = _usRegAddr;
	msgs[0].buf[1]  = _pRegDat;
	msgs[0].len     = 2;
	
  len = rt_i2c_transfer((struct rt_i2c_bus_device *)i2c1dev, msgs, 1);
  if (len == 1) {
      // have read data
      return 0;
  }else {
		  rt_kprintf("i2c write reg failed \n");
	} 
	return 1;   // read data is error 
}


/*
*********************************************************************************************************
*	函 数 名: FT6336_Timer1ms
*	功能说明: 每隔1ms调用1次
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void ft6336_timer1ms(void)
{
	g_tFT6336.ucTimerCount++;
}


/*
*********************************************************************************************************
*	NAME     : gt911_readid
*	         : void
*	return	   0 : succeed read product id
*              1 : read product id failed
*	 
*********************************************************************************************************
*/
uint8_t ft6336_readid(void) 
{ 
	uint8_t id;
	uint8_t productcmd[] = {FTS_REG_CHIP_ID};
	
	ft6336_readreg(productcmd, &id, 1);
	
	g_tFT6336.ucChipID = id;		/* get id */
	return id;
} 



/*
*********************************************************************************************************
*	NAME      : ft6336_onepiont_scan
*	parameter :            
*	return    : 0          : succeed write touch data
*	            1          : read  reg data failed 
*               2          : write reg data failed
*               3          : no init touch 
*               4          : no get int signal
*               5          : no touch data

*********************************************************************************************************
*/
GUI_PID_STATE State;
uint8_t ft6336_osnepiont_scan(void)
{
	uint8_t buf[CFG_POINT_READ_BUF];
	static uint8_t s_tp_down = 0;
	uint16_t x, y;
	uint8_t datacmd[] = {TD_STATUS};
	uint8_t read_touchcmd[] = {TS_DEVICE_MODE};

	if (g_tFT6336.ucEnable == 0) {
		return 3;
	}


	if (ft6336_penint() == 0) {
			return 4;
	}

//	rt_kprintf("have enter ft6336 \n");

	if( 0 != ft6336_readreg(datacmd , buf , 1) ){
			return 1;
	}

  // check touch if down
	if( 0 == (buf[0] & 0x0F) ) {  // no touch 
		if (s_tp_down == 1)
		{
			/* State.x?State.y???????,State?????,???????????? */
			s_tp_down = 0;
			State.Pressed = 0;
			GUI_PID_StoreState(&State);	/* ?? */
		}
		return 3;
	}
	
	/* 有触摸，读取完整的数据，这里读取了一次 */
	if( 0 != ft6336_readreg(read_touchcmd , buf , CFG_POINT_READ_BUF) ){
		return 1;
	}
	

	g_tFT6336.sX[0] = (int16_t)(buf[3] & 0x0F)<<8 | (int16_t)buf[4];
	g_tFT6336.sY[0] = (int16_t)(buf[5] & 0x0F)<<8 | (int16_t)buf[6];
	g_tFT6336.ucEvent[0] = buf[0x3] >> 6;
	g_tFT6336.ucid[0] = (buf[5])>>4;

	
	x = g_tFT6336.sX[0];
	y = g_tFT6336.sY[0];	
	
	if (x > 799) {
		x = 799;
	}			
	if (y > 1279) {
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

#if 0  
	rt_kprintf("(%5d,%5d,%3d,%3d) ",  g_tFT6336.sX[0], g_tFT6336.sY[0], g_tFT6336.ucEvent[0],  g_tFT6336.ucid[0]);
#endif	
  return 0;
}



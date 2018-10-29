/*
*********************************************************************************************************
*
*	NAME      :    gt911 touch driver
*	FILENMAME :    bsp_ts_gt911.c
*   AUTHOR    :    Bearpeak
*	VERSION   :    V1.0
*
*********************************************************************************************************
*/

#include "bsp_ts_gt911.h"


GT911_T  g_tGT911;

rt_device_t touchdev;


/*
#define RCC_TP_INT					RCC_AHB1Periph_GPIOH
#define PORT_TP_INT					GPIOH
#define PIN_TP_INT					GPIO_Pin_7

#define RCC_TP_RST                  RCC_AHB1Periph_GPIOH
#define PORT_TP_RST                 GPIOH
#define PIN_TP_RST                  GPIO_Pin_6
*/

static uint8_t gt911_readreg(uint8_t  *_usRegAddr,uint8_t *_pRegBuf, uint8_t _ucLen);
static uint8_t gt911_writereg(uint8_t *_usRegAddr,uint8_t _pRegDat, uint8_t _ucLen);

static void gt911_Delay(void)
{
	uint16_t i;
	for (i = 0; i < 360; i++);
}


// return 0 : open touch device is ok
uint8_t gt911_inithard(char *devicename)
{	
	uint8_t searchcmd[] = {GT911_PRODUCT_ID_REG>>8,GT911_PRODUCT_ID_REG};
    struct rt_i2c_msg msgs[1];
    msgs[0].flags    = RT_I2C_WR;
    msgs[0].buf      = searchcmd;
    msgs[0].len      = 2;

    touchdev = rt_device_find(devicename);
    if (!touchdev) {
        rt_kprintf("search gt911 device failed: %s\n", devicename);
		g_tGT911.Enable = 0;
        return 1;
    }

    if (rt_device_open(touchdev, RT_DEVICE_OFLAG_RDWR) != RT_EOK) {
        rt_kprintf("open gt911 device failed: %s\n", devicename);
		g_tGT911.Enable = 0;
        return 2;
    }

//  set int gpio mode is input
    rt_pin_mode(GT911_INT_PIN, PIN_MODE_INPUT_PULLDOWN);

    rt_kprintf("open gt911 i2c bus: %s\n", devicename);

//  search addr
   
    for (int i = 0; i < 0xFF ; i++) {
        uint8_t len = 0;
		msgs[0].addr  = i;
        len = rt_i2c_transfer((struct rt_i2c_bus_device *)touchdev, msgs, 1);
        if (len == 1) {
			g_tGT911.DeviceAddr = i;
            rt_kprintf("Search device addr : %02X \n", g_tGT911.DeviceAddr);			
        }
    }

	g_tGT911.DeviceAddr = 0x5D;
	g_tGT911.TimerCount = 0;
	g_tGT911.Enable = 1;
	
    return 0;
}

uint8_t gt911_penint(void)
{
    if( rt_pin_read(GT911_INT_PIN) ){
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
static uint8_t gt911_readreg(uint8_t *_usRegAddr,uint8_t *_pRegBuf, uint8_t _ucLen)
{
	uint8_t len = 0;
	
    struct rt_i2c_msg msgs[2];

    if( 1 != g_tGT911.Enable ) {
		return 2;
	}	
	msgs[0].addr  = g_tGT911.DeviceAddr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = _usRegAddr;
    msgs[0].len   = 2;

	msgs[1].addr  = g_tGT911.DeviceAddr;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = _pRegBuf;
    msgs[1].len   = _ucLen;	

    len = rt_i2c_transfer((struct rt_i2c_bus_device *)touchdev, msgs, 2);
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
*	NAME      : gt911_writereg
*	parameter : dev        : Operating's equipment
*	            _usRegAddr : gt911 reg address
*			    _pRegDat   : data want send to reg
*			    _ucLen     : want to read data len
*	return    : 0          : succeed write reg data
*	            1          : write reg data failed 
*	            2          : touch device is not open 
*********************************************************************************************************
*/
static uint8_t gt911_writereg(uint8_t *_usRegAddr,uint8_t _pRegDat, uint8_t _ucLen)
{
	uint8_t len = 0;
	
    struct rt_i2c_msg msgs[1];

	if( 1 != g_tGT911.Enable ) {
		return 2;
	}	
	msgs[0].addr    = g_tGT911.DeviceAddr;
    msgs[0].flags   = RT_I2C_WR;
    msgs[0].buf     =  _usRegAddr;
	msgs[0].buf[2]  =  _pRegDat;
    msgs[0].len     = 3;
	
    len = rt_i2c_transfer((struct rt_i2c_bus_device *)touchdev, msgs, 1);
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
*	函 数 名: gt911_Timer1ms
*	功能说明: 每隔1ms调用1次
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void gt911_timer1ms(void)
{
	g_tGT911.TimerCount++;
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
uint8_t *gt911_readid(void) 
{ 
    uint8_t productcmd[] = {GT911_PRODUCT_ID_REG>>8,GT911_PRODUCT_ID_REG};
	/* Product_ID*/ 
	if( 1 != g_tGT911.Enable ) {
		g_tGT911.ChipID[0] = 0;
		return NULL;
	}
	if( 0 != gt911_readreg( productcmd,g_tGT911.ChipID , 4) ) {
		rt_kprintf("get gt911 product id failed \r\n");
	}
	g_tGT911.ChipID[4] = 0;
	return g_tGT911.ChipID;
} 


/*
*********************************************************************************************************
*	NAME      : gt911_OnePiontScan
*   parameter : void 
*
*   return    : void
*	 
*********************************************************************************************************
*/
//extern GUI_PID_STATE State;
/*
*********************************************************************************************************
*	NAME      : gt911_onepiont_scan
*	parameter :  
*	            
*	return    : 0          : succeed write touch data
*	            1          : read  reg data failed 
*               2          : write reg data failed
*               3          : no init touch 
*               4          : no get int signal
*               5          : no touch data

*********************************************************************************************************
*/
uint8_t gt911_onepiont_scan(void)
{
	uint8_t readxycmd[] = {GT911_READ_XY_REG>>8,GT911_READ_XY_REG};
	uint8_t clearbuf = 0;
	uint16_t x, y;
	uint8_t buf[GT911_POINT_READ_BUF];
	static uint8_t s_tp_down = 0;

	if (g_tGT911.Enable == 0) {
		return 3;
	}
	/*
	if (gt911_penint() == 0) {
		return 4;
	}*/
	
    if( 0 != gt911_readreg(readxycmd , buf , GT911_POINT_READ_BUF) ){
        return 1;
	}
	if( 0 != gt911_writereg(readxycmd , clearbuf, 1) ) {
		return 2; 
	}	
	if( 0 == (buf[0] & 0x0F) ) {  // no touch 
		return 3;
	}

	g_tGT911.X[0] = ((uint16_t)buf[3] << 8) + buf[2]; 
	g_tGT911.Y[0] = ((uint16_t)buf[5] << 8) + buf[4]; 
	
	x = g_tGT911.X[0];
	y = g_tGT911.Y[0];	
	
// Prevent coordinate overflow
	if (x > 1279) {
		x = 1279;
	}			
	if (y > 799) {
		y = 799;
	}			
	
#if 1 
	rt_kprintf("X : %5d Y : %5d E : %3d ID : %3d) ",  g_tGT911.X[0], g_tGT911.Y[0], g_tGT911.Event[0],  g_tGT911.id[0]);
#endif
    return 0;
}







#include "bsp_cgk_module.h"
#include "bsp_i2cdev_init.h"
#include "public_h.h"

CgkSensorData g_CgkSensorData;


// return 0 : open touch device is ok
// return 1 : cgk i2c device is not inits
// return 2 : cgk i2c device addr is not correct
uint8_t cgk_inithard(void)
{	
	struct rt_i2c_msg msgs[1];
	uint8_t len = 0;
  uint8_t cgk_addrcmd[] = {CO2MEASURE};
	
  msgs[0].flags	 = RT_I2C_WR;
	msgs[0].buf    = cgk_addrcmd;
	msgs[0].len 	 = 1;
	msgs[0].addr   = CGKI2CADDR;

 	if(i2c1dev == NULL) {
		rt_kprintf("cgk i2c bus is not init .\n");
		g_CgkSensorData.ucEnable = 0;
   	return 1;
 	}

	len = rt_i2c_transfer((struct rt_i2c_bus_device *)i2c1dev, msgs, 1);
	if (len == 1) {
		g_CgkSensorData.ucDeviceAddr = CGKI2CADDR;
    g_CgkSensorData.ucEnable = 1;  		
		rt_kprintf("the cgk i2c addr is ok %2x \n", g_CgkSensorData.ucDeviceAddr);	
		return 0;
	}
	rt_kprintf("the cgk i2c addr is error,please check it addr :%2x \n", CGKI2CADDR);	
  return 2;
}



/**
  * @brief  CGKSendByte_S , 
  *         
  * @param  SlaAdd is the device addr , SendDat is send cmd 
  * @retval None 
  */
static uint8_t Cgk_sendcmd(uint8_t _ucCmd)
{
	uint8_t len = 0;
	uint8_t cgk_cmd[1];
	cgk_cmd[0] = _ucCmd;
  struct rt_i2c_msg msgs[1];

	if( 1 != g_CgkSensorData.ucEnable ) {
		return 2;
	}	
//    g_CgkSensorData.ucDeviceAddr = CGKI2CADDR;
	msgs[0].addr    = g_CgkSensorData.ucDeviceAddr;
	msgs[0].flags   = RT_I2C_WR;
	msgs[0].buf     = cgk_cmd;
	msgs[0].len     = 1;
	
  len = rt_i2c_transfer((struct rt_i2c_bus_device *)i2c1dev, msgs, 1);
  if (len == 1) {
    // have read data
    return 0;
  }else {
		rt_kprintf("i2c write reg failed \n");
	}
	return 1;   // read data is error 
}



static uint8_t cgk_readsevendata(uint32_t *_uiReadDat)
{
	uint8_t iCount = 0, checkSum = 0;
	uint8_t tempDat[7] = {0};

	uint8_t len = 0;

	struct rt_i2c_msg msgs[1];

	if( 1 != g_CgkSensorData.ucEnable ) {
		return 2;
	}	

	msgs[0].addr  = g_CgkSensorData.ucDeviceAddr;
	msgs[0].flags = RT_I2C_RD;
	msgs[0].buf   = tempDat;
	msgs[0].len   = 7; 

  len = rt_i2c_transfer((struct rt_i2c_bus_device *)i2c1dev, msgs, 1);
 	  if (len != 1) {
			rt_kprintf("i2c read reg failed \n");		
		return 3;
  }
	
  // check the sum
	for(iCount =0 ; iCount < 7; iCount++ ){
		checkSum += tempDat[iCount];			
	}		
	if(checkSum != 0x00){
		return 4;
	}
	//rt_kprintf("i2c read 7 data is ok \n");	

	// checksum is ok
	//*_uiPm03Dat = (uint32_t)( tempDat[0]*256*256*256 + tempDat[1]*256*256 + tempDat[2]*256 + tempDat[3] ) ;
	*_uiReadDat = (uint32_t)( (tempDat[0]<<8) | tempDat[1] ) ;
	return 0;
}

static uint8_t cgk_readpmdata(CgkSensorData *g_CgkSensorDataP)
{
	uint8_t iCount = 0, checkSum = 0;
	uint8_t tempDat[17] = {0};

	uint8_t len = 0;

	struct rt_i2c_msg msgs[1];

	if( 1 != g_CgkSensorData.ucEnable ) {
		return 2;
	}	

	msgs[0].addr  = g_CgkSensorData.ucDeviceAddr;
	msgs[0].flags = RT_I2C_RD;
	msgs[0].buf   = tempDat;
	msgs[0].len   = 17; 

    len = rt_i2c_transfer((struct rt_i2c_bus_device *)i2c1dev, msgs, 1);
    if (len != 1) {
		rt_kprintf("i2c read reg failed \n");		
		return 3;
    }

	for(iCount =0 ; iCount < 17; iCount++ ){
	  checkSum += tempDat[iCount];			  
	  //putchar(tempDat[iCount]);
	} 	  
	if(checkSum != 0x00){
	  return 4;
	}
	
	// checksum is ok
	//*_uiPm03Dat = (uint32_t)( tempDat[0]*256*256*256 + tempDat[1]*256*256 + tempDat[2]*256 + tempDat[3] ) ;
	g_CgkSensorDataP->uiPm03Dat = (uint32_t)( (tempDat[0]<<24) | (tempDat[1]<<16) | (tempDat[2]<<8) | tempDat[3] ) ;
	g_CgkSensorDataP->uiPm25Dat = (uint32_t)( (tempDat[4]<<24) | (tempDat[5]<<16) | (tempDat[6]<<8) | tempDat[7] ) ;
	g_CgkSensorDataP->uiPm10Dat = (uint32_t)( (tempDat[8]<<24) | (tempDat[9]<<16) | (tempDat[10]<<8) | tempDat[11] ) ;

	return 0;

}


//  CGK_CMD_TYPE
//	ELECMEASURE = 0x01,
//	PMMEASURE   = 0x05,
//	CO2MEASURE  = 0x06,
//  CO2OPEN     = 0x10,
//	CO2CLOSE    = 0x12,
//	PMCLOSE     = 0x15,
uint8_t cgk_sendcmd(uint8_t _cmd) 
{
	uint8_t ucCmdret = 0;
	if(g_CgkSensorData.uiNowCmdStatus == _cmd) {
		return 1;   // the status is you want cmd
	}
	ucCmdret = Cgk_sendcmd(_cmd);
	if(ucCmdret == 0) {
		g_CgkSensorData.uiNowCmdStatus = _cmd;
	}
	return ucCmdret;
}

void CgkPMDataCollection(void)
{
	cgk_readpmdata(&g_CgkSensorData);
	g_CgkSensorData.uiPm25Ug =(unsigned int)( (float)g_CgkSensorData.uiPm03Dat * 0.00015);
	g_CgkSensorData.uiPm10Ug =(unsigned int)( g_CgkSensorData.uiPm25Ug + (float)g_CgkSensorData.uiPm25Dat * 0.08);
}

void CgkCO2DataCollection(void)
{
	cgk_readsevendata(&g_CgkSensorData.uiCO2Dat);
}

void CgkUltDataCollection(void)
{
	cgk_readsevendata(&g_CgkSensorData.uiUltlDat);
}

void CgkElecDataCollection(void)
{
	cgk_readsevendata(&g_CgkSensorData.uiElecDat);
}

extern uint8_t CGK_cmdswitch;
extern PAGEONE_CONTROL_STU g_pageone_control_stu;
void Cgk_test_example(void )
{
	static uint8_t meaCount = 0;
	uint8_t sensor_ret = 0;

	switch(/*CGK_cmdswitch*/NULL) {
		case ELECMEASURE:
			cgk_sendcmd(ELECMEASURE);	
			rt_thread_delay(150);	
			break;
		case ULTLMEASURE:
			cgk_sendcmd(ULTLMEASURE);	
			rt_thread_delay(150);	
			
			break;
		case PMMEASURE:
			cgk_sendcmd(PMMEASURE);	
			rt_thread_delay(150);	
			
			break;
		case CO2MEASURE: 
			cgk_sendcmd(CO2OPEN);	
			rt_thread_delay(150);	
			cgk_sendcmd(CO2MEASURE);	
			rt_thread_delay(150);	
			
			break;
		case CO2CLOSE: 
			cgk_sendcmd(CO2CLOSE);	
			rt_thread_delay(150);	
			
			break;
		case PMCLOSE: 
			cgk_sendcmd(PMCLOSE);	
			rt_thread_delay(150);	

			break;
		default:			
			break;

	}

	switch(g_CgkSensorData.uiNowCmdStatus) {
		case ELECMEASURE:
			CgkElecDataCollection(); 
			break;
		case ULTLMEASURE:
			CgkUltDataCollection(); 
			break;
		case PMMEASURE:
			CgkPMDataCollection(); 
			break;
		case CO2MEASURE: 
			CgkCO2DataCollection(); 
			break;
		case CO2CLOSE: 
			break;
		case PMCLOSE: 
			break;
		default:			
			break;
	}
	//rt_thread_delay(1000);	

	
//	  CgkCO2PowerClose();
//	  CgkPMPowerClose();
	//rt_kprintf("pm03 %d, pm25 %d,pm10 %d,co2 %d,elec %d ,ultl %d ,pm25ug = %d , pm10ug = %d \n",g_CgkSensorData.uiPm03Dat,	
	//	g_CgkSensorData.uiPm25Dat,g_CgkSensorData.uiPm10Dat,	g_CgkSensorData.uiCO2Dat,	g_CgkSensorData.uiElecDat,
	//	g_CgkSensorData.uiUltlDat, g_CgkSensorData.uiPm25Ug,g_CgkSensorData.uiPm10Ug);	

}



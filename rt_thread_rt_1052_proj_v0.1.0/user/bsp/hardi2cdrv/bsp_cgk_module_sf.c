
#include "bsp_cgk_module_sf.h"
#include "bsp_i2cdev_init.h"
#include "public_h.h"

CgkSensorData g_CgkSensorData;


// return 0 : open touch device is ok
// return 1 : cgk i2c device is not inits
// return 2 : cgk i2c device addr is not correct
uint8_t cgk_inithard(void)
{	
	bsp_cgkInitI2C();
		
	g_CgkSensorData.ucDeviceAddr = CGKI2CADDR;
    g_CgkSensorData.ucEnable = 1;  		
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
	i2c_cgkStart();
	i2c_cgkSendByte(g_CgkSensorData.ucDeviceAddr);

	if( i2c_cgkWaitAck() ) {
		return 2;
	}

	i2c_cgkSendByte(_ucCmd);
	if( i2c_cgkWaitAck() ) {
		return 2;
	}
		
	i2c_cgkStop();
	return 0;
}
static void CGK_Delay(void)
{
	uint16_t i;

	for (i = 0; i < 200; i++);//此事件非常重要，关系到能否读到正确的数据
}

static uint8_t cgk_readstrdata(uint8_t _ucSlaAdd,uint8_t *_usRcvDat, uint8_t _ucRcvNum)
{
	uint8_t i;
	i2c_cgkStart();
	i2c_cgkSendByte(_ucSlaAdd+1);

	if( i2c_cgkWaitAck() ) {
		return 2;
	}
	
	for (i = 0;i < _ucRcvNum; i++)
	{
		_usRcvDat[i] = i2c_cgkReadByte();	
		i2c_cgkAck();		
	}
	i2c_cgkNAck();
	i2c_cgkStop();
	return 0;

}


static uint8_t cgk_readsevendata(uint32_t *_uiReadDat)
{
	uint8_t ucPmret = 0,iCount = 0, checkSum = 0;
	uint8_t tempDat[7] = {0};

	if( 1 != g_CgkSensorData.ucEnable ) {
		return 2;
	}	
	
	ucPmret = cgk_readstrdata(g_CgkSensorData.ucDeviceAddr,tempDat,7);

	if(ucPmret == 0x00) {
	  // check the sum
	  for(iCount =0 ; iCount < 7; iCount++ ){
		  checkSum += tempDat[iCount];			  
		  //putchar(tempDat[iCount]);
	  } 	  
	  if(checkSum != 0x00){
		  return 1;
	  }
	}else if( ucPmret == 0x02 ) {
	  return ucPmret;
	}

	// checksum is ok
	*_uiReadDat = (uint32_t)( (tempDat[0]<<8) | tempDat[1] ) ;

	return 0;
}

static uint8_t cgk_readpmdata(CgkSensorData *g_CgkSensorDataP)
{
	uint8_t ucPmret = 0,iCount = 0, checkSum = 0;
	uint8_t tempDat[17] = {0};

	if( 1 != g_CgkSensorData.ucEnable ) {
		return 2;
	}	
	rt_kprintf("debug\n");
	ucPmret = cgk_readstrdata(g_CgkSensorData.ucDeviceAddr,tempDat,17);

	return 3;
	if(ucPmret == 0x00) {
	  // check the sum
	  for(iCount =0 ; iCount < 17; iCount++ ){
		  checkSum += tempDat[iCount];			  
		  //putchar(tempDat[iCount]);
	  } 	  
	  if(checkSum != 0x00){
		  return 1;
	  }
	}else if( ucPmret == 0x02 ) {
	  return ucPmret;
	}
	rt_kprintf("debug2\n");
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

extern PAGEONE_CONTROL_STU g_pageone_control_stu;
void Cgk_test_example(void )
{
	static uint8_t meaCount = 0;
	uint8_t sensor_ret = 0;

		
	cgk_sendcmd(PMMEASURE);
	//rt_thread_delay(200); 	
	
	/*
	if (g_pageone_control_stu.ButtonCo2timeDelay > 32) {
		// open co2
		sensor_ret = cgk_sendcmd(CO2OPEN);
		rt_thread_delay(200);   
		sensor_ret = cgk_sendcmd(CO2MEASURE);
		rt_thread_delay(200); 
	}else if(g_pageone_control_stu.ButtonCo2timeDelay < 2) {
		// close co2
		sensor_ret = cgk_sendcmd(CO2CLOSE);
	}
	
	if (g_pageone_control_stu.ButtonPmtimeDelay > 32) {
		// open co2
		sensor_ret = cgk_sendcmd(PMMEASURE);
		rt_thread_delay(200); 
	}else if(g_pageone_control_stu.ButtonPmtimeDelay < 2) {
		// close co2
		sensor_ret = cgk_sendcmd(PMCLOSE);
	}	
	*/
	

	switch(g_CgkSensorData.uiNowCmdStatus) {
		case ELECMEASURE:
			CgkElecDataCollection(); 
			break;
		case ULTLMEASURE:
			CgkUltDataCollection(); 
			break;
		case PMMEASURE:
			rt_kprintf("debug0\n");
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
	rt_kprintf("pm03 %d, pm25 %d,pm10 %d,co2 %d,elec %d ,ultl %d ,pm25ug = %d , pm10ug = %d \n",g_CgkSensorData.uiPm03Dat,	
		g_CgkSensorData.uiPm25Dat,g_CgkSensorData.uiPm10Dat,	g_CgkSensorData.uiCO2Dat,	g_CgkSensorData.uiElecDat,
		g_CgkSensorData.uiUltlDat, g_CgkSensorData.uiPm25Ug,g_CgkSensorData.uiPm10Ug);	

}


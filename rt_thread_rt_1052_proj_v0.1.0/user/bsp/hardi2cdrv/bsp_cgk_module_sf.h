
#ifndef __BSP_CGK_MODULE_SF_H
#define	__BSP_CGK_MODULE_SF_H

#include <rtdevice.h> 
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"


typedef struct _CgkSensorData{
	unsigned int  uiElecDat;	
	unsigned int  uiUltlDat;
    unsigned int  uiPm03Dat;	
	unsigned int  uiPm25Dat;			
	unsigned int  uiPm10Dat;			
	unsigned int  uiCO2Dat;	
	
	unsigned int  uiPm25Ug;	
	unsigned int  uiPm10Ug;			
	
	unsigned int  uiCO2CalDat;	
	unsigned int  uiNowCmdStatus;	
	unsigned char ucEnable;	
    unsigned char ucDeviceAddr;	
}CgkSensorData;

typedef enum{
	ELECMEASURE = 0x01,
	ULTLMEASURE = 0x02,
	PMMEASURE   = 0x05,
	CO2MEASURE  = 0x06,
  CO2OPEN     = 0x10,
	CO2CLOSE    = 0x12,
	PMCLOSE     = 0x15,
}CGK_CMD_TYPE;


#define CGKI2CADDR 0x22

extern CgkSensorData g_CgkSensorData;


/**
  * @brief  cgk_sendcmd , Measuring dust command function ,Send different instructions 
  *         
  * @param  _cmd is you want to send cmd
  * @retval if return 0 is ok , if return 2 is error the device is no ack 
  */

uint8_t cgk_inithard(void );


uint8_t cgk_sendcmd(uint8_t _cmd) ;


void CgkPMDataCollection(void);

void CgkUltDataCollection(void);

void CgkCO2DataCollection(void);

void CgkElecDataCollection(void);

void Cgk_test_example(void );

void cgk_userapp(void);



#endif



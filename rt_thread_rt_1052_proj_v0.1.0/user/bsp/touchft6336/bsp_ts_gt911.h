/*
*********************************************************************************************************
*
*	Ä£¿éÃû³Æ : ft5x06µçÈÝ´¥ÃþÐ¾Æ¬Çý¶¯³ÌÐò
*	ÎÄ¼þÃû³Æ : bsp_ts_ft5x06.h
*	Ëµ    Ã÷ : Í·ÎÄ¼þ
*	°æ    ±¾ : V1.0
*
*	Copyright (C), 2015-2020, °²¸»À³µç×Ó www.armfly.com
*********************************************************************************************************
*/

#ifndef _BSP_TS_GT911__H
#define _BSP_TS_GT911__H

#include <rtdevice.h> 
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"

#define GT911_INT_PIN  112


/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define GT911_I2C_ADDR             (0xBA>>1)

#define GT911_TOUCH_POINTS         5		   

#define GT911_POINT_READ_BUF       6  // 0x804E - 0x8054

#define GTP_REG_CONFIG_DATA        0x8050

#define GT911_READ_XY_REG          0x814E /* ����Ĵ��� */ 

#define GT911_CLEARBUF_REG         0x814E /* �������Ĵ��� */ 

#define GT911_CONFIG_REG           0x8047 /* ���ò����Ĵ��� */ 

#define GT911_COMMAND_REG          0x8040 /* ʵʱ���� */ 

#define GT911_PRODUCT_ID_REG       0x8140 /* оƬID */ 
 
#define GT911_VENDOR_ID_REG        0x814A /* ��ǰģ��ѡ����Ϣ */ 

#define GT911_CONFIG_VERSION_REG   0x8047 /* �����ļ��汾�� */ 

#define GT911_CONFIG_CHECKSUM_REG  0x80FF /* �����ļ�У���� */ 

#define GT911_FIRMWARE_VERSION_REG 0x8144 /* �̼��汾�� */ 


typedef struct
{
    uint8_t  DeviceAddr;
	uint8_t  ChipID[5];
	uint8_t  Enable;
	uint8_t  TimerCount;
	
	uint8_t  Count;			/* ¼¸¸öµã°´ÏÂ */
	
	uint16_t X[GT911_TOUCH_POINTS];
	uint16_t Y[GT911_TOUCH_POINTS];	
	uint8_t  id[GT911_TOUCH_POINTS];
	uint8_t  Event[GT911_TOUCH_POINTS];
}GT911_T;




uint8_t     gt911_inithard(char *devicename);

uint8_t *   gt911_readid(void);

uint8_t     gt911_penint(void);

uint16_t    gt911_readversion(void);
void        gt911_scan(void);
void        gt911_timer1ms(void);
uint8_t     gt911_onepiont_scan(void);

uint8_t     gt911_sendcfg(uint8_t mode);

extern GT911_T g_tGT911;

#endif


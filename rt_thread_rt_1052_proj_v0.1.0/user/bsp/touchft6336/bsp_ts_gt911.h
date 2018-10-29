/*
*********************************************************************************************************
*
*	脛拢驴茅脙没鲁脝 : ft5x06碌莽脠脻麓楼脙镁脨戮脝卢脟媒露炉鲁脤脨貌
*	脦脛录镁脙没鲁脝 : bsp_ts_ft5x06.h
*	脣碌    脙梅 : 脥路脦脛录镁
*	掳忙    卤戮 : V1.0
*
*	Copyright (C), 2015-2020, 掳虏赂禄脌鲁碌莽脳脫 www.armfly.com
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

#define GT911_READ_XY_REG          0x814E /* 坐标寄存器 */ 

#define GT911_CLEARBUF_REG         0x814E /* 清除坐标寄存器 */ 

#define GT911_CONFIG_REG           0x8047 /* 配置参数寄存器 */ 

#define GT911_COMMAND_REG          0x8040 /* 实时命令 */ 

#define GT911_PRODUCT_ID_REG       0x8140 /* 芯片ID */ 
 
#define GT911_VENDOR_ID_REG        0x814A /* 当前模组选项信息 */ 

#define GT911_CONFIG_VERSION_REG   0x8047 /* 配置文件版本号 */ 

#define GT911_CONFIG_CHECKSUM_REG  0x80FF /* 配置文件校验码 */ 

#define GT911_FIRMWARE_VERSION_REG 0x8144 /* 固件版本号 */ 


typedef struct
{
    uint8_t  DeviceAddr;
	uint8_t  ChipID[5];
	uint8_t  Enable;
	uint8_t  TimerCount;
	
	uint8_t  Count;			/* 录赂赂枚碌茫掳麓脧脗 */
	
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


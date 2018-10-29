/*
*********************************************************************************************************
*
*	NAME      :    ft6336 touch driver
*	FILENMAME :    bsp_ts_ft6336.h
*   AUTHOR    :    Bearpeak
*	VERSION   :    V1.0
*
*********************************************************************************************************
*/



#ifndef _BSP_TS_FT6336_H
#define _BSP_TS_FT6336_H

#include <rtdevice.h> 
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"



#define LCD_XSIZE             720
#define LCD_YSIZE             1280

/* I2C device Address */
#define FT6336_I2C_ADDR       (0x38)

#define FT6336_TOUCH_POINTS   1		/* 支持的触摸点数 */

#define FT6336_INT_PIN        106  //GPIO_B1_15


/* 寄存器地址 */
#define FT6336_REG_FW_VER     0xA6		/* 固件版本 */
#define FT6336_REG_POINT_RATE 0x88		/* 速率 */
#define FT6336_REG_THGROUP    0x80		/* 门槛 */

/*Chip Device Type*/
#define IC_FT6336             0	/* x=2,3,4 */
#define IC_FT5606             1	/* ft5506/FT5606/FT5816 */
#define IC_FT5316             2	/* ft5x16 */
#define IC_FT6208             3	/* ft6208 */
#define IC_FT6x06             4	/* ft6206/FT6306 */
#define IC_FT6336i            5	/* ft5306i */
#define IC_FT5x36             6	/* ft5336/ft5436/FT5436i */



/*register address*/
#define TS_DEVICE_MODE        0x00
#define GEST_ID               0x01
#define TD_STATUS             0x02
#define TOUCH1_XH             0x03
#define TOUCH1_XL             0x04
#define TOUCH1_YH             0x05
#define TOUCH1_YL             0x06
#define TOUCH2_XH             0x09
#define TOUCH2_XL             0x0A
#define TOUCH2_YH             0x0B
#define TOUCH2_YL             0x0C
#define TOUCH3_XH             0x0F
#define TOUCH3_XL             0x10
#define TOUCH3_YH             0x11
#define TOUCH3_YL             0x12

#define FTS_REG_CHIP_ID       0xA3	/* chip ID */
#define FTS_REG_INT_MODE      0xA4  /* FT6236中断模式控制寄存器 */
#define FTS_REG_FW_VER        0xA6	/* FW  version */
#define FTS_REG_VENDOR_ID     0xA8	/* TP vendor ID */
#define FTS_REG_POINT_RATE    0x88	/* report rate */

#define CFG_POINT_READ_BUF  (3 + 6 * (FT6336_TOUCH_POINTS))    // 9 字节

typedef struct
{
  uint8_t  ucDeviceAddr;
	uint8_t  ucChipID;
	uint8_t  ucEnable;
	uint8_t  ucTimerCount;
	
	uint8_t  ucCount;			/* 几个点按下 */
	
	uint16_t sX[FT6336_TOUCH_POINTS];
	uint16_t sY[FT6336_TOUCH_POINTS];	
	uint8_t  ucid[FT6336_TOUCH_POINTS];
	uint8_t  ucEvent[FT6336_TOUCH_POINTS];
}FT6336_T;

uint8_t  ft6336_inithard(void );
uint8_t  ft6336_readid(void);
uint8_t  ft6336_penint(void);
void     ft6336_scan(void);
void     ft6336_timer1ms(void);
uint8_t  ft6336_osnepiont_scan(void);


extern FT6336_T g_tFT6336;

#endif



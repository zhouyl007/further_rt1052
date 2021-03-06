#ifndef _BSP_TS_FT5X06_H
#define _BSP_TS_FT5X06_H


#include "bsp_i2c_gpio.h"
#include <rtdevice.h> 
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"


/* I2C总线，器件ID */
#define FT5X06_I2C_ADDR       0x70

#define FT5X06_TOUCH_POINTS   1		    /* 支持的触摸点数 */

/* 寄存器地址 */
#define FT5X06_REG_FW_VER     0xA6		/* 固件版本 */
#define FT5X06_REG_POINT_RATE 0x88		/* 速率 */
#define FT5X06_REG_THGROUP    0x80		/* 门槛 */

/*Chip Device Type*/
#define IC_FT5X06       0	/* x=2,3,4 */
#define IC_FT5606       1	/* ft5506/FT5606/FT5816 */
#define IC_FT5316       2	/* ft5x16 */
#define IC_FT6208       3	/* ft6208 */
#define IC_FT6x06       4	/* ft6206/FT6306 */
#define IC_FT5x06i      5	/* ft5306i */
#define IC_FT5x36       6	/* ft5336/ft5436/FT5436i */

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

#define CFG_POINT_READ_BUF    9


typedef struct
{
	uint8_t ChipID;
	uint8_t Enable;
	uint8_t TimerCount;
	
	uint8_t Count;			/* 几个点按下 */
	
	uint16_t X[FT5X06_TOUCH_POINTS];
	uint16_t Y[FT5X06_TOUCH_POINTS];	
	uint8_t id[FT5X06_TOUCH_POINTS];
	uint8_t Event[FT5X06_TOUCH_POINTS];
}FT5X06_T;

void FT5X06_InitHard(void);
uint8_t FT5X06_ReadID(void);
uint8_t FT5X06_PenInt(void);
uint16_t FT5X06_ReadVersion(void);
void FT5X06_Scan(void);
void FT5X06_Timer1ms(void);
void FT5X06_OnePiontScan(void);


extern FT5X06_T g_tFT5X06;

#endif



/*
*********************************************************************************************************
*
*	模块名称 : I2C总线驱动模块
*	文件名称 : bsp_i2c_gpio.h
*	版    本 : V1.0
*	说    明 : 头文件。
*
*
*********************************************************************************************************
*/

#ifndef _BSP_CGKI2C_GPIO_H
#define _BSP_CGKI2C_GPIO_H

#include "fsl_gpio.h"
#include "fsl_iomuxc.h"



#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */

void bsp_cgkInitI2C(void);
void i2c_cgkStart(void);
void i2c_cgkStop(void);
void i2c_cgkSendByte(uint8_t _ucByte);
uint8_t i2c_cgkReadByte(void);
uint8_t i2c_cgkWaitAck(void);
void i2c_cgkAck(void);
void i2c_cgkNAck(void);
uint8_t i2c_cgkCheckDevice(uint8_t _Address);

#endif




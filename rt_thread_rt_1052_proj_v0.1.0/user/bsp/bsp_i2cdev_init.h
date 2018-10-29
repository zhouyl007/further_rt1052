
#ifndef __BSP_I2CDEV_INIT_H
#define	__BSP_I2CDEV_INIT_H

#include <rtdevice.h> 
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"

extern rt_device_t i2c1dev ;

uint8_t i2c1dev_hard_init(char *devicename);

#endif 



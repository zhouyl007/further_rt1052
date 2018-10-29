
#include "bsp_i2cdev_init.h"


rt_device_t i2c1dev = NULL;


// return 0 : open touch device is ok
// return 1 : find i2c1 device is error
// return 2 : open i2c1 device is error 
uint8_t i2c1dev_hard_init(char *devicename)
{	
	struct rt_i2c_msg msgs[1];
		
	i2c1dev = rt_device_find(devicename);
	if (!i2c1dev) {
		rt_kprintf("search %s device failed !!!\n", devicename);
		return 1;
	}

	if (rt_device_open(i2c1dev, RT_DEVICE_OFLAG_RDWR) != RT_EOK) {
		rt_kprintf("open %s device failed !!!n", devicename);
		return 2;
	}

	rt_kprintf("open %s device bus succeed !!!\n", devicename);
  return 0;
}




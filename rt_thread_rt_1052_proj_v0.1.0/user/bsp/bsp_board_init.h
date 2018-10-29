#ifndef __BSP_BOARD_INIT_H
#define __BSP_BOARD_INIT_H

#include "bsp_i2cdev_init.h"
#include "bsp_cgk_module.h"
#include "bsp_ts_ft6336_sf.h"
#include "bsp_adc_bat.h"
#include "bsp_basetimer.h"
#include "bsp_spiflash.h"
#include "bsp_spiflash2.h"
#include "bsp_uartwifi.h"


                                                  
uint8_t rt1052_board_user_init(void);

#define POWER_STATUS     73  
#define POWER_COTL       101 


#endif																


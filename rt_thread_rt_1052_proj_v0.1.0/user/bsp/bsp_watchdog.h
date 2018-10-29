
#ifndef __BSP_WATCH_DOG_H
#define	__BSP_WATCH_DOG_H


#include "fsl_adc.h"
#include <rtdevice.h> 
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"

void bsp_rtwdog_init(uint8_t src,uint8_t pscen,uint16_t toval,uint16_t win);
void bsp_rtwog_feed(void);



#endif /* __WATCH_DOG_H */


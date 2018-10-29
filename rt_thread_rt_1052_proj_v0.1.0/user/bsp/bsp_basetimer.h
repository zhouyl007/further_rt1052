
#ifndef __BSP_BASETIMER_H
#define	__BSP_BASETIMER_H


#include "fsl_adc.h"
#include <rtdevice.h> 
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"

#include "fsl_qtmr.h"



void bsp_qtimer1_ch0int_init(uint8_t prisrc,uint16_t cmp1);
void bsp_qtimer2_ch0int_init(uint8_t prisrc,uint16_t cmp1);



#endif  


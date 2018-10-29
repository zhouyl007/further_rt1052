
#ifndef __BSP_ADC_BAT_H
#define	__BSP_ADC_BAT_H


#include "fsl_adc.h"
#include <rtdevice.h> 
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"




typedef struct _AdcVatData{
  uint8_t  ucEnable;
	uint16_t uiOriginalValue;
	float    fVoltageValue;
}AdcVatData;


extern AdcVatData g_AdcVatData;

#define BAT_USE_ADC         ADC1
#define BAT_NXP_ADC_CH  		7 		 		// BAT adc channel is 7


void     bsp_batadc1_init(void);
uint16_t bsp_batadc1_getvolvalue(uint8_t ch);
uint8_t  bsp_batadc1_getvolvalue_average(uint8_t ch,uint8_t times);





#endif /* __ADC_BAT_H */



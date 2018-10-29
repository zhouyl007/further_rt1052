
#ifndef __BSP_POWERPWM_H
#define	__BSP_POWERPWM_H


#include "fsl_adc.h"
#include <rtdevice.h> 
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"

#include "fsl_qtmr.h"
#include "fsl_iomuxc.h"



//ͨ���ı�TMR4->CHANNEL[3].CMPLD1��ֵ���ı�͵�ƽ��ռ�ձȣ��Ӷ�����LED1������
#define LCDBK_PWM_VAL TMR4->CHANNEL[3].CMPLD1

void bsp_qtmr4ch3_pwminit(uint8_t prisrc,uint32_t clk, uint8_t duty);
void bsp_qtmr4ch3_pwmdutyset(uint8_t prisrc,uint32_t clk, uint8_t duty);


#endif  


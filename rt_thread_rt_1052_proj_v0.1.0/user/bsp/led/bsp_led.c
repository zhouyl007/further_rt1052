/**
  ******************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   led应用函数接口
  */
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"  
	
#include "pad_config.h"  
#include "bsp_led.h"   

 /**
  * @brief  the board led 
  * @param  void
  * @retval void 
  * this function will init led gpio and light led
  */
void LED_GPIO_Config(void)
{	
	gpio_pin_config_t config =	{kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

	config.outputLogic = 1;
	GPIO_PinInit(CORE_BOARD_LED_GPIO, CORE_BOARD_LED_GPIO_PIN, &config);
	GPIO_PinWrite(CORE_BOARD_LED_GPIO, CORE_BOARD_LED_GPIO_PIN, 0);   // light led because of led low is on

}


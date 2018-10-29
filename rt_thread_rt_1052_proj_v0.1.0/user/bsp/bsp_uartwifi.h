#ifndef _BSP_UARTWIFI_H
#define _BSP_UARTWIFI_H


#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "fsl_lpuart.h"


#define BOARD_DEBUG_UART_TYPE 		  DEBUG_CONSOLE_DEVICE_TYPE_LPUART
#define BOARD_DEBUG_UART_BASEADDR 	(uint32_t) LPUART4

#define LPUART4_REC_LEN          200  	    //定义最大接收字节数 200
#define EN_LPUART4_RX 			     1		      //使能（1）/禁止（0）串口1接收
	  	
#if EN_LPUART4_RX
	extern uint8_t  LPUART4_RX_BUF[LPUART4_REC_LEN]; //接收缓冲,最大LPUART_REC_LEN个字节.末字节为换行符 
	extern uint16_t LPUART4_RX_STA;         		     //接收状态标记	
#endif 

void bsp_uart4wifi_init(uint32_t bound);

void bsp_uart4_sendbyte(uint8_t _cData);

void bsp_uart4_sendstr(const uint8_t *strP,uint8_t _num);

#endif

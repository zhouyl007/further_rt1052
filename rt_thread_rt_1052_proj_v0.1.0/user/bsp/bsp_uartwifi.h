#ifndef _BSP_UARTWIFI_H
#define _BSP_UARTWIFI_H


#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "fsl_lpuart.h"


#define BOARD_DEBUG_UART_TYPE 		  DEBUG_CONSOLE_DEVICE_TYPE_LPUART
#define BOARD_DEBUG_UART_BASEADDR 	(uint32_t) LPUART4

#define LPUART4_REC_LEN          200  	    //�����������ֽ��� 200
#define EN_LPUART4_RX 			     1		      //ʹ�ܣ�1��/��ֹ��0������1����
	  	
#if EN_LPUART4_RX
	extern uint8_t  LPUART4_RX_BUF[LPUART4_REC_LEN]; //���ջ���,���LPUART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
	extern uint16_t LPUART4_RX_STA;         		     //����״̬���	
#endif 

void bsp_uart4wifi_init(uint32_t bound);

void bsp_uart4_sendbyte(uint8_t _cData);

void bsp_uart4_sendstr(const uint8_t *strP,uint8_t _num);

#endif

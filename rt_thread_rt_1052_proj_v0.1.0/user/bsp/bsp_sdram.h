#ifndef _BSP_SDRAM_H
#define _BSP_SDRAM_H

#include "fsl_common.h"
#include "fsl_semc.h"
#include "fsl_gpio.h"
#include "pin_mux.h"
#include "fsl_iomuxc.h"

typedef uint8_t  u8;
typedef uint32_t u32;
typedef volatile uint8_t vu8;

#define SDRAM_START_ADDR    ((u32)(0X80000000)) //SDRAM开始地址
#define SDRAM_SIZE_KBYTES   ((u32)(16*1024))    //SDRAM大小，单位kBytes

void SDRAM_Init(void);
void SEMC_SDRAM_WriteBuffer(u8 *pBuffer,u32 WriteAddr,u32 n);
void SEMC_SDRAM_ReadBuffer(u8 *pBuffer,u32 ReadAddr,u32 n);
#endif

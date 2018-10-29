
#ifndef __BSP_SPIFLASH2_H
#define	__BSP_SPIFLASH2_H


#include <rtdevice.h> 
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"


#include "fsl_iomuxc.h"
#include "fsl_lpspi.h"


#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
#define W25Q256	0XEF18


extern uint16_t W25Q64_TYPE;				

// SPI CS GPIO3_IO13

#define	W25Q64_CS(n)    (n?GPIO_PinWrite(GPIO3,13,1):GPIO_PinWrite(GPIO3,13,0)) 	


#define W64_WriteEnable		  	0x06 
#define W64_WriteDisable		  	0x04 
#define W64_ReadStatusReg1			0x05 
#define W64_ReadStatusReg2			0x35 
#define W64_ReadStatusReg3		  0x15 
#define W64_WriteStatusReg1    0x01 
#define W64_WriteStatusReg2    0x31 
#define W64_WriteStatusReg3    0x11 
#define W64_ReadData						0x03 
#define W64_FastReadData				0x0B 
#define W64_FastReadDual				0x3B 
#define W64_PageProgram				0x02 
#define W64_BlockErase					0xD8 
#define W64_SectorErase				0x20 
#define W64_ChipErase					0xC7 
#define W64_PowerDown					0xB9 
#define W64_ReleasePowerDown		0xAB 
#define W64_DeviceID						0xAB 
#define W64_ManufactDeviceID		0x90 
#define W64_JedecDeviceID			0x9F 
#define W64_Enable4ByteAddr    0xB7
#define W64_Exit4ByteAddr      0xE9


uint16_t W25Q64_ReadID(void);  	    	 
uint8_t W25Q64_ReadSR(uint8_t regno);            
void W25Q64_4ByteAddr_Enable(void);     
void W25Q64_Write_SR(uint8_t regno,uint8_t sr);   
void W25Q64_Write_Enable(void);  		  
void W25Q64_Write_Disable(void);		 
void W25Q64_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void W25Q64_Erase_Chip(void);    	  	 
void W25Q64_Erase_Sector(uint32_t Dst_Addr); 
void W25Q64_Wait_Busy(void);           	 
void W25Q64_PowerDown(void);         
void W25Q64_WAKEUP(void);				 


void bsp_w25q64_read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);  
void bsp_w25q64_write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite); 
void bsp_w25q64_init(void);
uint8_t bsp_test_spiflashwq256(void);


#endif



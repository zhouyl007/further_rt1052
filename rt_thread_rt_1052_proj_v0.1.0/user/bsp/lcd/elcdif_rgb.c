/*
 * Copyright (c) 2017, NXP Semiconductors, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_common.h"
#include "fsl_elcdif.h"
#include "fsl_debug_console.h"

#include "pin_mux.h"
#include "board.h"
#include "fsl_gpio.h"
#include "clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_ELCDIF LCDIF

#define APP_IMG_HEIGHT  1280
#define APP_IMG_WIDTH   720


#define APP_POL_FLAGS \
(kELCDIF_DataEnableActiveHigh | kELCDIF_VsyncActiveLow | kELCDIF_HsyncActiveLow | kELCDIF_DriveDataOnRisingClkEdge)
//(kELCDIF_DataEnableActiveLow | kELCDIF_VsyncActiveHigh | kELCDIF_HsyncActiveHigh | kELCDIF_DriveDataOnRisingClkEdge)
//(kELCDIF_DataEnableActiveLow | kELCDIF_VsyncActiveHigh | kELCDIF_HsyncActiveHigh | kELCDIF_DriveDataOnRisingClkEdge)
//kELCDIF_DataEnableActiveHigh   kELCDIF_DataEnableActiveLow
//kELCDIF_VsyncActiveLow     kELCDIF_VsyncActiveHigh
//kELCDIF_HsyncActiveLow    kELCDIF_HsyncActiveHigh
//kELCDIF_DriveDataOnRisingClkEdge   kELCDIF_DriveDataOnFallingClkEdge   


/* Display reset*/
#define LCD_DISP_GPIO         GPIO3
#define LCD_DISP_GPIO_PIN     16
/* Back light. */
#define LCD_BL_GPIO           GPIO2
#define LCD_BL_GPIO_PIN       29

/* Frame buffer data alignment, for better performance, the LCDIF frame buffer should be 64B align. */
#define FRAME_BUFFER_ALIGN    64

#ifndef APP_LCDIF_DATA_BUS
#define APP_LCDIF_DATA_BUS    kELCDIF_DataBus16Bit
#endif


/**********************TTL 输出设置参??***********/
#define	LCD_XSIZE		720
#define	LCD_YSIZE		1280

#define	LCD_BPP			16           // 16  24位选择

#define  PCLK       	60           // 60MHZ

#define HBPD        	60  //60
#define HFPD			    20  //20/*72*/
#define HSPW			    20  //20

#define VBPD			    27  //27
#define VFPD			    20  //20
#define VSPW			    4   //4


//#define HBPD         36
//#define HFPD         26
//#define HSPW         2
//#define VBPD         16
//#define VFPD         16
//#define VSPW         4


///////////////////////////////
void spi_delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 1000; ++i)
    {
        //__asm("NOP"); /* delay */
    }
}

void delay_ms(uint32_t ms)
{
    volatile uint32_t i = 0;
    for (i = 0; i < ms; ++i)
    {
        spi_delay(); /* delay */
    }
}

#define SIM_SPI_DELAY(us)        delay_ms(us)



// CS     129
// SCK    119
// MOSI   120
// RESET  316
// MISO   121

#define SSD2828_RST_SET_()       {GPIO_PinWrite(GPIO3,  16, 1U); SIM_SPI_DELAY(1);}
#define SSD2828_RST_CLR_()       {GPIO_PinWrite(GPIO3,  16, 0U); SIM_SPI_DELAY(1);}
#define SSD2828_RST_SET()        {GPIO_PinWrite(GPIO3,  16, 1U); SIM_SPI_DELAY(1);}
#define SSD2828_RST_CLR()        {GPIO_PinWrite(GPIO3,  16, 0U); SIM_SPI_DELAY(1);}

#define SSD2828_CS_SET()         {GPIO_PinWrite(GPIO1,  29, 1U); SIM_SPI_DELAY(1);}
#define SSD2828_CS_CLR()         {GPIO_PinWrite(GPIO1,  29, 0U); SIM_SPI_DELAY(1);}

#define SSD2828_SCK_SET()        {GPIO_PinWrite(GPIO1,  19, 1U); SIM_SPI_DELAY(1);}
#define SSD2828_SCK_CLR()        {GPIO_PinWrite(GPIO1,  19, 0U); SIM_SPI_DELAY(1);}

#define SSD2828_MOSI_SET()       {GPIO_PinWrite(GPIO1,  20, 1U); SIM_SPI_DELAY(1);}
#define SSD2828_MOSI_CLR()       {GPIO_PinWrite(GPIO1,  20, 0U); SIM_SPI_DELAY(1);}


typedef enum ssd2828_send_flg {

    SSD2828_SEND_CMD  = 0,
    SSD2828_SEND_DATA = 1,
    
} ssd2828_send_flg_t;

void ssd2828_write_byte (uint8_t bData, ssd2828_send_flg_t data_flg)
{
    // RS=0
    int i;

    SSD2828_CS_CLR();

    if (data_flg == SSD2828_SEND_CMD) {
        SSD2828_MOSI_CLR();  //BIT8 = 0 CMD
    } else {
        SSD2828_MOSI_SET();  //BIT8 = 1 DATA 
    }

    SSD2828_SCK_CLR();
    SSD2828_SCK_SET();

    for(i = 0; i < 8; i++) {

        if (bData & 0x80) {      //MSB
            SSD2828_MOSI_SET();
        } else {
            SSD2828_MOSI_CLR();
        }

        SSD2828_SCK_CLR();
        SSD2828_SCK_SET();

        bData <<= 1;
    }

    SSD2828_CS_SET();
}

void PacketSize_b(unsigned char *pbuf)
{
    unsigned int i;
    unsigned char  num;
    num = *pbuf;

    ssd2828_write_byte(0xB7, SSD2828_SEND_CMD);
    ssd2828_write_byte(0x50, SSD2828_SEND_DATA);
    ssd2828_write_byte(0x02, SSD2828_SEND_DATA);

    ssd2828_write_byte(0xBD, SSD2828_SEND_CMD);
    ssd2828_write_byte(0x00, SSD2828_SEND_DATA);
    ssd2828_write_byte(0x00, SSD2828_SEND_DATA);

    ssd2828_write_byte(0xBC, SSD2828_SEND_CMD);
    ssd2828_write_byte(num , SSD2828_SEND_DATA);
    ssd2828_write_byte(0x00, SSD2828_SEND_DATA);
    //delay1(1);

    ssd2828_write_byte(0xbf, SSD2828_SEND_CMD);

    for(i = 0;i < num; i++){
        ssd2828_write_byte(*(pbuf + i + 1), SSD2828_SEND_DATA);
    }
}


uint8_t dsi_read_lcd_id(void)
{
    uint8_t id = 0;
    
    unsigned char tab1[]={7,/*0xDA*/0x04,0X80,0x81,0x82,0x83,0x84,0x85};
    PacketSize_b (tab1);
    
    
    return id;
}

void rm68200_2b(unsigned char cmd,unsigned char dat)
{
    ssd2828_write_byte(0xB7, SSD2828_SEND_CMD);
    ssd2828_write_byte(0x50, SSD2828_SEND_DATA);
    ssd2828_write_byte(0x02, SSD2828_SEND_DATA);

    ssd2828_write_byte(0xBD, SSD2828_SEND_CMD);
    ssd2828_write_byte(0x00, SSD2828_SEND_DATA);
    ssd2828_write_byte(0x00, SSD2828_SEND_DATA);

    ssd2828_write_byte(0xBC, SSD2828_SEND_CMD);
    ssd2828_write_byte( 2,   SSD2828_SEND_DATA);
    ssd2828_write_byte(0x00, SSD2828_SEND_DATA);
    //delay1(1);

    ssd2828_write_byte(0xbf, SSD2828_SEND_CMD);

	  ssd2828_write_byte(cmd, SSD2828_SEND_DATA);
	  ssd2828_write_byte(dat, SSD2828_SEND_DATA);
}

void rm68200_1b(unsigned char cmd)
{
    ssd2828_write_byte(0xB7, SSD2828_SEND_CMD);
    ssd2828_write_byte(0x50, SSD2828_SEND_DATA);
    ssd2828_write_byte(0x02, SSD2828_SEND_DATA);

    ssd2828_write_byte(0xBD, SSD2828_SEND_CMD);
    ssd2828_write_byte(0x00, SSD2828_SEND_DATA);
    ssd2828_write_byte(0x00, SSD2828_SEND_DATA);

    ssd2828_write_byte(0xBC, SSD2828_SEND_CMD);
    ssd2828_write_byte( 1,   SSD2828_SEND_DATA);
    ssd2828_write_byte(0x00, SSD2828_SEND_DATA);
    //delay1(1);

    
	  ssd2828_write_byte(0xbf, SSD2828_SEND_CMD);
    ssd2828_write_byte(cmd, SSD2828_SEND_DATA);
}


#define Generic_Short_Write_1P(a,b)    rm68200_2b(a,b)
#define DCS_Short_Write_NP(a)          rm68200_1b(a)

void HX8394F_init(void)
{
	unsigned char tab1[]={4,0xB9,0xFF,0x83,0x94};
	unsigned char tab2[]={11,0xB1,0x48,0x14,0x74,0x09,0x33,0x54,0x71,0x31,0x57,0x2F};
	unsigned char tab3[]={7,0xBA,0x63,0x03,0x68,0x6B,0xB2,0xC0};
	unsigned char tab4[]={7,0xB2,0x00,0x80,0x64,0x06,0x0E,0x2F};
	unsigned char tab5[]={22,0xB4,0x01,0x70,0x01,0x70,0x01,0x70,0x01,0x05,0x86,0x35,0x00,0x3F,0x1C,0x70,0x01,0x70,0x01,0x70,0x01,0x05,0x86};
	unsigned char tab6[]={34,0xD3,0x00,0x00,0x00,0x00,0x08,0x08,0x10,0x10,0x32,0x10,0x03,0x00,0x03,0x32,0x15,0x07,0x05,0x07,0x32,0x10,0x08,0x00,0x00,0x17,0x01,0x07,0x07,0x17,0x05,0x05,0x17,0x06,0x40};
	unsigned char tab7[]={45,0xD5,0x18,0x18,0x18,0x18,0x26,0x27,0x24,0x25,0x06,0x07,0x04,0x05,0x02,0x03,0x00,0x01,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x20,0x21,0x22,0x23,0x18,0x18,0x18,0x18};
	unsigned char tab8[]={45,0xD6,0x18,0x18,0x18,0x18,0x21,0x20,0x23,0x22,0x01,0x00,0x03,0x02,0x05,0x04,0x07,0x06,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x27,0x26,0x25,0x24,0x18,0x18,0x18,0x18};
	unsigned char tab9[]={59,0xE0,0x00,0x02,0x06,0x0A,0x0C,0x0E,0x10,0x0D,0x1A,0x28,0x38,0x36,0x3F,0x50,0x59,0x61,0x72,0x7B,0x7D,0x93,0xAC,0x56,0x56,0x5C,0x62,0x67,0x71,0x7A,0x7F,0x00,0x02,0x06,0x0A,0x0B,0x0E,0x10,0x0D,0x1A,0x28,0x38,0x36,0x3F,0x50,0x59,0x61,0x73,0x7C,0x7D,0x94,0xAC,0x57,0x56,0x5C,0x62,0x67,0x71,0x7A,0x7F};
	unsigned char tab10[]={2,0xCC,0x0b};
	unsigned char tab11[]={3,0xC0,0x1F,0x73};
	unsigned char tab12[]={3,0xB6,0x3C,0x3C};
	unsigned char tab13[]={2,0xD4,0x02};
	unsigned char tab14[]={2,0xBD,0x01};
	unsigned char tab15[]={2,0x36,0x00};
	unsigned char tab16[]={2,0xB1,0x00};
	unsigned char tab17[]={2,0xBD,0x00};
	unsigned char tab18[]={2,0xC6,0xEF};
	unsigned char tab19[]={1,0x11};
	unsigned char tab20[]={1,0x29};
	//unsigned char tab21[]={1,0x2C};


	PacketSize_b (tab1 );
	PacketSize_b (tab2 );
	PacketSize_b (tab3 );
	PacketSize_b (tab4 );
	PacketSize_b (tab5 );
	PacketSize_b (tab6 );
	PacketSize_b (tab7 );
	PacketSize_b (tab8 );
	PacketSize_b (tab9 );
	PacketSize_b (tab10 );
	PacketSize_b (tab11 );
	PacketSize_b (tab12 );
	PacketSize_b (tab13 );
	PacketSize_b (tab14 );
	PacketSize_b (tab15 );
	PacketSize_b (tab16 );
	PacketSize_b (tab17 );
	PacketSize_b (tab18 );
	PacketSize_b (tab19 );


	delay_ms(150);
	PacketSize_b (tab20 );
	delay_ms(50);
}

void HX8394F_selftest(void)
{
	// Set EXTC
	unsigned char tab1[]={4,0xB9,0xFF,0x83,0x94};
	// Set Power  VSPR=VSNR=5.0  VGH=16V,VGL=-10V
	unsigned char tab2[]={11,0xB1,0x48,0x14,0x74,0x09,0x33,0x54,0x71,0x31,0x57,0x2F};
	// Set MIPI
	unsigned char tab3[]={7,0xBA,0x63,0x03,0x68,0x6B,0xB2,0xC0};
	// Set Display
	unsigned char tab4[]={7,0xB2,0x00,0x80,0x64,0x06,0x0E,0x2F};
	//Modified By GRG For Line Imagesticking
	unsigned char tab5[]={22,0xB4,0x01,0x70,0x01,0x70,0x01,0x70,0x01,0x05,0x86,0x35,0x00,0x3F,0x1C,0x70,0x01,0x70,0x01,0x70,0x01,0x05,0x86};
	// Set D3H
	unsigned char tab6[]={34,0xD3,0x00,0x00,0x00,0x00,0x08,0x08,0x10,0x10,0x32,0x10,0x03,0x00,0x03,0x32,0x15,0x07,0x05,0x07,0x32,0x10,0x08,0x00,0x00,0x17,0x01,0x07,0x07,0x17,0x05,0x05,0x17,0x06,0x40};
	// Set GIP
	unsigned char tab7[]={45,0xD6,0x18,0x18,0x18,0x18,0x21,0x20,0x23,0x22,0x01,0x00,0x03,0x02,0x05,0x04,0x07,0x06,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x27,0x26,0x25,0x24,0x18,0x18,0x18,0x18};
	//Set Gamma
	unsigned char tab8[]={59,0xE0,0x00,0x02,0x06,0x0A,0x0C,0x0E,0x10,0x0D,0x1A,0x28,0x38,0x36,0x3F,0x50,0x59,0x61,0x72,0x7B,0x7D,0x93,0xAC,0x56,0x56,0x5C,0x62,0x67,0x71,0x7A,0x7F,0x00,0x02,0x06,0x0A,0x0B,0x0E,0x10,0x0D,0x1A,0x28,0x38,0x36,0x3F,0x50,0x59,0x61,0x73,0x7C,0x7D,0x94,0xAC,0x57,0x56,0x5C,0x62,0x67,0x71,0x7A,0x7F};
	// Set Panel
	unsigned char tab9[]={2,0xCC,0x0b};
	// Set C0H
	unsigned char tab10[]={3,0xC0,0x1F,0x73};
	// Set VCOM
	unsigned char tab11[]={3,0xB6,0x3C,0x3C};
	// Set D4h
	unsigned char tab12[]={2,0xD4,0x02};
	// Set BDH
	unsigned char tab13[]={2,0xBD,0x01};

	unsigned char tab14[]={2,0x36,0x00};
	// Set GAS
	unsigned char tab15[]={2,0xB1,0x00};
	// Set BD
	unsigned char tab16[]={2,0xBD,0x00};
	
	unsigned char tab17[]={2,0xC6,0xEF};
	//Enable DISP_BIST_EN to start self test
	unsigned char tab18[]={12,0xB2,0x40,0x80,0x64,0x08,0x08,0x2F,0x00,0x00,0x00,0x00,0xC8};
	// Sleep Out
	unsigned char tab19[]={1,0x11};
	// Display ON
	unsigned char tab20[]={1,0x29};
	
	PacketSize_b (tab1 );
	PacketSize_b (tab2 );
	PacketSize_b (tab3 );
	PacketSize_b (tab4 );
	PacketSize_b (tab5 );
	PacketSize_b (tab6 );
	PacketSize_b (tab7 );
	PacketSize_b (tab8 );
	PacketSize_b (tab9 );
	PacketSize_b (tab10 );
	PacketSize_b (tab11 );
	PacketSize_b (tab12 );
	PacketSize_b (tab13 );
	PacketSize_b (tab14 );
	PacketSize_b (tab15 );
	PacketSize_b (tab16 );
	PacketSize_b (tab17 );
	PacketSize_b (tab18 );
	PacketSize_b (tab19 );
	delay_ms(150);
	PacketSize_b (tab20 );
	delay_ms(50);
	
//	  while(1)
//	  {
//		 ssd2828_write_byte ( 0xB0, SSD2828_SEND_CMD );
//		 ssd2828_write_byte ( 0xFA, SSD2828_SEND_CMD );
//	  }

}


void rm6820_init(void)
{
	Generic_Short_Write_1P(0xFE,0x01);
	Generic_Short_Write_1P(0x24,0xC0);
	Generic_Short_Write_1P(0x25,0x53);
	Generic_Short_Write_1P(0x26,0x00);
	Generic_Short_Write_1P(0x2B,0xE5);
	Generic_Short_Write_1P(0x27,0x0A);
	Generic_Short_Write_1P(0x29,0x0A);
	Generic_Short_Write_1P(0x16,0x52);
	Generic_Short_Write_1P(0x2F,0x53);
	Generic_Short_Write_1P(0x34,0x5A);//VGL 5A
	Generic_Short_Write_1P(0x1B,0x00);
	Generic_Short_Write_1P(0x12,0x0A);
	Generic_Short_Write_1P(0x1A,0x06);
	Generic_Short_Write_1P(0x46,0x3F);//VCOM
	Generic_Short_Write_1P(0x52,0xA0);
	Generic_Short_Write_1P(0x53,0x00);
	Generic_Short_Write_1P(0x54,0xA0);
	Generic_Short_Write_1P(0x55,0x00);
	Generic_Short_Write_1P(0xFE,0x03);
	Generic_Short_Write_1P(0x00,0x05);
	Generic_Short_Write_1P(0x01,0x16);
	Generic_Short_Write_1P(0x02,0x0B);
	Generic_Short_Write_1P(0x03,0x0F);
	Generic_Short_Write_1P(0x04,0x7D);
	Generic_Short_Write_1P(0x05,0x00);
	Generic_Short_Write_1P(0x06,0x50);
	Generic_Short_Write_1P(0x07,0x05);
	Generic_Short_Write_1P(0x08,0x16);
	Generic_Short_Write_1P(0x09,0x0D);
	Generic_Short_Write_1P(0x0A,0x11);
	Generic_Short_Write_1P(0x0B,0x7D);
	Generic_Short_Write_1P(0x0C,0x00);
	Generic_Short_Write_1P(0x0D,0x50);
	Generic_Short_Write_1P(0x0E,0x07);
	Generic_Short_Write_1P(0x0F,0x08);
	Generic_Short_Write_1P(0x10,0x01);
	Generic_Short_Write_1P(0x11,0x02);
	Generic_Short_Write_1P(0x12,0x00);
	Generic_Short_Write_1P(0x13,0x7D);
	Generic_Short_Write_1P(0x14,0x00);
	Generic_Short_Write_1P(0x15,0x85);
	Generic_Short_Write_1P(0x16,0x08);
	Generic_Short_Write_1P(0x17,0x03);
	Generic_Short_Write_1P(0x18,0x04);
	Generic_Short_Write_1P(0x19,0x05);
	Generic_Short_Write_1P(0x1A,0x06);
	Generic_Short_Write_1P(0x1B,0x00);
	Generic_Short_Write_1P(0x1C,0x7D);
	Generic_Short_Write_1P(0x1D,0x00);
	Generic_Short_Write_1P(0x1E,0x85);
	Generic_Short_Write_1P(0x1F,0x08);
	Generic_Short_Write_1P(0x20,0x00);
	Generic_Short_Write_1P(0x21,0x00);
	Generic_Short_Write_1P(0x22,0x00);
	Generic_Short_Write_1P(0x23,0x00);
	Generic_Short_Write_1P(0x24,0x00);
	Generic_Short_Write_1P(0x25,0x00);
	Generic_Short_Write_1P(0x26,0x00);
	Generic_Short_Write_1P(0x27,0x00);
	Generic_Short_Write_1P(0x28,0x00);
	Generic_Short_Write_1P(0x29,0x00);
	Generic_Short_Write_1P(0x2A,0x07);
	Generic_Short_Write_1P(0x2B,0x08);
	Generic_Short_Write_1P(0x2D,0x01);
	Generic_Short_Write_1P(0x2F,0x02);
	Generic_Short_Write_1P(0x30,0x00);
	Generic_Short_Write_1P(0x31,0x40);
	Generic_Short_Write_1P(0x32,0x05);
	Generic_Short_Write_1P(0x33,0x08);
	Generic_Short_Write_1P(0x34,0x54);
	Generic_Short_Write_1P(0x35,0x7D);
	Generic_Short_Write_1P(0x36,0x00);
	Generic_Short_Write_1P(0x37,0x03);
	Generic_Short_Write_1P(0x38,0x04);
	Generic_Short_Write_1P(0x39,0x05);
	Generic_Short_Write_1P(0x3A,0x06);
	Generic_Short_Write_1P(0x3B,0x00);
	Generic_Short_Write_1P(0x3D,0x40);
	Generic_Short_Write_1P(0x3F,0x05);
	Generic_Short_Write_1P(0x40,0x08);
	Generic_Short_Write_1P(0x41,0x54);
	Generic_Short_Write_1P(0x42,0x7D);
	Generic_Short_Write_1P(0x43,0x00);
	Generic_Short_Write_1P(0x44,0x00);
	Generic_Short_Write_1P(0x45,0x00);
	Generic_Short_Write_1P(0x46,0x00);
	Generic_Short_Write_1P(0x47,0x00);
	Generic_Short_Write_1P(0x48,0x00);
	Generic_Short_Write_1P(0x49,0x00);
	Generic_Short_Write_1P(0x4A,0x00);
	Generic_Short_Write_1P(0x4B,0x00);
	Generic_Short_Write_1P(0x4C,0x00);
	Generic_Short_Write_1P(0x4D,0x00);
	Generic_Short_Write_1P(0x4E,0x00);
	Generic_Short_Write_1P(0x4F,0x00);
	Generic_Short_Write_1P(0x50,0x00);
	Generic_Short_Write_1P(0x51,0x00);
	Generic_Short_Write_1P(0x52,0x00);
	Generic_Short_Write_1P(0x53,0x00);
	Generic_Short_Write_1P(0x54,0x00);
	Generic_Short_Write_1P(0x55,0x00);
	Generic_Short_Write_1P(0x56,0x00);
	Generic_Short_Write_1P(0x58,0x00);
	Generic_Short_Write_1P(0x59,0x00);
	Generic_Short_Write_1P(0x5A,0x00);
	Generic_Short_Write_1P(0x5B,0x00);
	Generic_Short_Write_1P(0x5C,0x00);
	Generic_Short_Write_1P(0x5D,0x00);
	Generic_Short_Write_1P(0x5E,0x00);
	Generic_Short_Write_1P(0x5F,0x00);
	Generic_Short_Write_1P(0x60,0x00);
	Generic_Short_Write_1P(0x61,0x00);
	Generic_Short_Write_1P(0x62,0x00);
	Generic_Short_Write_1P(0x63,0x00);
	Generic_Short_Write_1P(0x64,0x00);
	Generic_Short_Write_1P(0x65,0x00);
	Generic_Short_Write_1P(0x66,0x00);
	Generic_Short_Write_1P(0x67,0x00);
	Generic_Short_Write_1P(0x68,0x00);
	Generic_Short_Write_1P(0x69,0x00);
	Generic_Short_Write_1P(0x6A,0x00);
	Generic_Short_Write_1P(0x6B,0x00);
	Generic_Short_Write_1P(0x6C,0x00);
	Generic_Short_Write_1P(0x6D,0x00);
	Generic_Short_Write_1P(0x6E,0x00);
	Generic_Short_Write_1P(0x6F,0x00);
	Generic_Short_Write_1P(0x70,0x00);
	Generic_Short_Write_1P(0x71,0x00);
	Generic_Short_Write_1P(0x72,0x20);
	Generic_Short_Write_1P(0x73,0x00);
	Generic_Short_Write_1P(0x74,0x08);
	Generic_Short_Write_1P(0x75,0x08);
	Generic_Short_Write_1P(0x76,0x08);
	Generic_Short_Write_1P(0x77,0x08);
	Generic_Short_Write_1P(0x78,0x08);
	Generic_Short_Write_1P(0x79,0x08);
	Generic_Short_Write_1P(0x7A,0x00);
	Generic_Short_Write_1P(0x7B,0x00);
	Generic_Short_Write_1P(0x7C,0x00);
	Generic_Short_Write_1P(0x7D,0x00);
	Generic_Short_Write_1P(0x7E,0xBF);
	Generic_Short_Write_1P(0x7F,0x02);
	Generic_Short_Write_1P(0x80,0x06);
	Generic_Short_Write_1P(0x81,0x14);
	Generic_Short_Write_1P(0x82,0x10);
	Generic_Short_Write_1P(0x83,0x16);
	Generic_Short_Write_1P(0x84,0x12);
	Generic_Short_Write_1P(0x85,0x08);
	Generic_Short_Write_1P(0x86,0x3F);
	Generic_Short_Write_1P(0x87,0x3F);
	Generic_Short_Write_1P(0x88,0x3F);
	Generic_Short_Write_1P(0x89,0x3F);
	Generic_Short_Write_1P(0x8A,0x3F);
	Generic_Short_Write_1P(0x8B,0x0C);
	Generic_Short_Write_1P(0x8C,0x0A);
	Generic_Short_Write_1P(0x8D,0x0E);
	Generic_Short_Write_1P(0x8E,0x3F);
	Generic_Short_Write_1P(0x8F,0x3F);
	Generic_Short_Write_1P(0x90,0x00);
	Generic_Short_Write_1P(0x91,0x04);
	Generic_Short_Write_1P(0x92,0x3F);
	Generic_Short_Write_1P(0x93,0x3F);
	Generic_Short_Write_1P(0x94,0x3F);
	Generic_Short_Write_1P(0x95,0x3F);
	Generic_Short_Write_1P(0x96,0x05);
	Generic_Short_Write_1P(0x97,0x01);
	Generic_Short_Write_1P(0x98,0x3F);
	Generic_Short_Write_1P(0x99,0x3F);
	Generic_Short_Write_1P(0x9A,0x0F);
	Generic_Short_Write_1P(0x9B,0x0B);
	Generic_Short_Write_1P(0x9C,0x0D);
	Generic_Short_Write_1P(0x9D,0x3F);
	Generic_Short_Write_1P(0x9E,0x3F);
	Generic_Short_Write_1P(0x9F,0x3F);
	Generic_Short_Write_1P(0xA0,0x3F);
	Generic_Short_Write_1P(0xA2,0x3F);
	Generic_Short_Write_1P(0xA3,0x09);
	Generic_Short_Write_1P(0xA4,0x13);
	Generic_Short_Write_1P(0xA5,0x17);
	Generic_Short_Write_1P(0xA6,0x11);
	Generic_Short_Write_1P(0xA7,0x15);
	Generic_Short_Write_1P(0xA9,0x07);
	Generic_Short_Write_1P(0xAA,0x03);
	Generic_Short_Write_1P(0xAB,0x3F);
	Generic_Short_Write_1P(0xAC,0x3F);
	Generic_Short_Write_1P(0xAD,0x05);
	Generic_Short_Write_1P(0xAE,0x01);
	Generic_Short_Write_1P(0xAF,0x17);
	Generic_Short_Write_1P(0xB0,0x13);
	Generic_Short_Write_1P(0xB1,0x15);
	Generic_Short_Write_1P(0xB2,0x11);
	Generic_Short_Write_1P(0xB3,0x0F);
	Generic_Short_Write_1P(0xB4,0x3F);
	Generic_Short_Write_1P(0xB5,0x3F);
	Generic_Short_Write_1P(0xB6,0x3F);
	Generic_Short_Write_1P(0xB7,0x3F);
	Generic_Short_Write_1P(0xB8,0x3F);
	Generic_Short_Write_1P(0xB9,0x0B);
	Generic_Short_Write_1P(0xBA,0x0D);
	Generic_Short_Write_1P(0xBB,0x09);
	Generic_Short_Write_1P(0xBC,0x3F);
	Generic_Short_Write_1P(0xBD,0x3F);
	Generic_Short_Write_1P(0xBE,0x07);
	Generic_Short_Write_1P(0xBF,0x03);
	Generic_Short_Write_1P(0xC0,0x3F);
	Generic_Short_Write_1P(0xC1,0x3F);
	Generic_Short_Write_1P(0xC2,0x3F);
	Generic_Short_Write_1P(0xC3,0x3F);
	Generic_Short_Write_1P(0xC4,0x02);
	Generic_Short_Write_1P(0xC5,0x06);
	Generic_Short_Write_1P(0xC6,0x3F);
	Generic_Short_Write_1P(0xC7,0x3F);
	Generic_Short_Write_1P(0xC8,0x08);
	Generic_Short_Write_1P(0xC9,0x0C);
	Generic_Short_Write_1P(0xCA,0x0A);
	Generic_Short_Write_1P(0xCB,0x3F);
	Generic_Short_Write_1P(0xCC,0x3F);
	Generic_Short_Write_1P(0xCD,0x3F);
	Generic_Short_Write_1P(0xCE,0x3F);
	Generic_Short_Write_1P(0xCF,0x3F);
	Generic_Short_Write_1P(0xD0,0x0E);
	Generic_Short_Write_1P(0xD1,0x10);
	Generic_Short_Write_1P(0xD2,0x14);
	Generic_Short_Write_1P(0xD3,0x12);
	Generic_Short_Write_1P(0xD4,0x16);
	Generic_Short_Write_1P(0xD5,0x00);
	Generic_Short_Write_1P(0xD6,0x04);
	Generic_Short_Write_1P(0xD7,0x3F);
	Generic_Short_Write_1P(0xDC,0x02);
	Generic_Short_Write_1P(0xDE,0x12);
	Generic_Short_Write_1P(0xFE,0x0E);
	Generic_Short_Write_1P(0x01,0x75);
	Generic_Short_Write_1P(0x54,0x01);
	Generic_Short_Write_1P(0xFE,0x04);
	Generic_Short_Write_1P(0x60,0x00);
	Generic_Short_Write_1P(0x61,0x0C);
	Generic_Short_Write_1P(0x62,0x12);
	Generic_Short_Write_1P(0x63,0x0E);
	Generic_Short_Write_1P(0x64,0x06);
	Generic_Short_Write_1P(0x65,0x12);
	Generic_Short_Write_1P(0x66,0x0E);
	Generic_Short_Write_1P(0x67,0x0B);
	Generic_Short_Write_1P(0x68,0x15);
	Generic_Short_Write_1P(0x69,0x0B);
	Generic_Short_Write_1P(0x6A,0x10);
	Generic_Short_Write_1P(0x6B,0x07);
	Generic_Short_Write_1P(0x6C,0x0F);
	Generic_Short_Write_1P(0x6D,0x12);
	Generic_Short_Write_1P(0x6E,0x0C);
	Generic_Short_Write_1P(0x6F,0x00);
	Generic_Short_Write_1P(0x70,0x00);
	Generic_Short_Write_1P(0x71,0x0C);
	Generic_Short_Write_1P(0x72,0x12);
	Generic_Short_Write_1P(0x73,0x0E);
	Generic_Short_Write_1P(0x74,0x06);
	Generic_Short_Write_1P(0x75,0x12);
	Generic_Short_Write_1P(0x76,0x0E);
	Generic_Short_Write_1P(0x77,0x0B);
	Generic_Short_Write_1P(0x78,0x15);
	Generic_Short_Write_1P(0x79,0x0B);
	Generic_Short_Write_1P(0x7A,0x10);
	Generic_Short_Write_1P(0x7B,0x07);
	Generic_Short_Write_1P(0x7C,0x0F);
	Generic_Short_Write_1P(0x7D,0x12);
	Generic_Short_Write_1P(0x7E,0x0C);
	Generic_Short_Write_1P(0x7F,0x00);
	Generic_Short_Write_1P(0xFE,0x00);
	Generic_Short_Write_1P(0x58,0xAD);


	//Generic_Short_Write_1P(0xFE,0x0E);
	//Generic_Short_Write_1P(0x35,0x80);

	DCS_Short_Write_NP(0x11);
	delay_ms(800);

	DCS_Short_Write_NP(0x29);
	delay_ms(20);
}

void LCD_InitCFG(void) 
{  

    volatile uint16_t id = 0x00;

//// Reset LCD Driver and ssd2828
	
	SSD2828_RST_SET_();
//	SSD2828_RST_SET();
	delay_ms(10);      // Delay 10ms
	SSD2828_RST_CLR_();
//  SSD2828_RST_CLR();
    delay_ms(10);      // Delay 10ms This Delay time is necessary
    SSD2828_RST_SET_();
//  SSD2828_RST_SET();
	delay_ms(50); // Delay 50 ms
	
//	Test the ssd2828 singal
/*
while(1)
{			
	SSD2828_RST_SET_();
	delay_ms(1);      // Delay 1ms

	SSD2828_RST_CLR_();
	delay_ms(1);      // Delay 1ms
		
	ssd2828_write_byte ( 0xB0, SSD2828_SEND_CMD );
	ssd2828_write_byte ( 0xFA, SSD2828_SEND_CMD );
}
*/
    //SSD2825_Initial
	ssd2828_write_byte(0xb7,SSD2828_SEND_CMD);
	ssd2828_write_byte(0x50,SSD2828_SEND_DATA);		// 50=TX_CLK 70=PCLK
	ssd2828_write_byte(0x00,SSD2828_SEND_DATA);     // Configuration Register

	ssd2828_write_byte(0xb8,SSD2828_SEND_CMD);
	ssd2828_write_byte(0x00,SSD2828_SEND_DATA);
	ssd2828_write_byte(0x00,SSD2828_SEND_DATA);     // VC(Virtual ChannelID) Control Register

	ssd2828_write_byte(0xb9,SSD2828_SEND_CMD);
	ssd2828_write_byte(0x00,SSD2828_SEND_DATA);		// 1 = PLL disable
	ssd2828_write_byte(0x00,SSD2828_SEND_DATA);
    
	//TX_CLK/MS should be between 5Mhz to100Mhz
	ssd2828_write_byte(0xBA,SSD2828_SEND_CMD); 		// PLL = (TX_CLK/MS)*NS 8228=480M 4428=240M  061E=120M 4214=240M 821E=360M 8219=300M
	ssd2828_write_byte(0x1E,SSD2828_SEND_DATA);		// D7-0 = NS(0x01 : NS=1)
	ssd2828_write_byte(0x82,SSD2828_SEND_DATA);		// D15-14 = PLL范围 00=62.5-125 01=126-250 10=251-500 11=501-1000  DB12-8=MS(01:MS=1)

	ssd2828_write_byte(0xBB,SSD2828_SEND_CMD);		// LP Clock Divider LP clock = 400MHz / LPD / 8 = 240 / 8 / 4 = 7.5MHz
	ssd2828_write_byte(0x06,SSD2828_SEND_DATA);		// D5-0 = LPD=0x1 ??Divide by 2
	ssd2828_write_byte(0x00,SSD2828_SEND_DATA);

	ssd2828_write_byte(0xb9,SSD2828_SEND_CMD);
	ssd2828_write_byte(0x01,SSD2828_SEND_DATA);		// 1 = PLL enable
	ssd2828_write_byte(0x00,SSD2828_SEND_DATA);
    delay_ms(100);

	//MIPI lane configuration
	ssd2828_write_byte(0xDE,SSD2828_SEND_CMD);		// 通道??
	ssd2828_write_byte(0x03,SSD2828_SEND_DATA);		// 11=4LANE 10=3LANE 01=2LANE 00=1LANE
	ssd2828_write_byte(0x00,SSD2828_SEND_DATA);


// init lcd driver
	rm6820_init();



	//SSD2825_Initial

	ssd2828_write_byte(0xc9,SSD2828_SEND_CMD);
	ssd2828_write_byte(/*0x02*/0x02,SSD2828_SEND_DATA);
	ssd2828_write_byte(/*0x23*/0x23,SSD2828_SEND_DATA);   	// p1: HS-Data-zero  p2: HS-Data- prepare  --> 8031 issue
	//delay_ms(5);
	
	ssd2828_write_byte(0xCA,SSD2828_SEND_CMD);
	ssd2828_write_byte(/*0x02*/0x02,SSD2828_SEND_DATA);   	// CLK Prepare
	ssd2828_write_byte(/*0x23*/0x23,SSD2828_SEND_DATA);   	// Clk Zero
	
	ssd2828_write_byte(0xCB,SSD2828_SEND_CMD); 		// local_write_reg(addr=0xCB,data=0x0510)
	ssd2828_write_byte(/*0x02*/0x02,SSD2828_SEND_DATA); 	// Clk Post
	ssd2828_write_byte(/*0x23*/0x23,SSD2828_SEND_DATA); 	// Clk Per

	ssd2828_write_byte(0xCC,SSD2828_SEND_CMD); 		// local_write_reg(addr=0xCC,data=0x100A)
	ssd2828_write_byte(0x05,SSD2828_SEND_DATA); 	// HS Trail
	ssd2828_write_byte(0x10,SSD2828_SEND_DATA); 	// Clk Trail
	//delay_ms(5);

	ssd2828_write_byte(0xD0,SSD2828_SEND_CMD);
	ssd2828_write_byte(0x00,SSD2828_SEND_DATA);
	ssd2828_write_byte(0x00,SSD2828_SEND_DATA);
	//delay_ms(5);
	
	//RGB interface configuration
	ssd2828_write_byte(0xB1,SSD2828_SEND_CMD);
	ssd2828_write_byte(HSPW,SSD2828_SEND_DATA);
	ssd2828_write_byte(VSPW,SSD2828_SEND_DATA);

	ssd2828_write_byte(0xB2,SSD2828_SEND_CMD);
	ssd2828_write_byte(HBPD,SSD2828_SEND_DATA);
	ssd2828_write_byte(VBPD,SSD2828_SEND_DATA);

	ssd2828_write_byte(0xB3,SSD2828_SEND_CMD);
	ssd2828_write_byte(HFPD,SSD2828_SEND_DATA);
	ssd2828_write_byte(VFPD,SSD2828_SEND_DATA);

	ssd2828_write_byte(0xB4,SSD2828_SEND_CMD);		// Horizontal active period 720=02D0
	ssd2828_write_byte(0xD0,SSD2828_SEND_DATA);		// 013F = 319 02D0 = 720
	ssd2828_write_byte(0x02,SSD2828_SEND_DATA);		// HACT = 0x01E0 = 480	,0x021c = 540

	ssd2828_write_byte(0xB5,SSD2828_SEND_CMD);		// Vertical active period 1280 = 0500
	ssd2828_write_byte(0x00,SSD2828_SEND_DATA);		// 01DF = 479 0500 = 1280
	ssd2828_write_byte(0x05,SSD2828_SEND_DATA);		// VACT = 0x0320=800	,0x03c0 = 960	 ,0x0356 = 854

	ssd2828_write_byte(0xB6,SSD2828_SEND_CMD);		// RGB CLK  16BPP=00 18BPP=01
	ssd2828_write_byte(0x24/*0x07*/,SSD2828_SEND_DATA);		// D7=0 D6=0 D5=0  D1-0=11 ??24bpp
	ssd2828_write_byte(0x21/*0xe1*/,SSD2828_SEND_DATA);		// D15=VS D14=HS D13=CLK D12-9=NC D8=0=Video with blanking packet. 00-F0

	//MIPI lane configuration
	ssd2828_write_byte(0xDE,SSD2828_SEND_CMD);		// 通道??
	ssd2828_write_byte(0x03,SSD2828_SEND_DATA);		// 11 = 4LANE 10 = 3LANE 01 = 2LANE 00 = 1LANE
	ssd2828_write_byte(0x00,SSD2828_SEND_DATA);

	ssd2828_write_byte(0xD6,SSD2828_SEND_CMD);		// 05 = BGR  04 = RGB
	ssd2828_write_byte(0x04,SSD2828_SEND_DATA);		// D0 = 0 = RGB 1:BGR D1 = 1 = Most significant byte sent first
	ssd2828_write_byte(0x00,SSD2828_SEND_DATA);

	ssd2828_write_byte(0xB7,SSD2828_SEND_CMD);
	ssd2828_write_byte(0x4b,SSD2828_SEND_DATA);    // 5b
	ssd2828_write_byte(0x03,SSD2828_SEND_DATA);    // 02
	delay_ms(10);

	ssd2828_write_byte(0x2C,SSD2828_SEND_CMD);
	
} 


/*******************************************************************************
 * Code
 ******************************************************************************/

/* Initialize the LCD_DISP. */
void BOARD_InitLcd(void)
{
    volatile uint32_t i = 0x100U;

    gpio_pin_config_t config =  {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

#if 0
    /* Reset the LCD. */
    GPIO_PinInit(LCD_DISP_GPIO, LCD_DISP_GPIO_PIN, &config);

    GPIO_PinWrite(LCD_DISP_GPIO, LCD_DISP_GPIO_PIN, 0);

    while (i--)
    {
    }

    GPIO_PinWrite(LCD_DISP_GPIO, LCD_DISP_GPIO_PIN, 1);
#endif
    /* open lcd Backlight. */
    config.outputLogic = 1;
    GPIO_PinInit(LCD_BL_GPIO, LCD_BL_GPIO_PIN, &config);
	GPIO_PinWrite(LCD_BL_GPIO, LCD_BL_GPIO_PIN, 1);
}

void BOARD_InitLcdifPixelClock(void)
{
    /*
     * The desired output frame rate is 60Hz. So the pixel clock frequency is:
     * (720 + 41 + 4 + 18) * (1280 + 10 + 4 + 2) * 60 =65M.
     * Here set the LCDIF pixel clock to 65M.
     */

    /*
     * Initialize the Video PLL.
     * Video PLL output clock is OSC24M * (loopDivider + (denominator / numerator)) / postDivider = 93MHz.
     */
    clock_video_pll_config_t config = {
        .loopDivider = 31, .postDivider = 4, .numerator = 0, .denominator = 0,
    };

    CLOCK_InitVideoPll(&config);

    /*
     * 000 derive clock from PLL2
     * 001 derive clock from PLL3 PFD3
     * 010 derive clock from PLL5
     * 011 derive clock from PLL2 PFD0
     * 100 derive clock from PLL2 PFD1
     * 101 derive clock from PLL3 PFD1
     */
    CLOCK_SetMux(kCLOCK_LcdifPreMux, 2); 

    CLOCK_SetDiv(kCLOCK_LcdifPreDiv, 5);  //4

    CLOCK_SetDiv(kCLOCK_LcdifDiv, 0);  // 0

}

void ssd2828_gpioinit(void ) 

{
    /* Define the init structure for the output SSD2828 pin*/
    gpio_pin_config_t lcd_config =  {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
    gpio_pin_config_t lcd_config1 = {kGPIO_DigitalInput,  0, kGPIO_NoIntmode};

	/* Init output SSD2828 GPIO. */

// CS     129
// SCK    119
// MOSI   120
// RESET  316
// MISO   121

	GPIO_PinInit(GPIO1, 29, &lcd_config);
	GPIO_PinInit(GPIO1, 19, &lcd_config);
	GPIO_PinInit(GPIO1, 20, &lcd_config);
	//GPIO_PinInit(GPIO3, 15, &lcd_config1);
	GPIO_PinInit(GPIO3, 16, &lcd_config);

}

/*!
 * @brief Main function
 */
int Lcd_InitPort(void)
{

	/*Init LCD Pins;*/
  	LCD_InitPins();
    BOARD_InitLcdifPixelClock();

	// open lcd backlight
    BOARD_InitLcd();
    // init ssd2828 spi gpio
	ssd2828_gpioinit();

    SSD2828_CS_SET();
    // init lcd driver and ssd2828
    LCD_InitCFG();
}



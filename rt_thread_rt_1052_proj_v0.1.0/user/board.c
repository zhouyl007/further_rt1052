/*
 * The Clear BSD License
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"

#include <rthw.h>
#include <rtthread.h>


#include "bsp_led.h"
#include "bsp_sdram.h"


/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Get debug console frequency. */
uint32_t BOARD_DebugConsoleSrcFreq(void)
{
    uint32_t freq;

    /* To make it simple, we assume default PLL and divider settings, and the only variable
       from application is use PLL3 source or OSC source */
    if (CLOCK_GetMux(kCLOCK_UartMux) == 0) /* PLL3 div6 80M */
    {
        freq = (CLOCK_GetPllFreq(kCLOCK_PllUsb1) / 6U) / (CLOCK_GetDiv(kCLOCK_UartDiv) + 1U);
    }
    else
    {
        freq = CLOCK_GetOscFreq() / (CLOCK_GetDiv(kCLOCK_UartDiv) + 1U);
    }

    return freq;
}

/* Initialize debug console. */
void BOARD_InitDebugConsole(void)
{
    uint32_t uartClkSrcFreq = BOARD_DebugConsoleSrcFreq();

    DbgConsole_Init(BOARD_DEBUG_UART_BASEADDR, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}

/* MPU configuration. */
void BOARD_ConfigMPU(void)
{
    /* Disable I cache and D cache */
    if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR)) {
        SCB_DisableICache();
    }
    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR)) {
        SCB_DisableDCache();
    }

    /* Disable MPU */
    ARM_MPU_Disable();

    /* MPU configure:
     * Use ARM_MPU_RASR(DisableExec, AccessPermission, TypeExtField, IsShareable, IsCacheable, IsBufferable, SubRegionDisable, Size) 
     * API in core_cm7.h.
     * param DisableExec       Instruction access (XN) disable bit,0=instruction fetches enabled, 1=instruction fetches disabled.
     * param AccessPermission  Data access permissions, allows you to configure read/write access for User and Privileged mode.
     *      Use MACROS defined in core_cm7.h: ARM_MPU_AP_NONE/ARM_MPU_AP_PRIV/ARM_MPU_AP_URO/ARM_MPU_AP_FULL/ARM_MPU_AP_PRO/ARM_MPU_AP_RO
     * Combine TypeExtField/IsShareable/IsCacheable/IsBufferable to configure MPU memory access attributes.
     *  TypeExtField  IsShareable  IsCacheable  IsBufferable   Memory Attribtue    Shareability        Cache
     *     0             x           0           0             Strongly Ordered    shareable
     *     0             x           0           1              Device             shareable           
     *     0             0           1           0              Normal             not shareable   Outer and inner write through no write allocate           
     *     0             0           1           1              Normal             not shareable   Outer and inner write back no write allocate           
     *     0             1           1           0              Normal             shareable       Outer and inner write through no write allocate    
     *     0             1           1           1              Normal             shareable       Outer and inner write back no write allocate    
     *     1             0           0           0              Normal             not shareable   outer and inner noncache
     *     1             1           0           0              Normal             shareable       outer and inner noncache
     *     1             0           1           1              Normal             not shareable   outer and inner write back write/read acllocate
     *     1             1           1           1              Normal             shareable       outer and inner write back write/read acllocate
     *     2             x           0           0              Device              not shareable   
     *  Above are normal use settings, if your want to see more details or want to config different inner/outter cache policy.      
     *  please refer to Table 4-55 /4-56 in arm cortex-M7 generic user guide <dui0646b_cortex_m7_dgug.pdf>
     * param SubRegionDisable  Sub-region disable field. 0=sub-region is enabled, 1=sub-region is disabled.
     * param Size              Region size of the region to be configured. use ARM_MPU_REGION_SIZE_xxx MACRO in core_cm7.h.
     */

    /* Region 0 setting: Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(0, 0xC0000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);

    /* Region 1 setting: Memory with Device type, not shareable,  non-cacheable. */
    //MPU->RBAR = ARM_MPU_RBAR(1, 0x80000000U);
    //MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1GB);

/* Region 2 setting */
#if defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
    /* Setting Memory with Normal type, not shareable, outer/inner write back. */
    MPU->RBAR = ARM_MPU_RBAR(2, 0x60000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_512MB);
#else
    /* Setting Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(2, 0x60000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);
#endif

    /* Region 3 setting: Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(3, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1GB);

    /* Region 4 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(4, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_128KB);

    /* Region 5 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(5, 0x20000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_128KB);

    /* Region 6 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(6, 0x20200000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_256KB);

    /* Region 7 setting: Memory with Normal type, not shareable, outer/inner write back */
    //MPU->RBAR = ARM_MPU_RBAR(7, 0x80000000U);
    //MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_16MB);

    /* Region 8 setting, set last 2MB of SDRAM can't be accessed by cache, glocal variables which are not expected to be
     * accessed by cache can be put here */
    /* Memory with Normal type, not shareable, non-cacheable */
    //MPU->RBAR = ARM_MPU_RBAR(8, 0x80280000);
    //MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_16MB);

    /* Enable MPU */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);

    /* Enable I cache and D cache */
    SCB_EnableDCache();
    SCB_EnableICache();
}

/*
*************************************************************************
*                         以下是RT-Thread相关的函数
*************************************************************************
*/

#ifdef RT_USING_HEAP

#if defined(RT_USING_MEMHEAP) && defined(RT_USING_MEMHEAP_AS_HEAP)
// 从外部SDRAM里面分配一部分静态内存来作为rtt的堆空间，这里配置为30MB 

#define SDRAM_BEGIN         (0x80500000u)
#define SDRAM_END           (0x80700000u)

//#define LCD_FB_BASE         (0x80280000u)

#elif defined(RT_USING_SMALL_MEM)
// 从内部SRAM（即DTCM）里面分配一部分静态内存来作为rtt的堆空间，这里配置为4KB 

#define RT_HEAP_SIZE (1024 * 64)
static uint32_t rt_heap[RT_HEAP_SIZE];

RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif

#endif 


/**
  * @brief  开发板硬件初始化函数
  * @param  无
  * @retval 无
  *
  * @attention
  * RTT把开发板相关的初始化函数统一放到board.c文件中实现，
  * 当然，你想把这些函数统一放到main.c文件也是可以的。
  */
extern int Lcd_InitPort(void);
extern int imxrt_sdram_init(void);

void RT1052_NVIC_SetPriorityGrouping(u8 grpx)
{
	NVIC_SetPriorityGrouping((uint32_t)grpx);
}

//脡猫脰脙脰脨露脧脫脜脧脠录露
//IRQn:	脰脨露脧脭麓
//prep拢潞脟脌脮录脫脜脧脠录露
//subp拢潞脳脫脫脜脧脠录露
void RT1052_NVIC_SetPriority(IRQn_Type IRQn,u32 prep,u32 subp)
{ 
	u32 prioritygroup = 0x00;
  
	prioritygroup=NVIC_GetPriorityGrouping();
	NVIC_SetPriority(IRQn,NVIC_EncodePriority(prioritygroup,prep,subp));
}

//脡猫脰脙脧貌脕驴卤铆脝芦脪脝碌脴脰路
//vtab:禄霉脰路
//offset:脝芦脪脝脕驴		 
void MY_NVIC_SetVectorTable(u32 vtab,u32 offset)	 
{ 	   	  
	SCB->VTOR=vtab|(offset&(u32)0xFFFFFE00);	//脡猫脰脙NVIC碌脛脧貌脕驴卤铆脝芦脪脝录脛麓忙脝梅,VTOR碌脥9脦禄卤拢脕么,录麓[8:0]卤拢脕么隆拢
}


void rt_hw_board_init(void)
{  

	/* 初始化内存管理单元 */
    BOARD_ConfigMPU();  // init the MPU
  
	/* 初始化时钟 */
    BOARD_BootClockRUN();   // init systeam clock
  
	/* 初始化调试串口 */
	/*BOARD_InitDebugConsole();*/
	// Do not use imxrt_sdram_init this function, use it will cause the system to run very slowly
	// Do not use imxrt_sdram_init this function, use it will cause the system to run very slowly
	// Do not use imxrt_sdram_init this function, use it will cause the system to run very slowly
	
	#if XIP_EXTERNAL_FLASH == 1
	   //imxrt_sdram_init();  // zlg SDRAM init
	   SDRAM_Init();          // zhengdianyuanzi SDRAM init 
	#endif
	RT1052_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4); 

	
	/* 初始化LED引脚 */
	LED_GPIO_Config();       // init the used gpio

	Lcd_InitPort();          // init lcd gpio
	
    /* 初始化SysTick */
    SysTick_Config( SystemCoreClock / RT_TICK_PER_SECOND );  // init rtthread tick 

/* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif
    
#if defined(RT_USING_CONSOLE) && defined(RT_USING_DEVICE)
	rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif
    
#ifdef RT_USING_HEAP
	#if defined(RT_USING_MEMHEAP) && defined(RT_USING_MEMHEAP_AS_HEAP)
    	rt_kprintf("sdram heap, begin: 0x%p, end: 0x%p\n", SDRAM_BEGIN, SDRAM_END);
    	rt_system_heap_init((void *)SDRAM_BEGIN, (void *)SDRAM_END);
	#elif defined(RT_USING_SMALL_MEM)
    /*rt_kprintf("sram heap, begin: 0x%p, end: 0x%p\n", HEAP_BEGIN, HEAP_END);*/
    	rt_system_heap_init((void *)rt_heap_begin_get, (void *)rt_heap_end_get);
	#endif
#endif 
}


/**
  * @brief  SysTick中断服务函数
  * @param  无
  * @retval 无
  *
  * @attention
  *
  */
void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}



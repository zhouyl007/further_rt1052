/**
  ******************************************************************
  * @file    main.c
  * @author  
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   用V2.3.1版本库建的工程模板
  ******************************************************************
  */
/*
*************************************************************************
*                             包含的头文件
*************************************************************************
*/ 

#include "board.h"
#include "fsl_debug_console.h"
#include "bsp_led.h"

/* RT-Thread 头文件 */
#include "rtthread.h"
#include <rtdevice.h> 

// bsp driver include files 
#include "bsp_board_init.h"
#include "bsp_powerpwm.h"
#include "bsp_ts_ft6336_sf.h"
#include "bsp_spiflash.h"
#include "bsp_spiflash2.h"

#include "ff.h"

/*
*************************************************************************
*                               宏定义&变量
*************************************************************************
*/

#define LED1_THREAD_PRIORITY            10
#define LED1_THREAD_STACK_SIZE          1024
#define LED1_THREAD_TIMESLICE           50

#define SENSOR_THREAD_PRIORITY          9
#define SENSOR_THREAD_STACK_SIZE        (1024)
#define SENSOR_THREAD_TIMESLICE         50
ALIGN(4)
static rt_uint8_t sensor_thread_stack[SENSOR_THREAD_STACK_SIZE];	/* 线程2栈 */

#define LCD_THREAD_PRIORITY          	  5
#define LCD_THREAD_STACK_SIZE        	(1024 * 2)
#define LCD_THREAD_TIMESLICE         	100
ALIGN(4)
static rt_uint8_t lcd_thread_stack[LCD_THREAD_STACK_SIZE];	/* 线程3栈 */

#define TOUCH_THREAD_PRIORITY          	  11
#define TOUCH_THREAD_STACK_SIZE        	(512 * 2)
#define TOUCH_THREAD_TIMESLICE         	10
ALIGN(4)
static rt_uint8_t touch_thread_stack[TOUCH_THREAD_STACK_SIZE];	 



/*
*************************************************************************
*                               变量
*************************************************************************
*/
/* 定义线程控制块指针 */
static  rt_thread_t led_thread = RT_NULL;
/* 线程2控制块 */
static struct rt_thread sensor_thread;
/* 线程3控制块 */
static struct rt_thread lcd_thread;

static struct rt_thread touch_thread;

/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void led_thread_entry(void* parameter);
static void sensor_thread_entry(void* parameter);
static void lcd_thread_entry(void* parameter);
static void touch_thread_entry(void* parameter);

/* 消息队列控制块 */
static struct rt_messagequeue mq;
/* 消息队列中用到的放置消息的内存池 */
static char msg_pool[64];

/* 指向线程控制块的指针 */
static rt_mutex_t ui_mutex = RT_NULL;
static rt_mutex_t button_mutex = RT_NULL;

/*
*************************************************************************
*                             main 函数
*************************************************************************
*/
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
// this is the flag of system is on
// if sysruning = 0, system is power down
// if sysruning = 1, system is power on
static int timer_init(void);
int main(void)
{
	rt_err_t result;
    /* 
	   * 开发板硬件初始化，RTT系统初始化已经在main函数之前完成，
	   * 即在component.c文件中的rtthread_startup()函数中完成了。
	   * 所以在main函数中，只需要创建线程和启动线程即可。
	   */

	rt_kprintf("sdram heap, begin: 0x%p, end\n");


	rt1052_board_user_init();
	timer_init();

    /* 初始化消息队列 */
    rt_mq_init(&mq, "mqt",
               &msg_pool[0],        /* 内存池指向msg_pool */
               32 - sizeof(void*),  /* 每个消息的大小是 128 - void* */
               sizeof(msg_pool),    /* 内存池的大小是msg_pool的大小 */
               RT_IPC_FLAG_FIFO);   /* 如果有多个线程等待，按照先来先得到的方法分配消息 */

    /* 创建互斥锁 */
    ui_mutex = rt_mutex_create("mutex", RT_IPC_FLAG_FIFO);
    if (ui_mutex == RT_NULL) {
        //tc_stat(TC_STAT_END | TC_STAT_FAILED);
        return -1;
    }

    /* 创建互斥锁 */
    button_mutex = rt_mutex_create("mutex", RT_IPC_FLAG_FIFO);
    if (button_mutex == RT_NULL) {
        //tc_stat(TC_STAT_END | TC_STAT_FAILED);
        return -1;
    }
	
	led_thread = rt_thread_create("led",                   	 /* 线程名字，字符串形式 */
                                    led_thread_entry,           /* 线程入口函数 */
                                    RT_NULL,                    /* 线程入口函数参数 */
                                    LED1_THREAD_STACK_SIZE,     /* 线程栈大小，单位为字节 */
                                    LED1_THREAD_PRIORITY,       /* 线程优先级，数值越大，优先级越小 */                  
                                    LED1_THREAD_TIMESLICE);     /* 线程时间片 */
  
  	if (led_thread != RT_NULL)
   	   	rt_thread_startup(led_thread);
  	else
    	return -1;

	/* 初始化线程2 */
	result = rt_thread_init(&sensor_thread, 
							"sensor", 							/* 线程名：sensor */
							sensor_thread_entry, 				/* 线程的入口是sensor_thread_entry，入口参数是2 */
							(void*)2,					 
							&sensor_thread_stack[0], 
							sizeof(sensor_thread_stack), 		/* 线程栈是thread2_stack */
							SENSOR_THREAD_PRIORITY, 
							SENSOR_THREAD_TIMESLICE);
	
	if (result == RT_EOK) {										/* 如果返回正确，启动线程2 */
		rt_kprintf("create sensor_thread succeed \r\n"); 
		rt_thread_startup(&sensor_thread);
  }else {
		rt_kprintf("create sensor_thread failed \r\n");   
		return -1;
  }

    #if 1
	/* 初始化线程3 */
	result = rt_thread_init(&lcd_thread, 
							"lcd", 								/* 线程名：lcd */
							lcd_thread_entry, 					/* 线程的入口是sensor_thread_entry，入口参数是2 */
							(void*)4,					 
							&lcd_thread_stack[0], 
							sizeof(lcd_thread_stack), 			/* 线程栈是thread3_stack */
							LCD_THREAD_PRIORITY, 
							LCD_THREAD_TIMESLICE);
	
	if (result == RT_EOK) {									/* 如果返回正确，启动线程3 */ 
		rt_kprintf("create lcd_thread succeed \r\n");      
		rt_thread_startup(&lcd_thread);
	} else {		
		rt_kprintf("create lcd_thread failed \r\n");      
		return -1;
	}
    #endif
    
	result = rt_thread_init(&touch_thread, 
							"touch", 								/* 线程名：lcd */
							touch_thread_entry, 					/* 线程的入口是sensor_thread_entry，入口参数是2 */
							(void*)4,					 
							&touch_thread_stack[0], 
							sizeof(touch_thread_stack), 			/* 线程栈是thread3_stack */
							TOUCH_THREAD_PRIORITY, 
							TOUCH_THREAD_TIMESLICE);
	
	if (result == RT_EOK) {									/* 如果返回正确，启动线程3 */ 
		rt_kprintf("create touch_thread succeed \r\n");      
		rt_thread_startup(&touch_thread);
	} else {		
		rt_kprintf("create touch_thread failed \r\n");      
		return -1;
	}



	return 0;
}



#define GT911DEVICE_NAME  "i2c1"
//#define GT911DEVICE_NAME  "i2c2"
//#define GT911DEVICE_NAME  "i2c3"
//#define GT911DEVICE_NAME  "i2c4"

#define FT6336DEVICE_NAME "i2c1"

//#define GT911

//#define  TEST_SRAM


// touch usage 
/*
   i2c1 is ok
*  touchic_inithard(DEVICE_NAME)
*  touchic_readid()
*  touchic_onepiont_scan()

   uart6 is ok
*  uart_test("uart6");
*/
static void led_thread_entry(void* parameter)
{
#ifdef TEST_SRAM			
		uint32_t sdcount = 0;
		uint32_t ADDR = 0x80E00000;

		for(sdcount = 0;sdcount < 200 ;sdcount++ ) {
		    *(uint32_t *)ADDR = sdcount;
			ADDR += 4;
		}
		ADDR = 0x80E00000;
		for(sdcount = 0;sdcount < 100 ;sdcount++ ) {
			rt_kprintf("%4d",*(uint32_t *)ADDR );
			ADDR += 4;
			if(sdcount % 10 ==  0) {
				rt_kprintf("\n");
			}
		}
		rt_kprintf("\n");
#endif	

	rt_pin_mode(74, PIN_MODE_OUTPUT);

  while (1) 
	{
		//Cgk_test_example();

		rt_thread_delay(1000); 
		// test i2c1 and uart6	
		//i2c_test("i2c1",0xBA);

		//uart_test("uart4");
		rt_kprintf("led_thread address: 0x%p \r\n", led_thread);      
  }
}


static void sensor_thread_entry(void* parameter)
{
	uint8_t adc_ret = 0;
	uint8_t *sentPdat = "test is ok";
	while (1)
	{  
		//_search_i2c_device(dev,0x00);
		//i2c_test("i2c1",0x00);


		//bsp_uart4_sendbyte('A');

	    //bsp_uart4_sendstr(sentPdat,10);
		
		
		//adc_ret = bsp_batadc1_getvolvalue_average(BAT_NXP_ADC_CH,5);
		//rt_kprintf("bat vlaue %d , bat fVoltageValue %d V\r\n",g_AdcVatData.uiOriginalValue,(int)(g_AdcVatData.fVoltageValue*100)); 		
		rt_thread_delay(2000);   /* 延时，单位为tick */  
	}
}

extern void SmartPhomeMenuMainTask(void); 
extern void MainTask(void);
static void lcd_thread_entry(void* parameter)
{	
	//初始化PWM，定时器时钟为：150/64=2.34375Mhz，设置频率为400hz,50%占空比
	bsp_qtmr4ch3_pwminit(14,400,99); 
	bsp_qtmr4ch3_pwmdutyset(14,400,99);
    while (1)
    { 
    	rt_kprintf("lcd_thread address: 0x%p \r\n", &lcd_thread);    
    	SmartPhomeMenuMainTask();
    	//MainTask();

    	rt_thread_delay(1000);   /* 延时，单位为tick */
    }
}


char buffer[4096] = {0};
uint8_t create_file(char *pFilename) 
{	
	  FILINFO finfo;
		UINT    br,bw;			//读写变量
		FRESULT res;
		int i;
		int rsize = 0 , wsize = 0; 
    	
		f_stat("1:/Ping40.xbf", &finfo);
	/*
		res = f_open(&g_fileObject, "1:/Ping40.xbf", FA_CREATE_ALWAYS | FA_WRITE);
		if (res) {
			return 1;
		}		
		res = f_write (&g_fileObject,buffer, sizeof(buffer), &bw);	 
		
		wsize = f_tell(&g_fileObject);
		
	*/
		
    return 0;
}
static void touch_thread_entry(void* parameter)
{	
	uint8_t flash_ret ;

	FT5X06_InitHard();
	bsp_qtimer1_ch0int_init(15,11718);
	
// flash_ret = bsp_test_spiflashwq256();
	
	flash_ret = bsp_test_spiflashwq64();
	

	//rt_thread_delay(1000);
	//create_file("xf") ;
	
  while (1)
  { 	
		//rt_kprintf("lcd_thread address: 0x%p \r\n", &touch_thread);   		

		rt_thread_delay(800);    
  }
}


typedef enum {
    
	KEY_SWICH_ON  = 0,
	KEY_SWICH_OFF = 1,
	KEY_IDLE      = -1,
	
}HARD_KEY_STATE_t;

HARD_KEY_STATE_t HARD_KEY_STATE = KEY_SWICH_ON;

static unsigned char Trgg_sta = 0;  // 短按状态
static unsigned char cntu_sta = 0;  // 长按状态

static void KeyRead( void )
{
    unsigned char ReadData = !(rt_pin_read(POWER_STATUS));

    Trgg_sta = ReadData & (ReadData ^ cntu_sta);      
    cntu_sta = ReadData;                               
}

static rt_timer_t timer_key;
static void timeout_key(void* parameter)
{

	KeyRead();
	
	if(Trgg_sta == 1 && HARD_KEY_STATE == KEY_SWICH_ON)
	{
		HARD_KEY_STATE = KEY_SWICH_OFF;

		rt_pin_write(POWER_COTL, 1);

	}
	else if(Trgg_sta == 1 && HARD_KEY_STATE == KEY_SWICH_OFF)
	{
		HARD_KEY_STATE = KEY_SWICH_ON;

		rt_pin_write(POWER_COTL, 0);

	}
}

static int timer_init(void)
{
	/* ?????1 */
	timer_key = rt_timer_create("timer1", 				/* ?????? timer1 */
								timeout_key, 			/* ?????????? */
								RT_NULL, 				/* ????????? */
								1000, 					/* ????,?OS Tick???,?10?OS Tick */
								RT_TIMER_FLAG_PERIODIC); /* ?????? */
	/* ????? */
	if (timer_key != RT_NULL) 
		rt_timer_start(timer_key);
		return 0;
}

int GUI_recvMsg(char *msgBuf) {
    char buf[16];

    if(msgBuf == NULL)
        return -RT_ERROR;

    rt_memset(&buf[0], 0, sizeof(buf));

    /* 从消息队列中接收消息 */
    if (rt_mq_recv(&mq, &buf[0], sizeof(buf), 5) != RT_EOK) {
        rt_kprintf("thread1: recv msg from message queue, the content:%s\n", buf);
        
        return -RT_ERROR;
    }
    else {
        memmove(msgBuf, buf, sizeof(buf));

        return RT_EOK;
    }
}

int GUI_sendMsg(char *msgBuf) {

    int result;

    if(msgBuf == NULL)
        return -RT_ERROR;

    result = rt_mq_send(&mq, msgBuf, sizeof(msgBuf));
    
    if ( result == -RT_EFULL) {
        /* 消息队列满， 延迟1s时间 */
        rt_kprintf("message queue full, delay 1s\n");
        return -RT_EFULL;
    }
    
    return RT_EOK;
}

int GUI_mutex_take(void) {

    rt_err_t result;

    result = rt_mutex_take(ui_mutex, 1000);
    
    if (result != RT_EOK){
        //tc_stat(TC_STAT_END | TC_STAT_FAILED);

        return -RT_ERROR;
    }

    return RT_EOK;
}

int GUI_mutex_release(void) {

    rt_mutex_release(ui_mutex);

    return RT_EOK;
}

int GUI_Button_mutex_take(void) {

    rt_err_t result;

    result = rt_mutex_take(ui_mutex, 1000);
    
    if (result != RT_EOK){
        //tc_stat(TC_STAT_END | TC_STAT_FAILED);

        return -RT_ERROR;
    }

    return RT_EOK;
}

int GUI_Button_mutex_release(void) {

    rt_mutex_release(ui_mutex);

    return RT_EOK;
}



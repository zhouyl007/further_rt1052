
#include "bsp_board_init.h"
#include "ff.h"


static FATFS g_fileSystem; /* File system object */
uint8_t rt1052_board_user_init(void)
{
	uint8_t board_ret = 0;
	// init i2c1 device

	board_ret = i2c1dev_hard_init("i2c1");
	
	
	board_ret = cgk_inithard();
	if(board_ret != 0) {
	   rt_kprintf("can not find cgk i2c bus .\n");    
	}

// touch 10ms timer init 
  
	bsp_qtimer1_ch0int_init(15,11728);  // 10ms interrupt
	
	bsp_qtimer2_ch0int_init(15,4687);   // 4ms interrupt use to check uart4 recv data
	
    rt_pin_mode(POWER_STATUS, PIN_MODE_INPUT_PULLUP);

	rt_pin_mode(POWER_COTL, PIN_MODE_OUTPUT);
	
	bsp_batadc1_init();
		
	bsp_w25q256_init();
	
	bsp_w25q64_init();	
	
	
	/*
	board_ret = f_mount(&g_fileSystem,"1:",1); 				//����FLASH.	
	if(board_ret==0X0D) {  										//FLASH����,FAT�ļ�ϵͳ����,���¸�ʽ��FLASH
		board_ret = 0;
		board_ret=f_mkfs("1:",1,4096);					//��ʽ��FLASH,1,�̷�;1,����Ҫ������,8������Ϊ1����
		if(board_ret==0) {
			//f_setlabel((const TCHAR *)"1:XF");	  //����Flash���̵�����Ϊ��XF
		}
	}	
	*/
	
	bsp_uart4wifi_init(115200);

	
	return 0;
}





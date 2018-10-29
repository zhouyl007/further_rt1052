#ifndef __PYINPUT_H
#define __PYINPUT_H

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//拼音输入法 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/1/12
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//*******************************************************************************
//修改信息
//无
////////////////////////////////////////////////////////////////////////////////// 	   

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;


typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
//拼音码表与拼音的对应表
typedef struct
{
  u8 *py_input;//输入的字符串
  u8 *py;	   //对应的拼音
  u8 *pymb;	   //码表
}py_index;

#define MAX_MATCH_PYMB 	10	//最大匹配数

//拼音输入法
typedef struct
{
	unsigned char(*getpymb)(unsigned char *instr);			//字符串到码表获取函数
  py_index *pymb[MAX_MATCH_PYMB];	//码表存放位置
}pyinput;

extern pyinput t9;
unsigned char str_match(unsigned char*str1, unsigned char*str2);
unsigned char get_matched_pymb(unsigned char *strin, py_index **matchlist);
unsigned char get_pymb(unsigned char* str);
void test_py(unsigned char *inputstr);
#endif









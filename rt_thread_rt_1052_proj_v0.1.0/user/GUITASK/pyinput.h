#ifndef __PYINPUT_H
#define __PYINPUT_H

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//ƴ�����뷨 ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/1/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//*******************************************************************************
//�޸���Ϣ
//��
////////////////////////////////////////////////////////////////////////////////// 	   

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;


typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
//ƴ�������ƴ���Ķ�Ӧ��
typedef struct
{
  u8 *py_input;//������ַ���
  u8 *py;	   //��Ӧ��ƴ��
  u8 *pymb;	   //���
}py_index;

#define MAX_MATCH_PYMB 	10	//���ƥ����

//ƴ�����뷨
typedef struct
{
	unsigned char(*getpymb)(unsigned char *instr);			//�ַ���������ȡ����
  py_index *pymb[MAX_MATCH_PYMB];	//�����λ��
}pyinput;

extern pyinput t9;
unsigned char str_match(unsigned char*str1, unsigned char*str2);
unsigned char get_matched_pymb(unsigned char *strin, py_index **matchlist);
unsigned char get_pymb(unsigned char* str);
void test_py(unsigned char *inputstr);
#endif









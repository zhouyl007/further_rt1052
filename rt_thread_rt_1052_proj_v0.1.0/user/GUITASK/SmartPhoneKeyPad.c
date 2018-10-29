#include <stddef.h>
#include "DIALOG.h"
#include "keypad.h"

#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "pyinput.h"

#include "WM.h"

#include "public_h.h"

extern GUI_CONST_STORAGE GUI_FONT GUI_FontPingfang34;
extern GUI_CONST_STORAGE GUI_BITMAP bmsogoupinyin;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontPingFangRegular60;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontPingfang34;

extern GUI_CONST_STORAGE GUI_FONT GUI_FontSong2K60;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontydinsertfont40;

extern GUI_CONST_STORAGE GUI_BITMAP bmyesbutton;
extern GUI_CONST_STORAGE GUI_BITMAP bmcancelbutton;

WM_HWIN hMulti = 0;       //?àDD??±?
KEYPADStructTypeDef keypad_dev;


static int _DrawSkinFlex_BUTTON(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo);
static int _DrawChineseSkin_BUTTON(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo);

typedef struct
{
	int          xPos;              //°′?￥X????
	int          yPos;              //°′?￥Y????
	int          xSize;             //°′?￥?í?è
	int          ySize;             //°′?￥???è
	const char * acLabel;           //°′?￥?ù′ú±íμ?×?·?
	void(*pfDraw)(WM_HWIN hWin);  //°′?￥μ?????oˉêy
	const char * ButString;         //°′?￥é???ê?μ?×?·?
} BUTTON_DATA;


//?ú°′?￥é???????í?￡?±èè???3μ?ü￡?é?3y?üμè
static void _DrawCentered(WM_HWIN hWin, const GUI_BITMAP * pBM)
{
	int xSizeWin;
	int ySizeWin;
	int xSizeBMP;
	int ySizeBMP;
	int xPos;
	int yPos;

	xSizeWin = WM_GetWindowSizeX(hWin);
	ySizeWin = WM_GetWindowSizeY(hWin);
	xSizeBMP = pBM->XSize;
	ySizeBMP = pBM->YSize;
	xPos = (xSizeWin - xSizeBMP) >> 1;
	yPos = (ySizeWin - ySizeBMP) >> 1;
	GUI_DrawBitmap(pBM, xPos, yPos);
}

static void _DrawBkSpc(WM_HWIN hWin) { _DrawCentered(hWin, &bmBackSpace); }	//????í????ü(é?3y?ü)
static void _DrawEnter(WM_HWIN hWin) { _DrawCentered(hWin, &bmEnter); }		//??????3μ?ü
static void _DrawShift(WM_HWIN hWin) { _DrawCentered(hWin, &bmShift); }		//????shift?ü
static void _DrawRetern(WM_HWIN hWin) { _DrawCentered(hWin, &bmReturn); }		//????·μ???ü



static  BUTTON_DATA _aEngButtonData[] = {
		{ 12, 16, 62, 96, "Q", 0, "Q" },
		{ 83, 16, 62, 96, "W", 0, "W" },
		{ 153, 16, 62, 96, "E", 0, "E" },
		{ 224, 16, 62, 96, "R", 0, "R" },
		{ 295, 16, 62, 96, "T", 0, "T" },
		{ 365, 16, 62, 96, "Y", 0, "Y" },
		{ 436, 16, 62, 96, "U", 0, "U" },
		{ 507, 16, 62, 96, "I", 0, "I" },
		{ 577, 16, 62, 96, "O", 0, "O" },
		{ 648, 16, 62, 96, "P", 0, "P" },

		{ 47, 130, 62, 96, "A", 0, "A" },
		{ 117, 130, 62, 96, "S", 0, "S" },
		{ 188, 130, 62, 96, "D", 0, "D" },
		{ 259, 130, 62, 96, "F", 0, "F" },
		{ 329, 130, 62, 96, "G", 0, "G" },
		{ 400, 130, 62, 96, "H", 0, "H" },
		{ 471, 130, 62, 96, "J", 0, "J" },
		{ 542, 130, 62, 96, "K", 0, "K" },
		{ 612, 130, 62, 96, "L", 0, "L" },

		{ 12, 244, 96, 96, "\x19", _DrawShift },
		{ 117, 244, 62, 96, "Z", 0, "Z" },
		{ 188, 244, 62, 96, "X", 0, "X" },
		{ 259, 244, 62, 96, "C", 0, "C" },
		{ 329, 244, 62, 96, "V", 0, "V" },
		{ 400, 244, 62, 96, "B", 0, "B" },
		{ 471, 244, 62, 96, "N", 0, "N" },
		{ 542, 244, 62, 96, "M", 0, "M" },
		{ 612, 244, 96, 96, "\x08", _DrawBkSpc },

		{ 12, 358, 96, 96, "A", 0, "\xE7\xAC\xA6" },
		{ 117, 358, 62, 96, "Z", 0, "\xE6\x95\xB0" },
		{ 202, 358, 62, 96, "X", 0, "," },
		{ 272, 358, 176, 96, "C", 0, "C" },
		{ 457, 358, 62, 96, "V", 0, "." },
		{ 527, 358, 76, 96, "B", 0, "\xE8\x8B\xB1" },
		{ 612, 358, 96, 96, "N", 0, "N" },
};

static  BUTTON_DATA _aNumButtonData[] = {
		{ 9, 12, 106, 107, "#", 0, "#" },
		{ 126, 12, 152, 107, "1", 0, "1" },
		{ 286, 12, 152, 107, "2", 0, "2" },
		{ 445, 12, 152, 107, "3", 0, "3" },
		{ 607, 12, 110, 107, "X", 0, "X" },

		{ 9, 129, 106, 107, "-", 0, "-" },
		{ 126, 129, 152, 107, "4", 0, "4" },
		{ 286, 129, 152, 107, "5", 0, "5" },
		{ 445, 129, 152, 107, "6", 0, "6" },
		{ 607, 129, 110, 107, ".", 0, "." },


		{ 9, 244, 106, 107, "+", 0, "+" },
		{ 126, 244, 152, 107, "7", 0, "7" },
		{ 286, 244, 152, 107, "8", 0, "8" },
		{ 445, 244, 152, 107, "8", 0, "9" },
		{ 607, 244, 110, 107, "@", 0, "@" },


		{ 9, 360, 106, 100, "A", 0, "A" },
		{ 126, 360, 152, 100, "B", 0, "B" },
		{ 286, 360, 152, 100, "0", 0, "0" },
		{ 445, 360, 152, 100, "D", 0, "D" },
		{ 607, 360, 110, 100, "E", 0, "E" },
};

static  BUTTON_DATA _aSinButtonData[] = {
		{ 9, 12, 106, 107, "?", 0, "?" },
		{ 126, 12, 152, 107, "%", 0, "%" },
		{ 286, 12, 152, 107, "@", 0, "@" },
		{ 445, 12, 152, 107, "_", 0, "_" },
		{ 607, 12, 110, 107, "X", 0, "X" },

		{ 9, 129, 106, 107, "+", 0, "+" },
		{ 126, 129, 152, 107, "-", 0, "-" },
		{ 286, 129, 152, 107, ":", 0, ":" },
		{ 445, 129, 152, 107, ",", 0, "," },
		{ 607, 129, 110, 107, ".", 0, "." },


		{ 9, 244, 106, 107, "/", 0, "/" },
		{ 126, 244, 152, 107, "!", 0, "!" },
		{ 286, 244, 152, 107, "&", 0, "&" },
		{ 445, 244, 152, 107, "*", 0, "*" },
		{ 607, 244, 110, 107, "$", 0, "$" },


		{ 9, 360, 106, 100, "(", 0, "(" },
		{ 126, 360, 152, 100, ")", 0, ")" },
		{ 286, 360, 152, 100, "^", 0, "^" },
		{ 445, 360, 152, 100, "D", 0, "D" },
		{ 607, 360, 110, 100, "E", 0, "E" },
};

//中文键盘1位置
static  BUTTON_DATA _aChineseButtonData1[] = {
		{ 124, 104, 152, 107, "1", 0, "1" },
		{ 284, 104, 152, 107, "2", 0, "ABC" },
		{ 443, 104, 152, 107, "3", 0, "DEF" },
		{ 605, 104, 110, 107, "\x08", _DrawBkSpc },

		{ 124, 219, 152, 107, "4", 0, "GHI" },
		{ 284, 219, 152, 107, "5", 0, "JKL" },
		{ 443, 219, 152, 107, "6", 0, "MNO" },
		{ 605, 219, 110, 107, " ", 0, "└┘" },

		{ 124, 335, 152, 107, "7", 0, "PQRS" },
		{ 284, 335, 152, 107, "8", 0, "TUV" },
		{ 443, 335, 152, 107, "9", 0, "WXYZ" },
		{ 605, 335, 110, 107, "\x04", _DrawRetern },
};

//?D???ü?ì2????
static BUTTON_DATA _aChineseButtonData2[] = {
		{ 0, 0, 90, 90, "", 0, "<" },
		{ 90, 0, 90, 90, "2", 0, " " },
		{ 180, 0, 90, 90, "3", 0, " " },
		{ 270, 0, 90, 90, "4", 0, " " },
		{ 360, 0, 90, 90, "5", 0, " " },
		{ 450, 0, 90, 90, "6", 0, " " },
		{ 540, 0, 90, 90, "7", 0, " " },
		{ 630, 0, 90, 90, "", 0, ">" },

		{ 0, 90, 115, 62, "\x05", 0, "^" },
		{ 0, 90 + 62, 115, 62, "B", 0, " " },
		{ 0, 90 + 62 * 2, 115, 62, "C", 0, " " },
		{ 0, 90 + 62 * 3, 115, 62, "D", 0, " " },
		{ 0, 90 + 62 * 4, 115, 62, "E", 0, " " },
		{ 0, 90 + 62 * 5, 115, 62, "\x06", 0, "V" },
};


//大小写切换处理
void changecase(u8 change_flag, BUTTON_DATA *buttondata)
{
	uint16_t i;
	char NumToStr;
	char buffer[5];
	if (keypad_dev.LowerCase)		//小写,按钮修改为小写
	{
		//更改按钮的字符
		for (i = 0; i < 27; i++)
		{
			if (i == 19) continue;
			NumToStr = (char)((int)*(buttondata[i].acLabel) + 0x20);
			sprintf(buffer, "%c", NumToStr);              //转换为字符
			BUTTON_SetFont(keypad_dev.EngButton_Handle[i], &GUI_FontPingFangRegular60);
			BUTTON_SetText(keypad_dev.EngButton_Handle[i], buffer);
			BUTTON_SetFocussable(keypad_dev.EngButton_Handle[i], 0);
		}
	}
	else 	//大写，按钮上的字符改为大写
	{
		//更改按钮的字符
		for (i = 0; i < 27; i++)
		{
			if (i == 19) continue;
			BUTTON_SetFont(keypad_dev.EngButton_Handle[i], &GUI_FontPingFangRegular60);
			BUTTON_SetText(keypad_dev.EngButton_Handle[i], buttondata[i].acLabel);
			BUTTON_SetFocussable(keypad_dev.EngButton_Handle[i], 0);
		}
	}
}


//绘制英文键盘
//hWin:父窗口
void drawenglish_pad(WM_HWIN hWin)
{
	uint16_t i;

	//设置BUTTON的皮肤	
	//BUTTON_SetDefaultSkin(_DrawSkinFlex_BUTTON);
	//设置BUTTON的皮肤	
	for (i = 0; i < GUI_COUNTOF(_aEngButtonData); i++)
	{
		// 创建按钮
		keypad_dev.EngButton_Handle[i] = BUTTON_CreateEx(_aEngButtonData[i].xPos, _aEngButtonData[i].yPos, _aEngButtonData[i].xSize, _aEngButtonData[i].ySize,
			hWin, WM_CF_SHOW | WM_CF_HASTRANS, 0, ID_BUTTON + i);           // ID_BUTTON = (GUI_ID_USER + 0) = 0x800		
		BUTTON_SetFont(keypad_dev.EngButton_Handle[i], &GUI_FontPingFangRegular60);
		// 设置按钮显示内容
		BUTTON_SetText(keypad_dev.EngButton_Handle[i], _aEngButtonData[i].ButString);
		// 设置按钮焦点
		BUTTON_SetFocussable(keypad_dev.EngButton_Handle[i], 0);
	}
	changecase(keypad_dev.LowerCase, _aEngButtonData);
}

//????êy×??ü?ì
void drawnumber_pad(WM_HWIN hWin)
{
	u16 i;
	//BUTTON_SetDefaultSkin(_DrawSkinFlex_BUTTON); //éè??BUTTONμ??¤·?
	for (i = 0; i<GUI_COUNTOF(_aNumButtonData); i++)								//′′?¨êy×??ü?ì
	{
		keypad_dev.NumButton_Handle[i] = BUTTON_CreateEx(_aNumButtonData[i].xPos, _aNumButtonData[i].yPos, _aNumButtonData[i].xSize, _aNumButtonData[i].ySize,
			hWin, WM_CF_SHOW | WM_CF_HASTRANS, 0, ID_BUTTON + i);
		BUTTON_SetFont(keypad_dev.NumButton_Handle[i], &GUI_FontPingFangRegular60);
		BUTTON_SetText(keypad_dev.NumButton_Handle[i], _aNumButtonData[i].ButString);
		BUTTON_SetFocussable(keypad_dev.NumButton_Handle[i], 0);
	}
}

//????·?o??ü?ì
void drawsign_pad(WM_HWIN hWin)
{
	u16 i;
	//BUTTON_SetDefaultSkin(_DrawSkinFlex_BUTTON); //éè??BUTTONμ??¤·?
	for (i = 0; i<GUI_COUNTOF(_aSinButtonData); i++)								//′′?¨êy×??ü?ì
	{		
		keypad_dev.SignButton_Handle[i] = BUTTON_CreateEx(_aSinButtonData[i].xPos, _aSinButtonData[i].yPos, _aSinButtonData[i].xSize, _aSinButtonData[i].ySize,
			hWin, WM_CF_SHOW | WM_CF_HASTRANS, 0, ID_BUTTON + i);
		BUTTON_SetFont(keypad_dev.SignButton_Handle[i], &GUI_FontPingFangRegular60);
		BUTTON_SetText(keypad_dev.SignButton_Handle[i], _aSinButtonData[i].ButString);
		BUTTON_SetFocussable(keypad_dev.SignButton_Handle[i], 0);
	}
}

//?????D???ü?ì1
void drawchinese1_pad(WM_HWIN hWin)
{
	u16 i;
	//BUTTON_SetDefaultSkin(_DrawSkinFlex_BUTTON); //éè??BUTTONμ??¤·?
	for (i = 0; i<GUI_COUNTOF(_aChineseButtonData1); i++)					//′′?¨?D???ü?ì1
	{
		keypad_dev.ChineseButton_Handle[i] = BUTTON_CreateEx(_aChineseButtonData1[i].xPos, _aChineseButtonData1[i].yPos, _aChineseButtonData1[i].xSize, _aChineseButtonData1[i].ySize,
			hWin, WM_CF_SHOW | WM_CF_HASTRANS, 0, ID_BUTTON + i);
		BUTTON_SetFont(keypad_dev.ChineseButton_Handle[i], &GUI_FontPingFangRegular60);
		BUTTON_SetText(keypad_dev.ChineseButton_Handle[i], _aChineseButtonData1[i].ButString);
		BUTTON_SetFocussable(keypad_dev.ChineseButton_Handle[i], 0);
	}
}

void drawchinese2_pad(WM_HWIN hWin)
{
	u16 i;
	//BUTTON_SetDefaultSkin(_DrawChineseSkin_BUTTON); //éè??BUTTONμ??¤·?
	for (i = 0; i<GUI_COUNTOF(_aChineseButtonData2); i++)					//′′?¨?D???ü?ì2
	{	
		keypad_dev.ChineseButton_Handle[i + 12] = BUTTON_CreateEx(_aChineseButtonData2[i].xPos, _aChineseButtonData2[i].yPos, _aChineseButtonData2[i].xSize, _aChineseButtonData2[i].ySize,
			hWin, WM_CF_SHOW | WM_CF_HASTRANS, 0, ID_BUTTON + i + 12);
		BUTTON_SetFont(keypad_dev.ChineseButton_Handle[i + 12], &GUI_FontPingFangRegular60);
		BUTTON_SetText(keypad_dev.ChineseButton_Handle[i + 12], _aChineseButtonData2[i].ButString);
		BUTTON_SetFocussable(keypad_dev.ChineseButton_Handle[i + 20], 0);
	}
}
//英文键盘处理函数
//buttondata:英文键盘按钮参数
//Id:按钮ID
//*pMsg:指向消息
void engkeypad_process(BUTTON_DATA *buttondata, int Id, WM_MESSAGE *pMsg)
{
	char 		c;
	WM_MESSAGE 	Msg;
	u8          KeyID;

	KeyID = Id - ID_BUTTON;   // 获取按键 id 值
	if (buttondata[Id - ID_BUTTON].acLabel)         // 按下的按键是有效的
	{
		c = buttondata[Id - ID_BUTTON].acLabel[0];
		if (KeyID == 19)                                   //大小写切换，对应的按键 iD 值为19
		{
			keypad_dev.LowerCase ^= 1;		               //对LowerCase进行异或操作改变大小写的状态
			changecase(keypad_dev.LowerCase, buttondata);  //刷新大小写显示
		}
		else  if (KeyID == 29)                             //切换到数字键盘
		{
			keypad_dev.padtype = NUMBER_KEYPAD;		       //数字键盘
			Msg.MsgId = CHANGE_ENGLISHPADTO_NUMPAD;        //设置消息值
			Msg.hWinSrc = pMsg->hWin;
			WM_SendMessage(keypad_dev.hKeypad, &Msg);      //发送消息到 keypad
		}
		else if (KeyID == 28)                              //切换到符号键盘
		{
			keypad_dev.padtype = SIGN_KEYPAD;
			Msg.MsgId = CHANGE_ENGLISHPADTO_SIGNPAD1;
			Msg.hWinSrc = pMsg->hWin;
			WM_SendMessage(keypad_dev.hKeypad, &Msg);
		}
		else if (KeyID == 33)                              //切换到中文键盘
		{
			keypad_dev.padtype = CHINESE_KEYPAD;
			Msg.MsgId = CHANGE_ENGLISHPADTO_CHINESEPAD;
			Msg.hWinSrc = pMsg->hWin;
			WM_SendMessage(keypad_dev.hKeypad, &Msg);
		}
		else  // 除此之外的按键按下，在缓冲器中显示字符
		{
			if (keypad_dev.LowerCase && (c >= 0X41) && (c <= 0X5A)) //小写
			{
				c += 0x20;
			}
			GUI_StoreKeyMsg(c, 1);	//把消息存进键盘缓冲器，按下状态
			GUI_StoreKeyMsg(c, 0);	//把消息存进键盘缓冲器，松开状态
		}
	}
}

//数字键盘处理函数
//buttondata:数字键盘按钮参数
//Id:按钮ID
//*pMsg:指向消息
void numkeypad_process(BUTTON_DATA *buttondata, int Id, WM_MESSAGE *pMsg)
{
	char 		c;
	WM_MESSAGE 	Msg;
	u8          KeyID;

	KeyID = Id - ID_BUTTON;
	if (buttondata[Id - ID_BUTTON].acLabel)
	{
		c = buttondata[Id - ID_BUTTON].acLabel[0];

		if (KeyID == 19)                               //返回英文键盘
		{
			keypad_dev.padtype = ENGLISH_KEYPAD;		//英文键盘
			Msg.MsgId = CHANGE_NUMPADTO_ENGLISHPAD;
			Msg.hWinSrc = pMsg->hWin;
			WM_SendMessage(keypad_dev.hKeypad, &Msg);
		}
		else
		{
			GUI_StoreKeyMsg(c, 1);				//把消息存进键盘缓冲器，按下状态
			GUI_StoreKeyMsg(c, 0);				//把消息存进键盘缓冲器，松开状态 
		}
	}
}

//符号键盘处理函数
//buttondata:数字键盘按钮参数
//Id:按钮ID
//*pMsg:指向消息
void signkeypad_process(BUTTON_DATA *buttondata, int Id, WM_MESSAGE *pMsg)
{
	char 		c;
	WM_MESSAGE 	Msg;
	u8          KeyID;

	KeyID = Id - ID_BUTTON;
	if (buttondata[Id - ID_BUTTON].acLabel)
	{
		c = buttondata[Id - ID_BUTTON].acLabel[0];
		if (KeyID == 19)                             //返回英文键盘
		{
			keypad_dev.padtype = ENGLISH_KEYPAD;		//ó￠???ü?ì
			Msg.MsgId = CHANGE_SIGNPADTO_ENGLISHPAD;
			Msg.hWinSrc = pMsg->hWin;
			WM_SendMessage(keypad_dev.hKeypad, &Msg);
		}
		else
		{
			GUI_StoreKeyMsg(c, 1);				//把消息存进键盘缓冲器，按下状态
			GUI_StoreKeyMsg(c, 0);				//把消息存进键盘缓冲器，松开状态 
		}
	}
}


//中文处理函数
//buttondata:数字键盘按钮参数
//Id:按钮ID
//*pMsg:指向消息
void chinesekeypad_process(BUTTON_DATA *buttondata, int Id, WM_MESSAGE *pMsg)
{
	char 		c;
	WM_MESSAGE 	Msg;
	u8          key;
	u8          KeyID;
	u8          TempID;
	u8          i = 0;

	KeyID = Id - ID_BUTTON;
	if (buttondata[Id - ID_BUTTON].acLabel)
	{
		c = buttondata[Id - ID_BUTTON].acLabel[0];
		if (KeyID == 3)                               //删除键
		{
			keypad_dev.cur_index = 1;                 //当删除输入的拼音的时候，强制让索引为第一个
			if (keypad_dev.inputlen)
			{
				keypad_dev.inputlen--;
				keypad_dev.inputstr[keypad_dev.inputlen] = '\0';  //添加结束符
			}
			else       //没有字符串输入
			{
				GUI_StoreKeyMsg(c, 1);				//把消息存进键盘缓冲器，按下状态
				GUI_StoreKeyMsg(c, 0);				//把消息存进键盘缓冲器，松开状态
			}
		}
		else if (KeyID == 11)                       //英文键盘
		{
			keypad_dev.padtype = ENGLISH_KEYPAD;		//英文键盘
			Msg.MsgId = CHANGE_CHINESEPADTO_ENGLISHPAD;
			Msg.hWinSrc = pMsg->hWin;
			WM_SendMessage(keypad_dev.hKeypad, &Msg);
		}
		else if (KeyID == 20)                        //拼音上一页
		{
			if (keypad_dev.pypagenum>0)
			{
				if (keypad_dev.pynowpage>0)keypad_dev.pynowpage--;
				else keypad_dev.pynowpage = 0;
			}
		}
		else  if (KeyID == 25)                      //拼音下一页
		{
			if (keypad_dev.pypagenum>0)
			{
				if (keypad_dev.pynowpage<(keypad_dev.pypagenum - 1)) keypad_dev.pynowpage++;
				else keypad_dev.pynowpage = keypad_dev.pypagenum - 1;
			}
		}
		else if (KeyID == 12)                        //汉字上一页         
		{
			if (keypad_dev.dispagenum>0)
			{
				if (keypad_dev.dispagecur>0) keypad_dev.dispagecur--;
				else keypad_dev.dispagecur = 0;
			}
		}
		else if (KeyID == 19)                       //汉字下一页
		{
			if (keypad_dev.dispagenum>0)
			{
				if (keypad_dev.dispagecur<(keypad_dev.dispagenum - 1)) keypad_dev.dispagecur++;
				else keypad_dev.dispagecur = keypad_dev.dispagenum - 1;
			}
		}
		else if ((KeyID > 12) && (KeyID < 19))      //横排
		{
			TempID = KeyID - 12;
			TempID = TempID + 6 * keypad_dev.dispagecur;  //得到当前的汉字索引
			if (TempID <= keypad_dev.hznum)
			{
				keypad_dev.cur_hz = keypad_dev.butname[(KeyID - 12) - 1]; //获得选中的汉字
				if (keypad_dev.cur_hz != NULL)
				{
					MULTIEDIT_AddText(hMulti, (const char*)keypad_dev.cur_hz);    //在多行文本中显示选中的汉字
				}
				keypad_dev.result_num = 0;
				keypad_dev.dispagecur = 0;
				keypad_dev.dispagenum = 0;
				keypad_dev.cur_index = 0;
				keypad_dev.pynowpage = 0;
				keypad_dev.pypagenum = 0;
				keypad_dev.inputlen = 0;
				for (i = 0; i<7; i++)    keypad_dev.inputstr[i] = 0;  //输入清零
			}

		}
		else if ((KeyID > 20) && (KeyID <25))       //竖排
		{
			TempID = KeyID - 20;
			TempID = TempID + 3 * keypad_dev.pynowpage; //得到当前的拼音索引
			if (TempID <= keypad_dev.result_num)
			{
				keypad_dev.cur_index = TempID;
				keypad_dev.dispagecur = 0;
				keypad_dev.dispagenum = 0;
			}
		}
		else
		{
			if ((c == ';') || (c == ' '))
			{
				GUI_StoreKeyMsg(c, 1);				    //把消息存进键盘缓冲器，按下状态
				GUI_StoreKeyMsg(c, 0);				    //把消息存进键盘缓冲器，松开状态
			}
			else
			{
				keypad_dev.inputstr[keypad_dev.inputlen] = c + '\0';   //输入字符
				if (keypad_dev.inputlen<6)
					keypad_dev.inputlen++;
			}

		}
		if ((keypad_dev.inputstr[0]) != NULL)
		{
			key = t9.getpymb(keypad_dev.inputstr);          //得到匹配的结果数  
			if (key)                                        //有部分匹配/完全匹配的结果
			{
				keypad_dev.result_num = key & 0X7F;         //总匹配结果
				if ((keypad_dev.sta & 0X80) == 0)
				{
					keypad_dev.sta |= 0X80;
					keypad_dev.cur_index = 1;               //默认第一个索引
				}
				if (key & 0x80)                            //部分匹配
				{
					keypad_dev.inputlen = key & 0x7f;      //有效匹配位数
					keypad_dev.inputstr[keypad_dev.inputlen] = '\0';  //不匹配的位数去掉
					if (keypad_dev.inputlen>1)keypad_dev.result_num = t9.getpymb(keypad_dev.inputstr);//重新获取完全匹配字符数
				}
			}
			else
			{
				keypad_dev.inputlen--;
				keypad_dev.inputstr[keypad_dev.inputlen] = '\0';
			}
		}
		else
		{
			keypad_dev.sta &= 0x7f;
			keypad_dev.cur_index = 0;             //清零
			keypad_dev.result_num = 0;
			keypad_dev.dispagecur = 0;
			keypad_dev.dispagenum = 0;
			keypad_dev.pynowpage = 0;
			keypad_dev.pypagenum = 0;
		}
		if (keypad_dev.result_num & 0x80) keypad_dev.result_num = 1;
		Msg.MsgId = CHANGE_CHINESE_PY;
		Msg.hWinSrc = pMsg->hWin;
		WM_SendMessage(keypad_dev.hKeypad, &Msg);
	}
}


//éè??BUTTONòaê1ó?μ??¤·?
static int _DrawSkinFlex_BUTTON(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo)
{
	static GUI_MEMDEV_Handle ahMemEdges[4];
	GUI_MEMDEV_Handle        hMem;
	WM_HWIN                  hWin;
	unsigned                 i;
	int                      xPos;
	int                      yPos;
	int                      xSize;
	int                      ySize;
	int                      x;
	int                      y;
	int                      Id;
	int                      IsPressed;
	int                      butheigh;
	void(*pfDraw)(WM_HWIN hWin);

	x = 0;
	y = 0;
	switch (pDrawItemInfo->Cmd)
	{
	case WIDGET_ITEM_CREATE:
		if (ahMemEdges[0] == 0)
		{
			//创建一个小的帮助窗口，但是不可见
			hWin = WM_CreateWindowAsChild(0, 0, 16, 16, WM_HBKWIN, WM_CF_SHOW | WM_CF_STAYONTOP, NULL, 0);
			WM_SelectWindow(hWin);
			//为button的每个边缘创建存储设备
			for (i = 0; i < GUI_COUNTOF(ahMemEdges); i++)
			{
				switch (i)
				{
				case 0:
					x = 0;
					y = 0;
					break;
				case 1:
					x = -16;
					y = 0;
					break;
				case 2:
					x = -16;
					y = -16;
					break;
				case 3:
					x = 0;
					y = -16;
					break;
				}
				//创建放大设备
				hMem = GUI_MEMDEV_CreateFixed(0, 0, 16, 16, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUI_COLOR_CONV_8888);
				GUI_MEMDEV_Select(hMem);
				GUI_SetBkColor(GUI_TRANSPARENT);
				GUI_Clear();
				GUI_SetColor(COLOR_BORDER);
				//绘制换肤后的按钮四角的圆弧
				//GUI_DrawRoundedFrame(x, y, x + 62, y + 96, 16, 4);	

				//收缩到指定的大小
				ahMemEdges[i] = GUI_MEMDEV_CreateFixed(0, 0, 4, 4, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUI_COLOR_CONV_8888);
				GUI_MEMDEV_Select(ahMemEdges[i]);
				GUI_SetBkColor(GUI_TRANSPARENT);
				GUI_Clear();
				GUI_MEMDEV_RotateHQ(hMem, ahMemEdges[i], -6, -6, 0, 250);
				GUI_MEMDEV_Delete(hMem);
				GUI_MEMDEV_Select(0);
			}
			WM_DeleteWindow(hWin);
		}

		break;
	case WIDGET_ITEM_DRAW_TEXT:
		//在按钮上显示自定义或者默认的颜色或者文本		
		hWin = pDrawItemInfo->hWin;
		Id = WM_GetId(hWin);
		i = Id - ID_BUTTON;
		if (keypad_dev.padtype == ENGLISH_KEYPAD) pfDraw = _aEngButtonData[i].pfDraw;
		else if (keypad_dev.padtype == NUMBER_KEYPAD) pfDraw = _aNumButtonData[i].pfDraw;
		else if (keypad_dev.padtype == SIGN_KEYPAD) pfDraw = _aSinButtonData[i].pfDraw;
		else if (keypad_dev.padtype == CHINESE_KEYPAD) pfDraw = _aChineseButtonData1[i].pfDraw;

		if (pfDraw)
			pfDraw(hWin);
		else
			BUTTON_DrawSkinFlex(pDrawItemInfo);
		break;

	case WIDGET_ITEM_DRAW_BACKGROUND:
		//绘制按钮的背景
		IsPressed = BUTTON_IsPressed(pDrawItemInfo->hWin); 	//判断是否按钮是否被按下
		xPos = WM_GetWindowOrgX(pDrawItemInfo->hWin);	    //获取按钮小工具的X方向原点位置
		yPos = WM_GetWindowOrgY(pDrawItemInfo->hWin);	    //获取按钮小工具的Y方向原点位置
		xSize = WM_GetWindowSizeX(pDrawItemInfo->hWin);	    //获取按钮小工具的X大小
		ySize = WM_GetWindowSizeY(pDrawItemInfo->hWin);	    //获取按钮小工具的Y大小
		if (IsPressed) 										//如果按钮被按下
		{
			//GUI_DrawGradientRoundedV(0, 0, xSize - 1, ySize - 1, 4, BUTTON_COLOR2, BUTTON_COLOR3);//????′1?±??é?ìY?èì?3?μ??2????D?
			GUI_SetColor(BUTTON_COLOR2);
			GUI_FillRoundedRect(0, 0, xSize - 2, ySize - 2, 10);
		}
		else
		{
			//GUI_DrawGradientRoundedV(0, 0, xSize - 1, ySize - 1, 4, BUTTON_COLOR0, BUTTON_COLOR1);//????′1?±??é?ìY?èì?3?μ??2????D?
			GUI_SetColor(BUTTON_COLOR0);
			GUI_FillRoundedRect(0, 0, xSize - 1, ySize - 1, 10);
		}
		GUI_SetColor(COLOR_BORDER);
		//绘制按钮的外框
		GUI_AA_DrawRoundedRect(0, 0, xSize - 2, ySize - 2, 10);
		//GUI_AA_DrawRoundedRect(1, 1, xSize - 3, ySize - 3, 10);		
		break;
	default:
		return BUTTON_DrawSkinFlex(pDrawItemInfo); //其他命令就使用默认的皮肤
	}
	return 0;
}

//设置中文键盘要使用的皮肤
static int _DrawChineseSkin_BUTTON(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo)
{
	WM_HWIN                  hWin;
	unsigned                 i;
	int                      xSize;
	int                      ySize;
	int                      Id;
	int                      IsPressed;
	void(*pfDraw)(WM_HWIN hWin);

	switch (pDrawItemInfo->Cmd)
	{
	case WIDGET_ITEM_DRAW_TEXT:
		//?ú°′?￥é???ê?×??¨ò??ò????è?μ???é??ò????±?
		hWin = pDrawItemInfo->hWin;
		Id = WM_GetId(hWin);
		i = Id - ID_BUTTON;
		pfDraw = _aChineseButtonData1[i].pfDraw;

		if (pfDraw) pfDraw(hWin);
		else BUTTON_DrawSkinFlex(pDrawItemInfo);
		break;
	case WIDGET_ITEM_DRAW_BACKGROUND:
		//????°′?￥μ?±3?°
		IsPressed = BUTTON_IsPressed(pDrawItemInfo->hWin); 	//?D??ê?·?°′?￥ê?·?±?°′??
		xSize = WM_GetWindowSizeX(pDrawItemInfo->hWin);	    //??è?°′?￥D?1¤??μ?X′óD?
		ySize = WM_GetWindowSizeY(pDrawItemInfo->hWin);	    //??è?°′?￥D?1¤??μ?Y′óD?
		if (IsPressed) 										//è?1?°′?￥±?°′??
		{
			GUI_DrawGradientV(0, 0, xSize - 1, ySize - 1, BUTTON_COLOR2, BUTTON_COLOR3);
		}
		else
		{
			GUI_DrawGradientV(0, 0, xSize - 1, ySize - 1, GUI_WHITE, GUI_WHITE);
		}
		GUI_SetColor(COLOR_BORDER);
		//????°′?￥μ???ì?±???
		GUI_DrawHLine(0, 0, xSize - 1);
		GUI_DrawHLine(ySize - 1, 0, xSize - 1);
		GUI_DrawVLine(0, 0, ySize - 1);
		GUI_DrawVLine(xSize - 1, 0, ySize - 1);
		break;
	default:
		return BUTTON_DrawSkinFlex(pDrawItemInfo);//?????üá??íê1ó???è?μ??¤·?
	}
	return 0;
}


//更新横向的6个按钮
void keypad_hbtn_updc(u8* str)
{
	u8 i = 0;
	if ((keypad_dev.dispagecur >= keypad_dev.dispagenum) && keypad_dev.dispagenum)
		return;  //当前页等于或者大于总页数
	for (i = 0; i<6; i++) 
		keypad_dev.butname[i] = NULL;
	i = 0;
	for (i = 0; i<6; i++)
	{
		if ((i + 6 * (keypad_dev.dispagecur))>(keypad_dev.hznum - 1)) //超出相应拼音的汉字字数
		{
			keypad_dev.disbuf[i][0] = ' ';
			keypad_dev.disbuf[i][1] = ' ';
			keypad_dev.disbuf[i][2] = ' ';
			keypad_dev.disbuf[i][3] = '\0';
			keypad_dev.butname[i] = keypad_dev.disbuf[i];
		}
		else
		{
			keypad_dev.disbuf[i][0] = *str++;
			keypad_dev.disbuf[i][1] = *str++;
			keypad_dev.disbuf[i][2] = *str++;
			keypad_dev.disbuf[i][3] = '\0';
			keypad_dev.butname[i] = keypad_dev.disbuf[i];
		}
	}
}

//绘制横向的6个按钮
void keypad_draw_hbtn(WM_HWIN *handle)
{
	u8 i;
	for (i = 0; i<6; i++)
	{
		BUTTON_SetFont(handle[13 + i], &GUI_Fontydinsertfont40);
		BUTTON_SetText(handle[13 + i], (char*)keypad_dev.butname[i]);
	}
}


static void _cbKeyPad(WM_MESSAGE * pMsg)
{
	WM_HWIN    hWin;
	int        Id;
	int        NCode;
	int        xSize;
	int        ySize;
	u16        slen;
	unsigned   i;

	hWin = pMsg->hWin;
	switch (pMsg->MsgId)
	{
	case WM_CREATE:
		drawenglish_pad(hWin);		//此消息在窗口创建后立即发送，是窗口可以初始化并创建任何子窗口 ,默认起始是英文键盘
		break;
	case WM_PAINT:

		GUI_SetBkColor(0xDEDEDE);       // bk color	
		GUI_Clear();

		break;

	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (NCode)
		{
		case WM_NOTIFICATION_RELEASED:	 //按钮被释放
			if (keypad_dev.padtype == ENGLISH_KEYPAD)	            //英文键盘
			{
				engkeypad_process(_aEngButtonData, Id, pMsg);		//处理英文键盘
			}
			else if (keypad_dev.padtype == NUMBER_KEYPAD)			//数字键盘
			{
				numkeypad_process(_aNumButtonData, Id, pMsg);		//处理数字键盘
			}
			else if (keypad_dev.padtype == SIGN_KEYPAD)
			{
				signkeypad_process(_aSinButtonData, Id, pMsg);
			}
			else if (keypad_dev.padtype == CHINESE_KEYPAD)
			{
				chinesekeypad_process(_aChineseButtonData1, Id, pMsg);
			}
			break;
		}
		break;
	case CHANGE_ENGLISHPADTO_NUMPAD:			//从英文键盘切换到数字键盘
		for (i = 0; i<GUI_COUNTOF(_aEngButtonData); i++) WM_DeleteWindow(keypad_dev.EngButton_Handle[i]);
		for (i = 0; i<GUI_COUNTOF(_aEngButtonData); i++) keypad_dev.EngButton_Handle[i] = 0;
		drawnumber_pad(hWin);
		break;
	case CHANGE_NUMPADTO_ENGLISHPAD:			//从数字键盘切换到英文键盘
		for (i = 0; i<GUI_COUNTOF(_aNumButtonData); i++) WM_DeleteWindow(keypad_dev.NumButton_Handle[i]);
		for (i = 0; i<GUI_COUNTOF(_aEngButtonData); i++) keypad_dev.NumButton_Handle[i] = 0;
		drawenglish_pad(hWin);
		break;
	case CHANGE_ENGLISHPADTO_SIGNPAD1:			//从英文键盘切换到符号键盘
		for (i = 0; i<GUI_COUNTOF(_aEngButtonData); i++) WM_DeleteWindow(keypad_dev.EngButton_Handle[i]);
		for (i = 0; i<GUI_COUNTOF(_aEngButtonData); i++) keypad_dev.SignButton_Handle[i] = 0;
		drawsign_pad(hWin);
		break;
	case CHANGE_SIGNPADTO_ENGLISHPAD:			//从符号键盘切换到英文键盘
		for (i = 0; i<GUI_COUNTOF(_aSinButtonData); i++) WM_DeleteWindow(keypad_dev.SignButton_Handle[i]);
		for (i = 0; i<GUI_COUNTOF(_aSinButtonData); i++) keypad_dev.SignButton_Handle[i] = 0;
		drawenglish_pad(hWin);
		break;
	case CHANGE_SIGNPAD:						//切换符号键盘
		for (i = 0; i<GUI_COUNTOF(_aSinButtonData); i++) WM_DeleteWindow(keypad_dev.SignButton_Handle[i]);
		for (i = 0; i<GUI_COUNTOF(_aSinButtonData); i++) keypad_dev.SignButton_Handle[i] = 0;
		drawsign_pad(hWin);
		break;
	case CHANGE_ENGLISHPADTO_CHINESEPAD:         //从英文键盘切换到中文键盘
		for (i = 0; i<GUI_COUNTOF(_aEngButtonData); i++) WM_DeleteWindow(keypad_dev.EngButton_Handle[i]);
		for (i = 0; i<GUI_COUNTOF(_aEngButtonData); i++) keypad_dev.EngButton_Handle[i] = 0;
		drawchinese1_pad(hWin);
		drawchinese2_pad(hWin);
		break;
	case CHANGE_CHINESEPADTO_ENGLISHPAD:        //从中文键盘切换到英文键盘
		for (i = 0; i<(GUI_COUNTOF(_aChineseButtonData1) + GUI_COUNTOF(_aChineseButtonData2)); i++) WM_DeleteWindow(keypad_dev.ChineseButton_Handle[i]);
		for (i = 0; i<(GUI_COUNTOF(_aChineseButtonData1) + GUI_COUNTOF(_aChineseButtonData2)); i++) keypad_dev.ChineseButton_Handle[i] = 0;
		memset(keypad_dev.inputstr, 0, 7);
		keypad_dev.cur_index = 0;
		keypad_dev.inputlen = 0;
		keypad_dev.result_num = 0;
		keypad_dev.pypagenum = 0;
		keypad_dev.pynowpage = 0;
		keypad_dev.dispagecur = 0;
		keypad_dev.dispagenum = 0;
		keypad_dev.sta = 0;
		drawenglish_pad(hWin);
		break;
	case CHANGE_CHINESE_PY:                                                 //显示九宫格输入的拼音选项
		if (keypad_dev.cur_index>0)                                         //keypad_dev.curindex>0说明有拼音选中
		{
			keypad_dev.disstr = t9.pymb[keypad_dev.cur_index - 1]->pymb;    //获得当前码表
			slen = strlen((const char*)keypad_dev.disstr);                  //得到当前选中的拼音对应的汉字字符串的长度
			keypad_dev.hznum = slen / 3;                                    //因为是中文，每个字视为2个字符。 
		}
		else                                                                //没有拼音选中
		{
			keypad_dev.disstr = NULL;
			slen = 0;
			keypad_dev.hznum = 0;
		}
		keypad_dev.dispagenum = keypad_dev.hznum / 6 + (keypad_dev.hznum % 6 ? 1 : 0);              //得到汉字字符串的总页数
		keypad_dev.pypagenum = keypad_dev.result_num / 4 + (keypad_dev.result_num % 4 ? 1 : 0);     //得到拼音页数
		if (keypad_dev.inputstr[0] != NULL)
		{
			for (i = 0; i<4; i++)
			{
				BUTTON_SetFont(keypad_dev.ChineseButton_Handle[21 + i], &GUI_FontPingFangRegular60);
				if ((i + 4 * (keypad_dev.pynowpage))>(keypad_dev.result_num - 1))
				{
					BUTTON_SetText(keypad_dev.ChineseButton_Handle[21 + i], " ");
				}
				else
				{
					BUTTON_SetText(keypad_dev.ChineseButton_Handle[21 + i], (const char*)t9.pymb[i + 3 * (keypad_dev.pynowpage)]->py);
				}
			}

			keypad_hbtn_updc(keypad_dev.disstr + (keypad_dev.dispagecur * 6 * 3));  //更新横向按钮名字
			keypad_draw_hbtn(keypad_dev.ChineseButton_Handle);                      //绘制横向按钮
		}
		else                                                                       //没有输入任何拼音
		{
			for (i = 0; i<3; i++)
			{
				BUTTON_SetFont(keypad_dev.ChineseButton_Handle[21 + i], &GUI_FontPingFangRegular60);
				BUTTON_SetText(keypad_dev.ChineseButton_Handle[21 + i], " ");
			}

			for (i = 0; i<6; i++)
			{
				keypad_dev.disbuf[i][0] = ' ';
				keypad_dev.disbuf[i][1] = ' ';
				keypad_dev.disbuf[i][2] = '\0';
				keypad_dev.butname[i] = keypad_dev.disbuf[i];
			}
			keypad_draw_hbtn(keypad_dev.ChineseButton_Handle);    //绘制横向按钮
		}
		break;

	}
}


static void _cbBk(WM_MESSAGE * pMsg)
{
	switch (pMsg->MsgId)
	{
	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		//GUI_DrawGradientV(0, 0, 720, 1280, COLOR_BACK0, COLOR_BACK1);
		break;
	}
}

static void _cbhFrame(WM_MESSAGE * pMsg) {
	WM_HWIN    hWin;
	
	hWin = pMsg->hWin;
	switch (pMsg->MsgId) {
	
	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}



extern BUTTON_STATE_T BUTTON_STATE;
extern unsigned char LAST_PAGE;
void _cbMenuPageTwoInsertAddr(WM_MESSAGE * pMsg) {
	WM_HWIN  hFrame;

	WM_HWIN  hButton0;
	WM_HWIN  hButton1;
	WM_HWIN  hWin;
	int NCode;
	int Id = 0;
	hWin = pMsg->hWin;

	switch (pMsg->MsgId) {
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);      // Id of widget
		NCode = pMsg->Data.v;              // Notification code
		switch (Id)
		{
		case GUI_ID_BUTTON0:
			BUTTON_STATE = (BUTTON_STATE_T)LAST_PAGE;
			break;
		case GUI_ID_BUTTON1:

			break;
		default:
			break;
		}
		break;
	case WM_CREATE:

		keypad_dev.xpos = 0;
		keypad_dev.ypos = 724;
		keypad_dev.width = 720;
		keypad_dev.height = 556;
		keypad_dev.padtype = ENGLISH_KEYPAD;
		keypad_dev.signpad_flag = 0;
		keypad_dev.signpad_num = 2;
		keypad_dev.inputlen = 0;
		keypad_dev.pynowpage = 0;
		keypad_dev.cur_index = 0;
		keypad_dev.sta = 0;

		hButton0 = BUTTON_CreateEx(0, 180, 160, 90, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON0);
		BUTTON_SetBitmapEx(hButton0, BUTTON_BI_UNPRESSED, &bmyesbutton, 0, 0);
		BUTTON_SetBitmapEx(hButton0, BUTTON_BI_PRESSED, &bmyesbutton, 0, 0);
		
		hButton1 = BUTTON_CreateEx(560, 180, 160, 90, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON1);
		BUTTON_SetBitmapEx(hButton1, BUTTON_BI_UNPRESSED, &bmcancelbutton, 0, 0);
		BUTTON_SetBitmapEx(hButton1, BUTTON_BI_PRESSED, &bmcancelbutton, 0, 0);

		BUTTON_SetDefaultSkin(_DrawSkinFlex_BUTTON);

		keypad_dev.hKeypad = WM_CreateWindowAsChild(0, 724, 720, 466, hWin, WM_CF_SHOW | WM_CF_STAYONTOP, _cbKeyPad, 0);


		BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
		FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
		FRAMEWIN_SetDefaultTextAlign(GUI_TA_HCENTER);

		//创建FRAME窗口
		hFrame = WM_CreateWindowAsChild(0, 0, 720, 160, hWin, WM_CF_SHOW, _cbhFrame, 0);

		//创建一个multi edit(多行文本小工具)小工具
		hMulti = MULTIEDIT_CreateEx(0, 0, 0, 0, WM_GetClientWindow(hFrame), WM_CF_SHOW, 0, GUI_ID_MULTIEDIT0, 100, NULL);
		MULTIEDIT_EnableBlink(hMulti, 500, 1);		    	//开启光标,周期500ms
		MULTIEDIT_SetInsertMode(hMulti, 1);             //开启插入模式
		MULTIEDIT_SetFont(hMulti, &GUI_Fontydinsertfont40);
		WM_SetFocus(hMulti);

		break;
	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();

		//GUI_SetColor(GUI_WHITE);
		//GUI_SetFont(&GUI_Font32B_1);
		//GUI_DispStringAt("null", 5, 10);
		break;
	}
}





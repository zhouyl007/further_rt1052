/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2016  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.38 - Graphical user interface for embedded applications **
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only  be used  in accordance  with  a license  and should  not be  re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : MOTION_SmartPhoneMenu.c
Purpose     : Shows how to create a radial menu with motion support
Requirements: WindowManager - (x)
              MemoryDevices - (x)
              AntiAliasing  - (x)
              VNC-Server    - ( )
              PNG-Library   - ( )
              TrueTypeFonts - ( )
----------------------------------------------------------------------
*/

/*********************************************************************
*
*       Includes
*
**********************************************************************
*/
#include "DIALOG.h"
#include "ICONVIEW.h"
#include "public_h.h"
#include "bsp_board_init.h"



PAGEONE_CONTROL_STU g_pageone_control_stu;


static void _cbMenu(WM_MESSAGE * pMsg);
static void _CreateSelected(int Index, WM_HWIN hWin);


extern GUI_CONST_STORAGE GUI_BITMAP bmiconperformanceparameter;
extern GUI_CONST_STORAGE GUI_BITMAP bmiconco2knowledge;
extern GUI_CONST_STORAGE GUI_BITMAP bmiconuvknowledge;
extern GUI_CONST_STORAGE GUI_BITMAP bmicondustknowledge;
extern GUI_CONST_STORAGE GUI_BITMAP bmiconelecradknowledge;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontPingFangRegular67;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontHoloLensMDL2Assets64;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontHoloLensMDL2Assets48;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontPingFangRegular67;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontPingFangRegular52;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontPingFangRegular41;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontDingbats2114;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontPingFangRegular90;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontPingFangRegular135;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontHoloLensMDL2Assets133;

extern GUI_CONST_STORAGE GUI_FONT GUI_FontPingFang67;


extern GUI_CONST_STORAGE GUI_FONT GUI_FontSong2K60;

extern GUI_CONST_STORAGE GUI_BITMAP bmco2bar;
extern GUI_CONST_STORAGE GUI_BITMAP bmuvbar;
extern GUI_CONST_STORAGE GUI_BITMAP bmpmbar;
extern GUI_CONST_STORAGE GUI_BITMAP bmreturnleft;
extern GUI_CONST_STORAGE GUI_BITMAP bmreturnright;
extern GUI_CONST_STORAGE GUI_BITMAP bmreturnbkleft;
extern GUI_CONST_STORAGE GUI_BITMAP bmreturnbkright;

extern GUI_CONST_STORAGE GUI_BITMAP bmiconuv;
extern GUI_CONST_STORAGE GUI_BITMAP bmiconco2;
extern GUI_CONST_STORAGE GUI_BITMAP bmiconknow;
extern GUI_CONST_STORAGE GUI_BITMAP bmiconbrowse;

extern GUI_CONST_STORAGE GUI_FONT GUI_Fontydfont32;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontydfont44;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontydfont46;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontydfont47;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontydfont54;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontydfont62;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontydinsertfont40;

#define ID_TIMER_TIME       1
#define ID_TIMER_CHARGE     2


BUTTON_STATE_T BUTTON_STATE = BUTTON_PAGE_ONE;

DEV_STATE_CTR_T dev_state;


GUI_RECT Rect_0 = { 20, 90, 480, 130 };
GUI_RECT Rect_1 = { 150, 160, 300, 300 };
GUI_RECT Rect_2 = { 150, 380, 300, 500 };
GUI_RECT Rect_3 = { 20, 560, 400, 753 };
GUI_RECT Rect_4 = { 380, 400, 479, 450 };

#define GUI_ID_USER_BUTTON_0   (GUI_ID_USER + 0x01)
#define GUI_ID_USER_BUTTON_1   (GUI_ID_USER + 0x0A)
#define GUI_ID_USER_BUTTON_2   (GUI_ID_USER + 0x0C)

#define GUI_ID_USER_DUST_SET 	     (GUI_ID_BUTTON0 + 1000)
#define GUI_ID_USER_SYSTEM_SET	   (GUI_ID_BUTTON0 + 1001)
#define GUI_ID_USER_NET_SET        (GUI_ID_BUTTON0 + 1002)
#define GUI_ID_USER_SCREEN_SET 	   (GUI_ID_BUTTON0 + 1003)
#define GUI_ID_USER_ENGINEER_SET   (GUI_ID_BUTTON0 + 1004)

unsigned short BatteryIndex[] = { 0xEBA0, 0xEBA1, 0xEBA2, 0xEBA3, 0xEBA4, 0xEBA5, 0xEBA6, 0xEBA7,
0xEBA8, 0xEBA9, 0xEBAA, 0xEBAB, 0xEBAC, 0xEBAD, 0xEBAE, 0xEBAF,
0xEBB0, 0xEBB1, 0xEBB2, 0xEBB3, 0xEBB4, 0xEBB5 };


static const char ucWeekDay[7][4] =
{
	"Mon",
	"Tue",
	"Wed",
	"Thu",
	"Fri",
	"Sat",
	"Sun"
};

static const GUI_POINT _aPointArrow[] = {
		{ 0, 0 },
		{ 29, 29 },
		{ 29, 16 },
		{ 68, 16 },
		{ 68, -16 },
		{ 29, -16 },
		{ 29, -29 },
};

typedef struct
{
	const GUI_BITMAP * pBitmap;
	const char       * pTextEn;
	const char       * pTextCn;
} BITMAP_ITEM;



unsigned char CGK_cmdswitch = 0;
unsigned char PagethreeSkinFlag = 0;
unsigned char LAST_PAGE = 0;


#define ICONVIEW_TBorder   60   		/* 控件ICONVIEW的上边距 */
#define ICONVIEW_LBorder   95   		/* 控件ICONVIEW的左边距 */  
#define ICONVIEW_Width     173  		/* 控件ICONVIEW的宽 */  
#define ICONVIEW_Height    173  		/* 控件ICONVIEW的高 */  
#define ICONVIEW_YSpace    61   		/* 控件ICONVIEW的Y方向间距 */
#define ICONVIEW_XSpace    61    		/* 控件ICONVIEW的X方向间距 */
#define ICON_Width     	   173      	/* 控件ICONVIEW的图标的宽度 */  
#define ICON_Height    	   173       	/* 控件ICONVIEW的图标的高度, 含图标下面的文本，一起的高度 */ 

static WM_HWIN hIcon;
static WM_HWIN hButton_Cancel;
static WM_HWIN hButton_OK;
static WM_HWIN hButton_Close;

/*
*********************************************************************************************************
*	函 数 名: Caculate_RTC
*	功能说明: 显示RTC时间
*	形    参: pMsg  WM_MESSAGE类型指针变量
*	返 回 值: 无
*********************************************************************************************************
*/

static void Caculate_RTC(WM_MESSAGE * pMsg)
{
	char buf[30];
	//TM_RTC_t DateTime;
	WM_HWIN hWin = pMsg->hWin;

	//TM_RTC_GetDateTime(&DateTime,RTC_Format_BIN);

	sprintf(buf,
		"%0.2d:%0.2d:%0.2d %0.3s",
		0,
		0,
		0,
		0);

	sprintf(buf,
		"20%0.2d.%0.2d.%0.2d",
		18,
		7,
		11);
}


/*********************************************************************
*
*       _cbStatus
*/
static void _cbStatus(WM_MESSAGE * pMsg) {
	int bat_percent = 0;
	static int Time = 9 * 60;
	static int Charge = GUI_COUNTOF(BatteryIndex);
	WM_HWIN    hWin;
	static     WM_HTIMER hTimerTime;
	static     WM_HTIMER hTimerCharge;
	int        xSize;
	int        ySize;
	int        Id;
	int        i;
	int        xPos;
	char 		   buf[30];

	hWin = pMsg->hWin;
	switch (pMsg->MsgId) {
	case WM_PRE_PAINT:
		/*GUI_MULTIBUF_Begin();*/
		break;
	case WM_POST_PAINT:
		/*GUI_MULTIBUF_End();*/
		break;
	case WM_CREATE:
		hTimerTime = WM_CreateTimer(hWin, ID_TIMER_TIME, 1000, 0);
		hTimerCharge = WM_CreateTimer(hWin, ID_TIMER_CHARGE, 2100, 0);
		break;
	case WM_DELETE:
		WM_DeleteTimer(hTimerTime);
		WM_DeleteTimer(hTimerCharge);
		break;
	case WM_TIMER:
		Id = WM_GetTimerId(pMsg->Data.v);
		switch (Id) {
		case ID_TIMER_TIME:    // 1s * 24 * 60
			Time += 1;
			if (Time == 24 * 60) {
				Time = 0;
			}
			break;
		case ID_TIMER_CHARGE:  // 2.1s 
			Charge -= 1;
			if (Charge < 0) {
				Charge = GUI_COUNTOF(BatteryIndex);
			}
			break;
		}
		WM_InvalidateWindow(hWin);
		WM_RestartTimer(pMsg->Data.v, 0);
		break;
	case WM_PAINT:
		xSize = WM_GetWindowSizeX(hWin);
		ySize = WM_GetWindowSizeY(hWin);
		//
		// Draw background
		//
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		GUI_SetColor(GUI_GRAY);
		GUI_DrawHLine(73, 0, xSize);
		GUI_DrawHLine(74, 0, xSize);
		GUI_DrawHLine(75, 0, xSize);
		GUI_DrawHLine(76, 0, xSize);
		//
		// Draw time
		//
		GUI_SetTextMode(GUI_TM_TRANS);
		GUI_SetColor(GUI_WHITE);
		GUI_SetFont(&GUI_FontPingFangRegular52);

		sprintf(buf,
			"20%0.2d.%0.2d.%0.2d",
			18,
			7,
			11);

		GUI_DispStringAt(buf, 26, 10);

		//GUI_SetFont(&GUI_FontOpenSans51);
		GUI_GotoXY(230, 10);
		GUI_DispDec(Time / 60, 2);
		GUI_DispChar(':');
		GUI_DispDec(Time / 60, 2);
		GUI_DispChar(':');
		GUI_DispDec(Time % 60, 2);
		//
		// Draw charge level
		//
		// 3.06V - 4.2V
		bat_percent = (int)(g_AdcVatData.fVoltageValue * 10000 / 57.0 - 268);
		if (bat_percent < 0) {
			bat_percent = 0;
		}
		else if (bat_percent >= 100){
			bat_percent = 99;
		}
		if (bat_percent < 10) {
			sprintf(buf, "%0.1d", bat_percent);
		}
		else {
			sprintf(buf, "%0.2d", bat_percent);
		}

		GUI_DispStringAt(buf, 550, 10);
		GUI_DispStringAt("\x25", 592, 10); // "%"

		GUI_SetFont(&GUI_FontHoloLensMDL2Assets48);
		GUI_SetColor(GUI_WHITE);
		GUI_DispCharAt(BatteryIndex[0], 630, 10);  // 妗?
		//    if (Charge) {
		GUI_DispCharAt(BatteryIndex[bat_percent / 10], 630, 10);  // 鐢甸噺
		//    }
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}


static int _ButtonPagethreeSkin(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) {
	int Index, xSize, ySize, IsPressed;
	WM_HWIN hWin;
	GUI_COLOR Color;

	hWin = pDrawItemInfo->hWin;
	switch (pDrawItemInfo->Cmd) {
	case WIDGET_ITEM_DRAW_BACKGROUND:

		BUTTON_GetUserData(hWin, &Index, sizeof(Index));
		IsPressed = BUTTON_IsPressed(pDrawItemInfo->hWin);

		xSize = WM_GetWindowSizeX(hWin);
		ySize = WM_GetWindowSizeY(hWin);
		

		if (PagethreeSkinFlag == (Index - 1000 - 0x170)) {
			Color = GUI_MAGENTA;
		}
		else {
			Color = GUI_BLACK;
		}
		GUI_SetBkColor(Color);
		GUI_Clear();

		GUI_SetColor(GUI_WHITE);
		GUI_SetTextMode(GUI_TM_TRANS);
		GUI_SetFont(&GUI_Fontydfont47);


		switch (Index) {
		case GUI_ID_USER_DUST_SET:
			GUI_DispStringAt("\xe7\xb2\x89\xe5\xb0\x98\xe8\x87\xaa\xe5\x8a\xa8\xe6\xa3\x80\xe6\xb5\x8b\xe9\x97\xb4\xe9\x9a\x94", 100, 10);
			break;
		case GUI_ID_USER_SYSTEM_SET:
			GUI_DispStringAt("\xe7\xb3\xbb\xe7\xbb\x9f\xe8\xae\xbe\xe7\xbd\xae", 100, 10);
			break;
		case GUI_ID_USER_NET_SET:
			GUI_DispStringAt("\xe7\xbd\x91\xe7\xbb\x9c\xe8\xae\xbe\xe7\xbd\xae", 100, 10);
			break;
		case GUI_ID_USER_SCREEN_SET:
			GUI_DispStringAt("\xe5\xb1\x8f\xe5\xb9\x95\xe4\xba\xae\xe5\xba\xa6", 100, 10);
			break;
		case GUI_ID_USER_ENGINEER_SET:
			GUI_DispStringAt("\xe5\xb7\xa5\xe7\xa8\x8b\xe5\xb8\x88\xe8\xae\xbe\xe7\xbd\xae", 100, 10);
			break;
		default:
			break;
		}
		
		break;
	}
	return 0;
}



static int _Button_PM25_Skin(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) {
	int Index, xSize, ySize, IsPressed;
	WM_HWIN hWin;
	const GUI_BITMAP * pBm;
	GUI_COLOR Color;

	hWin = pDrawItemInfo->hWin;
	switch (pDrawItemInfo->Cmd) {
	case WIDGET_ITEM_DRAW_BACKGROUND:
		xSize = WM_GetWindowSizeX(hWin);
		ySize = WM_GetWindowSizeY(hWin);

		// Draw antialiased rounded rectangle as background for image
		GUI_SetColor(GUI_MAGENTA);

		GUI_AA_FillCircle(80, 80, 75);
		GUI_SetColor(GUI_WHITE);
		GUI_SetTextMode(GUI_TM_TRANS);
		GUI_SetFont(&GUI_FontPingFang67);

		if (g_pageone_control_stu.ButtonPmisPress) {
			GUI_DispStringAt("\xE7\xAB\x8B\xE5\x8D\xB3", 30,  20);   // 立即
			GUI_DispStringAt("\xE5\x81\x9C\xE6\xAD\xA2", 30, 75);    // 停止

		}
		else {
			GUI_DispStringAt("\xe9\xa9\xac\xe4\xb8\x8a", 30, 20);     // 马上
			GUI_DispStringAt("\xe6\xa3\x80\xe6\xb5\x8b", 30, 75);    // 检测
		}

		//WM_InvalidateWindow(hWin);
		break;
	}
	return 0;
}

static int _Button_CO2_Skin(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) {
	int Index, xSize, ySize, IsPressed;
	WM_HWIN hWin;
	const GUI_BITMAP * pBm;
	GUI_COLOR Color;

	hWin = pDrawItemInfo->hWin;
	switch (pDrawItemInfo->Cmd) {
	case WIDGET_ITEM_DRAW_BACKGROUND:

		xSize = WM_GetWindowSizeX(hWin);
		ySize = WM_GetWindowSizeY(hWin);

		// Draw antialiased rounded rectangle as background for image
		
		GUI_SetColor(GUI_MAGENTA);
		GUI_AA_FillCircle(80, 80, 75);
		GUI_SetColor(GUI_WHITE);
		GUI_SetTextMode(GUI_TM_TRANS);
		GUI_SetFont(&GUI_FontPingFang67);
		if (g_pageone_control_stu.ButtonCo2isPress) {
			GUI_DispStringAt("\xE7\xAB\x8B\xE5\x8D\xB3", 30, 20);   // 立即
			GUI_DispStringAt("\xE5\x81\x9C\xE6\xAD\xA2", 30, 75);    // 停止
		}
		else {
			GUI_DispStringAt("\xe9\xa9\xac\xe4\xb8\x8a", 30, 20);   // 马上
			GUI_DispStringAt("\xe6\xa3\x80\xe6\xb5\x8b", 30, 75);   // 检测
		}
		//WM_InvalidateWindow(hWin);
		break;
	}
	return 0;
}

static int _ButtonSkinAddr(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) {
	int Index, xSize, ySize, IsPressed;
	WM_HWIN hWin;
	const GUI_BITMAP * pBm;
	GUI_COLOR Color;

	hWin = pDrawItemInfo->hWin;
	switch (pDrawItemInfo->Cmd) {
	case WIDGET_ITEM_DRAW_BACKGROUND:
		IsPressed = BUTTON_IsPressed(pDrawItemInfo->hWin);

		xSize = WM_GetWindowSizeX(hWin);
		ySize = WM_GetWindowSizeY(hWin);

		if (IsPressed) {
			Color = GUI_DARKGRAY;
		}
		else {
			Color = GUI_MAGENTA;
		}

		//GUI_SetBkColor(Color);
		GUI_Clear();
		//
		// Draw antialiased rounded rectangle as background for image
		//
		GUI_SetColor(Color);
		GUI_AA_FillRoundedRect(0, 10, xSize, ySize, 10);
		//
		// Draw button text
		//
		GUI_SetColor(GUI_WHITE);
		GUI_SetTextMode(GUI_TM_TRANS);
		GUI_SetFont(&GUI_FontPingFang67);
		GUI_DispStringAt("\xe8\xbe\x93\xe5\x85\xa5\xe5\x9c\xb0\xe5\x9d\x80", 5, 10); // 输入地址

		break;
	}
	return 0;
}

/*********************************************************************
*
*       _cbSelected
*
* Purpose:
*   Callback function of window created in reaction of pressing a button.
*   Responsible for drawing only.
*/

static int XPOS_OFFSET = 50;
static int XPOS_OFFSET_VAR = 250;
static int YPOS_OFFSET = 180;
static int YPOS_OFFSET_GRAD = 80;
static int YPOS_OFFSET_PROG = 100;

#define CO2_TYPE   	0
#define PM25_TYPE  	1
#define EMR_TYPE   	2
#define UV_TYPE   	3

static void  _CreateProgressBar(int xPos0, int yPos0, int xPos1, int yPos1, int r, int x_offset) {
	if (x_offset >= yPos1 - yPos0)
		x_offset -= 25;

	GUI_SetColor(GUI_WHITE);
	GUI_AA_FillRoundedRect(xPos0 + x_offset, yPos0 - (70 - (yPos1 - yPos0)) / 2, xPos0 + 25 + x_offset, yPos1 + (70 - (yPos1 - yPos0)) / 2, 12);
}

static void  _CreateProgressBarligttle(int xPos0, int yPos0, int xPos1, int yPos1, int r, int x_offset) {
	if (x_offset >= yPos1 - yPos0)
		x_offset -= 18;

	GUI_SetColor(GUI_WHITE);
	GUI_AA_FillRoundedRect(xPos0 + x_offset, yPos0 - (43 - (yPos1 - yPos0)) / 2, xPos0 + 18 + x_offset, yPos1 + (43 - (yPos1 - yPos0)) / 2, 7);
}

static int GUI_ProgbarCustom_UV(int SampleVal, int Sensor)
{
	static int x_offset = 0;

	if (Sensor == UV_TYPE)
		x_offset = SampleVal;
	else
		return 0;

	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetColor(GUI_WHITE);

	GUI_SetFont(&GUI_FontHoloLensMDL2Assets133);
	GUI_DispCharAt(0xE706, 530, 60); // SUN
	GUI_SetFont(&GUI_FontPingFangRegular90);
	GUI_UC_SetEncodeUTF8();
	GUI_DispStringAt("\xe7\xb4\xab\xe5\xa4\x96\xe7\xba\xbf", 30, 75); // ???

	GUI_SetColor(0xff8000);
	GUI_FillRect(0, 250, 720, 650);

	GUI_SetColor(GUI_WHITE);
	if (SampleVal < 2) {
		GUI_SetFont(&GUI_FontPingFangRegular135);
		GUI_DispStringAt("\xe9\x9b\xb6\xe7\xba\xa7", 30, 450); // ??
		GUI_SetFont(&GUI_FontPingFangRegular52);
		GUI_DispStringAt("\xe6\x97\xa0\xe5\x8d\xb1\xe5\xae\xb3", 341, 280); // ???

	}
	else if (SampleVal <= 3 && SampleVal >= 2) {
		GUI_SetFont(&GUI_FontPingFangRegular135);
		GUI_DispStringAt("\xe4\xb8\x80\xe7\xba\xa7", 30, 450); // ??
		GUI_SetFont(&GUI_FontPingFangRegular52);
		GUI_DispStringAt("\xe8\xbd\xbb\xe5\x8d\xb1\xe5\xae\xb3", 341, 280); // ???
	}
	else if (SampleVal > 4) {
		GUI_SetFont(&GUI_FontPingFangRegular135);
		GUI_DispStringAt("\xe4\xba\x8c\xe7\xba\xa7", 30, 450); // ??
		GUI_SetFont(&GUI_FontPingFangRegular52);
		GUI_DispStringAt("\xe5\x8d\xb1\xe5\xae\xb3", 341, 280); // ??
	}

	GUI_DrawBitmap(&bmuvbar, 30, 285);

	_CreateProgressBarligttle(30, 288, 321, 320, 12, x_offset);

	return 0;
}

static int GUI_ProgbarCustom_PM(int SampleVal, int Sensor)
{
	static int x_offset = 0;

	if (Sensor == PM25_TYPE)
		x_offset = (460 * SampleVal) / 300000;
	else
		return 0;

	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetColor(GUI_WHITE);
	GUI_SetFont(&GUI_FontPingFangRegular67);
	GUI_UC_SetEncodeUTF8();

	if (SampleVal < 100000)
		GUI_DispStringAt("\xe6\x97\xa0\xe6\xb1\xa1\xe6\x9f\x93", 540, 80); // 无污染
	else if (SampleVal <= 200000 && SampleVal >= 100000)
		GUI_DispStringAt("\xe4\xb8\xad\xe6\xb1\xa1\xe6\x9f\x93", 540, 80); // 中污染
	else if (SampleVal > 200000)
		GUI_DispStringAt("\xe9\x87\x8d\xe6\xb1\xa1\xe6\x9f\x93", 540, 80); // 重污染

	GUI_DispStringAt("PM2.5\xe7\xb2\x89\xe5\xb0\x98", 30, 10);              // PM2.5粉尘
	GUI_DispStringAt("\xe5\xbe\xae\xe5\x85\x8b/\xe7\xab\x8b\xe6\x96\xb9\xe7\xb1\xb3 \x28        \x29", 30, 157); // 微克/立方米 (ug / m3)

	GUI_UC_SetEncodeNone();
	GUI_DispStringAt("\xB5g/", 326, 157); // ug/
	GUI_DispCharAt(0x33a5, 399, 157); 	  // "m3"

	GUI_SetFont(&GUI_FontPingFangRegular41);
	GUI_UC_SetEncodeUTF8();
	// 可吸入颗粒物
	GUI_DispStringAt("\xe5\x8f\xaf\xe5\x90\xb8\xe5\x85\xa5\xe9\xa2\x97\xe7\xb2\x92\xe7\x89\xa9", 280, 27);

	GUI_DrawBitmap(&bmpmbar, 30, 85);

	_CreateProgressBar(30, 90, 520, 130, 17, x_offset);

	return 0;
}

static int GUI_ProgbarCustom_CO2(int SampleVal, int Sensor)
{
	char buf[32];
	static int x_offset = 0;

	if (Sensor == CO2_TYPE)
		x_offset = (286 * SampleVal) / 5000;
	else
		return 0;

	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetColor(GUI_WHITE);
	GUI_SetFont(&GUI_FontPingFangRegular67);
	GUI_UC_SetEncodeUTF8();
	GUI_DispStringAt("\xe4\xba\x8c\xe6\xb0\xa7\xe5\x8c\x96\xe7\xa2\xb3 CO", 30, 730); // 二氧化碳
	GUI_UC_SetEncodeNone();
	GUI_DispCharAt(0x2082, 304, 728); // SUBSCRIPT 2

	GUI_SetFont(&GUI_FontPingFangRegular52);
	GUI_UC_SetEncodeUTF8();
	if (SampleVal < 1200)
		GUI_DispStringAt("\xe4\xb8\x8d\xe7\xbc\xba\xe6\xb0\xa7", 348, 800); // 不缺氧
	else if (SampleVal >= 1200 && SampleVal <= 2400)
		GUI_DispStringAt("\xe5\xbe\xae\xe7\xbc\xba\xe6\xb0\xa7", 348, 800); // 微缺氧
	else if (SampleVal > 2400 && SampleVal <= 4000)
		GUI_DispStringAt("\xe7\xbc\xba\xe6\xb0\xa7", 348, 800); // 缺氧
	else if (SampleVal > 4000)
		GUI_DispStringAt("\xe9\x87\x8d\xe7\xbc\xba\xe6\xb0\xa7", 348, 800); // 重缺氧

	sprintf(buf, "%d ppm", SampleVal);
	GUI_DispStringAt(buf, 348, 740); // ppm

	GUI_DrawBitmap(&bmco2bar, 30, 809);

	_CreateProgressBarligttle(30, 812, 321, 844, 12, x_offset);

	return 0;
}

static int GUI_ProgbarCustom_EMR(int SampleVal, int Sensor)
{
	char buf[32];
	static int x_offset = 0;

	if (Sensor == EMR_TYPE)
		x_offset = (286 * SampleVal) / 500;
	else
		return 0;

	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetColor(GUI_WHITE);
	GUI_SetFont(&GUI_FontPingFangRegular67);
	GUI_UC_SetEncodeUTF8();
	GUI_DispStringAt("\xe7\x94\xb5\xe7\xa3\x81\xe8\xbe\x90\xe5\xb0\x84", 30, 930); // 电磁辐射

	GUI_SetFont(&GUI_FontPingFangRegular52);
	if (SampleVal < 50)
		GUI_DispStringAt("\xe6\x97\xa0\xe6\xb1\xa1\xe6\x9f\x93", 348, 1003); // 无污染
	else if (SampleVal >= 50 && SampleVal <= 120)
		GUI_DispStringAt("\xe4\xb8\xad\xe6\xb1\xa1\xe6\x9f\x93", 348, 1003); // 中污染
	else if (SampleVal > 120)
		GUI_DispStringAt("\xe9\x87\x8d\xe6\xb1\xa1\xe6\x9f\x93", 348, 1003); // 重污染

	sprintf(buf, "%d ", SampleVal);
	GUI_DispStringAt(buf, 348, 940);
	GUI_DispStringAt("W", 430, 940);
	GUI_UC_SetEncodeNone();
	GUI_DispCharAt(0xb5, 405, 940);

	GUI_SetFont(&GUI_FontPingFangRegular41);
	GUI_UC_SetEncodeUTF8();
	GUI_DispStringAt("\xe5\xae\x9e\xe6\x97\xb6", 600, 1023); // 实时

	GUI_DrawBitmap(&bmco2bar, 30, 1009);



	_CreateProgressBarligttle(30, 1012, 321, 1044, 12, x_offset);

	return 0;
}


static void GUI_SampleValue_Disp(int PM25_Value, int PM10_Value, int CO2_Value,
	int PM03_Count, int PM25_Count, int PM10_Count,
	int ELEC_Value)
{
	char buf[32];

	GUI_SetBkColor(GUI_BLACK);
	GUI_Clear();

	GUI_SetColor(GUI_WHITE);
	GUI_SetTextMode(GUI_TM_TRANS);
	if (g_pageone_control_stu.ButtonPmisPress) {
		GUI_SetFont(&GUI_FontPingFang67);
		GUI_GotoXY(588, 150);
		GUI_DispDec(g_pageone_control_stu.ButtonPmtimeDelay, 2);
	}
	

	GUI_SetFont(&GUI_FontPingFangRegular41);
	GUI_UC_SetEncodeUTF8();
	// 激光个数
	GUI_DispStringAt("\xe6\xbf\x80\xe5\x85\x89\xe4\xb8\xaa\xe6\x95\xb0 \xe4\xb8\xaa/\xe5\x8d\x87 \x28L\x29", 80, 500);
	GUI_DispStringAt("PM0.3\x7e 2.5", 80, 553);
	GUI_DispStringAt("PM2.5\x7e 10", 80, 600);
	GUI_DispStringAt("PM10\xe4\xbb\xa5\xe4\xb8\x8a", 80, 647);

	GUI_SetFont(&GUI_FontPingFangRegular90);
	GUI_DispStringAt("PM2.5", 30, 265);
	GUI_DispStringAt("PM10", 30, 375);

	GUI_SetFont(&GUI_FontPingFangRegular135);
	GUI_GotoXY(255, 245);
	GUI_DispDecMin(PM25_Value); // pm2.5
	GUI_GotoXY(255, 355);
	GUI_DispDecMin(PM10_Value); // pm10

	GUI_SetFont(&GUI_FontPingFangRegular52);
	GUI_GotoXY(260, 548);
	GUI_DispDecMin(PM03_Count); // pm0.3 pm counting
	GUI_GotoXY(260, 595);
	GUI_DispDecMin(PM25_Count); // pm2.5 pm counting
	GUI_GotoXY(260, 642);
	GUI_DispDecMin(PM10_Count); // pm10 pm counting

	GUI_ProgbarCustom_PM(PM03_Count, PM25_TYPE);
	GUI_ProgbarCustom_CO2(CO2_Value, CO2_TYPE);
	GUI_ProgbarCustom_EMR(ELEC_Value, EMR_TYPE);
}

/*********************************************************************
*
*       _CreateSelected
*
* Purpose:
*   This function is called when a button is pressed. It creates a new
*   window and shows a small screen animation.
*/
static void _CreateSelected(int Index, WM_HWIN hWin) {
	WM_HWIN hWinBase;     // Window to be shifted out
	WM_HWIN hWinSelected; // New window to become visible behind hWinBase

	int xSize, ySize;

	hWinBase = WM_GetParent(WM_GetParent(hWin));
	xSize = WM_GetXSize(hWinBase);
	ySize = WM_GetYSize(hWinBase);

	if (Index == 0)
		BUTTON_STATE = BUTTON_PAGE_TWO;   // go to page 2 (right arrow)
	else if (Index == 1)
		BUTTON_STATE = BUTTON_PAGE_THREE; // go to page 3 (right arrow)
	else if (Index == 2)
		BUTTON_STATE = BUTTON_PAGE_ONE; 	// back to page 1 (left arrow)
	else if (Index == 4)
		BUTTON_STATE = BUTTON_PAGE_TWO; 	// back to page 2 left arrow)
	else if (Index == 1000)
		BUTTON_STATE = BUTTON_PAGE_TWO;   // windows back to page 2
}

/*********************************************************************
*
*       _cbMenu
*
* Purpose:
*   Callback function of menu window.
*/
static WM_HWIN hWinMenu;     // Menu window moveable within viewport window
static void _cbMenuPageOne(WM_MESSAGE * pMsg) {
	WM_HWIN	hButton;
	WM_HWIN hButton_0;
    WM_HWIN hButton_1;
	WM_HWIN hButton_2;
	WM_HWIN hButton_3;	
	WM_HWIN hWin;
	
	int xSize, i, NCode;
	int Id = 0;

	WM_MOTION_INFO * pInfo;

	hWin = pMsg->hWin;
	switch (pMsg->MsgId) {
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);      // Id of widget
		NCode = pMsg->Data.v;                 // Notification code

		switch (NCode) {
		case WM_NOTIFICATION_CLICKED:
			switch (Id)
			{
			case GUI_ID_BUTTON0:
				BUTTON_STATE = BUTTON_PAGE_TWO;
				break;
			case GUI_ID_BUTTON1:
				g_pageone_control_stu.ButtonPmtimeDelay = 35;
				g_pageone_control_stu.ButtonPmisPress ^= 1;
				g_pageone_control_stu.ButtonCo2timeDelay = 0;	
				if (g_pageone_control_stu.ButtonPmisPress) {
					CGK_cmdswitch = PMMEASURE;
					g_pageone_control_stu.ButtonCo2isPress = 0;
				}
				else {
					CGK_cmdswitch = PMCLOSE;
				}
			
				break;
			case GUI_ID_BUTTON2:
				g_pageone_control_stu.InsertAddrIndex = 1;  // PM addr
			  LAST_PAGE = BUTTON_PAGE_ONE;
				BUTTON_STATE = BUTTON_PAGE_INSERADDR;
				break;
			case GUI_ID_BUTTON3:
				g_pageone_control_stu.ButtonCo2isPress ^= 1;
				g_pageone_control_stu.ButtonCo2timeDelay = 35;
				g_pageone_control_stu.ButtonPmtimeDelay = 0;
				if (g_pageone_control_stu.ButtonCo2isPress) {
					CGK_cmdswitch = CO2MEASURE;
					//CGK_cmdswitch = ULTLMEASURE;
					g_pageone_control_stu.ButtonPmisPress = 0;
				}
				else {
					CGK_cmdswitch = CO2CLOSE;
				}
				break;
			case GUI_ID_BUTTON4:
				g_pageone_control_stu.InsertAddrIndex = 2;  // CO2 addr
				BUTTON_STATE = BUTTON_PAGE_INSERADDR;
				break;
			default:
				break;
			}
			WM_InvalidateWindow(hWin);
			break;
		}
		break;
	case WM_CREATE:		
		WM_CreateTimer(hWin, 0, 1000, 0);  // 2s reflash srceen 
			
		hButton = BUTTON_CreateEx(360, 1090, 360, 110, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON0);
		BUTTON_SetBitmapEx(hButton, BUTTON_BI_UNPRESSED, &bmreturnright, 0, 0);
		BUTTON_SetBitmapEx(hButton, BUTTON_BI_PRESSED, &bmreturnbkright, 0, 0);

		
		hButton_0 = BUTTON_CreateUser(540, 213, 162, 162, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON1, 0);
		BUTTON_SetSkin(hButton_0, _Button_PM25_Skin);
		hButton_1 = BUTTON_CreateUser(510, 373, 200, 80, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON2, 0);
		BUTTON_SetSkin(hButton_1, _ButtonSkinAddr);
		
		hButton_2 = BUTTON_CreateUser(540, 680, 162, 162, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON3, 0);
		BUTTON_SetSkin(hButton_2, _Button_CO2_Skin);
		hButton_3 = BUTTON_CreateUser(510, 840, 200, 80, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON4, 0);
		BUTTON_SetSkin(hButton_3, _ButtonSkinAddr);
		
		break;

	case WM_TIMER:
		
		if (g_pageone_control_stu.ButtonPmtimeDelay < 1) {
			if(g_pageone_control_stu.ButtonPmisPress) {
				g_pageone_control_stu.ButtonPmisPress = 0;   // close pm
				g_pageone_control_stu.ButtonPmtimeDelay = 0;
				CGK_cmdswitch = PMCLOSE;
			}
		}else {
			g_pageone_control_stu.ButtonPmtimeDelay--;
		}
		
		
		if (g_pageone_control_stu.ButtonCo2timeDelay < 1) {
			if(g_pageone_control_stu.ButtonCo2isPress) {
				g_pageone_control_stu.ButtonCo2isPress = 0;   // close pm
				g_pageone_control_stu.ButtonCo2timeDelay = 0;
				CGK_cmdswitch = CO2CLOSE;
			}
		}else {
			g_pageone_control_stu.ButtonCo2timeDelay--;
		}
		
		if( (g_pageone_control_stu.ButtonPmisPress == 0) && (g_pageone_control_stu.ButtonCo2isPress == 0) ) {
			g_pageone_control_stu.BUttonElectimeDelay++ ;
			if(g_pageone_control_stu.BUttonElectimeDelay > 10) {
				CGK_cmdswitch = ELECMEASURE;
				g_pageone_control_stu.BUttonElectimeDelay = 0;
			}
		}
		
		WM_InvalidateWindow(hWin);
		WM_RestartTimer(pMsg->Data.v, 0);

		break;
	case WM_PAINT:
		//  display the main sensor data and button
		//GUI_SampleValue_Disp((int)(g_CgkSensorData.uiPm25Ug*1.5f), (int)(g_CgkSensorData.uiPm10Ug*1.5f), g_CgkSensorData.uiCO2Dat,
	GUI_SampleValue_Disp((int)(g_CgkSensorData.uiPm25Ug*1.5f), (int)(g_CgkSensorData.uiPm10Ug*1.5f), (int)g_CgkSensorData.uiUltlDat*2.5f,
			(int)(g_CgkSensorData.uiPm03Dat*1.5f), (int)(g_CgkSensorData.uiPm25Dat*1.5f), (int)(g_CgkSensorData.uiPm10Dat*1.5f),
			g_CgkSensorData.uiElecDat);
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}


static void _cbMenuPageTwoBrowse(WM_MESSAGE * pMsg) {

	WM_HWIN  hButton0;
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
			BUTTON_STATE = BUTTON_PAGE_TWO;
			break;
		default:
			break;
		}
		break;
	case WM_CREATE:
		hButton0 = BUTTON_CreateEx(0, 1090, 360, 110, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON0);
		BUTTON_SetBitmapEx(hButton0, BUTTON_BI_UNPRESSED, &bmreturnleft, 0, 0);
		BUTTON_SetBitmapEx(hButton0, BUTTON_BI_PRESSED, &bmreturnleft, 0, 0);
		break;
	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();

		GUI_SetColor(GUI_WHITE);
		GUI_SetFont(&GUI_Font32B_1);
		GUI_DispStringAt("null", 5, 10);
		break;
	}
}

static void _cbMenuPageTwoUv(WM_MESSAGE * pMsg) {

	WM_HWIN  hButton0;
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
			BUTTON_STATE = BUTTON_PAGE_TWO;
			break;
		default:
			break;
		}
		break;
	case WM_CREATE:
		hButton0 = BUTTON_CreateEx(0, 1090, 360, 110, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON0);
		BUTTON_SetBitmapEx(hButton0, BUTTON_BI_UNPRESSED, &bmreturnleft, 0, 0);
		BUTTON_SetBitmapEx(hButton0, BUTTON_BI_PRESSED, &bmreturnleft, 0, 0);
		break;
	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		GUI_ProgbarCustom_UV(g_CgkSensorData.uiUltlDat, UV_TYPE);
		break;
	}
}


static void _cbMenuPageTwoKnow(WM_MESSAGE * pMsg) {
	WM_HWIN  hButton0;
	WM_HWIN  hButton1;
	WM_HWIN  hButton2;
	WM_HWIN  hButton3;
	WM_HWIN  hButton4;
	WM_HWIN  hButton5;

	int xSize, i, NCode;
	int Id = 0;

	WM_MOTION_INFO * pInfo;
	WM_HWIN hWin, hButton;
	static int IsPressed;

	hWin = pMsg->hWin;
	switch (pMsg->MsgId) {
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);         // Id of widget
		NCode = pMsg->Data.v;                 // Notification code
		switch (Id)
		{
		case GUI_ID_BUTTON0:
			BUTTON_STATE = BUTTON_PAGE_TWO_KONW_PARA;
			break;
		case GUI_ID_BUTTON1:
			BUTTON_STATE = BUTTON_PAGE_TWO;
			break;
		case GUI_ID_BUTTON2:
			BUTTON_STATE = BUTTON_PAGE_TWO_KONW_DUST;
			break;
		case GUI_ID_BUTTON3:
			BUTTON_STATE = BUTTON_PAGE_TWO_KONW_CO2;
			break;
		case GUI_ID_BUTTON4:
			BUTTON_STATE = BUTTON_PAGE_TWO_KONW_ELEC;
			break;
		case GUI_ID_BUTTON5:
			BUTTON_STATE = BUTTON_PAGE_TWO_KONW_UV;
			break;

		default:
			break;
		}
		break;
	case WM_CREATE:
		hButton0 = BUTTON_CreateEx(410, 1020, 261, 87, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON0);
		BUTTON_SetBitmapEx(hButton0, BUTTON_BI_UNPRESSED, &bmiconperformanceparameter, 0, 0);
		BUTTON_SetBitmapEx(hButton0, BUTTON_BI_PRESSED, &bmiconperformanceparameter, 0, 0);

		hButton1 = BUTTON_CreateEx(0, 1020, 360, 110, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON1);
		BUTTON_SetBitmapEx(hButton1, BUTTON_BI_UNPRESSED, &bmreturnleft, 0, 0);
		BUTTON_SetBitmapEx(hButton1, BUTTON_BI_PRESSED, &bmreturnbkleft, 0, 0);


		hButton2 = BUTTON_CreateEx(96, 68, 153, 153, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON2);
		BUTTON_SetBitmapEx(hButton2, BUTTON_BI_UNPRESSED, &bmicondustknowledge, 0, 0);
		BUTTON_SetBitmapEx(hButton2, BUTTON_BI_PRESSED, &bmicondustknowledge, 0, 0);

		hButton3 = BUTTON_CreateEx(96, 301, 153, 153, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON3);
		BUTTON_SetBitmapEx(hButton3, BUTTON_BI_UNPRESSED, &bmiconco2knowledge, 0, 0);
		BUTTON_SetBitmapEx(hButton3, BUTTON_BI_PRESSED, &bmiconco2knowledge, 0, 0);

		hButton4 = BUTTON_CreateEx(96, 537, 153, 153, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON4);
		BUTTON_SetBitmapEx(hButton4, BUTTON_BI_UNPRESSED, &bmiconelecradknowledge, 0, 0);
		BUTTON_SetBitmapEx(hButton4, BUTTON_BI_PRESSED, &bmiconelecradknowledge, 0, 0);

		hButton5 = BUTTON_CreateEx(96, 769, 153, 153, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON5);
		BUTTON_SetBitmapEx(hButton5, BUTTON_BI_UNPRESSED, &bmiconuvknowledge, 0, 0);
		BUTTON_SetBitmapEx(hButton5, BUTTON_BI_PRESSED, &bmiconuvknowledge, 0, 0);

		break;

	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		GUI_SetColor(GUI_WHITE);
		GUI_SetTextMode(GUI_TM_TRANS);
		GUI_SetFont(&GUI_FontPingFangRegular90);
		GUI_UC_SetEncodeUTF8();
		GUI_DispStringAt("\xE7\xB2\x89\xE5\xB0\x98", 276, 105); 			                                    // 粉尘
		GUI_DispStringAt("\xe4\xba\x8c\xe6\xb0\xa7\xe5\x8c\x96\xe7\xa2\xb3", 276, 336); // 二氧化碳
		GUI_DispStringAt("\xE7\x94\xB5\xE7\xA3\x81\xE8\xBE\x90\xE5\xB0\x84", 276, 574); // 电磁辐射
		GUI_DispStringAt("\xe7\xb4\xab\xe5\xa4\x96\xe7\xba\xbf", 276, 792); 	        // 紫外线

		GUI_SetFont(&GUI_FontPingFang67);
		GUI_DispStringAt("\xE7\x9F\xA5\xE8\xAF\x86", 409, 120); 	// 知识		                                    
		GUI_DispStringAt("\xE7\x9F\xA5\xE8\xAF\x86", 541, 354);     // 知识
		GUI_DispStringAt("\xE7\x9F\xA5\xE8\xAF\x86", 541, 592); 	// 知识                                        
		GUI_DispStringAt("\xE7\x9F\xA5\xE8\xAF\x86", 474, 808);     // 知识 
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}


/*********************************************************************
*
*       _cbMenu
*
* Purpose:
*   Callback function of menu window.
*/
static void _cbMenuPageTwo(WM_MESSAGE * pMsg) {
	WM_HWIN  hButton0;
	WM_HWIN  hButton1;
	WM_HWIN  hButton2;
	WM_HWIN  hButton3;
	WM_HWIN  hButton4;
	WM_HWIN  hButton5;
	
	int xSize, i, NCode;
	int Id = 0;

	WM_MOTION_INFO * pInfo;
	WM_HWIN hWin, hButton;
	static int IsPressed;

	hWin = pMsg->hWin;
	switch (pMsg->MsgId) {
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);      // Id of widget
		NCode = pMsg->Data.v;                 // Notification code
		switch (Id)
		{
		case GUI_ID_BUTTON0:
			//WM_DeleteWindow(hWinHeading);
			BUTTON_STATE = BUTTON_PAGE_THREE;
			break;
		case GUI_ID_BUTTON1:
			//WM_DeleteWindow(hWinHeading);
			BUTTON_STATE = BUTTON_PAGE_ONE;
			break;

		case GUI_ID_BUTTON2:
			BUTTON_STATE = BUTTON_PAGE_TWO_UV;
			break;
		case GUI_ID_BUTTON3:
			//BUTTON_STATE = BUTTON_PAGE_TWO_UV;
			break;
		case GUI_ID_BUTTON4:
			BUTTON_STATE = BUTTON_PAGE_TWO_KNOW;
			break;
		case GUI_ID_BUTTON5:
			BUTTON_STATE = BUTTON_PAGE_TWO_BROWSE;
			break;

		default:
			break;
		}
		break;
	case WM_CREATE:
		hButton0 = BUTTON_CreateEx(360, 1090, 360, 110, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON0);
		BUTTON_SetBitmapEx(hButton0, BUTTON_BI_UNPRESSED, &bmreturnright, 0, 0);
		BUTTON_SetBitmapEx(hButton0, BUTTON_BI_PRESSED, &bmreturnbkright, 0, 0);

		hButton1 = BUTTON_CreateEx(0, 1090, 360, 110, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON1);
		BUTTON_SetBitmapEx(hButton1, BUTTON_BI_UNPRESSED, &bmreturnleft, 0, 0);
		BUTTON_SetBitmapEx(hButton1, BUTTON_BI_PRESSED, &bmreturnbkleft, 0, 0);
		

		hButton2 = BUTTON_CreateEx(96, 51, 170, 170, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON2);
		BUTTON_SetBitmapEx(hButton2, BUTTON_BI_UNPRESSED, &bmiconuv, 0, 0);
		BUTTON_SetBitmapEx(hButton2, BUTTON_BI_PRESSED, &bmiconuv, 0, 0);

		hButton3 = BUTTON_CreateEx(96, 289, 170, 170, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON3);
		BUTTON_SetBitmapEx(hButton3, BUTTON_BI_UNPRESSED, &bmiconco2, 0, 0);
		BUTTON_SetBitmapEx(hButton3, BUTTON_BI_PRESSED, &bmiconco2, 0, 0);

		hButton4 = BUTTON_CreateEx(96, 522, 170, 170, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON4);
		BUTTON_SetBitmapEx(hButton4, BUTTON_BI_UNPRESSED, &bmiconknow, 0, 0);
		BUTTON_SetBitmapEx(hButton4, BUTTON_BI_PRESSED, &bmiconknow, 0, 0);

		hButton5 = BUTTON_CreateEx(96, 756, 170, 170, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON5);
		BUTTON_SetBitmapEx(hButton5, BUTTON_BI_UNPRESSED, &bmiconbrowse, 0, 0);
		BUTTON_SetBitmapEx(hButton5, BUTTON_BI_PRESSED, &bmiconbrowse, 0, 0);

		break;

	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		GUI_SetColor(GUI_WHITE);
		GUI_SetTextMode(GUI_TM_TRANS);
		GUI_SetFont(&GUI_FontPingFangRegular90);
		GUI_UC_SetEncodeUTF8();
		GUI_DispStringAt("\xe7\xb4\xab\xe5\xa4\x96\xe7\xba\xbf", 290, 54); 				        // 紫外线
		GUI_DispStringAt("\xe6\xa3\x80\xe6\xb5\x8b", 290, 135); 							    // 检测
		GUI_DispStringAt("\xe4\xba\x8c\xe6\xb0\xa7\xe5\x8c\x96\xe7\xa2\xb3", 290, 284); 	    // 二氧化碳
		GUI_DispStringAt("\xe6\x88\xb7\xe5\xa4\x96\xe6\xa0\xa1\xe5\x87\x86", 290, 364); 	    // 户外校准
		
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}


/*********************************************************************
*
*       _cbMenu
*
* Purpose:
*   Callback function of menu window.
*/
static void _cbMenuPageThree(WM_MESSAGE * pMsg) {
	WM_HWIN hButton0;
	WM_HWIN hButton_1;
	WM_HWIN hButton_2;
	WM_HWIN hButton_3;
	WM_HWIN hButton_4;
	WM_HWIN hButton_5;


	int xSize, userId, NCode;
	int Id = 0;

	WM_MESSAGE 	Msg;
	WM_MOTION_INFO * pInfo;
	WM_HWIN hWin, hButton;
	static int IsPressed;

	hWin = pMsg->hWin;
	switch (pMsg->MsgId) {
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);         // Id of widget
		NCode = pMsg->Data.v;                 // Notification code

		switch (NCode) {
		case WM_NOTIFICATION_CLICKED:
			switch (Id)
			{
			case GUI_ID_BUTTON0:
				
				BUTTON_STATE = BUTTON_PAGE_TWO;
				break;

			case GUI_ID_USER_DUST_SET:
				PagethreeSkinFlag = 0;

				GUI_SetColor(GUI_WHITE);
				GUI_SetTextMode(GUI_TM_TRANS);
				GUI_UC_SetEncodeUTF8();
				GUI_SetFont(&GUI_FontPingFangRegular90);
				GUI_DispStringAt("\xe5\x8f\x82\xe6\x95\xb0\xe8\xae\xbe\xe7\xbd\xae", 30, 15); 				// 参数设置

				GUI_SetFont(&GUI_FontPingFangRegular67);
				GUI_DispStringAt("\xe7\xb2\x89\xe5\xb0\x98\xe8\x87\xaa\xe5\x8a\xa8\xe6\xa3\x80\xe6\xb5\x8b\xe9\x97\xb4\xe9\x9a\x94", 30, 583);

				GUI_SetPenSize(6);
				GUI_AA_FillCircle(65, 685, 10);
				GUI_AA_DrawArc(65, 685, 18, 18, 0, 360);
				GUI_AA_DrawArc(65, 755, 18, 18, 0, 360);
				GUI_AA_DrawArc(65, 825, 18, 18, 0, 360);
				GUI_AA_DrawArc(65, 895, 18, 18, 0, 360);
				break;
			case GUI_ID_USER_SYSTEM_SET:
				PagethreeSkinFlag = 1;
				GUI_SetColor(GUI_WHITE);
				GUI_SetPenSize(6);
				GUI_AA_FillCircle(65, 755, 10);
				GUI_AA_DrawArc(65, 685, 18, 18, 0, 360);
				GUI_AA_DrawArc(65, 755, 18, 18, 0, 360);
				GUI_AA_DrawArc(65, 825, 18, 18, 0, 360);
				GUI_AA_DrawArc(65, 895, 18, 18, 0, 360);
				GUI_SetColor(GUI_BLACK);
				GUI_AA_FillCircle(65, 685, 10);
				GUI_AA_FillCircle(65, 825, 10);
				GUI_AA_FillCircle(65, 895, 10);
				break;
			case GUI_ID_USER_NET_SET:
				PagethreeSkinFlag = 2;
				GUI_SetColor(GUI_WHITE);
				GUI_SetPenSize(6);
				GUI_AA_FillCircle(65, 825, 10);
				GUI_AA_DrawArc(65, 685, 18, 18, 0, 360);
				GUI_AA_DrawArc(65, 755, 18, 18, 0, 360);
				GUI_AA_DrawArc(65, 825, 18, 18, 0, 360);
				GUI_AA_DrawArc(65, 895, 18, 18, 0, 360);
				GUI_SetColor(GUI_BLACK);
				GUI_AA_FillCircle(65, 685, 10);
				GUI_AA_FillCircle(65, 755, 10);
				GUI_AA_FillCircle(65, 895, 10);
				break;
			case GUI_ID_USER_SCREEN_SET:
				PagethreeSkinFlag =  3;
				GUI_SetColor(GUI_WHITE);
				GUI_SetPenSize(6);
				GUI_AA_FillCircle(65, 895, 10);
				GUI_AA_DrawArc(65, 685, 18, 18, 0, 360);
				GUI_AA_DrawArc(65, 755, 18, 18, 0, 360);
				GUI_AA_DrawArc(65, 825, 18, 18, 0, 360);
				GUI_AA_DrawArc(65, 895, 18, 18, 0, 360);
				GUI_SetColor(GUI_BLACK);
				GUI_AA_FillCircle(65, 685, 10);
				GUI_AA_FillCircle(65, 755, 10);
				GUI_AA_FillCircle(65, 825, 10);
				break;
			case GUI_ID_USER_ENGINEER_SET:
				PagethreeSkinFlag =  4;
				break;
			default:
				break;
			}
			WM_InvalidateWindow(hWin);    // paint all window
			break;
		}
		break;
	case WM_CREATE:

		hButton0 = BUTTON_CreateEx(0, 1090, 360, 110, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON0);
		BUTTON_SetBitmapEx(hButton0, BUTTON_BI_UNPRESSED, &bmreturnleft, 0, 0);
		BUTTON_SetBitmapEx(hButton0, BUTTON_BI_PRESSED, &bmreturnbkleft, 0, 0);

		userId = GUI_ID_USER_DUST_SET;
		hButton_1 = BUTTON_CreateUser(0, 100, 720, 70, hWin, WM_CF_SHOW, 0, GUI_ID_USER_DUST_SET, sizeof(userId));
		BUTTON_SetSkin(hButton_1, _ButtonPagethreeSkin);
		BUTTON_SetUserData(hButton_1, &userId, sizeof(userId));

		userId = GUI_ID_USER_SYSTEM_SET;
		hButton_2 = BUTTON_CreateUser(0, 170, 720, 70, hWin, WM_CF_SHOW, 0, GUI_ID_USER_SYSTEM_SET, sizeof(userId));
		BUTTON_SetSkin(hButton_2, _ButtonPagethreeSkin);
		BUTTON_SetUserData(hButton_2, &userId, sizeof(userId));

		userId = GUI_ID_USER_NET_SET;
		hButton_3 = BUTTON_CreateUser(0, 240, 720, 70, hWin, WM_CF_SHOW, 0, GUI_ID_USER_NET_SET, sizeof(userId));
		BUTTON_SetSkin(hButton_3, _ButtonPagethreeSkin);
		BUTTON_SetUserData(hButton_3, &userId, sizeof(userId));

		userId = GUI_ID_USER_SCREEN_SET;
		hButton_4 = BUTTON_CreateUser(0, 310, 720, 70, hWin, WM_CF_SHOW, 0, GUI_ID_USER_SCREEN_SET, sizeof(userId));
		BUTTON_SetSkin(hButton_4, _ButtonPagethreeSkin);
		BUTTON_SetUserData(hButton_4, &userId, sizeof(userId));

		userId = GUI_ID_USER_ENGINEER_SET;
		hButton_5 = BUTTON_CreateUser(0, 380, 720, 70, hWin, WM_CF_SHOW, 0, GUI_ID_USER_ENGINEER_SET, sizeof(userId));
		BUTTON_SetSkin(hButton_5, _ButtonPagethreeSkin);
		BUTTON_SetUserData(hButton_5, &userId, sizeof(userId));

		break;
	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();

		GUI_SetColor(GUI_DARKGRAY);
		GUI_DrawHLine(570, 0, 720);
		GUI_DrawHLine(571, 0, 720);
		GUI_DrawHLine(572, 0, 720);
		GUI_DrawHLine(573, 0, 720);

		
		GUI_SetColor(GUI_WHITE);
		GUI_SetTextMode(GUI_TM_TRANS);
		GUI_UC_SetEncodeUTF8();
		GUI_SetFont(&GUI_Fontydfont62);
		GUI_DispStringAt("\xe5\x8f\x82\xe6\x95\xb0\xe8\xae\xbe\xe7\xbd\xae", 30, 15); 				// 参数设置

		GUI_SetFont(&GUI_Fontydfont47);
		// 粉尘自动检测间隔
		GUI_DispStringAt("\xe7\xb2\x89\xe5\xb0\x98\xe8\x87\xaa\xe5\x8a\xa8\xe6\xa3\x80\xe6\xb5\x8b\xe9\x97\xb4\xe9\x9a\x94", 30, 583);

		GUI_SetPenSize(6);
		GUI_AA_FillCircle(65, 685, 10);
		GUI_AA_DrawArc(65, 685, 18, 18, 0, 360);
		GUI_AA_DrawArc(65, 755, 18, 18, 0, 360);
		GUI_AA_DrawArc(65, 825, 18, 18, 0, 360);
		GUI_AA_DrawArc(65, 895, 18, 18, 0, 360);
		// 手动检测
		GUI_DispStringAt("\xE6\x89\x8B\xE5\x8A\xA8\xE6\xA3\x80\xE6\xB5\x8B", 90, 660);
		// 每天检测1次
		GUI_DispStringAt("\xE6\xAF\x8F\xE5\xA4\xA9\xE6\xA3\x80\xE6\xB5\x8B\xE4\xB8\x80\xE6\xAC\xA1", 90, 730);
		// 每天检测2次
		GUI_DispStringAt("\xE6\xAF\x8F\xE5\xA4\xA9\xE6\xA3\x80\xE6\xB5\x8B\xE4\xBA\x8C\xE6\xAC\xA1", 90, 800);
		// 每天检测4次
		GUI_DispStringAt("\xE6\xAF\x8F\xE5\xA4\xA9\xE6\xA3\x80\xE6\xB5\x8B\xE5\x9B\x9B\xE6\xAC\xA1", 90, 870);
				
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}



/*********************************************************************
*
*       _cbDummy
*
* Purpose:
*   Required for base window and viewport window to make sure WM_DefaultProc() is called.
*/
static void _cbDummy(WM_MESSAGE * pMsg) {
	switch (pMsg->MsgId) {
	default:
		WM_DefaultProc(pMsg);
	}
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       MainTask
*
* Purpose:
*
*/
GUI_PID_STATE state;

void MainTask(void) {

	int xSize, ySize;
	static WM_HWIN hWinBase;     // Parent window for heading and viewport
	static WM_HWIN hWinHeading;  // Heading window
	static WM_HWIN hWinViewport; // Viewport window
	
	GUI_Init();

	//WM_SetCreateFlags(WM_CF_MEMDEV);

	WM_MULTIBUF_Enable(1);

	BUTTON_SetReactOnLevel();
	//
	// Get display dimension
	// 
	xSize = LCD_GetXSize();
	ySize = LCD_GetYSize();
	
	WM_SetSize(WM_HBKWIN, xSize, ySize);
	
	// Create windows

	hWinBase = WM_CreateWindow(0, 0, xSize, ySize, WM_CF_SHOW, _cbDummy, 0);
	
	hWinHeading = WM_CreateWindowAsChild(0, 0, xSize, 80, hWinBase, WM_CF_SHOW, _cbStatus, 0);

	memset(&dev_state, NULL, sizeof(DEV_STATE_CTR_T));

	BUTTON_STATE = BUTTON_PAGE_ONE; // just for testing

	while (1) {

		switch ((int)BUTTON_STATE) {

		case BUTTON_PAGE_ONE:
			WM_DeleteWindow(hWinMenu);
			// Menu window moveable within viewport window
			// 800 * 80
			hWinMenu = WM_CreateWindowAsChild(0, 80, xSize, ySize - 80, hWinBase, WM_CF_SHOW, _cbMenuPageOne, 0);

			BUTTON_STATE = BUTTON_PAGE_IDLE;
			break;

		case BUTTON_PAGE_TWO:
			WM_DeleteWindow(hWinMenu);

			hWinMenu = WM_CreateWindowAsChild(0, 80, xSize, ySize - 80, hWinBase, WM_CF_SHOW, _cbMenuPageTwo, 0);

			BUTTON_STATE = BUTTON_PAGE_IDLE;
			break;

		case BUTTON_PAGE_THREE:
			WM_DeleteWindow(hWinMenu);
			hWinMenu = WM_CreateWindowAsChild(0, 80, xSize, ySize - 80, hWinBase, WM_CF_SHOW, _cbMenuPageThree, 0);

			BUTTON_STATE = BUTTON_PAGE_IDLE;
			break;
		case BUTTON_PAGE_TWO_UV:
			WM_DeleteWindow(hWinMenu);
			hWinMenu = WM_CreateWindowAsChild(0, 80, xSize, ySize - 80, hWinBase, WM_CF_SHOW, _cbMenuPageTwoUv,0);

			BUTTON_STATE = BUTTON_PAGE_IDLE;
			break;
		case BUTTON_PAGE_TWO_KNOW:
			WM_DeleteWindow(hWinMenu);
			hWinMenu = WM_CreateWindowAsChild(0, 80, xSize, ySize - 80, hWinBase, WM_CF_SHOW, _cbMenuPageTwoKnow, 0);

			BUTTON_STATE = BUTTON_PAGE_IDLE;
			break;

		case BUTTON_PAGE_TWO_BROWSE:
			WM_DeleteWindow(hWinMenu);
			hWinMenu = WM_CreateWindowAsChild(0, 80, xSize, ySize - 80, hWinBase, WM_CF_SHOW, _cbMenuPageTwoBrowse, 0);

			BUTTON_STATE = BUTTON_PAGE_IDLE;
			break;
		case BUTTON_PAGE_INSERADDR:
			WM_DeleteWindow(hWinMenu);
			hWinMenu = WM_CreateWindowAsChild(0, 80, xSize, ySize - 80, hWinBase, WM_CF_SHOW, _cbMenuPageTwoInsertAddr, 0);

			BUTTON_STATE = BUTTON_PAGE_IDLE;
			break;
		case BUTTON_PAGE_IDLE:
			break;
		}

		GUI_Delay(70);
	}
}



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
#include "SLIDER.h"
#include "public_h.h"
#include "bsp_board_init.h"

/*********************************************************************
*
*       Static (const) data
*
**********************************************************************
*/
/*********************************************************************
*
*       
*/
static void _cbMenu(WM_MESSAGE * pMsg);
static void _CreateSelected(int Index, WM_HWIN hWin);
extern void _cbButtonAndEdit(WM_MESSAGE * pMsg);
static void WM_Custom_DeleteButtonWindow(void);
static void WM_Custom_DeleteEnWindow(void);
static void App_EnSetCtr(WM_HWIN hWin);
extern int GUI_mutex_take(void);
extern int GUI_mutex_release(void);
extern int GUI_Button_mutex_take(void);
extern int GUI_Button_mutex_release(void);

/*#####################################################################################*/
extern GUI_CONST_STORAGE GUI_FONT GUI_Fonthanyi60;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fonthanyi48;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fonthanyi43;
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
/*#####################################################################################*/
extern GUI_CONST_STORAGE GUI_BITMAP bmuv;
extern GUI_CONST_STORAGE GUI_BITMAP bmco2;
extern GUI_CONST_STORAGE GUI_BITMAP bmknow;
extern GUI_CONST_STORAGE GUI_BITMAP bmlook;

#define ID_TIMER_TIME       1
#define ID_TIMER_CHARGE     2

/*********************************************************************
*
*       _aMenu
*/
typedef enum {
	BUTTON_PAGE_ONE     	= 0,
	BUTTON_PAGE_TWO_L,
	BUTTON_PAGE_TWO_R,
	BUTTON_PAGE_THREE_L,
	BUTTON_PAGE_THREE_R,
	BUTTON_PAGE_FOUR_L,
	BUTTON_PAGE_FOUR_R,
	BUTTON_PAGE_FIVE,
	BUTTON_PAGE_IDLE,
} BUTTON_STATE_T;

BUTTON_STATE_T BUTTON_STATE = BUTTON_PAGE_ONE;

DEV_STATE_CTR_T dev_state; 
UI_CTR_t uiWinCtl;

GUI_RECT Rect_0 = {20,   90, 480, 130};
GUI_RECT Rect_1 = {150, 160, 300, 300};
GUI_RECT Rect_2 = {150, 380, 300, 500};
GUI_RECT Rect_3 = {20,  560, 400, 753};
GUI_RECT Rect_4 = {380, 400, 479, 450};

#define true    (1U)
#define false   (0U)

#define GUI_ID_USER_BUTTON_0        (GUI_ID_USER + 0x01)
#define GUI_ID_USER_BUTTON_1        (GUI_ID_USER + 0x0A)
#define GUI_ID_USER_BUTTON_2        (GUI_ID_USER + 0x0C)
#define GUI_ID_USER_CLOSE 	        (GUI_ID_BUTTON0 + 1000)
#define GUI_ID_USER_OK 		        (GUI_ID_BUTTON0 + 1001)
#define GUI_ID_USER_CANCEL          (GUI_ID_BUTTON0 + 1002)
#define GUI_ID_USER_PM 		        (GUI_ID_BUTTON0 + 1003)
#define GUI_ID_USER_SYS 	        (GUI_ID_BUTTON0 + 1004)
#define GUI_ID_USER_SET             (GUI_ID_BUTTON0 + 1007)
#define GUI_ID_USER_DET_M           (GUI_ID_BUTTON0 + 1008)
#define GUI_ID_USER_DET_1           (GUI_ID_BUTTON0 + 1009)
#define GUI_ID_USER_DET_2           (GUI_ID_BUTTON0 + 1010)
#define GUI_ID_USER_DET_4           (GUI_ID_BUTTON0 + 1011)
#define GUI_ID_USER_EDIT0           (GUI_ID_BUTTON0 + 1012)
#define GUI_ID_USER_EDIT1           (GUI_ID_BUTTON0 + 1013)
#define GUI_ID_USER_EDIT2           (GUI_ID_BUTTON0 + 1014)
#define GUI_ID_USER_1_TS            (GUI_ID_BUTTON0 + 1015)
#define GUI_ID_USER_2_0_TS          (GUI_ID_BUTTON0 + 1016)
#define GUI_ID_USER_2_1_TS          (GUI_ID_BUTTON0 + 1017)
#define GUI_ID_USER_4_0_TS          (GUI_ID_BUTTON0 + 1018)
#define GUI_ID_USER_4_1_TS          (GUI_ID_BUTTON0 + 1019)
#define GUI_ID_USER_4_2_TS          (GUI_ID_BUTTON0 + 1020)
#define GUI_ID_USER_4_3_TS          (GUI_ID_BUTTON0 + 1021)
#define GUI_ID_USER_EDIT3           (GUI_ID_BUTTON0 + 1022)
#define GUI_ID_USER_EDIT4           (GUI_ID_BUTTON0 + 1023)
#define GUI_ID_USER_EDIT5           (GUI_ID_BUTTON0 + 1024)
#define GUI_ID_USER_EDIT6           (GUI_ID_BUTTON0 + 1025)
#define GUI_ID_USER_VER             (GUI_ID_BUTTON0 + 1005)
#define GUI_ID_USER_LAG 	        (GUI_ID_BUTTON0 + 1006)
#define GUI_ID_USER_PMC 	        (GUI_ID_BUTTON0 + 1026)
#define GUI_ID_USER_BACK 	        (GUI_ID_BUTTON0 + 1027)
#define GUI_ID_USER_FAC 	        (GUI_ID_BUTTON0 + 1028)
#define GUI_ID_USER_DONE 	        (GUI_ID_BUTTON0 + 1029)
#define GUI_ID_USER_PRE 	        (GUI_ID_BUTTON0 + 1030)
#define GUI_ID_USER_FAC_YES 	    (GUI_ID_BUTTON0 + 1031)
#define GUI_ID_USER_FAC_NO 	        (GUI_ID_BUTTON0 + 1032)

#define GUI_ID_USER_NPOL 	        (GUI_ID_BUTTON0 + 1033)
#define GUI_ID_USER_LPOL 	        (GUI_ID_BUTTON0 + 1034)
#define GUI_ID_USER_MPOL 	        (GUI_ID_BUTTON0 + 1035)
#define GUI_ID_USER_HPOL 	        (GUI_ID_BUTTON0 + 1036)
#define GUI_ID_USER_CHPOL 	        (GUI_ID_BUTTON0 + 1037)
#define GUI_ID_USER_RCHPOL 	        (GUI_ID_BUTTON0 + 1038)

#define MSG_CHANGE_M                (WM_USER + 0)
#define MSG_CHANGE_1                (WM_USER + 1)
#define MSG_CHANGE_2                (WM_USER + 2)
#define MSG_CHANGE_3                (WM_USER + 3)
#define MSG_WM_CREAT_USR            (WM_USER + 4)


unsigned short BatteryIndex[] = {0xEBA0,0xEBA1,0xEBA2,0xEBA3,0xEBA4,0xEBA5,0xEBA6,0xEBA7,
								 0xEBA8,0xEBA9,0xEBAA,0xEBAB,0xEBAC,0xEBAD,0xEBAE,0xEBAF,
								 0xEBB0,0xEBB1,0xEBB2,0xEBB3,0xEBB4,0xEBB5};


static const char ucWeekDay[7][3] =
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
  {  0,   0 },
  { 29,  29 },
  { 29,  16 },
  { 68,  16 },
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


static const BITMAP_ITEM _aBitmapItem[] = {
  {&bmuv, 		"" , 	""},
  {&bmco2,   	"" , 	""},
  {&bmknow,   	"" , 	""},
  {&bmlook,   	"" , 	""},
};

#define ICONVIEW_TBorder   60   		/* ¿Ø¼þICONVIEWµÄÉÏ±ß¾à */
#define ICONVIEW_LBorder   95   		/* ¿Ø¼þICONVIEWµÄ×ó±ß¾à */  
#define ICONVIEW_Width     173  		/* ¿Ø¼þICONVIEWµÄ¿í */  
#define ICONVIEW_Height    173  		/* ¿Ø¼þICONVIEWµÄ¸ß */  
#define ICONVIEW_YSpace    61   		/* ¿Ø¼þICONVIEWµÄY·½Ïò¼ä¾à */
#define ICONVIEW_XSpace    61    		/* ¿Ø¼þICONVIEWµÄX·½Ïò¼ä¾à */
#define ICON_Width     	   173      	/* ¿Ø¼þICONVIEWµÄÍ¼±êµÄ¿í¶È */  
#define ICON_Height    	   173       	/* ¿Ø¼þICONVIEWµÄÍ¼±êµÄ¸ß¶È, º¬Í¼±êÏÂÃæµÄÎÄ±¾£¬Ò»ÆðµÄ¸ß¶È */ 

static WM_HWIN hIcon;
static WM_HWIN hButtonCancel;
static WM_HWIN hButtonOK;
static WM_HWIN hButtonClose;
static WM_HWIN hButtonPMSet 	= NULL;
static WM_HWIN hButtonSysSet 	= NULL;
static WM_HWIN hButtonEnSet	    = NULL;
static WM_HWIN hWinTimSet	    = NULL;
static WM_HWIN hWinVer	        = NULL;
static WM_HWIN hWinVerFac	    = NULL;
static WM_HWIN hWinLag	        = NULL;
static WM_HWIN hWinPmc	        = NULL;
static GUI_HMEM hQR;

/*
*********************************************************************************************************
*	º¯ Êý Ãû: Caculate_RTC
*	¹¦ÄÜËµÃ÷: ÏÔÊ¾RTCÊ±¼ä
*	ÐÎ    ²Î: pMsg  WM_MESSAGEÀàÐÍÖ¸Õë±äÁ¿           
*	·µ »Ø Öµ: ÎÞ
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
  static int Time      = 9 * 60;
  static int Charge    = GUI_COUNTOF(BatteryIndex);
  WM_HWIN    hWin;
  static     WM_HTIMER hTimerTime;
  static     WM_HTIMER hTimerCharge;
  int        xSize;
  int        ySize;
  int        Id;
  int        i;
  int        xPos;
  char 		   buf[30];

  hWin      = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_PRE_PAINT:
    /*GUI_MULTIBUF_Begin();*/
    break;
  case WM_POST_PAINT:
    /*GUI_MULTIBUF_End();*/
    break;
  case WM_CREATE:
    hTimerTime      = WM_CreateTimer(hWin, ID_TIMER_TIME,      1000, 0);
    hTimerCharge    = WM_CreateTimer(hWin, ID_TIMER_CHARGE,    2100, 0);
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

	GUI_DispStringAt(buf,26,10);
	
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
  	bat_percent = (int)( g_AdcVatData.fVoltageValue*10000/57.0 - 268 ); 
  	if( bat_percent < 0 ) {
			bat_percent = 0;
		}else if(bat_percent >= 100){
			bat_percent = 99;
		}
		if(bat_percent < 10) {
			sprintf(buf, "%0.1d", bat_percent);
		}else {
			sprintf(buf, "%0.2d", bat_percent);
		}
	
		GUI_DispStringAt(buf,550,10);
		GUI_DispStringAt("\x25",592,10); // "%"
	
    GUI_SetFont(&GUI_FontHoloLensMDL2Assets48);
	  GUI_SetColor(GUI_WHITE);
	  GUI_DispCharAt(BatteryIndex[0], 630, 10);  // æ¡†
//    if (Charge) {
    GUI_DispCharAt(BatteryIndex[bat_percent/10], 630, 10);  // ç”µé‡
//    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}


/*********************************************************************
*
*       _ButtonSkin
*
* Purpose:
*   Skinning function of a single button. Responsible for custom drawing only.
*/
static int _ButtonSkin(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) {
  int Index, xSize, ySize, IsPressed;
  WM_HWIN hWin;
  const GUI_BITMAP * pBm;
  GUI_COLOR Color;
  U16 RightArrow[] = {0xE76C};

  hWin = pDrawItemInfo->hWin;
  switch (pDrawItemInfo->Cmd) {
  case WIDGET_ITEM_DRAW_BACKGROUND:
    //
    // Get state of button
    //
    IsPressed = BUTTON_IsPressed(pDrawItemInfo->hWin);
    //
    // Get window dimension
    //
    xSize = WM_GetWindowSizeX(hWin);
    ySize = WM_GetWindowSizeY(hWin);
    //
    // Get button index
    //
    BUTTON_GetUserData(hWin, &Index, sizeof(Index));
    //
    // Draw background color depending on state
    //
    if (IsPressed) {
      Color = GUI_DARKGRAY;
    } else {
      Color = GUI_BLACK;
    }
    GUI_SetBkColor(Color);
    GUI_Clear();
    //
    // Draw button text
    //
    GUI_SetColor(GUI_MAGENTA);
    GUI_SetFont(&GUI_FontDingbats2114);
    GUI_SetTextAlign(GUI_TA_LEFT | GUI_TA_VCENTER);
    GUI_DispCharAt(0x007c, 600, 0);
    break;
  }
  return 0;
}

static int _ButtonSkinPageTwoRight(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) {
  int Index, xSize, ySize, IsPressed;
  WM_HWIN hWin;
  const GUI_BITMAP * pBm;
  GUI_COLOR Color;
  U16 RightArrow[] = {0xE76C};

  hWin = pDrawItemInfo->hWin;
  switch (pDrawItemInfo->Cmd) {
  case WIDGET_ITEM_DRAW_BACKGROUND:
    //
    // Get state of button
    //
    IsPressed = BUTTON_IsPressed(pDrawItemInfo->hWin);
    //
    // Get window dimension
    //
    xSize = WM_GetWindowSizeX(hWin);
    ySize = WM_GetWindowSizeY(hWin);
    //
    // Get button index
    //
    BUTTON_GetUserData(hWin, &Index, sizeof(Index));
    //
    // Draw background color depending on state
    //
    if (IsPressed) {
      Color = GUI_DARKGRAY;
    } else {
      Color = GUI_BLACK;
    }
    GUI_SetBkColor(Color);
    GUI_Clear();
    //
    // Draw arrow at the right
    //
    GUI_SetColor(GUI_MAGENTA);
    GUI_SetFont(&GUI_FontDingbats2114);
    GUI_SetTextAlign(GUI_TA_LEFT | GUI_TA_VCENTER);
    GUI_DispCharAt(0x007c, 270, 0);
    break;
  }
  return 0;
}

static int _ButtonSkinPageTwoLeft(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) {
  int Index, xSize, ySize, IsPressed;
  WM_HWIN hWin;
  const GUI_BITMAP * pBm;
  GUI_COLOR Color;
  U16 LeftArrow[] = {0xE76B};

  hWin = pDrawItemInfo->hWin;
  switch (pDrawItemInfo->Cmd) {
  case WIDGET_ITEM_DRAW_BACKGROUND:
    //
    // Get state of button
    //
    IsPressed = BUTTON_IsPressed(pDrawItemInfo->hWin);
    //
    // Get window dimension
    //
    xSize = WM_GetWindowSizeX(hWin);
    ySize = WM_GetWindowSizeY(hWin);
    //
    // Get button index
    //
    BUTTON_GetUserData(hWin, &Index, sizeof(Index));
    //
    // Draw background color depending on state
    //
    if (IsPressed) {
      Color = GUI_DARKGRAY;
    } else {
      Color = GUI_BLACK;
    }
    GUI_SetBkColor(Color);
    GUI_Clear();
    //
    // Draw arrow at the right
    //
    GUI_SetColor(GUI_MAGENTA);
    GUI_SetTextAlign(GUI_TA_LEFT | GUI_TA_VCENTER);
	GUI_FillPolygon (&_aPointArrow[0], 7, 30, 40);
    break;
  }
  return 0;
}

static int _ButtonSkinPageThreeLeft(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) {
  int Index, xSize, ySize, IsPressed;
  WM_HWIN hWin;
  const GUI_BITMAP * pBm;
  GUI_COLOR Color;
  U16 LeftArrow[] = {0xE76B};

  hWin = pDrawItemInfo->hWin;
  switch (pDrawItemInfo->Cmd) {
  case WIDGET_ITEM_DRAW_BACKGROUND:
    //
    // Get state of button
    //
    IsPressed = BUTTON_IsPressed(pDrawItemInfo->hWin);
    //
    // Get window dimension
    //
    xSize = WM_GetWindowSizeX(hWin);
    ySize = WM_GetWindowSizeY(hWin);
    //
    // Get button index
    //
    BUTTON_GetUserData(hWin, &Index, sizeof(Index));
    //
    // Draw background color depending on state
    //
    if (IsPressed) {
      Color = GUI_DARKGRAY;
    } else {
      Color = GUI_BLACK;
    }
    GUI_SetBkColor(Color);
    GUI_Clear();

    //
    // Draw arrow at the right
    //
    GUI_SetColor(GUI_MAGENTA);
    GUI_SetTextAlign(GUI_TA_LEFT | GUI_TA_VCENTER);
	GUI_FillPolygon (&_aPointArrow[0], 7, 30, 40);
    break;
  }
  return 0;
}



static int _ButtonSkinPageThreeSet(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) {
  int Index,IsPressed;
  WM_HWIN hWin;
  GUI_COLOR Color;
  
  hWin = pDrawItemInfo->hWin;
  switch (pDrawItemInfo->Cmd) {
  case WIDGET_ITEM_DRAW_BACKGROUND:
    //
    // Get state of button
    //
    IsPressed = BUTTON_IsPressed(pDrawItemInfo->hWin);
    //
    // Get button index
    //
    BUTTON_GetUserData(hWin, &Index, sizeof(Index));
    //
    // Draw background color depending on state
    //
    if (IsPressed) {
      Color = GUI_MAGENTA;
    } else {
      Color = GUI_BLACK;
    }
    GUI_SetBkColor(Color);
    GUI_Clear();
    //
    // Draw arrow at the right
    //
    GUI_SetColor(GUI_WHITE);
	GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetFont(&GUI_FontPingFangRegular67);
	GUI_UC_SetEncodeUTF8();
	if(Index == GUI_ID_USER_PM)
		GUI_DispStringAt("\xe7\xb2\x89\xe5\xb0\x98\xe8\x87\xaa\xe5\x8a\xa8\xe6\xa3\x80\xe6\xb5\x8b\xe9\x97\xb4\xe9\x9a\x94",100,10); // ·Û³¾×Ô¶¯¼ì²â¼ä¸ô
	else if(Index == GUI_ID_USER_SYS)
		GUI_DispStringAt("\xe7\xb3\xbb\xe7\xbb\x9f\xe8\xae\xbe\xe7\xbd\xae",100,10); 				// ÏµÍ³ÉèÖÃ
	else if(Index == GUI_ID_USER_SET)
		GUI_DispStringAt("\xe5\xb7\xa5\xe7\xa8\x8b\xe5\xb8\x88\xe8\xae\xbe\xe7\xbd\xae",100,10); 	// ¹¤³ÌÊ¦ÉèÖÃ
    break;
  }
  return 0;
}

static int _ButtonSkinPageEnSetup(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) {
  int Index,IsPressed;
  WM_HWIN hWin;
  GUI_COLOR Color;
  
  hWin = pDrawItemInfo->hWin;
  switch (pDrawItemInfo->Cmd) {
  case WIDGET_ITEM_DRAW_BACKGROUND:
    //
    // Get state of button
    //
    IsPressed = BUTTON_IsPressed(pDrawItemInfo->hWin);
    //
    // Get button index
    //
    BUTTON_GetUserData(hWin, &Index, sizeof(Index));
    //
    // Draw background color depending on state
    //
    if (IsPressed) {
      Color = GUI_MAGENTA;
    } else {
      Color = GUI_BLACK;
    }
    GUI_SetBkColor(Color);
    GUI_Clear();
    //
    // Draw arrow at the right
    //
    GUI_SetColor(GUI_WHITE);
	GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetFont(&GUI_Fonthanyi48);
	GUI_UC_SetEncodeUTF8();
	if(Index == GUI_ID_USER_VER)
		GUI_DispStringAt("\xe7\x89\x88\xe6\x9c\xac\xe4\xbf\xa1\xe6\x81\xaf",100,10); // °æ±¾ÐÅÏ¢
	else if(Index == GUI_ID_USER_LAG)
		GUI_DispStringAt("\xe8\xbf\x9c\xe7\xa8\x8b\xe8\xae\xbe\xe7\xbd\xae",100,10); // Ô¶³ÌÉèÖÃ
	else if(Index == GUI_ID_USER_PMC)
		GUI_DispStringAt("\xe7\xb2\x89\xe5\xb0\x98\xe6\xa3\x80\xe6\xb5\x8b\xe6\xac\xa1\xe6\x95\xb0",100,10); // ·Û³¾¼ì²â´ÎÊý
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
    //
    // Get state of button
    //
    IsPressed = BUTTON_IsPressed(pDrawItemInfo->hWin);
    //
    // Get window dimension
    //
    xSize = WM_GetWindowSizeX(hWin);
    ySize = WM_GetWindowSizeY(hWin);

    //
    // Draw background color depending on state
    //

    if (dev_state.IsPM25Pressed) {
      Color = GUI_DARKGRAY;
    } else {
      Color = GUI_MAGENTA;
    }
	
    //GUI_SetBkColor(Color);
    GUI_Clear();
    //
    // Draw antialiased rounded rectangle as background for image
    //
    GUI_SetColor(Color);
    GUI_AA_FillCircle(80,80,75);
		//
    // Draw button text
    //
    GUI_SetColor(GUI_WHITE);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(&GUI_FontPingFangRegular67);
	GUI_DispStringAt("\xe9\xa9\xac\xe4\xb8\x8a", 30, 20); // 
	GUI_DispStringAt("\xe6\xa3\x80\xe6\xb5\x8b", 30, 75); // 

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
    //
    // Get state of button
    //
    IsPressed = BUTTON_IsPressed(pDrawItemInfo->hWin);
    //
    // Get window dimension
    //
    xSize = WM_GetWindowSizeX(hWin);
    ySize = WM_GetWindowSizeY(hWin);

    //
    // Draw background color depending on state
    //

    if (dev_state.IsCO2Pressed) {
      Color = GUI_DARKGRAY;
    } else {
      Color = GUI_MAGENTA;
    }
	
    //GUI_SetBkColor(Color);
    GUI_Clear();
    //
    // Draw antialiased rounded rectangle as background for image
    //
    GUI_SetColor(Color);
		GUI_AA_FillCircle(80,80,75);
		//
    // Draw button text
    //
    GUI_SetColor(GUI_WHITE);
		GUI_SetTextMode(GUI_TM_TRANS);
		GUI_SetFont(&GUI_FontPingFangRegular67);
		GUI_DispStringAt("\xe9\xa9\xac\xe4\xb8\x8a", 30, 20); // ÂíÉÏ
		GUI_DispStringAt("\xe6\xa3\x80\xe6\xb5\x8b", 30, 75); // ¼ì²â
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
    //
    // Get state of button
    //
    IsPressed = BUTTON_IsPressed(pDrawItemInfo->hWin);
    //
    // Get window dimension
    //
    xSize = WM_GetWindowSizeX(hWin);
    ySize = WM_GetWindowSizeY(hWin);

    //
    // Draw background color depending on state
    //

    if (IsPressed) {
      Color = GUI_DARKGRAY;
    } else {
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
	GUI_SetFont(&GUI_FontPingFangRegular67);
	GUI_DispStringAt("\xe8\xbe\x93\xe5\x85\xa5\xe5\x9c\xb0\xe5\x9d\x80", 5, 10); // ÊäÈëµØÖ·

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

static int XPOS_OFFSET 			= 50;
static int XPOS_OFFSET_VAR 		= 250;
static int YPOS_OFFSET 			= 180;
static int YPOS_OFFSET_GRAD 	= 80;
static int YPOS_OFFSET_PROG 	= 100;

#define CO2_TYPE   	0
#define PM25_TYPE  	1
#define EMR_TYPE   	2
#define UV_TYPE   	3

static void  _CreateProgressBar(int xPos0, int yPos0, int xPos1, int yPos1,int r ,int x_offset) {

  GUI_SetColor(0x62bb72);  // green
  GUI_AA_FillRoundedRect(xPos0, yPos0, xPos0 + (xPos1 - xPos0) / 3 + 12, yPos1, r);
  GUI_SetColor(0x2745e6);  // red
  GUI_AA_FillRoundedRect(xPos0 +  ((xPos1 - xPos0) * 2)  / 3 - 12, yPos0, xPos1, yPos1, r);
  GUI_SetColor(0x2Aaef3);  // orenge
  GUI_FillRect(xPos0 + (xPos1 - xPos0) / 3, yPos0,xPos0 +  ((xPos1 - xPos0) * 2)  / 3, yPos1);

  if(x_offset >= yPos1 - yPos0)
  	 x_offset -= 25;
  
  GUI_SetColor(GUI_WHITE);	
  GUI_AA_FillRoundedRect(xPos0 + x_offset, yPos0 - (70 - (yPos1 - yPos0)) / 2, xPos0 + 25 + x_offset, yPos1 + (70 - (yPos1 - yPos0)) / 2, 12);
}

static void  _CreateProgressBarligttle(int xPos0, int yPos0, int xPos1, int yPos1, int r ,int x_offset) {

  GUI_SetColor(0x62bb72);
  GUI_AA_FillRoundedRect(xPos0,yPos0,xPos0 + (xPos1 - xPos0) / 3 + 10, yPos1, r);
  GUI_SetColor(0x2745e6);
  GUI_AA_FillRoundedRect(xPos0 +  ((xPos1 - xPos0) * 2)  / 3 - 10, yPos0, xPos1, yPos1, r);
  GUI_SetColor(0x34b1f4);
  GUI_FillRect(xPos0 + (xPos1 - xPos0) / 3,yPos0,xPos0 +  ((xPos1 - xPos0) * 2)  / 3, yPos1);

  if(x_offset >= yPos1 - yPos0)
  	 x_offset -= 14;
  	
  GUI_SetColor(GUI_WHITE);
  GUI_AA_FillRoundedRect(xPos0 + x_offset, yPos0 - (43 - (yPos1 - yPos0)) / 2, xPos0 + 14 + x_offset, yPos1 + (43 - (yPos1 - yPos0)) / 2, 7);
}

static int GUI_ProgbarCustom_UV(int SampleVal,int Sensor)
{
	static int x_offset = 0;
	
	if(Sensor == UV_TYPE)
		x_offset = SampleVal;
	else
		 return 0;
		
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetColor(GUI_WHITE);

	GUI_SetFont(&GUI_FontHoloLensMDL2Assets133);
	GUI_DispCharAt(0xE706,530,60); // SUN
	GUI_SetFont(&GUI_FontPingFangRegular90);
	GUI_UC_SetEncodeUTF8();
	GUI_DispStringAt("\xe7\xb4\xab\xe5\xa4\x96\xe7\xba\xbf",30,75); // 

	GUI_SetColor(0xff8000);
	GUI_FillRect(0,250,720,650);

	GUI_SetColor(GUI_WHITE);
	if(SampleVal < 2) {
		GUI_SetFont(&GUI_FontPingFangRegular135);
		GUI_DispStringAt("\xe9\x9b\xb6\xe7\xba\xa7",30,450); // 
		GUI_SetFont(&GUI_FontPingFangRegular52);
		GUI_DispStringAt("\xe6\x97\xa0\xe5\x8d\xb1\xe5\xae\xb3",341,280); // 
		
	}
	else if(SampleVal <= 3 && SampleVal >= 2) {
		GUI_SetFont(&GUI_FontPingFangRegular135);
		GUI_DispStringAt("\xe4\xb8\x80\xe7\xba\xa7",30,450); // 
		GUI_SetFont(&GUI_FontPingFangRegular52);
		GUI_DispStringAt("\xe8\xbd\xbb\xe5\x8d\xb1\xe5\xae\xb3",341,280); // 
	}
	else if(SampleVal > 4) {
		GUI_SetFont(&GUI_FontPingFangRegular135);
		GUI_DispStringAt("\xe4\xba\x8c\xe7\xba\xa7",30,450); // 
		GUI_SetFont(&GUI_FontPingFangRegular52);
		GUI_DispStringAt("\xe5\x8d\xb1\xe5\xae\xb3",341,280); // 
	}

	_CreateProgressBarligttle(30,290,321,318,12,x_offset);
	
	return 0;
}

static int GUI_ProgbarCustom_PM(int SampleVal,int Sensor)
{
	static int x_offset = 0;
	
	if(Sensor == PM25_TYPE)
		x_offset = (490 * SampleVal) / 490;
	else
		 return 0;
		
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetColor(GUI_WHITE);
	GUI_SetFont(&GUI_FontPingFangRegular67);
	GUI_UC_SetEncodeUTF8();

	if(SampleVal < 30) 
		GUI_DispStringAt("\xe6\x97\xa0\xe6\xb1\xa1\xe6\x9f\x93",540,80); // ÎÞÎÛÈ¾
	else if(SampleVal <= 60 && SampleVal >= 30) 
		GUI_DispStringAt("\xe4\xb8\xad\xe6\xb1\xa1\xe6\x9f\x93",540,80); // ÖÐÎÛÈ¾
	else if(SampleVal > 60) 
		GUI_DispStringAt("\xe9\x87\x8d\xe6\xb1\xa1\xe6\x9f\x93",540,80); // ÖØÎÛÈ¾

	GUI_DispStringAt("PM2.5\xe7\xb2\x89\xe5\xb0\x98",30,10); // PM2.5·Û³¾
	GUI_DispStringAt("\xe5\xbe\xae\xe5\x85\x8b/\xe7\xab\x8b\xe6\x96\xb9\xe7\xb1\xb3 \x28        \x29",30,157); // Î¢¿Ë/Á¢·½Ã× (ug / m3)

	GUI_UC_SetEncodeNone();
	GUI_DispStringAt("\xB5g/",326,157); // ug/
	GUI_DispCharAt(0x33a5,399,157); 	// "m3"

	GUI_SetFont(&GUI_FontPingFangRegular41);
	GUI_UC_SetEncodeUTF8();
	// ¿ÉÎüÈë¿ÅÁ£Îï
	GUI_DispStringAt("\xe5\x8f\xaf\xe5\x90\xb8\xe5\x85\xa5\xe9\xa2\x97\xe7\xb2\x92\xe7\x89\xa9",280,27);
	
	_CreateProgressBar(30,90,520,130,17,x_offset);
	
	return 0;
}

static int GUI_ProgbarCustom_CO2(int SampleVal,int Sensor)
{
	char buf[32];
	static int x_offset = 0;
	
	if(Sensor == CO2_TYPE)
		x_offset = (291 * SampleVal) / 5000;
	else 
		return 0;

	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetColor(GUI_WHITE);
	GUI_SetFont(&GUI_FontPingFangRegular67);
	GUI_UC_SetEncodeUTF8();
	GUI_DispStringAt("\xe4\xba\x8c\xe6\xb0\xa7\xe5\x8c\x96\xe7\xa2\xb3 CO",30,730); // ¶þÑõ»¯Ì¼
	GUI_UC_SetEncodeNone();
	GUI_DispCharAt(0x2082,304,728); // SUBSCRIPT 2

	GUI_SetFont(&GUI_FontPingFangRegular52);
	GUI_UC_SetEncodeUTF8();
	if(SampleVal < 1200) 	
		GUI_DispStringAt("\xe4\xb8\x8d\xe7\xbc\xba\xe6\xb0\xa7",348,800); // ²»È±Ñõ
	else if(SampleVal >= 1200 && SampleVal <= 2400) 
		GUI_DispStringAt("\xe5\xbe\xae\xe7\xbc\xba\xe6\xb0\xa7",348,800); // Î¢È±Ñõ
	else if(SampleVal > 2400 && SampleVal <= 4000) 
		GUI_DispStringAt("\xe7\xbc\xba\xe6\xb0\xa7",348,800); // È±Ñõ
	else if(SampleVal > 4000) 
		GUI_DispStringAt("\xe9\x87\x8d\xe7\xbc\xba\xe6\xb0\xa7",348,800); // ÖØÈ±Ñõ

	sprintf(buf, "%d ppm", SampleVal);
	GUI_DispStringAt(buf,348,740); // ppm

	_CreateProgressBarligttle(30,814,321,842,12,x_offset);
	
	return 0;
}

static int GUI_ProgbarCustom_EMR(int SampleVal,int Sensor)
{
	char buf[32];
	static int x_offset = 0;
	
	if(Sensor == EMR_TYPE)
		x_offset = (291 * SampleVal) / 500;
	else 
		return 0;

	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetColor(GUI_WHITE);
	GUI_SetFont(&GUI_FontPingFangRegular67);
	GUI_UC_SetEncodeUTF8();
	GUI_DispStringAt("\xe7\x94\xb5\xe7\xa3\x81\xe8\xbe\x90\xe5\xb0\x84",30,930); // µç´Å·øÉä

	GUI_SetFont(&GUI_FontPingFangRegular52);
	if(SampleVal < 50) 	
		GUI_DispStringAt("\xe6\x97\xa0\xe6\xb1\xa1\xe6\x9f\x93",348,1003); // ÎÞÎÛÈ¾
	else if(SampleVal >= 50 && SampleVal <= 120) 
		GUI_DispStringAt("\xe4\xb8\xad\xe6\xb1\xa1\xe6\x9f\x93",348,1003); // ÖÐÎÛÈ¾
	else if(SampleVal > 120) 
		GUI_DispStringAt("\xe9\x87\x8d\xe6\xb1\xa1\xe6\x9f\x93",348,1003); // ÖØÎÛÈ¾

	sprintf(buf, "%d ", SampleVal);
	GUI_DispStringAt(buf,348,940);
	GUI_DispStringAt("W",430,940);
	GUI_UC_SetEncodeNone();
	GUI_DispCharAt(0xb5,405,940);

	GUI_SetFont(&GUI_FontPingFangRegular41);
	GUI_UC_SetEncodeUTF8();
	GUI_DispStringAt("\xe5\xae\x9e\xe6\x97\xb6",600,1023); // ÊµÊ±

	_CreateProgressBarligttle(30,1014,321,1042,12,x_offset);
	
	return 0;
}


static void GUI_SampleValue_Disp(int PM25_Value,int PM10_Value,int CO2_Value,
											  int PM03_Count,int PM25_Count,int PM10_Count,
											  int ELEC_Value)
{
	char buf[32];
	
	GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();

	GUI_SetColor(GUI_WHITE);
    GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(&GUI_FontPingFangRegular41);
	GUI_UC_SetEncodeUTF8();
	// ¼¤¹â¸öÊý
	GUI_DispStringAt("\xe6\xbf\x80\xe5\x85\x89\xe4\xb8\xaa\xe6\x95\xb0 \xe4\xb8\xaa/\xe5\x8d\x87 \x28L\x29",80,500);
	GUI_DispStringAt("PM0.3\x7e 2.5",80,553);
	GUI_DispStringAt("PM2.5\x7e 10",80,600);
	GUI_DispStringAt("PM10\xe4\xbb\xa5\xe4\xb8\x8a",80,647);

	GUI_SetFont(&GUI_FontPingFangRegular90);
	GUI_DispStringAt("PM2.5",30,265);
	GUI_DispStringAt("PM10" ,30,375);

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
	
	GUI_ProgbarCustom_PM(PM25_Value ,PM25_TYPE);
	GUI_ProgbarCustom_CO2(CO2_Value ,CO2_TYPE );
	GUI_ProgbarCustom_EMR(ELEC_Value,EMR_TYPE );
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
  //WM_HWIN hWinBase;     // Window to be shifted out
  //WM_HWIN hWinSelected; // New window to become visible behind hWinBase
  
  //int xSize, ySize;

  //hWinBase = WM_GetParent(WM_GetParent(hWin));
  //xSize = WM_GetXSize(hWinBase);
  //ySize = WM_GetYSize(hWinBase);
  
  if(Index == 0)
   	BUTTON_STATE = BUTTON_PAGE_TWO_L;   // go to page 2 (right arrow)
  else if(Index == 1)
    BUTTON_STATE = BUTTON_PAGE_THREE_L; // go to page 3 (right arrow)
  else if(Index == 2)
    BUTTON_STATE = BUTTON_PAGE_ONE; 	// back to page 1 (left arrow)
  else if(Index == 3)
    BUTTON_STATE = BUTTON_PAGE_FOUR_L;  // go to page 4 (right arrow)
  else if(Index == 4)
    BUTTON_STATE = BUTTON_PAGE_TWO_L; 	// back to page 2 left arrow)
  else if(Index == 1000)
	BUTTON_STATE = BUTTON_PAGE_TWO_L;   // windows back to page 2
}

/*********************************************************************
*
*       _cbMenu
*
* Purpose:
*   Callback function of menu window.
*/
static void _cbMenuPageOne(WM_MESSAGE * pMsg) {
  int xSize, i, NCode;
  int Id = 0;
  
  WM_MOTION_INFO * pInfo;
  WM_HWIN hWin, hButton;
  static int IsPressed;

  hWin = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);      // Id of widget
    NCode = pMsg->Data.v;                 // Notification code

		switch (NCode) {
	    case WM_NOTIFICATION_CLICKED:
	       	IsPressed = 1;
			switch (Id) 
			{
				case GUI_ID_BUTTON9:
					dev_state.IsPM25Pressed = 1;
					dev_state.PM25PressedCtr = 0;
					break;
				case GUI_ID_USER_BUTTON_0:
					break;
				case GUI_ID_USER_BUTTON_1:
					dev_state.IsCO2Pressed = 1;
					dev_state.CO2PressedCtr = 0;
					break;
				case GUI_ID_USER_BUTTON_2:
					break;
				default:
		  			_CreateSelected(Id - GUI_ID_BUTTON0, pMsg->hWin);
			}
	      
			WM_InvalidateWindow(hWin);
	      break;
	    case WM_NOTIFICATION_RELEASED:
	      if (IsPressed) {
	        //
	        // React only if window is not moving
	        //
	        	IsPressed = 0;
	      }
	      break;
	    }
    	break;
  case WM_CREATE:
	    //
	    // Create all buttons
	    //
	  xSize = WM_GetWindowSizeX(hWin);

		WM_CreateTimer(hWin, 0, 2000, 0);

		switch (BUTTON_STATE) {
			
			case BUTTON_PAGE_ONE:
				i = 0;
    		    hButton = BUTTON_CreateUser(0, 1100, xSize, 180, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON0 + i, sizeof(i));
    		    BUTTON_SetSkin(hButton, _ButtonSkin);
    		    BUTTON_SetUserData(hButton, &i, sizeof(i));
				break;
			case BUTTON_PAGE_TWO_L:
				break;
			case BUTTON_PAGE_THREE_L:
				break;
			case BUTTON_PAGE_FOUR_L:
				break;
			case BUTTON_PAGE_FIVE:
				break;
		}

	    break;

  case WM_TIMER:
        WM_InvalidateWindow(hWin);
        WM_RestartTimer(pMsg->Data.v, 0);
        break;
	case WM_PAINT:
// display the main sensor data and button
		GUI_SampleValue_Disp((int)(g_CgkSensorData.uiPm25Ug*1.5f),(int)(g_CgkSensorData.uiPm10Ug*1.5f),g_CgkSensorData.uiCO2Dat,
							 (int)(g_CgkSensorData.uiPm03Dat*1.5f),(int)(g_CgkSensorData.uiPm25Dat*1.5f),(int)(g_CgkSensorData.uiPm10Dat*1.5f),
							 g_CgkSensorData.uiElecDat);
	
		break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*
*********************************************************************************************************
*	º¯ Êý Ãû: _CreateICONVIEW
*	¹¦ÄÜËµÃ÷: ´´½¨ICONVIEW
*	ÐÎ    ²Î£ºhParent   ¸¸´°¿Ú
*             pBm       ICONVIEWÉÏµÄÎ»Í¼
*             BitmapNum ICONVIEWÉÏÍ¼±ê¸öÊý       
*             x         xÖá×ø±ê
*             y         yÖá×ø±ê
*             w         ICONVIEW¿í
*             h         ICONVIEW¸ß   
*	·µ »Ø Öµ: ÎÞ
*********************************************************************************************************
*/
static WM_HWIN _CreateICONVIEW(WM_HWIN hParent, const BITMAP_ITEM *pBm, int BitmapNum, int Id, int x, int y, int w, int h) 
{
	WM_HWIN hIcon;
	int i;
	
	/*ÔÚÖ¸¶¨Î»ÖÃ´´½¨Ö¸¶¨³ß´çµÄICONVIEW Ð¡¹¤¾ß*/
	hIcon = ICONVIEW_CreateEx(x, 					/* Ð¡¹¤¾ßµÄ×î×óÏñËØ£¨ÔÚ¸¸×ø±êÖÐ£©*/
						     y, 					/* Ð¡¹¤¾ßµÄ×îÉÏÏñËØ£¨ÔÚ¸¸×ø±êÖÐ£©*/
							 w,                     /* Ð¡¹¤¾ßµÄË®Æ½³ß´ç£¨µ¥Î»£ºÏñËØ£©*/
							 h, 	                /* Ð¡¹¤¾ßµÄ´¹Ö±³ß´ç£¨µ¥Î»£ºÏñËØ£©*/
	                         hParent, 				            /* ¸¸´°¿ÚµÄ¾ä±ú¡£Èç¹ûÎª0 £¬ÔòÐÂÐ¡¹¤¾ß½«³ÉÎª×ÀÃæ£¨¶¥¼¶´°¿Ú£©µÄ×Ó´°¿Ú */
							 WM_CF_SHOW | WM_CF_HASTRANS,       /* ´°¿Ú´´½¨±ê¼Ç¡£ÎªÊ¹Ð¡¹¤¾ßÁ¢¼´¿É¼û£¬Í¨³£Ê¹ÓÃ WM_CF_SHOW */ 
	                         0,//ICONVIEW_CF_AUTOSCROLLBAR_V, 	/* Ä¬ÈÏÊÇ0£¬Èç¹û²»¹»ÏÖÊµ¿ÉÉèÖÃÔö¼õ´¹Ö±¹ö¶¯Ìõ */
							 Id, 			                    /* Ð¡¹¤¾ßµÄ´°¿ÚID */
							 ICON_Width, 				        /* Í¼±êµÄË®Æ½³ß´ç */
							 ICON_Height);						/* Í¼±êµÄ´¹Ö±³ß´ç,Í¼±êºÍÎÄ¼þ¶¼°üº¬ÔÚÀïÃæ£¬²»Òª´óÓÚICONVIEWµÄ¸ß¶È£¬µ¼ÖÂTextÏÔÊ¾²»ÍêÕû*/
	

	/* ÏòICONVIEW Ð¡¹¤¾ßÌí¼ÓÐÂÍ¼±ê */
	
	for (i = 0; i < BitmapNum; i++) 
	{
		ICONVIEW_AddBitmapItem(hIcon, pBm[i].pBitmap, pBm[i].pTextCn);
	}
	
	//ICONVIEW_SetFont(hIcon, &GUI_SysFontHZ16);
	
	/* ÉèÖÃÐ¡¹¤¾ßµÄ±³¾°É« 32 Î»ÑÕÉ«ÖµµÄÇ°8 Î»¿ÉÓÃÓÚalpha»ìºÏ´¦ÀíÐ§¹û*/
	ICONVIEW_SetBkColor(hIcon, ICONVIEW_CI_SEL, GUI_BLACK | 0x80000000);
	
	/* ÉèÖÃX·½ÏòµÄ±ß½çÖµÎª0£¬Ä¬ÈÏ²»ÊÇ0, Y·½ÏòÄ¬ÈÏÊÇ0£¬ÕâÀïÎÒÃÇÒ²½øÐÐÒ»ÏÂÉèÖÃ£¬·½±ãÒÔºóÐÞ¸Ä */
	ICONVIEW_SetFrame(hIcon, GUI_COORD_X, 0);
	ICONVIEW_SetFrame(hIcon, GUI_COORD_Y, 0);
	
	/* ÉèÖÃÍ¼±êÔÚx »òy ·½ÏòÉÏµÄ¼ä¾à¡£*/
	ICONVIEW_SetSpace(hIcon, GUI_COORD_X, ICONVIEW_XSpace);
	ICONVIEW_SetSpace(hIcon, GUI_COORD_Y, ICONVIEW_YSpace);
	
	/* ÉèÖÃ¶ÔÆë·½Ê½ ÔÚ5.22°æ±¾ÖÐ×îÐÂ¼ÓÈëµÄ */
	ICONVIEW_SetIconAlign(hIcon, ICONVIEW_IA_HCENTER|ICONVIEW_IA_TOP);
	
	//ICONVIEW_SetTextColor(hIcon, ICONVIEW_CI_UNSEL, 0xF020A0);
	return hIcon;
}

static void _cbBkCancel_UV(WM_MESSAGE * pMsg) {

  int xSize, i, NCode;
  int Id = 0;

  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);      // Id of widget
    NCode = pMsg->Data.v;                 // Notification code

	switch (NCode) {
	case WM_NOTIFICATION_CLICKED:
		if(Id == GUI_ID_USER_CLOSE)
			_CreateSelected(Id - GUI_ID_BUTTON0, pMsg->hWin);
      break;
    case WM_NOTIFICATION_RELEASED:
        //
        // React only if window is not moving
        //
        break;
	}
  case WM_PAINT:
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
	GUI_ProgbarCustom_UV(g_CgkSensorData.uiUltlDat,UV_TYPE);
    break;
  }
}

static void _cbBkCancel_CO2(WM_MESSAGE * pMsg) {

  int xSize, i, NCode;
  int Id = 0;

  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);      // Id of widget
    NCode = pMsg->Data.v;                 // Notification code

	switch (NCode) {
	case WM_NOTIFICATION_CLICKED:
		if(Id == GUI_ID_USER_CLOSE)
			_CreateSelected(Id - GUI_ID_BUTTON0, pMsg->hWin);
      break;
    case WM_NOTIFICATION_RELEASED:
        //
        // React only if window is not moving
        //
        break;
	}
  case WM_PAINT:
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
		
	GUI_SetColor(GUI_WHITE);
    GUI_SetFont(&GUI_Font32B_1);
	GUI_DispStringAt("null", 5, 10);
    break;
  }
}

void App_UVContrl(WM_HWIN hWin) 
{
	WM_HWIN hButton;
	
	hButtonCancel = WINDOW_CreateUser(0, 80, 720, 1280, 0, WM_CF_SHOW, 0, 0, _cbBkCancel_UV, 0);
	hButton = BUTTON_CreateUser(0, 1100, 360, 180, hButtonCancel, WM_CF_SHOW, 0, GUI_ID_USER_CLOSE, 0);
  	BUTTON_SetSkin(hButton, _ButtonSkinPageTwoLeft);

    
}

void App_CO2Contrl(WM_HWIN hWin)
{
	WM_HWIN hButton;
	
	hButtonCancel = WINDOW_CreateUser(0, 80, 720, 1280, 0, WM_CF_SHOW, 0, 0, _cbBkCancel_CO2, 0);
	hButton = BUTTON_CreateUser(0, 1100, 360, 180, hButtonCancel, WM_CF_SHOW, 0, GUI_ID_USER_CLOSE, 0);
  	BUTTON_SetSkin(hButton, _ButtonSkinPageTwoLeft);

}

void App_KnowContrl(WM_HWIN hWin)
{
	WM_HWIN hButton;
	
	hButtonCancel = WINDOW_CreateUser(0, 80, 720, 1280, 0, WM_CF_SHOW, 0, 0, _cbBkCancel_CO2, 0);
	hButton = BUTTON_CreateUser(0, 1100, 360, 180, hButtonCancel, WM_CF_SHOW, 0, GUI_ID_USER_CLOSE, 0);
  	BUTTON_SetSkin(hButton, _ButtonSkinPageTwoLeft);
}

void App_LookContrl(WM_HWIN hWin)
{
	WM_HWIN hButton;
	
	hButtonCancel = WINDOW_CreateUser(0, 80, 720, 1280, 0, WM_CF_SHOW, 0, 0, _cbBkCancel_CO2, 0);
	hButton = BUTTON_CreateUser(0, 1100, 360, 180, hButtonCancel, WM_CF_SHOW, 0, GUI_ID_USER_CLOSE, 0);
  	BUTTON_SetSkin(hButton, _ButtonSkinPageTwoLeft);
}

static void (* _IconModules[])( WM_HWIN hWin) = 
{
	App_UVContrl,
	App_CO2Contrl,
	App_KnowContrl,
	App_LookContrl,
};

/*********************************************************************
*
*       _cbMenu
*
* Purpose:
*   Callback function of menu window.
*/
static void _cbMenuPageTwo(WM_MESSAGE * pMsg) {
  int xSize, i, NCode;
  int Id = 0;
  
  WM_MOTION_INFO * pInfo;
  WM_HWIN hWin, hButton;
  static int IsPressed;

  hWin = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);      // Id of widget
    NCode = pMsg->Data.v;                 // Notification code

		switch (Id) 
		{
			case GUI_ID_ICONVIEW0:
				switch (NCode) {
					case WM_NOTIFICATION_SEL_CHANGED:
				        break;
					case WM_NOTIFICATION_CLICKED:
						_IconModules[ICONVIEW_GetSel(hIcon)](hWin);
						break;
					case WM_NOTIFICATION_RELEASED:
						
						break;
				    }
				break;
			default:
				switch (NCode) {
					case WM_NOTIFICATION_CLICKED:
			  			IsPressed = 1;
			  			_CreateSelected(Id - GUI_ID_BUTTON0, pMsg->hWin);
			  			break;
					case WM_NOTIFICATION_RELEASED:
			  			if (IsPressed) {
				        //
				        // React only if window is not moving
				        //
				        IsPressed = 0;
						break;
			  			}
		    	    }
		}
    break;
  case WM_CREATE:
	    //
	    // Create all buttons
	    //
	    xSize = WM_GetWindowSizeX(hWin);

			switch ((int)BUTTON_STATE) {
				
				case BUTTON_PAGE_ONE:
					break;
				case BUTTON_PAGE_TWO_L:

					i = 1;
		    		hButton = BUTTON_CreateUser(360, 1100, xSize / 2, 180, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON0 + i, sizeof(i));
		    		BUTTON_SetSkin(hButton, _ButtonSkinPageTwoRight);
		    		BUTTON_SetUserData(hButton, &i, sizeof(i));

					i = 2;
	    			hButton = BUTTON_CreateUser(  0, 1100, xSize / 2, 180, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON0 + i, sizeof(i));
	    			BUTTON_SetSkin(hButton, _ButtonSkinPageTwoLeft);
	    			BUTTON_SetUserData(hButton, &i, sizeof(i));

					break;
				case BUTTON_PAGE_THREE_L:
					break;
				case BUTTON_PAGE_FOUR_L:
					break;
				case BUTTON_PAGE_FIVE:
					break;
			}

	    break;
	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
	    GUI_Clear();
		GUI_SetColor(GUI_WHITE);
	    GUI_SetTextMode(GUI_TM_TRANS);
		GUI_SetFont(&GUI_FontPingFangRegular90);
		GUI_UC_SetEncodeUTF8();
		GUI_DispStringAt("\xe7\xb4\xab\xe5\xa4\x96\xe7\xba\xbf",290,64); 				    // ×ÏÍâÏß
		GUI_DispStringAt("\xe6\xa3\x80\xe6\xb5\x8b",290,145); 							    // ¼ì²â
		GUI_DispStringAt("\xe4\xba\x8c\xe6\xb0\xa7\xe5\x8c\x96\xe7\xa2\xb3",290,300); 	    // ¶þÑõ»¯Ì¼
		GUI_DispStringAt("\xe6\x88\xb7\xe5\xa4\x96\xe6\xa0\xa1\xe5\x87\x86",290,380); 	    // »§ÍâÐ£×¼
		break;
  default:
    	WM_DefaultProc(pMsg);
  }
}


/*********************************************************************
*
*       _cbButton
*/
static void _cbButtonUsrParam(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) {
  WM_HWIN  hWin;
  int Index;
  
  hWin = pDrawItemInfo->hWin;
  switch (pDrawItemInfo->Cmd) {
  case WIDGET_ITEM_DRAW_BACKGROUND:
    BUTTON_GetUserData(hWin, &Index, sizeof(Index));
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    GUI_SetColor(GUI_WHITE);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(&GUI_FontPingFangRegular67);
    if(Index == GUI_ID_USER_DET_M)
        GUI_DispStringAt("\xe6\x89\x8b\xe5\x8a\xa8\xe6\xa3\x80\xe6\xb5\x8b",0,0); 	// ÊÖ¶¯¼ì²â
    else if(Index == GUI_ID_USER_DET_1)  
        GUI_DispStringAt("\xe6\xaf\x8f\xe5\xa4\xa9\xe6\xa3\x80\xe6\xb5\x8b\x31\xe6\xac\xa1",0,0); 	// Ã¿Ìì¼ì²â1´Î
    else if(Index == GUI_ID_USER_DET_2)
        GUI_DispStringAt("\xe6\xaf\x8f\xe5\xa4\xa9\xe6\xa3\x80\xe6\xb5\x8b\x32\xe6\xac\xa1",0,0); 	// Ã¿Ìì¼ì²â2´Î
     else if(Index == GUI_ID_USER_DET_4)
        GUI_DispStringAt("\xe6\xaf\x8f\xe5\xa4\xa9\xe6\xa3\x80\xe6\xb5\x8b\x34\xe6\xac\xa1",0,0); 	// Ã¿Ìì¼ì²â4´Î
    break;
  default:
    break;
  }
}

static void _cbButtonUsrtimSet(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) {
  int Index;
  WM_HWIN  hWin;
  char buf[32];

  rt_memset(buf, NULL, sizeof(buf));
  hWin = pDrawItemInfo->hWin;
  switch (pDrawItemInfo->Cmd) {
  case WIDGET_ITEM_DRAW_BACKGROUND:
    BUTTON_GetUserData(hWin, &Index, sizeof(Index));

    //rt_kprintf("tmSet -------------- %d ---------------\r\n",Index);
    
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    GUI_SetColor(GUI_WHITE);
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetFont(&GUI_FontPingFangRegular67);
    
    if(Index == GUI_ID_USER_1_TS){
        sprintf(buf, "%02s:%02s",uiWinCtl.timbuf_1,uiWinCtl.timbuf_1_);
        GUI_DispStringAt(buf,0,0);
    }
    else if(Index == GUI_ID_USER_2_0_TS){
        sprintf(buf, "%02s:%02s",uiWinCtl.timbuf_2_0,uiWinCtl.timbuf_2_0_);
        GUI_DispStringAt(buf,0,0);
    }
    else if(Index == GUI_ID_USER_2_1_TS){
        sprintf(buf, "%02s:%02s",uiWinCtl.timbuf_2_1,uiWinCtl.timbuf_2_1_);
        GUI_DispStringAt(buf,0,0);
    }
    else if(Index == GUI_ID_USER_4_0_TS){
        sprintf(buf, "%02s:%02s",uiWinCtl.timbuf_4_0,uiWinCtl.timbuf_4_0_);
        GUI_DispStringAt(buf,0,0);
    }
    else if(Index == GUI_ID_USER_4_1_TS){
        sprintf(buf, "%02s:%02s",uiWinCtl.timbuf_4_1,uiWinCtl.timbuf_4_1_);
        GUI_DispStringAt(buf,0,0);
    }
    else if(Index == GUI_ID_USER_4_2_TS){
        sprintf(buf, "%02s:%02s",uiWinCtl.timbuf_4_2,uiWinCtl.timbuf_4_2_);
        GUI_DispStringAt(buf,0,0);
    }
    else if(Index == GUI_ID_USER_4_3_TS){
        sprintf(buf, "%02s:%02s",uiWinCtl.timbuf_4_3,uiWinCtl.timbuf_4_3_);
        GUI_DispStringAt(buf,0,0);
    }
    break;
  default:
    break;
  }
}

static void _cbButtonSysParam(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) {
  WM_HWIN  hWin;
  int Index;
  
  hWin = pDrawItemInfo->hWin;
  switch (pDrawItemInfo->Cmd) {
  case WIDGET_ITEM_DRAW_BACKGROUND:
    BUTTON_GetUserData(hWin, &Index, sizeof(Index));

    //rt_kprintf("SysParam -------------- %d ---------------\r\n",Index);
    
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    GUI_SetColor(GUI_WHITE);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(&GUI_Fonthanyi48);
    if(Index == GUI_ID_USER_DET_M)
        GUI_DispStringAt("WIFI",0,10); 	// WIFI
    else if(Index == GUI_ID_USER_DET_1)  
        GUI_DispStringAt("\xe9\x95\xbf\xe4\xba\xae",0,0); 	// ³¤ÁÁ
    else if(Index == GUI_ID_USER_DET_2)
        GUI_DispStringAt("\xe4\xb8\x80\xe5\x88\x86\xe9\x92\x9f\xe4\xba\xae",0,0); 	// Ò»·ÖÖÓÁÁ
    break;
  default:
    break;
  }
}

static void _cbButtonSystimSet(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) {
  WM_HWIN  hWin;
  char buf[64];
  int Index;

  rt_memset(buf, NULL, sizeof(buf));
  hWin = pDrawItemInfo->hWin;
  switch (pDrawItemInfo->Cmd) {
  case WIDGET_ITEM_DRAW_BACKGROUND:
    BUTTON_GetUserData(hWin, &Index, sizeof(Index));

    //rt_kprintf("SystmSet -------------- %d ---------------\r\n",Index);
    
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    GUI_SetColor(GUI_WHITE);
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetFont(&GUI_Fonthanyi48);
    
    if(Index == GUI_ID_USER_1_TS){
        sprintf(buf, "%04s.%02s.%02s",uiWinCtl.yearbuf,uiWinCtl.monthbuf,uiWinCtl.daybuf);
        GUI_DispStringAt(buf,0,0);
    }
    else if(Index == GUI_ID_USER_2_0_TS){
        sprintf(buf, "%02s:%02s",uiWinCtl.hourbuf,uiWinCtl.minbuf);
        GUI_DispStringAt(buf,0,0);
    }
    break;
  default:
    break;
  }
}

#define TEXT_MAXLEN             40
#define XPOS_                   110
#define XPOS_R1                 250
#define XPOS_R2                 390
#define YPOS_                   180
#define XPOS_RAD                55
#define YPOS_RAD                120
#define XPOS_BUT                80
#define YPOS_BUT                85
#define XPOS_SYS_RAD            60
#define YPOS_SYS_RAD            600
#define XPOS_SYS_OFFSET         280

/*********************************************************************
*
*       _CreateButtonDevices
*/
/*static*/ void _CreateButtonDevices(int xPos, int yPos, int xSize, int ySize, int r/*, GUI_MEMDEV_Handle * phMem*/) {
  GUI_MEMDEV_Handle hMemPrev;
  int               i;

  //*(phMem + 0) = GUI_MEMDEV_CreateFixed(xPos, yPos, xSize, ySize, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_16, GUI_COLOR_CONV_565);
  //*(phMem + 1) = GUI_MEMDEV_CreateFixed(xPos, yPos, xSize, ySize, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_16, GUI_COLOR_CONV_565);
  for (i = 0; i < 2; i++) {
    
    //hMemPrev = GUI_MEMDEV_Select(*(phMem + i));
    if (i) {
      GUI_DrawGradientV(1, 0, xSize, ySize - 1, /*0xFFBB55*/GUI_LIGHTGRAY, /*LIGHTBLUE*/GUI_WHITE);
    } else {
      GUI_DrawGradientV(1, 0, xSize, ySize - 1, /*LIGHTBLUE*/GUI_WHITE, /*0xFFBB55*/GUI_LIGHTGRAY);
    }
    //GUI_SetColor(GUI_LIGHTGRAY);
    //GUI_DrawRoundedRect(0, 0, xSize - 1, ySize - 1, r);
    //GUI_SetColor(GUI_GRAY);
    //GUI_DrawRoundedRect(0, 0, xSize - 1, ySize - 2, r);
    //_RoundUpCorners(*(phMem + i), LEFT | RIGHT,  r, TOP | BOTTOM);
    //GUI_MEMDEV_Select(hMemPrev);
  }
}

/*********************************************************************
*
*       _cbButton
*/
static void _cbButtonAndEdit(WM_MESSAGE * pMsg) {
  //static GUI_MEMDEV_Handle ahMem[2];
  GUI_RECT                 Rect;
  WM_HWIN                  hWin;
  char                     acText[20];
  int                      xSize;
  int                      ySize;
  int                      Index;
  int                      xPos;
  int                      yPos;
  int                      Len;


  hWin = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_PAINT:
    GUI_Button_mutex_take();
    
    BUTTON_GetText(hWin, acText, sizeof(acText));
    xPos  = WM_GetWindowOrgX(hWin);
    yPos  = WM_GetWindowOrgY(hWin);
    xSize = WM_GetWindowSizeX(hWin);
    ySize = WM_GetWindowSizeY(hWin);
    
    //if (ahMem[0] == 0) {
      _CreateButtonDevices(xPos, yPos, xSize, ySize, 5);
    //}
    
    Index = BUTTON_IsPressed(hWin);
    //GUI_MEMDEV_WriteAt(ahMem[Index], xPos, yPos);
    Len = GUI__strlen(acText);
    GUI_SetTextMode(GUI_TM_TRANS);
    Rect.x0 = 0;
    Rect.y0 = Index - 1;
    Rect.x1 = xSize - 1;
    Rect.y1 = ySize - 1 + Index - 1;
    GUI_SetFont(&GUI_Font24_ASCII);
    GUI_SetColor(GUI_BLACK);
    GUI_DispStringInRectMax(acText, &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER, Len);
    Rect.y0++;
    Rect.y1++;
    GUI_SetColor(GUI_GRAY);
    GUI_DispStringInRectMax(acText, &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER, Len);

    rt_kprintf("_cbButtonAndEdit -------------- %s ---------------\r\n",acText);
    
    GUI_Button_mutex_release();
    break;
  default:
    BUTTON_Callback(pMsg);
  }
}

static void _cbBktimSet(WM_MESSAGE * pMsg) {
  WM_HWIN hWin;
  static EDIT_Handle hEdit, hEdit1;
  static WM_HWIN hButton,hButton1,hButton2,hButton3,hButton4;
  char initTimSetCtrl = NULL;
  GUI_RECT Rect;
  char aBuffer[TEXT_MAXLEN];
  char aBuffer_[TEXT_MAXLEN];
  int xSize, i, NCode;
  int Id = 0;
  WM_MESSAGE Msg;

  memset(aBuffer,NULL,sizeof(aBuffer));
  memset(aBuffer_,NULL,sizeof(aBuffer_));
  hWin = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);      // Id of widget
    NCode = pMsg->Data.v;                 // Notification code
    
	switch (NCode) {
        case WM_NOTIFICATION_CLICKED:
            switch (Id) {
                case GUI_ID_USER_EDIT0: // -
                    EDIT_GetText(hEdit, aBuffer, TEXT_MAXLEN);
    			    sprintf(aBuffer,"%d",atoi(aBuffer) - 1);
                    EDIT_SetFont(hEdit, &GUI_Font8x16);
                    EDIT_SetTextColor(hEdit, EDIT_CI_DISABLED, GUI_BLACK);
                    EDIT_SetTextAlign(hEdit,GUI_TA_HCENTER | GUI_TA_VCENTER);
                    EDIT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT0), aBuffer);
                    break;
                case GUI_ID_USER_EDIT1: // +
                    EDIT_GetText(hEdit, aBuffer, TEXT_MAXLEN);
    			    sprintf(aBuffer,"%d",atoi(aBuffer) + 1);
                    EDIT_SetFont(hEdit, &GUI_Font8x16);
                    EDIT_SetTextColor(hEdit, EDIT_CI_DISABLED, GUI_BLACK);
                    EDIT_SetTextAlign(hEdit,GUI_TA_HCENTER | GUI_TA_VCENTER);
                    EDIT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT0), aBuffer);
					break;
                case GUI_ID_USER_EDIT3: // -
                    EDIT_GetText(hEdit1, aBuffer_, TEXT_MAXLEN);
    			    sprintf(aBuffer_,"%d",atoi(aBuffer_) - 1);
                    EDIT_SetFont(hEdit1, &GUI_Font8x16);
                    EDIT_SetTextColor(hEdit1, EDIT_CI_DISABLED, GUI_BLACK);
                    EDIT_SetTextAlign(hEdit1,GUI_TA_HCENTER | GUI_TA_VCENTER);
                    EDIT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT1), aBuffer_);
                    break;
                case GUI_ID_USER_EDIT4: // +
                    EDIT_GetText(hEdit1, aBuffer_, TEXT_MAXLEN);
    			    sprintf(aBuffer_,"%d",atoi(aBuffer_) + 1);
                    EDIT_SetFont(hEdit1, &GUI_Font8x16);
                    EDIT_SetTextColor(hEdit1, EDIT_CI_DISABLED, GUI_BLACK);
                    EDIT_SetTextAlign(hEdit1,GUI_TA_HCENTER | GUI_TA_VCENTER);
                    EDIT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT1), aBuffer_);
					break;
                case GUI_ID_USER_EDIT2: // OK
                    EDIT_GetText(hEdit, aBuffer, TEXT_MAXLEN);
                    EDIT_GetText(hEdit1, aBuffer_, TEXT_MAXLEN);

                    WM_DeleteWindow(hWinTimSet);
                    WM_DeleteWindow(hEdit);
                    WM_DeleteWindow(hEdit1);
                    WM_DeleteWindow(hButton);
                    WM_DeleteWindow(hButton1);
                    WM_DeleteWindow(hButton2);
                    WM_DeleteWindow(hButton3);
                    WM_DeleteWindow(hButton4);
                    
                    if(uiWinCtl.msgButton1Flag) {
                        uiWinCtl.msgButton1Flag = false;

                        rt_memset(uiWinCtl.timbuf_1,NULL,sizeof(uiWinCtl.timbuf_1));
                        rt_memset(uiWinCtl.timbuf_1_,NULL,sizeof(uiWinCtl.timbuf_1_));
                        
                        memcpy(uiWinCtl.timbuf_1,aBuffer,strlen(aBuffer));
                        memcpy(uiWinCtl.timbuf_1_,aBuffer_,strlen(aBuffer_));
                    }
                    else if(uiWinCtl.msgButton20Flag) {
                        uiWinCtl.msgButton20Flag = false;

                        rt_memset(uiWinCtl.timbuf_2_0,NULL,sizeof(uiWinCtl.timbuf_2_0));
                        rt_memset(uiWinCtl.timbuf_2_0_,NULL,sizeof(uiWinCtl.timbuf_2_0_));
                        
                        memcpy(uiWinCtl.timbuf_2_0,aBuffer,strlen(aBuffer));
                        memcpy(uiWinCtl.timbuf_2_0_,aBuffer_,strlen(aBuffer_));
                    }
                    else if(uiWinCtl.msgNetTimButton1Flag) {
                        uiWinCtl.msgNetTimButton1Flag = false;

                        rt_memset(uiWinCtl.hourbuf,NULL,sizeof(uiWinCtl.hourbuf));
                        rt_memset(uiWinCtl.minbuf,NULL,sizeof(uiWinCtl.minbuf));
                        
                        memcpy(uiWinCtl.hourbuf,aBuffer,strlen(aBuffer));
                        memcpy(uiWinCtl.minbuf,aBuffer_,strlen(aBuffer_));
                    }
                    else if(uiWinCtl.msgButton21Flag) {
                        uiWinCtl.msgButton21Flag = false;

                        rt_memset(uiWinCtl.timbuf_2_1,NULL,sizeof(uiWinCtl.timbuf_2_1));
                        rt_memset(uiWinCtl.timbuf_2_1_,NULL,sizeof(uiWinCtl.timbuf_2_1_));
                        
                        memcpy(uiWinCtl.timbuf_2_1,aBuffer,strlen(aBuffer));
                        memcpy(uiWinCtl.timbuf_2_1_,aBuffer_,strlen(aBuffer_));
                    }
                    else if(uiWinCtl.msgButton40Flag) {
                        uiWinCtl.msgButton40Flag = false;

                        rt_memset(uiWinCtl.timbuf_4_0,NULL,sizeof(uiWinCtl.timbuf_4_0));
                        rt_memset(uiWinCtl.timbuf_4_0_,NULL,sizeof(uiWinCtl.timbuf_4_0_));
                        
                        memcpy(uiWinCtl.timbuf_4_0,aBuffer,strlen(aBuffer));
                        memcpy(uiWinCtl.timbuf_4_0_,aBuffer_,strlen(aBuffer_));
                    }
                    else if(uiWinCtl.msgButton41Flag) {
                        uiWinCtl.msgButton41Flag = false;

                        rt_memset(uiWinCtl.timbuf_4_1,NULL,sizeof(uiWinCtl.timbuf_4_1));
                        rt_memset(uiWinCtl.timbuf_4_1_,NULL,sizeof(uiWinCtl.timbuf_4_1_));
                        
                        memcpy(uiWinCtl.timbuf_4_1,aBuffer,strlen(aBuffer));
                        memcpy(uiWinCtl.timbuf_4_1_,aBuffer_,strlen(aBuffer_));
                    }
                    else if(uiWinCtl.msgButton42Flag) {
                        uiWinCtl.msgButton42Flag = false;

                        rt_memset(uiWinCtl.timbuf_4_2,NULL,sizeof(uiWinCtl.timbuf_4_2));
                        rt_memset(uiWinCtl.timbuf_4_2_,NULL,sizeof(uiWinCtl.timbuf_4_2_));
                        
                        memcpy(uiWinCtl.timbuf_4_2,aBuffer,strlen(aBuffer));
                        memcpy(uiWinCtl.timbuf_4_2_,aBuffer_,strlen(aBuffer_));
                    }
                    else if(uiWinCtl.msgButton43Flag) {
                        uiWinCtl.msgButton43Flag = false;

                        rt_memset(uiWinCtl.timbuf_4_3,NULL,sizeof(uiWinCtl.timbuf_4_3));
                        rt_memset(uiWinCtl.timbuf_4_3_,NULL,sizeof(uiWinCtl.timbuf_4_3_));
                        
                        memcpy(uiWinCtl.timbuf_4_3,aBuffer,strlen(aBuffer));
                        memcpy(uiWinCtl.timbuf_4_3_,aBuffer_,strlen(aBuffer_));
                    }
                    break;
                }
            break;
        case WM_NOTIFICATION_RELEASED:
            break;
        }
       break;
   case WM_CREATE:
       GUI_mutex_take();

       hButton = BUTTON_CreateEx(XPOS_ + 100, 330, 120, 70, hWin, WM_CF_SHOW, 0, GUI_ID_USER_EDIT2);
       BUTTON_SetText(hButton, "OK");
       WM_SetCallback(hButton, _cbButtonAndEdit);
       //WM_DisableMemdev(hButton);
       //WM_SetHasTrans(hButton);

       /*######################################################################*/
        
       hButton1 = BUTTON_CreateEx(XPOS_, 80, 120, 70, hWin, WM_CF_SHOW, 0, GUI_ID_USER_EDIT1);
       BUTTON_SetText(hButton1, "+");
       WM_SetCallback(hButton1, _cbButtonAndEdit);
       //WM_DisableMemdev(hButton1);
       //WM_SetHasTrans(hButton1);

       hEdit = EDIT_CreateEx(XPOS_, 150, 122, 70, hWin, WM_CF_SHOW, 1, GUI_ID_EDIT0, TEXT_MAXLEN);
       EDIT_SetFont(hEdit, &GUI_Font8x16);
       EDIT_SetTextColor(hEdit, 1, GUI_BLACK);
       EDIT_SetTextAlign(hEdit,GUI_TA_HCENTER | GUI_TA_VCENTER);
       EDIT_SetText(hEdit, "23");

       hButton2 = BUTTON_CreateEx(XPOS_, 220, 120, 70, hWin, WM_CF_SHOW, 0, GUI_ID_USER_EDIT0);
       BUTTON_SetText(hButton2, "-");
       WM_SetCallback(hButton2, _cbButtonAndEdit);
       //WM_DisableMemdev(hButton2);
       //WM_SetHasTrans(hButton2);

       /*#######################################################################*/

       hButton3 = BUTTON_CreateEx(XPOS_R1, 80, 120, 70, hWin, WM_CF_SHOW, 0, GUI_ID_USER_EDIT4);
       BUTTON_SetText(hButton3, "+");
       WM_SetCallback(hButton3, _cbButtonAndEdit);
       //WM_DisableMemdev(hButton3);
       //WM_SetHasTrans(hButton3);
       
       hEdit1 = EDIT_CreateEx(XPOS_R1, 150, 122, 70, hWin, WM_CF_SHOW, 1, GUI_ID_EDIT1, TEXT_MAXLEN);
       EDIT_SetFont(hEdit1, &GUI_Font8x16);
       EDIT_SetTextColor(hEdit1, 1, GUI_BLACK);
       EDIT_SetTextAlign(hEdit1,GUI_TA_HCENTER | GUI_TA_VCENTER);
       EDIT_SetText(hEdit1, "20");

       hButton4 = BUTTON_CreateEx(XPOS_R1, 220, 120, 70, hWin, WM_CF_SHOW, 0, GUI_ID_USER_EDIT3);
       BUTTON_SetText(hButton4, "-");
       WM_SetCallback(hButton4, _cbButtonAndEdit);
       //WM_DisableMemdev(hButton4);
       //WM_SetHasTrans(hButton4);
       
       /*##########################################################################*/
       
       GUI_mutex_release();
       break;
   case WM_PAINT:
       GUI_EnableAlpha(1);
       GUI_SetBkColor(0x60000000);
       GUI_Clear();
       GUI_SetColor(GUI_LIGHTGRAY);
       GUI_DrawRect(1,1,449,449);
       GUI_DrawRect(2,2,448,448);
       GUI_SetColor(0x60000000);                       // 0x60000000 ÑÕÉ«´øÍ¸Ã÷¶È,ÆäÖÐ0x60ÎªAphaÖµ
       GUI_FillRect(2,40,440,440);
       GUI_SetColor(GUI_WHITE);
       GUI_AA_FillCircle(45,45,15);
       GUI_SetColor(GUI_BLACK);
       GUI_SetTextMode(GUI_TM_TRANS);
       GUI_SetFont(&GUI_Font8x16_ASCII);
       GUI_DispStringAt("i",40,40);
       GUI_EnableAlpha(0);

       Rect.x0 = 60;
       Rect.y0 = 20;
       Rect.x1 = 390;
       Rect.y1 = 90;
       GUI_MoveRect(&Rect, 0, 330);
       GUI_DrawGradientV(Rect.x0, Rect.y0, Rect.x1, Rect.y1, GUI_LIGHTGRAY, GUI_GRAY);
       break;
  }
}

static void _cbBkNetDatSysSet(WM_MESSAGE * pMsg) {
  WM_HWIN hWin;
  static EDIT_Handle hEdit3, hEdit4, hEdit5;
  static WM_HWIN hButton5, hButton6,hButton7,hButton8,hButton9,hButton10,hButton11,hButton12;
  char initTimSetCtrl = NULL;
  GUI_RECT Rect;
  char aBuffer[TEXT_MAXLEN];
  char aBuffer_[TEXT_MAXLEN];
  char aBuffer__[TEXT_MAXLEN];
  int xSize, i, NCode;
  int Id = 0;
  WM_MESSAGE Msg;

  memset(aBuffer,NULL,sizeof(aBuffer));
  memset(aBuffer_,NULL,sizeof(aBuffer_));
  memset(aBuffer__,NULL,sizeof(aBuffer__));
  hWin = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);      // Id of widget
    NCode = pMsg->Data.v;                 // Notification code
    
	switch (NCode) {
        case WM_NOTIFICATION_CLICKED:
            switch (Id) {
                case GUI_ID_USER_EDIT0: // -
                    EDIT_GetText(hEdit3, aBuffer, TEXT_MAXLEN);
    			    sprintf(aBuffer,"%d",atoi(aBuffer) - 1);
                    EDIT_SetFont(hEdit3, &GUI_Font8x16);
                    EDIT_SetTextColor(hEdit3, EDIT_CI_DISABLED, GUI_BLACK);
                    EDIT_SetTextAlign(hEdit3,GUI_TA_HCENTER | GUI_TA_VCENTER);
                    EDIT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT0), aBuffer);
                    break;
                case GUI_ID_USER_EDIT1: // +
                    EDIT_GetText(hEdit3, aBuffer, TEXT_MAXLEN);
    			    sprintf(aBuffer,"%d",atoi(aBuffer) + 1);
                    EDIT_SetFont(hEdit3, &GUI_Font8x16);
                    EDIT_SetTextColor(hEdit3, EDIT_CI_DISABLED, GUI_BLACK);
                    EDIT_SetTextAlign(hEdit3,GUI_TA_HCENTER | GUI_TA_VCENTER);
                    EDIT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT0), aBuffer);
					break;
                case GUI_ID_USER_EDIT3: // -
                    EDIT_GetText(hEdit4, aBuffer_, TEXT_MAXLEN);
    			    sprintf(aBuffer_,"%d",atoi(aBuffer_) - 1);
                    EDIT_SetFont(hEdit4, &GUI_Font8x16);
                    EDIT_SetTextColor(hEdit4, EDIT_CI_DISABLED, GUI_BLACK);
                    EDIT_SetTextAlign(hEdit4,GUI_TA_HCENTER | GUI_TA_VCENTER);
                    EDIT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT1), aBuffer_);
                    break;
                case GUI_ID_USER_EDIT4: // +
                    EDIT_GetText(hEdit4, aBuffer_, TEXT_MAXLEN);
    			    sprintf(aBuffer_,"%d",atoi(aBuffer_) + 1);
                    EDIT_SetFont(hEdit4, &GUI_Font8x16);
                    EDIT_SetTextColor(hEdit4, EDIT_CI_DISABLED, GUI_BLACK);
                    EDIT_SetTextAlign(hEdit4,GUI_TA_HCENTER | GUI_TA_VCENTER);
                    EDIT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT1), aBuffer_);
					break;
                case GUI_ID_USER_EDIT5: // -
                    EDIT_GetText(hEdit5, aBuffer__, TEXT_MAXLEN);
    			    sprintf(aBuffer__,"%d",atoi(aBuffer__) - 1);
                    EDIT_SetFont(hEdit5, &GUI_Font8x16);
                    EDIT_SetTextColor(hEdit5, EDIT_CI_DISABLED, GUI_BLACK);
                    EDIT_SetTextAlign(hEdit5,GUI_TA_HCENTER | GUI_TA_VCENTER);
                    EDIT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT2), aBuffer__);
                    break;
                case GUI_ID_USER_EDIT6: // +
                    EDIT_GetText(hEdit5, aBuffer__, TEXT_MAXLEN);
    			    sprintf(aBuffer__,"%d",atoi(aBuffer__) + 1);
                    EDIT_SetFont(hEdit5, &GUI_Font8x16);
                    EDIT_SetTextColor(hEdit5, EDIT_CI_DISABLED, GUI_BLACK);
                    EDIT_SetTextAlign(hEdit5,GUI_TA_HCENTER | GUI_TA_VCENTER);
                    EDIT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT2), aBuffer__);
					break;
                case GUI_ID_USER_EDIT2: // OK
                    EDIT_GetText(hEdit3,  aBuffer,   TEXT_MAXLEN);
                    EDIT_GetText(hEdit4, aBuffer_,  TEXT_MAXLEN);
                    EDIT_GetText(hEdit5, aBuffer__, TEXT_MAXLEN);
                        
                    WM_DeleteWindow(hWinTimSet);
                    WM_DeleteWindow(hEdit3);
                    WM_DeleteWindow(hEdit4);
                    WM_DeleteWindow(hEdit5);
                    WM_DeleteWindow(hButton5);
                    WM_DeleteWindow(hButton6);
                    WM_DeleteWindow(hButton7);
                    WM_DeleteWindow(hButton8);
                    WM_DeleteWindow(hButton9);
                    WM_DeleteWindow(hButton10);
                    WM_DeleteWindow(hButton11);
                    
                    if(uiWinCtl.msgNetTimButtonFlag) {
                        uiWinCtl.msgNetTimButtonFlag = false;

                        rt_memset(uiWinCtl.yearbuf,NULL,sizeof(uiWinCtl.yearbuf));
                        rt_memset(uiWinCtl.monthbuf,NULL,sizeof(uiWinCtl.monthbuf));
                        rt_memset(uiWinCtl.daybuf,NULL,sizeof(uiWinCtl.daybuf));

                        memcpy(uiWinCtl.yearbuf,aBuffer,strlen(aBuffer));
                        memcpy(uiWinCtl.monthbuf,aBuffer_,strlen(aBuffer_));
                        memcpy(uiWinCtl.daybuf,aBuffer__,strlen(aBuffer__));
                    }
                    break;
                }
            break;
        case WM_NOTIFICATION_RELEASED:
            break;
        }
       break;
   case WM_CREATE:
       GUI_mutex_take();

       hButton11 = BUTTON_CreateEx(XPOS_ + 100, 330, 120, 70, hWin, WM_CF_SHOW, 0, GUI_ID_USER_EDIT2);
       BUTTON_SetText(hButton11, "OK");
       WM_SetCallback(hButton11, _cbButtonAndEdit);
       //WM_DisableMemdev(hButton11);
       //WM_SetHasTrans(hButton11);

       /*#####################################################################*/
       hButton5 = BUTTON_CreateEx(XPOS_, 80, 120, 70, hWin, WM_CF_SHOW, 0, GUI_ID_USER_EDIT1);
       BUTTON_SetText(hButton5, "+");
       WM_SetCallback(hButton5, _cbButtonAndEdit);
       //WM_DisableMemdev(hButton5);
       //WM_SetHasTrans(hButton5);

       hEdit3 = EDIT_CreateEx(XPOS_, 150, 122, 70, hWin, WM_CF_SHOW, 1, GUI_ID_EDIT0, TEXT_MAXLEN);
       EDIT_SetFont(hEdit3, &GUI_Font8x16);
       EDIT_SetTextColor(hEdit3, EDIT_CI_DISABLED, GUI_BLACK);
       EDIT_SetTextAlign(hEdit3,GUI_TA_HCENTER | GUI_TA_VCENTER);
       EDIT_SetText(hEdit3, "2000");

       hButton6 = BUTTON_CreateEx(XPOS_, 220, 120, 70, hWin, WM_CF_SHOW, 0, GUI_ID_USER_EDIT0);
       BUTTON_SetText(hButton6, "-");
       WM_SetCallback(hButton6, _cbButtonAndEdit);
       //WM_DisableMemdev(hButton6);
       //WM_SetHasTrans(hButton6);
       /*#####################################################################*/
       hButton7 = BUTTON_CreateEx(XPOS_R1, 80, 120, 70, hWin, WM_CF_SHOW, 0, GUI_ID_USER_EDIT4);
       BUTTON_SetText(hButton7, "+");
       WM_SetCallback(hButton7, _cbButtonAndEdit);
       //WM_DisableMemdev(hButton7);
       //WM_SetHasTrans(hButton7);
       
       hEdit4 = EDIT_CreateEx(XPOS_R1, 150, 122, 70, hWin, WM_CF_SHOW, 1, GUI_ID_EDIT1, TEXT_MAXLEN);
       EDIT_SetFont(hEdit4, &GUI_Font8x16);
       EDIT_SetTextColor(hEdit4, EDIT_CI_DISABLED, GUI_BLACK);
       EDIT_SetTextAlign(hEdit4,GUI_TA_HCENTER | GUI_TA_VCENTER);
       EDIT_SetText(hEdit4, "1");
       
       hButton8 = BUTTON_CreateEx(XPOS_R1, 220, 120, 70, hWin, WM_CF_SHOW, 0, GUI_ID_USER_EDIT3);
       BUTTON_SetText(hButton8, "-");
       WM_SetCallback(hButton8, _cbButtonAndEdit);
       //WM_DisableMemdev(hButton8);
       //WM_SetHasTrans(hButton8);

       /*#####################################################################*/
       
       hButton9 = BUTTON_CreateEx(XPOS_R2, 80, 120, 70, hWin, WM_CF_SHOW, 0, GUI_ID_USER_EDIT6);
       BUTTON_SetText(hButton9, "+");
       WM_SetCallback(hButton9, _cbButtonAndEdit);
       //WM_DisableMemdev(hButton9);
       //WM_SetHasTrans(hButton9);
       
       hEdit5 = EDIT_CreateEx(XPOS_R2, 150, 122, 70, hWin, WM_CF_SHOW, 1, GUI_ID_EDIT2, TEXT_MAXLEN);
       EDIT_SetFont(hEdit5, &GUI_Font8x16);
       EDIT_SetTextColor(hEdit5, EDIT_CI_DISABLED, GUI_BLACK);
       EDIT_SetTextAlign(hEdit5,GUI_TA_HCENTER | GUI_TA_VCENTER);
       EDIT_SetText(hEdit5, "2");
       
       hButton10 = BUTTON_CreateEx(XPOS_R2, 220, 120, 70, hWin, WM_CF_SHOW, 0, GUI_ID_USER_EDIT5);
       BUTTON_SetText(hButton10, "-");
       WM_SetCallback(hButton10, _cbButtonAndEdit);
       //WM_DisableMemdev(hButton10);
       //WM_SetHasTrans(hButton10);

       /*#####################################################################*/

       GUI_mutex_release();
       break;
   case WM_PAINT:
       GUI_EnableAlpha(1);
       GUI_SetBkColor(0x60000000);
       GUI_Clear();
       GUI_SetColor(GUI_LIGHTGRAY);
       GUI_DrawRect(1,1,449,449);
       GUI_DrawRect(2,2,448,448);
       GUI_SetColor(0x60000000);                       // 0x60000000 ÑÕÉ«´øÍ¸Ã÷¶È,ÆäÖÐ0x60ÎªAphaÖµ
       GUI_FillRect(2,40,440,440);
       GUI_SetColor(GUI_WHITE);
       GUI_AA_FillCircle(45,45,15);
       GUI_SetColor(GUI_BLACK);
       GUI_SetTextMode(GUI_TM_TRANS);
       GUI_SetFont(&GUI_Font8x16_ASCII);
       GUI_DispStringAt("i",40,40);
       GUI_EnableAlpha(0);

       Rect.x0 = 60;
       Rect.y0 = 20;
       Rect.x1 = 390;
       Rect.y1 = 90;
       GUI_MoveRect(&Rect, 0, 330);
       GUI_DrawGradientV(Rect.x0, Rect.y0, Rect.x1, Rect.y1, GUI_LIGHTGRAY, GUI_GRAY);
       break;
  }
}


static void _cbBkEnFacSet(WM_MESSAGE * pMsg) {
  WM_HWIN hWin;
  static WM_HWIN hButton, hButton1;
  char initTimSetCtrl = NULL;
  GUI_RECT Rect;
  int xSize, i, NCode;
  int Id = 0;

  hWin = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);      // Id of widget
    NCode = pMsg->Data.v;                 // Notification code
    
	switch (NCode) {
        case WM_NOTIFICATION_CLICKED:
            switch (Id) {
                case GUI_ID_USER_FAC_YES: // YES
                    WM_DeleteWindow(hWinVerFac);
                    WM_DeleteWindow(hButton);
					break;
                case GUI_ID_USER_FAC_NO:  // NO
                    WM_DeleteWindow(hWinVerFac);
                    WM_DeleteWindow(hButton);
                    break;
                }
            break;
        case WM_NOTIFICATION_RELEASED:
            break;
        }
       break;
   case WM_CREATE:
       hButton = BUTTON_CreateEx(XPOS_R2, 220, 120, 70, hWin, WM_CF_SHOW, 0, GUI_ID_USER_FAC_YES);
       BUTTON_SetText(hButton, "NO");
       WM_SetCallback(hButton, _cbButtonAndEdit);
       WM_DisableMemdev(hButton);
       WM_SetHasTrans(hButton);

       hButton = BUTTON_CreateEx(XPOS_ + 100, 330, 120, 70, hWin, WM_CF_SHOW, 0, GUI_ID_USER_FAC_YES);
       BUTTON_SetText(hButton, "YES");
       WM_SetCallback(hButton, _cbButtonAndEdit);
       WM_DisableMemdev(hButton);
       WM_SetHasTrans(hButton);
       break;
   case WM_PAINT:
       GUI_EnableAlpha(1);
       GUI_SetBkColor(0x60000000);
       GUI_Clear();
       GUI_SetColor(GUI_LIGHTGRAY);
       GUI_DrawRect(1,1,449,449);
       GUI_DrawRect(2,2,448,448);
       GUI_SetColor(0x60000000);                       // 0x60000000 ÑÕÉ«´øÍ¸Ã÷¶È,ÆäÖÐ0x60ÎªAphaÖµ
       GUI_FillRect(2,40,440,440);
       GUI_SetColor(GUI_WHITE);
       GUI_AA_FillCircle(45,45,15);
       GUI_SetColor(GUI_BLACK);
       GUI_SetTextMode(GUI_TM_TRANS);
       GUI_SetFont(&GUI_Font8x16_ASCII);
       GUI_DispStringAt("i",40,40);
       GUI_EnableAlpha(0);

       Rect.x0 = 60;
       Rect.y0 = 20;
       Rect.x1 = 390;
       Rect.y1 = 90;
       GUI_MoveRect(&Rect, 0, 330);
       GUI_DrawGradientV(Rect.x0, Rect.y0, Rect.x1, Rect.y1, GUI_LIGHTGRAY, GUI_GRAY);
       break;
  }
}


static void _cbBkPMSet(WM_MESSAGE * pMsg) {
  WM_HWIN   hWin,hButton,hButton0,hButton1,hButton2;
  int xSize, i, NCode ,Id;
  static int IdButton = MSG_CHANGE_M;
  static char initCtrl = NULL;
  GUI_RECT Rect;
  char aBuffer[TEXT_MAXLEN];

  hWin = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);      // Id of widget
    NCode = pMsg->Data.v;                 // Notification code

	switch (NCode) {
	case WM_NOTIFICATION_CLICKED:
        switch (Id) {
            case GUI_ID_USER_DET_M:
    	        IdButton = MSG_CHANGE_M;
        	    break;
            case GUI_ID_USER_DET_1:
    	        IdButton = MSG_CHANGE_1;
        	    break;
            case GUI_ID_USER_1_TS:
                uiWinCtl.msgButtonFlag = true;
                uiWinCtl.msgButton1Flag = true;
                break;
            case GUI_ID_USER_DET_2:
    	        IdButton = MSG_CHANGE_2;
                break;
            case GUI_ID_USER_2_0_TS:
                uiWinCtl.msgButtonFlag = true;
                uiWinCtl.msgButton20Flag = true;
                break;
            case GUI_ID_USER_2_1_TS:
                uiWinCtl.msgButtonFlag = true;
                uiWinCtl.msgButton21Flag = true;
                break;
            case GUI_ID_USER_DET_4:
    	        IdButton = MSG_CHANGE_3;
                break;
            case GUI_ID_USER_4_0_TS:
                uiWinCtl.msgButtonFlag = true;
                uiWinCtl.msgButton40Flag = true;
                break;
            case GUI_ID_USER_4_1_TS:
                uiWinCtl.msgButtonFlag = true;
                uiWinCtl.msgButton41Flag = true;
                break;
            case GUI_ID_USER_4_2_TS:
                uiWinCtl.msgButtonFlag = true;
                uiWinCtl.msgButton42Flag = true;
                break;
            case GUI_ID_USER_4_3_TS:
                uiWinCtl.msgButtonFlag = true;
                uiWinCtl.msgButton43Flag = true;
                break;
        }  
        WM_InvalidateWindow(hWin);
        break;
    case WM_NOTIFICATION_RELEASED:
        break;
  }
  case WM_CREATE:
    if(!initCtrl || uiWinCtl.msgWMCreatFlag) {
        initCtrl = true;
        uiWinCtl.msgWMCreatFlag = false;

        i = GUI_ID_USER_DET_M;
        hButton = BUTTON_CreateUser(XPOS_BUT,  YPOS_BUT, 290, 90, hWin, WM_CF_SHOW, 0, GUI_ID_USER_DET_M, sizeof(i));
    	BUTTON_SetSkin(hButton, _cbButtonUsrParam);
        BUTTON_SetUserData(hButton, &i, sizeof(i));

        i = GUI_ID_USER_DET_1;
        hButton = BUTTON_CreateUser(XPOS_BUT, YPOS_BUT + 70, 290, 90, hWin, WM_CF_SHOW, 0, GUI_ID_USER_DET_1, sizeof(i));
    	BUTTON_SetSkin(hButton, _cbButtonUsrParam);
        BUTTON_SetUserData(hButton, &i, sizeof(i));

        i = GUI_ID_USER_1_TS;
        hButton0 = BUTTON_CreateUser(400, YPOS_BUT + 70, 140, 90, hWin, WM_CF_SHOW, 0, GUI_ID_USER_1_TS, sizeof(i));
    	BUTTON_SetSkin(hButton0, _cbButtonUsrtimSet);
        BUTTON_SetUserData(hButton0, &i, sizeof(i));

        i = GUI_ID_USER_DET_2;
        hButton = BUTTON_CreateUser(XPOS_BUT, YPOS_BUT + 140, 290, 90, hWin, WM_CF_SHOW, 0, GUI_ID_USER_DET_2, sizeof(i));
    	BUTTON_SetSkin(hButton, _cbButtonUsrParam);
        BUTTON_SetUserData(hButton, &i, sizeof(i));

        i = GUI_ID_USER_2_0_TS;
        hButton0 = BUTTON_CreateUser(400, YPOS_BUT + 140, 140, 90, hWin, WM_CF_SHOW, 0, GUI_ID_USER_2_0_TS, sizeof(i));
    	BUTTON_SetSkin(hButton0, _cbButtonUsrtimSet);
        BUTTON_SetUserData(hButton0, &i, sizeof(i));
        
        i = GUI_ID_USER_2_1_TS;
        hButton0 = BUTTON_CreateUser(560, YPOS_BUT + 140, 140, 90, hWin, WM_CF_SHOW, 0, GUI_ID_USER_2_1_TS, sizeof(i));
    	BUTTON_SetSkin(hButton0, _cbButtonUsrtimSet);
        BUTTON_SetUserData(hButton0, &i, sizeof(i));
        
        i = GUI_ID_USER_DET_4;
        hButton = BUTTON_CreateUser(XPOS_BUT, YPOS_BUT + 210, 290, 90, hWin, WM_CF_SHOW, 0, GUI_ID_USER_DET_4, sizeof(i));
    	BUTTON_SetSkin(hButton, _cbButtonUsrParam);
        BUTTON_SetUserData(hButton, &i, sizeof(i));

        i = GUI_ID_USER_4_0_TS;
        hButton1 = BUTTON_CreateUser(400, YPOS_BUT + 210, 140, 90, hWin, WM_CF_SHOW, 0, GUI_ID_USER_4_0_TS, sizeof(i));
    	BUTTON_SetSkin(hButton1, _cbButtonUsrtimSet);
        BUTTON_SetUserData(hButton1, &i, sizeof(i));

        i = GUI_ID_USER_4_1_TS;
        hButton1 = BUTTON_CreateUser(560, YPOS_BUT + 210, 140, 90, hWin, WM_CF_SHOW, 0, GUI_ID_USER_4_1_TS, sizeof(i));
    	BUTTON_SetSkin(hButton1, _cbButtonUsrtimSet);
        BUTTON_SetUserData(hButton1, &i, sizeof(i));

        i = GUI_ID_USER_4_2_TS;
        hButton1 = BUTTON_CreateUser(400, YPOS_BUT + 280, 140, 90, hWin, WM_CF_SHOW, 0, GUI_ID_USER_4_2_TS, sizeof(i));
    	BUTTON_SetSkin(hButton1, _cbButtonUsrtimSet);
        BUTTON_SetUserData(hButton1, &i, sizeof(i));

        i = GUI_ID_USER_4_3_TS;
        hButton1 = BUTTON_CreateUser(560, YPOS_BUT + 280, 140, 90, hWin, WM_CF_SHOW, 0, GUI_ID_USER_4_3_TS, sizeof(i));
    	BUTTON_SetSkin(hButton1, _cbButtonUsrtimSet);
        BUTTON_SetUserData(hButton1, &i, sizeof(i));
    }
    /*####################################################################################################*/
    if(uiWinCtl.msgButtonFlag) {
        uiWinCtl.msgButtonFlag = false;
        hWinTimSet = WM_CreateWindowAsChild(30, 0, 680, 650, hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbBktimSet, NULL);
    }
    /*####################################################################################################*/
    break;
  case WM_PAINT:
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    
    if(!initCtrl || uiWinCtl.msgWMCreatFlag) {
        WM_SendMessageNoPara(hWin, WM_CREATE);
    }
    
    GUI_SetColor(GUI_WHITE);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(&GUI_FontPingFangRegular67); 
    // ·Û³¾×Ô¶¯¼ì²â¼ä¸ô
	GUI_DispStringAt("\xe7\xb2\x89\xe5\xb0\x98\xe8\x87\xaa\xe5\x8a\xa8\xe6\xa3\x80\xe6\xb5\x8b\xe9\x97\xb4\xe9\x9a\x94",30,10);
	GUI_SetPenSize(6);
    
    switch (IdButton) {
        case MSG_CHANGE_M:
        	GUI_AA_FillCircle(XPOS_RAD,YPOS_RAD,10);
        	GUI_AA_DrawArc(XPOS_RAD,YPOS_RAD,18,18,0,360);
        	GUI_AA_DrawArc(XPOS_RAD,YPOS_RAD + 70,18,18,0,360);
        	GUI_AA_DrawArc(XPOS_RAD,YPOS_RAD + 140,18,18,0,360);
        	GUI_AA_DrawArc(XPOS_RAD,YPOS_RAD + 210,18,18,0,360);
        	GUI_SetColor(GUI_BLACK);
        	GUI_AA_FillCircle(XPOS_RAD,YPOS_RAD + 70,10);
        	GUI_AA_FillCircle(XPOS_RAD,YPOS_RAD + 140,10);
        	GUI_AA_FillCircle(XPOS_RAD,YPOS_RAD + 210,10);
            break;
        case MSG_CHANGE_1:
            GUI_AA_FillCircle(XPOS_RAD,YPOS_RAD + 70,10); 
        	GUI_AA_DrawArc(XPOS_RAD,YPOS_RAD,18,18,0,360);
        	GUI_AA_DrawArc(XPOS_RAD,YPOS_RAD + 70,18,18,0,360);
        	GUI_AA_DrawArc(XPOS_RAD,YPOS_RAD + 140,18,18,0,360);
        	GUI_AA_DrawArc(XPOS_RAD,YPOS_RAD + 210,18,18,0,360);
        	GUI_SetColor(GUI_BLACK);
            GUI_AA_FillCircle(XPOS_RAD,YPOS_RAD,10);
        	GUI_AA_FillCircle(XPOS_RAD,YPOS_RAD + 140,10);
        	GUI_AA_FillCircle(XPOS_RAD,YPOS_RAD + 210,10);
            break;
        case MSG_CHANGE_2:
            GUI_AA_FillCircle(XPOS_RAD,YPOS_RAD + 140,10);
        	GUI_AA_DrawArc(XPOS_RAD,YPOS_RAD,18,18,0,360);
        	GUI_AA_DrawArc(XPOS_RAD,YPOS_RAD + 70,18,18,0,360);
        	GUI_AA_DrawArc(XPOS_RAD,YPOS_RAD + 140,18,18,0,360);
        	GUI_AA_DrawArc(XPOS_RAD,YPOS_RAD + 210,18,18,0,360);
        	GUI_SetColor(GUI_BLACK);
            GUI_AA_FillCircle(XPOS_RAD,YPOS_RAD,10);
            GUI_AA_FillCircle(XPOS_RAD,YPOS_RAD + 70,10);
        	GUI_AA_FillCircle(XPOS_RAD,YPOS_RAD + 210,10);
            break;
        case MSG_CHANGE_3:
            GUI_AA_FillCircle(XPOS_RAD,YPOS_RAD + 210,10);
        	GUI_AA_DrawArc(XPOS_RAD,YPOS_RAD,18,18,0,360);
        	GUI_AA_DrawArc(XPOS_RAD,YPOS_RAD + 70,18,18,0,360);
        	GUI_AA_DrawArc(XPOS_RAD,YPOS_RAD + 140,18,18,0,360);
        	GUI_AA_DrawArc(XPOS_RAD,YPOS_RAD + 210,18,18,0,360);
        	GUI_SetColor(GUI_BLACK);
            GUI_AA_FillCircle(XPOS_RAD,YPOS_RAD,10);
            GUI_AA_FillCircle(XPOS_RAD,YPOS_RAD + 70,10);
        	GUI_AA_FillCircle(XPOS_RAD,YPOS_RAD + 140,10);
            break;
        }
    break;
    default:
        WM_DefaultProc(pMsg);
  }
}

static void _cbBkSysSet(WM_MESSAGE * pMsg) {
  WM_HWIN   hWin,hButton,hButton0,hButton1,hButton2;
  int xSize, i, NCode ,Id;
  static int Value;
  static int IdButton = MSG_CHANGE_1;
  static char initCtrl = NULL;
  static char wifiCtrl = NULL;
  GUI_RECT Rect;
  char aBuffer[TEXT_MAXLEN];

  memset(aBuffer, NULL, sizeof(aBuffer));
  hWin = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);      // Id of widget
    NCode = pMsg->Data.v;                 // Notification code
    
	switch (NCode) {
    case WM_NOTIFICATION_VALUE_CHANGED:
        Value = SLIDER_GetValue(pMsg->hWinSrc);
        //rt_kprintf("SLIDER_GetValue -------------- %d ---------------\r\n",Value);
        WM_InvalidateWindow(hWin);
      break;
	case WM_NOTIFICATION_CLICKED:
        switch (Id) {
            case GUI_ID_USER_DET_M:
    	        wifiCtrl ^= true;
        	    break;
            case GUI_ID_USER_DET_1:
    	        IdButton = MSG_CHANGE_1;    // ³¤ÁÁ°´¼ü
        	    break;
            case GUI_ID_USER_DET_2:      
    	        IdButton = MSG_CHANGE_2;    // Ò»·ÖÖÓ³¤ÁÁ
                break;
            case GUI_ID_USER_1_TS:
                uiWinCtl.msgNetButtonFlag = true;
                uiWinCtl.msgNetTimButtonFlag = true;
            break;
            case GUI_ID_USER_2_0_TS:
                uiWinCtl.msgNet1ButtonFlag = true;
                uiWinCtl.msgNetTimButton1Flag = true;
            break;
        }
        WM_InvalidateWindow(hWin);
        break;
    case WM_NOTIFICATION_RELEASED:
        break;
	}
  case WM_CREATE:
    if(!initCtrl || uiWinCtl.msgWMNetCreatFlag) {
        initCtrl = true;
        uiWinCtl.msgWMNetCreatFlag = false;

        i = GUI_ID_USER_DET_M; // wifi
        hButton = BUTTON_CreateUser(180,  280, 120, 70, hWin, WM_CF_SHOW, 0, GUI_ID_USER_DET_M, sizeof(i));
    	BUTTON_SetSkin(hButton, _cbButtonSysParam);
        BUTTON_SetUserData(hButton, &i, sizeof(i));

        i = GUI_ID_USER_DET_1; // ³¤ÁÁ
        hButton = BUTTON_CreateUser(90, 570, 120, 70, hWin, WM_CF_SHOW, 0, GUI_ID_USER_DET_1, sizeof(i));
    	BUTTON_SetSkin(hButton, _cbButtonSysParam);
        BUTTON_SetUserData(hButton, &i, sizeof(i));

        i = GUI_ID_USER_DET_2; // Ò»·ÖÖÓ³¤ÁÁ
        hButton = BUTTON_CreateUser(100 + 267, 570, 200, 70, hWin, WM_CF_SHOW, 0, GUI_ID_USER_DET_2, sizeof(i));
    	BUTTON_SetSkin(hButton, _cbButtonSysParam);
        BUTTON_SetUserData(hButton, &i, sizeof(i));

        i = GUI_ID_USER_1_TS;
        hButton0 = BUTTON_CreateUser(300, 80, 250, 70, hWin, WM_CF_SHOW, 0, GUI_ID_USER_1_TS, sizeof(i));
    	BUTTON_SetSkin(hButton0, _cbButtonSystimSet);
        BUTTON_SetUserData(hButton0, &i, sizeof(i));

        i = GUI_ID_USER_2_0_TS;
        hButton0 = BUTTON_CreateUser(300, 150, 140, 70, hWin, WM_CF_SHOW, 0, GUI_ID_USER_2_0_TS, sizeof(i));
    	BUTTON_SetSkin(hButton0, _cbButtonSystimSet);
        BUTTON_SetUserData(hButton0, &i, sizeof(i));
    }
    /*####################################################################################################*/
    if(uiWinCtl.msgNetButtonFlag) {
        uiWinCtl.msgNetButtonFlag = false;
        hWinTimSet = WM_CreateWindowAsChild(30, 0, 680, 650, hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbBkNetDatSysSet, NULL);
    }
    else if(uiWinCtl.msgNet1ButtonFlag) {
        uiWinCtl.msgNet1ButtonFlag = false;
        hWinTimSet = WM_CreateWindowAsChild(30, 0, 680, 650, hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbBktimSet, NULL);
    }
    /*####################################################################################################*/
    break;
  case WM_PAINT:
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    GUI_SetColor(GUI_WHITE);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(&GUI_Fonthanyi48);
    GUI_DispStringAt("\xe6\x97\xb6\xe9\x97\xb4\xe8\xae\xbe\xe7\xbd\xae",30,10); // Ê±¼äÉèÖÃ
    GUI_DispStringAt("\xe6\x97\xa5\xe6\x9c\x9f",130,10 + 70); // ÈÕÆÚ
    GUI_DispStringAt("\xe6\x97\xb6\xe9\x97\xb4",130,10 + 70*2); // Ê±¼ä
    GUI_DispStringAt("\xe7\xbd\x91\xe7\xbb\x9c\xe8\xae\xbe\xe7\xbd\xae",30,10 + 70*3); // ÍøÂçÉèÖÃ
    GUI_DispCharAt(0xFFFF,130,10 + 70*4);   // "·½¿ò"
    GUI_DispStringAt("\xe5\xb1\x8f\xe5\xb9\x95\xe4\xba\xae\xe5\xba\xa6",30,10 + 70*5); // ÆÁÄ»ÁÁ¶È
    GUI_DispStringAt("\xe5\xb1\x8f\xe5\xb9\x95\xe8\x87\xaa\xe5\x8a\xa8\xe5\x85\xb3\xe9\x97\xad\xe6\x97\xb6\xe9\x97\xb4",30,10 + 70*7); // ÆÁÄ»×Ô¶¯¹Ø±ÕÊ±¼ä
    sprintf(aBuffer, "%d",Value);
    GUI_DispStringAt(aBuffer,545,15 + 70*6);
    
    if(!initCtrl || uiWinCtl.msgWMNetCreatFlag){
        WM_SendMessageNoPara(hWin, WM_CREATE);
    }

    if(wifiCtrl){
        GUI_DispCharAt(0xFFFF,130,10 + 70*4);   // "·½¿ò"
        GUI_DispCharAt(0x221A,130,10 + 70*4);   // "¶ÔºÅ"
    }
    else
        GUI_DispCharAt(0xFFFF,130,10 + 70*4);   // "·½¿ò"
        
    GUI_SetPenSize(6);
    switch (IdButton) {
        case MSG_CHANGE_1:
        	GUI_AA_FillCircle(XPOS_SYS_RAD,YPOS_SYS_RAD,10);
        	GUI_AA_DrawArc(XPOS_SYS_RAD   ,YPOS_SYS_RAD,18,18,0,360);
        	GUI_AA_DrawArc(XPOS_SYS_RAD + XPOS_SYS_OFFSET,YPOS_SYS_RAD,18,18,0,360);
        	GUI_SetColor(GUI_BLACK);
            GUI_AA_FillCircle(XPOS_SYS_RAD + XPOS_SYS_OFFSET,YPOS_SYS_RAD,10); 
            break;
        case MSG_CHANGE_2:
            GUI_AA_FillCircle(XPOS_SYS_RAD + XPOS_SYS_OFFSET,YPOS_SYS_RAD,10); 
        	GUI_AA_DrawArc(XPOS_SYS_RAD,YPOS_SYS_RAD,18,18,0,360);
        	GUI_AA_DrawArc(XPOS_SYS_RAD + XPOS_SYS_OFFSET,YPOS_SYS_RAD,18,18,0,360);
        	GUI_SetColor(GUI_BLACK);
            GUI_AA_FillCircle(XPOS_SYS_RAD,YPOS_SYS_RAD,10);
            break;
        }
	break;
    default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _cbIntro
*/
static void _cbIntro(WM_MESSAGE * pMsg) {
  GUI_RECT Rect = {0};
  int      xSize;
  int      ySize;

  switch (pMsg->MsgId) {
  case WM_PAINT:
    GUI_EnableAlpha(1);
    xSize = WM_GetWindowSizeX(pMsg->hWin);
    ySize = WM_GetWindowSizeY(pMsg->hWin);
    Rect.x0 = 5;
    Rect.y0 = 5;
    Rect.x1 = xSize - 11;
    Rect.y1 = ySize - 11;
    GUI_SetColor(GUI_GRAY);
    GUI_DrawRect(0, 0, xSize - 2, ySize - 2);
    GUI_SetColor(GUI_DARKGRAY);
    GUI_DrawRect(1, 1, xSize - 1, ySize - 1);
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(&GUI_Font8x16_ASCII);
    GUI_SetColor(GUI_LIGHTGRAY);
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_DispStringInRectWrap("test...", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER, GUI_WRAPMODE_WORD);
    GUI_EnableAlpha(0);
    break;
  }
}

/*********************************************************************
*
*       _DrawSkinFlex
*
* Function description
*   Skinning function for slider widget
*/
static int _DrawSkinFlex(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) {
  int x;
  int y;

  switch (pDrawItemInfo->Cmd) {
  case WIDGET_ITEM_DRAW_THUMB:
    GUI_SetColor(GUI_WHITE);
    GUI_AA_FillRoundedRect(pDrawItemInfo->x0, pDrawItemInfo->y1 / 2 - 18, pDrawItemInfo->x1, pDrawItemInfo->y1 - 18, 3);
    GUI_SetColor(GUI_WHITE);
    GUI_AA_DrawRoundedRect(pDrawItemInfo->x0, pDrawItemInfo->y1 / 2 - 18, pDrawItemInfo->x1, pDrawItemInfo->y1 - 18, 3);
    return 0;
  case WIDGET_ITEM_DRAW_TICKS:
    return 0;
  case WIDGET_ITEM_DRAW_FOCUS:
    return 0;
  case WIDGET_ITEM_DRAW_SHAFT:
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    y = (pDrawItemInfo->y1 / 6) * 2;
    x = pDrawItemInfo->x0;
    GUI_DrawGradientRoundedH(x, y, x + 500, y + 25,7,GUI_BLACK,GUI_WHITE);   
    return 0;
  default:
    return SLIDER_DrawSkinFlex(pDrawItemInfo);
  }
}

static void _cbButtonEnSet(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) {
  WM_HWIN  hWin;
  int Index;
  
  hWin = pDrawItemInfo->hWin;
  switch (pDrawItemInfo->Cmd) {
  case WIDGET_ITEM_DRAW_BACKGROUND:
    BUTTON_GetUserData(hWin, &Index, sizeof(Index));
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    GUI_SetColor(GUI_WHITE);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(&GUI_Fonthanyi48);
    if(Index == GUI_ID_USER_FAC)
        GUI_DispStringAt("\xe6\x81\xa2\xe5\xa4\x8d\xe5\x87\xba\xe5\x8e\x82\xe8\xae\xbe\xe7\xbd\xae",0,0); 	// »Ö¸´³ö³§ÉèÖÃ
    else if(Index == GUI_ID_USER_DONE) {
        GUI_SetColor(GUI_MAGENTA);
        GUI_AA_FillRoundedRect(0,0,120,70,10);
        GUI_SetColor(GUI_WHITE);
        GUI_DispStringAt("\xe7\xa1\xae\xe5\xae\x9a",7,7); 	// È·¶¨
    }
    break;
  default:
    break;
  }
}

static int _ButtonSkinPageEnPMCount(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) {
  int Index,IsPressed;
  WM_HWIN hWin;
  GUI_COLOR Color;
  
  hWin = pDrawItemInfo->hWin;
  switch (pDrawItemInfo->Cmd) {
  case WIDGET_ITEM_DRAW_BACKGROUND:
    //
    // Get state of button
    //
    IsPressed = BUTTON_IsPressed(pDrawItemInfo->hWin);
    //
    // Get button index
    //
    BUTTON_GetUserData(hWin, &Index, sizeof(Index));
    //
    // Draw background color depending on state
    //
    if (IsPressed) {
      Color = GUI_MAGENTA;
    } else {
      Color = GUI_BLACK;
    }
    GUI_SetBkColor(Color);
    GUI_Clear();
    //
    // Draw arrow at the right
    //
    GUI_SetColor(GUI_WHITE);
	GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetFont(&GUI_Fonthanyi43);
	GUI_UC_SetEncodeUTF8();
	if(Index == GUI_ID_USER_NPOL){
		GUI_DispStringAt("\xe6\x97\xa0\xe6\xb1\xa1\xe6\x9f\x93",1,20); // ÎÞÎÛÈ¾
        GUI_SetTextAlign(GUI_TA_RIGHT);
		GUI_DispStringAt("10 \xe6\xac\xa1",720,20);
	}
	else if(Index == GUI_ID_USER_LPOL){
		GUI_DispStringAt("\xe8\xbd\xbb\xe6\xb1\xa1\xe6\x9f\x93",1,20); // ÇáÎÛÈ¾
		GUI_SetTextAlign(GUI_TA_RIGHT);
		GUI_DispStringAt("20 \xe6\xac\xa1",720,20);
	}
	else if(Index == GUI_ID_USER_MPOL){
		GUI_DispStringAt("\xe4\xb8\xad\xe6\xb1\xa1\xe6\x9f\x93",1,20); // ÖÐÎÛÈ¾
		GUI_SetTextAlign(GUI_TA_RIGHT);
		GUI_DispStringAt("100 \xe6\xac\xa1",720,20);
	}
    else if(Index == GUI_ID_USER_HPOL){
		GUI_DispStringAt("\xe9\x87\x8d\xe6\xb1\xa1\xe6\x9f\x93",1,20); // ÖØÎÛÈ¾
		GUI_SetTextAlign(GUI_TA_RIGHT);
		GUI_DispStringAt("1000 \xe6\xac\xa1",720,20);
    }
    else if(Index == GUI_ID_USER_CHPOL){
		GUI_DispStringAt("\xe6\x8a\x98\xe7\xae\x97\xe9\x87\x8d\xe6\xb1\xa1\xe6\x9f\x93",1,20); // ÕÛËãÖØÎÛÈ¾
		GUI_SetTextAlign(GUI_TA_RIGHT);
		GUI_DispStringAt("3000 \xe6\xac\xa1",720,20);
    }
    else if(Index == GUI_ID_USER_RCHPOL){
		GUI_DispStringAt("\xe5\x89\xa9\xe4\xbd\x99\xe6\x8a\x98\xe7\xae\x97\xe9\x87\x8d\xe6\xb1\xa1\xe6\x9f\x93",1,20);
        GUI_SetTextAlign(GUI_TA_RIGHT);
		GUI_DispStringAt("4000 \xe6\xac\xa1",720,20);// Ê£ÓàÕÛËãÖØÎÛÈ¾
    }
    break;
  }
  return 0;
}


static void _cbBkEnVerSet(WM_MESSAGE * pMsg) {
  WM_HWIN hWin, hButton;
  
  int xSize, i, NCode;
  int Id = 0;
  static char initCtrl = NULL;
  static char FacCtrl = NULL;

  hWin = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);      // Id of widget
    NCode = pMsg->Data.v;                 // Notification code

	switch (NCode) {
	case WM_NOTIFICATION_CLICKED:
        switch (Id) {
            case GUI_ID_USER_FAC:
                FacCtrl = true;
        	    break;
        }
        break;
    case WM_NOTIFICATION_RELEASED:
        break;
	}
  case WM_CREATE:
    if(!initCtrl || uiWinCtl.msgWMVerFacCtlFlag) {
        initCtrl = true;
        uiWinCtl.msgWMVerFacCtlFlag = false;

        i = GUI_ID_USER_FAC; 
        hButton = BUTTON_CreateUser(30,  500, 300, 70, hWin, WM_CF_SHOW, 0, GUI_ID_USER_FAC, sizeof(i));
    	BUTTON_SetSkin(hButton, _cbButtonEnSet);
        BUTTON_SetUserData(hButton, &i, sizeof(i));

    }
    /*####################################################################################################*/
    if(FacCtrl) {
        FacCtrl = false;
        hWinVerFac = WM_CreateWindowAsChild(30, 0, 680, 650, hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbBkEnFacSet, NULL);
    }
    /*####################################################################################################*/
    break;
  case WM_PAINT:
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
	GUI_SetColor(GUI_WHITE);
	GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetFont(&GUI_Fonthanyi48);
    GUI_DispStringAt("\xe7\xa8\x8b\xe5\xba\x8f\xe7\x89\x88\xe6\x9c\xac\xe5\x8f\xb7 :", 30, 15); // ³ÌÐò°æ±¾ºÅ:
    GUI_DispStringAt("MAC\xe5\x9c\xb0\xe5\x9d\x80 :", 30, 85); // MACµØÖ·£º

    GUI_QR_Draw(hQR,150, 160);
    
    if(!initCtrl || uiWinCtl.msgWMVerFacCtlFlag){
        WM_SendMessageNoPara(hWin, WM_CREATE);
    }
	break;
  default:
    WM_DefaultProc(pMsg);
  }
}

static void _cbBkEnLagSet(WM_MESSAGE * pMsg) {
  WM_HWIN hWin, hButton;
  static WM_HWIN hIntro = NULL;
  int xSize, i, NCode;
  int Id = 0;
  static EDIT_Handle hEdit, hEdit1;
  static char initCtrl = NULL;
  static char doneCtrl = NULL;

  hWin = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);      // Id of widget
    NCode = pMsg->Data.v;                 // Notification code

	switch (NCode) {
    case WM_NOTIFICATION_VALUE_CHANGED: // EDIT GOT VALUE
        switch (Id) {
            case GUI_ID_EDIT4:
        	    break;
            case GUI_ID_EDIT5:
        	    break;
        }
        break;
    case WM_NOTIFICATION_GOT_FOCUS: // EDIT GOT FOCUS
        switch (Id) {
            case GUI_ID_EDIT4:
        	    break;
            case GUI_ID_EDIT5:
        	    break;
        }
        break;
	case WM_NOTIFICATION_CLICKED:
        switch (Id) {
            case GUI_ID_USER_DONE:
                doneCtrl = true;
        	    break;
        }
        break;
    case WM_NOTIFICATION_RELEASED:
        break;
	}
  case WM_CREATE:
    if(!initCtrl || uiWinCtl.msgWMLagEditCtlFlag) {
        initCtrl = true;
        uiWinCtl.msgWMLagEditCtlFlag = false;
        
        hEdit  = EDIT_CreateEx(170, 80, 300, 60, hWin, WM_CF_SHOW, 1, GUI_ID_EDIT4, TEXT_MAXLEN);
        EDIT_SetFont(hEdit, &GUI_Fonthanyi48);
        EDIT_SetTextAlign(hEdit,GUI_TA_HCENTER | GUI_TA_VCENTER);
        EDIT_SetText(hEdit, "192.168.1.1");
        EDIT_SetTextColor(hEdit, 1, GUI_BLACK);
        hEdit1 = EDIT_CreateEx(170, 150, 150, 60, hWin, WM_CF_SHOW, 1, GUI_ID_EDIT5, TEXT_MAXLEN);
        EDIT_SetFont(hEdit1, &GUI_Fonthanyi48);
        EDIT_SetTextAlign(hEdit1,GUI_TA_HCENTER | GUI_TA_VCENTER);
        EDIT_SetText(hEdit1, "8080");
        EDIT_SetTextColor(hEdit1, 1, GUI_BLACK);

        i = GUI_ID_USER_DONE; 
        hButton = BUTTON_CreateUser(250,  260, 150, 70, hWin, WM_CF_SHOW, 0, GUI_ID_USER_DONE, sizeof(i));
    	BUTTON_SetSkin(hButton, _cbButtonEnSet);
        BUTTON_SetUserData(hButton, &i, sizeof(i));
    }
    else if(doneCtrl) {
        doneCtrl = false;

        hIntro = WM_CreateWindowAsChild(30, 400, 100, 70, hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbIntro, NULL);
        GUI_MEMDEV_FadeInWindow(hIntro, 1000);
        GUI_Delay(1000);
        GUI_MEMDEV_FadeOutWindow(hIntro, 1000);
        uiWinCtl.msgWMPmcCEdittlFlag = true;
    }
    break;
  case WM_PAINT:
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
	GUI_SetColor(GUI_WHITE);
	GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetFont(&GUI_Fonthanyi48);
    GUI_DispStringAt("\xe8\xbf\x9c\xe7\xa8\x8b\xe8\xae\xbe\xe7\xbd\xae", 30, 15); // Ô¶³ÌÉèÖÃ
    GUI_DispStringAt("IP\xe5\x9c\xb0\xe5\x9d\x80", 30, 85); // IPµØÖ·
    GUI_DispStringAt("\xe7\xab\xaf\xe5\x8f\xa3", 30, 155); // ¶Ë¿Ú

    if(!initCtrl || uiWinCtl.msgWMLagEditCtlFlag){
        WM_SendMessageNoPara(hWin, WM_CREATE);
    }
	break;
  default:
    WM_DefaultProc(pMsg);
  }  
}

#define XPOS_PM_0FF 90

static void _cbBkEnPmcSet(WM_MESSAGE * pMsg) {
  WM_HWIN hWin, hButton;
  int xSize, i, NCode;
  int Id = 0;
  static char initCtrl = NULL;

  hWin = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);      // Id of widget
    NCode = pMsg->Data.v;                 // Notification code

	switch (NCode) {
	case WM_NOTIFICATION_CLICKED:
        switch (Id) {
            case GUI_ID_USER_PRE:
                WM_DeleteWindow(hWin);
                uiWinCtl.msgWMEnCreatFlag = true; // Ç¿ÖÆ´ò¿ª°æ±¾ÐÅÏ¢½çÃæ
                uiWinCtl.msgWMVerFacCtlFlag = true; // Ç¿ÖÆ¿ªÆô»Ö¸´³ö³§°´¼ü
        	    break;
        }
        break;
    case WM_NOTIFICATION_RELEASED:
        break;
	}
  case WM_CREATE:
       if(!initCtrl || uiWinCtl.msgWMPmcCEdittlFlag) {
        initCtrl = true;
        uiWinCtl.msgWMPmcCEdittlFlag = false;

        i = GUI_ID_USER_PRE;
		hButton = BUTTON_CreateUser(0, 1100, 360, 180, hWin, WM_CF_SHOW, 0, GUI_ID_USER_PRE, sizeof(i));
		BUTTON_SetSkin(hButton, _ButtonSkinPageThreeLeft);
		BUTTON_SetUserData(hButton, &i, sizeof(i));
    
        i = GUI_ID_USER_NPOL; 
        hButton = BUTTON_CreateUser(1,  135,                719, XPOS_PM_0FF - 2, hWin, WM_CF_SHOW, 0, GUI_ID_USER_NPOL, sizeof(i));
    	BUTTON_SetSkin(hButton, _ButtonSkinPageEnPMCount);
        BUTTON_SetUserData(hButton, &i, sizeof(i));

        i = GUI_ID_USER_LPOL; 
        hButton = BUTTON_CreateUser(1, 135 + XPOS_PM_0FF,   719, XPOS_PM_0FF - 2, hWin, WM_CF_SHOW, 0, GUI_ID_USER_LPOL, sizeof(i));
    	BUTTON_SetSkin(hButton, _ButtonSkinPageEnPMCount);
        BUTTON_SetUserData(hButton, &i, sizeof(i));

        i = GUI_ID_USER_MPOL; 
        hButton = BUTTON_CreateUser(1, 135 + XPOS_PM_0FF*2, 719, XPOS_PM_0FF - 2, hWin, WM_CF_SHOW, 0, GUI_ID_USER_MPOL, sizeof(i));
    	BUTTON_SetSkin(hButton, _ButtonSkinPageEnPMCount);
        BUTTON_SetUserData(hButton, &i, sizeof(i));

        i = GUI_ID_USER_HPOL;
        hButton = BUTTON_CreateUser(1, 135 + XPOS_PM_0FF*3, 719, XPOS_PM_0FF - 2, hWin, WM_CF_SHOW, 0, GUI_ID_USER_HPOL, sizeof(i));
    	BUTTON_SetSkin(hButton, _ButtonSkinPageEnPMCount);
        BUTTON_SetUserData(hButton, &i, sizeof(i));

        i = GUI_ID_USER_CHPOL;
        hButton = BUTTON_CreateUser(1, 135 + XPOS_PM_0FF*4, 719, XPOS_PM_0FF - 2, hWin, WM_CF_SHOW, 0, GUI_ID_USER_CHPOL, sizeof(i));
    	BUTTON_SetSkin(hButton, _ButtonSkinPageEnPMCount);
        BUTTON_SetUserData(hButton, &i, sizeof(i));

        i = GUI_ID_USER_RCHPOL;
        hButton = BUTTON_CreateUser(1, 135 + XPOS_PM_0FF*5, 719, XPOS_PM_0FF - 2, hWin, WM_CF_SHOW, 0, GUI_ID_USER_RCHPOL, sizeof(i));
    	BUTTON_SetSkin(hButton, _ButtonSkinPageEnPMCount);
        BUTTON_SetUserData(hButton, &i, sizeof(i));
    }
    break;
  case WM_PAINT:
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    GUI_SetColor(0xff8000);
    GUI_FillRect(0,0,720,135);
	GUI_SetColor(GUI_WHITE);
	GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetFont(&GUI_Fonthanyi48);
    GUI_DispStringHCenterAt("\xe7\xb2\x89\xe5\xb0\x98\xe6\xa3\x80\xe6\xb5\x8b\xe6\xac\xa1\xe6\x95\xb0",360,15); // ·Û³¾¼ì²â´ÎÊý
    GUI_DispStringHCenterAt("2018-10-25 18:15:30",360,75);
    GUI_SetBkColor(GUI_LIGHTGRAY);
    GUI_DrawHLine(135 + XPOS_PM_0FF - 1, 0, 720);
    GUI_DrawHLine(135 + XPOS_PM_0FF*2 - 1, 0, 720);
    GUI_DrawHLine(135 + XPOS_PM_0FF*3 - 1, 0, 720);
    GUI_DrawHLine(135 + XPOS_PM_0FF*4 - 1, 0, 720);
    GUI_DrawHLine(135 + XPOS_PM_0FF*5 - 1, 0, 720);
    GUI_DrawHLine(135 + XPOS_PM_0FF*6 - 1, 0, 720);

    if(!initCtrl || uiWinCtl.msgWMPmcCEdittlFlag){
        WM_SendMessageNoPara(hWin, WM_CREATE);
    }
	break;
  default:
    WM_DefaultProc(pMsg);
  }  
    
}

static void _cbBkEnSet(WM_MESSAGE * pMsg) {
  WM_HWIN hWin, hButton;
  int xSize, i, NCode;
  int Id = 0;
  static char initCtrl = NULL;

  hWin = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);      // Id of widget
    NCode = pMsg->Data.v;                 // Notification code

	switch (NCode) {
	case WM_NOTIFICATION_CLICKED:
        switch (Id) {
            case GUI_ID_USER_BACK:
                BUTTON_STATE = BUTTON_PAGE_THREE_L;
        	    break;
            case GUI_ID_USER_VER:
                WM_Custom_DeleteEnWindow();
                uiWinCtl.msgWMVerCtlFlag = true;
                uiWinCtl.msgWMVerFacCtlFlag = true;
        	    break;
            case GUI_ID_USER_LAG:
                WM_Custom_DeleteEnWindow();
                uiWinCtl.msgWMLagCtlFlag = true;
                uiWinCtl.msgWMLagEditCtlFlag = true;
                break;
            case GUI_ID_USER_PMC:
                WM_Custom_DeleteEnWindow();
                uiWinCtl.msgWMPmcCtlFlag = true;
                uiWinCtl.msgWMPmcCEdittlFlag = true;
                break;
        }
        break;
    case WM_NOTIFICATION_RELEASED:
        break;
	}
  case WM_CREATE:
    if(!initCtrl || uiWinCtl.msgWMEnCreatFlag){
        initCtrl = true;
        uiWinCtl.msgWMEnCreatFlag = false;

        i = GUI_ID_USER_BACK;
		hButton = BUTTON_CreateUser(0, 1100, 360, 180, hWin, WM_CF_SHOW, 0, GUI_ID_USER_BACK, sizeof(i));
		BUTTON_SetSkin(hButton, _ButtonSkinPageThreeLeft);
		BUTTON_SetUserData(hButton, &i, sizeof(i));
        
        i = GUI_ID_USER_VER;
    	hButton = BUTTON_CreateUser(0, 100, 720, 80, hWin, WM_CF_SHOW, 0, GUI_ID_USER_VER, sizeof(i));
    	BUTTON_SetSkin(hButton, _ButtonSkinPageEnSetup);
    	BUTTON_SetUserData(hButton, &i, sizeof(i));

        uiWinCtl.msgWMVerCtlFlag = true; // Ç¿ÖÆ´ò¿ª°æ±¾ÐÅÏ¢½çÃæ
        uiWinCtl.msgWMVerFacCtlFlag = true; // Ç¿ÖÆ¿ªÆô»Ö¸´³ö³§°´¼ü

    	i = GUI_ID_USER_LAG;
    	hButton = BUTTON_CreateUser(0, 180, 720, 80, hWin, WM_CF_SHOW, 0, GUI_ID_USER_LAG, sizeof(i));
    	BUTTON_SetSkin(hButton, _ButtonSkinPageEnSetup);
    	BUTTON_SetUserData(hButton, &i, sizeof(i));

    	i = GUI_ID_USER_PMC;
    	hButton = BUTTON_CreateUser(0, 260, 720, 80, hWin, WM_CF_SHOW, 0, GUI_ID_USER_PMC, sizeof(i));
    	BUTTON_SetSkin(hButton, _ButtonSkinPageEnSetup);
    	BUTTON_SetUserData(hButton, &i, sizeof(i));
    }
    /*####################################################################################################*/
    if(uiWinCtl.msgWMVerCtlFlag) {
        uiWinCtl.msgWMVerCtlFlag = false;
        hWinVer = WM_CreateWindowAsChild(0, 360, 720, 650, hWin, WM_CF_SHOW, _cbBkEnVerSet, NULL);
    }
    else if(uiWinCtl.msgWMLagCtlFlag) {
        uiWinCtl.msgWMLagCtlFlag = false;
        hWinLag = WM_CreateWindowAsChild(0, 360, 720, 650, hWin, WM_CF_SHOW, _cbBkEnLagSet, NULL);
    }
    else if(uiWinCtl.msgWMPmcCtlFlag) {
        uiWinCtl.msgWMPmcCtlFlag = false;
        hWinPmc = WM_CreateWindowAsChild(0, 0, 720, 1200, hWin, WM_CF_SHOW, _cbBkEnPmcSet, NULL);
    }
    /*####################################################################################################*/
    
    break;
  case WM_PAINT:
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
	GUI_SetColor(GUI_WHITE);
	GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetFont(&GUI_Fonthanyi60);
	GUI_DispStringAt("\xe5\xb7\xa5\xe7\xa8\x8b\xe5\xb8\x88\xe8\xae\xbe\xe7\xbd\xae", 30, 15); // ¹¤³ÌÊ¦ÉèÖÃ
	GUI_SetColor(GUI_DARKGRAY);
    GUI_DrawHLine(350, 0, 720);
	GUI_DrawHLine(351, 0, 720);
	GUI_DrawHLine(352, 0, 720);
	GUI_DrawHLine(353, 0, 720);

    if(!initCtrl || uiWinCtl.msgWMEnCreatFlag){
        WM_SendMessageNoPara(hWin, WM_CREATE);
    }
	break;
  default:
    WM_DefaultProc(pMsg);
  }
}

static void App_PMSetCtr(WM_HWIN hWin)
{
	if(hButtonPMSet == NULL) 
		hButtonPMSet = WINDOW_CreateUser(0, 420, 720, 600, hWin, WM_CF_SHOW, 0, 0, _cbBkPMSet, 0); 
	
}

static void App_SysSetCtr(WM_HWIN hWin)
{   
    WM_HWIN hSlider;

    if(hButtonSysSet == NULL){
	    hButtonSysSet = WINDOW_CreateUser(0, 420, 720, 650, hWin, WM_CF_SHOW, 0, 0, _cbBkSysSet, 0);

        SLIDER_SetDefaultSkin(_DrawSkinFlex);
        hSlider = SLIDER_Create(30, 5 + 70 * 6, 505, 70, hButtonSysSet, GUI_ID_SLIDER0, WM_CF_SHOW | WM_CF_MEMDEV, 0);
        SLIDER_SetRange(hSlider, 0, 100);
        SLIDER_SetWidth(hSlider,20);
        SLIDER_SetValue(hSlider,60);
    }
}

static void App_EnSetCtr(WM_HWIN hWin)
{
    if(hButtonEnSet == NULL)
	    hButtonEnSet = WINDOW_CreateUser(0, 0, 720, 1200, hWin, WM_CF_SHOW, 0, 0, _cbBkEnSet, 0);
}


static void WM_Custom_DeleteButtonWindow(void)
{
	WM_DeleteWindow(hButtonPMSet);
	hButtonPMSet = NULL;
	
	WM_DeleteWindow(hButtonSysSet);
	hButtonSysSet = NULL;
	
	WM_DeleteWindow(hButtonEnSet);
	hButtonEnSet = NULL;

	WM_DeleteWindow(hButtonCancel);
	hButtonCancel = NULL;  
}

static void WM_Custom_DeleteEnWindow(void)
{
	WM_DeleteWindow(hWinVer);
	hWinVer = NULL;
	
	WM_DeleteWindow(hWinLag);
	hWinLag = NULL;
	
	WM_DeleteWindow(hWinPmc);
	hWinPmc = NULL;
}


/*********************************************************************
*
*       _cbMenu
*
* Purpose:
*   Callback function of menu window.
*/
static void _cbMenuPageThree(WM_MESSAGE * pMsg) {
  int xSize, i, NCode;
  int Id = 0;
  WM_MESSAGE Message;
  WM_MOTION_INFO * pInfo;
  WM_HWIN hWin, hButton;
  int IsPressed = NULL;

  hWin = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);      // Id of widget
    NCode = pMsg->Data.v;                 // Notification code

	switch (NCode) {
    case WM_NOTIFICATION_CLICKED:
		switch (Id) {
            case GUI_ID_USER_PM:
                WM_Custom_DeleteButtonWindow();
    			App_PMSetCtr(hWin);
                uiWinCtl.msgWMCreatFlag = true;
    		    break;
            case GUI_ID_USER_SYS:
                WM_Custom_DeleteButtonWindow();
                App_SysSetCtr(hWin);
                uiWinCtl.msgWMNetCreatFlag = true;
                break;
            case GUI_ID_USER_SET:
                WM_Custom_DeleteButtonWindow();
                App_EnSetCtr(hWin);
                uiWinCtl.msgWMEnCreatFlag = true;
                break;
			default:
	  			_CreateSelected(Id - GUI_ID_BUTTON0, pMsg->hWin);
		}
      break;
    case WM_NOTIFICATION_RELEASED:
      break;
    }
    break;
  case WM_CREATE:
	    //
	    // Create all buttons
	    //
	    xSize = WM_GetWindowSizeX(hWin);

		switch ((int)BUTTON_STATE) {
			case BUTTON_PAGE_ONE:
				break;
			case BUTTON_PAGE_TWO_L:
				break;
			case BUTTON_PAGE_THREE_L:
				i = 4;
	    		hButton = BUTTON_CreateUser(0, 1100, xSize / 2, 180, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON0 + i, sizeof(i));
	    		BUTTON_SetSkin(hButton, _ButtonSkinPageThreeLeft);
	    		BUTTON_SetUserData(hButton, &i, sizeof(i));

				i = GUI_ID_USER_PM;
				hButton = BUTTON_CreateUser(0, 110, xSize, 90, hWin, WM_CF_SHOW, 0, GUI_ID_USER_PM, sizeof(i));
	    		BUTTON_SetSkin(hButton, _ButtonSkinPageThreeSet);
	    		BUTTON_SetUserData(hButton, &i, sizeof(i));

                uiWinCtl.msgWMCtlFlag = false; // Ç¿ÖÆ´ò¿ª·Û³¾¼ì²âÒ³Ãæ

				i = GUI_ID_USER_SYS;
				hButton = BUTTON_CreateUser(0, 200, xSize, 90, hWin, WM_CF_SHOW, 0, GUI_ID_USER_SYS, sizeof(i));
	    		BUTTON_SetSkin(hButton, _ButtonSkinPageThreeSet);
	    		BUTTON_SetUserData(hButton, &i, sizeof(i));

				i = GUI_ID_USER_SET;
				hButton = BUTTON_CreateUser(0, 290, xSize, 90, hWin, WM_CF_SHOW, 0, GUI_ID_USER_SET, sizeof(i));
	    		BUTTON_SetSkin(hButton, _ButtonSkinPageThreeSet);
	    		BUTTON_SetUserData(hButton, &i, sizeof(i));
				break;
			case BUTTON_PAGE_FOUR_L:
				break;		
			case BUTTON_PAGE_FIVE:
				break;
		}
	    break;
	case WM_PAINT:
		GUI_SetBkColor(GUI_BLACK);
	    GUI_Clear();

		GUI_SetColor(GUI_DARKGRAY);
        GUI_DrawHLine(400, 0, 720);
		GUI_DrawHLine(401, 0, 720);
		GUI_DrawHLine(402, 0, 720);
		GUI_DrawHLine(403, 0, 720);

        GUI_SetColor(GUI_WHITE);
	    GUI_SetTextMode(GUI_TM_TRANS);
		GUI_UC_SetEncodeUTF8();
		GUI_SetFont(&GUI_FontPingFangRegular90);
		GUI_DispStringAt("\xe5\x8f\x82\xe6\x95\xb0\xe8\xae\xbe\xe7\xbd\xae",30,15); 				// ²ÎÊýÉèÖÃ

        if(!uiWinCtl.msgWMCtlFlag) {
            uiWinCtl.msgWMCtlFlag = true;
            
            uiWinCtl.msgWMCreatFlag = true;
            WM_Custom_DeleteButtonWindow();
        	App_PMSetCtr(hWin);
        }   
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

void SmartPhomeMenuMainTask(void) {

  int xSize, ySize;
  static WM_HWIN hWinBase;     // Parent window for heading and viewport
  static WM_HWIN hWinHeading;  // Heading window
  static WM_HWIN hWinViewport; // Viewport window
  static WM_HWIN hWinMenu;     // Menu window moveable within viewport window
  static WM_HWIN hButton_0;
  static WM_HWIN hButton_1;
  static WM_HWIN hButton_2;
  static WM_HWIN hButton_3;

  //WM_SetCreateFlags(WM_CF_MEMDEV);
  GUI_Init();
  WM_MULTIBUF_Enable(1);
  BUTTON_SetReactOnLevel();
  //
  // Get display dimension
  // 
  xSize = LCD_GetXSize();
  ySize = LCD_GetYSize();
  //
  // Limit desktop window to display size
  //
  WM_SetSize(WM_HBKWIN, xSize, ySize);
  // Create windows
  // 720 * 1280
  hWinBase     = WM_CreateWindow(0, 0, xSize, ySize, WM_CF_SHOW, _cbDummy, 0);
  // 720 * 80
  hWinHeading  = WM_CreateWindowAsChild(0, 0, xSize, 80, hWinBase,  WM_CF_SHOW, _cbStatus, 0);
	
  memset(&dev_state, NULL, sizeof(DEV_STATE_CTR_T));  

  hQR = GUI_QR_Create("01:23:ab:cd:01:23:ab:cd",10,GUI_QR_ECLEVEL_H,0);
  /*##################################################################*/	// testing setting
  //BUTTON_STATE = BUTTON_PAGE_ONE; // just for testing
  //BUTTON_STATE = BUTTON_PAGE_TWO_L;
  BUTTON_STATE = BUTTON_PAGE_THREE_L;
  
  memcpy(uiWinCtl.timbuf_1,  "00",strlen("00"));
  memcpy(uiWinCtl.timbuf_1_,  "00",strlen("00"));
  memcpy(uiWinCtl.timbuf_2_0,"10",strlen("10"));
  memcpy(uiWinCtl.timbuf_2_0_,"11",strlen("11"));
  memcpy(uiWinCtl.timbuf_2_1,"12",strlen("12"));
  memcpy(uiWinCtl.timbuf_2_1_,"20",strlen("20"));
  memcpy(uiWinCtl.timbuf_4_0,"14",strlen("14"));
  memcpy(uiWinCtl.timbuf_4_0_,"18",strlen("18"));
  memcpy(uiWinCtl.timbuf_4_1,"20",strlen("20"));
  memcpy(uiWinCtl.timbuf_4_1_,"2",strlen("2"));
  memcpy(uiWinCtl.timbuf_4_2,"21",strlen("21"));
  memcpy(uiWinCtl.timbuf_4_2_,"10",strlen("10"));
  memcpy(uiWinCtl.timbuf_4_3,"22",strlen("22"));
  memcpy(uiWinCtl.timbuf_4_3_,"15",strlen("15"));
  memcpy(uiWinCtl.yearbuf,"2000",strlen("2000"));
  memcpy(uiWinCtl.monthbuf,"2",strlen("2"));
  memcpy(uiWinCtl.daybuf,"10",strlen("10"));
  memcpy(uiWinCtl.hourbuf,"10",strlen("10"));
  memcpy(uiWinCtl.minbuf,"10",strlen("10"));
  
  /*##################################################################*/
  //
  // Keep demo alive
  //
  while (1) {
	switch ((int)BUTTON_STATE) {
		case BUTTON_PAGE_ONE:
			WM_DeleteWindow(hWinMenu); 
			// Menu window moveable within viewport window
			// 720 * 80
			hWinMenu = WM_CreateWindowAsChild(0,  80, xSize, ySize - 80, hWinBase, WM_CF_SHOW, _cbMenuPageOne, 0);

			hButton_0 = BUTTON_CreateUser(540, 213, 162, 162, hWinMenu, WM_CF_SHOW, 0, GUI_ID_BUTTON9, 0);
  			BUTTON_SetSkin(hButton_0, _Button_PM25_Skin);
            
			hButton_1 = BUTTON_CreateUser(510, 373, 200, 80, hWinMenu, WM_CF_SHOW, 0, GUI_ID_USER_BUTTON_0, 0);
  			BUTTON_SetSkin(hButton_1, _ButtonSkinAddr);

			hButton_2 = BUTTON_CreateUser(540, 680, 162, 162, hWinMenu, WM_CF_SHOW, 0, GUI_ID_USER_BUTTON_1, 0);
  			BUTTON_SetSkin(hButton_2, _Button_CO2_Skin);
            
			hButton_3 = BUTTON_CreateUser(510, 840, 200, 80, hWinMenu, WM_CF_SHOW, 0, GUI_ID_USER_BUTTON_2, 0);
  			BUTTON_SetSkin(hButton_3, _ButtonSkinAddr);

			BUTTON_STATE = BUTTON_PAGE_IDLE;
			break;
		case BUTTON_PAGE_TWO_L:
			WM_DeleteWindow(hWinMenu);
            WM_Custom_DeleteButtonWindow();
			
			hWinMenu = WM_CreateWindowAsChild(0,  80, xSize, ySize - 80, hWinBase, WM_CF_SHOW, _cbMenuPageTwo, 0);
			
			hIcon =  _CreateICONVIEW(hWinMenu, 
	                				_aBitmapItem, 
									GUI_COUNTOF(_aBitmapItem), 
									GUI_ID_ICONVIEW0, 
									ICONVIEW_LBorder, 
									ICONVIEW_TBorder, 
									ICONVIEW_Width, 
									ICONVIEW_Height * 6);

			BUTTON_STATE = BUTTON_PAGE_IDLE;
			break;
		case BUTTON_PAGE_THREE_L:
			WM_DeleteWindow(hWinMenu);
            WM_Custom_DeleteButtonWindow();
			hWinMenu = WM_CreateWindowAsChild(0,  80, xSize, ySize - 80, hWinBase, WM_CF_SHOW, _cbMenuPageThree, 0);
			BUTTON_STATE = BUTTON_PAGE_IDLE;
			break;
		case BUTTON_PAGE_FOUR_L:
			break;
		case BUTTON_PAGE_FIVE:
			break;
		case BUTTON_PAGE_IDLE:
			break;
	}

	GUI_Delay(50);
  }
}

/*************************** End of file ****************************/

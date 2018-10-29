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
File        : WIDGET_Edit.c
Purpose     : Example demonstrating the use of a EDIT widget
Requirements: WindowManager - (x)
              MemoryDevices - ( )
              AntiAliasing  - ( )
              VNC-Server    - ( )
              PNG-Library   - ( )
              TrueTypeFonts - ( )
----------------------------------------------------------------------
*/
#include "GUI.h"
#include "EDIT.h"

extern int GUI_Button_mutex_take(void);
extern int GUI_Button_mutex_release(void);

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define WM_APP_SHOW_TEXT (WM_USER + 0)
#define TEXT_MAXLEN      40

#define LEFT      (1 << 0)
#define MIDDLE    (1 << 1)
#define RIGHT     (1 << 2)
#define TOP       (1 << 3)
#define BOTTOM    (1 << 4)
#define LIGHTBLUE 0xEE8822
#define GET_MODE  (WM_USER + 0)
#define TRANS0    0x11000000
#define TRANS1    0xEE000000
#define TRANS2    0xFF000000

#define GUI_ID_USER_EDIT0       (GUI_ID_BUTTON0 + 1012)
#define GUI_ID_USER_EDIT1       (GUI_ID_BUTTON0 + 1013)
#define GUI_ID_USER_EDIT2       (GUI_ID_BUTTON0 + 1014)
#define GUI_ID_USER_EDIT3       (GUI_ID_USER + 0)

//EDIT_Handle hEdit;

//
// Recommended memory to run the sample with adequate performance
//
#define RECOMMENDED_MEMORY (1024L * 5)

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/


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
/*static*/ void _cbButtonAndEdit(WM_MESSAGE * pMsg) {
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

/*********************************************************************
*
*       _cbBk
*/
static void _cbBk(WM_MESSAGE * pMsg) {

  static GUI_MEMDEV_Handle ahMem[2];
  GUI_MEMDEV_Handle hMemPrev;
  //static WM_HWIN hEdit;
  GUI_RECT Rect;
  static U8      ShowText;
  char           aBuffer[TEXT_MAXLEN];
  static int     Mode = 1;
  int            NCode;
  int            Id;

  switch (pMsg->MsgId) {

  case WM_NOTIFY_PARENT:
        Id    = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        
        switch (NCode) {
        case WM_NOTIFICATION_GOT_FOCUS:

            NCode = WM_NOTIFICATION_CLICKED;
            
            break;
        case WM_NOTIFICATION_CLICKED:
            switch (Id) {
              case GUI_ID_EDIT0:
                Mode ^= 1;
                break;
              }
            break;
        case WM_NOTIFICATION_RELEASED:
  
          switch (Id) {
          case GUI_ID_BUTTON0:
            #if 0
            Mode ^= 1;
            if (Mode) {
              BUTTON_SetText(pMsg->hWinSrc, "1");
            } else {
              BUTTON_SetText(pMsg->hWinSrc, "2");
            }
            #endif
            #if 0
            EDIT_GetText(hEdit, aBuffer, TEXT_MAXLEN);
            
			sprintf(aBuffer,"%d",atoi(aBuffer) - 1);
            
            EDIT_SetFont(hEdit, &GUI_Font8x16);
            EDIT_SetTextAlign(hEdit,GUI_TA_HCENTER | GUI_TA_VCENTER);
            EDIT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT0), aBuffer);
            EDIT_SetTextColor(hEdit, 1, GUI_BLACK);
            #endif
            break;
          case GUI_ID_BUTTON1:
            #if 0
            EDIT_GetText(hEdit, aBuffer, TEXT_MAXLEN);
            
			sprintf(aBuffer,"%d",atoi(aBuffer) + 1);
            
            EDIT_SetFont(hEdit, &GUI_Font8x16);
            EDIT_SetTextAlign(hEdit,GUI_TA_HCENTER | GUI_TA_VCENTER);
            EDIT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT0), aBuffer);
            EDIT_SetTextColor(hEdit, 1, GUI_BLACK);
            #endif
            break;
          }
          break;
       }
    break;
  
  case WM_PAINT:
    GUI_SetBkColor(GUI_DARKRED);
    GUI_Clear();

    #if 0
    GUI_EnableAlpha(1);
    GUI_SetColor(GUI_WHITE);
    GUI_DrawRect(59,29,391,391);
    
    GUI_SetFont(&GUI_Font8x16_ASCII);
    GUI_DispStringAt("transparency",100,200); // 透明度测试
    
    GUI_SetColor(0x30000000);       // 0x30000000 颜色带透明度
    GUI_FillRect(60,40,390,390);
    GUI_SetColor(GUI_WHITE);
    GUI_AA_FillCircle(100,100,15);
    GUI_SetColor(GUI_BLACK);
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetFont(&GUI_Font8x16_ASCII);
    GUI_DispStringAt("i",95,95);
    GUI_EnableAlpha(0);

    Rect.x0 = 60;
    Rect.y0 = 20;
    Rect.x1 = 390;
    Rect.y1 = 70;
    GUI_MoveRect(&Rect, 0, 280);
    GUI_DrawGradientV(Rect.x0, Rect.y0, Rect.x1, Rect.y1, GUI_LIGHTGRAY, GUI_WHITE);
    #endif
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _cbEdit
*/
static void _cbEdit(WM_MESSAGE * pMsg) {
  const WM_KEY_INFO * pInfo;
  WM_MESSAGE          Msg;

  switch (pMsg->MsgId) {

  case WM_KEY:
    pInfo = (WM_KEY_INFO *)pMsg->Data.p;
    if (pInfo->Key == GUI_KEY_ENTER) {
      if (pInfo->PressedCnt == 0) {
        Msg.MsgId   = WM_APP_SHOW_TEXT;
        Msg.hWinSrc = pMsg->hWin;
        WM_SendMessage(WM_HBKWIN, &Msg);
        return;
      }
    }
  }
  EDIT_Callback(pMsg);
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
*/
#define XPOS_ 180
#define YPOS_ 180


EDIT_Handle _editAndButtonCreat(WM_HWIN hWin) {

  WM_HWIN       hButton0;
  WM_HWIN       hButton1;
  WM_HWIN       hButton2;
  static EDIT_Handle   hEdit;
  
  #if 0
  //WM_SetCallback(hWin, _cbBk);
  hEdit = EDIT_CreateEx(XPOS_, YPOS_, 81, 50, hWin, WM_CF_SHOW, 1, GUI_ID_EDIT0, TEXT_MAXLEN);
  //WM_SetCallback(hEdit, _cbEdit);
  EDIT_SetFont(hEdit, &GUI_Font8x16);
  EDIT_SetTextAlign(hEdit,GUI_TA_HCENTER | GUI_TA_VCENTER);
  EDIT_SetText(hEdit, "23");
  EDIT_SetTextColor(hEdit, 1, GUI_BLACK);
  //EDIT_EnableBlink(hEdit, 300, 1);
  //WM_SetFocus(hEdit);
  
  //
  // Create buttons
  //
  hButton0 = BUTTON_CreateEx(XPOS_, YPOS_ + 50, 80, 45, hWin, WM_CF_SHOW, 0, GUI_ID_USER_EDIT0);
  BUTTON_SetText(hButton0, "-");
  WM_SetCallback(hButton0, _cbButton);
  WM_DisableMemdev(hButton0);
  WM_SetHasTrans(hButton0);

  hButton1 = BUTTON_CreateEx(XPOS_, YPOS_ - 45, 80, 45, hWin, WM_CF_SHOW, 0, GUI_ID_USER_EDIT1);
  BUTTON_SetText(hButton1, "+");
  WM_SetCallback(hButton1, _cbButton);
  WM_DisableMemdev(hButton1);
  WM_SetHasTrans(hButton1);

  hButton2 = BUTTON_CreateEx(XPOS_ - 20, YPOS_ + 130, 120, 30, hWin, WM_CF_SHOW, 0, GUI_ID_USER_EDIT2);
  BUTTON_SetText(hButton2, "OK");
  WM_SetCallback(hButton2, _cbButton);
  WM_DisableMemdev(hButton2);
  WM_SetHasTrans(hButton2);

  return hEdit;
  #endif
}

/*************************** End of file ****************************/

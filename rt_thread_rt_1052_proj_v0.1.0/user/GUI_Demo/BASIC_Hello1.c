
#include "GUI.h"
#include "DIALOG.h"
#include "WM.h"
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "LISTBOX.h"
#include "MULTIEDIT.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "TEXT.h"
#include "PROGBAR.h"
#include "SCROLLBAR.h"
#include "LISTVIEW.h"
#include "GRAPH.h"
#include "MENU.h"
#include "MULTIPAGE.h"
#include "ICONVIEW.h"
#include "TREEVIEW.h"

#include "ff.h"


/* 变量 */
GUI_XBF_DATA XBF_Data;
GUI_FONT     XBF_Font;
FIL          Fontfile;

FRESULT result;
UINT bw;
static int _cbGetData(U32 Off, U16 NumBytes, void * pVoid, void * pBuffer)
{
	FIL *FontFile;

	/* 文件句柄 */
	FontFile = (FIL *)pVoid;

	/*
	* 指向数据地址
	*/
	result =f_lseek(FontFile, Off);
	if (result != FR_OK) 
	{
		return 1;
	}

	/*
	* 读取数据到buffer
	*/
	result = f_read(FontFile, pBuffer, NumBytes, &bw);
	if (result != FR_OK) 
	{
		return 1; 
	}
	
	return 0;

}

/*
*********************************************************************************************************
*	函 数 名: _ShowXBF
*	功能说明: 使用XBF字体
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void _ShowXBF(void) 
{
	result = f_open(&Fontfile, "1:/Ping40.xbf", FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (result != FR_OK)
	{
		return;

	}

	/* 创建XBF字体 */
	GUI_XBF_CreateFont(&XBF_Font,       
					 &XBF_Data,        
					 GUI_XBF_TYPE_PROP, 
					 _cbGetData,      
					 &Fontfile);       
}

/*
*********************************************************************************************************
*	对话框信息
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { FRAMEWIN_CreateIndirect,  "armfly",            0,                       0,  0,  800,480,FRAMEWIN_CF_MOVEABLE,0},
    { BUTTON_CreateIndirect,    "SFGD",        GUI_ID_BUTTON0,          390,40,360,100,0,0},
    { TEXT_CreateIndirect,      "SFGD",        GUI_ID_TEXT0,            5, 10, 300, 70, 0,0},
    { TEXT_CreateIndirect,      "SFGD",        GUI_ID_TEXT1,            5, 80,300, 70, 0,0},
    { TEXT_CreateIndirect,      "SFGD",        GUI_ID_TEXT2,            5, 160,300, 70, 0,0}
};

/*
*********************************************************************************************************
*	函 数 名: PaintDialog
*	功能说明: 重绘函数
*	形    参：pMsg 消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
void PaintDialog(WM_MESSAGE * pMsg)
{
//    WM_HWIN hWin = pMsg->hWin;
}

/*
*********************************************************************************************************
*	函 数 名: InitDialog
*	功能说明: 对话框初始化 
*	形    参: pMsg 消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    //
    //FRAMEWIN
    //
    FRAMEWIN_SetFont(hWin,&GUI_Font32B_ASCII);
    FRAMEWIN_SetTextAlign(hWin,GUI_TA_VCENTER|GUI_TA_CENTER);
    FRAMEWIN_AddCloseButton(hWin, FRAMEWIN_BUTTON_RIGHT, 0);
    FRAMEWIN_AddMaxButton(hWin, FRAMEWIN_BUTTON_RIGHT, 1);
    FRAMEWIN_AddMinButton(hWin, FRAMEWIN_BUTTON_RIGHT, 2);
    FRAMEWIN_SetTitleHeight(hWin,35);
	
    //
    //GUI_ID_BUTTON0
    //
	_ShowXBF();
	GUI_UC_SetEncodeUTF8();
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON0),&XBF_Font);
	
    //
    //GUI_ID_TEXT0
    //
	TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT0), GUI_RED);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT0),&XBF_Font);
	TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT1), GUI_GREEN);
	TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT1),&XBF_Font);
	TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT2), GUI_BLUE);
	TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT2),&XBF_Font);
	
// 	GUI_UC_SetEncodeNone();
}

/*
*********************************************************************************************************
*	函 数 名: _cbCallback
*	功能说明: 对话框回调函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbCallback(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            PaintDialog(pMsg);
            break;
        case WM_INIT_DIALOG:
            InitDialog(pMsg);
            break;
        case WM_KEY:
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
            {
                case GUI_KEY_ESCAPE:
                    GUI_EndDialog(hWin, 1);
                    break;
                case GUI_KEY_ENTER:
                    GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (Id) 
            {
                case GUI_ID_OK:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
                case GUI_ID_CANCEL:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;

            }
            break;
        default:
            WM_DefaultProc(pMsg);
    }
}


/*
*********************************************************************************************************
*	函 数 名: MainTask
*	功能说明: GUI主函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void MainTask(void) 
{ 
	/* 初始化 */
    GUI_Init();
    WM_SetDesktopColor(GUI_WHITE);      
    WM_SetCreateFlags(WM_CF_MEMDEV);     
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
    GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
	
	while(1)
	{
		GUI_Delay(10);
	}
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/

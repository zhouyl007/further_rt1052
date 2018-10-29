#ifndef _PUBLIC_H_
#define _PUBLIC_H_


#include "WM.h"
#include "bsp_cgk_module.h"


typedef struct _DEV_STATE_CTR{
	unsigned char ButtonCtr;
	unsigned char SensorCtr;
	unsigned char IsPM25Pressed;
	unsigned char PM25PressedCtr;
	unsigned char IsCO2Pressed;
	unsigned char CO2PressedCtr;
	
}DEV_STATE_CTR_T;


extern DEV_STATE_CTR_T dev_state; 

typedef struct _PAGEONE_CONTROL_STU{
	unsigned char ButtonPmisPress;
	unsigned char ButtonCo2isPress;
	unsigned char BUttonElectimeDelay;
	unsigned char ButtonPmtimeDelay;
	unsigned char ButtonCo2timeDelay;
	unsigned char InsertAddrIndex;
	unsigned char Pmaddress[12][4];
	unsigned char Co2address[12][4];
}PAGEONE_CONTROL_STU;

/*
typedef enum {
	BUTTON_PAGE_ONE = 0,
	BUTTON_PAGE_TWO,
	BUTTON_PAGE_THREE,

	BUTTON_PAGE_TWO_UV,
	BUTTON_PAGE_TWO_KNOW,
	BUTTON_PAGE_TWO_BROWSE,

	BUTTON_PAGE_TWO_KONW_DUST,
	BUTTON_PAGE_TWO_KONW_CO2,
	BUTTON_PAGE_TWO_KONW_ELEC,
	BUTTON_PAGE_TWO_KONW_UV,
	BUTTON_PAGE_TWO_KONW_PARA,

	BUTTON_PAGE_INSERADDR,

	BUTTON_PAGE_IDLE,
} BUTTON_STATE_T;
*/

typedef struct {
    uint8_t msgWMCreatFlag;
    uint8_t msgWMVerCtlFlag;
    uint8_t msgWMVerFacCtlFlag;
    uint8_t msgWMLagCtlFlag;
    uint8_t msgWMLagEditCtlFlag;
    uint8_t msgWMPmcCtlFlag;
    uint8_t msgWMPmcCEdittlFlag;
    uint8_t msgWMCtlFlag;
    uint8_t msgButtonFlag;
    uint8_t msgWMEnCreatFlag;
    uint8_t msgWMNetCreatFlag;
    uint8_t msgNetButtonFlag;
    uint8_t msgNet1ButtonFlag;
    uint8_t msgNetTimButtonFlag;
    uint8_t msgNetTimButton1Flag;
    uint8_t msgButton1Flag;
    uint8_t msgButton20Flag;
    uint8_t msgButton21Flag;
    uint8_t msgButton40Flag;
    uint8_t msgButton41Flag;
    uint8_t msgButton42Flag;
    uint8_t msgButton43Flag;
    uint8_t timbuf_1[16];
    uint8_t timbuf_1_[16];
    uint8_t timbuf_2_0[16];
    uint8_t timbuf_2_0_[16];
    uint8_t timbuf_2_1[16];
    uint8_t timbuf_2_1_[16];
    uint8_t timbuf_4_0[16];
    uint8_t timbuf_4_0_[16];
    uint8_t timbuf_4_1[16];
    uint8_t timbuf_4_1_[16];
    uint8_t timbuf_4_2[16];
    uint8_t timbuf_4_2_[16];
    uint8_t timbuf_4_3[16];
    uint8_t timbuf_4_3_[16];
    uint8_t hourbuf[16];
    uint8_t minbuf[16];
    uint8_t yearbuf[16];
    uint8_t monthbuf[16];
    uint8_t daybuf[16];
}UI_CTR_t;

extern UI_CTR_t uiWinCtl;

void _cbMenuPageTwoInsertAddr(WM_MESSAGE * pMsg);

#endif
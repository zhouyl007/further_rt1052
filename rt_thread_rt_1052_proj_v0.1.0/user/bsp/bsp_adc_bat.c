
#include "bsp_adc_bat.h"

adc_config_t adc1_config;
adc_channel_config_t adc1_chan7;    //ADC channel 7

AdcVatData g_AdcVatData;

void bsp_batadc1_init(void)
{  
    ADC_GetDefaultConfig(&adc1_config);             
    adc1_config.referenceVoltageSource=kADC_ReferenceVoltageSourceAlt0; 
    adc1_config.samplePeriodMode=kADC_SamplePeriod2or12Clocks;   
    adc1_config.clockSource=kADC_ClockSourceAD;     
    adc1_config.clockDriver=kADC_ClockDriver1;      
    adc1_config.resolution=kADC_Resolution12Bit;     
    ADC_Init(BAT_USE_ADC,&adc1_config);                    
    ADC_EnableHardwareTrigger(BAT_USE_ADC, false);          
    
    ADC_DoAutoCalibration(BAT_USE_ADC); 
		g_AdcVatData.ucEnable = 1;
}


uint16_t bsp_batadc1_getvolvalue(uint8_t ch)   
{
    uint32_t time_out = 0xFFFFFF;
		uint16_t adc_value;
    
    adc1_chan7.channelNumber=ch;                            
    adc1_chan7.enableInterruptOnConversionCompleted=false;  
    ADC_SetChannelConfig(BAT_USE_ADC,0,&adc1_chan7);
    
    while(ADC_GetChannelStatusFlags(BAT_USE_ADC,0)==0 && (time_out > 0) ){
			time_out-- ;
		};          
		if(time_out == 0) {
			return 0;
		}
    adc_value=ADC_GetChannelConversionValue(BAT_USE_ADC,0);       
//    g_AdcVatData.uiOriginalValue = adc_value;
//		g_AdcVatData.fVoltageValue = (float)adc_value*(3.3/4096);
    return adc_value;
}


// low  power 3.05V
// full power 3.8V
uint8_t bsp_batadc1_getvolvalue_average(uint8_t ch,uint8_t times)
{
	uint32_t temp_val=0;
	uint8_t  t;
	for(t=0;t<times;t++) {
		temp_val+=bsp_batadc1_getvolvalue(ch);
    rt_thread_delay(10);
	}
	g_AdcVatData.uiOriginalValue = temp_val/times;
	g_AdcVatData.fVoltageValue = ((float)g_AdcVatData.uiOriginalValue)*3.3/4096;
	return 0;
} 



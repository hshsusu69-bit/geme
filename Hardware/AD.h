#ifndef __AD_H
#define __AD_H


#include "stm32f10x.h"  
#include "delay.h"  

#define ADC1_GPIO_A      GPIOA
#define ADCR_Pin_X       GPIO_Pin_1
#define ADCR_Pin_Y       GPIO_Pin_2
#define ADCL_Pin_X       GPIO_Pin_3
#define ADCL_Pin_Y       GPIO_Pin_4
#define ADC_Bat          GPIO_Pin_5


extern uint16_t AD_Value[5];
extern uint16_t AV_ADC_Channel0_Sample;	// ADC_CH1平均值
extern uint16_t AV_ADC_Channel1_Sample;	// ADC_CH1平均值

extern uint16_t AV_ADC_Channel2_Sample;	// ADC_CH1平均值
extern uint16_t AV_ADC_Channel3_Sample;	// ADC_CH1平均值
extern uint16_t AV_ADC_Channel4_Sample;	// ADC_CH1平均值


void AD_Init(void);
//void AD_GetValue( void);
void ADC1_Value_average(void);   




                    
#endif

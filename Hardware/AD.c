#include  "AD.h"

uint16_t AD_Value[5];  // 存放ADC转换结果

uint32_t AM_ADC_Channel0_Sample = 0 ;	// ADC通道1采样总计
uint32_t AM_ADC_Channel1_Sample = 0 ;	// ADC通道1采样总计
uint16_t AV_ADC_Channel0_Sample =2125;	// ADC_CH1平均值
uint16_t AV_ADC_Channel1_Sample =2125;	// ADC_CH1平均值

uint32_t AM_ADC_Channel2_Sample = 0 ;	// ADC通道1采样总计
uint32_t AM_ADC_Channel3_Sample = 0 ;	// ADC通道1采样总计
uint16_t AV_ADC_Channel2_Sample =2125;	// ADC_CH1平均值
uint16_t AV_ADC_Channel3_Sample =2125;	// ADC_CH1平均值


uint32_t AM_ADC_Channel4_Sample = 0 ;	// ADC通道1采样总计
uint16_t AV_ADC_Channel4_Sample =2125;	// ADC_CH1平均
void AD_Init(void) {

	GPIO_InitTypeDef GPIO_lnitstructure;
    ADC_InitTypeDef ADC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//开启DMA1时钟
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);//72MHZ/6=12MHz,ADC时钟必须小于14MHz

	GPIO_lnitstructure.GPIO_Pin=ADCL_Pin_X |ADCL_Pin_Y |ADCR_Pin_X |ADCR_Pin_Y |ADC_Bat; //PA1 PA2 PA3 PA4模拟输入
	GPIO_lnitstructure.GPIO_Mode=GPIO_Mode_AIN; //设置为模拟输入
    GPIO_lnitstructure.GPIO_Speed=GPIO_Speed_50MHz; //设置速度为50MHz
	GPIO_Init(ADC1_GPIO_A, &GPIO_lnitstructure);

	
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);		//规则组序列1的位置，配置为通道0
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_55Cycles5);		//规则组序列2的位置，配置为通道0
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_55Cycles5);		//规则组序列1的位置，配置为通道0
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 4, ADC_SampleTime_55Cycles5);		//规则组序列1的位置，配置为通道0
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 5, ADC_SampleTime_55Cycles5);		//规则组序列1的位置，配置为通道0

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;				//ADC工作在独立模式
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;          //右对齐    
    ADC_InitStructure.ADC_ExternalTrigConv  = ADC_ExternalTrigConv_None; //转换由软件触发
    ADC_InitStructure.ADC_NbrOfChannel = 5;                          //规则组转换通道的数量
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;              //循环转换模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;                     //扫描模式
	ADC_Init(ADC1,&ADC_InitStructure);                               //初始化ADC

	
    
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;              //起始地址参数传值   ADC1->DR DMA数据寄存器地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //数据宽度16位半字
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;             //地址不自增，始终转运同一个位置的数据
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;                   //存储器起始地址
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;          //数据宽度半字16
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                      //地址自增
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                           //外设地址作为源，传输方向由外设到存储器
	DMA_InitStructure.DMA_BufferSize = 5;                        //传输次数
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                //循环模式
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                 //硬件触发，触发源为ADC1
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;        //中等优先级
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);                  //DMA初始化

    DMA_Cmd(DMA1_Channel1,ENABLE);              //使能
    ADC_Cmd(ADC1,ENABLE);                       //开启ADC1

	ADC_DMACmd(ADC1,ENABLE);                    //ADC1开启硬件DMA输出信号
    ADC_ResetCalibration(ADC1);                                                 //复位校准
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);                          //等待复位校准完成
	ADC_StartCalibration(ADC1);                                                 //开始校准
	while(ADC_GetCalibrationStatus(ADC1) == SET);                               //等待校准完成
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//软件触发ADC转换开始转换
}
/* 
单次扫描模式+DMA单次传输
*/
/* void AD_GetValue( void)
{
	DMA_Cmd(DMA1_Channel1,DISABLE);//失能
	DMA_SetCurrDataCounter(DMA1_Channel1,4);//给传输计数器写数据
	DMA_Cmd(DMA1_Channel1,ENABLE);//使能
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//软件触发ADC转换开始转换
	
	uint32_t timeout = 0xFFFF;
    while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET)
    {
        timeout--;
        if(timeout == 0)
        {
            AV_ADC_Channel1_Sample = 0; // OLED显示0，标记异常
            break;
        }
    }

    // 正确清除标志位（关键修正）
    DMA_ClearITPendingBit(DMA1_IT_TC1);
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC); // 清除ADC转换完成标志
	
} */


void ADC1_Value_average(void)
{
	uint16_t i,count=0;
	for(i=0;i<10;i++)
	{
		if(count == 0)
		{
		AM_ADC_Channel0_Sample += AD_Value[0];//10次ADC值累加
		AM_ADC_Channel1_Sample += AD_Value[1];//10次ADC值累加
		AM_ADC_Channel2_Sample += AD_Value[2];//10次ADC值累加
		AM_ADC_Channel3_Sample += AD_Value[3];//10次ADC值累加
        AM_ADC_Channel4_Sample += AD_Value[4];//10次ADC值累加
		count = 50;
		}
		else
		{
			count--;
		}
		
	}
	
	
    AV_ADC_Channel0_Sample = (float)AM_ADC_Channel0_Sample/10/2.075;//求平均值
	AV_ADC_Channel1_Sample = (float)AM_ADC_Channel1_Sample/10/2.075;//求平均值
	AV_ADC_Channel2_Sample = (float)AM_ADC_Channel2_Sample/10/2.075;//求平均值
	AV_ADC_Channel3_Sample = (float)AM_ADC_Channel3_Sample/10/2.075;//求平均值
	AV_ADC_Channel4_Sample = (float)AM_ADC_Channel4_Sample/10/2.075;//求平均值
	AM_ADC_Channel0_Sample = 0;
	AM_ADC_Channel1_Sample = 0;
	AM_ADC_Channel2_Sample = 0;
	AM_ADC_Channel3_Sample = 0;
	AM_ADC_Channel4_Sample = 0;
	
}
 

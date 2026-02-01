#include "task.h"

/* typedef struct
{
	uint8_t Priority;					//优先级
	uint8_t Run;               			//运行或停止
	uint16_t Timer;             		//计数值
	uint16_t ItvTime;           		//重载值
	void (*TaskHook)(void);    			//函数句柄
} TASK_COMPONENTS; */
static TASK_COMPONENTS TaskComps[] =///优先级  运行状态  计数值 重载值 任务函数
{
	{ PRIORITY_NOR, 0, 1, 10,	Task0 },
    { PRIORITY_NOR, 0, 2, 100,	Task1 },
	{ PRIORITY_NOR, 0, 3, 50,	Task2 },
	{ PRIORITY_NOR, 0, 5, 250,	Task3 },
	{ PRIORITY_NOR, 0, 7, 1000,	Task4 },
};

/* 
任务：任务0
作用：接受数据。触发adc采样
*/
void Task0(void)
{   
    uint16_t nrf_data[32]={0x00,0x00,0x00,0x00,0x00};
    ADC1_Value_average();

    NRF24L_Receive((uint8_t*)nrf_data);
  
}

/* 
任务：任务一
作用：发送数据
*/
void Task1(void)
{
    uint16_t nrf_data[]={AV_ADC_Channel0_Sample,AV_ADC_Channel1_Sample,
                        AV_ADC_Channel2_Sample,AV_ADC_Channel3_Sample};
    NRF24L_Send((uint8_t*)nrf_data);
   
}
/* 
任务：任务二
作用
*/
void Task2(void)
{
  
}
/* 
任务：任务三
作用：
*/
void Task3(void)
{
   
}
/* 
任务：任务四
作用：
*/
void Task4(void)
{
   
}

/* 
任务：执行任务
作用：执行上述任务
*/
void Task_Process(void)
{
    uint8_t i;

	for(i = 0; i < TASK_MAX - 1; i++)
	{
		if(TaskComps[i].Run && (TaskComps[i].Priority == PRIORITY_NOR))					//低优先级任务	
		{
			TaskComps[i].Run = TASK_STOP;                                     			//任务失能
			TaskComps[i].TaskHook();                                          			//任务执行
		}
	}

}

/* 
任务：计数延时
作用：确保优先级高的任务先执行
*/
void Task_Count(void)
{

	uint8_t i;

	for(i = 0; i < TASK_MAX; i++)
	{
		if(TaskComps[i].Timer) 															//任务计数，低优先级和高优先级
		{
			TaskComps[i].Timer--;
			if(TaskComps[i].Timer <= 0)
			{
				TaskComps[i].Timer = TaskComps[i].ItvTime;   							//重载计数值
				TaskComps[i].Run   = TASK_RUN;               							//任务使能
			}
		}

		if(TaskComps[i].Run && (TaskComps[i].Priority == PRIORITY_HIG))					//高优先级任务		
		{
			TaskComps[i].Run = TASK_STOP;                                     			//任务失能
			TaskComps[i].TaskHook();                                          			//任务执行
		}
	}
}

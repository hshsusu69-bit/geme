#ifndef __TASK_H
#define __TASK_H

#include "stm32f10x.h"  
#include "sys.h"
#include "delay.h"
#include "OLED.H"
#include "KEY.h"
#include "AD.h"	
#include "Menu.h"
#include "NRF24L.h"

#define PRIORITY_NOR	1				//低优先级
#define PRIORITY_HIG	2				//高优先级

#define TASK_RUN		1				//运行
#define TASK_STOP		0				//停止

//任务结构体
typedef struct
{
	uint8_t Priority;					//优先级
	uint8_t Run;               			//运行或停止
	uint16_t Timer;             		//计数值
	uint16_t ItvTime;           		//重载值
	void (*TaskHook)(void);    			//函数句柄
} TASK_COMPONENTS;

//任务列表
typedef enum
{
	TASK0, 
	TASK1,
	TASK2,
	TASK3,
	TASK4,
	TASK_MAX                			//最大任务数
} TASK_LIST;

void Task0(void);
void Task1(void);   
void Task2(void);
void Task3(void);
void Task4(void);
void Task_Process(void);    
void Task_Count(void);  



#endif 

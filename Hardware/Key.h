#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

/************************* 按键引脚定义（PA8~PA10） *************************/
#define KEY1_GPIO_PORT    GPIOA
#define KEY1_GPIO_PIN     GPIO_Pin_8
#define KEY2_GPIO_PORT    GPIOA
#define KEY2_GPIO_PIN     GPIO_Pin_9
#define KEY3_GPIO_PORT    GPIOA
#define KEY3_GPIO_PIN     GPIO_Pin_10
#define KEY_GPIO_CLK      RCC_APB2Periph_GPIOA  // PA口时钟统一使能

/************************* 按键状态宏定义（匹配你主函数的KEY1_PRES） *************************/
#define KEY_UP           0   // 无按键按下
#define KEY1_PRES        1   // KEY1按下（你主函数菜单切换用）
#define KEY2_PRES        2   // KEY2按下（预留：设置界面+调节）
#define KEY3_PRES        3   // KEY3按下（预留：设置界面-调节）

/************************* 函数声明 *************************/
void Key_Init(void);        // 按键初始化（你主函数已调用，直接匹配）
uint8_t KEY_Scan(uint8_t mode); // 按键扫描函数（你主函数已调用，直接匹配）

#endif



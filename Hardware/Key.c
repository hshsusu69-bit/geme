#include "KEY.h"
#include "delay.h"  // 依赖江科大的delay延时函数，你工程已包含，无需额外添加

/**********************************************************
* 函数名：Key_Init
* 功  能：按键引脚初始化（PA8/PA9/PA10 上拉输入）
* 参  数：无
* 返  回：无
* 备  注：你主函数中已调用该函数，直接匹配
**********************************************************/
void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;  // 定义GPIO初始化结构体（STM32标准库）

    RCC_APB2PeriphClockCmd(KEY_GPIO_CLK, ENABLE);  // 使能GPIOA时钟

    // 配置KEY1(PA8)/KEY2(PA9)/KEY3(PA10) 为上拉输入模式
    GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN | KEY2_GPIO_PIN | KEY3_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入：默认高电平，按下低电平
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 速度不影响输入模式，默认配50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);  // 统一初始化GPIOA的3个按键引脚
}

/**********************************************************
* 函数名：KEY_Scan
* 功  能：按键扫描（含软件消抖，支持单次按/长按连按）
* 参  数：mode - 扫描模式
*         0：单次按（默认，你主函数用的模式），松手仅触发一次，防误触
*         1：长按连按，按住按键会持续触发，适合参数调节
* 返  回：KEY_UP(0)/KEY1_PRES(1)/KEY2_PRES(2)/KEY3_PRES(3)
* 备  注：你主函数中 KEY_Scan(0) 直接调用，无需修改
**********************************************************/
uint8_t KEY_Scan(uint8_t mode)
{
    static uint8_t key_up = 1;  // 静态变量：按键松开标志，保存上一次状态（实现单次按）
    uint8_t key_val = KEY_UP;   // 初始化返回值为“无按键”

    // 1. 如果是长按连按模式，重置松开标志（按住一直触发）
    if(mode == 1)
    {
        key_up = 1;
    }

    // 2. 检测是否有按键按下（任意一个按键为低电平）
    if(key_up && (GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == 0 ||
                  GPIO_ReadInputDataBit(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == 0 ||
                  GPIO_ReadInputDataBit(KEY3_GPIO_PORT, KEY3_GPIO_PIN) == 0))
    {
        delay_ms(20);  // 3. 软件消抖：延时20ms，跳过机械抖动阶段
        key_up = 0;    // 4. 标记按键已按下，避免重复触发（单次按模式）

        // 5. 再次检测，确认按键真的被按下（消抖后）
        if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == 0)
        {
            key_val = KEY1_PRES; // KEY1按下，返回1
        }
        else if(GPIO_ReadInputDataBit(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == 0)
        {
            key_val = KEY2_PRES; // KEY2按下，返回2
        }
        else if(GPIO_ReadInputDataBit(KEY3_GPIO_PORT, KEY3_GPIO_PIN) == 0)
        {
            key_val = KEY3_PRES; // KEY3按下，返回3
        }
    }
    // 6. 检测到所有按键都松手（高电平），重置松开标志
    else if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == 1 &&
            GPIO_ReadInputDataBit(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == 1 &&
            GPIO_ReadInputDataBit(KEY3_GPIO_PORT, KEY3_GPIO_PIN) == 1)
    {
        key_up = 1;
    }

    return key_val;  // 返回按键状态
}


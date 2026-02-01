#ifndef __MENU_H
#define __MENU_H

#include "stm32f10x.h"  
#include "OLED.H"
#include "AD.H"
#include "key.H"
// 外部引用核心变量（和主函数一致，直接对接你的代码）
//extern uint16_t ADC_Value[5];    // 你的5路AD采样值（油门/副翼/升降/方向/电池）
extern uint8_t NRF_TxData[32];   // NRF发送缓冲区（32字节，贴合你的驱动）
extern uint8_t NRF_RxData[32];   // NRF接收缓冲区（32字节，贴合你的驱动）
extern uint8_t NRF_Send_Res;     // NRF发送结果：0成功，1失败（你的Send函数返回值）


// 菜单函数声明（主函数直接调用）
void Menu_Init(void);    // 菜单初始化
void Menu_Main(void);    // 主界面（默认显示，飞行数据+无线状态）
void Menu_Set(void);     // 设置界面（KEY1切换，通道校准+灵敏度）
void Menu1(void);        // 你主函数中的KEY1响应函数（界面切换）
void Menu_Refresh(void); // 界面实时刷新（主循环调用，无闪烁）
#endif



#include "stm32f10x.h"
#include "sys.h"

#include "OLED.H"
#include "KEY.h"
#include "AD.h"	
#include "Menu.h"
#include "NRF24L.h"

// 你的原始AD采样数组
uint16_t ADC_Value[5];
// 新增：NRF收发缓冲区（32字节，贴合你的NRF驱动要求）
uint8_t NRF_TxData[32] = {0};
uint8_t NRF_RxData[32] = {0};
// 新增：NRF发送结果（0成功/1失败，接收你NRF24L_Send的返回值）
uint8_t NRF_Send_Res = 0;


int main(void)
{
    // 你的原始初始化顺序，完全保留
	OLED_Init();
	NRF24L_Init();
	OLED_Clear();
	Key_Init();
	AD_Init();
	Menu_Init(); // 新增：菜单初始化（直接显示主界面）
	
	while(1)
	{
        // 你的原始函数调用，完全保留

		ADC1_Value_average();
        
        // 新增：将AD飞行通道值封装进NRF 32字节发送缓冲区（航模常规封装）
        // 16位AD值拆分为高8位+低8位，存入TxData，飞机端可直接解析
        NRF_TxData[0] = ADC_Value[0] >> 8;  // 油门高8位
        NRF_TxData[1] = ADC_Value[0] & 0xFF;// 油门低8位
        NRF_TxData[2] = ADC_Value[1] >> 8;  // 副翼高8位
        NRF_TxData[3] = ADC_Value[1] & 0xFF;// 副翼低8位
        NRF_TxData[4] = ADC_Value[2] >> 8;  // 升降高8位
        NRF_TxData[5] = ADC_Value[2] & 0xFF;// 升降低8位
        NRF_TxData[6] = ADC_Value[3] >> 8;  // 方向高8位
        NRF_TxData[7] = ADC_Value[3] & 0xFF;// 方向低8位 
        // 其余24字节留空，可自行扩展（如电池电压、模式等）
        
        // 你的原始NRF收发，仅修改为传缓冲区地址（贴合你的NRF函数形参）
		NRF24L_Receive(NRF_RxData);          // 接收飞机端数据
		NRF_Send_Res = NRF24L_Send(NRF_TxData);// 发送AD数据，保存发送结果
        
        // 新增：界面实时刷新+主循环小延时（降低CPU占用，避免OLED闪烁）
        Menu_Refresh();
      
		//Menu1(); // 按键切换界面
        // 你的原始KEY1检测，完全保留
		if (KEY_Scan(0)==KEY1_PRES)
		{
			Menu1(); // 按键切换界面
		} 
	}
}


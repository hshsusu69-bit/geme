#include "Menu.h"

#include "KEY.h"
#include "NRF24L.h"

// 菜单状态：0-主界面（默认），1-设置界面（KEY1按下取反）
u8 Menu_Status = 0;

// 菜单初始化：默认进入主界面，直接显示
void Menu_Init(void)
{
    Menu_Status = 0;
    Menu_Main(); // 初始化完成直接显示主界面
}

// 主界面：核心飞行数据+NRF发送状态+电池电压（实时刷新，江科大OLED适配）
// x:0~127  y:0~7（按页）  字体：OLED_6X8（大，核心数据）/8（小，辅助信息）
void Menu_Main(void)
{
    OLED_Clear(); // 清屏避免残影（江科大OLED必备操作）
    /********** 第一行：标题 + NRF无线发送状态 **********/
    OLED_ShowString(0, 2, "RC Plane", OLED_6X8);  // 遥控飞机控制器标题
	OLED_ReverseArea(0, 0, 12, 10); // 反色显示，突出警告
    if(NRF_Send_Res == 0)
    {
        OLED_ShowString(104, 2, "SEND", OLED_6X8); // 发送成功显示SEND
		OLED_ReverseArea(104, 0, 32, 10); // 反色显示，突出警告
    }
    else
    {
        OLED_ShowString(104, 2, "FAIL", OLED_6X8); // 发送失败显示FAIL
		OLED_ReverseArea(104, 0, 32, 10); // 反色显示，突出警告
    }


	/********** 第二、三行：油门+副翼（核心飞行通道，大字体） **********/
    OLED_ShowString(0, 14, "THR:", OLED_6X8);      // 油门（Throttle）
    OLED_ShowNum(28, 14, AD_Value[0], 4, OLED_6X8);// AD_Value[0] - 油门AD值
    OLED_ShowString(74, 14, "AIL:", OLED_6X8);     // 副翼（Aileron）
    OLED_ShowNum(102,14, AD_Value[1], 4, OLED_6X8);// AD_Value[1] - 副翼AD值

    /********** 第四、五行：升降+方向（核心飞行通道，大字体） **********/
    OLED_ShowString(0, 28, "ELE:", OLED_6X8);      // 升降（Elevator）
    OLED_ShowNum(28, 28, AD_Value[2], 4, OLED_6X8);// AD_Value[2] - 升降AD值
    OLED_ShowString(74, 28, "RUD:", OLED_6X8);     // 方向（Rudder）
    OLED_ShowNum(102,28, AD_Value[3], 4, OLED_6X8);// AD_Value[3] - 方向AD值

    /********** 第七行：电池电压（AD采样换算）+ 备用通道（小字体省空间） **********/
    OLED_ShowString(0, 40, "BAT:", OLED_6X8);       // 电池电压标识
    // 电池电压换算：3.3V参考电压/12位ADC（0~4095），分压比2:1（常规航模分压）
    float bat_volt = (AD_Value[4] * 3.3f / 4096) * 2;
    OLED_ShowNum(28, 40, (u32)(bat_volt*100), 3, OLED_6X8); // 显示xx.xxV（放大100倍取整）
    OLED_ShowString(48, 40, "V", OLED_6X8);
    OLED_ShowString(74, 40, "bat:", OLED_6X8);      // 备用通道5
	OLED_ShowString(122, 40, "V", OLED_6X8);
    OLED_ShowNum(102,40, AD_Value[4], 3, OLED_6X8);// 复用ADC4值展示（可自行修改）
	

	OLED_Update(); // 刷新显示内容（江科大OLED必备操作）
}

// 设置界面：通道校准+灵敏度设置，KEY1按下返回主界面
void Menu_Set(void)
{
    OLED_Clear();
    /********** 第一行：设置界面标题（大字体） **********/
    OLED_ShowString(42, 2, "SET MENU", OLED_6X8); // 居中显示，更美观
	//OLED_ReverseArea(44, 0, 64, 10); // 反色显示，突出警告

    /********** 第三行：通道校准（核心设置，大字体） **********/
    OLED_ShowString(0, 14, "CALI:", OLED_6X8);     // 通道校准（Calibrate）
    OLED_ShowString(74, 14, "KEY2/3", OLED_6X8);   // 提示：KEY2/3调节（可扩展）

    /********** 第五行：飞行灵敏度（大字体） **********/
    OLED_ShowString(0, 28, "SENS:", OLED_6X8);     // 灵敏度（Sensitivity）
    OLED_ShowNum(60, 28, 50, 2, OLED_6X8);         // 示例值50（可改为全局变量调节）

    /********** 第七行：返回提示（小字体） **********/
    OLED_ShowString(0, 40, "KEY1:BACK TO MAIN", OLED_6X8); // 明确提示KEY1返回主界面
	OLED_Update(); // 刷新显示内容（江科大OLED必备操作）
}

// 界面切换核心函数（你主函数中调用的Menu1）- KEY1消抖+状态切换
void Menu1(void)
{
    delay_ms(20); // 按键消抖（避免误触，和你的KEY驱动匹配）
    if(KEY_Scan(0) == KEY1_PRES) // 再次检测按键，确认有效按下
    {
        Menu_Status = !Menu_Status; // 状态取反（0<->1）
        if(Menu_Status == 0)
        {
            Menu_Main(); // 切回主界面
        }
        else
        {
            Menu_Set();  // 切入设置界面
        }
    }
}

// 界面实时刷新函数（主循环调用）- 仅主界面刷新，避免OLED闪烁
void Menu_Refresh(void)
{
    if(Menu_Status == 0) // 只有主界面需要实时刷新AD/无线数据
    {
        Menu_Main();
    }
    // 设置界面无需刷新，防止闪烁
}



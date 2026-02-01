#ifndef __NRF24L01P_INS_H
#define __NRF24L01P_INS_H

/**
 * NRF24L01引脚定义
 * UTF-8编码注释
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IRQ_Port 	  GPIOB    // IRQ中断引脚端口
#define CE_Port 	  GPIOB    // CE引脚端口（模式控制）
#define CSN_Port 	  GPIOB    // CSN引脚端口（SPI片选）
#define SCK_Port 	  GPIOB    // SCK引脚端口（SPI时钟）
#define MOSI_Port 	  GPIOA    // MOSI引脚端口（SPI主机输出）
#define MISO_Port 	  GPIOA    // MISO引脚端口（SPI主机输入）

#define IRQ_Pin 	  GPIO_Pin_11  // IRQ中断引脚
#define CE_Pin 	      GPIO_Pin_0  // CE引脚（芯片使能）	
#define CSN_Pin 	  GPIO_Pin_10  // CSN引脚（SPI片选）
#define SCK_Pin 	  GPIO_Pin_1  // SCK引脚（SPI时钟）
#define MOSI_Pin 	  GPIO_Pin_7  // MOSI引脚（SPI主机输出）
#define MISO_Pin 	  GPIO_Pin_6  // MISO引脚（SPI主机输入）

/**
 * NRF24L01寄存器定义
 * UTF-8编码注释
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 寄存器操作指令
#define NRF_READ_REG    0x00    // 读寄存器指令，低5位为寄存器地址
#define NRF_WRITE_REG   0x20    // 写寄存器指令，低5位为寄存器地址
#define RD_RX_PLOAD     0x61    // 读取RX有效载荷（1~32字节）
#define WR_TX_PLOAD     0xA0    // 写入TX有效载荷（1~32字节）
#define FLUSH_TX        0xE1    // 清空TX FIFO寄存器（发射模式下使用）
#define FLUSH_RX        0xE2    // 清空RX FIFO寄存器（接收模式下使用）
#define REUSE_TX_PL     0xE3    // 重复使用上一包数据，CE为高时数据包将被重新发送
#define R_RX_PL_WID     0x60    // 读取接收数据长度
#define NOP             0xFF    // 空操作，可用于读取状态寄存器
#define W_ACK_PLOAD		0xA8    // 写入ACK有效载荷
#define WR_TX_PLOAD_NACK 0xB0   // 写入无ACK的TX有效载荷

// SPI(NRF24L01)寄存器地址
#define CONFIG          0x00    // 配置寄存器
                                // bit0: 1=接收模式, 0=发射模式
                                // bit1: 断电模式选择
                                // bit2: CRC模式（0=1字节CRC, 1=2字节CRC）
                                // bit3: CRC使能（0=禁用, 1=启用）
                                // bit4: MAX_RT（达到最大重发次数中断）使能
                                // bit5: TX_DS（发送完成中断）使能
                                // bit6: RX_DR（接收完成中断）使能	
#define EN_AA           0x01    // 自动应答使能寄存器，bit0~5 对应通道0~5
#define EN_RXADDR       0x02    // 接收地址使能寄存器，bit0~5 对应通道0~5
#define SETUP_AW        0x03    // 地址宽度设置（所有接收通道）
                                // bit0~1: 00=3字节, 01=4字节, 10=5字节
#define SETUP_RETR      0x04    // 自动重发设置
                                // bit0~3: 自动重发次数（0~15）
                                // bit4~7: 自动重发延时 250*x + 86us
#define RF_CH           0x05    // RF通道寄存器，bit0~6 设置通道频率（2.400~2.527GHz）
#define RF_SETUP        0x06    // RF配置寄存器
                                // bit3: 数据速率（0=1Mbps, 1=2Mbps）
                                // bit1~2: 发射功率（00=-18dBm, 01=-12dBm, 10=-6dBm, 11=0dBm）
                                // bit0: 锁相环校准
#define STATUS          0x07    // 状态寄存器
                                // bit0: TX FIFO空标志
                                // bit1~3: 数据接收通道号（0~5，6=未接收）
                                // bit4: 达到最大重发次数标志
                                // bit5: 数据发送完成中断标志
                                // bit6: 数据接收完成中断标志

// 状态寄存器位定义
#define MAX_TX  		0x10    // 达到最大重发次数中断标志
#define TX_OK   		0x20    // TX发送完成中断标志
#define RX_OK   		0x40    // 数据接收完成中断标志
#define TRX_OK   		0x60    // 收发完成中断标志（TX_OK | RX_OK）

#define OBSERVE_TX      0x08    // 发送观察寄存器
                                // bit7~4: 数据包丢失计数器
                                // bit3~0: 重发计数器
#define CD              0x09    // 载波检测寄存器，bit0=载波检测标志
#define RX_ADDR_P0      0x0A    // 接收通道0地址寄存器（最多5字节，低字节在前）
#define RX_ADDR_P1      0x0B    // 接收通道1地址寄存器（最多5字节，低字节在前）
#define RX_ADDR_P2      0x0C    // 接收通道2地址寄存器（仅1字节，高字节同RX_ADDR_P1[39:8]）
#define RX_ADDR_P3      0x0D    // 接收通道3地址寄存器（仅1字节，高字节同RX_ADDR_P1[39:8]）
#define RX_ADDR_P4      0x0E    // 接收通道4地址寄存器（仅1字节，高字节同RX_ADDR_P1[39:8]）
#define RX_ADDR_P5      0x0F    // 接收通道5地址寄存器（仅1字节，高字节同RX_ADDR_P1[39:8]）
#define TX_ADDR         0x10    // 发送地址寄存器（低字节在前），ShockBurst模式下需与RX_ADDR_P0相同
#define RX_PW_P0        0x11    // 接收通道0有效数据长度（1~32字节），设为0则禁用
#define RX_PW_P1        0x12    // 接收通道1有效数据长度（1~32字节），设为0则禁用
#define RX_PW_P2        0x13    // 接收通道2有效数据长度（1~32字节），设为0则禁用
#define RX_PW_P3        0x14    // 接收通道3有效数据长度（1~32字节），设为0则禁用
#define RX_PW_P4        0x15    // 接收通道4有效数据长度（1~32字节），设为0则禁用
#define RX_PW_P5        0x16    // 接收通道5有效数据长度（1~32字节），设为0则禁用
#define NRF_FIFO_STATUS 0x17    // FIFO状态寄存器
                                // bit0: RX FIFO空标志
                                // bit1: RX FIFO满标志
                                // bit2~3: 保留
                                // bit4: TX FIFO空标志
                                // bit5: TX FIFO满标志
                                // bit6: 1=循环使用最后一个数据包，0=不循环								
#define DYNPD			0x1C    // 动态有效载荷长度使能寄存器
#define FEATRUE			0x1D    // 功能寄存器

//////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif



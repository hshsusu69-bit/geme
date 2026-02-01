#include "NRF24L.h"  

uint8_t T_ADDR[5]={0xF0,0xF0,0xF0,0xF0,0xF0};
uint8_t R_ADDR[5]={0xF0,0xF0,0xF0,0xF0,0xF0};
// 控制引脚MOSI
void NRF24L_W_MOSI(uint8_t Value)
{
    GPIO_WriteBit(MOSI_Port, MOSI_Pin, (BitAction)(Value));
}

// SCK，控制SPI时钟引脚
void NRF24L_W_SCK(uint8_t Value)
{
    GPIO_WriteBit(SCK_Port, SCK_Pin, (BitAction)(Value));
}

// 控制引脚CSN
void NRF24L_W_CSN(uint8_t Value)
{
    GPIO_WriteBit(CSN_Port, CSN_Pin, (BitAction)(Value));
}

// 控制引脚CE
void NRF24L_W_CE(uint8_t Value)
{
    GPIO_WriteBit(CE_Port, CE_Pin, (BitAction)(Value));
}

// 读取MISO引脚状态
uint8_t NRF24L_R_MISO(void)
{
    return GPIO_ReadInputDataBit(MISO_Port, MISO_Pin);
}

// 读取IRQ引脚状态
uint8_t NRF24L_R_IRQ(void)
{
    return GPIO_ReadInputDataBit(IRQ_Port, IRQ_Pin);
}

// SPI引脚初始化（修正所有CSK→SCK）
void NRF24L_SPI_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
 	
    // SPI SCK
    GPIO_InitStructure.GPIO_Pin = SCK_Pin; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
 	GPIO_Init(SCK_Port, &GPIO_InitStructure);
    
    // SPI MISO
    GPIO_InitStructure.GPIO_Pin = MISO_Pin; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
 	GPIO_Init(MISO_Port, &GPIO_InitStructure);
    
    // SPI MOSI
    GPIO_InitStructure.GPIO_Pin = MOSI_Pin; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
 	GPIO_Init(MOSI_Port, &GPIO_InitStructure);
    
    // CSN
    GPIO_InitStructure.GPIO_Pin = CSN_Pin; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
 	GPIO_Init(CSN_Port, &GPIO_InitStructure);
    
    // CE
    GPIO_InitStructure.GPIO_Pin = CE_Pin; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
 	GPIO_Init(CE_Port, &GPIO_InitStructure);
    
    // IRQ
    GPIO_InitStructure.GPIO_Pin = IRQ_Pin; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
 	GPIO_Init(IRQ_Port, &GPIO_InitStructure);
    

}

// SPI交换一个字节（修正CSK→SCK）
uint8_t NRF24L_SPI_SwapByte(uint8_t Byte)
{
 	uint8_t i,ByteReceive=0x00;
	for(i=0;i<8;i++)
	{
		NRF24L_W_MOSI(Byte&(0x80>>i));
		NRF24L_W_SCK(1);
		if(NRF24L_R_MISO()==1)
		{
			ByteReceive=ByteReceive|(0x80>>i);
		}
		NRF24L_W_SCK(0);
	}
	return ByteReceive;
}

// 写NRF24L01寄存器
void NRF24L_W_Register(uint8_t Reg, uint8_t Value)
{
    NRF24L_W_CSN(0);
    NRF24L_SPI_SwapByte(Reg);
    NRF24L_SPI_SwapByte(Value);
    NRF24L_W_CSN(1);

}

// 读NRF24L01寄存器
uint8_t NRF24L_R_Register(uint8_t Reg)
{
    uint8_t Value;
    NRF24L_W_CSN(0);
    NRF24L_SPI_SwapByte(Reg);
    Value = NRF24L_SPI_SwapByte(NOP);
    NRF24L_W_CSN(1);
    return Value;
}

// 写NRF24L01缓冲区
void NRF24L_W_Buffer(uint8_t Reg, uint8_t* pBuffer, uint8_t Length)
{
    uint8_t i;
    NRF24L_W_CSN(0);
    NRF24L_SPI_SwapByte(Reg);  // 注：写缓冲区时Reg已包含NRF_WRITE_REG
    for(i=0;i<Length;i++)
    {
        NRF24L_SPI_SwapByte(pBuffer[i]);
    }
    NRF24L_W_CSN(1);
}

// 读NRF24L01缓冲区
void NRF24L_R_Buffer(uint8_t Reg, uint8_t* pBuffer, uint8_t Length)
{
    uint8_t i;
    NRF24L_W_CSN(0);
    NRF24L_SPI_SwapByte(Reg);  // 注：读缓冲区时Reg已包含NRF_READ_REG
    for(i=0;i<Length;i++)
    {
        pBuffer[i] = NRF24L_SPI_SwapByte(NOP);
    }
    NRF24L_W_CSN(1);
}

// NRF24L01初始化
void NRF24L_Init(void)
{
    NRF24L_SPI_Init();
    NRF24L_W_CE(0);
   
    
    // 配置基础参数
    NRF24L_W_Buffer(NRF_WRITE_REG + TX_ADDR, T_ADDR, 5);    // 发送地址
    NRF24L_W_Buffer(NRF_WRITE_REG + RX_ADDR_P0, R_ADDR, 5); // 接收地址0
    NRF24L_W_Register(NRF_WRITE_REG + CONFIG, 0x0F);        // 接收模式、上电、CRC使能
    NRF24L_W_Register(NRF_WRITE_REG + EN_AA, 0x01);         // 使能通道0自动应答
    NRF24L_W_Register(NRF_WRITE_REG + RF_CH, 0x00);         // 射频通道2（避开默认0通道干扰）
    NRF24L_W_Register(NRF_WRITE_REG + RX_PW_P0, 32);        // 通道0数据宽度32字节
    NRF24L_W_Register(NRF_WRITE_REG + EN_RXADDR, 0x01);     // 使能通道0
    NRF24L_W_Register(NRF_WRITE_REG + SETUP_RETR, 0x1A);    // 500us重发间隔，10次重发
    NRF24L_W_Register(FLUSH_RX, NOP);                       // 清空RX FIFO
                           
    NRF24L_W_CE(1);  // 拉高CE，进入接收模式
  
}

// 接收数据（修正拼写Reveive→Receive）
void NRF24L_Receive(uint8_t* pBuffer)
{
    uint8_t Status;
    Status = NRF24L_R_Register(NRF_READ_REG + STATUS);  // 读状态寄存器
    if(Status & RX_OK)                   // 检测到接收完成
    {
         NRF24L_R_Register(R_RX_PL_WID); // 读取接收数据宽度
        NRF24L_R_Buffer(RD_RX_PLOAD, pBuffer, 32);  // 读取接收数据
        NRF24L_W_Register(FLUSH_RX, NOP);  // 清除中断标志
        NRF24L_W_Register(NRF_WRITE_REG + STATUS, Status);                  // 清空RX FIFO
        delay_us(100);
       
    }
   

}

// 发送数据（优化时序+发送后切回接收模式）
uint8_t NRF24L_Send(uint8_t* pBuffer)
{
    uint8_t Status,CONG;
     NRF24L_W_Buffer(WR_TX_PLOAD, pBuffer, 32);  // 写待发送数据
    NRF24L_W_CE(0);  
    NRF24L_W_Register(NRF_WRITE_REG + CONFIG,0x0E);  // 读状态寄存器，清除中断标志
    NRF24L_W_CE(1);  // 拉高CE，启动发送

    while (NRF24L_R_IRQ() == 1);
    Status = NRF24L_R_Register(NRF_READ_REG + STATUS);  // 读状态寄存器
    if (Status & MAX_TX)
    {
        NRF24L_W_Register(FLUSH_TX, NOP);  // 达到最大重发次数，清空TX FIFO
        NRF24L_W_Register(NRF_WRITE_REG + STATUS, Status);  // 清除中断标志
        CONG = 1;  // 发送失败
    }
    else if (Status & TX_OK)
    {
        
        NRF24L_W_Register(NRF_WRITE_REG + STATUS, Status);  // 清除中断标志
        CONG = 0;  // 发送成功
    }
    return CONG;
}




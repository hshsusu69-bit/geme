#ifndef __NRF24L_H
#define __NRF24L_H


#include "stm32f10x.h"  
#include "Delay.h"
#include "NRF24L01P_Ins.h" //指令集所在



void NRF24L_SPI_Init(void);
uint8_t NRF24L_SPI_SwapByte(uint8_t Byte);//SPI交换一个字节
void NRF24L_W_Register(uint8_t Reg, uint8_t Value);//写NRF24L01寄存器
uint8_t NRF24L_R_Register(uint8_t Reg);//读NRF24L01寄存器
void NRF24L_W_Buffer(uint8_t Reg, uint8_t* pBuffer, uint8_t Length);//写NRF24L01缓冲区
void NRF24L_R_Buffer(uint8_t Reg, uint8_t* pBuffer, uint8_t Length);//读NRF24L01缓冲区
void NRF24L_Init(void);
void NRF24L_Receive(uint8_t* pBuffer);//接收数据
uint8_t NRF24L_Send(uint8_t* pBuffer);//发送数据

                
#endif

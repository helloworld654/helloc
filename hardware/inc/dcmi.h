#ifndef _DCMI_H
#define _DCMI_H
#include "bsp.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//DCMI ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/14
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

void My_DCMI_Init(void);
void DCMI_DMA_Init(u32 DMA_Memory0BaseAddr,uint16_t DMA_BufferSize,u32 DMA_MemoryDataSize,u32 DMA_MemoryInc);
void DCMI_Start(void);
void DCMI_Stop(void);

void DCMI_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);
void DCMI_CR_Set(u8 pclk,u8 hsync,u8 vsync);

#endif






















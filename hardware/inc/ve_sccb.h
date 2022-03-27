#ifndef __SCCB_H
#define __SCCB_H
#include "camera.h"
// #if deined(USE_F407VE_BOARD) && USE_F407VE_BOARD
#if USE_F407VE_BOARD == 1
#include "bsp.h"
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 

//OV系列摄像头 SCCB 驱动代码	   
							  
////////////////////////////////////////////////////////////////////////////////// 


//IO方向设置
#define SCCB_SDA_IN()  {GPIOB->MODER&=~(3<<(11*2));GPIOB->MODER|=0<<11*2;}	//PB11 输入
#define SCCB_SDA_OUT() {GPIOB->MODER&=~(3<<(11*2));GPIOB->MODER|=1<<11*2;} 	//PB11 输出


//IO操作函数	 
#define SCCB_SCL    		PBout(10)	 	//SCL
#define SCCB_SDA    		PBout(11) 		//SDA	 

#define SCCB_READ_SDA    	PBin(11)  		//输入SDA    
#define SCCB_ID   			0X60  			//OV2640的ID

///////////////////////////////////////////
void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
uint8_t SCCB_WR_Byte(uint8_t dat);
uint8_t SCCB_RD_Byte(void);
uint8_t SCCB_WR_Reg(uint8_t reg,uint8_t data);
uint8_t SCCB_RD_Reg(uint8_t reg);

#endif

#endif

#ifndef __KEY_H
#define __KEY_H	 

/* 上拉按键:KEY0:PB9  KEY1:PB8
   下拉按键：KEUP:PA0   */

/*下面的方式是通过直接操作库函数方式读取IO*/
#define KEY0 		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)
#define KEY1 		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)
#define WK_UP 	    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0

/*下面方式是通过位带操作方式读取IO*/
/*
#define KEY0 		PEin(4)   	//PE4
#define KEY1 		PEin(3)		//PE3 
#define KEY2 		PEin(2)		//P32
#define WK_UP 	PAin(0)		//PA0
*/

#define KEY0_PRES   1
#define KEY1_PRES	2
#define WKUP_PRES   3

void KEY_Init(void);	//IO初始化
uint8_t KEY_Scan(uint8_t);  		//按键扫描函数	

#endif

#ifndef __KEY_H
#define __KEY_H	 

/* ��������:KEY0:PB9  KEY1:PB8
   ����������KEUP:PA0   */

/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/
#define KEY0 		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)
#define KEY1 		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)
#define WK_UP 	    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0

/*���淽ʽ��ͨ��λ��������ʽ��ȡIO*/
/*
#define KEY0 		PEin(4)   	//PE4
#define KEY1 		PEin(3)		//PE3 
#define KEY2 		PEin(2)		//P32
#define WK_UP 	PAin(0)		//PA0
*/

#define KEY0_PRES   1
#define KEY1_PRES	2
#define WKUP_PRES   3

void KEY_Init(void);	//IO��ʼ��
uint8_t KEY_Scan(uint8_t);  		//����ɨ�躯��	

#endif

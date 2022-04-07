#include "motor.h"
#include "stm32f4xx.h"

void pwm_func_test(void)
{
	bsp_SetTIMOutPWM(GPIOA,GPIO_Pin_3,TIM9,2,100000,4000);
}

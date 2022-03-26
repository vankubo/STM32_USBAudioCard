#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
 
//按键输入驱动代码	   
//STM32F4工程-库函数版本
//淘宝店铺：http://mcudev.taobao.com								  
////////////////////////////////////////////////////////////////////////////////// 	 

/*下面的方式是通过直接操作库函数方式读取IO*/
#define KEY0 		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) //PB0


void KEY_Init(void);	//IO初始化
void KEY_EXTI_Init(void);
#endif

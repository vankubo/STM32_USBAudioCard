#ifndef __OLEDIIC_H
#define __OLEDIIC_H
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
 
//IIC 驱动代码	   
//STM32F4工程-库函数版本
//淘宝店铺：http://mcudev.taobao.com								  
////////////////////////////////////////////////////////////////////////////////// 	
   	   		   
//IO方向设置
#define OLEDSDA_IN()  {GPIOA->MODER&=~(3<<(3*2));GPIOA->MODER|=0<<3*2;}	//PA3输入模式
#define OLEDSDA_OUT() {GPIOA->MODER&=~(3<<(3*2));GPIOA->MODER|=1<<3*2;} //PA3输出模式
//IO操作函数	 
#define OIIC_SCL    PAout(2) //SCL
#define OIIC_SDA    PAout(3) //SDA	 
#define OREAD_SDA   PAin(3)  //输入SDA 

//IIC所有操作函数
void OLEDIIC_Init(void);                //初始化IIC的IO口				 
void OLEDIIC_Start(void);				//发送IIC开始信号
void OLEDIIC_Stop(void);	  			//发送IIC停止信号
void OLEDIIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 OLEDIIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 OLEDIIC_Wait_Ack(void); 				//IIC等待ACK信号
void OLEDIIC_Ack(void);					//IIC发送ACK信号
void OLEDIIC_NAck(void);				//IIC不发送ACK信号

void OLEDIIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 OLEDIIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif

















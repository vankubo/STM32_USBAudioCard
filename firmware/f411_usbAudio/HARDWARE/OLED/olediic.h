#ifndef __OLEDIIC_H
#define __OLEDIIC_H
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
 
//IIC ��������	   
//STM32F4����-�⺯���汾
//�Ա����̣�http://mcudev.taobao.com								  
////////////////////////////////////////////////////////////////////////////////// 	
   	   		   
//IO��������
#define OLEDSDA_IN()  {GPIOA->MODER&=~(3<<(3*2));GPIOA->MODER|=0<<3*2;}	//PA3����ģʽ
#define OLEDSDA_OUT() {GPIOA->MODER&=~(3<<(3*2));GPIOA->MODER|=1<<3*2;} //PA3���ģʽ
//IO��������	 
#define OIIC_SCL    PAout(2) //SCL
#define OIIC_SDA    PAout(3) //SDA	 
#define OREAD_SDA   PAin(3)  //����SDA 

//IIC���в�������
void OLEDIIC_Init(void);                //��ʼ��IIC��IO��				 
void OLEDIIC_Start(void);				//����IIC��ʼ�ź�
void OLEDIIC_Stop(void);	  			//����IICֹͣ�ź�
void OLEDIIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 OLEDIIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 OLEDIIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void OLEDIIC_Ack(void);					//IIC����ACK�ź�
void OLEDIIC_NAck(void);				//IIC������ACK�ź�

void OLEDIIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 OLEDIIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif

















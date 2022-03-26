#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
 
//SPI ��������	   
//STM32F4����-�⺯���汾
//�Ա����̣�http://mcudev.taobao.com										  
////////////////////////////////////////////////////////////////////////////////// 	

/*
SPI1
CLK-PA5
MISO
MOSI-PA7
NSS-PA4
*/
 
						  	    													  
void SPI1_Init(void);			 //��ʼ��SPI1��
void SPI1_SetSpeed(u8 SpeedSet); //����SPI1�ٶ�   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI1���߶�дһ���ֽ�
void SPI1_CS(u8 cs);	
u8 SPI1_sendData(u16 data);
void SPI1_setDataSize(uint16_t SPI_DataSize);
#endif


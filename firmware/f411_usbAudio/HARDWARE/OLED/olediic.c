#include "olediic.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
 
//IIC ��������	   
//STM32F4����-�⺯���汾
//�Ա����̣�http://mcudev.taobao.com								  
////////////////////////////////////////////////////////////////////////////////// 	

//��ʼ��IIC
void OLEDIIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOBʱ��

  //GPIOB8,B9��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	OIIC_SCL=1;
	OIIC_SDA=1;
}
//����IIC��ʼ�ź�
void OLEDIIC_Start(void)
{
	OLEDSDA_OUT();     //sda�����
	OIIC_SDA=1;	  	  
	OIIC_SCL=1;
	delay_us(4);
 	OIIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	OIIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void OLEDIIC_Stop(void)
{
	OLEDSDA_OUT();//sda�����
	OIIC_SCL=0;
	OIIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	OIIC_SCL=1; 
	OIIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 OLEDIIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	OLEDSDA_IN();      //SDA����Ϊ����  
	OIIC_SDA=1;delay_us(1);	   
	OIIC_SCL=1;delay_us(1);	 
	while(OREAD_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			OLEDIIC_Stop();
			return 1;
		}
	}
	OIIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void OLEDIIC_Ack(void)
{
	OIIC_SCL=0;
	OLEDSDA_OUT();
	OIIC_SDA=0;
	delay_us(2);
	OIIC_SCL=1;
	delay_us(2);
	OIIC_SCL=0;
}
//������ACKӦ��		    
void OLEDIIC_NAck(void)
{
	OIIC_SCL=0;
	OLEDSDA_OUT();
	OIIC_SDA=1;
	delay_us(2);
	OIIC_SCL=1;
	delay_us(2);
	OIIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void OLEDIIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	OLEDSDA_OUT(); 	    
    OIIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        OIIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		OIIC_SCL=1;
		delay_us(2); 
		OIIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 OLEDIIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	OLEDSDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        OIIC_SCL=0; 
        delay_us(2);
		OIIC_SCL=1;
        receive<<=1;
        if(OREAD_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        OLEDIIC_NAck();//����nACK
    else
        OLEDIIC_Ack(); //����ACK   
    return receive;
}




























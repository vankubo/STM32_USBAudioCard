#include "oled.h"
#include "olediic.h"
#include "stdio.h"
#include "delay.h"
void IIC_WriteREG(u8 byte)
{
	OLEDIIC_Start();
	OLEDIIC_Send_Byte(0x78); //写数据指令
	OLEDIIC_Send_Byte(0x00);
	OLEDIIC_Send_Byte(byte);
}

void IIC_WriteDAT(u8 byte)
{
	OLEDIIC_Start();
	OLEDIIC_Send_Byte(0x78); //写数据指令
	OLEDIIC_Send_Byte(0x40);
	OLEDIIC_Send_Byte(byte);
}

void LCD_WR_REG(u8 data)
{ 

	IIC_WriteREG(data);
	
}

void LCD_WR_DATA(u8 data)
{
	
	IIC_WriteDAT(data);

}

void oled_test()
{
	while(1)
	{
		OIIC_SCL=1;
		OIIC_SDA=0;
		delay_ms(1000);
		OIIC_SCL=0;
		OIIC_SDA=1;
		delay_ms(1000);
	}
	//OLEDIIC_Send_Byte(0x78);
	//while(OLEDIIC_Wait_Ack()==1)
		//printf("nack\n");
}


void OLED_Init(void)
{

	OLEDIIC_Init();
	

	//************* Start Initial Sequence **********//		
	//开始初始化液晶屏
	
	LCD_WR_REG(0xae);     //display off

   LCD_WR_REG(0x15);    //   set column address
   LCD_WR_REG(0x00);    //  start column   0
   LCD_WR_REG(0x7f);    //  end column   127

   LCD_WR_REG(0x75);    //   set row address
   LCD_WR_REG(0x00);    //  start row   0
   LCD_WR_REG(0x7f);    //  end row   127
   LCD_WR_REG(0x81);  // set contrast control
   LCD_WR_REG(0x80);
   LCD_WR_REG(0xa0);    // gment remap
   LCD_WR_REG(0x51);   //
   LCD_WR_REG(0xa1);  // start line
   LCD_WR_REG(0x00);
   LCD_WR_REG(0xa2);  // display offset
   LCD_WR_REG(0x00);
   LCD_WR_REG(0xa4);    // rmal display
   LCD_WR_REG(0xa8);    // set multiplex ratio
   LCD_WR_REG(0x7f); 
   LCD_WR_REG(0xb1);  // set phase leghth
   LCD_WR_REG(0xf1);
   LCD_WR_REG(0xb3);  // set dclk
   LCD_WR_REG(0x00);  //80Hz:0xc1 90Hz:0xe1   100Hz:0x00   110Hz:0x30 120Hz:0x50   130Hz:0x70     01
   LCD_WR_REG(0xab);  // 
   LCD_WR_REG(0x01);  //
   LCD_WR_REG(0xb6);  // set phase leghth
   LCD_WR_REG(0x0f);
   LCD_WR_REG(0xbe);
   LCD_WR_REG(0x0f);
   LCD_WR_REG(0xbc);
   LCD_WR_REG(0x08);
   LCD_WR_REG(0xd5);
   LCD_WR_REG(0x62);  
   LCD_WR_REG(0xfd);
   LCD_WR_REG(0x12);
   LCD_WR_REG(0xaf); 
	 
	 
	
}



//=================================

//全屏清屏
void OLED_ClearScreen()
{
  int i,j;
  LCD_WR_REG(0x15);
  LCD_WR_REG(0x00);
  LCD_WR_REG(0x3f);

  LCD_WR_REG(0x75);
  LCD_WR_REG(0x00);
  LCD_WR_REG(0x7f);

 for(i=0;i<128;i++) 
  {
    for(j=0;j<64;j++)
     {
      LCD_WR_DATA(0xff);//RAM data write
     }
  }
}
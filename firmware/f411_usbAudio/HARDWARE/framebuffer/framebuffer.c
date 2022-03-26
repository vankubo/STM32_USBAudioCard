#include "framebuffer.h"
#include "usart.h"
#include "malloc.h"
extern u8 dma2_send_finish;//@dma.c

//显示缓冲区
//static u16 framebuffer;//[FRAMEBUFFER_SIZE];



void framebuffer_init(framebuffer_typedef *framebuf,u32 width,u32 height)
{
	//初始化硬件
	Lcd_Init();
	//初始化framebuffer
	//framebuf->buffer=framebuffer;
	framebuf->buffer=(u16*)mymalloc(SRAMIN,FRAMEBUFFER_SIZE*2);			//内存分配(外部调用)
	if(framebuf->buffer==NULL)
		printf("fb malloc fail\n");
	framebuf->buffer_fresh=0;
	framebuf->height=height;
	framebuf->width=width;
	//设置SPI速率
	SPI1_SetSpeed(SPI_BaudRatePrescaler_2);
	//开启SPI dma
	SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx, ENABLE);
	//设置dma中断
}

/*
清空buffer
*/
void framebuffet_clear(framebuffer_typedef *framebuf,u16 color)
{
	u32 i,j;
	u32 w,h;
	w=framebuf->width;
	h=framebuf->height;
	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			framebuf->buffer[i*w+j]=color;
		}
	}
	framebuf->buffer_fresh=1;
}

/*
将显存更新到屏幕
可选择局部更新
*/
void framebuffer_dump(framebuffer_typedef *framebuf,u16 x_start,u16 y_start,u16 x_end,u16 y_end)
{
	u32 i,j;
	u32 w,h,index=0;
	
	//检查缓存
	if(framebuf->buffer_fresh==1)
	{
		while(dma2_send_finish==0){};//等待dma传输完成
		dma2_send_finish=0;	
		SPI1_setDataSize(SPI_DataSize_8b);
		//设置更新区域
		Lcd_SetRegion(x_start,y_start,x_end-1,y_end-1);
		Lcd_WriteIndex(0x2C);
		//设置SPI数据宽度16bit
		SPI1_setDataSize(SPI_DataSize_16b);
		//开启DMA传输
		w=x_end-x_start;
		h=y_end-y_start;
		MYDMA_Config(DMA2_Stream3,DMA_Channel_3,(u32)&SPI1->DR,(u32)framebuf->buffer,w*h);
		DMA2_TransITconfig();
		SPI1_CS(1);
		LCD_DC(1);
		SPI1_CS(0);
		MYDMA_Enable(DMA2_Stream3,w*h);
		framebuf->buffer_fresh=0;
	
	}
	
}
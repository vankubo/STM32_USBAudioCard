#ifndef __FRAMEBUFFER_H
#define __FRAMEBUFFER_H
#include "sys.h"
#include "spi.h"
#include "lcd_Driver.h"
#include "dma.h"

/*
����λ��Ĭ��16bit
*/

//�Դ�������
#define FRAMEBUFFER_SIZE 12800
#define PIXEL_SIZE 2



typedef struct 
{
	u16 *buffer;//�Դ�ָ��
	u32 height;//�߶�
	u32 width;//���
	u8 buffer_fresh;//�Դ���±�־
}framebuffer_typedef;


void framebuffer_init(framebuffer_typedef *framebuf,u32 width,u32 height);
void framebuffer_dump(framebuffer_typedef *framebuf,u16 x_start,u16 y_start,u16 x_end,u16 y_end);
void framebuffet_clear(framebuffer_typedef *framebuf,u16 color);
#endif

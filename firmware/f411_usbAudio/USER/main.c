#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "malloc.h"
#include "wm8978.h"
#include "usbd_audio_core.h"
#include "stm324xg_usb_audio_codec.h"
#include "usbd_usr.h"
#include "usb_conf.h" 	
#include "framebuffer.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include "gui.h"
#include "adc.h"
#include "key.h"

//usb
USB_OTG_CORE_HANDLE USB_OTG_dev;
extern vu8 bDeviceState;		//USB���� ���
extern u8 volume;				//����(��ͨ����������)

//fb
framebuffer_typedef framebuffer;

//������
u16  drawX[160];
u16  drawY[160];
#define PI2 6.28318530717959

//fft
arm_rfft_fast_instance_f32 S;
#define NPT  2048  //1024��FFT
#define Fs 48000  //����Ƶ�� 48000Hz
float32_t  testOutput_f32[NPT];
float32_t  testInput_f32[NPT];
float32_t  testOutput[NPT];
u8 draw[NPT];
float32_t maxValue;
uint32_t testIndex = 0;
extern u8 *i2sbuf;//��Ƶ�����
extern vu8 i2splaybuf;//��ǰ���ڲ��ŵ���Ƶ���	
extern vu8 i2ssavebuf;							//��ǰ���浽����Ƶ������ 
#define MBUFSIZE  

//-----------
extern  uint8_t key;
#define MODE 2
float32_t theta,w,p,q;
int i;
void plot1(void);
void plot2(void);

int main(void)
{        
 	RCC_ClocksTypeDef   RCC_Clocks;
	u8 Divece_STA=0XFF;
	u8 mode=0;
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(96);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED  
	KEY_EXTI_Init();
	
	while(WM8978_Init()!=0)				//��ʼ��WM8978
	{
		printf("wm8978 err\n");
		delay_ms(1000);
	}
	WM8978_ADDA_Cfg(1,0);		//����DAC
	WM8978_Input_Cfg(0,0,0);	//�ر�����ͨ��
	WM8978_Output_Cfg(1,0);		//����DAC��� 
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��   

	//USB
 	USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&AUDIO_cb,&USR_cb);  
	delay_ms(2000);
	
	RCC_GetClocksFreq(&RCC_Clocks); //  ��ȡ  RCC_Clocks  ϵͳʱ��
	printf("--------->\n");
	printf("sys run at:%dMHZ\n",RCC_Clocks.SYSCLK_Frequency/1000000);
	printf("hclk run at:%dMHZ\n",RCC_Clocks.HCLK_Frequency/1000000);
	printf("pll1 run at:%dMHZ\n",RCC_Clocks.PCLK1_Frequency/1000000);
	printf("pll2 run at:%dMHZ\n",RCC_Clocks.PCLK2_Frequency/1000000);
	
//====================
	Adc_Init(); 
	framebuffer_init(&framebuffer,160,80);
	framebuffet_clear(&framebuffer,BLACK);
	framebuffer_dump(&framebuffer,0,0,160,80);

	arm_rfft_fast_init_f32(&S, NPT);
   p=2;
   q=3;
   theta=0;
while(1)
{
		if(key==1)
		{
			key=0;
			mode++;
			mode=mode%MODE;
			//printf("mode:%d\n",mode);
		}
	
		switch(mode)
		{
			case 0:
			{
				plot1();
			}break;
			case 1:
			{
				plot2();
			}break;
		}
		
		
		//dump
		framebuffer_dump(&framebuffer,0,0,160,80);
		delay_ms(10);
		framebuffet_clear(&framebuffer,BLACK);
}

	while(0)
	{ 
		LED0=!LED0;
		if(Divece_STA!=bDeviceState)//״̬�ı���
		{
			if(bDeviceState==1)printf("USB Connected\n");//��ʾUSB�����Ѿ�����
			else printf("USB DisConnected\n");				//��ʾUSB����ʧ��
			Divece_STA=bDeviceState;
		}	
		Lcd_Clear(RED);
		delay_ms(2000);
		Lcd_Clear(BLUE);
		delay_ms(2000);
	} 
}

/*
��ʾ����1
*/
void plot1(void)
{
	
	//������ͼ��
		w=0;
		for(i=0;i<60;i++)
		{
			w+=PI2/60;
			drawX[i]=(u16)70*arm_cos_f32(p*w)+80;;
			drawY[i]=(u16)35*arm_sin_f32(q*w+theta-20)+40;
		}
		theta+=PI2/100;
		if(theta>PI2)
			theta=0;
		//������ͼ�λ�ͼ
		Gui_DrawPoints(drawX,drawY,60,GREEN);
		
		
		//fft 
		//��ȡ����
		//*pcm=i2sbuf[i2splaybuf];
		for(i=0;i<NPT;i++)
		{
			
			//testInput_f32[i]=10*(pcm[i]/(2^16));
			//testInput_f32[i]=pcm[i*2];
			testInput_f32[i]=60*(Get_Adc(ADC_Channel_1)*3.3/4096-1.6);
		}
		//do fft
		/* 1024��ʵ���п���FFT */  
  	arm_rfft_fast_f32(&S, testInput_f32, testOutput_f32, 0); 
		arm_cmplx_mag_f32(testOutput_f32, testOutput, NPT);
		/* Calculates maxValue and returns corresponding BIN value */
 		 arm_max_f32(testOutput, NPT, &maxValue, &testIndex);
		//plot fft
		for(i=0;i<NPT/2;i++)
		{
			draw[i]=(int)(20*80*(testOutput[i])/maxValue)%40;
			draw[i]=draw[i]<2?0:draw[i];
			//xprintf("%d\n",draw[i]);
		}
		for(i=0;i<160;i++)
		{
			Gui_DrawLine(i, 40-draw[(int)i/2],i, 40+draw[(int)i/2],BLUE); 
		}
}

/*
��ʾ����2
*/
void plot2(void)
{
	static float f;
	f=3;
	//Բ
	w=0;
	for(i=0;i<60;i++)
	{
		w+=PI2/60;
		drawX[i]=(u16)35*arm_cos_f32(f*w+theta)+80;;
		drawY[i]=(u16)35*arm_sin_f32(f*w+theta)+40;
	}
	//��ͼ
	Gui_DrawPoints(drawX,drawY,60,GREEN);
	//Բ
	w=0;
	for(i=0;i<60;i++)
	{
		w+=PI2/60;
		drawX[i]=(u16)20*arm_cos_f32(f*w+(PI2-theta))+80;;
		drawY[i]=(u16)20*arm_sin_f32(f*w+(PI2-theta))+40;
	}
	//��ͼ
	Gui_DrawPoints(drawX,drawY,60,RED);
	
	theta+=PI2/500;
	if(theta>PI2)
		theta=0;
}

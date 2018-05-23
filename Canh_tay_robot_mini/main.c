#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_dma.h"              // Keil::Device:StdPeriph Drivers:DMA
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_i2c.h"              // Keil::Device:StdPeriph Drivers:I2C
#include "delay.h"
#include "I2C.h"
#include "LiquidCrystal_I2C.h"
#include "stdio.h"
#include "string.h"	

uint8_t character1[8]={0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t character2[8]={0x18, 0x1C, 0x1E, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F};
uint8_t character3[8]={0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x0F, 0x07, 0x03};
uint8_t character4[8]={0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F};
uint8_t character5[8]={0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1E, 0x1C, 0x18};
uint8_t character6[8]={0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x1F, 0x1F};
uint8_t character7[8]={0x1F, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F};
uint8_t character8[8]={0x03, 0x07, 0x0F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F};
unsigned char MaLCD[]={"0123456789"};
unsigned char buf[13];
char dl1[3],dl2[3],dl3[3],dl4[3];
unsigned short ADCvalue[4];
unsigned short goc1[3], goc2[3], goc3[3],goc4[3];
unsigned char position1;
unsigned char position2;
unsigned char position3;
unsigned char position4;
unsigned char vitri;
unsigned char vt;
unsigned short goc[4];

struct{
	unsigned char mode : 1;
	unsigned char chedo : 1;
}BienSo;

	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
	ADC_InitTypeDef  ADC_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

void intro(void);
int to_digit( unsigned char _c )
{
	return ( _c - 0x30 );
}
void RCC_Enable(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1  | RCC_APB2Periph_GPIOC|
												 RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB|
												 RCC_APB2Periph_AFIO  | RCC_APB2Periph_USART1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
};
void IO_Init_()
{ 
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_1|GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//PA9~TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10~RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void USART1_Init()
{
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_SetPriority(SysTick_IRQn, 0);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void DMA_Init_()
{
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(ADC1 -> DR));
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADCvalue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 4;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1, ENABLE);
}
void ADC_Init_()
{
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv =  ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 4;
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_239Cycles5);//ADC1 Channel 9 ~ PB1
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 2, ADC_SampleTime_239Cycles5);//ADC1 Channel 8 ~ PB0
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 3, ADC_SampleTime_239Cycles5);//ADC1 Channel 7 ~ PA7
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 4, ADC_SampleTime_239Cycles5);//ADC1 Channel 6 ~ PA6
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
void PWM_Init_() 
{
	// Create 50Hz PWM
	// Prescale timer clock from 72MHz to 720kHz by prescaler = 100
	TIM_TimeBaseInitStructure.TIM_Prescaler = 100;
	// TIM_Period = (timer_clock / PWM_frequency) - 1
	// TIM_Period = (720000Hz / 50Hz) - 1 = 14399
	TIM_TimeBaseInitStructure.TIM_Period = 14399;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	TIM_Cmd(TIM2, ENABLE);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);//PWM TIM2 channel 1 ~ PA0
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);//PWM TIM2 channel 2 ~ PA1
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);//PWM TIM2 channel 3 ~ PA2
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);//PWM TIM2 channel 4 ~ PA3
	TIM_ARRPreloadConfig(TIM2, ENABLE);
}
void USART1_PutChar(unsigned char c)
{
	while (!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
	USART_SendData(USART1, c);
}

void USART1_PutString(unsigned char *s)
{
	while (*s)
	{
		USART1_PutChar(*s++);
	}
}

void move_servo(uint8_t servo, uint8_t position){
	uint16_t prescale = 505 + 7.1*position;
  switch (servo){
		case 1: TIM2->CCR1 = prescale;
						break;
		case 2: TIM2->CCR2 = prescale;
						break;
		case 3: TIM2->CCR3 = prescale;
						break;
		case 4: TIM2->CCR4 = prescale;
						break;
		default: break;
	}
}
void _m_servo(uint8_t servo, uint8_t position){
	uint16_t prescale = 505 + 7.1*position;
	uint8_t duty1, duty2, duty3;
  switch (servo){
		case 1: duty1 = TIM2 ->CCR1;
						if(duty1 < prescale)
						{
							for(int x = duty1; x <= prescale; x++)
							{
								TIM2 -> CCR1 = x;
								Delay(2);
							}
						}
						else if(duty1 > prescale)
						{
							for(int x = duty1; x >= prescale; x--)
							{
								TIM2 -> CCR1 = x;
								Delay(2);
							}
						}
						break;
		case 2: duty2 = TIM2 ->CCR2;
						if(duty2 < prescale)
						{
							for(int x = duty2; x <= prescale; x++)
							{
								TIM2 -> CCR2 = x;
								Delay(3);
							}
						}
						else if (duty2 > prescale)
						{
							for(int x = duty2; x >= prescale; x--)
							{
								TIM2 -> CCR2 = x;
								Delay(3);
							}
						}
						break;
		case 3: duty3 = TIM2 ->CCR3;
						if(duty1 < prescale)
						{
							for(int x = duty3; x <= prescale; x++)
							{
								TIM2 -> CCR3 = x;
								Delay(5);
							}
						}
						else if(duty3 > prescale)
						{
							for(int x = duty3; x >= prescale; x--)
							{
								TIM2 -> CCR3 = x;
								Delay(5);
							}
						}
						break;
		case 4: TIM2 -> CCR4 = prescale;
						Delay(500);
						break;
		default: break;
	}
}
void USART1_IRQHandler()
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE))
	{
		// Index for receive buffer
		static uint8_t i = 0;
		// Read received char
		unsigned char c = USART_ReceiveData(USART1);
		// Read chars until newline
		if (c != '\n')
		{
			if (i < 12)
			{
				buf[i] = c;
				i++;
			}
			else
			{
				buf[i] = c;
				i = 0;
			}
		}
		else
		{ //444 333 222 111
			goc[0] = to_digit(buf[0])*100 + to_digit(buf[1])*10 + to_digit(buf[2]);
			goc[1] = to_digit(buf[3])*100 + to_digit(buf[4])*10 + to_digit(buf[5]);
			goc[2] = to_digit(buf[6])*100 + to_digit(buf[7])*10 + to_digit(buf[8]);
			goc[3] = to_digit(buf[9])*100 + to_digit(buf[10])*10 + to_digit(buf[11]);
			if(BienSo.chedo==1)
			{
				move_servo(1, goc[0]);
				move_servo(2, goc[1]);
				move_servo(3, goc[2]);
				move_servo(4, goc[3]);
				
				LCDI2C_setCursor(12,0);
				LCDI2C_write(MaLCD[goc[0]/100]);
				LCDI2C_write(MaLCD[(goc[0]%100)/10]);
				LCDI2C_write(MaLCD[(goc[0]%100)%10]);
				
				LCDI2C_setCursor(1,1);
				LCDI2C_write(MaLCD[goc[1]/100]);
				LCDI2C_write(MaLCD[(goc[1]%100)/10]);
				LCDI2C_write(MaLCD[(goc[1]%100)%10]);
				
				LCDI2C_setCursor(6,1);
				LCDI2C_write(MaLCD[goc[2]/100]);
				LCDI2C_write(MaLCD[(goc[2]%100)/10]);
				LCDI2C_write(MaLCD[(goc[2]%100)%10]);
				
				LCDI2C_setCursor(11,1);
				LCDI2C_write(MaLCD[goc[3]/100]);
				LCDI2C_write(MaLCD[(goc[3]%100)/10]);
				LCDI2C_write(MaLCD[(goc[3]%100)%10]);
			}
			// Clear buffer
			memset(&buf[0], 0, sizeof(buf));
			i = 0;
		}
	}
}
//ADC STM32F103 ~ 12 bit. tu 0 den 4096
int map1(unsigned short value)//map servo duoi
{//Pham vi quay tu 0 den 180 do
	return value/22.75;
}
int map2(unsigned short value)// map servo trai
{//Pham vi quay tu 0 den 110 do
	return (value/37.24);
}
int map3(unsigned short value)// map servo phai
{//Pham vi quay tu 100 den 180 do
	return (value/51.2)+100;
}
int map4(unsigned short value)// map servo tay gap
{//Pham vi quay tu 0 den 15 do
	return (value/273.1);
}

int record_button()
{
		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13))
		{
			Delay(20);
			if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13))
			{
				while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)){};
				return 1;
			}
		}
		return 0;
}
int play_button()
{
		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14))
		{
			Delay(20);
			if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14))
			{
				while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14)){};
				return 1;
			}
		}
		return 0;
}
int mode_button()
{
		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15))
		{
			Delay(20);
			if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15))
			{
				while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15)){};
				return 1;
			}
		}
		return 0;
}
int main(void)
{ 
	BienSo.mode = 0;
	BienSo.chedo = 0;
	vitri = 0;
  vt = 0;
	SystemInit();
  RCC_Enable();
	IO_Init_();
	DMA_Init_();
	ADC_Init_();
	PWM_Init_();
	USART1_Init();
  LCDI2C_init(0x27,16,2);//PB6 ~ I2C1 SCL //PB7 ~ I2C1 SDA											 
  LCDI2C_createChar(0, character1);
	LCDI2C_createChar(1, character2);
	LCDI2C_createChar(2, character3);
	LCDI2C_createChar(3, character4);
	LCDI2C_createChar(4, character5);
	LCDI2C_createChar(5, character6);
	LCDI2C_createChar(6, character7);
	LCDI2C_createChar(7, character8);
  LCDI2C_clear();
  LCDI2C_backlight();
	Delay(100);
	//intro();
  while (1){
		if(play_button()==1){
			BienSo.mode=!BienSo.mode;
		}
		if(mode_button()==1){
			BienSo.chedo=!BienSo.chedo;
		}
		if(BienSo.chedo==0)
		{
			if(BienSo.mode==0){
				position1 = map1(ADCvalue[0]);
				position2 = map2(ADCvalue[1]);
				position3 = map3(ADCvalue[2]);
				position4 = map4(ADCvalue[3]);
				
				LCDI2C_setCursor(12,0);
				LCDI2C_write(MaLCD[position1/100]);
				LCDI2C_write(MaLCD[(position1%100)/10]);
				LCDI2C_write(MaLCD[(position1%100)%10]);
				
				LCDI2C_setCursor(1,1);
				LCDI2C_write(MaLCD[position2/100]);
				LCDI2C_write(MaLCD[(position2%100)/10]);
				LCDI2C_write(MaLCD[(position2%100)%10]);
				
				LCDI2C_setCursor(6,1);
				LCDI2C_write(MaLCD[position3/100]);
				LCDI2C_write(MaLCD[(position3%100)/10]);
				LCDI2C_write(MaLCD[(position3%100)%10]);
				
				LCDI2C_setCursor(11,1);
				LCDI2C_write(MaLCD[position4/100]);
				LCDI2C_write(MaLCD[(position4%100)/10]);
				LCDI2C_write(MaLCD[(position4%100)%10]);
				
				Delay(10);
				move_servo(1, position1);
				move_servo(2, position2);
				move_servo(3, position3);
				move_servo(4, position4);
				
				if(record_button()==1){
					goc1[vt] = position1;
					goc2[vt] = position2;
					goc3[vt] = position3;
					goc4[vt] = position4;
					vt+=1;
					if(vt>=3)	vt=0;
				}
			}
			else{
				
					_m_servo(1, goc1[vitri]);
					_m_servo(2, goc2[vitri]);
					move_servo(3, goc3[vitri]);
					Delay(500);
					_m_servo(4, goc4[vitri]);
					
					LCDI2C_setCursor(12,0);
					LCDI2C_write(MaLCD[goc1[vitri]/100]);
					LCDI2C_write(MaLCD[(goc1[vitri]%100)/10]);
					LCDI2C_write(MaLCD[(goc1[vitri]%100)%10]);
					
					LCDI2C_setCursor(1,1);
					LCDI2C_write(MaLCD[goc2[vitri]/100]);
					LCDI2C_write(MaLCD[(goc2[vitri]%100)/10]);
					LCDI2C_write(MaLCD[(goc2[vitri]%100)%10]);
					
					LCDI2C_setCursor(6,1);
					LCDI2C_write(MaLCD[goc3[vitri]/100]);
					LCDI2C_write(MaLCD[(goc3[vitri]%100)/10]);
					LCDI2C_write(MaLCD[(goc3[vitri]%100)%10]);
					
					LCDI2C_setCursor(11,1);
					LCDI2C_write(MaLCD[goc4[vitri]/100]);
					LCDI2C_write(MaLCD[(goc4[vitri]%100)/10]);
					LCDI2C_write(MaLCD[(goc4[vitri]%100)%10]);
					
					vitri+=1;
					if(vitri>=3) vitri=0;
			}
		}
  }
}
void intro(void){
	LCDI2C_setCursor(0,0);
	LCDI2C_write(0xff);
	LCDI2C_write(0);
	LCDI2C_write(1);
	LCDI2C_setCursor(0,1);
	LCDI2C_write(0xff);
	LCDI2C_write(0);
	LCDI2C_write(1);
	
	LCDI2C_setCursor(3,0);
	LCDI2C_write(7);
	LCDI2C_write(0);
	LCDI2C_write(1);
	LCDI2C_setCursor(3,1);
	LCDI2C_write(2);
	LCDI2C_write(3);
	LCDI2C_write(4);
	
	LCDI2C_setCursor(6,0);
	LCDI2C_write(0xff);
	LCDI2C_write(5);
	LCDI2C_write(4);
	LCDI2C_setCursor(6,1);
	LCDI2C_write(0xff);
	LCDI2C_write(3);
	LCDI2C_write(1);
	
	LCDI2C_setCursor(9,0);
	LCDI2C_write(7);
	LCDI2C_write(0);
	LCDI2C_write(1);
	LCDI2C_setCursor(9,1);
	LCDI2C_write(2);
	LCDI2C_write(3);
	LCDI2C_write(4);
	
	LCDI2C_setCursor(12,0);
	LCDI2C_write(0);
	LCDI2C_write(0xff);
	LCDI2C_write(0);
	LCDI2C_setCursor(12,1);
	LCDI2C_write(0x20);
	LCDI2C_write(0xff);
	LCDI2C_write(0x20);
	
	LCDI2C_setCursor(15,0);
	LCDI2C_write(0xff);
	LCDI2C_setCursor(15,1);
	LCDI2C_write(5);
	
	Delay(2000);
	LCDI2C_clear();
	Delay(5);
	LCDI2C_setCursor(0,1);
	LCDI2C_write(0xA1);
	LCDI2C_write(0xA1);
	LCDI2C_setCursor(14,1);
	LCDI2C_write(0xA1);
	LCDI2C_write(0xA1);
	
	LCDI2C_setCursor(2,0);
	LCDI2C_write(7);
	LCDI2C_write(0);
	LCDI2C_write(1);
	LCDI2C_setCursor(2,1);
	LCDI2C_write(0xff);
	LCDI2C_write(0);
	LCDI2C_write(0xff);
	
	LCDI2C_setCursor(6,0);
	LCDI2C_write(0xff);
	LCDI2C_write(0);
	LCDI2C_write(1);
	LCDI2C_setCursor(6,1);
	LCDI2C_write(0xff);
	LCDI2C_write(0);
	LCDI2C_write(1);
	
	LCDI2C_setCursor(10,0);
	LCDI2C_write(0xff);
	LCDI2C_write(1);
	LCDI2C_write(7);
	LCDI2C_write(0xff);
	LCDI2C_setCursor(10,1);
	LCDI2C_write(0xff);
	LCDI2C_write(0x20);
	LCDI2C_write(0x20);
	LCDI2C_write(0xff);
	
	Delay(2000);
	LCDI2C_clear();
	Delay(5);
	
	LCDI2C_setCursor(0,0);
  LCDI2C_write_String("Loading");
	Delay(1000);
	LCDI2C_write_String("..");
	
	LCDI2C_setCursor(9,0);
	LCDI2C_write(5);
	LCDI2C_write(5);
	LCDI2C_write(1);
	LCDI2C_setCursor(9,1);
	LCDI2C_write(0xff);
	LCDI2C_write(6);
	LCDI2C_write(6);
	LCDI2C_setCursor(15,0);
	
	LCDI2C_setCursor(12,0);
	LCDI2C_write(0xff);
	LCDI2C_write(5);
	LCDI2C_write(5);
	LCDI2C_setCursor(12,1);
	LCDI2C_write(6);
	LCDI2C_write(6);
	LCDI2C_write(4);
	
	LCDI2C_setCursor(15,0);
	LCDI2C_write_String("%");
	
	Delay(800);
	LCDI2C_setCursor(0,1);
	LCDI2C_write_String("..");
	
	LCDI2C_setCursor(9,0);
	LCDI2C_write(0xff);
	LCDI2C_write(5);
	LCDI2C_write(5);
	LCDI2C_setCursor(9,1);
	LCDI2C_write(6);
	LCDI2C_write(6);
	LCDI2C_write(4);
	
	LCDI2C_setCursor(12,0);
	LCDI2C_write(7);
	LCDI2C_write(0);
	LCDI2C_write(1);
	LCDI2C_setCursor(12,1);
	LCDI2C_write(2);
	LCDI2C_write(3);
	LCDI2C_write(4);
	
	Delay(800);
	LCDI2C_setCursor(2,1);
	LCDI2C_write_String("..");
	
	LCDI2C_setCursor(9,0);
	LCDI2C_write(0);
	LCDI2C_write(0);
	LCDI2C_write(0xff);
	LCDI2C_setCursor(9,1);
	LCDI2C_write(0x20);
	LCDI2C_write(0xff);
	LCDI2C_write(0x20);
	
	LCDI2C_setCursor(12,0);
	LCDI2C_write(0xff);
	LCDI2C_write(5);
	LCDI2C_write(5);
	LCDI2C_setCursor(12,1);
	LCDI2C_write(6);
	LCDI2C_write(6);
	LCDI2C_write(4);
	
	Delay(800);
	LCDI2C_setCursor(4,1);
	LCDI2C_write_String("....");
	
	LCDI2C_setCursor(9,0);
	LCDI2C_write(7);
	LCDI2C_write(5);
	LCDI2C_write(0xff);
	LCDI2C_setCursor(9,1);
	LCDI2C_write(3);
	LCDI2C_write(3);
	LCDI2C_write(4);
	
	LCDI2C_setCursor(12,0);
	LCDI2C_write(7);
	LCDI2C_write(5);
	LCDI2C_write(0xff);
	LCDI2C_setCursor(12,1);
	LCDI2C_write(3);
	LCDI2C_write(3);
	LCDI2C_write(4);
	
	Delay(1000);
	LCDI2C_clear();
	Delay(5);
	LCDI2C_setCursor(0,1);
	LCDI2C_write_String("Ready....");
	LCDI2C_setCursor(9,0);
	LCDI2C_write(7);
	LCDI2C_write(0);
	LCDI2C_write(0);
	LCDI2C_setCursor(9,1);
	LCDI2C_write(2);
	LCDI2C_write(3);
	LCDI2C_write(1);
	
	LCDI2C_setCursor(12,0);
	LCDI2C_write(7);
	LCDI2C_write(0);
	LCDI2C_write(1);
	LCDI2C_setCursor(12,1);
	LCDI2C_write(2);
	LCDI2C_write(3);
	LCDI2C_write(4);
	
	LCDI2C_setCursor(15,0);
	LCDI2C_write(0xff);
	LCDI2C_setCursor(15,1);
	LCDI2C_write(5);
	
	Delay(800);
	LCDI2C_clear();
	Delay(5);
	LCDI2C_setCursor(0,0);
	LCDI2C_write_String("4axis value: ");
	
	LCDI2C_setCursor(15,0);
	LCDI2C_write(0xDF);
	
	LCDI2C_setCursor(0,1);
	LCDI2C_write_String("~");
	
	LCDI2C_setCursor(4,1);
	LCDI2C_write(0xDF);
	LCDI2C_write_String("~");
	LCDI2C_setCursor(9,1);
	LCDI2C_write(0xDF);
	LCDI2C_write_String("~");
	LCDI2C_setCursor(14,1);
	LCDI2C_write(0xDF);
}
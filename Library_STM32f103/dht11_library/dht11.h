#ifndef __DHT11_1_H
#define __DHT11_1_H
#include "stm32f10x.h"  
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "delay.h"
//#define         DHT_DATA_IN    GPIOB
//#define         DHT_DATA_OUT   GPIOB   
//#define         DHT_DDR_DATA   GPIOB  
#define         DHT_ER         0  
#define         DHT_OK         1    
void GPIO_OUT_DHT11(void);
void GPIO_IN_DHT11(void);
unsigned char DHT_GetTemHumi (unsigned char *tem,unsigned char *humi);
#endif

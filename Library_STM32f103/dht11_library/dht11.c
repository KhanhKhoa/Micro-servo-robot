#include  "dht11.h"

void GPIO_OUT_DHT11()
{
	GPIO_InitTypeDef A;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	A.GPIO_Pin=GPIO_Pin_12;
	A.GPIO_Mode = GPIO_Mode_Out_PP;
  A.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &A);	
}
void GPIO_IN_DHT11()
{
	GPIO_InitTypeDef A;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	A.GPIO_Pin=GPIO_Pin_12;
	A.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &A);	
}



unsigned char DHT_GetTemHumi (unsigned char *tem,unsigned char *humi)
//float DHT_GetTemHumi (float *tem,float *humi)
{
    unsigned char buffer[5]={0x00,0x00,0x00,0x00,0x00};    // mang de doc du lieu tren chan DATA 
    unsigned char ii,i,checksum;
    
		GPIO_OUT_DHT11();
		DelayUs(60);
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
    DelayMs(25); // it nhat 18ms
    GPIO_SetBits(GPIOB, GPIO_Pin_12);
		
		
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
    DelayUs(60);      // Cho 60us kiem tra muc tren chan DATA
		
		GPIO_IN_DHT11();
		DelayUs(50);
    if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12))      // Neu chan DATA ko duoc keo xuong muc 0    
    return DHT_ER ;      // Tra ve gia tri 0    ==> ko giao tiep duoc voi DHT11
    else              // Chan DATA o muc thap 
			
		
    while(!(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)));    //Doi chan DaTa len 1
    DelayUs(80);        // Chan Data o muc cao 80us
		
    if(!(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)))        // Kiem tra muc tren chan DATA
    return DHT_ER;
    else 
    while((GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)));     //Doi chan Data ve 0
    //Bat dau doc du lieu
    for(i=0;i<5;i++)  // nhan 5byte du lieu
    {
        for(ii=0;ii<8;ii++)     // nhan tung bit
        {    
        while((!(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)))); //Doi Data len 1
        DelayUs(50);
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12))
            {
            buffer[i]|=(0x01<<(7-ii));
            while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)) ;   //Doi Data xuong 0
            }
        }
    }
    //Tinh toan check sum
    checksum=buffer[0]+buffer[1]+buffer[2]+buffer[3];
    //Kiem tra check sum
    if((checksum)!=buffer[4])return DHT_ER;
    //Lay du lieu
    *tem  =   buffer[2];
    *humi =   buffer[0];
    return DHT_OK;
}
/*while(1)
	{
		ok=DHT_GetTemHumi (&temp,&hum) ;
       if(ok==1)
       {
        temp_last=temp;
        hum_last=hum;  
        num1=(char)(temp_last/10);
        num2=temp_last-num1*10;  
        num5=(char)(hum_last/10);
        num6=hum_last-num5*10; 
        } 
        DelayMs(200);
	}
*/

/******************************************/

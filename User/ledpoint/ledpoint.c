#include "ledpoint.h"

void Write_Max72191_byte(uint8_t DATA)
{
	uint8_t i;
	max72191_cs_L;
	for(i=8;i>=1;i--)
	{
		max72191_clk_L;
		if((DATA&0X80)==0)
			max72191_din_L;
		else
			max72191_din_H;
		DATA=DATA<<1;
		max72191_clk_H;
	}
}
void Write_max72191(uint8_t address,uint8_t dat)
{
	max72191_cs_L;
	Write_Max72191_byte(address);
	Write_Max72191_byte(dat);
	max72191_cs_H;
}
void max72191_INIT(void)
{
	Write_max72191(0x09,0x00);
	Write_max72191(0x0a,0x03);
	Write_max72191(0x0b,0x07);
	Write_max72191(0x0c,0x01);
	Write_max72191(0x0f,0x00);
}

void LEDPOINT_GPIO_Config(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启GPIOB和GPIOF的外设时钟*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 

		/*选择要控制的GPIOB引脚*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIOC*/
		GPIO_Init(GPIOA, &GPIO_InitStructure);	
}


void Write_Max72192_byte(uint8_t DATA)
{
	uint8_t i;
	max72192_cs_L;
	for(i=8;i>=1;i--)
	{
		max72192_clk_L;
		if((DATA&0X80)==0)
			max72192_din_L;
		else
			max72192_din_H;
		DATA=DATA<<1;
		max72192_clk_H;
	}
}
void Write_max72192(uint8_t address,uint8_t dat)
{
	max72192_cs_L;
	Write_Max72192_byte(address);
	Write_Max72192_byte(dat);
	max72192_cs_H;
}
void max72192_INIT(void)
{
	Write_max72192(0x09,0x00);
	Write_max72192(0x0a,0x03);
	Write_max72192(0x0b,0x07);
	Write_max72192(0x0c,0x01);
	Write_max72192(0x0f,0x00);
}


/*********************************************END OF FILE**********************/

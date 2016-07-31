#ifndef __LEDPOINT_H
#define	__LEDPOINT_H

#include "stm32f10x.h"

/* ����max7219���ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����Ĵ��뼴�ɸı���Ƶ�LED���� */
#define max72191_clk_H  	GPIO_SetBits(GPIOA,GPIO_Pin_3)		             
#define max72191_clk_L  	GPIO_ResetBits(GPIOA,GPIO_Pin_3)	

#define max72191_cs_H  	GPIO_SetBits(GPIOA,GPIO_Pin_1)		             
#define max72191_cs_L  	GPIO_ResetBits(GPIOA,GPIO_Pin_1)

#define max72191_din_H  	GPIO_SetBits(GPIOA,GPIO_Pin_0)		             
#define max72191_din_L  	GPIO_ResetBits(GPIOA,GPIO_Pin_0)

#define max72192_clk_H  	GPIO_SetBits(GPIOA,GPIO_Pin_6)		             
#define max72192_clk_L  	GPIO_ResetBits(GPIOA,GPIO_Pin_6)	

#define max72192_cs_H  	GPIO_SetBits(GPIOA,GPIO_Pin_5)		             
#define max72192_cs_L  	GPIO_ResetBits(GPIOA,GPIO_Pin_5)

#define max72192_din_H  	GPIO_SetBits(GPIOA,GPIO_Pin_4)		             
#define max72192_din_L  	GPIO_ResetBits(GPIOA,GPIO_Pin_4)


extern void LEDPOINT_GPIO_Config(void);
extern void max72191_INIT(void);
extern void max72192_INIT(void);
extern void Write_max72192(uint8_t address,uint8_t dat);
extern void Write_max72191(uint8_t address,uint8_t dat);
void turn_left_led();
void turn_right_led();
void turn_leftback_led();
void turn_leftfront_led();
void turn_rightback_led();
void turn_rightfront_led();
void turn_left_clear_led();
void turn_right_clear_led();
void clear_led();
void turn_goahead_right_led();
void turn_goahead_left_led();

#endif /* __LED_H */

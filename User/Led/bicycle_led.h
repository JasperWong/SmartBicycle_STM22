#ifndef __LED_H
#define	__LED_H


#include "stm32f10x.h"

/* ����LED���ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����Ĵ��뼴�ɸı���Ƶ�LED���� */
#define led_freq_r_on    	GPIO_SetBits(GPIOC,GPIO_Pin_0)			              /* GPIO�˿� */
#define led_freq_r_off    	GPIO_ResetBits(GPIOC,GPIO_Pin_0)

#define led_tip_r_green_on 	   GPIO_SetBits(GPIOC,GPIO_Pin_1)		/* GPIO�˿�ʱ�� */
#define led_tip_r_green_off 	   GPIO_ResetBits(GPIOC,GPIO_Pin_1)

#define led_tip_l_red_on 	   GPIOC->BSRR = GPIO_Pin_2		/* GPIO�˿�ʱ�� */
#define led_tip_l_red_off 	   GPIOC->BRR = GPIO_Pin_2

#define led_tip_l_green_on 	   GPIO_SetBits(GPIOC,GPIO_Pin_3)		/* GPIO�˿�ʱ�� */
#define led_tip_l_green_off 	   GPIO_ResetBits(GPIOC,GPIO_Pin_3)

#define led_tip_r_red_on 	   GPIO_SetBits(GPIOC,GPIO_Pin_4)		/* GPIO�˿�ʱ�� */
#define led_tip_r_red_off 	   GPIO_ResetBits(GPIOC,GPIO_Pin_4)

#define led_myfreq_r_on    	GPIO_SetBits(GPIOC,GPIO_Pin_5)			              /* GPIO�˿� */
#define led_myfreq_r_off    	GPIO_ResetBits(GPIOC,GPIO_Pin_5)

#define led_myfreq_l_on    	GPIO_SetBits(GPIOC,GPIO_Pin_6)			              /* GPIO�˿� */
#define led_myfreq_l_off    	GPIO_ResetBits(GPIOC,GPIO_Pin_6)

#define led_freq_l_on    			GPIOC->BSRR = GPIO_Pin_7	              /* GPIO�˿� */
#define led_freq_l_off    	GPIOC->BRR = GPIO_Pin_7


extern void LED_GPIO_Config(void);
#endif /* __LED_H */

#ifndef __LED_H
#define	__LED_H


#include "stm32f10x.h"

/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
#define led_freq_r_on    	GPIO_SetBits(GPIOC,GPIO_Pin_0)			              /* GPIO端口 */
#define led_freq_r_off    	GPIO_ResetBits(GPIOC,GPIO_Pin_0)

#define led_tip_r_green_on 	   GPIO_SetBits(GPIOC,GPIO_Pin_1)		/* GPIO端口时钟 */
#define led_tip_r_green_off 	   GPIO_ResetBits(GPIOC,GPIO_Pin_1)

#define led_tip_l_red_on 	   GPIO_SetBits(GPIOC,GPIO_Pin_2)		/* GPIO端口时钟 */
#define led_tip_l_red_off 	   GPIO_ResetBits(GPIOC,GPIO_Pin_2)

#define led_tip_l_green_on 	   GPIO_SetBits(GPIOC,GPIO_Pin_3)		/* GPIO端口时钟 */
#define led_tip_l_green_off 	   GPIO_ResetBits(GPIOC,GPIO_Pin_3)

#define led_tip_r_red_on 	   GPIO_SetBits(GPIOC,GPIO_Pin_4)		/* GPIO端口时钟 */
#define led_tip_r_red_off 	   GPIO_ResetBits(GPIOC,GPIO_Pin_4)

#define led_myfreq_r_on    	GPIO_SetBits(GPIOC,GPIO_Pin_5)			              /* GPIO端口 */
#define led_myfreq_r_off    	GPIO_ResetBits(GPIOC,GPIO_Pin_5)

#define led_myfreq_l_on    	GPIO_SetBits(GPIOC,GPIO_Pin_6)			              /* GPIO端口 */
#define led_myfreq_l_off    	GPIO_ResetBits(GPIOC,GPIO_Pin_6)

#define led_freq_l_on    	GPIO_SetBits(GPIOC,GPIO_Pin_0)			              /* GPIO端口 */
#define led_freq_l_off    	GPIO_ResetBits(GPIOC,GPIO_Pin_0)


extern void LED_GPIO_Config(void);
#endif /* __LED_H */

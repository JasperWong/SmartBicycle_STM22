#ifndef __bicycle_mlx9016_H
#define	__bicycle_mlx9016_H
#include "stm32f10x.h"
#define SDA_IN  GPIO_InitTypeDef GPIO_InitStructure;\
                GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  \
								GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;\
								GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; \
								GPIO_Init(GPIOF, &GPIO_InitStructure)
#define SDA_OUT GPIO_InitTypeDef GPIO_InitStructure;\
                GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  \
                GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;\
								GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; \
								GPIO_Init(GPIOF, &GPIO_InitStructure)

#define SDA_H GPIO_SetBits(GPIOF,GPIO_Pin_9)
#define SDA_L GPIO_ResetBits(GPIOF,GPIO_Pin_9)
#define SDA_IN_H  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_9)==1
#define SCL_H GPIO_SetBits(GPIOF,GPIO_Pin_10)
#define SCL_L GPIO_ResetBits(GPIOF,GPIO_Pin_10)
extern void mlx_GPIO_Config(void);
extern unsigned int memread(void);
#endif

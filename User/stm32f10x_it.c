/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bicycle.h"
#include "bicycle_mlx9016.h"
#include "bsp_usart1.h"
#include "string.h"
#include "timer.h"

unsigned int Task_Delay[NumOfTask]={0};
extern void TimingDelay_Decrement(void);
extern void TimeStamp_Increment(void);
extern void gyro_data_ready_cb(void);
extern BICYCLE mBicycle;
extern uint16_t freq_count;
extern 	uint8_t USART_RX_4[9];
extern 	uint8_t USART_RX_5[4];
extern uint8_t freq;
extern uint8_t new_speed_flag;
extern uint8_t  package_clear[3];
extern uint8_t swing_count;
extern uint8_t warm_1;


uint16_t count = 0;

//vino
extern 	char USART_RX_1[16];
extern uint8_t uasrt1_buff[11];
extern uint8_t USART_RX_3[11];
extern uint8_t Mpu_sign;
uint8_t usart3_counter;
extern uint8_t TIM3_count;
extern uint8_t warm_count;
//

//JJ
extern uint8_t myfreq_left;
extern uint8_t myfreq_right;
uint8_t speed_count=0;
//

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{

  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	unsigned char i;

	TimingDelay_Decrement();
	TimeStamp_Increment();
	
	for(i=0;i<NumOfTask;i++)
	{
		if(Task_Delay[i])
		{
			Task_Delay[i]--;
		}
	}
	count ++;
}



/// IO 线中断
void macEXTI_INT_FUNCTION (void)
{
//	MPU_DEBUG("intterrupt");

}

//void USART1_IRQHandler(void)
//{
//	static uint8_t USART1_RECEIVE = 0;
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//	{ 	
//		USART_RX_1[USART1_RECEIVE]	= USART_ReceiveData(USART1);

//		if(USART_RX_1[0] == 'm')
//		{
//			switch(USART_RX_1[1])
//			{
//				case '0':
//					//返回主界面 不做任何动作
//				break;
//				case '1':
//					mBicycle.outer_status = STATE_FREEDOM_MODE;
//				break;
//				case '2':
//					mBicycle.outer_status = STATE_SPORT_MODE_WARM1;
//				break;
//				case '3':
//					mBicycle.outer_status = STATE_SPORT_MODE_WARM2;
//				break;
//				case '4':
//					mBicycle.outer_status = STATE_SPORT_MODE_WARM3;
//				break;
//				case '5':
//					mBicycle.outer_status = STATE_SPORT_MODE_LEVEL;
//				break;
//				case '6':
//					mBicycle.outer_status = STATE_SPORT_MODE_CLIMB;
//				break;
//				case '7':
//					
//				break;

//				default :
//					USART1_RECEIVE = 0;
//				break;
//			}
//			USART1_RECEIVE++;
//		}
//		else
//		{
//			USART1_RECEIVE = 0;
//		}
//		
//		
//		
//	} 
//	 
//}

void USART1_IRQHandler(void)
{
	static uint8_t model_flag=0,freq_flag=0; 
	uint8_t temp;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		if(USART_ReceiveData(USART1) == 'c')
		{
			Usart_SendStr(USART1,package_clear);
		}
        else if(USART_ReceiveData(USART1) == 'r')
        {    
            printf("ring");
            Usart_SendStr(UART5,"ring");
        }
		else
		{
			temp=USART_ReceiveData(USART1);
		}
		
		if(model_flag)
		{
			model_flag=0;
			switch(temp)
			{
				case '0':
					freq = 0;
					TIM3_count = 0;
					GPIO_ResetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);																//返回主界面 初始化
					mBicycle.outer_status = STATE_START_MODE;
				break;
				case '1':
					freq = 0;
					TIM3_count = 0;
					GPIO_ResetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);	
					mBicycle.outer_status = STATE_FREEDOM_MODE;
				break;
				case '2':
					freq = 0;
					warm_1 = 0;
					warm_count = 0;
					TIM3_count = 0;
					GPIO_ResetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);	
					mBicycle.outer_status = STATE_SPORT_MODE_WARM1;
				break;
				case '3':
					swing_count = 0;
					freq = 0;
					TIM3_count = 0;
					GPIO_ResetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);	
					mBicycle.outer_status = STATE_SPORT_MODE_WARM2;
				break;
				case '4':
					freq = 0;
					TIM3_count = 0;
					GPIO_ResetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);	
					mBicycle.outer_status = STATE_SPORT_MODE_WARM3;
				break;
				case '5':
					freq = 0;
					TIM3_count = 0;
					GPIO_ResetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);	
					mBicycle.outer_status = STATE_SPORT_MODE_LEVEL;
					mBicycle.outer_status = STATE_LESSON_MODE_LEVEL;
				break;
				case '6':
					freq = 0;
					TIM3_count = 0;
					GPIO_ResetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);	
					mBicycle.outer_status = STATE_SPORT_MODE_CLIMB;
					mBicycle.outer_status = STATE_LESSON_MODE_CLIMB;
				break;
				case '7':
                    freq = 0;
					TIM3_count = 0;
                    GPIO_ResetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
                    mBicycle.outer_status = STATE_OUTDOOR_MODE;
                    printf("321");
				break;

				default:
				break;
			}	
		}
		if(temp=='m')
		{
			model_flag=1;
		}
		if(freq_flag)
		{
			freq_flag=0;
			freq=temp - '0';
		}
		if(temp=='f')
		{
			freq_flag=1;
		}
	}
	
		
	} 
	 

		
void USART3_IRQHandler(void)
{
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{ 	
		USART_RX_3[usart3_counter] = USART_ReceiveData(USART3);			//接收数据
		//printf("   %d",USART_ReceiveData(USART3));
		if(usart3_counter == 0 && USART_RX_3[0] != 0x55) return;		//缓冲区第一个数据不是包头  跳过
		
		usart3_counter ++;
		if( usart3_counter == 11)																		//接收11个数据
		{
			memcpy(uasrt1_buff,USART_RX_3,11);
			usart3_counter = 0;																				//计数器清零
			Mpu_sign = 1;																							//新数据标志位置一
		}
	} 
	 
}

void UART4_IRQHandler(void)
{
	
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{ 	
		  if(speed_count!=0)
			{
				USART_RX_4[speed_count]=USART_ReceiveData(UART4);
				if(speed_count!=2)
				  speed_count=(speed_count+1)%8;
				else 
				{
					speed_count=4;
				}
				if(!speed_count)
					new_speed_flag=1;
					
			}
      if(USART_ReceiveData(UART4)=='s')
			{
				if(new_speed_flag==0)
				  speed_count=1;
			}
			if(USART_ReceiveData(UART4)=='R')
			{
				myfreq_left=1;
				switch(mBicycle.outer_status)
				{
					case STATE_FREEDOM_MODE:
					case STATE_SPORT_MODE_WARM3:
					case STATE_LESSON_MODE_WARM3:
					case STATE_LESSON_MODE_CLIMB:
					case STATE_SPORT_MODE_CLIMB:
					case STATE_LESSON_MODE_LEVEL:
					case STATE_SPORT_MODE_LEVEL:
						
						GPIOC->BSRR = GPIO_Pin_6;
					
					break;
				}
				
				
			}
				
			if(USART_ReceiveData(UART4)=='L')
			{
				myfreq_right=1;
				switch(mBicycle.outer_status)
				{
					case STATE_FREEDOM_MODE:
					case STATE_SPORT_MODE_WARM3:
					case STATE_LESSON_MODE_WARM3:
					case STATE_LESSON_MODE_CLIMB:
					case STATE_SPORT_MODE_CLIMB:
					case STATE_LESSON_MODE_LEVEL:
					case STATE_SPORT_MODE_LEVEL:
						
						GPIOC->BSRR = GPIO_Pin_5;
					
					break;
				}
				
			}
				
			
	} 
	 
}

extern COMMAND_PARSER parser;

void UART5_IRQHandler(void)
{
//    printf("123");
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	{ 	
            if(USART_ReceiveData(UART5)=='G')
            {
                mBicycle.outer_status=STATE_OUTDOOR_MODE;
                printf("page outdoor_mode");
                printf("%c",0xff);
                printf("%c",0xff);
                printf("%c",0xff); 
            }
	   OnRecieve(USART_ReceiveData(UART5));
	} 
	 
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

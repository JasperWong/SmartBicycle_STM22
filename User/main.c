#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "bsp_MPU9250.h"
#include "bicycle_led.h" 
#include "ledpoint.h"
#include "bicycle_mlx9016.h"
#include "bsp_adc.h"
#include "timer.h"
#include "bicycle.h"

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
  
BICYCLE mBicycle;		

uint8_t USART_RX_1[4];
uint8_t USART_RX_4[4];
uint8_t USART_RX_5;
extern int IBI;          //相邻节拍时间
extern int BPM;          //心率值             
extern int Signal;       //原始信号值            
extern unsigned char QS; //发现心跳标志
extern uint16_t TIM3_count;






int main(void)
{
	SysTick_Init();
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
	/* LED 端口初始化 */
	LED_GPIO_Config();
	/* mlx温度传感器 端口初始化 */
	mlx_GPIO_Config();
	/* LED点阵 端口初始化 */
	LEDPOINT_GPIO_Config();
	/* LED点阵初始化 */
	max72192_INIT();
	max72191_INIT();
	/* 串口通信初始化 */
	USART1_Config();
	USART4_Config();
	USART5_Config();
	/* ADC 初始化*/
	ADC2_Init();
	/*定时周期2ms*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	TIM3_Int_Init(1999,71);
	/* TIM3 重新开时钟，开始计时 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
	/*I2C初始化*/
	I2C_Bus_Init();
  
	mpu9250_config();
 
	while(1)
    {

		switch(mBicycle.outer_status)
		{
			case STATE_OUTDOOR_MODE:
                freedom_mode(USART_RX_5);
                break;
            defalut:
                break;
		}
	}
			gyro_data_ready_cb();
			mpu9250_running();

			if (new_data) 
			{
				inv_execute_on_data();
				read_from_mpl();
			}
			//			if(TIM3_count == 50) 
//			{
//				printf("原始信号值 %d",Signal);
//				printf("心率值 %d \r\n ",BPM);
//				TIM3_count = 0;
//			}
//  }	

}


/*********************************************END OF FILE**********************/

#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "bsp_MPU9250.h"
#include "bicycle_led.h" 
#include "ledpoint.h"
#include "bicycle_mlx9016.h"
#include "bsp_adc.h"
#include "timer.h"
/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
	
uint8_t USART_RX_1[4];
uint8_t USART_RX_4[4];
uint8_t USART_RX_5[4];
extern int IBI;          //���ڽ���ʱ��
extern int BPM;          //����ֵ             
extern int Signal;       //ԭʼ�ź�ֵ            
extern unsigned char QS; //����������־
extern uint16_t TIM3_count;
	
int main(void)
{
	SysTick_Init();
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();
	/* mlx�¶ȴ����� �˿ڳ�ʼ�� */
	mlx_GPIO_Config();
	/* LED���� �˿ڳ�ʼ�� */
	LEDPOINT_GPIO_Config();
	/* LED�����ʼ�� */
	max72192_INIT();
	/* ����ͨ�ų�ʼ�� */
	USART1_Config();
	USART4_Config();
	USART5_Config();
	/* ADC ��ʼ��*/
	ADC2_Init();
	/*��ʱ����2ms*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	TIM3_Int_Init(1999,71);
	/* TIM3 ���¿�ʱ�ӣ���ʼ��ʱ */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
	/*I2C��ʼ��*/
	I2C_Bus_Init();
  
	mpu9250_config();
 
  while(1){
		
//			if(TIM3_count == 50) 
//			{
//				printf("ԭʼ�ź�ֵ %d",Signal);
//				printf("����ֵ %d \r\n ",BPM);
//				TIM3_count = 0;
//			}
		
			gyro_data_ready_cb();
			mpu9250_running();

			if (new_data) 
			{
				inv_execute_on_data();
				read_from_mpl();
			}
  }	
}



/*********************************************END OF FILE**********************/

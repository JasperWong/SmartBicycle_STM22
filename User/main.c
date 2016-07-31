#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "ledpoint.h"
#include "bicycle_mlx9016.h"
#include "bsp_adc.h"
#include "timer.h"
#include "bicycle.h"
#include "stdlib.h"
#include "bicycle_led.h"   
#include "bsp_usart1.h"
#include "bsp_SysTick.h"
#include "string.h"

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
	

uint8_t USART_RX_4[9];
uint16_t USART_RX_5;
uint8_t USART_RX_3[11];
uint8_t uasrt1_buff[11];
uint8_t Mpu_sign;
extern int IBI;          //相邻节拍时间
extern int BPM;          //心率值             
extern int Signal;       //原始信号值            
extern unsigned char QS; //发现心跳标志
BICYCLE mBicycle;	
extern uint16_t count;

//vino
char USART_RX_1[17];						//用于保存usart1接收到的数据
uint8_t myfreq_left = 0;
uint8_t myfreq_right = 0;
uint8_t freq = 0;
uint8_t  package_clear[3] = {0xff,0xaa,0x52};

uint8_t new_speed_flag=0;
uint8_t te[5];
uint8_t swing_count;
char  change_int_string[5];
extern void Freq_led(uint8_t set_freq);
uint8_t  package_end[3] = {0xff,0xff,0xff};
uint8_t press_flag = 0;
extern uint8_t warm_count;
float Pitch,Roll,Yaw;
void get_te()
{
	uint16_t i;
	i=memread()*2-27315;
	te[0]=i/1000+'0';
	te[1]=i/100%10+'3';
	te[2]=i/10%10+'0';
	te[3]=i%10+'0';
}
 void send_txt(uint8_t data[],uint8_t len)
 {
	 uint8_t i;
	 printf("\"");
	 for(i=0;i<len;i++)
	 {
		 Usart_SendByte(USART1,data[i]);
		
	 }
	  printf("\"");
 }
 
  void send_speed()
 {
	 uint8_t i;
	 printf("\"");
	 for(i=1;i<7;i++)
	 {
		 Usart_SendByte(USART1,USART_RX_4[i]);
		
	 }
	  printf("\"");
 }
 
 void add_point(uint8_t * temp)
 {
	temp[4] = temp[3];
	temp[3] = temp[2];
	temp[2] = '.';
 }
//
 
//static inline void run_self_test(void)
//{
//    int result;
//    long gyro[3], accel[3];
//    MPU_DEBUG_FUNC();
//#if defined (MPU6500) || defined (MPU9250)
//    result = mpu_run_6500_self_test(gyro, accel, 0);
//#elif defined (MPU6050) || defined (MPU9150)
//    result = mpu_run_self_test(gyro, accel);
//#endif
//    if (result == 0x7) {
//	MPL_LOGI("Passed!\n");
//        MPL_LOGI("accel: %7.4f %7.4f %7.4f\n",
//                    accel[0]/65536.f,
//                    accel[1]/65536.f,
//                    accel[2]/65536.f);
//        MPL_LOGI("gyro: %7.4f %7.4f %7.4f\n",
//                    gyro[0]/65536.f,
//                    gyro[1]/65536.f,
//                    gyro[2]/65536.f);
//        /* Test passed. We can trust the gyro data here, so now we need to update calibrated data*/

//#ifdef USE_CAL_HW_REGISTERS
//        /*
//         * This portion of the code uses the HW offset registers that are in the MPUxxxx devices
//         * instead of pushing the cal data to the MPL software library
//         */
//        unsigned char i = 0;

//        for(i = 0; i<3; i++) {
//        	gyro[i] = (long)(gyro[i] * 32.8f); //convert to +-1000dps
//        	accel[i] *= 2048.f; //convert to +-16G
//        	accel[i] = accel[i] >> 16;
//        	gyro[i] = (long)(gyro[i] >> 16);
//        }

//        mpu_set_gyro_bias_reg(gyro);

//#if defined (MPU6500) || defined (MPU9250)
//        mpu_set_accel_bias_6500_reg(accel);
//#elif defined (MPU6050) || defined (MPU9150)
//        mpu_set_accel_bias_6050_reg(accel);
//#endif
//#else
//        /* Push the calibrated data to the MPL library.
//         *
//         * MPL expects biases in hardware units << 16, but self test returns
//		 * biases in g's << 16.
//		 */
//    	unsigned short accel_sens;
//    	float gyro_sens;

//		mpu_get_accel_sens(&accel_sens);
//		accel[0] *= accel_sens;
//		accel[1] *= accel_sens;
//		accel[2] *= accel_sens;
//		inv_set_accel_bias(accel, 3);
//		mpu_get_gyro_sens(&gyro_sens);
//		gyro[0] = (long) (gyro[0] * gyro_sens);
//		gyro[1] = (long) (gyro[1] * gyro_sens);
//		gyro[2] = (long) (gyro[2] * gyro_sens);
//		inv_set_gyro_bias(gyro, 3);
//#endif
//    }
//    else {
//            if (!(result & 0x1))
//                MPL_LOGE("Gyro failed.\n");
//            if (!(result & 0x2))
//                MPL_LOGE("Accel failed.\n");
//            if (!(result & 0x4))
//                MPL_LOGE("Compass failed.\n");
//     }

//}

 
int main(void)
{
	
	
	uint16_t myFreq_l = 0;
	uint16_t myFreq_r = 0;

	uint8_t swing_flag;
	
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
	USART3_Config();
	USART4_Config();
	USART5_Config();
	/* ADC 初始化*/
	ADC2_Init();
	/*定时周期2ms*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	TIM3_Int_Init(1999,71);
	/* TIM3 重新开时钟，开始计时 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
	Usart_SendStr(USART3,package_clear);
    USART_RX_4[3] = '.';
    clear_led();
  while(1)
	{	
		if(Mpu_sign)								//如果有新数据
		{
			memcpy(USART_RX_3,uasrt1_buff,11);
			Mpu_sign = 0;
			if(uasrt1_buff[0] == 0x55)
			{
				if(uasrt1_buff[1] == 0x53)
				{
					Pitch = ((short)(USART_RX_3[3]<<8| USART_RX_3[2]))/32768.0*180;   //X轴滚转角（x 轴）
					Roll = ((short)(USART_RX_3[5]<<8| USART_RX_3[4]))/32768.0*180;   //Y轴俯仰角（y 轴）
					Yaw = ((short)(USART_RX_3[7]<<8| USART_RX_3[6]))/32768.0*180;   //Z轴偏航角（z 轴)
					
//					printf("Pitch   %.4f  ",Pitch );printf("\r\n");
//					printf("Roll   %.4f  ",Roll );printf("\r\n");
//					printf("Yaw   %.4f  ",Yaw );printf("\r\n");
//					printf ("\r\n  \r\n");
				}
			}
		}
        mBicycle.outer_status=STATE_OUTDOOR_MODE;
		switch (mBicycle.outer_status)
			{
				case STATE_FREEDOM_MODE:
					
					if(new_speed_flag)
					{
						printf("speed.txt=");
						send_speed();
						Usart_SendByte(USART1,0xff);				//??
						Usart_SendByte(USART1,0xff);
						Usart_SendByte(USART1,0xff);
						new_speed_flag=0;
					}
					printf("heart_rate.val=");
					sprintf(change_int_string,"%d",BPM);
					printf("%s",change_int_string);
					Usart_SendByte(USART1,0xff);				//??
					Usart_SendByte(USART1,0xff);
					Usart_SendByte(USART1,0xff);
					printf("temper.txt=");
//					sprintf(change_int_string,"%d",memread()*2-27315);
					get_te();
					add_point(te);
					send_txt(te,5);
//					send_txt(change_int_string,5);
					Usart_SendByte(USART1,0xff);				//??
					Usart_SendByte(USART1,0xff);
					Usart_SendByte(USART1,0xff);
					
					if(myfreq_left)
					{
						myFreq_l ++ ;
						if(myFreq_l == 5)
						{
							myfreq_left = 0;
							myFreq_l = 0;
							GPIOC->BRR = GPIO_Pin_6;
						}
					}
					if(myfreq_right)
					{
						myFreq_r ++ ;
						if(myFreq_r == 5)
						{
							myfreq_right = 0;
							myFreq_r = 0;
							GPIOC->BRR = GPIO_Pin_5;
						}
					}
				break;
				
				case STATE_SPORT_MODE_WARM1:						//压肩
										
					if(Pitch < 40 ) 
					{
						led_tip_l_green_on;
						led_tip_l_red_off;
						press_flag = 1;
//						if(warm_count == 5)
//						{
//							warm_count = 0;
//							printf("page warm_2");
//							Usart_SendStr(USART1,package_end);
//						}
						
					}
					else
					{
						led_tip_l_green_off;
						led_tip_l_red_on;
						press_flag = 0;
					}
					
				break;
					
				case STATE_SPORT_MODE_WARM2:			//扭腰
					
					led_tip_l_red_off;
						
					if(Yaw > 40)
					{
						swing_flag = 1;
						led_tip_l_green_on;
						led_tip_r_green_off;
					}
					else if(Yaw < -50)
					{
						swing_flag = 1;
						led_tip_r_green_on;
						led_tip_l_green_off;
					}
					else
					{
						if(swing_flag)
						{
							swing_flag = 0;
							swing_count ++ ;
						}
						
						led_tip_l_green_off;
						led_tip_r_green_off;
						printf("times.val=");
						Usart_SendByte(USART1,swing_count+'0');
						Usart_SendByte(USART1,0xff);				//??
						Usart_SendByte(USART1,0xff);
						Usart_SendByte(USART1,0xff);
	
					}
					
				
				break;	

				case STATE_SPORT_MODE_WARM3:						//直立慢行

					if(Pitch >100 && Pitch < 120)
					{
						led_tip_l_green_on;
						led_tip_l_red_off;
					}	
					else
					{
						led_tip_l_green_off;
						led_tip_l_red_on;
					}
					
					if(myfreq_left)
					{
						myFreq_l ++ ;
						if(myFreq_l == 5)
						{
							myfreq_left = 0;
							myFreq_l = 0;
							GPIOC->BRR = GPIO_Pin_6;
						}
					}
					if(myfreq_right)
					{
						myFreq_r ++ ;
						if(myFreq_r == 5)
						{
							myfreq_right = 0;
							myFreq_r = 0;
							GPIOC->BRR = GPIO_Pin_5;
						}
					}
				
					
				break;
					
				case STATE_SPORT_MODE_LEVEL:						//平路模式
				case STATE_SPORT_MODE_CLIMB:
				case STATE_LESSON_MODE_LEVEL:
				case STATE_LESSON_MODE_CLIMB:
						
					if(new_speed_flag)
					{
						printf("speed.txt=");
						send_speed();
						Usart_SendByte(USART1,0xff);				//??
						Usart_SendByte(USART1,0xff);
						Usart_SendByte(USART1,0xff);
						new_speed_flag=0;
					}
					printf("heart_rate.val=");
					sprintf(change_int_string,"%d",BPM);
					printf("%s",change_int_string);
					Usart_SendByte(USART1,0xff);				//??
					Usart_SendByte(USART1,0xff);
					Usart_SendByte(USART1,0xff);
					printf("temper.txt=");
//					sprintf(change_int_string,"%d",memread()*2-27315);
					get_te();
					add_point(te);
					send_txt(te,5);
//					send_txt(change_int_string,5);
					Usart_SendByte(USART1,0xff);				//??
					Usart_SendByte(USART1,0xff);
					Usart_SendByte(USART1,0xff);
					if(Pitch < 60)
					{
						led_tip_l_red_on;
						led_tip_l_green_off;
					}
					else if(Pitch > 60 && Pitch < 80)
					{
						led_tip_l_red_off;
						led_tip_l_green_on;
					}
					else
					{
						led_tip_l_red_on;
						led_tip_l_green_off;
					}
					
					if( GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) == 1 || GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) == 1)
					{
						led_tip_r_red_on;
						led_tip_r_green_off;
					}
					else
					{
						led_tip_r_green_on;
						led_tip_r_red_off;
					}
						
				
					if(myfreq_left)
					{
						myFreq_l ++ ;
						if(myFreq_l == 10)
						{
							myfreq_left = 0;
							myFreq_l = 0;
							GPIOC->BRR = GPIO_Pin_6;
						}
					}
					if(myfreq_right)
					{
						myFreq_r ++ ;
						if(myFreq_r == 10)
						{
							myfreq_right = 0;
							myFreq_r = 0;
							GPIOC->BRR = GPIO_Pin_5;
						}
					}
					
				break;
            case STATE_OUTDOOR_MODE:
//                printf("free");
                freedom_mode();
                break;
			default:
				
				break;
			}
		
  }	
}



/*********************************************END OF FILE**********************/

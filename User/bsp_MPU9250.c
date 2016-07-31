#include "bsp_MPU9250.h"

int new_data = 0;
unsigned char new_temp = 0;
unsigned long timestamp;
uint16_t Systick_count;

unsigned char *mpl_key = (unsigned char*)"eMPL 5.1";

/* Platform-specific information. Kinda like a boardfile. */
struct platform_data_s {
    signed char orientation[9];
};

/* The sensors can be mounted onto the board in any orientation. The mounting
 * matrix seen below tells the MPL how to rotate the raw data from the
 * driver(s).
 * TODO: The following matrices refer to the configuration on internal test
 * boards at Invensense. If needed, please modify the matrices to match the
 * chip-to-body matrix for your particular set up.
 */
static struct platform_data_s gyro_pdata = {
		
		 .orientation = { -1, 0, 0,
                     0, 0, 1,
                     0, 1, 0}
};

//extern struct inv_sensor_cal_t sensors;

/* Get data from MPL.
 * TODO: Add return values to the inv_get_sensor_type_xxx APIs to differentiate
 * between new and stale data.
 */
void read_from_mpl(void)
{
    long  data[9];
    int8_t accuracy;
		char cStr [ 70 ];
		unsigned long timestamp;

		MPU_DEBUG_FUNC();
	
	/*ªÒ»°≈∑¿≠Ω«*/
	if (inv_get_sensor_type_euler(data, &accuracy,(inv_time_t*)&timestamp))
	{
		float Pitch,Roll,Yaw;
		Pitch =data[0]*1.0/(1<<16) ;
		Roll = data[1]*1.0/(1<<16);
		Yaw = data[2]*1.0/(1<<16);
		
		if(Systick_count >= 250)
		{
			Systick_count = 0;
//			printf("Pitch :   %.4f   ",Pitch);printf ("\r\n");
//			printf ("Roll :   %.4f   ", Roll);printf ("\r\n");
			printf ("Yaw :   %.4f   ", Yaw);printf ("\r\n");
			printf ("\r\n  \r\n");
		}
		
		
		}
}


void gyro_data_ready_cb(void)
{

    hal.new_gyro = 1;
}

void mpu9250_config(void)
{
	inv_error_t result;
	unsigned char accel_fsr;
	unsigned short gyro_rate, gyro_fsr;
	
	result = mpu_init();
	  
	inv_init_mpl();
	  
	/* Compute 6-axis and 9-axis quaternions. */
	inv_enable_quaternion();
	inv_enable_9x_sensor_fusion();

	/* Update gyro biases when not in motion.
	 * WARNING: These algorithms are mutually exclusive.
	 */
	inv_enable_fast_nomot();
	
	/* Update gyro biases when temperature changes. */
	inv_enable_gyro_tc();

	/* Allows use of the MPL APIs in read_from_mpl. */
	inv_enable_eMPL_outputs();
	
	inv_start_mpl();

	mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);

	/* Push both gyro and accel data into the FIFO. */
	mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);
	mpu_set_sample_rate(DEFAULT_MPU_HZ);
	
	/* Read back configuration in case it was set improperly. */
	mpu_get_sample_rate(&gyro_rate);
	mpu_get_gyro_fsr(&gyro_fsr);
	mpu_get_accel_fsr(&accel_fsr);

	/* Sync driver configuration with MPL. */
	/* Sample rate expected in microseconds. */
	inv_set_gyro_sample_rate(1000000L / gyro_rate);
	inv_set_accel_sample_rate(1000000L / gyro_rate);
	
	/* Set chip-to-body orientation matrix.
	 * Set hardware units to dps/g's/degrees scaling factor.
	 */
	inv_set_gyro_orientation_and_scale(
					inv_orientation_matrix_to_scalar(gyro_pdata.orientation),
					(long)gyro_fsr<<15);
	inv_set_accel_orientation_and_scale(
					inv_orientation_matrix_to_scalar(gyro_pdata.orientation),
					(long)accel_fsr<<15);
	/* Initialize HAL state variables. */

	hal.sensors = ACCEL_ON | GYRO_ON;

	hal.dmp_on = 0;
	hal.report = 0;
	hal.rx.cmd = 0;
	hal.next_pedo_ms = 0;
	hal.next_compass_ms = 0;
	hal.next_temp_ms = 0;

//  /* Compass reads are handled by scheduler. */
//  get_tick_count(&timestamp);

	dmp_load_motion_driver_firmware();
	dmp_set_orientation(
			inv_orientation_matrix_to_scalar(gyro_pdata.orientation));

	/*
	 * Known Bug -
	 * DMP when enabled will sample sensor data at 200Hz and output to FIFO at the rate
	 * specified in the dmp_set_fifo_rate API. The DMP will then sent an interrupt once
	 * a sample has been put into the FIFO. Therefore if the dmp_set_fifo_rate is at 25Hz
	 * there will be a 25Hz interrupt from the MPU device.
	 *
	 * There is a known issue in which if you do not enable DMP_FEATURE_TAP
	 * then the interrupts will be at 200Hz even if fifo rate
	 * is set at a different rate. To avoid this issue include the DMP_FEATURE_TAP
	 *
	 * DMP sensor fusion works only with gyro at +-2000dps and accel +-2G
	 */
	hal.dmp_features = DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
			DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
			DMP_FEATURE_GYRO_CAL;
	dmp_enable_feature(hal.dmp_features);
	dmp_set_fifo_rate(DEFAULT_MPU_HZ);
	mpu_set_dmp_state(1);
	hal.dmp_on = 1;	
}

void mpu9250_running(void)
{
    unsigned long sensor_timestamp;

flag:
    get_tick_count(&timestamp);

		/* Temperature data doesn't need to be read with every gyro sample.
		 * Let's make them timer-based like the compass reads.
		 */
		if (timestamp > hal.next_temp_ms) {
				hal.next_temp_ms = timestamp + TEMP_READ_MS;
				new_temp = 1;
		}

    if (!hal.sensors || !hal.new_gyro) {
			new_temp = 0;
			goto flag;
    }    
		
		if (hal.new_gyro && hal.lp_accel_mode) {
				short accel_short[3];
				long accel[3];
				mpu_get_accel_reg(accel_short, &sensor_timestamp);
				accel[0] = (long)accel_short[0];
				accel[1] = (long)accel_short[1];
				accel[2] = (long)accel_short[2];
				inv_build_accel(accel, 0, sensor_timestamp);
				new_data = 1;
				hal.new_gyro = 0;
		} else if (hal.new_gyro && hal.dmp_on) {
				short gyro[3], accel_short[3], sensors;
				unsigned char more;
				long accel[3], quat[4], temperature;

				dmp_read_fifo(gyro, accel_short, quat, &sensor_timestamp, &sensors, &more);
				if (!more)
						hal.new_gyro = 0;
				if (sensors & INV_XYZ_GYRO) {
						/* Push the new data to the MPL. */
						inv_build_gyro(gyro, sensor_timestamp);
						new_data = 1;
						if (new_temp) {
								new_temp = 0;
								/* Temperature only used for gyro temp comp. */
								mpu_get_temperature(&temperature, &sensor_timestamp);
								inv_build_temp(temperature, sensor_timestamp);
						}
				}
				if (sensors & INV_XYZ_ACCEL) {
						accel[0] = (long)accel_short[0];
						accel[1] = (long)accel_short[1];
						accel[2] = (long)accel_short[2];
						inv_build_accel(accel, 0, sensor_timestamp);
						new_data = 1;
				}
				if (sensors & INV_WXYZ_QUAT) {
						inv_build_quat(quat, 0, sensor_timestamp);
						new_data = 1;
				}
		} else if (hal.new_gyro) {
				short gyro[3], accel_short[3];
				unsigned char sensors, more;
				long accel[3], temperature;

				hal.new_gyro = 0;
				mpu_read_fifo(gyro, accel_short, &sensor_timestamp,
						&sensors, &more);
				if (more)
						hal.new_gyro = 1;
				if (sensors & INV_XYZ_GYRO) {
						/* Push the new data to the MPL. */
						inv_build_gyro(gyro, sensor_timestamp);
					  new_data = 1;
						if (new_temp) {
								new_temp = 0;
								/* Temperature only used for gyro temp comp. */
								mpu_get_temperature(&temperature, &sensor_timestamp);
								inv_build_temp(temperature, sensor_timestamp);
						}
				}
				if (sensors & INV_XYZ_ACCEL) {
						accel[0] = (long)accel_short[0];
						accel[1] = (long)accel_short[1];
						accel[2] = (long)accel_short[2];
						inv_build_accel(accel, 0, sensor_timestamp);
						new_data = 1;
				}
		}

}


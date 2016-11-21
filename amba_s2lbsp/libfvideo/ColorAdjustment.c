#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <getopt.h>
#include <sched.h>
#include <errno.h>
#include <basetypes.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/signal.h>
#include "img_adv_struct_arch.h"
#include "mw_struct.h"
#include "mw_api.h"
#include "ipc_inc.h"
#include "img_api_adv_arch.h"
#include "ColorAdjustment.h"
#define AAA_DISABLE	0


void BrightnessAdjustment(int brightness)  //亮度
{
	_printd("===Set brightness %d\n", brightness);
	//mw_get_brightness(&brightness);
	//_printd("===get brightness %d\n", brightness);
	//return ;
	if(AAA_DISABLE){
		return ;
	}
	int tmp = 0;
	tmp = (brightness-50)*3;
	
	if (tmp > 255 || tmp < -255)
		return ;
	mw_set_brightness(tmp); //brightness: (range -255 ~ 255)
}
void ContrastAdjustment(int contrast)    //对比度
{
	_printd("===Set contrast %d\n", contrast);
	//mw_get_contrast(&contrast);
	//_printd("===get contrast %d\n", contrast);
	//return ;
	if(AAA_DISABLE){
		return ;
	}
	int tmp = 0;
	tmp = (contrast)*128/100;
	
	if (tmp > 128 || tmp < 0)
		return ;
	mw_set_contrast(tmp); //contrast: (range 0 ~ 128)
}

void SaturationAdjustment(int saturation)  //饱和度
{
	_printd("===Set saturation %d\n", saturation);
	//mw_get_saturation(&saturation);
	//_printd("===get saturation %d\n", saturation);
	//return ;
	if(AAA_DISABLE){
		return ;
	}
	int tmp = 0;
	tmp = (saturation)*128/100;
	
	if (tmp > 128 || tmp < 0)
		return ;
	mw_set_saturation(tmp);  //saturation: (range 0 ~ 255)
}

void HueAdjustment(int hue)  //色度  -15~+15
{
	_printd("===Set hue %d\n", hue);
	//return ;
	if(AAA_DISABLE){
		return ;
	}
	int tmp = 0;
	tmp = (hue-50)*3/10;
	
	if (tmp > 15 || tmp < -15)
		return ;
	img_set_color_hue(tmp);
}

void SharpnessAdjustment(int sharpness)  //锐度sharpness: (range 0 ~ 255)
{
	_printd("===Set sharpness %d\n", sharpness);
	//mw_get_sharpness(&sharpness);
	//_printd("===get sharpness %d\n", sharpness);
	//return ;
	if(AAA_DISABLE){
		return ;
	}
	int tmp = 0;
	tmp = sharpness*11/100;
	
	if (tmp > 11 || tmp < 0)
		return ;
	mw_set_sharpness(tmp);    
}	      

//增益调节需要关闭白平衡 不做处理
void GainAdjustment(int gain)
{
	_printd("===Set gain %d\n", gain);
	if(AAA_DISABLE){
		return ;
	}
	int tmp = 0;
	tmp = gain*255/100;
	
	if (tmp > 255 || tmp < 0)
		return ;
	//mw_enable_ae(0);
	
}

int DayNightMode(int mode)
{
	if(AAA_DISABLE){
		return 0;
	}
	return mw_enable_day_night_mode(mode);
}
void* AdjustmentInit(void* arg)
{
	if(AAA_DISABLE){
		return NULL;
	}
	if (mw_start_aaa(iav_fd()) < 0) {
		perror("mw_start_aaa");
		return NULL;
	}

	#if 1
	mw_enable_ae(1);
	if (mw_is_dc_iris_supported()) {
		//mw_enable_dc_iris_control(0);
		mw_dc_iris_pid_coef pid_coef= {2500, 2, 5000};
		mw_set_dc_iris_pid_coef(&pid_coef);
		//mw_enable_dc_iris_control(1);
	}

	mw_enable_awb(1);
	mw_set_saturation(64);
	mw_set_contrast(64);
	mw_set_sharpness(64);
	//mw_set_mctf_strength(6);
	
	static mw_sys_info G_mw_info;
	if (G_mw_info.res.hdr_expo_num <= MIN_HDR_EXPOSURE_NUM) {
		_printd("===Sensor is Normal mode\n");
	} else {
		_printd("===Sensor is %dX HDR mode", G_mw_info.res.hdr_expo_num);
	}

	if (G_mw_info.res.hdr_expo_num <= MIN_HDR_EXPOSURE_NUM) {
		mw_ae_param ae_param = {
			.anti_flicker_mode		= MW_ANTI_FLICKER_60HZ,
			.shutter_time_min		= SHUTTER_1BY8000_SEC,
			.shutter_time_max		= SHUTTER_1BY30_SEC,
			.sensor_gain_max		= ISO_6400,
			.slow_shutter_enable	= 0,
			.ir_led_mode			= MW_IR_LED_MODE_AUTO,
		};
		mw_set_ae_param(&ae_param);
		//mw_set_ae_metering_mode(metering_mode);
		mw_set_brightness(0);
		mw_set_auto_local_exposure_mode(0);
		mw_enable_backlight_compensation(0);
	}
	#endif
	return NULL;
}

pthread_t pid_aaa = 0;
void aaa_start()
{
	if(AAA_DISABLE){
		return ;
	}
	pthread_create(&pid_aaa, NULL, AdjustmentInit, NULL);
}

void aaa_stop()
{
	if(AAA_DISABLE){
		return ;
	}
	pthread_cancel(pid_aaa);
	mw_stop_aaa();
}
void Set_anti_flicker_mode(int antiflicker)
{
	if(AAA_DISABLE){
		return ;
	}
	static mw_ae_param			ae_param = {
		.anti_flicker_mode		= MW_ANTI_FLICKER_60HZ,
		.shutter_time_min		= SHUTTER_1BY8000_SEC,
		.shutter_time_max		= SHUTTER_1BY30_SEC,
		.sensor_gain_max		= ISO_6400,
		.slow_shutter_enable	= 0,
		.ir_led_mode			= MW_IR_LED_MODE_AUTO,
	};

	mw_get_ae_param(&ae_param);
#if 0
	switch(antiflicker)
	{
		case 0://50Hz indoor
			ae_param.anti_flicker_mode = MW_ANTI_FLICKER_50HZ;
			ae_param.ir_led_mode = MW_IR_LED_MODE_ON;
			break;
			
		case 1://50Hz outdoor
			ae_param.anti_flicker_mode = MW_ANTI_FLICKER_50HZ;
			ae_param.ir_led_mode = MW_IR_LED_MODE_OFF;
			break;
			
//		case 2://50Hz auto
//			ae_param.anti_flicker_mode = MW_ANTI_FLICKER_50HZ;
//			ae_param.ir_led_mode = MW_IR_LED_MODE_AUTO;
//			break;
			
		case 3://60Hz indoor
			ae_param.anti_flicker_mode = MW_ANTI_FLICKER_60HZ;
			ae_param.ir_led_mode = MW_IR_LED_MODE_ON;
			break;
				
		case 4://60Hz outdoor
			ae_param.anti_flicker_mode = MW_ANTI_FLICKER_60HZ;
			ae_param.ir_led_mode = MW_IR_LED_MODE_OFF;
			break;
				
//		case 5://60Hz auto
//			ae_param.anti_flicker_mode = MW_ANTI_FLICKER_60HZ;
//			ae_param.ir_led_mode = MW_IR_LED_MODE_AUTO;
//			break;
		default:
			break;

	}
#else
	if (2/*第2位*/ ==  antiflicker){
		ae_param.anti_flicker_mode = MW_ANTI_FLICKER_50HZ;//0 - 50Hz  1 - 60Hz
	}else{
		ae_param.anti_flicker_mode = MW_ANTI_FLICKER_60HZ;;
	}
#endif	

	mw_set_ae_param(&ae_param);
}








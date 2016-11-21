#include "Video.h"
#include "ipc_inc.h"
#include "ColorAdjustment.h"
#include "Color2Grey.h"

extern int DayNightFlag;
extern int HdrFlag;

extern pthread_rwlock_t lock_stream;

/// 得到视频输出的通道数目。
/// 
/// \param 无
/// \return视频输出通道数目。
int Video2GetOutChannels(void)
{
	_printd("Test\n");
	return 0;
}

int VideoGetColorByType(int channel, int iType,VIDEO_COLOR * pColor)
{
	_printd("Test\n");
	switch(iType)
	{
		case ENUM_COLOR_YANLI:
		{
			pColor->Brightness = 50;
			pColor->Contrast	=50;
			pColor->Saturation = 75;
			pColor->Hue = 25;
			pColor->reserved[1] = 50;
		}
		break;
		case ENUM_COLOR_BIAOZHUN:
		{
			pColor->Brightness = 50;
			pColor->Contrast	=50;
			pColor->Saturation = 50;
			pColor->Hue = 50;	
			pColor->reserved[1] = 50;
		}
		break;
		default:
		{
			pColor->Brightness = 50;
			pColor->Contrast	=50;
			pColor->Saturation = 75;
			pColor->Hue = 25;
			pColor->reserved[1] = 50;
		}
		break;		
		
	}
	
	return 0;
}

int VideoSetTVoutColor(VIDEO_COLOR * pColor)
{
	_printd("Test\n");
	return 0;
}
/// 创建视频设备
/// 
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功



/// 销毁视频设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功




/// 设置视频的颜色参数。
/// 
/// \param [in] channel 通道号。
/// \param [in] pColor 指向颜色结构VIDEO _ COLOR的指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int VideoSetColor(int channel, VIDEO_COLOR * pColor)
{

	_printd("Test\n");
	static int brightness = 0;
	static int contrast = 0;
	static int saturation = 0;
	static int hue = 0;
	static int sharpness = 0;
	//static int gain = 0;
	//static int whiteBalance = 0;
	_printd("brightness:%d contrast:%d saturation:%d hue:%d gain:%d whiteBalance:%d reserved[0]:%d reserved[1]:%d\n", pColor->Brightness, pColor->Contrast, pColor->Saturation, pColor->Hue, pColor->Gain, pColor->WhiteBalance, pColor->reserved[0], pColor->reserved[1]);
	if (brightness != pColor->Brightness)
	{
		brightness = pColor->Brightness;
		BrightnessAdjustment(brightness); //brightness: (range -255 ~ 255)
	}

	if (contrast != pColor->Contrast)
	{
		contrast = pColor->Contrast;
		ContrastAdjustment(contrast);  //contrast: (range 0 ~ 128)
	}

	if (saturation != pColor->Saturation)
	{
		saturation = pColor->Saturation;
		SaturationAdjustment(saturation);  //saturation: (range 0 ~ 255)
	}

	if (hue != pColor->Hue)
	{
		hue = pColor->Hue;
		HueAdjustment(hue);     // hue: (range -15 ~ +15)
	}
	
	if (sharpness != pColor->reserved[1])
	{
		sharpness = pColor->reserved[1];
		SharpnessAdjustment(sharpness);  //sharpness: (range 0 ~ 255)
	}
	return 0;
}



/// 创建视频设备
/// 
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int VideoCreate(void){_printd("Test\n"); return 0;}
int Video2Create(int index){_printd("Test\n"); return 0;}


/// 销毁视频设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int VideoDestory(void){_printd("Test\n"); return 0;}
int Video2Destroy(int index){_printd("Test\n"); return 0;}


/// 设置视频的颜色参数。
/// 
/// \param [in] channel 通道号。
/// \param [in] pColor 指向颜色结构VIDEO _ COLOR的指针。
/// \retval 0  设置成功
/// \retval <0  设置失败

 
/// 设置视频的控制参数。
/// 
/// \param [in] channel 通道号。
/// \param [in] pColor 指向视频控制结构VIDEO_CONTROL的指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int VideoSetControl(int channel, VIDEO_CONTROL * pColor){_printd("Test\n"); return 0;}


/// 设置覆盖区域。
/// 
/// \param [in] channel 通道号。
/// \param [in] index 遮挡区域号
/// \param [in] pParam 指向视频覆盖结构VIDEO_COVER_PARAM的指针。
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int VideoSetCover(int channel, int index, VIDEO_COVER_PARAM *pParam)
{
	_printd("Test\n");
	
	return 0;
}


/// 设置视频制式。
/// 
/// \param [in] channel 通道号。
/// \param [in] dwStandard 视频制式，取video_standard_t类型的值。
/// \retval 0  设置成功
/// \retval <0  设置失败
int VideoSetVstd(int channel,uint dwStandard){_printd("Test\n"); return 0;}	


/// 写一个字节到视频设备寄存器。
/// 
/// \param [in] Chip 芯片序号。
/// \param [in] Page 页面序号。
/// \param [in] Register 寄存器序号。
/// \param [in] Data 要写入的值。
/// \return 无
void VideoWriteRegister(uchar Chip, uchar Page, uchar Register, uchar Data)
{
	_printd("Test\n");
}


/// 从视频设备寄存器读一个字节。
/// 
/// \param [in] Chip 芯片序号。
/// \param [in] Page 页面序号。
/// \param [in] Register 寄存器序号。
/// \return读到的值。
uchar VideoReadRegister(uchar Chip, uchar Page, uchar Register){_printd("Test\n"); return 0;}


/// 切换视频输出的类型。
/// \param [in] dwType 输出的类型，取下表中的值。
/// \retval 0  切换成功。
/// \retval <0  切换失败。
int VideoSwitch(uint dwType){_printd("Test\n"); return 0;}
int Video2Switch(int index, uint dwType){_printd("Test\n"); return 0;}


/// 设置TV输出的边界。视频显示在某些监视器上时， 4个边上会有损失。这个函数的目的
/// 就是设置4个边界压缩的比例，使视频落在监视器的可视区域内。调节范围0-100是一个
/// 示意值，实际上对应缩小的尺寸需要在底层做转换和限制。
/// 
/// \param [in] left 左边界比率，取值0-100， 默认值为0。
/// \param [in] top 上边界比率，取值0-100， 默认值为0。
/// \param [in] right 右边界比率，取值0-100， 默认值为0。
/// \param [in] bottom 下边界比率，取值0-100， 默认值为0。
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int VideoSetTVMargin(uchar left, uchar top, uchar right, uchar bottom){_printd("Test\n"); return 0;}


/// 设置TV输出的颜色。
/// 
/// \param [in] brightness 亮度，取值0-100， 默认值为50。
/// \param [in] contrast 对比度，取值0-100， 默认值为50。
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int VideoSetTVColor(uchar brightness, uchar contrast){_printd("Test\n"); return 0;}


/// 设置TV输出防抖动参数，部分老硬件会有抖动的问题。
/// 
/// \param [in] level 防抖动参数，抖动程度与取值不是比例关系，而是一个经验值。
///        取值0-100， 默认值为50。
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int VideoSetTVAntiDither(uchar level){_printd("Test\n"); return 0;}


/// 设置视频输出的适配器类型。
/// 
/// \param [in] type 适配器类型，取video_output_t类型的值。
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int VideoSetOutputAdapter(uchar type){_printd("Test\n"); return 0;}


/// 获取视频输出的适配器类型。
/// 
/// \param [out] ptype 适配器类型，取video_output_t类型的值。
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int VideoGetOutputAdapter(uchar *ptype){_printd("Test\n"); return 0;}


/// 连接视频矩阵的输出通道和输入通道。
/// 
/// \param [in]  channelIn 视频输入通道
/// \param [in]  channelOut 视频输出通道
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int VideoMatrix(uchar channelIn,uchar channelOut){_printd("Test\n"); return 0;}


/// 开关TV监视器
/// 
/// \param [in] open TRUE-开TV监视器，FALSE-关TV监视器
/// \retval 0  操作成功。
/// \retval <0  操作失败。
int VideoSwitchTVMonitor(VD_BOOL open){_printd("Test\n"); return 0;}


/// 开关VGA监视器
/// 
/// \param [in] open TRUE-开VGA监视器，FALSE-关VGA监视器
/// \retval 0  操作成功。
/// \retval <0  操作失败。
int VideoSwitchVGAMonitor(VD_BOOL open){_printd("Test\n"); return 0;}
/// 配置采集芯片的采集区域
/// 
/// \param [in] 对应通道的水平垂直偏移，以及宽高属性
/// \retval 0  操作成功。
/// \retval <0  操作失败。
int VideoSetCropRegion(int channel,int Hoffset,int Voffset,int Width,int Height){_printd("Test\n"); return 0;}
/// 得到视频支持的特性。
/// 
/// \param [out] pCaps 指向视频特性结构VIDEO_CAPS的指针。
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int VideoGetCaps(VIDEO_CAPS * pCaps){_printd("Test\n"); return 0;}
int Video2GetCaps(int index, VIDEO_CAPS * pCaps){_printd("Test\n"); return 0;}


/// 设置没有视频的监视画面的背景颜色.
/// 
/// \param [in] color 要设置的颜色，16进制表示为0x00BBGGRR，最高字节保留。
/// \retval 0  设置成功
/// \retval <0  设置失败
int VideoSetBkColor(uint color){_printd("Test\n"); return 0;}
int Video2SetBkColor(int index, uint color){_printd("Test\n"); return 0;}


/// 设置视频捕获分辨率
int VideoRecordSetSize(int channel, int size){_printd("Test\n"); return 0;}
	/*
说明	开始自动检测

参数	无

返回值	=0  ：	  成功。
	其他值：  失败。

*/
int VideoAutoDetectStart(void){_printd("Test\n"); return 0;}
/*
说明	停止自动检测


参数	无

返回值	=0  ：	  成功。
	其他值：  失败。
	*/
int VideoAutoDetectStop(void){_printd("Test\n"); return 0;}

/// @} end of group

/*
	说明:获取白电平控制能力值
	返回值=0:成功
	其他值:失败
*/
int WhiteLevelGetCaps(WHITE_LEVEL_CAPS * pCaps){_printd("Test\n"); return 0;}

/*
	set or get video mode
	SystemSetVideoMode() return 0 success
*/
enum video_standard_t video_mode=VIDEO_STANDARD_PAL;
int fps_ratio_numerator[VIDEO_STANDARD_SECAM+1] = {25, 30};
enum video_standard_t SystemGetVideoMode(void)
{
	_printd("get video mode = %d\n", video_mode);
	return video_mode;
}
int SystemSetVideoMode(enum video_standard_t videostand)
{
	_printd("set video mode = %d\n", videostand);
	video_mode = videostand;
	return 0;
	struct vindev_fps vsrc_fps;
	vsrc_fps.vsrc_id = 0;

	if(video_mode == VIDEO_STANDARD_PAL){
		vsrc_fps.fps = AMBA_VIDEO_FPS_25;
	}else
		vsrc_fps.fps = AMBA_VIDEO_FPS_30;
	//printf("set framerate %d\n", vsrc_fps.fps);
	 AM_IOCTL(iav_fd(), IAV_IOC_VIN_SET_FPS, &vsrc_fps);

	return 0;
}

int SetVideoFlipMirror(int bFlip/*水平*/, int bMirror)
{
	//struct vindev_mirror mirror_mode;
	pthread_rwlock_wrlock(&lock_stream);
	mirror_mode.vsrc_id = 0;
	mirror_mode.pattern = VINDEV_MIRROR_HORRIZONTALLY*bFlip+VINDEV_MIRROR_VERTICALLY*bMirror;
	if(NULL != strstr(ipc.sensor_buf, "ar0130")){
		mirror_mode.bayer_pattern = VINDEV_BAYER_PATTERN_GR;
	}else
		mirror_mode.bayer_pattern = VINDEV_BAYER_PATTERN_AUTO;
	AM_IOCTL(iav_fd(), IAV_IOC_VIN_SET_MIRROR, &mirror_mode);
	pthread_rwlock_unlock(&lock_stream);
	return 0;
}
int Set_exposure_mode(int exposuretype, int shutter)
{
	#if 0
	struct vindev_shutter vsrc_shutter;
	u32 shutter_time_q9;
	int vin_eshutter_time = 60;
	
	if(exposuretype == 1){
		mw_enable_ae(1);
	}else if(exposuretype == 2){
		vin_eshutter_time = shutter;
		//vin_eshutter_time = 15;// 1/15  sec
		shutter_time_q9 = 512000000/vin_eshutter_time;
		vsrc_shutter.vsrc_id = 0;
		vsrc_shutter.shutter = shutter_time_q9;
		AM_IOCTL(iav_fd() ,IAV_IOC_VIN_SET_SHUTTER, &vsrc_shutter);		
	}
	#endif
	return 0;
}

#define SNR_WDR_EN_FILE "/mnt/mtd/Config/WdrEn"

int GetSnrWdrEnable()
{
	static int SnrWDREn = -1;
	if (-1 == SnrWDREn)
	{
		FILE *fp = fopen(SNR_WDR_EN_FILE, "rb");
		if(fp)
		{
			char buf[128];
			memset(buf, 0, sizeof(buf));
			if (fgets(buf, sizeof(buf), fp))
			{
				sscanf(buf, "wdrEn=%d", &SnrWDREn);
			}
			fclose(fp);
		}
		else
		{
			fp = fopen(SNR_WDR_EN_FILE, "w+");
			fprintf(fp, "wdrEn=%d\n", 0);
			fclose(fp);			
		}
		
		if (SnrWDREn < 0 || SnrWDREn > 1)
		{
			SnrWDREn = 0;
		}
	}
	return SnrWDREn;
}

void SetSnrWdrEnable(int SnrWDREn)
{
	int ret;
	FILE* fp = fopen(SNR_WDR_EN_FILE, "w+");
	if( NULL == fp )
	{
		ERR_PRINT("open file failed: %s\n", SNR_WDR_EN_FILE);
		return ;
	}

	ret = fprintf(fp, "wdrEn=%d\n", SnrWDREn);
	if( ret <= 0 )
	{
		ERR_PRINT("write file failed: %s\n", SNR_WDR_EN_FILE);
	}

	fclose(fp);
	return;
}

int SetHdrMode()
{
	_printd("test\n");
	if(DayNightFlag == 0)//hdr没有黑白模式，不作处理
		return 0;
	
	struct vindev_mode vsrc_mode;
	memset(&vsrc_mode, 0, sizeof(vsrc_mode));
	struct iav_system_resource resource;
	memset(&resource, 0, sizeof(resource));
	vsrc_mode.vsrc_id = 0;
	resource.encode_mode = DSP_ADVANCED_ISO_MODE;
	
	pthread_rwlock_wrlock(&lock_stream);
	AM_IOCTL(iav_fd(), IAV_IOC_VIN_GET_MODE, &vsrc_mode);
	AM_IOCTL(iav_fd(), IAV_IOC_GET_SYSTEM_RESOURCE, &resource);
	resource.exposure_num = 2;
	vsrc_mode.hdr_mode = AMBA_VIDEO_2X_HDR_MODE;
	goto_idle();
	AM_IOCTL(iav_fd(), IAV_IOC_VIN_SET_MODE, &vsrc_mode);
	AM_IOCTL(iav_fd(), IAV_IOC_VIN_SET_MIRROR, &mirror_mode);
	AM_IOCTL(iav_fd(), IAV_IOC_SET_SYSTEM_RESOURCE, &resource);
	set_vin_frame_rate(AMBA_VIDEO_FPS_25);
	enable_preview();
	if(start_encode((1<<MAX_ENCODE_STREAM_NUM) - 5) < 0){
		pthread_rwlock_unlock(&lock_stream);
		//SystemReboot();//编码失败后暂时重启规避
		return -1;
	}
	pthread_rwlock_unlock(&lock_stream);

return 0;
}


int SetLinearMode()
{
	_printd("test\n");

	if(DayNightFlag == 0)//hdr没有黑白模式，不作处理
		return 0;
	
	struct vindev_mode vsrc_mode;
	memset(&vsrc_mode, 0, sizeof(vsrc_mode));
	struct iav_system_resource resource;
	memset(&resource, 0, sizeof(resource));
	vsrc_mode.vsrc_id = 0;
	resource.encode_mode = DSP_ADVANCED_ISO_MODE;
	pthread_rwlock_wrlock(&lock_stream);
	AM_IOCTL(iav_fd(), IAV_IOC_VIN_GET_MODE, &vsrc_mode);
	AM_IOCTL(iav_fd(), IAV_IOC_GET_SYSTEM_RESOURCE, &resource);
	resource.exposure_num = 1;
	vsrc_mode.hdr_mode = AMBA_VIDEO_LINEAR_MODE;
	goto_idle();
	AM_IOCTL(iav_fd(), IAV_IOC_VIN_SET_MODE, &vsrc_mode);
	AM_IOCTL(iav_fd(), IAV_IOC_VIN_SET_MIRROR, &mirror_mode);
	AM_IOCTL(iav_fd(), IAV_IOC_SET_SYSTEM_RESOURCE, &resource);
	set_vin_frame_rate(AMBA_VIDEO_FPS_25);
	enable_preview();
	if(start_encode((1<<MAX_ENCODE_STREAM_NUM) - 5) < 0){
		pthread_rwlock_unlock(&lock_stream);
		return -1;
	}
	pthread_rwlock_unlock(&lock_stream);

	
return 0;
}
int SetWdrMode(int wdr)
{
	if(wdr == 1){
		HdrFlag = 1;
		SetHdrMode();
	}else{
		HdrFlag = 0;
		SetLinearMode();
	}
return 0;	
}

/*
	说明: 		设置
	参数in: 	iChannel 通道
				pFormat设置结构体
	参数out: 	无
	返回值	=0  ：	  成功。
		<0：  失败。

*/
int VideoInSetCamera(uint iChannel, CAMERA_FORMAT *pFormat)
{
	_printd("HorReverse = %d VerReverse = %d Color2WhiteBlack = %d ExposureType = %d shutter = %d\n",pFormat->HorReverse, pFormat->VerReverse, pFormat->Color2WhiteBlack, pFormat->ExposureType, pFormat->shutter);
	_printd("pFormat->AntiFlicker = %x PAA = %d\n", pFormat->AntiFlicker, pFormat->PAA);
	if (NULL != strstr(ipc.sensor_buf, "ov4689")){
		pFormat->HorReverse = !pFormat->HorReverse;
	}
	SetVideoFlipMirror(pFormat->HorReverse, pFormat->VerReverse);
	Set_anti_flicker_mode(pFormat->AntiFlicker);//0 - 50Hz  1 - 60Hz

	Set_exposure_mode(pFormat->ExposureType, pFormat->shutter);

	// 0 auto 1 open 2 close
	_printd("pFormat->IRCut = %d\n", pFormat->IRCut);
	SetIrCutStatus(pFormat->IRCut);
	return 0;
}

int VideoInSetWhiteBalance(uint iChannel, CAMERA_WHITEBALANCE *pWhiteBalance){_printd("Test\n"); return 0;}

int VideoInSetCameraExt(uint iChannel,CAMERA_FORMAT_EXT *pFormatExt)
{
	_printd("snrWDRenable = %d\n", pFormatExt->snrWDRenable);
	//SetWdrMode(pFormatExt->snrWDRenable);
	return 0;
}

/*
	说明: 		获取镜头能力结构体
	参数in: 	ich 通道
	参数out: 	pCaps，镜头能力结构体
	返回值	=0  ：	  成功。
		<0：  失败。

*/
//int VideoInGetChCaps(int ich , VIDEOIN_CAPS* pCaps){_printd("Test\n"); return 0;}

int VideoInGetChCaps(int ich , VIDEOIN_CAPS* pCaps)
{
	if (ich != 0)
		return -1;
	if (pCaps == NULL)
		return -1;

	memset(pCaps, 0, sizeof(VIDEOIN_CAPS));
	//pCaps->uiCamAbility = (1<<0)|(1<<1)|(1<<2)|(1<<5)|(1<<8)|(1<<13)|(1<<16);
	pCaps->uiCamAbility = 	(1 << 0)|
						 	(1 << 1)|
						 	(1 << 8)|			
						 	(1 << 11);
						 	//(1 << 26);   //hdr					 						 
	
	pCaps->uiSenSpan = 255;
	pCaps->uiBLightSpan = 15;
	pCaps->uiShutterL = 100;
	pCaps->uiShutterH = 15;
	pCaps->uiWhiteBlance =0xff;
//	pCaps->uiAntiFlicker =0x42d;
	pCaps->uiAntiFlicker = (1 << 2) | (1 <<5);
//	pCaps->uiAntiFlicker = (1 << 2)|(1 <<5)|(1 <<3)|(1 << 4);

	return 0;
}

int VideoBadPixelDetect(void){_printd("Test\n"); return 0;}


int g_initsensort = 0;
static VD_SENSOR_TYPE_E g_SensorType = VISIONDIGI_HITACHI_110_960P20;
//!设置Sensor type, 以供ipc使用
int VideoInSetSensorType(int ich, VD_SENSOR_TYPE_E type)
{
	return 0;
}
VD_SENSOR_TYPE_E VideoInGetSensorType(int ich)
{
	if(g_initsensort ==0)
	{
		g_initsensort = 1;
	}
	g_SensorType = 30;
	return g_SensorType;
}
//!设置Sensor 使用类型 1080P  用作720P 编码, 以供ipc使用
int VideoInSetSensorMode(int ich, VD_SENSOR_MOED type){_printd("Test\n"); return 0;}
VD_SENSOR_MOED VideoInGetSensorMode(int ich){_printd("Test\n"); return 0;}


int VideoInGetSensorCap(VD_SENSOR_CAP_T* pCaps){_printd("Test\n"); return 0;}
int GetProfileType(){_printd("Test\n"); return 0;}
int SetProfileType(int type){_printd("Test\n"); return 0;}

int SetCameraVal(int id,int id2,int value){_printd("Test\n"); return 0;}


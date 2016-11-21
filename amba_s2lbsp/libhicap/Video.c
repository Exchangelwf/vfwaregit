#include "Video.h"
#include "ipc_inc.h"
#include "ColorAdjustment.h"
#include "Color2Grey.h"

extern int DayNightFlag;
extern int HdrFlag;

extern pthread_rwlock_t lock_stream;

/// �õ���Ƶ�����ͨ����Ŀ��
/// 
/// \param ��
/// \return��Ƶ���ͨ����Ŀ��
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
/// ������Ƶ�豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�



/// ������Ƶ�豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�




/// ������Ƶ����ɫ������
/// 
/// \param [in] channel ͨ���š�
/// \param [in] pColor ָ����ɫ�ṹVIDEO _ COLOR��ָ�롣
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
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



/// ������Ƶ�豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int VideoCreate(void){_printd("Test\n"); return 0;}
int Video2Create(int index){_printd("Test\n"); return 0;}


/// ������Ƶ�豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int VideoDestory(void){_printd("Test\n"); return 0;}
int Video2Destroy(int index){_printd("Test\n"); return 0;}


/// ������Ƶ����ɫ������
/// 
/// \param [in] channel ͨ���š�
/// \param [in] pColor ָ����ɫ�ṹVIDEO _ COLOR��ָ�롣
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��

 
/// ������Ƶ�Ŀ��Ʋ�����
/// 
/// \param [in] channel ͨ���š�
/// \param [in] pColor ָ����Ƶ���ƽṹVIDEO_CONTROL��ָ�롣
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int VideoSetControl(int channel, VIDEO_CONTROL * pColor){_printd("Test\n"); return 0;}


/// ���ø�������
/// 
/// \param [in] channel ͨ���š�
/// \param [in] index �ڵ������
/// \param [in] pParam ָ����Ƶ���ǽṹVIDEO_COVER_PARAM��ָ�롣
/// \retval 0  ���óɹ���
/// \retval <0  ����ʧ�ܡ�
int VideoSetCover(int channel, int index, VIDEO_COVER_PARAM *pParam)
{
	_printd("Test\n");
	
	return 0;
}


/// ������Ƶ��ʽ��
/// 
/// \param [in] channel ͨ���š�
/// \param [in] dwStandard ��Ƶ��ʽ��ȡvideo_standard_t���͵�ֵ��
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int VideoSetVstd(int channel,uint dwStandard){_printd("Test\n"); return 0;}	


/// дһ���ֽڵ���Ƶ�豸�Ĵ�����
/// 
/// \param [in] Chip оƬ��š�
/// \param [in] Page ҳ����š�
/// \param [in] Register �Ĵ�����š�
/// \param [in] Data Ҫд���ֵ��
/// \return ��
void VideoWriteRegister(uchar Chip, uchar Page, uchar Register, uchar Data)
{
	_printd("Test\n");
}


/// ����Ƶ�豸�Ĵ�����һ���ֽڡ�
/// 
/// \param [in] Chip оƬ��š�
/// \param [in] Page ҳ����š�
/// \param [in] Register �Ĵ�����š�
/// \return������ֵ��
uchar VideoReadRegister(uchar Chip, uchar Page, uchar Register){_printd("Test\n"); return 0;}


/// �л���Ƶ��������͡�
/// \param [in] dwType ��������ͣ�ȡ�±��е�ֵ��
/// \retval 0  �л��ɹ���
/// \retval <0  �л�ʧ�ܡ�
int VideoSwitch(uint dwType){_printd("Test\n"); return 0;}
int Video2Switch(int index, uint dwType){_printd("Test\n"); return 0;}


/// ����TV����ı߽硣��Ƶ��ʾ��ĳЩ��������ʱ�� 4�����ϻ�����ʧ�����������Ŀ��
/// ��������4���߽�ѹ���ı�����ʹ��Ƶ���ڼ������Ŀ��������ڡ����ڷ�Χ0-100��һ��
/// ʾ��ֵ��ʵ���϶�Ӧ��С�ĳߴ���Ҫ�ڵײ���ת�������ơ�
/// 
/// \param [in] left ��߽���ʣ�ȡֵ0-100�� Ĭ��ֵΪ0��
/// \param [in] top �ϱ߽���ʣ�ȡֵ0-100�� Ĭ��ֵΪ0��
/// \param [in] right �ұ߽���ʣ�ȡֵ0-100�� Ĭ��ֵΪ0��
/// \param [in] bottom �±߽���ʣ�ȡֵ0-100�� Ĭ��ֵΪ0��
/// \retval 0  ���óɹ���
/// \retval <0  ����ʧ�ܡ�
int VideoSetTVMargin(uchar left, uchar top, uchar right, uchar bottom){_printd("Test\n"); return 0;}


/// ����TV�������ɫ��
/// 
/// \param [in] brightness ���ȣ�ȡֵ0-100�� Ĭ��ֵΪ50��
/// \param [in] contrast �Աȶȣ�ȡֵ0-100�� Ĭ��ֵΪ50��
/// \retval 0  ���óɹ���
/// \retval <0  ����ʧ�ܡ�
int VideoSetTVColor(uchar brightness, uchar contrast){_printd("Test\n"); return 0;}


/// ����TV���������������������Ӳ�����ж��������⡣
/// 
/// \param [in] level �����������������̶���ȡֵ���Ǳ�����ϵ������һ������ֵ��
///        ȡֵ0-100�� Ĭ��ֵΪ50��
/// \retval 0  ���óɹ���
/// \retval <0  ����ʧ�ܡ�
int VideoSetTVAntiDither(uchar level){_printd("Test\n"); return 0;}


/// ������Ƶ��������������͡�
/// 
/// \param [in] type ���������ͣ�ȡvideo_output_t���͵�ֵ��
/// \retval 0  ���óɹ���
/// \retval <0  ����ʧ�ܡ�
int VideoSetOutputAdapter(uchar type){_printd("Test\n"); return 0;}


/// ��ȡ��Ƶ��������������͡�
/// 
/// \param [out] ptype ���������ͣ�ȡvideo_output_t���͵�ֵ��
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int VideoGetOutputAdapter(uchar *ptype){_printd("Test\n"); return 0;}


/// ������Ƶ��������ͨ��������ͨ����
/// 
/// \param [in]  channelIn ��Ƶ����ͨ��
/// \param [in]  channelOut ��Ƶ���ͨ��
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int VideoMatrix(uchar channelIn,uchar channelOut){_printd("Test\n"); return 0;}


/// ����TV������
/// 
/// \param [in] open TRUE-��TV��������FALSE-��TV������
/// \retval 0  �����ɹ���
/// \retval <0  ����ʧ�ܡ�
int VideoSwitchTVMonitor(VD_BOOL open){_printd("Test\n"); return 0;}


/// ����VGA������
/// 
/// \param [in] open TRUE-��VGA��������FALSE-��VGA������
/// \retval 0  �����ɹ���
/// \retval <0  ����ʧ�ܡ�
int VideoSwitchVGAMonitor(VD_BOOL open){_printd("Test\n"); return 0;}
/// ���òɼ�оƬ�Ĳɼ�����
/// 
/// \param [in] ��Ӧͨ����ˮƽ��ֱƫ�ƣ��Լ��������
/// \retval 0  �����ɹ���
/// \retval <0  ����ʧ�ܡ�
int VideoSetCropRegion(int channel,int Hoffset,int Voffset,int Width,int Height){_printd("Test\n"); return 0;}
/// �õ���Ƶ֧�ֵ����ԡ�
/// 
/// \param [out] pCaps ָ����Ƶ���ԽṹVIDEO_CAPS��ָ�롣
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int VideoGetCaps(VIDEO_CAPS * pCaps){_printd("Test\n"); return 0;}
int Video2GetCaps(int index, VIDEO_CAPS * pCaps){_printd("Test\n"); return 0;}


/// ����û����Ƶ�ļ��ӻ���ı�����ɫ.
/// 
/// \param [in] color Ҫ���õ���ɫ��16���Ʊ�ʾΪ0x00BBGGRR������ֽڱ�����
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int VideoSetBkColor(uint color){_printd("Test\n"); return 0;}
int Video2SetBkColor(int index, uint color){_printd("Test\n"); return 0;}


/// ������Ƶ����ֱ���
int VideoRecordSetSize(int channel, int size){_printd("Test\n"); return 0;}
	/*
˵��	��ʼ�Զ����

����	��

����ֵ	=0  ��	  �ɹ���
	����ֵ��  ʧ�ܡ�

*/
int VideoAutoDetectStart(void){_printd("Test\n"); return 0;}
/*
˵��	ֹͣ�Զ����


����	��

����ֵ	=0  ��	  �ɹ���
	����ֵ��  ʧ�ܡ�
	*/
int VideoAutoDetectStop(void){_printd("Test\n"); return 0;}

/// @} end of group

/*
	˵��:��ȡ�׵�ƽ��������ֵ
	����ֵ=0:�ɹ�
	����ֵ:ʧ��
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

int SetVideoFlipMirror(int bFlip/*ˮƽ*/, int bMirror)
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
	if(DayNightFlag == 0)//hdrû�кڰ�ģʽ����������
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
		//SystemReboot();//����ʧ�ܺ���ʱ�������
		return -1;
	}
	pthread_rwlock_unlock(&lock_stream);

return 0;
}


int SetLinearMode()
{
	_printd("test\n");

	if(DayNightFlag == 0)//hdrû�кڰ�ģʽ����������
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
	˵��: 		����
	����in: 	iChannel ͨ��
				pFormat���ýṹ��
	����out: 	��
	����ֵ	=0  ��	  �ɹ���
		<0��  ʧ�ܡ�

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
	˵��: 		��ȡ��ͷ�����ṹ��
	����in: 	ich ͨ��
	����out: 	pCaps����ͷ�����ṹ��
	����ֵ	=0  ��	  �ɹ���
		<0��  ʧ�ܡ�

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
//!����Sensor type, �Թ�ipcʹ��
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
//!����Sensor ʹ������ 1080P  ����720P ����, �Թ�ipcʹ��
int VideoInSetSensorMode(int ich, VD_SENSOR_MOED type){_printd("Test\n"); return 0;}
VD_SENSOR_MOED VideoInGetSensorMode(int ich){_printd("Test\n"); return 0;}


int VideoInGetSensorCap(VD_SENSOR_CAP_T* pCaps){_printd("Test\n"); return 0;}
int GetProfileType(){_printd("Test\n"); return 0;}
int SetProfileType(int type){_printd("Test\n"); return 0;}

int SetCameraVal(int id,int id2,int value){_printd("Test\n"); return 0;}


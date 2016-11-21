#include "Capture.h"
#include "ipc_inc.h"
#include "Color2Grey.h"
#include "ColorAdjustment.h"
#include "DrawOsd.h"

#define DVR_CHN_MAX	1
#define PRIVACY_NUM		4

extern int adc_open_led_num;
extern int adc_close_led_num;

extern int draw_str_flag;
extern int draw_time_flag;
extern int draw_font_size;	//add by SuperJoki
static pthread_mutex_t capture_set_title_lock = PTHREAD_MUTEX_INITIALIZER; //add by SuperJoki
extern pthread_rwlock_t lock_stream; 

VIDEO_COVER_PARAM PRIVACY_PARAM[PRIVACY_NUM];
#define BSP_VERSION   "V1.00.000"

char * BSP_GetVersion(void)
{
	static char version[64]={0};
	sprintf(version, "S2LM" BSP_VERSION);
	return version;
}

int CapSize2Res(int CapRes, int *high, int *width)
{
	switch (CapRes)
	{
		case CAPTURE_SIZE_D1: ///< 720*576(PAL) 720*480(NTSC)
			*high = 576;
			*width = 720;
			break;
		case CAPTURE_SIZE_HD1:///< 352*576(PAL) 352*480(NTSC)
			*high = 352;
			*width = 576;
			break;
		case CAPTURE_SIZE_BCIF: ///< 720*288(PAL) 720*240(NTSC)
			*high = 288;
			*width = 720;
			break;
		case CAPTURE_SIZE_CIF: ///< 352*288(PAL) 352*240(NTSC)
			*high = 288;
			*width = 352;
			break;
		case CAPTURE_SIZE_QCIF: ///< 176*144(PAL) 176*120(NTSC)
			*high = 144;
			*width = 176;
			break;
		case CAPTURE_SIZE_VGA: ///< 640*480(PAL) 640*480(NTSC)
			*high = 480;
			*width = 640;
			break;
		case CAPTURE_SIZE_QVGA: ///< 320*240(PAL) 320*240(NTSC)
			*high = 240;
			*width = 320;
			break;
		case CAPTURE_SIZE_SVCD: ///< 480*480(PAL) 480*480(NTSC)
			*high = 480;
			*width = 480;
			break;
		case CAPTURE_SIZE_QQVGA: ///< 160*128(PAL) 160*128(NTSC)
			*high = 128;
			*width = 160;
			break;
		case CAPTURE_SIZE_720P: ///< 1280*720    720p
			*high = 720;
			*width = 1280;
			break;
		case CAPTURE_SIZE_1080P: ///< 1920*1080  1080p 
			*high = 1080;
			*width = 1920;
			break;
		case CAPTURE_SIZE_SVGA: // <  800*592
			*high = 592;
			*width = 800;
			break;
		case CAPTURE_SIZE_XVGA: // <  1024*768    VGA������룬2800��Ŀ��Ҫ
			*high = 768;
			*width = 1024;
			break;
		case CAPTURE_SIZE_WXGA: // <  1280*800
			*high = 800;
			*width = 1280;
			break;
		case CAPTURE_SIZE_SXGA: // <  1280*1024  
			*high = 1024;
			*width = 1280;
			break;
		case CAPTURE_SIZE_WSXGA: // <  1600*1024  
			*high = 1024;
			*width = 1600;
			break;
		case CAPTURE_SIZE_UXGA: // <  1600*1200
			*high = 1200;
			*width = 1600;
			break;
		case CAPTURE_SIZE_WUXGA: // <  1920*1200
			*high = 1200;
			*width = 1920;
			break;
		case CAPTURE_SIZE_1280_960: // <  1280*960
			*high = 960;
			*width = 1280;
			break;
		case CAPTURE_SIZE_300W: // <  2048*1520
			*high = 1520;
			*width = 2048;
			break;
		case CAPTURE_SIZE_400W: // <  2688*1520
			*high = 1520;
			*width = 2688;
			break;
		case CAPTURE_SIZE_500W: // <  2592*1944
			*high = 1944;
			*width = 2592;
			break;
		case CAPTURE_SIZE_600W: // <  3072*2048
			*high = 2048;
			*width = 3072;
			break;
		case CAPTURE_SIZE_800W: // <  3840*2160
			*high = 2160;
			*width = 3840;
			break;
		case CAPTURE_SIZE_4K: // <  4096*2160
			*high = 2160;
			*width = 4096;
			break;
		case CAPTURE_SIZE_8K: // <  7680*4320
			*high = 4320;
			*width = 7680;
			break;

		default:
			break;
	}
	
	return 0;
}

/// ���DSP�ı�����������Ҫ��GBE��LB2.0ʹ��
///
int CaptureGetDspInfo(CAPTURE_DSPINFO *pDspInfo)
{
	if(pDspInfo == NULL)
	{
		return -1;
	}
	
	pDspInfo->nMaxSupportChannel = DVR_CHN_MAX;
	pDspInfo->nMaxEncodePower =  DVR_CHN_MAX*7680*4320*30;
	pDspInfo->bChannelMaxSetSync = 0;
	pDspInfo->nExpandChannel	= 1;

	return 0;
}


/// �õ������ͨ����Ŀ�����������������ͨ�������������ͨ������󲶻�ͨ���ż�1��
/// 
/// \return�����ͨ����Ŀ��
int CaptureGetChannels(void)
{
	_printd("Test\n");
	return DVR_CHN_MAX;
}


/// �õ�����֧�ֵ����ԡ�
/// 
/// \param [out] pCaps ָ�򲶻����ԽṹCAPTURE_CAPS��ָ�롣
/// \retval 0  ��ȡ�ɹ���
/// \retval 0  ��ȡʧ�ܡ�
int CaptureGetCaps(CAPTURE_CAPS * pCaps)
{
	if(pCaps == NULL)
	{
		return -1;
	}

	pCaps->Reserved = 0;

	pCaps->BitRateControl = (1 << CAPTURE_BRC_CBR) | (1 << CAPTURE_BRC_VBR);
	pCaps->Compression = 1 << CAPTURE_COMP_H264;
	//getsensortype
	if(NULL != strstr(ipc.sensor_buf, "ar0130")){
		pCaps->ImageSize = (1 << CAPTURE_SIZE_1080P) | (1 << CAPTURE_SIZE_720P);
		//pCaps->ImageSize = (1 << CAPTURE_SIZE_1280_960) | (1 << CAPTURE_SIZE_720P);
	}
	else if (NULL != strstr(ipc.sensor_buf, "ar0330")){
		pCaps->ImageSize = (1 << CAPTURE_SIZE_300W) | (1 << CAPTURE_SIZE_1080P) | (1 << CAPTURE_SIZE_720P);
	}	
	else if (NULL != strstr(ipc.sensor_buf, "ov4689")){

			pCaps->ImageSize = (1 << CAPTURE_SIZE_400W) | (1 << CAPTURE_SIZE_300W) | (1 << CAPTURE_SIZE_1080P) | (1 << CAPTURE_SIZE_720P);
			//pCaps->ImageSize = (1 << CAPTURE_SIZE_300W) | (1 << CAPTURE_SIZE_1080P) | (1 << CAPTURE_SIZE_720P);
			//pCaps->ImageSize = (1 << CAPTURE_SIZE_1080P) | (1 << CAPTURE_SIZE_720P);
			//pCaps->ImageSize = (1 << CAPTURE_SIZE_1280_960) | (1 << CAPTURE_SIZE_720P);
	}
	else if (NULL != strstr(ipc.sensor_buf, "ov5658")){
		pCaps->ImageSize = (1 << CAPTURE_SIZE_500W) | (1 << CAPTURE_SIZE_300W) | (1 << CAPTURE_SIZE_1080P) | (1 << CAPTURE_SIZE_720P);
	}
	pCaps->ImageSize |=(1 << CAPTURE_SIZE_1280_960);
	pCaps->Media = CAPTURE_START_VIDEO|CAPTURE_START_AUDIO;
	pCaps->CoverBlocks = 4;
	pCaps->RegionCover = 2;
	pCaps->TitleCount = 6;

	return 0;
}

/**
 * @brief ȡ��ÿͨ���ĵĲ������ԣ��ڸ�ͨ��������һ��ʱ��ʹ��
 * @param chn :ָ��Ҫȡ��ͨ��
 * @param pCaps: ָ�򲶻����ԽṹCAPTURE_CAPS��ָ��
 * @return 0  ��ȡ�ɹ�
 */
int CaptureGetChnCaps(int chn, CAPTURE_CAPS * pCaps)
{	
	return CaptureGetCaps(pCaps);
}
/// �õ�����˫����֧�ֵ����ԡ�������CaptureGetCapsһ�������õ�����˫����֧�ֵ����ԣ�
/// ��������ͨ�������ԡ� 
/// 
/// \param [out] pCaps ָ�򲶻����ԽṹCAPTURE_CAPS��ָ�롣
/// \retval 0  ��ȡ�ɹ���
/// \retval 0  ��ȡʧ�ܡ�
int CaptureGetExtCaps(CAPTURE_EXT_STREAM *pCaps)
{
	if(pCaps == NULL)
	{
		return -1;
	}
	pCaps->ExtraStream	= (1 << CHL_MAIN_T)|(1 << CHL_2END_T)|(1 << CHL_JPEG_T);
	if(NULL != strstr(ipc.sensor_buf, "ar0330")){
		pCaps->CaptureSizeMask[CAPTURE_SIZE_300W] |= (1 <<CAPTURE_SIZE_D1);
		pCaps->CaptureSizeMask[CAPTURE_SIZE_300W] |= (1 <<CAPTURE_SIZE_QVGA);
		pCaps->CaptureSizeMask[CAPTURE_SIZE_300W] |= (1 <<CAPTURE_SIZE_VGA);
		
		pCaps->CaptureSizeMask[CAPTURE_SIZE_1080P] |= (1 <<CAPTURE_SIZE_D1);
		pCaps->CaptureSizeMask[CAPTURE_SIZE_1080P] |= (1 <<CAPTURE_SIZE_QVGA);
		pCaps->CaptureSizeMask[CAPTURE_SIZE_1080P] |= (1 <<CAPTURE_SIZE_VGA);
	}
	else if(NULL != strstr(ipc.sensor_buf, "ar0130")){
		pCaps->CaptureSizeMask[CAPTURE_SIZE_1080P] |= (1 <<CAPTURE_SIZE_D1);
		pCaps->CaptureSizeMask[CAPTURE_SIZE_1080P] |= (1 <<CAPTURE_SIZE_QVGA);
		pCaps->CaptureSizeMask[CAPTURE_SIZE_1080P] |= (1 <<CAPTURE_SIZE_VGA);
		
	//	pCaps->CaptureSizeMask[CAPTURE_SIZE_1280_960] |= (1 <<CAPTURE_SIZE_D1);
	//	pCaps->CaptureSizeMask[CAPTURE_SIZE_1280_960] |= (1 <<CAPTURE_SIZE_QVGA);
	//	pCaps->CaptureSizeMask[CAPTURE_SIZE_1280_960] |= (1 <<CAPTURE_SIZE_VGA);
	}
	else if(NULL != strstr(ipc.sensor_buf, "ov4689")){
			pCaps->CaptureSizeMask[CAPTURE_SIZE_400W] |= (1 <<CAPTURE_SIZE_D1);
			pCaps->CaptureSizeMask[CAPTURE_SIZE_400W] |= (1 <<CAPTURE_SIZE_QVGA);
			pCaps->CaptureSizeMask[CAPTURE_SIZE_400W] |= (1 <<CAPTURE_SIZE_VGA);
		
			pCaps->CaptureSizeMask[CAPTURE_SIZE_300W] |= (1 <<CAPTURE_SIZE_D1);
			pCaps->CaptureSizeMask[CAPTURE_SIZE_300W] |= (1 <<CAPTURE_SIZE_QVGA);
			pCaps->CaptureSizeMask[CAPTURE_SIZE_300W] |= (1 <<CAPTURE_SIZE_VGA);

			pCaps->CaptureSizeMask[CAPTURE_SIZE_1080P] |= (1 <<CAPTURE_SIZE_D1);
			pCaps->CaptureSizeMask[CAPTURE_SIZE_1080P] |= (1 <<CAPTURE_SIZE_QVGA);
			pCaps->CaptureSizeMask[CAPTURE_SIZE_1080P] |= (1 <<CAPTURE_SIZE_VGA);
	}
	else if(NULL != strstr(ipc.sensor_buf, "ov5658")){
		pCaps->CaptureSizeMask[CAPTURE_SIZE_500W] |= (1 <<CAPTURE_SIZE_D1);
		pCaps->CaptureSizeMask[CAPTURE_SIZE_500W] |= (1 <<CAPTURE_SIZE_QVGA);
		pCaps->CaptureSizeMask[CAPTURE_SIZE_500W] |= (1 <<CAPTURE_SIZE_VGA);
	
		pCaps->CaptureSizeMask[CAPTURE_SIZE_300W] |= (1 <<CAPTURE_SIZE_D1);
		pCaps->CaptureSizeMask[CAPTURE_SIZE_300W] |= (1 <<CAPTURE_SIZE_QVGA);
		pCaps->CaptureSizeMask[CAPTURE_SIZE_300W] |= (1 <<CAPTURE_SIZE_VGA);
		
		pCaps->CaptureSizeMask[CAPTURE_SIZE_1080P] |= (1 <<CAPTURE_SIZE_D1);
		pCaps->CaptureSizeMask[CAPTURE_SIZE_1080P] |= (1 <<CAPTURE_SIZE_QVGA);
		pCaps->CaptureSizeMask[CAPTURE_SIZE_1080P] |= (1 <<CAPTURE_SIZE_VGA);
	}
	pCaps->CaptureSizeMask[CAPTURE_SIZE_1280_960] |= (1 <<CAPTURE_SIZE_D1);
	pCaps->CaptureSizeMask[CAPTURE_SIZE_1280_960] |= (1 <<CAPTURE_SIZE_QVGA);
	pCaps->CaptureSizeMask[CAPTURE_SIZE_1280_960] |= (1 <<CAPTURE_SIZE_VGA);
	
	pCaps->CaptureSizeMask[CAPTURE_SIZE_720P] |= (1 <<CAPTURE_SIZE_D1);
	pCaps->CaptureSizeMask[CAPTURE_SIZE_720P] |= (1 <<CAPTURE_SIZE_VGA);
	pCaps->CaptureSizeMask[CAPTURE_SIZE_720P] |= (1 <<CAPTURE_SIZE_QVGA);

	return 0;
}
//!ȡͬϵͳ֧�ֵ��������ͣ��Լ���������֧�ֵķֱ���
int CaptureGetStreamImageCaps(CAPTURE_STREAM_IMAGE* pCaps )
{
	int j;
	if(pCaps == NULL)
	{
		return -1;
	}
	memset(pCaps, 0, sizeof(CAPTURE_STREAM_IMAGE));
	pCaps->StreamCaps = (1 << CHL_MAIN_T)| (1 << CHL_2END_T) | (1 << CHL_3IRD_T)| (1 << CHL_4RTH_T) | (1 << CHL_JPEG_T );
	for( j= 0; j < CHL_FUNCTION_NUM; j++)
	{
		pCaps->CaptureSizeMask[CAPTURE_SIZE_QVGA] |= BITMSK(j);
		pCaps->CaptureSizeMask[CAPTURE_SIZE_VGA] |= BITMSK(j);
		pCaps->CaptureSizeMask[CAPTURE_SIZE_D1] |= BITMSK(j);
		//pCaps->CaptureSizeMask[CAPTURE_SIZE_720P] |= BITMSK(j);
		//pCaps->CaptureSizeMask[CAPTURE_SIZE_1080P] |= BITMSK(j);
	}
	return 0;
}
/// ���������豸
/// 
/// \param [in] channel ͨ���š�
/// \retval 0 ����ʧ��
/// \retval 0 �����ɹ�
static void sigstop(int signo)
{		
	aaa_stop();	
	goto_idle();
	if(iav_fd() > 0){
		close(iav_fd());
	}
	exit(1);
}

int CaptureCreate(int channel)
{
	signal(SIGINT, sigstop);
	signal(SIGQUIT, sigstop);
	signal(SIGTERM, sigstop);
	SetNetEnv();
	pthread_rwlock_init(&lock_stream, NULL);
	if(stop_encode((1<<MAX_ENCODE_STREAM_NUM) - 5) < 0){
		return -1;
	}
	if(goto_idle() < 0){
		return -1;
	}
	aaa_start();
	
	FILE *fp = fopen(SENSOR_CON_FILE, "r");
	if (NULL == fp){
		ipc.video_mode = AMBA_VIDEO_MODE_AUTO;
		ipc.max_size.width = 1280;
		ipc.max_size.height = 960;
	}else{
		fgets(ipc.sensor_buf, 30, fp);
		fclose(fp);
		if(NULL != strstr(ipc.sensor_buf, "ar0130")){
			ipc.video_mode = AMBA_VIDEO_MODE_1280_960;
			ipc.max_size.width = 1280;
			ipc.max_size.height = 960;
		}else if (NULL != strstr(ipc.sensor_buf, "ar0330")){
			ipc.video_mode = AMBA_VIDEO_MODE_2304_1296;
			ipc.max_size.width = 2304;
			ipc.max_size.height = 1296;
		}else if (NULL != strstr(ipc.sensor_buf, "ov4689")){
			ipc.video_mode = AMBA_VIDEO_MODE_2688_1520;
			ipc.max_size.width = 2688;
			ipc.max_size.height = 1520;
		}else if (NULL != strstr(ipc.sensor_buf, "ov5658")){
			ipc.video_mode = AMBA_VIDEO_MODE_QSXGA;
			ipc.max_size.width = 2592;
			ipc.max_size.height = 1944;
		}else{
			ipc.video_mode = AMBA_VIDEO_MODE_AUTO;
			ipc.max_size.width = 1280;
			ipc.max_size.height = 960;
		}
	}
#if 1
//CVBS
	if(init_vout(VOUT_1, AMBA_VOUT_SINK_TYPE_CVBS, AMBA_VIDEO_MODE_576I) < 0){
		return -1;
	}
	iav_vout_enable_video_t enable_video;
	enable_video.vout_id = 1;
	enable_video.video_en = 1;
	AM_IOCTL(iav_fd(), IAV_IOC_VOUT_ENABLE_VIDEO, &enable_video);
#endif
	if(init_vin(ipc.video_mode) < 0){
		return -1;
	}

	struct iav_system_resource resource;
	memset(&resource, 0, sizeof(resource));
	resource.encode_mode = DSP_ADVANCED_ISO_MODE;
	AM_IOCTL(iav_fd(), IAV_IOC_GET_SYSTEM_RESOURCE, &resource);

	resource.mctf_pm_enable = 0;
	resource.buf_max_size[0].width = ipc.max_size.width;
	resource.buf_max_size[0].height = ipc.max_size.height;
	resource.buf_max_size[1].width = 720;
	resource.buf_max_size[1].height = 576;
	resource.buf_max_size[3].width = 320;
	resource.buf_max_size[3].height = 240;
	resource.stream_max_size[0].width = ipc.max_size.width;
	resource.stream_max_size[0].height = ipc.max_size.height;
	resource.stream_max_size[1].width = 720;
	resource.stream_max_size[1].height = 576;
	resource.stream_max_size[3].width = 320;
	resource.stream_max_size[3].height = 240;
	AM_IOCTL(iav_fd(), IAV_IOC_SET_SYSTEM_RESOURCE, &resource);

/*buffer***************************************/
	struct iav_srcbuf_setup buf_setup;
	memset(&buf_setup, 0, sizeof(buf_setup));
	AM_IOCTL(iav_fd(), IAV_IOC_GET_SOURCE_BUFFER_SETUP, &buf_setup);
	buf_setup.type[0] = IAV_SRCBUF_TYPE_ENCODE;
	buf_setup.size[0].width = ipc.max_size.width;
	buf_setup.size[0].height = ipc.max_size.height;
	buf_setup.input[0].width= ipc.max_size.width;
	buf_setup.input[0].height = ipc.max_size.height;
	buf_setup.type[1] = IAV_SRCBUF_TYPE_ENCODE;
	buf_setup.size[1].width = 720;
	buf_setup.size[1].height = 576;
	buf_setup.type[2] = IAV_SRCBUF_TYPE_PREVIEW;
	buf_setup.type[3] = IAV_SRCBUF_TYPE_ENCODE;
	buf_setup.size[3].width = 320;
	buf_setup.size[3].height = 240;
	AM_IOCTL(iav_fd(), IAV_IOC_SET_SOURCE_BUFFER_SETUP, &buf_setup);

/**********************************************/
	struct iav_stream_format format;
	memset(&format, 0, sizeof(format));
	format.id = 0;
	AM_IOCTL(iav_fd(), IAV_IOC_GET_STREAM_FORMAT, &format);
	format.type = IAV_STREAM_TYPE_H264;
	format.buf_id = 0;
	format.enc_win.width  = ipc.max_size.width;		
	format.enc_win.height = ipc.max_size.height;
	AM_IOCTL(iav_fd(), IAV_IOC_SET_STREAM_FORMAT, &format);
	
	memset(&format, 0, sizeof(format));
	format.id = 1;
	AM_IOCTL(iav_fd(), IAV_IOC_GET_STREAM_FORMAT, &format);
	format.type = IAV_STREAM_TYPE_H264;
	format.buf_id = 1;
	format.enc_win.width  = 720;		
	format.enc_win.height = 576;
	AM_IOCTL(iav_fd(), IAV_IOC_SET_STREAM_FORMAT, &format);	
	memset(&format, 0, sizeof(format));
	format.id = 3;
	AM_IOCTL(iav_fd(), IAV_IOC_GET_STREAM_FORMAT, &format);
	format.type = IAV_STREAM_TYPE_MJPEG;
	format.buf_id = 3;
	format.enc_win.width  = 320;		
	format.enc_win.height = 240;
	AM_IOCTL(iav_fd(), IAV_IOC_SET_STREAM_FORMAT, &format);
	struct iav_stream_cfg streamcfg;
	streamcfg.id = 3;
	streamcfg.cid = IAV_STMCFG_FPS;
	streamcfg.arg.fps.fps_multi = 1;
	streamcfg.arg.fps.fps_div = 25;//vin fps
	AM_IOCTL(iav_fd(), IAV_IOC_SET_STREAM_CONFIG, &streamcfg);
	
	if(enable_preview() < 0){
		return -1;
	}	
	if(start_encode((1<<MAX_ENCODE_STREAM_NUM) - 5) < 0){
		return -1;
	}

	//OSD show
	//��ʼ����������OSD
	OsdDrawInit();
	pthread_rwlock_init(&lock_overlay, NULL);//Add by SuperJoki 20151209
	// -A
	OsdDrawSetPostion(0, 0, 50, 0);
	OsdDrawSetPostion(0, 1, 0, 600);
	// -B
	OsdDrawSetPostion(1, 0, 50, 0);
	OsdDrawSetPostion(1, 1, 0, 600);
	// -D
	OsdDrawSetPostion(3, 0, 50, 0);
	OsdDrawSetPostion(3, 1, 0, 600);
	OsdDrawCheck();

	OsdDrawTimeStart(); 

	return 0;
}

/// ���ٲ����豸
/// 
/// \param [in] channel ͨ���š�
/// \retval 0 ����ʧ��
/// \retval 0 ���ٳɹ�
int CaptureDestroy(int channel)
{
	_printd("Test\n");
	return 0;
}

/// Ӧ�ó���ʱ���ã� ֪ͨ����ģ��������״̬�� ����д��� ����ģ�顣 
/// 
void CaptureSynchronize(void)
{
	_printd("Test\n");
}

/// ������Ƶ��ʽ��
/// 
/// \param [in]  channel ͨ���š�
/// \param [in]  dwStandard ��Ƶ��ʽ��ȡvideo_standard_t���͵�ֵ��
/// \retval 0  ���óɹ�
/// \retval 0  ����ʧ��
int CaptureSetVstd(int channel,uint dwStandard)
{
	_printd("Test\n");
	return 0;
}	


/// ���ò���ʱ��͸�ʽ��Ӧ�ó�����Ҫ��ʱ���ô˺����벶���ʱ�����ͬ����
/// 
/// \param [in] channel ͨ���š�
/// \param [in] pTime ָ��ϵͳʱ��ṹSYSTEM_TIME��ָ�롣
/// \param [in] datefmt ���ڸ�ʽ��ȡdate_fmtö��ֵ��
/// \param [in] datespc ���ڷָ����ȡdate_specö��ֵ��
/// \param [in] timefmt ʱ���ʽ��ȡtime_fmtö��ֵ��
/// \retval 0  ���óɹ���
/// \retval 0  ����ʧ�ܡ�
int CaptureSetTime(int channel, const SYSTEM_TIME * pTime, int datefmt,int datespc,int timefmt)
{
	_printd("set datefmt = %d datespc = %d timefmt = %d\n", datefmt, datespc, timefmt);
	OsdDrawSetTimeStly(datefmt, datespc, timefmt);
	CAPTURE_TITLE_PARAM CapTitleParam = {0};
	CapTitleParam.index = 0;
	CaptureSetTitle(-1, &CapTitleParam);
	CapTitleParam.index = 3;
	CaptureSetTitle(-1, &CapTitleParam);
	return 0;
}


/// ���ñ�����ӡ�
/// 
/// \param [in] channel ͨ���š�
/// \param [in] pParam ָ�������ӽṹCAPTURE_TITLE_PARAM��ָ�롣
/// \retval 0  ���óɹ���
/// \retval 0  ����ʧ�ܡ�
int CaptureSetTitle(int channel, CAPTURE_TITLE_PARAM *pParam)
{//return 0;
	_printd("channel == %d ---------- pParam->index == %d color=%x\n", channel, pParam->index, pParam->fg_color);
	int   x, y;
	static short LastTimeSet_x = 0, LastTimeSet_y = 0;
	static short LastTitelSet_x = 0, LastTitelSet_y = 0;
	
	x = pParam->x;
	y = pParam->y;

	if (-1 != channel)
	{
		printf("color[%x]R[%x]G[%x]B[%x]\n", pParam->fg_color,pParam->fg_color&0xff, (pParam->fg_color&0xff00)>>8, (pParam->fg_color&0xff0000)>>16);

		int area_id = pParam->index == 0? 0:1;
		OsdDrawSetColor(pParam->fg_color, area_id);
	}
	
	if (0 == pParam->index){
		if ((pthread_mutex_lock(&capture_set_title_lock)) != 0)
			return NULL;
		//ʱ��
		if (-1 == channel){
			x = LastTimeSet_x;
			y = LastTimeSet_y;
			pParam->enable = draw_time_flag;
		}//���ķֱ���ʱʹ��
		draw_time_flag = pParam->enable;
		OsdDrawSetPostion(0, 0, x, y);
		OsdDrawSetPostion(1, 0, x, y);
		OsdDrawSetPostion(3, 0, x, y);
		OsdDrawCheck();
		LastTimeSet_x = x;
		LastTimeSet_y = y;
		if ((pthread_mutex_unlock(&capture_set_title_lock)) != 0)
			return NULL;
	}else if (3 == pParam->index){
		if ((pthread_mutex_lock(&capture_set_title_lock)) != 0)
			return NULL;
		if (-1 == channel){
			x = LastTitelSet_x; 
			y = LastTitelSet_y;
			pParam->enable = draw_str_flag;
			printf("x==%d  y==%d\n", x, y);
		}//���ķֱ���ʱʹ��
		draw_str_flag = pParam->enable;
		OsdDrawSetContent(channel, pParam->RasStr);
		OsdDrawSetPostion(0, 1, x, y);
		OsdDrawSetPostion(1, 1, x, y);
		OsdDrawSetPostion(3, 1, x, y);
		OsdDrawCheck();

		LastTitelSet_x = x;
		LastTitelSet_y = y;
		if ((pthread_mutex_unlock(&capture_set_title_lock)) != 0)
			return NULL;
	}else{
		printf("I don't Kown Which OSD");
	}
	
	return 0;
}


/// ���ø�������
/// 
/// \param [in] channel ͨ���š�
/// \param [in] pParam ָ����Ƶ���ǽṹVIDEO_COVER_PARAM��ָ�롣
/// \retval 0  ���óɹ���
/// \retval 0  ����ʧ�ܡ�
int CaptureSetCover(int channel,int index, VIDEO_COVER_PARAM *pParam)
{
	_printd("channel======%d  index===========%d:  enable:%d    color:%d\n",channel, index, pParam->enable, pParam->color);
	int i = 0;

	switch (pParam->enable)
	{
		case VIDEO_COVER_WINDOW_CLEAR:///< ������ڸ�������
			PRIVACY_PARAM[index].enable = 0;
			
			extern int privacy_mask_set(VIDEO_COVER_PARAM *pParam);
			privacy_mask_set(&PRIVACY_PARAM[index]);
			PRIVACY_PARAM[index].rect.bottom = 0;
			PRIVACY_PARAM[index].rect.left = 0;
			PRIVACY_PARAM[index].rect.right = 0;
			PRIVACY_PARAM[index].rect.top = 0;
			
			for(i=0;i<PRIVACY_NUM;i++){
				if( PRIVACY_PARAM[i].enable)
					privacy_mask_set(&PRIVACY_PARAM[i]);
			}
			break;

		case VIDEO_COVER_WINDOW_DRAW:///< ���ƴ��ڸ�������
			if (0 == channel){
				PRIVACY_PARAM[index].enable = 0;
				privacy_mask_set(&PRIVACY_PARAM[index]);
				PRIVACY_PARAM[index] = *pParam;
			}
			for(i=0;i<PRIVACY_NUM;i++){
				if( PRIVACY_PARAM[i].enable)
					privacy_mask_set(&PRIVACY_PARAM[i]);
			}
			break;
		case VIDEO_COVER_FULLSCREEN_DRAW:///< ����ȫ����������
			for(i = 0; i < PRIVACY_NUM; i++){
				PRIVACY_PARAM[i].enable = 0;
				privacy_mask_set(&PRIVACY_PARAM[i]);
			}
			pParam->rect.bottom = 1080;
			pParam->rect.left   = 0;
			pParam->rect.right  = 1920;
			pParam->rect.top	= 0;
			pParam->enable = 1;
			privacy_mask_set(pParam);
			break;
		case VIDEO_COVER_FULLSCREEN_CLEAR:///< ���ȫ����������?
			for(i = 0; i < PRIVACY_NUM; i++){
				PRIVACY_PARAM[i].enable = 0;
				privacy_mask_set(&PRIVACY_PARAM[i]);
			}
			pParam->rect.bottom = 1080;
			pParam->rect.left   = 0;
			pParam->rect.right  = 1920;
			pParam->rect.top	= 0;
			pParam->enable = 0;
			privacy_mask_set(pParam);
			break;
	}
	return 0;
}


/// ���ò����������
/// 
/// \param [in] channel ͨ���š�
/// \param [in] dwLVolume ������������ȡֵ0-100��
/// \param [in] dwRVolume ������������ȡֵ0-100��
/// \retval 0  ���óɹ�
/// \retval 0  ����ʧ��
int CaptureSetVolume(int channel, uint dwLVolume, uint dwRVolume)
{
	_printd("Test\n");
	return 0;
}


/// ǿ�Ʊ���������I֡�� 
/// 
/// \param [in] channel ͨ���š�
/// \param [in] dwType
/// \retval 0  ���óɹ���
/// \retval 0  ����ʧ�ܡ�
int CaptureForceIFrame(int  channel, uint dwType)
{
	_printd("Test\n");
	return 0;
}

/// ���ò�������ѹ���ĸ�ʽ��
/// 
/// \param [in] channel ͨ���š�
/// \param [in] dwType
/// \param [in] pFormat ָ�򲶻��ʽ�ṹCAPTURE_FORMAT��ָ�롣
/// \retval 0  ���óɹ�
/// \retval 0  ����ʧ��
int CaptureSetFormat(int channel, uint dwType, CAPTURE_FORMAT * pFormat)
{
	_printd("CaptureSetFormat\n");	
	_printd("channel    = %d\n", channel);
	_printd("dwType     = %d\n", dwType);
	_printd("AVOption   = %d\n", pFormat->AVOption);
	_printd("BitRate    = %d\n", pFormat->BitRate);
	_printd("BitRateControl = %d\n", pFormat->BitRateControl);
	_printd("Compression    = %d\n", pFormat->Compression);
	_printd("FramesPerMinute= %d\n", pFormat->FramesPerMinute);
	_printd("FramesPerSecond= %d\n", pFormat->FramesPerSecond);
	_printd("Gop            = %d\n", pFormat->Gop);
	_printd("ImageQuality   = %d\n", pFormat->ImageQuality);
	_printd("ImageSize      = %d\n", pFormat->ImageSize);
	_printd("Reserved       = %d\n", pFormat->Reserved);
	
	if (dwType >= 2) {
		_printd("error dwType = %d\n", dwType);
		return 0;
	}

//ͨ��GOPֵ���ú���ƿ��ؿ���adc��ֵ������������
#if 0	
	if((pFormat->Gop >= 50) && (pFormat->Gop < 65)){
		if(dwType == 0){
			adc_open_led_num = pFormat->Gop-50;
		}else
			adc_close_led_num = pFormat->Gop-50;		
	}
#endif
	
	int high, width;	
	CapSize2Res(pFormat->ImageSize, &high, &width);
	if ((dwType == 0) && (NULL != strstr(ipc.sensor_buf, "ar0130"))){
		if(high == 1080)
		{
			high = 960;
			width = 1280;
		}
	}
	
	/*ov4689֡�ʲ��ܹ�����Ƶ������������*/
	if ((dwType == 0) && (NULL != strstr(ipc.sensor_buf, "ov4689"))){
		if (high == 1520 )
		{
			set_vin_frame_rate(AMBA_VIDEO_FPS_15);
		}else{
			set_vin_frame_rate(AMBA_VIDEO_FPS_25);
		}
	}else if((dwType == 0) && (NULL != strstr(ipc.sensor_buf, "ov5658"))){
		if (high == 1944 )
		{
			set_vin_frame_rate(AMBA_VIDEO_FPS_15);
		}else
			set_vin_frame_rate(AMBA_VIDEO_FPS_25);
	}	
	_printd("high=%d,width=%d\n",high,width);
	AmbaSetImage(dwType, width, high);	
	AmbaSetFps(dwType, pFormat->FramesPerSecond);	
	AmbaSetGop(dwType, pFormat->Gop);

	if (pFormat->BitRateControl == CAPTURE_BRC_CBR){
		_printd("Set BitRate = %d\n", pFormat->BitRate);
		if(pFormat->BitRate < 36){
			pFormat->BitRate = 36;
		}else if(pFormat->BitRate > 16000){
			pFormat->BitRate = 16000;
		}
		AmbaSetCbrValue(dwType, pFormat->BitRate * 1000);
	}else{
		_printd("Set VBR = %d\n", pFormat->ImageQuality);
		AmbaSetVbrLevel(dwType, pFormat->ImageQuality - 1);
	}
#if 1
	if(pFormat->AVOption & 0x01){
	}else{	
	}
	if(pFormat->AVOption & 0x02){
		venc_chn[dwType].enable = 1;
	}else{
		venc_chn[dwType].enable = 0;
	}
#endif

	return 0;
}

/// ��ȡ��������ѹ���ķֱ��ʡ�
/// 
/// \param [in] channel ͨ���š�
/// \param [in] dwType
/// \param [out] capture_size_t ָ�������ķֱ��ʡ�
/// \retval 0  ��óɹ�
/// \retval 0  ���ʧ��
int CaptureGetImageSize(int channel, uint dwType, enum capture_size_t *size)
{
	_printd("Test\n");
	return 0;
}


/// ��ʼ���롣����Ƶ��ʽ������ѡ�ͼ��������֡�ʡ��������Ʒ�ʽ���ֱ��ʡ�����
/// ģʽ�Ȳ������ı�ʱ��Ӧ�ó����ֹͣ����Ȼ�����¿�ʼ����
/// 
/// \param [in] channel ͨ���š�
/// \param [in] dwType ������ʾ���������ѡ���capture_start_t���͵�ֵ����ϡ�
/// \retval 0  ��ʼ�ɹ�
/// \retval 0  ��ʼʧ��
int CaptureStart(int  channel, uint dwType)
{
	_printd("********************CaptureStart*************************\n");
	capture_pthread(1);
	return 0;
}


/// ֹͣ���룬��ǰ�����������ء�
/// 
/// \param [in]  channel ͨ���š�
/// \param [in]  dwType
/// \retval 0  ֹͣ�ɹ�
/// \retval 0  ֹͣʧ��
int CaptureStop(int  channel, uint dwType)
{
	_printd("********************CaptureStop**************************\n");
	//capture_pthread(0);
	return 0;
}

/// ��ʼ��ץ�ģ���ǰ�����������ء�
/// 
/// \param [in]  channel ͨ���š�
/// \retval 0  ��ʼ���ɹ�
/// \retval >0  ��ʼ��ʧ��
//int CaptureCreateSnap(int channel);

//��ʼ��ʧ�ܺ���ô˺�����������
//Ret����Ϊ��ʼ��ʧ�ܵķ���ֵ
//Ҳ��������ֹͣץ�Ĵ�ʱRet Ϊ��
//
//int CaptureDestroySnap(int channel,int Ret);

/// ��ȡ�������ݣ���Ϊ������ʱģʽ���ڴ�����豸�������䣬�豸�õ��ʵ�������
/// �󷵻ء���ʱʱ�������أ����ڴ�����Ѿ�������ݣ�������ʵ�ʵ����ݳ��Ȳ����أ�
/// ���ڴ����û�����ݣ����ش��󣻽���ʱʱ�����õĽ�С������������ݵ�ʵʱ�ԡ�
/// 
/// \param [in] pchannel ͨ���ţ�-1��ʾ������ͨ�������ݣ�������ͨ������Ϊ����
///        ����ֵ���ء�
/// \param [in] pdwType
/// \param [out] phMemory �ڴ����ָ�룬��������ȡ�����ڴ�������ڴ���ʵ�����ݳ�
///        �ȱ�ʾ���񵽵����ݳ��ȡ�
/// \return>=0 ͨ���ţ�������ͨ����Ϊ-1ʱ��
/// \retval 0  ���ɹ�
/// \retval 0  ��ʧ��
int CaptureGetBuffer(int *pchannel, uint *pdwType, VD_HANDLE *phMemory)
{
	int i;
	int s32Ret;

	if(pchannel == NULL || pdwType == NULL || phMemory == NULL)
	{
		ERR_PRINT("\n");
		return -1;
	}

	while(1)
	{
		for( i = CHL_MAIN_T; i < CHL_FUNCTION_NUM; i++ )
		{
			s32Ret = GetCapturVideo(pchannel, i, phMemory);
			if( 0 == s32Ret )
			{
				//_printd("CaptureGetBuffer-i:%d\n", i);
				*pdwType = i;
				return 0;
			}
		}
		usleep(10*1000);
	}
	
	return 0;
}


/// ����ץͼ����
///
//int CaptureSetSnapParam(int channel, char imgq, int second,  int millisecond);


/// ��ȡͼƬ����
///
int CaptureGetSnapBuffer(int channel, VD_HANDLE *phMemory, uint dwTimeout)
{
	_printd("Test\n");
	return 0;
}



/// ��ȡͼ����ɫ
///
int CaptureGetColor(int channel, VIDEO_COLOR * pColor)
{
	_printd("Test\n");
	return 0;
}


/// ��ȡ����������
/// 
/// \param [in] channel ͨ����
/// \param [out] pVolume ������ָ�룬��ʵ������ֵ����Χ�ǣ���65K
/// \retval 0  ��ȡ�ɹ�
/// \retval 0  ��ȡʧ��
int CaptureGetVolume(int channel, int *pVolume)
{
	_printd("Test\n");
	return 0;
}


/// ���ò��������ԣ��ڲ�������У��˽ӿ�����ʱ���ò���Ч��δ����ʱ�����豸Ӧ��
/// �������ȣ�Ȼ��֤�����ʱ<=120ms��
/// \param channel ͨ���ţ�ͬһ��DSP�Ĳ�ͬͨ������ֿܷ�����
/// \param sizeOfPacket ���ݰ���С��ȡֵ1K-16K
/// \param maxFramesInPacket �������֡����ȡֵ1-8
/// \param maxMilliSecondOfPacket ������������������ȡֵ0-4000
/// \retval 0 ���óɹ�
/// \retval <0 ����ʧ��
int CaptureSetPacketPolicy(int channel, int sizeOfPacket, int maxFramesInPacket,
						   int maxMilliSecondOfPacket)
{
	_printd("Test\n");
	return 0;
}
/// ���ò������ݻ���ָʽ�����л���
/// 
/// \param [in] bigChannel ͨ���ţ�0Ϊ�±�
/// \param [in] dwChannels��С��������룬����ʾ�ָ��ɵ�����
/// \param [in] dwType ������ʾ���������ѡ���capture_start_t���͵�ֵ����ϡ�
/// \retval 0  ���óɹ�
/// \retval 0  ����ʧ��
int CaptureSetSplit(int bigChannel, uint dwChannels, uint dwType)
{
	_printd("Test\n");
	return 0;
}
/// ����CIF����D1�ֱ��ʻ���,����0�ɹ�
int ViSetCIF(int channel)
{
	_printd("Test\n");
	return 0;
}
int ViSetD1(int channel)
{
	_printd("Test\n");
	return 0;
}

/*
EventType,�¼�����,1:�ط�,2,����
enable,״̬,1:�¼���ʼ,2:�¼�����
channel,ͨ����,��λ
*/
int UpdateVBRValue(int EventType,int enable ,int channel,CAPTURE_FORMAT format[]) 
{
	_printd("Test\n");
	return 0;
}

//����OSD�ķŴ�����
///OSDZoom��ʾ�������
void InitOsdSize(int channel)
{
	_printd("Test\n");
}

//add by SuperJoki
int SetOSDSize(CAPTURE_OSD_ZOOM * OSDZoom, int channel)
{
	//return 0;
	//_printd("Test\n");
	 _printd("OSDZoom->u.size ===%d\n", OSDZoom->u.size);
	
	if(!OSDZoom || channel < 0 || channel >= DVR_CHN_MAX)
	{
		_printd("SetOSDSize error, OSDZoom[%p], channel[%d]\n",  OSDZoom, channel);
		return -1;
	}
	
	switch(OSDZoom->u.size)
	{
		case 1:
			draw_font_size = 48;//SuperJoki add
			break;
		case 2:
			draw_font_size = 56;
			break;
		default:
			printf("unknow option found: %c\n", OSDZoom->u.size);
			return -1;
	}
	
	return 0;
}
int GetOSDSize(CAPTURE_OSD_ZOOM* OSDZoom, int channel)
{
	_printd("Test\n");
	return 0;
}
int Osd_SetFontBgColor(OSD_FONT_BG_COLOR *rgbColor)
{
	_printd("Test\n");
	return 0;
}
int Osd_GetFontBgColor(OSD_FONT_BG_COLOR *rgbColor)
{
	_printd("Test\n");
	return 0;
}
int OSD_LoadBmp(const char *filename, char** data, unsigned short * width, unsigned short * height)
{
	_printd("Test\n");
	return 0;
}
int OSD_ReleaseBmp(char* data)
{
	_printd("Test\n");
	return 0;
}

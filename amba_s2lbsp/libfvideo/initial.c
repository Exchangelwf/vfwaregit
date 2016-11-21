
#include "ipc_inc.h"
//#include "Capture.h"
#include "CaptureList.h"
#include "Video.h"
#include "Color2Grey.h"

pthread_rwlock_t lock_stream;
IPC_CONF ipc={0};
int iav_fd(void)
{
	static int fd_iav=-1;
	if(fd_iav < 0){
		if((fd_iav = open("/dev/iav", O_RDWR, 0)) < 0){
			perror("/dev/iav");
		}
	}
	return fd_iav;
}

int get_vin0_idsp_interrupts_counts(u32 *value)
{
	char cmd[128] = "grep vin0_idsp /proc/interrupts   | grep -v vin0_idsp_sof | grep -v vin0_idsp_last_pixel | awk '{print $2}'";
	char buffer[32];
	FILE *fp;
	fp = popen(&cmd, "r");
	fgets(buffer, sizeof(buffer), fp);
	*value = atoi(buffer);
	pclose(fp);

return *value;
}

int force_stop_encode(u32 streamid)
{
	AM_IOCTL(iav_fd(), IAV_IOC_STOP_ENCODE, streamid);
	_printd("IAV_IOC_STOP_ENCODE 0x%x down\n", streamid);
	return 0;
}

int stop_encode(u32 streamid)
{
	struct iav_stream_info streaminfo;
	int i;
	for(i = 0; i < MAX_ENCODE_STREAM_NUM; ++i){
		if(streamid & (1 << i)){
			_printd("%d*****************************\n", i);
			streaminfo.id = i;
			AM_IOCTL(iav_fd(), IAV_IOC_GET_STREAM_INFO, &streaminfo);
			if(streaminfo.state != IAV_STREAM_STATE_ENCODING){
				streamid &= ~(1 << i);
			}
			_printd("%d*****************************\n", i);
		}
	}
	if(streamid == 0)	return 0;
	return force_stop_encode(streamid);
}

int goto_idle(void)
{
	_printd("IAVIOC_S_IDLE start\n");
	int ret = 1;
	while(ret != 0){
		ret = ioctl(iav_fd(), IAV_IOC_ENTER_IDLE, 0);
	}
	_printd("IAVIOC_S_IDLE done\n");
	return 0;
}

#define DIV_ROUND(divident, divider)    ( ( (divident)+((divider)>>1)) / (divider) )

int set_vin_frame_rate(int vin_framerate)
{
	struct vindev_fps vsrc_fps;
	vsrc_fps.vsrc_id = 0;
	vsrc_fps.fps = vin_framerate;
	if (ioctl(iav_fd(), IAV_IOC_VIN_SET_FPS, &vsrc_fps) < 0) {
		perror("IAV_IOC_VIN_SET_FPS");
		return -1;	
	}	
	return 0;
}
int get_vin_frame_rate()
{
	struct vindev_fps vsrc_fps;
	int vin_fps = 0;
	vsrc_fps.vsrc_id = 0;
	if (ioctl(iav_fd(), IAV_IOC_VIN_GET_FPS, &vsrc_fps) < 0) {
		perror("IAV_IOC_VIN_GET_FPS");
		return -1;	
	}
	vin_fps = 512000000 / vsrc_fps.fps;
	//printf("vin fps = %d\n", vin_fps);

	return vin_fps;
}


/*==============================================================================
  Get Sink ID from sink type
==============================================================================*/
int vout_get_sink_id(int chan, int sink_type)
{
	int					num;
	int					i;
	struct amba_vout_sink_info		sink_info;
	u32					sink_id = -1;

	num = 0;
	if (ioctl(iav_fd(), IAV_IOC_VOUT_GET_SINK_NUM, &num) < 0) {
		perror("IAV_IOC_VOUT_GET_SINK_NUM");
		return -1;
	}
	if (num < 1) {
		printf("Please load vout driver!\n");
		return -1;
	}

	for (i = num - 1; i >= 0; i--) {
		sink_info.id = i;
		if (ioctl(iav_fd(), IAV_IOC_VOUT_GET_SINK_INFO, &sink_info) < 0) {
			perror("IAV_IOC_VOUT_GET_SINK_INFO");
			return -1;
		}

		printf("sink %d is %s\n", sink_info.id, sink_info.name);

		if ((sink_info.sink_type == sink_type) &&
			(sink_info.source_id == chan))
			sink_id = sink_info.id;
	}

	printf("%s: %d %d, return %d\n", __func__, chan, sink_type, sink_id);

	return sink_id;
}

int get_hdmi_native_size(vout_size_t *psize)
{
	int					sink_id;
	struct amba_vout_sink_info		sink_info;

	sink_id = vout_get_sink_id(VOUT_1, AMBA_VOUT_SINK_TYPE_AUTO);
	if (sink_id < 0) {
		return -1;
	}

	sink_info.id = sink_id;
	if (ioctl(iav_fd(), IAV_IOC_VOUT_GET_SINK_INFO, &sink_info)) {
		return -1;
	}

	if (sink_info.hdmi_native_mode != AMBA_VIDEO_MODE_HDMI_NATIVE) {
		return -1;
	}

	psize->width	= sink_info.hdmi_native_width;
	psize->height	= sink_info.hdmi_native_height;

	return 0;
}

vout_res_t vout_res[] = {
	//Typically for Analog and HDMI
	{"480i",	AMBA_VIDEO_MODE_480I,			720, 480},
	{"576i",	AMBA_VIDEO_MODE_576I,			720, 576},
	{"480p",	AMBA_VIDEO_MODE_D1_NTSC,		720, 480},
	{"576p",	AMBA_VIDEO_MODE_D1_PAL,			720, 576},
	{"720p",	AMBA_VIDEO_MODE_720P,			1280, 720},
	{"720p50",	AMBA_VIDEO_MODE_720P50,		1280, 720},
	{"720p30",	AMBA_VIDEO_MODE_720P30,			1280, 720},
	{"720p25",	AMBA_VIDEO_MODE_720P25,			1280, 720},
	{"720p24",	AMBA_VIDEO_MODE_720P24,			1280, 720},
	{"1080i",	AMBA_VIDEO_MODE_1080I,			1920, 1080},
	{"1080i50",	AMBA_VIDEO_MODE_1080I50,		1920, 1080},
	{"1080p24",	AMBA_VIDEO_MODE_1080P24,		1920, 1080},
	{"1080p25",	AMBA_VIDEO_MODE_1080P25,		1920, 1080},
	{"1080p30",	AMBA_VIDEO_MODE_1080P30,		1920, 1080},
	{"1080p",	AMBA_VIDEO_MODE_1080P,			1920, 1080},
	{"1080p50",	AMBA_VIDEO_MODE_1080P50,		1920, 1080},
	{"native",	AMBA_VIDEO_MODE_HDMI_NATIVE,		0,    0},
	{"2160p30",	AMBA_VIDEO_MODE_2160P30,		3840, 2160},
	{"2160p25",	AMBA_VIDEO_MODE_2160P25,		3840, 2160},
	{"2160p24",	AMBA_VIDEO_MODE_2160P24,		3840, 2160},
	{"2160p24se",	AMBA_VIDEO_MODE_2160P24_SE,	4096, 2160},

	// Typically for LCD
	{"D480I",	AMBA_VIDEO_MODE_480I,			720, 480},
	{"D576I",	AMBA_VIDEO_MODE_576I,			720, 576},
	{"D480P",	AMBA_VIDEO_MODE_D1_NTSC,		720, 480},
	{"D576P",	AMBA_VIDEO_MODE_D1_PAL,			720, 576},
	{"D720P",	AMBA_VIDEO_MODE_720P,			1280, 720},
	{"D720P50",	AMBA_VIDEO_MODE_720P50,		1280, 720},
	{"D1080I",	AMBA_VIDEO_MODE_1080I,			1920, 1080},
	{"D1080I50",	AMBA_VIDEO_MODE_1080I50,		1920, 1080},
	{"D1080P24",	AMBA_VIDEO_MODE_1080P24,		1920, 1080},
	{"D1080P25",	AMBA_VIDEO_MODE_1080P25,		1920, 1080},
	{"D1080P30",	AMBA_VIDEO_MODE_1080P30,		1920, 1080},
	{"D1080P",	AMBA_VIDEO_MODE_1080P,			1920, 1080},
	{"D1080P50",	AMBA_VIDEO_MODE_1080P50,		1920, 1080},
	{"D960x240",	AMBA_VIDEO_MODE_960_240,		960, 240},	//AUO27
	{"D320x240",	AMBA_VIDEO_MODE_320_240,		320, 240},	//AUO27
	{"D320x288",	AMBA_VIDEO_MODE_320_288,		320, 288},	//AUO27
	{"D360x240",	AMBA_VIDEO_MODE_360_240,		360, 240},	//AUO27
	{"D360x288",	AMBA_VIDEO_MODE_360_288,		360, 288},	//AUO27
	{"D480x640",	AMBA_VIDEO_MODE_480_640,		480, 640},	//P28K
	{"D480x800",	AMBA_VIDEO_MODE_480_800,		480, 800},	//TPO648
	{"hvga",	AMBA_VIDEO_MODE_HVGA,			320,  480},	//TPO489
	{"vga",		AMBA_VIDEO_MODE_VGA,			640,  480},
	{"wvga",	AMBA_VIDEO_MODE_WVGA,			800,  480},	//TD043
	{"D240x400",	AMBA_VIDEO_MODE_240_400,		240,  400},	//WDF2440
	{"xga",		AMBA_VIDEO_MODE_XGA,		       1024,  768},	//EJ080NA
	{"wsvga",	AMBA_VIDEO_MODE_WSVGA,	       1024,  600},	//AT070TNA2
	{"D960x540",	AMBA_VIDEO_MODE_960_540,		960,  540},	//E330QHD
};

int get_vout_size(int mode, vout_size_t *psize)
{
	int i, got = 0;

	if (mode == AMBA_VIDEO_MODE_HDMI_NATIVE)
		return get_hdmi_native_size(psize);

	for (i = 0; i < sizeof(vout_res) / sizeof(vout_res[0]); i++) {
		if (vout_res[i].mode == mode) {
			psize->width = vout_res[i].width;
			psize->height = vout_res[i].height;
			got = 1;
			break;
		}
	}

	if (got)
		return 0;
	else
		return -1;
}

int init_vin(enum amba_video_mode video_mode)
{
	struct vindev_mode vsrc_mode;
	vsrc_mode.vsrc_id = 0;
	vsrc_mode.video_mode = (unsigned int)video_mode;
	vsrc_mode.hdr_mode = AMBA_VIDEO_LINEAR_MODE;
	vsrc_mode.bits = AMBA_VIDEO_BITS_AUTO;

	AM_IOCTL(iav_fd(), IAV_IOC_VIN_SET_MODE, &vsrc_mode);
	if ((NULL != strstr(ipc.sensor_buf, "ov4689")) || (NULL != strstr(ipc.sensor_buf, "ov5658"))){
		set_vin_frame_rate(AMBA_VIDEO_FPS_15);
		//struct vindev_mirror mirror_mode;
		pthread_rwlock_wrlock(&lock_stream);
		mirror_mode.vsrc_id = 0;
		mirror_mode.pattern = VINDEV_MIRROR_HORRIZONTALLY;
		mirror_mode.bayer_pattern = VINDEV_BAYER_PATTERN_AUTO;
		AM_IOCTL(iav_fd(), IAV_IOC_VIN_SET_MIRROR, &mirror_mode);
		pthread_rwlock_unlock(&lock_stream);
	}
	if (NULL != strstr(ipc.sensor_buf, "ar0130") || NULL != strstr(ipc.sensor_buf, "ar0330")){
		set_vin_frame_rate(AMBA_VIDEO_FPS_25);
	}
	
	_printd("init_vin done\n");
	return 0;
}

int init_vout(vout_id_t chan, enum amba_vout_sink_type sink_type, enum amba_video_mode mode)
{
	int num=0;
	int	i;
	u32 sink_id = -1;
	vout_size_t vout_size;
	struct amba_vout_sink_info		sink_info;
	struct amba_video_sink_mode sink_cfg;

	AM_IOCTL(iav_fd(), IAV_IOC_VOUT_GET_SINK_NUM, &num);
	if(num < 1){
		_printd("Please load vout driver!\n");
		return -1;
	}
	for(i = num - 1; i >= 0; i--){
		sink_info.id = i;
		AM_IOCTL(iav_fd(), IAV_IOC_VOUT_GET_SINK_INFO, &sink_info);
		_printd("sink %d is %s\n", sink_info.id, sink_info.name);
		if ((sink_info.sink_type == sink_type) && (sink_info.source_id == chan))
			sink_id = sink_info.id;
	}

	_printd("sink num=%d, id=%d\n", num, sink_id);
	AM_IOCTL(iav_fd(), IAV_IOC_VOUT_SELECT_DEV, sink_id);
	
	get_vout_size(mode, &vout_size);
	memset(&sink_cfg, 0, sizeof(sink_cfg));
	sink_cfg.id = sink_id;
	sink_cfg.mode = mode;
	sink_cfg.ratio = AMBA_VIDEO_RATIO_AUTO;
	sink_cfg.bits = AMBA_VIDEO_BITS_AUTO;
	sink_cfg.type = sink_type;
	sink_cfg.format = AMBA_VIDEO_FORMAT_INTERLACE;
	sink_cfg.frame_rate = AMBA_VIDEO_FPS_15;
	sink_cfg.csc_en = 1;
	sink_cfg.bg_color.y = 0x10;
	sink_cfg.bg_color.cb = 0x80;
	sink_cfg.bg_color.cr = 0x80;
	sink_cfg.display_input = AMBA_VOUT_INPUT_FROM_MIXER;
	sink_cfg.sink_type = AMBA_VOUT_SINK_TYPE_AUTO;
	sink_cfg.video_en = 1;
	sink_cfg.video_size.specified = 1;
	sink_cfg.video_size.vout_width = vout_size.width;
	sink_cfg.video_size.vout_height = vout_size.height;
	sink_cfg.video_size.video_width = vout_size.width;
	sink_cfg.video_size.video_height = vout_size.height;
	sink_cfg.video_flip = AMBA_VOUT_FLIP_NORMAL;
	sink_cfg.video_rotate = AMBA_VOUT_ROTATE_NORMAL;
	sink_cfg.fb_id = -2;
	sink_cfg.direct_to_dsp = 0;
	sink_cfg.lcd_cfg.mode = AMBA_VOUT_LCD_MODE_DISABLE;
	sink_cfg.hdmi_color_space = AMBA_VOUT_HDMI_CS_AUTO;
	sink_cfg.hdmi_3d_structure = DDD_RESERVED;
	sink_cfg.hdmi_overscan = AMBA_VOUT_HDMI_OVERSCAN_AUTO;
	AM_IOCTL(iav_fd(), IAV_IOC_VOUT_CONFIGURE_SINK, &sink_cfg);
	_printd("init VOUT_%d over.\n", chan);

	return 0;
}

int enable_preview(void)
{
	_printd("enable_preview start\n");
	AM_IOCTL(iav_fd(), IAV_IOC_ENABLE_PREVIEW, 31);
	_printd("enable_preview done\n");
	return 0;
}

int set_encode_format(void)
{
	struct iav_stream_format format;

	format.id = 0;
	AM_IOCTL(iav_fd(), IAV_IOC_GET_STREAM_FORMAT, &format);
	//AM_IOCTL(iav_fd(), IAV_IOC_SET_STREAM_FORMAT, &format);
	return 0;
}

int force_start_encode(u32 streamid)
{
	_printd("Start encoding for stream 0x%x\n", streamid);
	AM_IOCTL(iav_fd(), IAV_IOC_START_ENCODE, streamid);
	_printd("Start encoding for stream 0x%x successfully\n", streamid);
	return 0;
}

int start_encode(u32 streamid)
{
	struct iav_stream_info streaminfo;
	int i;

	for (i = 0; i < MAX_ENCODE_STREAM_NUM; i++) {
		if (streamid & (1 << i)) {
			_printd("%d*****************************\n", i);
			streaminfo.id = i;
			AM_IOCTL(iav_fd(), IAV_IOC_GET_STREAM_INFO, &streaminfo);
			if (streaminfo.state == IAV_STREAM_STATE_ENCODING) {
				streamid &= ~(1 << i);
				_printd("%d*****************************\n", i);
			}
		}
	}
	if (streamid == 0) {
		_printd("already in encoding, nothing to do\n");
		return 0;
	}

	return force_start_encode(streamid);
}

int stop_ipc(void)
{
	_printd("**************over****************\n");
	stop_encode((1<<MAX_ENCODE_STREAM_NUM) - 5);
	goto_idle();
	if(iav_fd() > 0){
		close(iav_fd());
	}
	_printd("**************over****************\n");
	
	return 0;
}

BLOCK_LIST *stream_list_handle(int channel)
{
	static BLOCK_LIST stream_list[CHL_FUNCTION_NUM];
	static int init_flag = 1;
	
	if(CHL_FUNCTION_NUM <= channel){
		return NULL;
	}
	if(init_flag == 1){
		int i = 0;
		for( i = 0; i < CHL_FUNCTION_NUM; i++ )
		{
			CapListCreate( &(stream_list[i]) );
		}
		init_flag = 0;
	}
	return stream_list+channel;
}

int FormStandHead(int vi_chn, uchar *buf, int framelen, int headtype, int width, int height)
{
	int head_len = 0;
	unsigned int frame_head[5];

	int pic_width  = width/8;
	int pic_height = height/8;
	int	frame_rate = 0;
	static int frame_rate_record[MAX_ENCODE_STREAM_NUM]={0};	
	
	if(headtype != PACK_TYPE_FRAME_AUDIO){
		//frame_rate = 30;
#if 1
		struct iav_stream_format  	encode_format;
		struct iav_stream_cfg streamcfg;
		memset(&encode_format, 0, sizeof(encode_format));
		encode_format.id = vi_chn;
		AM_IOCTL(iav_fd(), IAV_IOC_GET_STREAM_FORMAT, &encode_format);
		if (encode_format.type == IAV_STREAM_TYPE_H264) {
			streamcfg.id = vi_chn;
			streamcfg.cid = IAV_STMCFG_FPS;
			if(ioctl(iav_fd(), IAV_IOC_GET_STREAM_CONFIG, &streamcfg) >= 0){
				frame_rate 	= get_vin_frame_rate() * streamcfg.arg.fps.fps_multi  / streamcfg.arg.fps.fps_div;
				frame_rate_record[vi_chn] = frame_rate;
			}else{
				if(frame_rate_record[vi_chn] <= 0){
					return -1;
				}
				frame_rate = frame_rate_record[vi_chn];
			}
			//_printd("frame_rate = %d\n", frame_rate);
		}
#endif 
	}

	if (headtype == PACK_TYPE_FRAME_I)
	{	
		if((vi_chn == 0) && (ipc.max_size.width != width || ipc.max_size.height != height)){
			ipc.max_size.width = width;
			ipc.max_size.height = height;
		}
		time_t seconds;
		struct tm *ltm;
		unsigned int xrwh, datetime;

		seconds = time(NULL);
		ltm = localtime(&seconds);
		if(ltm->tm_year < 100)
		{
			ltm->tm_year = 0;
		}
		else
		{
			ltm->tm_year -= 100;
		}
 		datetime = (ltm->tm_sec) | (ltm->tm_min  << 6) | (ltm->tm_hour << 12) | (ltm->tm_mday << 17)
				| ((ltm->tm_mon + 1)  << 22) | ((ltm->tm_year) << 26);
		if(0 && pic_width <= (1920/8) && ipc.max_size.width <= 1920){
			xrwh = 2 | (frame_rate << 8) | (pic_width << 16) | (pic_height << 24);
			
			frame_head[0] = 0xFD010000;
			frame_head[1] = xrwh;
			frame_head[2] = datetime;
			frame_head[3] = framelen;
			memcpy(buf, frame_head, 4*sizeof(unsigned int));
			head_len = 16;
		}else{
			xrwh = 2 | (frame_rate << 8);
			
			frame_head[0] = 0xAD010000;
			frame_head[1] = xrwh;
			frame_head[2] = pic_width | (pic_height << 16);
			frame_head[3] = datetime;
			frame_head[4] = framelen;
			memcpy(buf, frame_head, 5*sizeof(unsigned int));
			head_len = 20;
		}
	}
	if (headtype == PACK_TYPE_FRAME_MOTION)
	{	
		time_t seconds;
		struct tm *ltm;
		unsigned int xrwh, datetime;

		seconds = time(NULL);
		ltm = localtime(&seconds);
		if(ltm->tm_year < 100)
		{
			ltm->tm_year = 0;
		}
		else
		{
			ltm->tm_year -= 100;
		}
		datetime = (ltm->tm_sec)
				| (ltm->tm_min  << 6)
				| (ltm->tm_hour << 12)
				| (ltm->tm_mday << 17)
				| ((ltm->tm_mon + 1)  << 22)
				| ((ltm->tm_year) << 26);

		assert( frame_rate != 0 );

		if(0 && pic_width <= (1920/8) && ipc.max_size.width <= 1920){
			xrwh = 2 | (frame_rate << 8) | (pic_width << 16) | (pic_height << 24);
			
			frame_head[0] = 0xFE010000;
			frame_head[1] = xrwh;
			frame_head[2] = datetime;
			frame_head[3] = framelen;
			memcpy(buf, frame_head, 4*sizeof(unsigned int));
			head_len = 16;
		}else{
			xrwh = 2 | (frame_rate << 8);
			
			frame_head[0] = 0xAE010000;
			frame_head[1] = xrwh;
			frame_head[2] = pic_width | (pic_height << 16);
			frame_head[3] = datetime;
			frame_head[4] = framelen;
			memcpy(buf, frame_head, 5*sizeof(unsigned int));
			head_len = 20;
		}
	}	
	else if(headtype == PACK_TYPE_FRAME_P)
	{	
		if(0 && pic_width <= (1920/8) && ipc.max_size.width <= 1920){
			frame_head[0] = 0xFC010000;
		}else{
			frame_head[0] = 0xAC010000;
		}
		frame_head[1] = framelen;
		memcpy(buf, frame_head, 2*sizeof(unsigned int));
		head_len = 8;
	}
	else if(headtype == PACK_TYPE_FRAME_AUDIO) //!音频头
	{
		if(0 && ipc.max_size.width <= 1920){
			frame_head[0] = 0xF0010000;
		}else{
			frame_head[0] = 0xA0010000;
		}
		frame_head[1] = 14 | (2 << 8) | (framelen << 16);
		memcpy(buf, frame_head, 2*sizeof(unsigned int));
		head_len = 8;		
	}
	else if(headtype == PACK_TYPE_FRAME_JEPG) /*抓拍头*/
	{
		time_t seconds;
		struct tm *ltm;
		unsigned int xwh, datetime;
		
		seconds = time(NULL);
		ltm = localtime(&seconds);
		
		if(ltm->tm_year < 100)
		{
			ltm->tm_year = 0;
		}
		else
		{
			ltm->tm_year -= 100;
		}
		datetime = (ltm->tm_sec)
				| (ltm->tm_min  << 6)
				| (ltm->tm_hour << 12)
				| (ltm->tm_mday << 17)
				| ((ltm->tm_mon + 1)  << 22)
				| ((ltm->tm_year) << 26);

		if(0 && pic_width <= (1920/8) && ipc.max_size.width <= 1920){
			xwh = 3 | (pic_width << 16) | (pic_height<< 24);
			frame_head[0] = 0xFA010000;
			frame_head[1] = xwh;
			frame_head[2] = datetime;
			frame_head[3] = framelen;
			memcpy(buf, frame_head, 4*sizeof(unsigned int));
			head_len = 16;
		}else{
			xwh = 3;
 			frame_head[0] = 0xAA010000;
			frame_head[1] = xwh;
			frame_head[2] = pic_width | (pic_height << 16);
			frame_head[3] = datetime;
			frame_head[4] = framelen;
			memcpy(buf, frame_head, 5*sizeof(unsigned int));
			head_len = 20;
		}
		
	}
	return head_len;
}

void *RestartVin(void *args)
{
 	u32 cnt[10] = {0};
	int count = 0;
	u32 vin0_idsp_interrupt_value;
	u32 fps = 0;
	while(1)
	{
	//当中断不再增加时，复位sensor 重启VIN		
		get_vin0_idsp_interrupts_counts(&vin0_idsp_interrupt_value);
		cnt[count] = vin0_idsp_interrupt_value;
		usleep(100*1000);
		count++;
		if(count == 5){
			if(cnt[count-1] == cnt[0]){
				pthread_rwlock_wrlock(&lock_stream);
				struct vindev_mode vsrc_mode;
				vsrc_mode.vsrc_id = 0;
				struct iav_system_resource resource;
				memset(&resource, 0, sizeof(resource));
				resource.encode_mode = DSP_ADVANCED_ISO_MODE;
				fps = get_vin_frame_rate();
				ioctl(iav_fd(), IAV_IOC_VIN_GET_MODE, &vsrc_mode);			
				ioctl(iav_fd(), IAV_IOC_GET_SYSTEM_RESOURCE, &resource);
				goto_idle();
				ioctl(iav_fd(), IAV_IOC_VIN_SET_MODE, &vsrc_mode);
				ioctl(iav_fd(), IAV_IOC_VIN_SET_MIRROR, &mirror_mode);
				if(fps == 15){			
					set_vin_frame_rate(AMBA_VIDEO_FPS_15);
				}else
					set_vin_frame_rate(AMBA_VIDEO_FPS_25);
				ioctl(iav_fd(), IAV_IOC_SET_SYSTEM_RESOURCE, &resource); 
				enable_preview();			
				if(start_encode((1<<MAX_ENCODE_STREAM_NUM) - 5) < 0){
					pthread_rwlock_unlock(&lock_stream);
					return -1;
				}
				pthread_rwlock_unlock(&lock_stream);
			}			
			count = 0;
		}
	}

return NULL;
}

void *GetStreamThread(void *args)
{
	u8 *bsb_mem;
	u32 bsb_size;
	unsigned int whole_pic_size=0;
	struct iav_querybuf querybuf;

	querybuf.buf = IAV_BUFFER_BSB;
	if (ioctl(iav_fd(), IAV_IOC_QUERY_BUF, &querybuf) < 0) {
		perror("IAV_IOC_QUERY_BUF");
		return NULL;
	}
	bsb_size = querybuf.length;
	bsb_mem = mmap(NULL, bsb_size * 2, PROT_READ, MAP_SHARED, iav_fd(), querybuf.offset);
	if (bsb_mem == MAP_FAILED) {
		perror("mmap (%d) failed: %s\n");
		return NULL;
	}
	_printd("bsb_mem = 0x%x, size = 0x%x\n", (u32)bsb_mem, bsb_size);
	_printd("***************start get stream thread*******************\n");

	while(1){
		struct iav_querydesc query_desc;
		memset(&query_desc, 0, sizeof(query_desc));
		query_desc.qid = IAV_DESC_FRAME;
		query_desc.arg.frame.id = -1;

		pthread_rwlock_wrlock(&lock_stream);	
		if (ioctl(iav_fd(), IAV_IOC_QUERY_DESC, &query_desc) < 0) {
			pthread_rwlock_unlock(&lock_stream);
			perror("IAV_IOC_QUERY_DESC");
			usleep(100 * 1000);
			continue;
		}
		pthread_rwlock_unlock(&lock_stream);
		if(query_desc.arg.frame.stream_end == 1){
			usleep(1000);
			continue;
		}
		/*_printd("id=%d, num=%d, pic_type=%d, end=%d, %d*%d\n", query_desc.arg.frame.id, query_desc.arg.frame.frame_num,
			query_desc.arg.frame.pic_type, query_desc.arg.frame.stream_end, query_desc.arg.frame.reso.width, 
			query_desc.arg.frame.reso.height);*/

		u32 pic_size = query_desc.arg.frame.size;
		whole_pic_size	+= (pic_size & (~(1<<23)));
		if (pic_size>>23) {
			//end of frame
			pic_size = pic_size & (~(1<<23));
		 	 //padding some data to make whole picture to be 32 byte aligned
			pic_size += (((whole_pic_size + 31) & ~31)- whole_pic_size);
			//rewind whole pic size counter
			// printf("whole %d, pad %d \n", whole_pic_size, (((whole_pic_size + 31) & ~31)- whole_pic_size));
			whole_pic_size = 0;
		}
		if(query_desc.arg.frame.id < MAX_ENCODE_STREAM_NUM && query_desc.arg.frame.frame_num > 0){
			#if 1
			BLOCK_LIST *list = stream_list_handle(query_desc.arg.frame.id);
			struct CAPTURE_LIST *pbuf=NULL;
			STREAM_INFO StreamInfo;
			uchar HeadArray[32];
			int HeadLen;
			int iVal = 0;
			switch(query_desc.arg.frame.pic_type){
				case 1:
					StreamInfo.FrameType = PACK_TYPE_FRAME_I;
					
					break;
				case 3:
					StreamInfo.FrameType = PACK_TYPE_FRAME_P;
					break;
				case 0:
					StreamInfo.FrameType = PACK_TYPE_FRAME_JEPG;
					list = stream_list_handle(CHL_JPEG_T);
					break;
				default:
					_printd("unknown type %d\n", query_desc.arg.frame.pic_type);
					continue;
					break;
			}
			HeadLen = FormStandHead(query_desc.arg.frame.id, HeadArray, pic_size, StreamInfo.FrameType, query_desc.arg.frame.reso.width, query_desc.arg.frame.reso.height);
			if(0 > HeadLen)	continue;
			StreamInfo.HeadTail = PACK_CONTAIN_FRAME_HEAD;
			StreamInfo.FrameLength = HeadLen+pic_size;
			StreamInfo.PacketLen = HeadLen+pic_size+1024;
			iVal = pthread_mutex_lock( &(list->lock) );
			assert(iVal == 0);					
			pbuf = CapListDetect(list, StreamInfo.PacketLen);
			if(pbuf == NULL)
			{
				ERR_PRINT("ERROR!!can not get memory stream_id=%d, frame_num=%d\r\n",query_desc.arg.frame.id,query_desc.arg.frame.frame_num);
				iVal = pthread_mutex_unlock( &(list->lock) );
				assert(iVal == 0);
				continue;
			}
			pbuf->start_length = pbuf->length;
			pbuf = StreamCopy(list, pbuf, HeadArray, HeadLen, &StreamInfo);
			if(pbuf == NULL)
			{
				ERR_PRINT("ERROR!!!StreamCopy stream_id=%d\r\n",query_desc.arg.frame.id);
				iVal = pthread_mutex_unlock( &(list->lock) );
				assert(iVal == 0);
				continue;
			}
			if(query_desc.arg.frame.data_addr_offset + pic_size < bsb_size){
				StreamInfo.HeadTail = PACK_CONTAIN_FRAME_TRAIL;
				pbuf = StreamCopy(list, pbuf, bsb_mem + query_desc.arg.frame.data_addr_offset, pic_size, &StreamInfo);
				if(pbuf == NULL)
				{
					ERR_PRINT("ERROR!!!StreamCopy stream_id=%d\r\n",query_desc.arg.frame.id);
					iVal = pthread_mutex_unlock( &(list->lock) );
					assert(iVal == 0);
					continue;
				}
			}else{
				u32 size = bsb_size - query_desc.arg.frame.data_addr_offset;
				u32 remain = pic_size - size;
				StreamInfo.HeadTail = PACK_CONTAIN_FRAME_NONHT;
				pbuf = StreamCopy(list, pbuf, bsb_mem + query_desc.arg.frame.data_addr_offset, size, &StreamInfo);
				if(pbuf == NULL)
				{
					ERR_PRINT("ERROR!!!StreamCopy stream_id=%d\r\n",query_desc.arg.frame.id);
					iVal = pthread_mutex_unlock( &(list->lock) );
					assert(iVal == 0);
					continue;
				}
				StreamInfo.HeadTail = PACK_CONTAIN_FRAME_TRAIL;
				pbuf = StreamCopy(list, pbuf, bsb_mem, remain, &StreamInfo);
				if(pbuf == NULL)
				{
					ERR_PRINT("ERROR!!!StreamCopy stream_id=%d\r\n",query_desc.arg.frame.id);
					iVal = pthread_mutex_unlock( &(list->lock) );
					assert(iVal == 0);
					continue;
				}
			}			
			CapList2Next(pbuf);
			iVal = pthread_mutex_unlock( &(list->lock) );
			assert(iVal == 0);
			#else
			FILE* fp = fopen("123.h264", "a+");
			if(query_desc.arg.frame.data_addr_offset + pic_size < bsb_size){
				fwrite(bsb_mem + query_desc.arg.frame.data_addr_offset, pic_size, 1, fp);
			}else{
				u32 size = bsb_size - query_desc.arg.frame.data_addr_offset;
				u32 remain = pic_size - size;
				fwrite(bsb_mem + query_desc.arg.frame.data_addr_offset, size, 1, fp);
				fwrite(bsb_mem , remain, 1, fp);
			}
			fclose(fp);
			static int flag=1;
			if(flag && 5 == query_desc.arg.frame.pic_type){
				FILE* fp = fopen("123.mjpeg", "w+");
				fwrite(bsb_mem + query_desc.arg.frame.data_addr_offset, pic_size, 1, fp);
				fclose(fp);
				flag = 0;
			}
			#endif
		}
	}

	return NULL;
}


//目前就让主码流有音频
int CaptureDev_SaveAencList(int size_audio, uchar *buff_audio, enum capture_channel_t dwType)
{
	int iVal = 0;
	BLOCK_LIST *list = stream_list_handle(dwType);
	int HeadType = PACK_TYPE_FRAME_AUDIO;
	struct CAPTURE_LIST *pbuf;
	
	iVal = pthread_mutex_lock( &(list->lock) );
	assert(iVal == 0);
	pbuf = CapListDetect(list, 8192);
	if(pbuf == NULL){
		iVal = pthread_mutex_unlock( &(list->lock) );
		assert(iVal == 0);
		return -1;
	}
	pbuf->start_length = pbuf->length;
	
	int DataLen, HeadLen;
	int FrameLen;
	STREAM_INFO StreamInfo;

	if (size_audio > 480){
		DataLen = 480;
	}else{
		DataLen = size_audio;
	}
	
	
	uchar HeadArray[8];
	HeadType = PACK_TYPE_FRAME_AUDIO;
	
	HeadLen = FormStandHead(0, HeadArray, DataLen, HeadType,0,0);
	FrameLen = HeadLen + DataLen;
	
	StreamInfo.FrameType = HeadType;
	StreamInfo.HeadTail = PACK_CONTAIN_FRAME_HEAD;
	StreamInfo.FrameLength = FrameLen;
	StreamInfo.RemainLen = FrameLen;
	StreamInfo.PacketLen = 8192;
	pbuf = StreamCopy(list, pbuf, HeadArray, HeadLen, &StreamInfo);
	if(pbuf == NULL)
	{
		iVal = pthread_mutex_unlock( &(list->lock) );
		assert(iVal == 0);
		return -1;
	}
	
	StreamInfo.HeadTail = PACK_CONTAIN_FRAME_TRAIL;
	pbuf = StreamCopy(list, pbuf, buff_audio, DataLen, &StreamInfo);
	if(pbuf == NULL)
	{
		iVal = pthread_mutex_unlock( &(list->lock) );
		assert(iVal == 0);
		return -1;
	}

	CapList2Next(pbuf);
	iVal = pthread_mutex_unlock( &(list->lock) );
	assert(iVal == 0);
	return 0;
}
	
int  capture_pthread(char flag)
{
	static pthread_t pid_capture = 0;
	static pthread_t pid_monochrome = 0;
	static pthread_t pid_sensor_reset = 0; 
	if(flag == 1 && pid_capture == 0 && pid_monochrome == 0){
		pthread_create(&pid_capture, NULL, GetStreamThread, NULL);
		pthread_create(&pid_monochrome, NULL, (void *)ColorGreyTask, NULL);
		pthread_create(&pid_sensor_reset, NULL, (void *)RestartVin, NULL);
		sleep(1);
	}else if(flag == 0 && pid_capture > 0 && pid_monochrome > 0){
		pthread_cancel(pid_capture);
		pthread_cancel(pid_monochrome);
		pthread_cancel(pid_sensor_reset);
		pid_capture = 0;
		pid_monochrome = 0;
		pid_sensor_reset = 0;
	}
	return 0;
}

int GetCapturVideo(int *pchannel, int stream, VD_HANDLE *phMemory)
{
	struct CAPTURE_LIST *list;
	if( pchannel == NULL || phMemory == NULL )
	{
		ERR_PRINT("\n");
		return -1;
	}
	BLOCK_LIST *stream_list = stream_list_handle(stream);

	int iVal = 0;
	iVal = pthread_mutex_lock(&(stream_list->lock));
	assert(iVal == 0);
	list = CapListGet(stream_list);
	if(list != NULL)
	{
		if(list->ready == 1)
		{
			memset(list->avenc_buf.header, 0xff, 104+4*8);
			int k;
			for(k = 0;k < list->count; k++)
			{
				memcpy(list->avenc_buf.header + sizeof(FRAMEINFO) * k
				,&(list->pack_head.FrameInfo[k]), sizeof(FRAMEINFO));
			}
			MemorySetLength(list->avenc_buf.handle, list->length);

			*pchannel = 0;			
			*phMemory = list->avenc_buf.handle;

			CapListFree(stream_list, list);
			iVal = pthread_mutex_unlock(&(stream_list->lock));
			assert(iVal == 0);

			return 0;
		}
	}
	iVal = pthread_mutex_unlock(&(stream_list->lock));
	assert(iVal == 0);

	return -1;
}

void SetNetEnv()
{
	char ipAddr[16] 	= {0};
	char Netmask[16] 	= {0};
	char gateway[16]	= {0};
	int len 		 	= 16;
	char dns1[24]		= {0};
	char dns2[24]		= {0};
	extern int NetGetHostIP(const char *, char *, const int, char *, const int);
	if (-1 == NetGetHostIP("eth0", ipAddr, len, Netmask, len)){
		sprintf (ipAddr,"192.168.1.123");
		sprintf (Netmask, "255.255.255.0");
	}
	extern int NetGetGateway(const char *, char *, const int);
	if ( -1 == NetGetGateway("eth0", gateway, len)){
		sprintf (gateway, "192.168.1.1");
	}
	extern int NetGetDNSAddress(char *,const int,char *, const int);
	if (-1 == NetGetDNSAddress(dns1, sizeof(dns1), dns2, sizeof(dns2))){
		sprintf (dns1, "8.8.8.8");
		sprintf (dns2, "8.8.8.8");
	}
	extern int NetSetHostIP(const char *, const char *, const char *);
	NetSetHostIP("eth0", ipAddr, Netmask);
	extern int NetSetGateway(const char *, const char *);
	NetSetGateway("eth0", gateway);
	extern int NetSetDNSAddress(const char *, const char *);
	NetSetDNSAddress(dns1, dns2);
	return;
}

int AmbaSetImage(int iStream, int CapImage_x, int CapImage_y)
{
	struct iav_stream_format format;
	CAPTURE_TITLE_PARAM CapTitleParam = {0};
	format.id = iStream;
	AM_IOCTL(iav_fd(), IAV_IOC_GET_STREAM_FORMAT, &format);
	_printd("\nset stream[%d]: %dx%d to %dx%d, x=%d, y=%d\n", iStream, format.enc_win.width, format.enc_win.height, 
		CapImage_x, CapImage_y, format.enc_win.x, format.enc_win.y);
	if((format.enc_win.width != CapImage_x) || (format.enc_win.height != CapImage_y)){
		format.enc_win.width = CapImage_x;
		format.enc_win.height = CapImage_y;
		
		if(iStream == 0){	
			struct iav_srcbuf_setup buf_setup;
			memset(&buf_setup, 0, sizeof(buf_setup));		
			AM_IOCTL(iav_fd(), IAV_IOC_GET_SOURCE_BUFFER_SETUP, &buf_setup);
			buf_setup.size[0].width = CapImage_x;
			buf_setup.size[0].height = CapImage_y;
			memset(&buf_setup.input, 0, sizeof(buf_setup.input));
			pthread_rwlock_wrlock(&lock_stream);
			goto_idle();
			ioctl(iav_fd(), IAV_IOC_SET_SOURCE_BUFFER_SETUP, &buf_setup);
			ioctl(iav_fd(), IAV_IOC_SET_STREAM_FORMAT, &format);
			enable_preview();
			start_encode((1<<MAX_ENCODE_STREAM_NUM)-5);			
			pthread_rwlock_unlock(&lock_stream);
		}
		else{			
			struct iav_srcbuf_format buf_format;
			buf_format.buf_id = iStream;
			AM_IOCTL(iav_fd(), IAV_IOC_GET_SOURCE_BUFFER_FORMAT, &buf_format);
			buf_format.size.width= CapImage_x;
			buf_format.size.height = CapImage_y;
			pthread_rwlock_wrlock(&lock_stream);
			stop_encode(1<<iStream);
			AM_IOCTL(iav_fd(), IAV_IOC_SET_SOURCE_BUFFER_FORMAT, &buf_format);
			AM_IOCTL(iav_fd(), IAV_IOC_SET_STREAM_FORMAT, &format);
			start_encode(1<<iStream);
			pthread_rwlock_unlock(&lock_stream);
		}
		
		VIDEO_COVER_PARAM pParam = {{0,0,0,0},0,1};
		if (0 == iStream){
			CaptureSetCover(-1, 0, &pParam);
		}
		CapTitleParam.index = 0;
		CaptureSetTitle(-1, &CapTitleParam);
		CapTitleParam.index = 3;
		CaptureSetTitle(-1, &CapTitleParam);
	}

	return 0;

}

int AmbaSetFps(int iStream, int fps)
{
	struct iav_stream_cfg streamcfg;
	memset(&streamcfg, 0, sizeof(streamcfg));
	streamcfg.id = iStream;
	_printd("stream=%d\n",iStream);
	streamcfg.cid = IAV_STMCFG_FPS;
	int vin_fps=get_vin_frame_rate();
	if(fps > vin_fps){
		streamcfg.arg.fps.fps_multi = vin_fps;
	}else if(fps < 1){
		streamcfg.arg.fps.fps_multi = 1;
	}else{
		streamcfg.arg.fps.fps_multi = fps;
	}
	streamcfg.arg.fps.fps_div = vin_fps;
	AM_IOCTL(iav_fd(), IAV_IOC_SET_STREAM_CONFIG, &streamcfg);

	return 0;
}

int AmbaSetCbrValue(int iStream, unsigned int value)
{
	struct iav_stream_cfg cfg;
	struct iav_bitrate *bitrate;
	
	memset(&cfg, 0, sizeof(cfg));
	cfg.id = iStream;
	cfg.cid = IAV_H264_CFG_BITRATE;
	AM_IOCTL(iav_fd(), IAV_IOC_GET_STREAM_CONFIG, &cfg);
	bitrate = &cfg.arg.h264_rc;
	
	if((bitrate->vbr_setting != IAV_BRC_SCBR) || (bitrate->average_bitrate != value)){
		bitrate->vbr_setting = IAV_BRC_SCBR;
		bitrate->average_bitrate = value;
		#if 1
		bitrate->qp_max_on_I = 51;
		bitrate->qp_min_on_I = 1;
		bitrate->qp_max_on_P = 51;
		bitrate->qp_min_on_P = 1;
		bitrate->i_qp_reduce = 1;
		bitrate->p_qp_reduce = 1;
		bitrate->adapt_qp = 0;
		#else
		bitrate->qp_max_on_I = 51;
		bitrate->qp_min_on_I = 1;
		bitrate->qp_max_on_P = 51;
		bitrate->qp_min_on_P = 1;
		bitrate->i_qp_reduce = 6;
		bitrate->p_qp_reduce = 3;
		bitrate->adapt_qp = 2;
		#endif
		
		AM_IOCTL(iav_fd(), IAV_IOC_SET_STREAM_CONFIG, &cfg);
	}

	return 0;
}

#define BITRATE_NUM	11

static u32 rc_br_table[BITRATE_NUM] =
{
	256000,  512000,  768000,  1000000,
	1500000, 2000000, 3000000, 4000000,
	6000000, 8000000, 10000000,
};

#define FORMAT_NUM	13//添加分辨率修改相应的值下面的二维数组添加相应的数值

static u32 rc_reso_table[FORMAT_NUM] =
{
	2592*1944, 2048*1536, 1920*1080, 1280*1024,
	1280*960,  1280*720,  1024*768,  800*600,
	720*576,   720*480,   640*480,   352*288,   320*240,
};

static u32 rc_qp_for_vbr_lut[BITRATE_NUM][FORMAT_NUM] =
{
	{35, 33, 31, 29, 27, 27, 26, 23, 23, 22, 22, 17, 16},	/* 256 kbps */
	{34, 32, 30, 26, 25, 25, 24, 22, 21, 20, 20, 16, 15},	/* 512 kbps */
	{32, 30, 28, 25, 24, 24, 23, 21, 20, 19, 19, 15, 14},	/* 768 kbps */
	{31, 29, 27, 24, 23, 23, 22, 20, 19, 18, 18, 14, 13},	/* 1 Mbps */
	{30, 28, 26, 24, 22, 22, 21, 19, 18, 17, 17, 12, 11},	/* 1.5 Mbps */
	{29, 27, 25, 23, 22, 21, 19, 18, 17, 16, 16, 11, 10},	/* 2 Mbps */
	{28, 26, 24, 22, 21, 20, 19, 17, 16, 15, 15, 9,  8},	/* 3 Mbps */
	{27, 25, 23, 21, 20, 19, 18, 16, 15, 14, 14, 8,  7},	/* 4 Mbps */
	{26, 24, 22, 20, 19, 18, 17, 15, 14, 13, 12, 5,  1},	/* 6 Mbps */
	{25, 23, 21, 19, 18, 17, 16, 14, 13, 12, 11, 1,  1},	/* 8 Mbps */
	{24, 23, 21, 18, 17, 16, 15, 13, 12, 11, 10, 1,  1},	/* 10 Mbps */
};

int h264_calc_target_qp(u32 bitrate, u32 resolution)
{
	int i, j;

	for (i = 0; i < BITRATE_NUM; i++) {
		if (bitrate <= rc_br_table[i])
			break;
	}

	for (j = 0; j < FORMAT_NUM; j++) {
		if (resolution >= rc_reso_table[j])
			break;
	}

	if (i == BITRATE_NUM)
		i = BITRATE_NUM-1;
	if (j == FORMAT_NUM)
		j = 0;
	return rc_qp_for_vbr_lut[i][j];
}

int AmbaSetVbrLevel(int iStream, int level)
{
	struct iav_stream_cfg cfg;
	struct iav_bitrate *bitrate;
		
	static unsigned int qTab[6][2] = {{256000,512000}, {768000,1000000}, {1000000,2000000}, {2000000,4000000}, {4000000,6000000}, {8000000,10000000}};
	memset(&cfg, 0, sizeof(cfg));
	cfg.id = iStream;
	cfg.cid = IAV_H264_CFG_BITRATE;
	AM_IOCTL(iav_fd(), IAV_IOC_GET_STREAM_CONFIG, &cfg);
	bitrate = &cfg.arg.h264_rc;
	
	if (bitrate->average_bitrate != qTab[level][1]){
		struct iav_stream_format format;
		
		format.id = iStream;
		AM_IOCTL(iav_fd(), IAV_IOC_GET_STREAM_FORMAT, &format);
		u32 qp = h264_calc_target_qp(qTab[level][0], format.enc_win.width*format.enc_win.height);
		bitrate->vbr_setting = IAV_BRC_SCBR;
		bitrate->average_bitrate = qTab[level][1];
		bitrate->qp_max_on_I = qp;
		bitrate->qp_min_on_I = qp;
		bitrate->qp_max_on_P = qp;
		bitrate->qp_min_on_P = qp;
		AM_IOCTL(iav_fd(), IAV_IOC_SET_STREAM_CONFIG, &cfg);
	}
	
	return 0;
}

int AmbaSetGop(int iStream, int GopValue)
{
	_printd("set gop\n");
	if((GopValue <= 0) || (GopValue >= 255)){
		_printd("set gop value error : gop = %d\n", GopValue);
		return -1;
	}
	struct iav_stream_cfg cfg;
	struct iav_h264_gop *h264_gop;
	memset(&cfg, 0, sizeof(cfg));
	cfg.cid = IAV_H264_CFG_GOP;
	cfg.id =  iStream;
	AM_IOCTL(iav_fd(), IAV_IOC_GET_STREAM_CONFIG, &cfg);
	h264_gop = &cfg.arg.h264_gop;
	if(h264_gop->N == GopValue){
		return 0;
	}

	h264_gop->id = iStream;
	h264_gop->N = GopValue;
	AM_IOCTL(iav_fd(), IAV_IOC_SET_STREAM_CONFIG, &cfg);

	return 0;
}

int AmbaForceIFrame(uint channel)
{
	struct iav_stream_cfg stream_cfg;

	memset(&stream_cfg, 0, sizeof(stream_cfg));
	stream_cfg.id = channel;
	stream_cfg.cid = IAV_H264_CFG_FORCE_IDR;
	AM_IOCTL(iav_fd(), IAV_IOC_SET_STREAM_CONFIG, &stream_cfg);

	return 0;
}

#if 0
int main(int argc, char *argv[])
{
	if(stop_encode((1<<MAX_ENCODE_STREAM_NUM) - 1) < 0){
		return -1;
	}
	if(goto_idle() < 0){
		return -1;
	}
	if(init_vin(AMBA_VIDEO_MODE_AUTO) < 0){
		return -1;
	}
	
	struct iav_srcbuf_setup buf_setup;
	memset(&buf_setup, 0, sizeof(buf_setup));
	AM_IOCTL(iav_fd(), IAV_IOC_GET_SOURCE_BUFFER_SETUP, &buf_setup);
	buf_setup.type[2] = IAV_SRCBUF_TYPE_OFF;
	AM_IOCTL(iav_fd(), IAV_IOC_SET_SOURCE_BUFFER_SETUP, &buf_setup);
	
	struct iav_stream_format format;
	memset(&format, 0, sizeof(format));
	format.id = 0;
	AM_IOCTL(iav_fd(), IAV_IOC_GET_STREAM_FORMAT, &format);
	format.type = IAV_STREAM_TYPE_H264;
//****************************************
//随sensor不同而变化
	format.enc_win.width  = 1920;		
	format.enc_win.height = 1080;
//****************************************
	AM_IOCTL(iav_fd(), IAV_IOC_SET_STREAM_FORMAT, &format);
	
	if(enable_preview() < 0){
		return -1;
	}
	if(start_encode(1<<0) < 0){
		return -1;
	}
	pthread_t pid_capture;
	pthread_create(&pid_capture, NULL, GetStreamThread, NULL);
	#if 0
	sleep(5);
	_printd("*******1111\n");
	memset(&buf_setup, 0, sizeof(buf_setup));
	AM_IOCTL(iav_fd(), IAV_IOC_GET_SOURCE_BUFFER_SETUP, &buf_setup);
	buf_setup.size[0].width = 1280;
	buf_setup.size[0].height = 720;
	memset(&buf_setup.input, 0, sizeof(buf_setup.input));
	stop_encode((1<<MAX_ENCODE_STREAM_NUM)-1);
	goto_idle();
	AM_IOCTL(iav_fd(), IAV_IOC_SET_SOURCE_BUFFER_SETUP, &buf_setup);
	
	format.id = 0;
	AM_IOCTL(iav_fd(), IAV_IOC_GET_STREAM_FORMAT, &format);
	format.enc_win.width = 1280;
	format.enc_win.height = 720;
	AM_IOCTL(iav_fd(), IAV_IOC_SET_STREAM_FORMAT, &format);
	enable_preview();
	start_encode(1<<0);
	_printd("*******22\n");
	#endif
	
	getchar();
	stop_ipc();
	
	return 0;
}
#endif





#ifndef __IPC_INC_H__
#define __IPC_INC_H__

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <sched.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <assert.h>
#include <signal.h>
#include <pthread.h>
#include "basetypes.h"
#include "iav_ioctl.h"
#include "amba_debug.h"
#include "CaptureList.h"
#include "Capture.h"


typedef enum {
	IAV_CBR = 0,
	IAV_VBR,
	IAV_CBR_QUALITY_KEEPING,
	IAV_VBR_QUALITY_KEEPING,
	IAV_CBR2,
	IAV_VBR2,
} iav_rate_control_mode;

typedef enum {
	IAV_SHOW_SYSTEM_STATE = 0x00,
	IAV_SHOW_ENCODE_CFG = 0x01,
	IAV_SHOW_STREAM_INFO = 0x02,
	IAV_SHOW_BUFFER_INFO = 0x03,
	IAV_SHOW_RESOURCE_INFO = 0x04,
	IAV_SHOW_ENCMODE_CAP = 0x05,
	IAV_SHOW_DRIVER_INFO = 0x06,
	IAV_SHOW_CHIP_INFO = 0x07,
	IAV_SHOW_DRAM_LAYOUT = 0x08,
	IAV_SHOW_FEATURE_SET = 0x09,
	IAV_SHOW_CMD_EXAMPPLES = 0x0A,
	IAV_SHOW_INFO_TOTAL_NUM,
	IAV_SHOW_INFO_FIRST = 0,
	IAV_SHOW_INFO_LAST = IAV_SHOW_INFO_TOTAL_NUM,
} iav_show_info;

//*********************vout****************************************
typedef enum {
	VOUT_0 = 0,
	VOUT_1 = 1,
	VOUT_TBD = 2,
} vout_id_t;

typedef struct {
	const char 	*name;
	int		mode;
	int		width;
	int		height;
} vout_res_t;

typedef struct {
	int		width;
	int		height;
} vout_size_t;

//*********************vin**************************
struct vindev_mirror mirror_mode;
//****************************************************

#define G_PROJECT           0

#ifndef WIN32
#define _printd(fmt, ...)	printf ("\e[31m[%s][%s][%d]\e[0m"fmt"", __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#else
#define _printd(fmt, ...)	printf ("\e[31m[%s][%d]"fmt"\n\e[0m\n", (char *)(strrchr(__FILE__, '\\') + 1), __LINE__ - 2, ##__VA_ARGS__)
#endif

#ifndef AM_IOCTL
#define AM_IOCTL(_filp, _cmd, _arg)	\
		do { 						\
			if (ioctl(_filp, _cmd, _arg) < 0) {	\
				perror(#_cmd);		\
				return -1;			\
			}						\
		} while (0)
#endif

#define MAX_ENCODE_STREAM_NUM 4
#define SENSOR_CON_FILE "/mnt/custom/sensortype.ini"

typedef struct tagIPC_CONF{
	enum amba_video_mode video_mode;
	struct iav_window max_size;
	char sensor_buf[128];
}IPC_CONF;

IPC_CONF ipc;

//音视频使能结构体
typedef struct tagVENC_CHN_INFO
{
	int created; //!1-创建,0未创建，不可使用
	int enable;	//!指示使能
	int started;
} VENC_CHN_INFO_t;
VENC_CHN_INFO_t venc_chn[CHL_FUNCTION_NUM];


/// 获取iav驱动控制文件描述符
/// 
/// \param 无
/// \retval >=0	获取成功
/// \retval <0	获取失败
int iav_fd(void);

/// 获取iav驱动控制文件描述符
/// 
/// \param 无
/// \retval >=0	获取成功
/// \retval <0	获取失败
int force_stop_encode(u32 streamid);

int set_vin_frame_rate(int vin_framerate);
int get_vin_frame_rate();
int stop_encode(u32 streamid);
int goto_idle(void);
int init_vin(enum amba_video_mode video_mode);
int init_vout(vout_id_t chan, enum amba_vout_sink_type sink_type, enum amba_video_mode mode);
int enable_preview(void);
int set_encode_format(void);
int force_start_encode(u32 streamid);
int start_encode(u32 streamid);
int stop_ipc(void);

/// 执行视频流线程的启动与退出
/// 
/// \param [in] flag启动与退出标志(1:启动 2:退出)
/// \retval 0	执行成功
/// \retval !=0	执行失败
int capture_pthread(char flag);

int GetCapturVideo(int *pchannel, int stream, VD_HANDLE *phMemory);

void SetNetEnv();
//编码参数设置
int AmbaSetImage(int iStream, int CapImage_x, int CapImage_y);
int AmbaSetFps(int iStream, int fps);
int AmbaSetGop(int iStream, int GopValue);
int AmbaSetCbrValue(int iStream, unsigned int value);
int AmbaSetVbrLevel(int iStream, int level);
int AmbaForceIFrame(uint channel);

int CaptureDev_SaveAencList(int size_audio, uchar *buff_audio, enum capture_channel_t dwType);


#endif


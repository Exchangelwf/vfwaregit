#ifndef __WJALINK_API_H__
#define __WJALINK_API_H__
#define WJALNK_CODEC_H264							0x34363248	//H264
#define WJALNK_CODEC_MJPEG							0x45464758	//mjpeg
typedef enum Wja_ContrlPlaybackType_
{
	WjaPausePlayBack=1,
	WjaStopPlayBack,
	WjaResumePlayBack ,
	WjaFastPlayBack ,
	WjaSlowPlayBack ,
	WjaJumpPlayBack
} Wja_ContrlSendPlayType;

typedef struct InitNetworkServiceParam_Wja
{
	char 			remotehost[32];
	unsigned short	remoteport;
	char 			uuid[32];
	char 			dir[128];
	char 			filename[32];
	unsigned short	channelnum;		// 1-16
}Wja_InitNetworkParam;

// Functionality:
//   Initialize wja netservice terminal services. If net_params is not NULL, the setting
//   will be used to configure the device network interface.
// Parameters:
//   @[in]netparam: init info
//   @[in]upnpparams: network parameter settings, support multiple settings.
//   @[in]param_size: number of network parameter settings
// Return:
//   0 on success, other values on error
//
int wja_netservice_init( Wja_InitNetworkParam netparam);

// Functionality:
//   Terminate wja netservice , all resources will be released.
// Parameters:
//   N/A
// Return:
//   0 on success, otherwise errorno.
int wja_netservice_destroy();


typedef void (*callback)(void*);
// Functionality:
//   Packet real time video frame packet to the dest networkSDK
// Parameters:
//   @[in] channel: the channel of the video data
//     @[in] ismainorsub: is main:0 or subnumber:1  videostream
//	 @[in] isIFrame: 1 for the frame is I frame, 0 for the frame is P frame
//	 @[in] videoData: the video data of the frame
//	 @[in] videoDataLen: the video data length
//	@[in] fun: videoData need not copy ,direct quote,videoData use finish,call fun to release videoData
//	@[in] arg: videoData need not copy ,direct quote,videoData use finish,call fun to release videoData,arg is release parameter
// Return:
//   packeted length

int wja_sendvideodata(unsigned char  channel,
						unsigned char ismainorsub,
			                    char isIFrame, 
			                    void* Data, 
			                    unsigned  DataLen,callback fun,void* arg);

// Functionality:
//   Packet playback video frame packet to the dest networkSDK
int wja_sendplaybackdata(unsigned char channel, 
							char isIFrame, 
							void* Data, 
						unsigned DataLen,callback fun,void* arg);

// Functionality:
//   Packet record download video frame packet to the dest networkSDK,the packet Data include wja private head information
int wja_sendrecorddata(unsigned char channel,                            
							void* Data, 
						unsigned DataLen,unsigned int clientid,callback fun,void* arg);

// Functionality:
//   Packet snap frame packet to the dest networkSDK

int wja_sendsnapdata(unsigned char channel,                            
							void* Data, 
						unsigned DataLen,callback fun,void* arg);

#endif

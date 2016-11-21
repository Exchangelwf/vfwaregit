//
//  "$Id$"
//
//  Copyright (c)1992-2007, Shanghai VisionDigi Technology CO.LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		2010-01-12 eric.yu created.
//

#ifndef __DECODER_API_H__
#define __DECODER_API_H__

#include "Types.h"

#ifdef __cplusplus
extern "C" {
#endif
	/// \defgroup DecoderAPI API Capture
	/// 多路多码流视频解码接口，依赖于Memory接口。
	///	\n 调用流程图:
	/// \code
	///    ============================================
	///                   	|   
	///			    DecoderGetChannels
	///									       |                         
	///                DecoderCreate           |  
	///       +-------------|------------------+         
	///       | DecoderSetAudioVideoEnable     |
	///       |           	|           	   |	 
	///       |        DecoderStart     	   |   
	///       | +-----------|                  |   
	///       | |  DigitCaputurePutBuffer  	   |  
	///       | +-----------|                  |         
	///       |        DecoderStop             |         
	///       +-------------|------------------+         
	///               DecoderDestroy                       
	///                     |                            
	///    ============================================	


/// 解码策略,值越大表示越流畅但延迟越大
enum DECODER_POLICY
{
	decoderPolicyMoreRealTime	= -2,	///< 最实时
	decoderPolicyRealTime		= -1,	///< 实时
	decoderPolicyDefault		= 0,	///< 默认情况
	decoderPolicyFluency		= 1,	///< 流畅
	decoderPolicyMoreFluency	= 2,	///< 最流畅
};


/// dsp 解码信息结构
typedef struct DECODER_DSPINFO 
{
		int   iChannel;					//解码通道号
		char sVideoType[8];				//视频类型：        “MPG4” “ H.264" 都用4字节文本
		int   iVideoWidth;				//视频宽
		int   iVideoHeight;				//视频高
		int   iVideoFramesPerSecond;    //视频平均帧率
		int   iFramesType;				//场或者帧   有些D1是两场，有些是帧，帧为0，场为1
		char sAudioType[8];				//音频类型：用字符串表示       "NONE" "ADPM" "G711"  等等
		int   iAudioSampleKRate;		//音频采样率        8K？ 16K？,用数值8或16
		int   iKBPS;

}DECODER_DSPINFO, *PDECODER_DSPINFO;

//获取解码dsp信息，0表示成功，非0表示获取失败
int DecoderGetDspInfo(int iChannel, DECODER_DSPINFO *pDspInfo);

/// 获得支持的数字解码通道
/// \retval 获得通道，0表示不支持。
int DecoderGetChannels(void);

///　创建解码设备
/// \param [in] iChannel 通道号
/// \retval 0  创建成功。
/// \retval !=0  创建失败。
int DecoderCreate(int iChannel);

/// 销毁解码设备
int DecoderDestroy(int iChannel);

/// 设置解码策略
/// \param [in] iChannel 通道号
/// \param[in] iPolicy 策略
/// \		   目前一共有5档(-2 -1 0 1 2),值越大表示越流畅但延迟越大
/// \		   -2表示实时性最好，2表示流畅性最好，0默认
/// \retval 0  设置成功。
/// \retval !=0  设置失败。
int DecoderSetPolicy(int iChannel, int iPolicy);

/// 设置解码
///int DecoderSetFormat(int iChannel, int imagesize);

/// 获取解码的分辨率，注意：此函数的返回和当前分割有关
/// \param [in] iChannel 通道号
/// \retval 分辨率，参照capture_size_t。
int DecoderGetImageSize(int iChannel);

/// 解码开始
/// \param [in] iChannel 通道号
/// \retval 0  成功。
/// \retval !=0  失败。
int DecoderStart(int iChannel);

///　解码停止
/// \param [in] iChannel 通道号
/// \retval 0  成功。
/// \retval !=0  失败。
int DecoderStop(int iChannel);

/// 设置数据给解码器输出
/// \param [in] iChannel 通道号
/// \param [in] pBuffer	数据指针
/// \param [in] iLength	数据长度
/// \param [in] iTimeOut 设置数据的超时时间，以ms为单位
/// \retval 0  成功。
/// \retval !=0  失败。
int DecoderPutBuffer(int iChannel, void* pBuffer, int iLength, int iTimeOut);

typedef struct DECODER_BUF_GROUP
{
    uchar   *pu8Addr[32];
    uint   u32Len[32];
    int 	count;	//!缓冲区个数
} DECODER_BUF_GROUP;

//!放多个缓冲区进去，返回0成功，全部使用，否则一个缓冲区都不会用
int DecodePutMultiBuf(int iChannel, DECODER_BUF_GROUP* pDecoderBuf, int iTimeOut);

/// 解码数据包结构
typedef struct DECODER_BLOCK
{
	/// 需要的数据块字节数。
	uint		length;
}DECODER_BLOCK;

int DecoderGetBuffer(int channel, DECODER_BLOCK *pBlock);

/// 设置数字通道对应的预览通道号
/// \param[in] iChannel 数值通道号，取值0~DecoderGetChannels()
/// \param[in] targetDisplayChannel　显示通道号，取值0~15
/// \retval 0  成功。
/// \retval !=0  失败。
int DecoderMapChannel(int iChannel, int targetDisplayChannel);

/// 取消数字通道和显示通道的对应关系
/// \param[in] iChannel 数值通道号，取值0~DecoderGetChannels()
/// \retval 0  成功。
/// \retval !=0  失败。
int DecoderUnmapChannel(int iChannel);

/// 返回数字通道对应的显示通道，如果数字通道未被map，或者其它错误，返回-1
/// 否则返回显示通道号(>=0)
/// \param[in] iChannel 数值通道号，取值0~DecoderGetChannels()
/// \return 成功返回显示通道号，失败返回-1
int DecoderGetMappedChannel(int iChannel);

//设置解码音视频使能
/// \param [in] iChannel 通道号 取值0~DecoderGetChannels()-1
/// \param [in] iAudioEnable	音频解码使能
/// \param [in] iVideoEnable	视频解码使能
/// \retval 0  成功。
/// \retval !=0  失败。
////create时默认全部打开
int  DecoderSetAVEnable(int iChannel,int iAudioEnable,int iVideoEnable);

//设置解码音视频使能
/// \param [in] iChannel 通道号 取值0~DecoderGetChannels()-1
/// \param [out] piAEnable	音频解码使能
/// \param [out] piVEnable	视频解码使能
/// \retval 0  成功。
/// \retval !=0  失败。
int  DecoderGetAVEnable(int iChannel,int *piAEnable,int *piVEnable);

/// 清空解码缓冲，重新搜寻I帧。上层丢数据时会调用
/// 
/// \param [in] channel 通道号。
/// \retval 0  清空成功
/// \retval <0  清空失败
int DecoderFlush(int iChannel);

#ifdef __cplusplus
}
#endif
#endif//__DECODER_API_H__


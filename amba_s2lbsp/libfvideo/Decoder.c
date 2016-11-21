#include "Decoder.h"
#include "Types.h"
#include "ipc_inc.h"

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


//获取解码dsp信息，0表示成功，非0表示获取失败
int DecoderGetDspInfo(int iChannel, DECODER_DSPINFO *pDspInfo){  return 0;}

/// 获得支持的数字解码通道
/// \retval 获得通道，0表示不支持。
int DecoderGetChannels(void){  return 0;}

///　创建解码设备
/// \param [in] iChannel 通道号
/// \retval 0  创建成功。
/// \retval !=0  创建失败。
int DecoderCreate(int iChannel){  return 0;}

/// 销毁解码设备
int DecoderDestroy(int iChannel){  return 0;}

/// 设置解码策略
/// \param [in] iChannel 通道号
/// \param[in] iPolicy 策略
/// \		   目前一共有5档(-2 -1 0 1 2),值越大表示越流畅但延迟越大
/// \		   -2表示实时性最好，2表示流畅性最好，0默认
/// \retval 0  设置成功。
/// \retval !=0  设置失败。
int DecoderSetPolicy(int iChannel, int iPolicy){  return 0;}

/// 设置解码
///int DecoderSetFormat(int iChannel, int imagesize){  return 0;}

/// 获取解码的分辨率，注意：此函数的返回和当前分割有关
/// \param [in] iChannel 通道号
/// \retval 分辨率，参照capture_size_t。
int DecoderGetImageSize(int iChannel){  return 0;}

/// 解码开始
/// \param [in] iChannel 通道号
/// \retval 0  成功。
/// \retval !=0  失败。
int DecoderStart(int iChannel){  return 0;}

///　解码停止
/// \param [in] iChannel 通道号
/// \retval 0  成功。
/// \retval !=0  失败。
int DecoderStop(int iChannel){  return 0;}

/// 设置数据给解码器输出
/// \param [in] iChannel 通道号
/// \param [in] pBuffer	数据指针
/// \param [in] iLength	数据长度
/// \param [in] iTimeOut 设置数据的超时时间，以ms为单位
/// \retval 0  成功。
/// \retval !=0  失败。
int DecoderPutBuffer(int iChannel, void* pBuffer, int iLength, int iTimeOut){  return 0;}

//!放多个缓冲区进去，返回0成功，全部使用，否则一个缓冲区都不会用
int DecodePutMultiBuf(int iChannel, DECODER_BUF_GROUP* pDecoderBuf, int iTimeOut){  return 0;}

int DecoderGetBuffer(int channel, DECODER_BLOCK *pBlock){  return 0;}

/// 设置数字通道对应的预览通道号
/// \param[in] iChannel 数值通道号，取值0~DecoderGetChannels()
/// \param[in] targetDisplayChannel　显示通道号，取值0~15
/// \retval 0  成功。
/// \retval !=0  失败。
int DecoderMapChannel(int iChannel, int targetDisplayChannel){  return 0;}

/// 取消数字通道和显示通道的对应关系
/// \param[in] iChannel 数值通道号，取值0~DecoderGetChannels()
/// \retval 0  成功。
/// \retval !=0  失败。
int DecoderUnmapChannel(int iChannel){  return 0;}

/// 返回数字通道对应的显示通道，如果数字通道未被map，或者其它错误，返回-1
/// 否则返回显示通道号(>=0)
/// \param[in] iChannel 数值通道号，取值0~DecoderGetChannels()
/// \return 成功返回显示通道号，失败返回-1
int DecoderGetMappedChannel(int iChannel){  return 0;}

//设置解码音视频使能
/// \param [in] iChannel 通道号 取值0~DecoderGetChannels()-1
/// \param [in] iAudioEnable	音频解码使能
/// \param [in] iVideoEnable	视频解码使能
/// \retval 0  成功。
/// \retval !=0  失败。
////create时默认全部打开
int  DecoderSetAVEnable(int iChannel,int iAudioEnable,int iVideoEnable){  return 0;}

//设置解码音视频使能
/// \param [in] iChannel 通道号 取值0~DecoderGetChannels()-1
/// \param [out] piAEnable	音频解码使能
/// \param [out] piVEnable	视频解码使能
/// \retval 0  成功。
/// \retval !=0  失败。
int  DecoderGetAVEnable(int iChannel,int *piAEnable,int *piVEnable){  return 0;}

/// 清空解码缓冲，重新搜寻I帧。上层丢数据时会调用
/// 
/// \param [in] channel 通道号。
/// \retval 0  清空成功
/// \retval <0  清空失败
int DecoderFlush(int iChannel){  return 0;}


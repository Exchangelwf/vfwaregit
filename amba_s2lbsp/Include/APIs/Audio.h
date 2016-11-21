

#ifndef __AUDIO_API_H__
#define __AUDIO_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif
	
/// \defgroup AudioOutAPI API Audio Output 
/// 音频解码与输出控制设备接口，独立于回放设备。
/// \n 调用流程图:
/// \code
///    ===============================================
///               |
///          AudioCreate
///               |------------+------------------+
///               |            |                  |  
///               |      AudioOutSetFormat        | 
///               |            |                  |
///               |      AudioOutStart            |	
///        AudioSwitch         |--------------+   |
///               |      AudioOutPutBuf       |   |
///               |            |--------------+   |
///               |        AudioOutStop           |
///               |            |                  |
///               |------------+------------------+
///          AudioDestory
///               |
///    ===============================================
/// \endcode
/// @{

/// 音频切换类型
enum audio_switch_t{
	AUDIO_SWITCH_MONITOR,	///< 切换到监视输出。
	AUDIO_SWITCH_PLAY,		///< 切换到回放输出。
	AUDIO_SWITCH_MUTE,		///< 静音。
	AUDIO_SWITCH_SPEAK		///< 对讲。
};

typedef struct tagAUDIOIN_CAPS
{
       /* 按位表示支持的音频格式。
       第1位表示line in 第2 位表示mic in */
       uchar ucAudioInType;
       //!前两字节接位表示相应通道是否支持音频, ucRes[2]暂不使用
       //!此字节为新增加，为了和以前进兼容0-表示支持， 1-表示不支持
       uchar  ucRes[3];
} AUDIOIN_CAPS;

/// 创建音频设备
///
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int AudioCreate(void);


/// 销毁音频设备
///
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int AudioDestory(void);


/// 切换音视频输出的类型。
///
/// \param [in] dwType 输出的类型，取下表中的值。audio_switch_t
/// \param [in] dwChannel 监视或回放的通道号。
/// \retval 0  切换成功。
/// \retval <0  切换失败。
int AudioSwitch(uint dwType, uint dwChannel);

/// 开启语音对讲功能
///
/// \param 无
/// \return>0 开启失败
/// \retval 0 开启成功
int AudioOutStart(void);

/// 停止语音对讲功能
///
/// \param 无
/// \return>0 停止失败
/// \retval 0 停止成功
int AudioOutStop(void);

/// 语音对讲解码数据填充
///
/// \param [in] src 解码数据缓冲地址
/// \param [in] size 解码数据缓冲长度
/// \return>0 填充失败
/// \retval 0 填充成功
void AudioOutPutBuf(unsigned char *src, int size);


int AudioOutSetVolume(int iChannel, uint dwLVolume, uint dwRVolume);

int AudioOutGetCaps(AUDIOIN_CAPS * pCaps);

/// 设置数据的压缩格式
///
/// \param [in] coderType 压缩格式
/// \return>0 设置失败
/// \retval 0 设置成功
int AudioOutSetFormat(int coderType);

/// @}
/************************************************************************
	audio in
************************************************************************/

enum audio_encode_type
{
	NONE = 0,
	G729_8KBIT,
	G726_16KBIT,
	G726_24KBIT,
	G726_32KBIT,
	G726_40KBIT,
	PCM_8TO16BIT,
	PCM_ALAW,
	PCM_ULAW,
	ADPCM8K16BIT,
	ADPCM16K16BIT,
	G711_ALAW,   //zhuangke
	MPEG2_LAYER1,
	AMR8K16BIT,
	G711_ULAW,
	IMA_ADPCM_8K16BIT,
	MP3
};

typedef struct tagAUDIOIN_FORMAT
{
	uint	BitRate;	/**< 码流大小，kbps为单位，比如192kbps，128kbps。*/
	uint	SampleRate;	/**< 采样率，Hz为单位，比如44100Hz。*/
	uint	SampleBit;		/*!< 采样的位深 */
	int		EncodeType;		/*!< 编码方式，参照audio_encode_type定义 */
} AUDIOIN_FORMAT;

/*!
获得语音对讲支持的编码格式。
\param AUDIOIN_FORMAT* pFormat: 保存的格式。
\param int iMax: 最大个数
\return 支持的个数
*/
int AudioGetFormat(AUDIOIN_FORMAT* pFormat, int iMax);

/*
	change audio sample mode:line_input or mic_input
*/
typedef enum  
{
	AUDIO_INPUT_LINE,
	AUDIO_INPUT_MIC,
	AUDIO_INPUT_MODE_MAX
}AUDIO_INPUT_MODE;

int AudioInModeChange(const AUDIO_INPUT_MODE sample_mode);

//////////////////////////////////////////////////////////////////////////////
/// \defgroup AudioInAPI API Audio Input
/// 音频数据编码接口，独立于捕获设备。
/// \n 调用流程图:
/// \code
///    =========================================
///                   |                         
///      *AudioInGetChannels,AudioInGetCaps      
///                   |                         
///              AudioInCreate                  
///       +-----------|----------------+        	 
///       |     AudioInSetFormat       |        	
///       +-----------|                |        	 
///       |      AudioInStart          |        	 
///       | +---------|         AudioInSetVolume
///       | |  AudioInGetBuffer        |        	 
///       | +---------|                |           	
///       |      AudioInStop           |         	
///       +-----------|----------------+         	
///             AudioInDestroy                  
///                   |                         
///    =========================================
/// \endcode
/// @{

/// 音频输入格式
/*typedef struct AUDIOIN_FORMAT
{
	uint	BitRate;	///< 码流大小，kbps为单位，比如192kbps，128kbps。
	uint	SampleRate;	///< 采样率，Hz为单位，比如44100Hz。
} AUDIOIN_FORMAT;

/// 音频输入设备特性
typedef struct AUDIOIN_CAPS
{
	uint	Reserved;	///< 保留。
} AUDIOIN_CAPS;
*/

/// 得到额外的音频输入通道数, 不包括录像通道。以下所有AudioIn均表示额外的音频输入。
///
/// \param 无
/// \return额外的音频输入通道数目。
int AudioInGetChannels(void);


/// 创建音频输入设备
///
/// \param [in] iChannel 通道号
/// \retval <0 创建失败
/// \retval 0 创建成功
int AudioInCreate(int iChannel);


/// 销毁音频输入设备
///
/// \param [in] iChannel 通道号
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int AudioInDestroy(int iChannel);


/// 开始音频编码
///
/// \param [in] iChannel 通道号
/// \retval <0 开始失败
/// \retval 0 开始成功
int AudioInStart(int iChannel);


/// 停止音频编码
///
/// \param [in] iChannel 通道号
/// \retval <0 停止失败
/// \retval 0 停止成功
int AudioInStop(int iChannel);


/// 读取采样数据，读为阻塞超时模式。超时时立即返回，传出实际读到的数据，将超时时间
/// 设置的较小，可以提高数据的实时性。
///
/// \param [in] iChannel 通道号，-1表示读所有通道的数据，读到的通道号作为函数返回值返回。
/// \param [in] pBuffer 用来存放音频编码数据的缓冲指针。
/// \param [in,out] pdwCount 传入的值是缓冲大小，传出的值是实际读到数据的长度。
/// \param [in] dwTimeout 读超时的时间，单位为毫秒，设为0表示一直等到读出数据或出错为止。
/// \return>=0 通道号（在输入通道号为-1时）
/// \retval 0  读成功
/// \retval <0  读失败
int AudioInGetBuffer(int iChannel, uchar* pBuffer, uint* pdwCount, uint dwTimeout);


/// 设置音频数据编码的格式。
///
/// \param [in] iChannel 通道号。
/// \param [in] pFormat 指向音频编码格式结构AUDIOIN _FORMAT的指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int AudioInSetFormat(int iChannel, AUDIOIN_FORMAT * pFormat);


/// 设置音频输入的音量。
///
/// \param [in] iChannel 通道号。
/// \param [in] dwLVolume 左声道音量，取值0-100。
/// \param [in] dwRVolume 右声道音量，取值0-100。
/// \retval 0  设置成功
/// \retval <0  设置失败
int AudioInSetVolume(int iChannel, uint dwLVolume, uint dwRVolume);


/// 得到音频输入支持的特性。
///
/// \param [out] pCaps 指向音频输入特性结构AUDIOIN_CAPS的指针。
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int AudioInGetCaps(AUDIOIN_CAPS * pCaps);

/// @} end of group

/// 获得指定路径上的音频文件
/// 
/// \param [in] FilePath 音频文件路径
/// \param [in] MaxNum 获取的最大文件数
/// \param [in] FileNameMaxLen 文件名最大长度
/// \param [in] type 音频文件类型
/// \param [out] AudioFileName 文件名列表
/// \retval >=0  获取成功，并返回文件数。
/// \retval <0  获取失败，-1为不支持的格式，-2为没有TF卡，-3为未知错误。
int AudioGetAudioFiles(char *FilePath, int MaxNum, char *AudioFileName[], int FileNameMaxLen, enum audio_encode_type type);

/// 播放指定路径的音频文件
/// 
/// \param [in] type 音频文件类型
/// \param [in] loop 是否循环播放 1:是 0:否
/// \param [out] AudioFileName 文件名
/// \retval 0  播放成功
/// \retval <0  播放失败，-1为正在播放
int AudioPlayAudioStart(char *AudioFileName, enum audio_encode_type type, char loop);

/// 停止播放音频文件
/// 
/// \retval 0  停止成功
/// \retval <0  停止失败
int AudioPlayAudioStop();

/// 获取播放状态
/// 
/// \retval 1  播放状态
/// \retval 0  停止状态
int AudioPlayAudioStatus();

#ifdef __cplusplus
}
#endif

#endif //__AUDIO_API_H__




#ifndef __PLAY_API_H__
#define __PLAY_API_H__

#include "DVRDEF.H"
#include "Video.h" 
#include "System.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup PlayAPI API Play
/// ��·����Ƶ�طŽ���ӿڡ�
///	\n ��������ͼ:
/// \code
///    =======================================================
///                   |                            
///	         *PlayGetChannels       
///             PlayGetCaps            
///                   |                            
///              PlayCreate                      
///       +-----------|      
///       |      PlaySetFPS
///       |      PlaySetVstd                 	
///       |           |           	 
///       |       PlayStart       
///       | +---------|-------------+--------------+     
///       | |         |(Data)       |(Ctrl)        |(Ctrl)
///       | |   PlayGetBuffer   PlayPause     PlaySetRect          
///       | |         |             |        PlaySetVolume
///       | |    PlayPutBuffer  PlayStep      PlayGetTime
///       | |         |             |       PlaySetDirection
///       | |    PlaySetEof     PlayResume   PlaySetSpeed
///       | |     PlayFlush         |              |
///       | +---------|-------------+--------------+          
///       |        PlayStop   
///       +-----------|      
///              PlayDestory                       
///                   |                            
///    =======================================================
/// \endcode
/// @{

enum search_type
{
	BY_TIME = 0,
	BY_CARD,
	BY_MAX,
};

/// �ط����ݰ��ṹ
typedef struct PLAY_BLOCK
{
	/// ��Ҫ�����ݿ��ֽ�����
	uint		length;

	/// ��Ҫƫ�Ƶ��ֽ���������ط�ʱ�����ֵһֱ��0���ɣ�����ط�ʱ��
	/// ��Ҫ����ǰƫ�Ƶĳ��ȣ�Ϊһ��������
	int			offset;		
}PLAY_BLOCK;

/// �ط����Խṹ
typedef struct PLAY_CAPS
{
	/// ֧�ֵ����򲥷��ٶȵ����룬λ��Ŷ�Ӧö������play_speed_t��ÿһ��ֵ��
	/// ��1��ʾ֧�ָ�ö��ֵ��Ӧ�����ԣ���0��ʾ��֧�֡�
	uint ForwardSpeed;

	/// ֧�ֵĵ��򲥷��ٶȵ����룬λ��Ŷ�Ӧö������play_speed_t��ÿһ��ֵ��
	/// ��1��ʾ֧�ָ�ö��ֵ��Ӧ�����ԣ���0��ʾ��֧�֡�
	uint BackwardSpeed;

	/// ֧�ֵĵ������ŷ�������룬λ��Ŷ�Ӧö������play_direction_t��ÿһ��ֵ��
	/// ��1��ʾ֧�ָ�ö��ֵ��Ӧ�����ԣ���0��ʾ��֧�֡�
	uint StepDirection;

	/// �Ƿ�֧���������ƣ�0-��֧�֣�1-֧�֡�
	uchar  VolumeControl;

	/// ����, Reserved[0]: ��λ��ʾ����, ��1λ��ʾ: 1-֧��jpegץͼ�� 0-��֧��
	uchar  Reserved[3];		
}PLAY_CAPS;

/// �ط��ٶ�ö��ֵ
enum play_speed_t {
	PLAY_SPEED_SLOW4,		///< ����4
	PLAY_SPEED_SLOW3,		///< ����3
	PLAY_SPEED_SLOW2,		///< ����2
	PLAY_SPEED_SLOW1,		///< ����1
	PLAY_SPEED_NORMAL,		///< �����ٶ�
	PLAY_SPEED_FAST1,		///< ����1
	PLAY_SPEED_FAST2,		///< ����2
	PLAY_SPEED_FAST3,		///< ����3
	PLAY_SPEED_FAST4		///< ����4
};

/// �طŷ���ö��ֵ
enum play_direction_t {	
	PLAY_DIRECTION_FORWARD,	///< ���򲥷š�
	PLAY_DIRECTION_BACKWARD	///< ���򲥷š�
};

//!�طŷָ��Ԥ���ָ����岻̫һ�������¶�������
enum play_split_combine_t
{
	Play_SPLIT1,
	Play_SPLIT2,
	Play_SPLIT4,
	Play_SPLIT8,
	Play_SPLIT16,
	Play_SPLIT_num,
};

//!�طŽ�������
enum play_dec_type_t
{
	PLAY_H264,
	PLAY_JPEG
};

/// �õ��طŵ�ͨ����Ŀ�������������طŵ�ͨ���������ط�ͨ�������ط�ͨ���ż�1��
/// ���������ÿ����Ƶ������ϵĻط�ͨ����һ�£���PlayGetChannels����2��������
/// ��Ƶ����ˣ��������0�ϵĻط�ͨ�����Ϊ0��1�������1�ϵĻط�ͨ�����Ϊ2��3��
/// ����ϵͳ�������������ƣ����ֻ���������ط�ͨ����������ͬһ������˿ڣ�Ҳ
/// �����ڲ�ͬ�Ķ˿ڡ�
/// 
/// \param ��
/// \returnӳ�䵽��һ��Ƶ����˿��ϵ����طŵ�ͨ����Ŀ��
int PlayGetChannels(void);


/// �����ط��豸
/// 
/// \param [in] channel ͨ���š�
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int PlayCreate(int channel);


/// ���ٻط��豸
/// 
/// \param [in] channel ͨ���š�
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int PlayDestory(int channel);


/// ��ʼ����, �ײ���ջط�EOF��־, ���TOF��־.
/// 
/// \param [in] channel ͨ���š�
/// \retval 0  ��ʼ�ɹ�
/// \retval <0  ��ʼʧ��
int PlayStart(int channel);


/// ֹͣ���룬��ǰ���ڽ��е���������Ӧ�������ء�
/// 
/// \param [in] channel ͨ���š�
/// \retval 0  ֹͣ�ɹ�
/// \retval <0  ֹͣʧ��
int PlayStop(int channel);


/// ��ͣ���롣
/// 
/// \param [in] channel ͨ���š�
/// \retval 0  ��ͣ�ɹ�
/// \retval <0  ��ͣʧ��
int PlayPause(int channel);


/// �ָ����롣
/// 
/// \param [in] channel ͨ���š�
/// \retval 0  �ָ��ɹ�
/// \retval <0  �ָ�ʧ��
int PlayResume(int channel);


/// ��ս��뻺�壬������ѰI֡��
/// 
/// \param [in] channel ͨ���š�
/// \retval 0  ��ճɹ�
/// \retval <0  ���ʧ��
int PlayFlush(int channel);


/// ��ûط���Ҫ�����ݿ����Ϣ�����طŵ�EOF��־����ʱ, ����ײ㻺���Ѿ�ȫ������
/// ��ɣ�Ӧ�ó����´ε���PlayGetBufferʱ�᷵�ط��سɹ�, ����pBlock�еĻ��峤��
/// ����Ϊ0, Ӧ�ó���ݴ�ֹͣ�ط�, �ﵽ�طŽ���ͬ����Ŀ��.
///
/// \param [in] channel ͨ���š�
/// \param [out] pBlock ָ��ط����ݿ���Ϣ�ṹPLAY_BLOCK��ָ�롣Ӧ�ó���������
///        �ṹ��ƫ���ļ�ָ�룬��׼����Ӧ��С�����ݣ�Ȼ�����PlayPutBuffer�ӿ���
/// 	   �����ݡ��൱�ڻط�ʱ�ļ��ķ��ʲ�����API�������ƣ�Ӧ�ó���ֻ�����ļ���
///        ��ȡ��
/// \retval 0  ��óɹ��������峤�ȿ���Ϊ0
/// \retval <0  �طŻ��������� Ӧ�ó��������Ҫ˯��һ���ٳ��ԡ�/// \see   PlaySetEof, PlaySetTof, PlayPutBuffer
int PlayGetBuffer(int channel, PLAY_BLOCK *pBlock);


/// ������ݵ��طŻ��壬�첽��ʽ���ײ�������ʱ, ��Ҫ��ջ����ڵ����ݡ�
/// 
/// \param [in] channel ͨ���š�
/// \param [in] pBuffer ��д������ݻ���ָ�롣
/// \param [in] dwCount ��д������ݳ��ȡ�
/// \retval 0  д�ɹ�
/// \retval <0  дʧ��
int PlayPutBuffer(int channel, uchar * pBuffer, uint dwCount);


/// ���ûطŵ��ٶȡ�
/// 
/// \param [in] channel ͨ���š�
/// \param [in] dwSpeed �ٶȴ�С��ȡ\a  play_speed_t ���͵�ֵ�����ٵ�����Խ��
///       ��ʾԽ�������ٵ�����Խ�󣬱�ʾԽ�졣
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int PlaySetSpeed(int channel, uint dwSpeed);


/// ���ûطŵ�������ʵ�ֶ�·�طźͻطŻ������ŵ�Ŀ�ġ������õ���ʵ�ʻ�������
/// �ɵײ�����ײ���ڷֱ���ҲӦ���Զ�ʶ���ת����
/// 
/// \param [in] channel ͨ���š�
/// \param [in] pSourceRect ��ƵԴ���򣬲������������ϵ��
/// \param [in] pDestRect ��ƵĿ�����򣬲���ͼ��������ϵ��
/// \retval 0  д�ɹ�
/// \retval <0  дʧ��
int PlaySetRect(int channel, VIDEO_RECT *pSourceRect, VIDEO_RECT *pDestRect);

int PlaySyncTime(int channel, SYSTEM_TIME* pSyncTime, uint* millseconds);

int PlaySplitCombine( uint dwMode, uint dwIndex );


/// ���ûطŵ���ʽ��
/// 
/// \param [in] channel ͨ���š�
/// \param [in] dwStandard �ط���ʽ
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ�ܡ�
int PlaySetVstd(int channel,uint dwStandard);


/// ���ûطŵķ���
/// 
/// \param [in] channel ͨ���š�
/// \param [in] dwDirection �طŷ���ȡplay_direction_t���͵�ֵ��
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
/// \see play_direction_t
int PlaySetDirection(int channel, uint dwDirection);


/// ���ûطŵ�������
/// 
/// \param [in] channel ͨ���š�
/// \param [in] dwLVolume ������������ȡֵ0-100��
/// \param [in] dwRVolume ������������ȡֵ0-100��
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int PlaySetVolume(int channel, uint dwLVolume, uint dwRVolume);


/// �ֶ��������ţ���Ҫ�ڲ�����ͣ����ã�������������Ӧ֡�����Զ��ٴ���ͣ������
/// Ϊ��������ʽ������˺�������ʱ�ϴε������Ż�û����ɣ���ȡʣ��֡����������
/// ֡�������ֵ��Ϊ��Ҫ�������ŵ�֡����
/// 
/// \param [in] channel ͨ���š�
/// \param [in] dwFrames ÿ�β������ŵ�֡����
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int PlayStep(int channel, uint dwFrames);

/// ȡ��ǰ���ڻط���Ƶ�����ڵ�ʱ���롣ʱ����Ҫ�ڱ���ʱ��ӵ���Ƶ�����У��Ա�֤
/// �ط�ʱ׼ȷ��ʾʱ�䡣ͬʱ�طŽ���Ľ�����Ҳ�Ǹ������ʱ����ȷ�������, �ڴ�
/// �������ش���ʱ,�ط�ģ������ʱ��, ��ȷ���طŽ������������.
///
/// \param [in] channel ͨ���š�
/// \param [out] pTime �ط�ʱ�䣬ָ��ϵͳʱ��ṹSYSTEM_TIME��ָ�롣
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int PlayGetTime(int channel, SYSTEM_TIME * pTime);

/// ȡ��ǰ���ڻط���Ƶ�����ڵ�ʱ���롣ʱ����Ҫ�ڱ���ʱ��ӵ���Ƶ�����У��Ա�֤
/// �ط�ʱ׼ȷ��ʾʱ�䡣ͬʱ�طŽ���Ľ�����Ҳ�Ǹ������ʱ����ȷ�������, �ڴ�
/// �������ش���ʱ,�ط�ģ������ʱ��, ��ȷ���طŽ������������.
/// 
/// \param [in] channel ͨ���š�
/// \param [out] pTime �ط�ʱ�䣬ָ��ϵͳʱ��ṹSYSTEM_TIME��ָ�롣
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int PlayGetTimeEx(int channel, SYSTEM_TIME * pTime, uint* millseconds);


/// ���ûطŵĻ��ʡ�
/// 
/// \param [in] channel ͨ���š�
/// \param [in] iLevel ���ʵȼ�1-4
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int PlaySetQuality(int channel, int iLevel);


/// �õ��ط�֧�ֵ����ԡ�
/// 
/// \param [out] pCaps ָ��ط����ԽṹPLAY_CAPS��ָ�롣
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int PlayGetCaps(PLAY_CAPS * pCaps);


/// ���������ļ���ȡ��ϱ�־��End of File����
/// �ײ�ط�ģ���ڻ��������ȫ��������ɺ���������־�� 
/// 
/// \param [in] iChannel ͨ���š�
/// \param [in] bIsEof TRUE-���ñ�־��FALST-��ձ�־��
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
/// \see	PlayGetBuffer, PlayStart
void PlaySetEof(int iChannel, VD_BOOL bIsEof);


/// ���ûطŵ�֡�ʡ�
/// 
/// \param [in] channel ͨ���š�
/// \param [in] fps ¼�����ݵ�ʵ�ʱ���֡�ʡ�
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
void PlaySetFPS(int channel, int fps);


/// ���ûط�ʱ��Ƶ����֮��Ļط��������ɫ.
/// 
/// \param [in] color Ҫ���õ���ɫ��16���Ʊ�ʾΪ0x00BBGGRR������ֽڱ�����
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int PlaySetBkColor(uint color);


/// ���������ļ�����ĩβ��־��Tail of File����
/// �ײ�ط�ģ���ڽ��������������־������Tof��ʱ����Ӧ�ó�������� һ����
/// �ļ��������һ��ʱ���ã� �������Ա�֤�ļ��м�����ݴ��󱻺��ԣ����Ժ������
/// �����ˣ������ļ�ĩβ�����ݴ���ᴥ���ط�ֹͣ������ػ��ļ�ĩβ���������⣩��
///
/// \param [in] channel ͨ���š�
/// \param [in] bIsTof TRUE-���ñ�־��FALST-��ձ�־��
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��/// \see	PlayGetBuffer, PlayStart
int PlaySetTof(int channel, VD_BOOL bIsTof);

//!����¼���ļ��ķֱ��ʣ��ײ����ֻ�ǿ���D1�Ľ���ͨ���������¼�����ݷֱ���������ӿ�����
//!height��16λΪ¼���ļ��߶ȣ���16λΪ¼���ļ�֡��
int PlaySetResolution(int channel, uint width,uint height);

int PlaySetReal(int channel);

//!������Ӧͨ���Ľ���ط�����, �����enum play_dec_type_t ö��
int PlaySetDecType(int channel, uint dec_type);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__PLAY_API_H__


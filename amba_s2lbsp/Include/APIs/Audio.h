

#ifndef __AUDIO_API_H__
#define __AUDIO_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif
	
/// \defgroup AudioOutAPI API Audio Output 
/// ��Ƶ��������������豸�ӿڣ������ڻط��豸��
/// \n ��������ͼ:
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

/// ��Ƶ�л�����
enum audio_switch_t{
	AUDIO_SWITCH_MONITOR,	///< �л������������
	AUDIO_SWITCH_PLAY,		///< �л����ط������
	AUDIO_SWITCH_MUTE,		///< ������
	AUDIO_SWITCH_SPEAK		///< �Խ���
};

typedef struct tagAUDIOIN_CAPS
{
       /* ��λ��ʾ֧�ֵ���Ƶ��ʽ��
       ��1λ��ʾline in ��2 λ��ʾmic in */
       uchar ucAudioInType;
       //!ǰ���ֽڽ�λ��ʾ��Ӧͨ���Ƿ�֧����Ƶ, ucRes[2]�ݲ�ʹ��
       //!���ֽ�Ϊ�����ӣ�Ϊ�˺���ǰ������0-��ʾ֧�֣� 1-��ʾ��֧��
       uchar  ucRes[3];
} AUDIOIN_CAPS;

/// ������Ƶ�豸
///
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int AudioCreate(void);


/// ������Ƶ�豸
///
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int AudioDestory(void);


/// �л�����Ƶ��������͡�
///
/// \param [in] dwType ��������ͣ�ȡ�±��е�ֵ��audio_switch_t
/// \param [in] dwChannel ���ӻ�طŵ�ͨ���š�
/// \retval 0  �л��ɹ���
/// \retval <0  �л�ʧ�ܡ�
int AudioSwitch(uint dwType, uint dwChannel);

/// ���������Խ�����
///
/// \param ��
/// \return>0 ����ʧ��
/// \retval 0 �����ɹ�
int AudioOutStart(void);

/// ֹͣ�����Խ�����
///
/// \param ��
/// \return>0 ֹͣʧ��
/// \retval 0 ֹͣ�ɹ�
int AudioOutStop(void);

/// �����Խ������������
///
/// \param [in] src �������ݻ����ַ
/// \param [in] size �������ݻ��峤��
/// \return>0 ���ʧ��
/// \retval 0 ���ɹ�
void AudioOutPutBuf(unsigned char *src, int size);


int AudioOutSetVolume(int iChannel, uint dwLVolume, uint dwRVolume);

int AudioOutGetCaps(AUDIOIN_CAPS * pCaps);

/// �������ݵ�ѹ����ʽ
///
/// \param [in] coderType ѹ����ʽ
/// \return>0 ����ʧ��
/// \retval 0 ���óɹ�
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
	uint	BitRate;	/**< ������С��kbpsΪ��λ������192kbps��128kbps��*/
	uint	SampleRate;	/**< �����ʣ�HzΪ��λ������44100Hz��*/
	uint	SampleBit;		/*!< ������λ�� */
	int		EncodeType;		/*!< ���뷽ʽ������audio_encode_type���� */
} AUDIOIN_FORMAT;

/*!
��������Խ�֧�ֵı����ʽ��
\param AUDIOIN_FORMAT* pFormat: ����ĸ�ʽ��
\param int iMax: ������
\return ֧�ֵĸ���
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
/// ��Ƶ���ݱ���ӿڣ������ڲ����豸��
/// \n ��������ͼ:
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

/// ��Ƶ�����ʽ
/*typedef struct AUDIOIN_FORMAT
{
	uint	BitRate;	///< ������С��kbpsΪ��λ������192kbps��128kbps��
	uint	SampleRate;	///< �����ʣ�HzΪ��λ������44100Hz��
} AUDIOIN_FORMAT;

/// ��Ƶ�����豸����
typedef struct AUDIOIN_CAPS
{
	uint	Reserved;	///< ������
} AUDIOIN_CAPS;
*/

/// �õ��������Ƶ����ͨ����, ������¼��ͨ������������AudioIn����ʾ�������Ƶ���롣
///
/// \param ��
/// \return�������Ƶ����ͨ����Ŀ��
int AudioInGetChannels(void);


/// ������Ƶ�����豸
///
/// \param [in] iChannel ͨ����
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int AudioInCreate(int iChannel);


/// ������Ƶ�����豸
///
/// \param [in] iChannel ͨ����
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int AudioInDestroy(int iChannel);


/// ��ʼ��Ƶ����
///
/// \param [in] iChannel ͨ����
/// \retval <0 ��ʼʧ��
/// \retval 0 ��ʼ�ɹ�
int AudioInStart(int iChannel);


/// ֹͣ��Ƶ����
///
/// \param [in] iChannel ͨ����
/// \retval <0 ֹͣʧ��
/// \retval 0 ֹͣ�ɹ�
int AudioInStop(int iChannel);


/// ��ȡ�������ݣ���Ϊ������ʱģʽ����ʱʱ�������أ�����ʵ�ʶ��������ݣ�����ʱʱ��
/// ���õĽ�С������������ݵ�ʵʱ�ԡ�
///
/// \param [in] iChannel ͨ���ţ�-1��ʾ������ͨ�������ݣ�������ͨ������Ϊ��������ֵ���ء�
/// \param [in] pBuffer ���������Ƶ�������ݵĻ���ָ�롣
/// \param [in,out] pdwCount �����ֵ�ǻ����С��������ֵ��ʵ�ʶ������ݵĳ��ȡ�
/// \param [in] dwTimeout ����ʱ��ʱ�䣬��λΪ���룬��Ϊ0��ʾһֱ�ȵ��������ݻ����Ϊֹ��
/// \return>=0 ͨ���ţ�������ͨ����Ϊ-1ʱ��
/// \retval 0  ���ɹ�
/// \retval <0  ��ʧ��
int AudioInGetBuffer(int iChannel, uchar* pBuffer, uint* pdwCount, uint dwTimeout);


/// ������Ƶ���ݱ���ĸ�ʽ��
///
/// \param [in] iChannel ͨ���š�
/// \param [in] pFormat ָ����Ƶ�����ʽ�ṹAUDIOIN _FORMAT��ָ�롣
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int AudioInSetFormat(int iChannel, AUDIOIN_FORMAT * pFormat);


/// ������Ƶ�����������
///
/// \param [in] iChannel ͨ���š�
/// \param [in] dwLVolume ������������ȡֵ0-100��
/// \param [in] dwRVolume ������������ȡֵ0-100��
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int AudioInSetVolume(int iChannel, uint dwLVolume, uint dwRVolume);


/// �õ���Ƶ����֧�ֵ����ԡ�
///
/// \param [out] pCaps ָ����Ƶ�������ԽṹAUDIOIN_CAPS��ָ�롣
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int AudioInGetCaps(AUDIOIN_CAPS * pCaps);

/// @} end of group

/// ���ָ��·���ϵ���Ƶ�ļ�
/// 
/// \param [in] FilePath ��Ƶ�ļ�·��
/// \param [in] MaxNum ��ȡ������ļ���
/// \param [in] FileNameMaxLen �ļ�����󳤶�
/// \param [in] type ��Ƶ�ļ�����
/// \param [out] AudioFileName �ļ����б�
/// \retval >=0  ��ȡ�ɹ����������ļ�����
/// \retval <0  ��ȡʧ�ܣ�-1Ϊ��֧�ֵĸ�ʽ��-2Ϊû��TF����-3Ϊδ֪����
int AudioGetAudioFiles(char *FilePath, int MaxNum, char *AudioFileName[], int FileNameMaxLen, enum audio_encode_type type);

/// ����ָ��·������Ƶ�ļ�
/// 
/// \param [in] type ��Ƶ�ļ�����
/// \param [in] loop �Ƿ�ѭ������ 1:�� 0:��
/// \param [out] AudioFileName �ļ���
/// \retval 0  ���ųɹ�
/// \retval <0  ����ʧ�ܣ�-1Ϊ���ڲ���
int AudioPlayAudioStart(char *AudioFileName, enum audio_encode_type type, char loop);

/// ֹͣ������Ƶ�ļ�
/// 
/// \retval 0  ֹͣ�ɹ�
/// \retval <0  ֹͣʧ��
int AudioPlayAudioStop();

/// ��ȡ����״̬
/// 
/// \retval 1  ����״̬
/// \retval 0  ֹͣ״̬
int AudioPlayAudioStatus();

#ifdef __cplusplus
}
#endif

#endif //__AUDIO_API_H__


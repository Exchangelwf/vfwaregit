#include "Decoder.h"
#include "Types.h"
#include "ipc_inc.h"

	/// \defgroup DecoderAPI API Capture
	/// ��·��������Ƶ����ӿڣ�������Memory�ӿڡ�
	///	\n ��������ͼ:
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


//��ȡ����dsp��Ϣ��0��ʾ�ɹ�����0��ʾ��ȡʧ��
int DecoderGetDspInfo(int iChannel, DECODER_DSPINFO *pDspInfo){  return 0;}

/// ���֧�ֵ����ֽ���ͨ��
/// \retval ���ͨ����0��ʾ��֧�֡�
int DecoderGetChannels(void){  return 0;}

///�����������豸
/// \param [in] iChannel ͨ����
/// \retval 0  �����ɹ���
/// \retval !=0  ����ʧ�ܡ�
int DecoderCreate(int iChannel){  return 0;}

/// ���ٽ����豸
int DecoderDestroy(int iChannel){  return 0;}

/// ���ý������
/// \param [in] iChannel ͨ����
/// \param[in] iPolicy ����
/// \		   Ŀǰһ����5��(-2 -1 0 1 2),ֵԽ���ʾԽ�������ӳ�Խ��
/// \		   -2��ʾʵʱ����ã�2��ʾ��������ã�0Ĭ��
/// \retval 0  ���óɹ���
/// \retval !=0  ����ʧ�ܡ�
int DecoderSetPolicy(int iChannel, int iPolicy){  return 0;}

/// ���ý���
///int DecoderSetFormat(int iChannel, int imagesize){  return 0;}

/// ��ȡ����ķֱ��ʣ�ע�⣺�˺����ķ��غ͵�ǰ�ָ��й�
/// \param [in] iChannel ͨ����
/// \retval �ֱ��ʣ�����capture_size_t��
int DecoderGetImageSize(int iChannel){  return 0;}

/// ���뿪ʼ
/// \param [in] iChannel ͨ����
/// \retval 0  �ɹ���
/// \retval !=0  ʧ�ܡ�
int DecoderStart(int iChannel){  return 0;}

///������ֹͣ
/// \param [in] iChannel ͨ����
/// \retval 0  �ɹ���
/// \retval !=0  ʧ�ܡ�
int DecoderStop(int iChannel){  return 0;}

/// �������ݸ����������
/// \param [in] iChannel ͨ����
/// \param [in] pBuffer	����ָ��
/// \param [in] iLength	���ݳ���
/// \param [in] iTimeOut �������ݵĳ�ʱʱ�䣬��msΪ��λ
/// \retval 0  �ɹ���
/// \retval !=0  ʧ�ܡ�
int DecoderPutBuffer(int iChannel, void* pBuffer, int iLength, int iTimeOut){  return 0;}

//!�Ŷ����������ȥ������0�ɹ���ȫ��ʹ�ã�����һ����������������
int DecodePutMultiBuf(int iChannel, DECODER_BUF_GROUP* pDecoderBuf, int iTimeOut){  return 0;}

int DecoderGetBuffer(int channel, DECODER_BLOCK *pBlock){  return 0;}

/// ��������ͨ����Ӧ��Ԥ��ͨ����
/// \param[in] iChannel ��ֵͨ���ţ�ȡֵ0~DecoderGetChannels()
/// \param[in] targetDisplayChannel����ʾͨ���ţ�ȡֵ0~15
/// \retval 0  �ɹ���
/// \retval !=0  ʧ�ܡ�
int DecoderMapChannel(int iChannel, int targetDisplayChannel){  return 0;}

/// ȡ������ͨ������ʾͨ���Ķ�Ӧ��ϵ
/// \param[in] iChannel ��ֵͨ���ţ�ȡֵ0~DecoderGetChannels()
/// \retval 0  �ɹ���
/// \retval !=0  ʧ�ܡ�
int DecoderUnmapChannel(int iChannel){  return 0;}

/// ��������ͨ����Ӧ����ʾͨ�����������ͨ��δ��map�������������󣬷���-1
/// ���򷵻���ʾͨ����(>=0)
/// \param[in] iChannel ��ֵͨ���ţ�ȡֵ0~DecoderGetChannels()
/// \return �ɹ�������ʾͨ���ţ�ʧ�ܷ���-1
int DecoderGetMappedChannel(int iChannel){  return 0;}

//���ý�������Ƶʹ��
/// \param [in] iChannel ͨ���� ȡֵ0~DecoderGetChannels()-1
/// \param [in] iAudioEnable	��Ƶ����ʹ��
/// \param [in] iVideoEnable	��Ƶ����ʹ��
/// \retval 0  �ɹ���
/// \retval !=0  ʧ�ܡ�
////createʱĬ��ȫ����
int  DecoderSetAVEnable(int iChannel,int iAudioEnable,int iVideoEnable){  return 0;}

//���ý�������Ƶʹ��
/// \param [in] iChannel ͨ���� ȡֵ0~DecoderGetChannels()-1
/// \param [out] piAEnable	��Ƶ����ʹ��
/// \param [out] piVEnable	��Ƶ����ʹ��
/// \retval 0  �ɹ���
/// \retval !=0  ʧ�ܡ�
int  DecoderGetAVEnable(int iChannel,int *piAEnable,int *piVEnable){  return 0;}

/// ��ս��뻺�壬������ѰI֡���ϲ㶪����ʱ�����
/// 
/// \param [in] channel ͨ���š�
/// \retval 0  ��ճɹ�
/// \retval <0  ���ʧ��
int DecoderFlush(int iChannel){  return 0;}


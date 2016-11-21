

#ifndef __EXPANDED_CAPTURE_API_H__
#define __EXPANDED_CAPTURE_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup ExcaptureAPI API Excapture
/// ��չ����ӿڣ�����Ԥ�����ݵı��롣
///	\n ��������ͼ:
/// \code
///    =================================
///                   |                 
///            *ExCaptureGetCaps        
///                   |                 
///             ExCaptureCreate         
///       +-----------|     
///       |      ExCaptureSetVstd       
///       |     ExCaptureSetEncode      
///       |      ExCaptureSetSize       
///       |    ExCaptureSetQuality      
///       |      ExCaptureSetFrame      
///       |           |          	 
///       |     ExCaptureStart    
///       | +---------|          
///       | |  ExCaptureGetBuffer 
///       | | ExCaptureGetBufferSize 
///       | +---------|                 
///       |      ExCaptureStop          
///       +-----------|
///           ExCaptureDestory          
///                   |                 
///    =================================
/// \endcode
/// @{

/// ��չ�����豸���Խṹ
typedef struct EXCAPTURE_CAPS
{
	int Enable;			///< �Ƿ�֧����չ����
} EXCAPTURE_CAPS;


/// ������չ�����豸
/// 
/// \param [in] channel ͨ���š�
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int ExCaptureCreate(int channel);


/// ������չ�����豸
/// 
/// \param [in] channel ͨ���š�
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int ExCaptureDestory(int channel);


/// ������չ����
/// 
/// \param [in] dwFlags ������ʾ���������ѡ���capture_start_t���͵�ֵ����ϡ�
/// \retval 0  �����ɹ���
/// \retval <0  ����ʧ�ܡ�
int ExCaptureStart(uint dwFlags);


/// ֹͣ��չ����
/// 
/// \param ��
/// \retval 0  ֹͣ�ɹ���
/// \retval <0  ֹͣʧ�ܡ�
int ExCaptureStop(void);


/// ������չ�����ʽ��
/// 
/// \param [in]  dwStandard ��Ƶ��ʽ��ȡvideo_standard_t���͵�ֵ��
/// \retval 0  ���óɹ���
/// \retval <0  ����ʧ�ܡ�
int ExCaptureSetVstd(uint dwStandard);	


/// �õ��������ݿ�Ĵ�С���´ζ�����ʱ�������ݵĻ����Сһ�����ܱ����ֵС��
/// 
/// \param [out]  pCount ������ֵ��ʵ�ʶ������ֽ�����
/// \retval 0  �طųɹ���
/// \retval <0  �ط�ʧ�ܡ�
int ExCaptureGetBufferSize (uint * pCount);


/// �õ��������ݿ顣
/// 
/// \param [in] pBuffer ������Ų������ݵĻ���ָ�롣
/// \param [in] dwCount �����ֵ��Ҫ�������ֽ�����
/// \retval 0  �طųɹ���
/// \retval <0  �ط�ʧ�ܡ�
int ExCaptureGetBuffer (uchar * pBuffer, uint dwCount);


/// �õ���չ����֧�ֵ����ԡ�
/// 
/// \param [out]  pCaps ָ��ط����ԽṹEXCAPTURE_CAPS��ָ�롣
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int ExCaptureGetCaps(EXCAPTURE_CAPS * pCaps);


/// ǿ��I֡
int ExCaptureForceIFrame(int channel);


/// ���뷽ʽ
int ExCaptureSetEncode(int iChannel, int iEncode);
 

/// ��С
int ExCaptureSetSize(int iChannel, int iSize);


/// ����  	1-6 ��
int ExCaptureSetQuality(int iChannel, int iQuality);


/// ֡��  	1 2 3 6 12 25
int ExCaptureSetFrame(int iChannel, int iFrame);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__EXPANDED_CAPTURE_API_H__


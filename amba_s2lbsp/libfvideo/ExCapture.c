#include "ExCapture.h"
#include "ipc_inc.h"

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


/// ������չ�����豸
/// 
/// \param [in] channel ͨ���š�
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int ExCaptureCreate(int channel){  return 0;}


/// ������չ�����豸
/// 
/// \param [in] channel ͨ���š�
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int ExCaptureDestory(int channel){  return 0;}


/// ������չ����
/// 
/// \param [in] dwFlags ������ʾ���������ѡ���capture_start_t���͵�ֵ����ϡ�
/// \retval 0  �����ɹ���
/// \retval <0  ����ʧ�ܡ�
int ExCaptureStart(uint dwFlags){  return 0;}


/// ֹͣ��չ����
/// 
/// \param ��
/// \retval 0  ֹͣ�ɹ���
/// \retval <0  ֹͣʧ�ܡ�
int ExCaptureStop(void){  return 0;}


/// ������չ�����ʽ��
/// 
/// \param [in]  dwStandard ��Ƶ��ʽ��ȡvideo_standard_t���͵�ֵ��
/// \retval 0  ���óɹ���
/// \retval <0  ����ʧ�ܡ�
int ExCaptureSetVstd(uint dwStandard){  return 0;}	


/// �õ��������ݿ�Ĵ�С���´ζ�����ʱ�������ݵĻ����Сһ�����ܱ����ֵС��
/// 
/// \param [out]  pCount ������ֵ��ʵ�ʶ������ֽ�����
/// \retval 0  �طųɹ���
/// \retval <0  �ط�ʧ�ܡ�
int ExCaptureGetBufferSize (uint * pCount){  return 0;}


/// �õ��������ݿ顣
/// 
/// \param [in] pBuffer ������Ų������ݵĻ���ָ�롣
/// \param [in] dwCount �����ֵ��Ҫ�������ֽ�����
/// \retval 0  �طųɹ���
/// \retval <0  �ط�ʧ�ܡ�
int ExCaptureGetBuffer (uchar * pBuffer, uint dwCount){  return 0;}


/// �õ���չ����֧�ֵ����ԡ�
/// 
/// \param [out]  pCaps ָ��ط����ԽṹEXCAPTURE_CAPS��ָ�롣
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int ExCaptureGetCaps(EXCAPTURE_CAPS * pCaps){  return 0;}


/// ǿ��I֡
int ExCaptureForceIFrame(int channel){  return 0;}


/// ���뷽ʽ
int ExCaptureSetEncode(int iChannel, int iEncode){  return 0;}
 

/// ��С
int ExCaptureSetSize(int iChannel, int iSize){  return 0;}


/// ����  	1-6 ��
int ExCaptureSetQuality(int iChannel, int iQuality){  return 0;}


/// ֡��  	1 2 3 6 12 25
int ExCaptureSetFrame(int iChannel, int iFrame){  return 0;}

/// @} end of group


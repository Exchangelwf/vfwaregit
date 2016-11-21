#include "BlindDetect.h"
#include "ipc_inc.h"

/// \defgroup BlindDetectAPI API Blind Detect
/// ȫ����Χ�������ȵ��ڵ����ӿڡ�
///	\n ��������ͼ:
/// \code
///    ===========================
///                |
///        *BlindDetectGetCaps
///                |
///         BlindDetectCreate
///    +-----------|
///    |   BlindDetectGetState
///    |   BlindDetectSetParameter
///    +-----------|
///       BlindDetectDestory
///                |
///    ===========================
/// \endcode
/// @{

/// ������������豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int BlindDetectCreate(void)
{
	return 0; 
}


/// ���ٺ�������豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int BlindDetectDestory(void)
{
	return 0; 
}


/// ִ�к�����⡣�������ء�
/// 
/// \param [out] pData ָ��һ��uintֵ��ָ�룬uintֵ�Ǹ�����������ͨ����ƽ״̬��
///       ���롣��ͨ���ڵ�λ����ͨ���ڸ�λ���ߵ�ƽ��1���͵�ƽ��0�������ڵ�ͨ����0��
/// \retval 0  ���ɹ�
/// \retval <0  ���ʧ��
int BlindDetectGetState(uint* pData)
{
	return 0;
}


/// ���ú�����Ⲷ��Ĳ�����
/// 
/// \param [in] channel ͨ���š�
/// \param [in] pParam ָ������������ṹBLIND_DETECT_ PARAM��ָ�롣
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int BlindDetectSetParameter(int channel, BLIND_DETECT_PARAM *pParam)
{
	return 0; 
}


/// �õ�����֧�ֵ����ԡ�
/// 
/// \param [out] pCaps ָ��������ԽṹBLIND_DETECT _CAPS��ָ�롣
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int BlindDetectGetCaps(BLIND_DETECT_CAPS * pCaps)
{
	return 0; 
}

/// @} end of group


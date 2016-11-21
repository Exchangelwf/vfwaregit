
#include "LossDetect.h"
#include "ipc_inc.h"


/// \defgroup LossDetectAPI API Loss Detect
/// ��Ƶ��ʧ�ӿڡ�
///	\n ��������ͼ:
/// \code
///    ===========================
///                |
///        *LossDetectGetCaps
///                |
///         LossDetectCreate
///    +-----------|
///    |   LossDetectGetState
///    +-----------|
///       LossDetectDestory
///                |
///    ===========================
/// \endcode
/// @{


/// ������Ƶ��ʧ����豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int LossDetectCreate(void){  return 0;}


/// ������Ƶ��ʧ����豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int LossDetectDestory(void){  return 0;}


/// ִ����Ƶ��ʧ��⡣�������ء�
/// 
/// \param [out] pData ָ��һ��uintֵ��ָ�룬uintֵ�Ǹ�����������ͨ����ƽ״̬
///        �����롣��ͨ���ڵ�λ����ͨ���ڸ�λ���ߵ�ƽ��1���͵�ƽ��0�������ڵ�ͨ
///        ����0��
/// \retval 0  ���ɹ�
/// \retval <0  ���ʧ��
int LossDetectGetState(uint* pData)
{
	return 0;
}


/// �õ���Ƶ��ʧ֧�ֵ����ԡ�
/// 
/// \param [out] pCaps ָ����Ƶ��ʧ���ԽṹLOSS_DETECT _CAPS��ָ�롣
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int LossDetectGetCaps(LOSS_DETECT_CAPS * pCaps){  return 0;}

/// @} end of group


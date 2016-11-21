
#include "IntAlarm.h"
#include "ipc_inc.h"


/// \def group Intelligent Alarm API API Intelligent Alarm
/// ���ܱ����ӿڡ�
///	\n ��������ͼ:
/// \code
///    ===========================
///                |
///        *IntelligentAlarmGetCaps
///                |
///         IntelligentAlarmCreate
///    +-----------|
///    |   IntelligentAlarmGetEvent
///    |   IntelligentAlarmSetState
///    +-----------|
///       IntelligentAlarmDestory
///                |
///    ===========================
/// \endcode
/// @{


/// �������ܱ�������豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int IntelligentAlarmCreate(void)
{
	return 0; 
}


/// �������ܱ�������豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int IntelligentAlarmDestory(void)
{
	return 0; 
}


/// ִ�����ܱ�����־��⡣�������ء�
/// 
/// \param [in] serverip ָ���savia�豸��ip
/// \param [out] pData �¼����͵�ָ�룬��λ�����ұ�ʾ8�ֱ����¼��Ƿ������1��ʾ������0��ʾû�С�
/// \retval 0  ���ɹ�
/// \retval <0  ���ʧ��
int IntelligentAlarmGetEvent(IPADDR serverip, int *pData)
{
	return 0; 
}


/// ִ�����ܱ���״̬���á��������ء�
/// 
/// \param [in] serverip ָ���savia�豸��ip
/// \param [in] State �������ͣ���λ��ʾ�Ƿ�ֱ�����8���㷨��1��ʾ���ã�0��ʾֹͣ��
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int IntelligentAlarmSetState(IPADDR ip, int State)
{
	return 0; 
}


/// �õ����ܱ���֧�ֵ����ԡ�
/// 
/// \param [out] pCaps ָ�����ܱ������ԽṹIntelligent_Alarm _CAPS��ָ�롣
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int IntelligentAlarmGetCaps(Intelligent_Alarm_CAPS * pCaps)
{
	return 0; 
}

/// @} end of group



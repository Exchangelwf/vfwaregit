#include "Analyse.h"
#include "ipc_inc.h"

/******************************************************************/
/// ���ܷ����Ľӿں���
/******************************************************************/

/// ������ܷ���������
/// 
/// \param [out] pCaps ָ�����ܷ��������ṹ��ANALYSE_DETECT_CAPS
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int AnalyseDetectGetCaps(ANALYSE_DETECT_CAPS * pCaps)
{
	return 0; 
}

/// �������ܼ���豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int AnalyseDetectCreate(void)
{
	return 0; 
}


/// �������ܼ���豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int AnalyseDetectDestory(void)
{
	return 0; 
}
/// ���ܼ����
/// 
/// \param [in] channel ͨ���š�
/// \param [in] pResult ָ�����ܼ������ṹANALYSE_DETECT_RESULT��ָ�롣
///	type���ܱ�������
/// \retval 0  ��ȡ�ɹ�
/// \retval <0  ��ȡʧ��
int AnalyseDetectGetResult(int channel, INTELLIGENTTYPE type,ANALYSE_DETECT_RESULT *pResult)
{
	return 0; 
}



/// �������ܼ��ͨ������
/// 
/// \param [in] channel ͨ���š�
/// \param [in] pParam ָ�����ܼ������ṹANALYSE_DETECT_PARAM��ָ�롣
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int AnalyseDetectSetParameter(int channel, INTELLIGENTTYPE type,ANALYSE_DETECT_PARAM *pParam)
{
	return 0; 
}


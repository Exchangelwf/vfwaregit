
#include "Frontboard.h"
#include "ipc_inc.h"

/// \defgroup FrontboardAPI API Frontboard
/// ǰ������API�����ں͸���ǰ��������豸ͨѶ����ȡ��ֵ����ָʾ�ƿ������
/// ����USB�ӿ��ϵı�׼104�������������̼�ֵת����Ҳ����ͨ������ӿڷ���������
/// \n ��������ͼ:
/// \code
///    =========================================
///                   |                         
///              FrontboardCreate                  
///         +---------|      
///         |    FrontboardRead    
///         |    FrontboardWrite   
///         +---------|      	
///             FrontboardDestory                  
///                   |                         
///    =========================================
/// \endcode
/// @{


/// ����ǰ����豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int FrontboardCreate(void)
{
	return 0; 
}


/// ����ǰ����豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int FrontboardDestory(void)
{
	return 0; 
}


/// ��ǰ������ݣ�һֱ�ȵ�����ָ�����ֽڻ����ʱ�ŷ��أ�����������
/// 
/// \param [out] pData ���ݻ����ָ�롣
/// \param [in] nBytes Ҫ�������ֽ���
/// \return>0  �������ֽ���
/// \retval <=0 ��ʧ��
int FrontboardRead(void* pData, uint nBytes)
{
	return 0; 
}


/// дǰ������ݣ�һֱ�ȵ�д��ָ�����ֽڻ����ʱ�ŷ��أ�����������
/// 
/// \param [in] pData ���ݻ����ָ�롣
/// \param [in] nBytes Ҫд����ֽ���
/// \return>0  д����ֽ���
/// \retval <=0  дʧ��
int FrontboardWrite(void* pData, uint nBytes)
{
	return 0; 
}

/// @} end of group



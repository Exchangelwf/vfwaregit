#include "SlaveUart.h"
#include "ipc_inc.h"


//��UART2Create ����֮ǰ������Ч
//Ĭ��Ϊ����ģʽ
//iFlag 1 ������0,����
int SLAVEUARTSetNonBlock(int iFlag)
{
	 
	return 0;
}

/// ����ǰ����豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int SLAVEUARTCreate(void)
{
	 
	return 0;
}


/// ����ǰ����豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int SLAVEUARTDestory(void)
{
	 
	return 0;
}


/// ���ô������ԡ�
/// 
/// \param [in] pAttr ָ�򴮿����ԽṹCOMM_ATTR��ָ�롣
/// \retval 0  �رճɹ�
/// \retval <0  �ر�ʧ��
int SLAVEUARTSetAttribute(SLAVEUART_ATTR *pAttr)
{
	 
	return 0;
}

/// ��ǰ������ݣ�һֱ�ȵ�����ָ�����ֽڻ����ʱ�ŷ��أ�����������
/// 
/// \param [out] pData ���ݻ����ָ�롣
/// \param [in] nBytes Ҫ�������ֽ���
/// \return>0  �������ֽ���
/// \retval <=0 ��ʧ��
int SLAVEUARTRead(void* pData, uint nBytes)
{
	 
	return 0;
}


/// дǰ������ݣ�һֱ�ȵ�д��ָ�����ֽڻ����ʱ�ŷ��أ�����������
/// 
/// \param [in] pData ���ݻ����ָ�롣
/// \param [in] nBytes Ҫд����ֽ���
/// \return>0  д����ֽ���
/// \retval <=0  дʧ��
int SLAVEUARTWrite(void* pData, uint nBytes)
{
	 
	return 0;
}

/// @} end of group


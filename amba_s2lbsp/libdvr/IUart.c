#include "Comm.h"
#include "IUart.h"
#include "ipc_inc.h"


/// ���������豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int UARTCreate(char *strDev){  return 0;}


/// ���������豸
/// 
//iFlag 1 ������������UART,����0 
//
int UARTCreateNew(char *strDev,int iFlag){  return 0;}


/// ���ٴ����豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int UARTDestory(int dev_fd){  return 0;}


/// ���ô������ԡ�
/// 
/// \param [in] pAttr ָ�򴮿����ԽṹCOMM_ATTR��ָ�롣
/// \retval 0  �رճɹ�
/// \retval <0  �ر�ʧ��
int UARTSetAttribute(int dev_fd, COMM_ATTR *pattr){  return 0;}



/// ��ȡ�������ԡ�
/// 
/// \param [in] pAttr ָ�򴮿����ԽṹCOMM_ATTR��ָ�롣
/// \retval 0  �رճɹ�
/// \retval <0  �ر�ʧ��

int UARTGetAttribute(int dev_fd){  return 0;}


/// ���������ݣ�һֱ�ȵ�����ָ�����ֽڻ����ʱ�ŷ��أ�����������
/// 
/// \param [out] pData ���ݻ����ָ�롣
/// \param [in] nBytes Ҫ�������ֽ���
/// \retval >0  �������ֽ���
/// \retval <=0 ��ʧ��
int UARTRead(void *pdata, uint nbytes, int dev_fd){  return 0;}


/// д�������ݣ�һֱ�ȵ�д��ָ�����ֽڻ����ʱ�ŷ��أ�����������
/// 
/// \param [in] pData ���ݻ����ָ�롣
/// \param [in] nBytes Ҫд����ֽ���
/// \retval >0  д����ֽ���
/// \retval <=0 дʧ��
int UARTWrite(void *pdata, uint nbytes, int dev_fd){  return 0;}


/// ��ֹ���ڶ�д�������������������塣
/// 
/// \param [in] dwFlags Ҫִ�еĲ���,�������±���һ������ֵ��
/// \retval 0  �����ɹ�
/// \retval <0  ����ʧ��
int UARTPurge(uint dw_flags ,int dev_fd){  return 0;}

/// @} end of group


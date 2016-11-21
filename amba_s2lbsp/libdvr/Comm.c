#include "Comm.h"
#include "ipc_inc.h"

/// \defgroup CommAPI API Comm
/// ��׼�Ĵ���ͨѶ�ӿڡ�
/// \n ��������ͼ:
/// \code
///    =========================================
///                   |                         
///              CommCreate                  
///       +-----------|	          
///       |     CommSetAttribute  	
///       |     CommAsConsole     	
///       |           |           	 
///       |      CommOpen    
///       | +---------|      
///       | |    CommRead    
///       | |    CommWrite   
///       | |    CommPurge   
///       | +---------|      	
///       |      CommClose   
///       +-----------|
///             CommDestory                  
///                   |                         
///    =========================================
/// \endcode
/// @{

/// ���������豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int CommCreate(void){  return 0;}


/// ���ٴ����豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int CommDestory(void){  return 0;}


/// �򿪴����豸��
/// 
/// \retval >0  �򿪳ɹ�
/// \retval <=0  ��ʧ��
int CommOpen(void){  return 0;}


/// �رմ����豸����ǰ��д�����������ء�
/// 
/// \retval 0  �رճɹ�
/// \retval <0  �ر�ʧ��
int CommClose(void){  return 0;}


/// ���ô������ԡ�
/// 
/// \param [in] pAttr ָ�򴮿����ԽṹCOMM_ATTR��ָ�롣
/// \retval 0  �رճɹ�
/// \retval <0  �ر�ʧ��
int CommSetAttribute(COMM_ATTR * pAttr)
{
	return 0; 
}


/// ���������ݣ�һֱ�ȵ�����ָ�����ֽڻ����ʱ�ŷ��أ�����������
/// 
/// \param [out] pData ���ݻ����ָ�롣
/// \param [in] nBytes Ҫ�������ֽ���
/// \retval >0  �������ֽ���
/// \retval <=0 ��ʧ��
int CommRead(void* pData, uint nBytes)
{
	return 0; 
}


/// д�������ݣ�һֱ�ȵ�д��ָ�����ֽڻ����ʱ�ŷ��أ�����������
/// 
/// \param [in] pData ���ݻ����ָ�롣
/// \param [in] nBytes Ҫд����ֽ���
/// \retval >0  д����ֽ���
/// \retval <=0 дʧ��
int CommWrite(void* pData, uint nBytes)
{
	return 0; 
}


/// ��ֹ���ڶ�д�������������������塣
/// 
/// \param [in] dwFlags Ҫִ�еĲ���,�������±���һ������ֵ��
/// \retval 0  �����ɹ�
/// \retval <0  ����ʧ��
int CommPurge(uint dwFlags)
{
	return 0; 
}


/// ������Ϊ����̨
/// 
/// \param [in] flag ��־��0��ʾ����Ϊ���ڣ�1��ʾ��Ϊ���ڡ�
/// \retval 0  �����ɹ�
/// \retval <0  ����ʧ��
int CommAsConsole(int flag)
{
	return 0; 
}


/// �õ������Ƿ���Ϊ����̨ʹ��
void CommGetConsol(int* console)
{
}

/// @} end of group


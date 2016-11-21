
#ifndef __UART2_API_H__
#define __UART2_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif


/// �������Խṹ
typedef struct UART2_ATTR
{
    uint    baudrate;    ///< ʵ�ʵĲ�����ֵ��        
    uchar    databits;    ///< ʵ�ʵ�����λ����    
    uchar    parity;        ///< ��żУ��ѡ�ȡcomm_parity_t���͵�ö��ֵ��    
    uchar    stopbits;    ///< ֹͣλ����ȡcomm_stopbits_t���͵�ö��ֵ��    
    uchar    reserved;    ///< ����    
} UART2_ATTR;


//��UART2Create ����֮ǰ������Ч
//Ĭ��Ϊ����ģʽ
//iFlag 1 ������0,����
int UART2SetNonBlock(int iFlag);

/// ����ǰ����豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int UART2Create(void);


/// ����ǰ����豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int UART2Destory(void);


/// ���ô������ԡ�
/// 
/// \param [in] pAttr ָ�򴮿����ԽṹCOMM_ATTR��ָ�롣
/// \retval 0  �رճɹ�
/// \retval <0  �ر�ʧ��
int UART2SetAttribute(UART2_ATTR *pAttr);

/// ��ǰ������ݣ�һֱ�ȵ�����ָ�����ֽڻ����ʱ�ŷ��أ�����������
/// 
/// \param [out] pData ���ݻ����ָ�롣
/// \param [in] nBytes Ҫ�������ֽ���
/// \return>0  �������ֽ���
/// \retval <=0 ��ʧ��
int UART2Read(void* pData, uint nBytes);


/// дǰ������ݣ�һֱ�ȵ�д��ָ�����ֽڻ����ʱ�ŷ��أ�����������
/// 
/// \param [in] pData ���ݻ����ָ�롣
/// \param [in] nBytes Ҫд����ֽ���
/// \return>0  д����ֽ���
/// \retval <=0  дʧ��
int UART2Write(void* pData, uint nBytes);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__UART2_API_H__


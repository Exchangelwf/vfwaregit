
#ifndef __FUNCOM_API_H__
#define __FUNCOM_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif

/// �������Խṹ
typedef struct FUNCOM_ATTR
 {
	uint	baudrate;	///< ʵ�ʵĲ�����ֵ��		
	uchar	databits;	///< ʵ�ʵ�����λ����	
	uchar	parity;		///< ��żУ��ѡ�ȡcomm_parity_t���͵�ö��ֵ��	
	uchar	stopbits;	///< ֹͣλ����ȡcomm_stopbits_t���͵�ö��ֵ��	
	uchar	reserved;	///< ����	
} FUNCOM_ATTR;

//��UART2Create ����֮ǰ������Ч
//Ĭ��Ϊ����ģʽ
//iFlag 1 ������0,����
int SetBlockComm(int iComID,int iFlag);

/// ����ǰ����豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int CreateComm(int iComID);


/// ����ǰ����豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int DestoryComm(int iComID);


/// ���ô������ԡ�
/// 
/// \param [in] pAttr ָ�򴮿����ԽṹCOMM_ATTR��ָ�롣
/// \retval 0  �رճɹ�
/// \retval <0  �ر�ʧ��
int SetAttributeComm(int iComID,FUNCOM_ATTR *pAttr);

/// ��ǰ������ݣ�һֱ�ȵ�����ָ�����ֽڻ����ʱ�ŷ��أ�����������
/// 
/// \param [out] pData ���ݻ����ָ�롣
/// \param [in] nBytes Ҫ�������ֽ���
/// \return>0  �������ֽ���
/// \retval <=0 ��ʧ��
int ReadComm(int iComID,void *pdata, uint nbytes);


/// дǰ������ݣ�һֱ�ȵ�д��ָ�����ֽڻ����ʱ�ŷ��أ�����������
/// 
/// \param [in] pData ���ݻ����ָ�롣
/// \param [in] nBytes Ҫд����ֽ���
/// \return>0  д����ֽ���
/// \retval <=0  дʧ��
int WriteComm(int iComID,void *pdata, uint len);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__UART2_API_H__


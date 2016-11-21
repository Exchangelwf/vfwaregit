

#ifndef __PTZ_API_H__
#define __PTZ_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup PTZAPI API PTZ
/// ��̨�豸ͨѶ�ӿڡ�
/// \n ��������ͼ:
/// \code
///    =========================================
///                   |                         
///              PtzCreate                  
///       +-----------|	          
///       |     PtzSetAttribute  	
///       |           |           	 
///       |      PtzOpen    
///       | +---------|      
///       | |    PtzRead    
///       | |    PtzWrite   
///       | +---------|      	
///       |      PtzClose   
///       +-----------|
///             PtzDestory                  
///                   |                         
///    =========================================
/// \endcode
/// @{

/// ��̨�豸����
typedef struct PTZ_ATTR
 {
	uint	baudrate;		
	uchar	databits;
	uchar	parity;
	uchar	stopbits;	
	uchar	reserved;
} PTZ_ATTR;


//��PtzCreate ����֮ǰ������Ч
//Ĭ��Ϊ����ģʽ
//iFlag 1 ������0,����
int PtzSetNonBlock(int iFlag);

/// ������̨�豸
/// 
/// \param	��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int PtzCreate(void);


/// ������̨�豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int PtzDestory(void);

	
/// ����̨�豸��
/// 
/// \param ��
/// \retval 0  �򿪳ɹ�
/// \retval <0  ��ʧ��
int PtzOpen(void);


/// ������̨�豸���ԡ�
/// 
/// \param [in] pattr ��̨���ԽṹPTZ_ATTRָ�롣
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int PtzSetAttribute(PTZ_ATTR *pattr);


/// �ر���̨�豸����ǰ��д�����������ء�
/// 
/// \param ��
/// \retval 0  �رճɹ�
/// \retval <0  �ر�ʧ��
int PtzClose(void);


/// д��̨�������һֱ�ȵ�д��ָ�����ֽڻ����ʱ�ŷ��أ�����������
/// 
/// \param [in] pData ���ݻ����ָ�롣
/// \param [in] len Ҫд����ֽ���
/// \retval >0  д����ֽ���
/// \retval <=0 дʧ��
int PtzWrite(void* pData, uint len);


/// ����̨���ݣ�һֱ�ȵ�����ָ�����ֽڻ����ʱ�ŷ��أ�����������
/// 
/// \param [in] pData ���ݻ����ָ�롣
/// \param [in] len Ҫ�������ֽ���
/// \return >0  �������ֽ���
/// \retval <=0 ��ʧ��
int PtzRead(void* pData, uint len);

// ������������ʹ��
int PtzUpdate(void* pData,unsigned long len);

/// �����첽ͨѶ��չ
typedef struct UartExtendOperate
{
	int (*Open)();
	int (*Close)();
	int (*SetAttribute)(PTZ_ATTR *);
	int (*Read)(void *, uint);
	int (*Write)(void *, uint);
	int (*Ioctl)();
}UART_EXTEND;

//������� ��������ʡ��
//int UartExtendWrite(UART_EXTEND *, void *pdata, uint len);
//int UartExtendRead(UART_EXTEND *, void *pdata, uint len);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__PTZ_API_H__



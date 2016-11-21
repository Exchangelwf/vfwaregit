
#ifndef __COMM_API_H__
#define __COMM_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif

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

#define COMM_PURGE_TXABORT			0x0001	///< ��ֹд����
#define COMM_PURGE_RXABORT			0x0002	///< ��ֹ������
#define COMM_PURGE_TXCLEAR			0x0004	///< ����������
#define COMM_PURGE_RXCLEAR			0x0008	///< ������뻺��

/// �������Խṹ
typedef struct COMM_ATTR
 {
	uint	baudrate;	///< ʵ�ʵĲ�����ֵ��		
	uchar	databits;	///< ʵ�ʵ�����λ����	
	uchar	parity;		///< ��żУ��ѡ�ȡcomm_parity_t���͵�ö��ֵ��	
	uchar	stopbits;	///< ֹͣλ����ȡcomm_stopbits_t���͵�ö��ֵ��	
	uchar	reserved;	///< ����	
} COMM_ATTR;

/// ����ֹͣλ����
enum comm_stopbits_t 
{
	COMM_ONESTOPBIT,	///< 1 stop bit
	COMM_ONE5STOPBITS,	///< 1.5 stop bit
	COMM_TWOSTOPBITS	///< 2 stop bit
};

/// ����У��λ����
enum comm_parity_t 
{
	COMM_NOPARITY,	///< No parity
	COMM_ODDPARITY,	///< Odd parity
	COMM_EVENPARITY,///< Even parity
	COMM_MARK,		///< 
	COMM_SPACE		///< 
};


/// ���������豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int CommCreate(void);


/// ���ٴ����豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int CommDestory(void);


/// �򿪴����豸��
/// 
/// \retval >0  �򿪳ɹ�
/// \retval <=0  ��ʧ��
int CommOpen(void);


/// �رմ����豸����ǰ��д�����������ء�
/// 
/// \retval 0  �رճɹ�
/// \retval <0  �ر�ʧ��
int CommClose(void);


/// ���ô������ԡ�
/// 
/// \param [in] pAttr ָ�򴮿����ԽṹCOMM_ATTR��ָ�롣
/// \retval 0  �رճɹ�
/// \retval <0  �ر�ʧ��
int CommSetAttribute(COMM_ATTR * pAttr);


/// ���������ݣ�һֱ�ȵ�����ָ�����ֽڻ����ʱ�ŷ��أ�����������
/// 
/// \param [out] pData ���ݻ����ָ�롣
/// \param [in] nBytes Ҫ�������ֽ���
/// \retval >0  �������ֽ���
/// \retval <=0 ��ʧ��
int CommRead(void* pData, uint nBytes);


/// д�������ݣ�һֱ�ȵ�д��ָ�����ֽڻ����ʱ�ŷ��أ�����������
/// 
/// \param [in] pData ���ݻ����ָ�롣
/// \param [in] nBytes Ҫд����ֽ���
/// \retval >0  д����ֽ���
/// \retval <=0 дʧ��
int CommWrite(void* pData, uint nBytes);


/// ��ֹ���ڶ�д�������������������塣
/// 
/// \param [in] dwFlags Ҫִ�еĲ���,�������±���һ������ֵ��
/// \retval 0  �����ɹ�
/// \retval <0  ����ʧ��
int CommPurge(uint dwFlags);


/// ������Ϊ����̨
/// 
/// \param [in] flag ��־��0��ʾ����Ϊ���ڣ�1��ʾ��Ϊ���ڡ�
/// \retval 0  �����ɹ�
/// \retval <0  ����ʧ��
int CommAsConsole(int flag);


/// �õ������Ƿ���Ϊ����̨ʹ��
void CommGetConsol(int* console);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__COMM_API_H__


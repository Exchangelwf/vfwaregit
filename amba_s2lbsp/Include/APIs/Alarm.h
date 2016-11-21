
#ifndef __ALARM_API_H__
#define __ALARM_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup AlarmAPI API Alarm
/// ������������˿ڣ����ڣ���ʹ�ã������ⲿ��ƽ������ߴ����̵��������ⲿ��·��
///	\n ��������ͼ:
/// \code
///    ===========================
///                |
///   *AlarmGetInSlots,AlarmGetOutSlots
///                |
///           AlarmCreate
///                |------------+
///       AlarmRead,AlarmWrite  |
///                |------------+
///           AlarmDestory
///                |
///    ===========================
/// \endcode
/// @{


/// �õ������˿ڵ�����ͨ����Ŀ��
/// \return �����˿ڵ�����ͨ����Ŀ
int AlarmGetInSlots(void);

/// �õ������˿ڵ����ͨ����Ŀ��
/// \return �����˿ڵ����ͨ����Ŀ
int AlarmGetOutSlots(void);

/// ���������豸
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int AlarmCreate(void);

/// ���ٱ����豸
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int AlarmDestory(void);

/// ����������״̬��״̬�ı�ʱ�����ʱ���أ�����������
/// \param [out] pData ָ��һ��uintֵ��ָ�룬uintֵ�Ǹ�����������ͨ����ƽ״̬�����롣
///	       ��ͨ���ڵ�λ����ͨ���ڸ�λ���ߵ�ƽ��1���͵�ƽ��0�������ڵ�ͨ����0��
/// \retval >0  �������ֽ���
/// \retval <=0 ��ʧ��
int AlarmRead(uint* pData);

/// д��ȡ�������״̬��
/// \param [in] pData ָ��һ��uintֵ��ָ�룬uintֵ�Ǹ����������ͨ�������롣��ͨ����
///	       ��λ����ͨ���ڸ�λ����1����ߵ�ƽ����0����͵�ƽ�������ڵ�ͨ��λ��Ч��
/// \return >0  д����ֽ���
/// \retval <=0 дʧ��
//---------------------------//
//tiger//start
int ResetRead(uint* pData);
int C2BRead(uint * pData);
int reset_in_read(unsigned int * pdata);
int c2b_in_read(unsigned int * pdata);
int tiger_reset_in_read(void);
//tiger//stop
//---------------------------//
int AlarmWrite(uint* pData);
/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__ALARM_API_H__


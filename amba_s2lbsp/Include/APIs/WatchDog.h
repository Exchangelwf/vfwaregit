

#ifndef __WATCHDOG_API_H__
#define __WATCHDOG_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup WatchDogAPI API Watch Dog
/// ���Ź�API����Ӳ�������ʵ��ϵͳ�Ĺ��ϴ���.
/// @{

/// �������Ź��豸
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int WatchdogCreate(void);

/// ���ٿ��Ź��豸
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int WatchdogDestory(void);

/// 
int WatchdogClear(void);

/// 
int WatchdogRead(void* pData, uint nBytes);

///
int WatchdogWrite(void* pdata, uint nBytes);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__WATCHDOG_API_H__


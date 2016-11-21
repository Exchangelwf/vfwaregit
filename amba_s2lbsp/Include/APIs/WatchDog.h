

#ifndef __WATCHDOG_API_H__
#define __WATCHDOG_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup WatchDogAPI API Watch Dog
/// 看门狗API，软硬件配合来实现系统的故障处理.
/// @{

/// 创建看门狗设备
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int WatchdogCreate(void);

/// 销毁看门狗设备
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
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


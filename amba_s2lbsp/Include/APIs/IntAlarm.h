

#ifndef __INTELLIGENT_ALARM_API_H__
#define __INTELLIGENT_ALARM_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif

/// \def group Intelligent Alarm API API Intelligent Alarm
/// 智能报警接口。
///	\n 调用流程图:
/// \code
///    ===========================
///                |
///        *IntelligentAlarmGetCaps
///                |
///         IntelligentAlarmCreate
///    +-----------|
///    |   IntelligentAlarmGetEvent
///    |   IntelligentAlarmSetState
///    +-----------|
///       IntelligentAlarmDestory
///                |
///    ===========================
/// \endcode
/// @{

/// 智能报警检测设备特性结构
typedef struct Intelligent_Alarm_CAPS
{
	uint number;		///< 置n表示支持智能报警检测，置0表示不支持智能报警检测。
}Intelligent_Alarm_CAPS;


/// 创建智能报警检测设备
/// 
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int IntelligentAlarmCreate(void);


/// 销毁智能报警检测设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int IntelligentAlarmDestory(void);


/// 执行智能报警日志检测。立即返回。
/// 
/// \param [in] serverip 指向的savia设备的ip
/// \param [out] pData 事件类型的指针，按位从左到右表示8种报警事件是否产生。1表示产生，0表示没有。
/// \retval 0  检测成功
/// \retval <0  检测失败
int IntelligentAlarmGetEvent(IPADDR serverip, int *pData);


/// 执行智能报警状态设置。立即返回。
/// 
/// \param [in] serverip 指向的savia设备的ip
/// \param [in] State 报警类型，按位表示是否分别启用8种算法。1表示启用，0表示停止。
/// \retval 0  设置成功
/// \retval <0  设置失败
int IntelligentAlarmSetState(IPADDR ip, int State);


/// 得到智能报警支持的特性。
/// 
/// \param [out] pCaps 指向智能报警特性结构Intelligent_Alarm _CAPS的指针。
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int IntelligentAlarmGetCaps(Intelligent_Alarm_CAPS * pCaps);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__Intelligent_Alarm_API_H__


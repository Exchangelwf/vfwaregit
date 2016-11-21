

#ifndef __APPLICATION_GUARD_API_H__
#define __APPLICATION_GUARD_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup ApplicationAPI API Application
/// 应用程序控制API。通过这些接口可以单独的重启或关闭应用程序而不影响操作系统的
///	运行。
/// @{


/// 退出正在运行的应用程序，并重新运行。
void ApplicationRestart(void);


/// 退出正在运行的应用程序。
void ApplicationExit(void);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__APPLICATION_GUARD_API_H__


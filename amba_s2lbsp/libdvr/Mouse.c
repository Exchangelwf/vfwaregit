#include "Mouse.h"
#include "ipc_inc.h"

/// \defgroup MouseAPI API Mouse
/// 鼠标和触摸屏设备API。
///	\n 调用流程图:
/// \code
///    ===========================
///                |
///           MouseCreate
///                |--------+
///           MouseGetData  |
///                |--------+
///           MouseDestory
///                |
///    ===========================
/// \endcode
/// @{



/// 创建鼠标设备
/// 
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int MouseCreate(void){  return 0;}


/// 销毁鼠标设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int MouseDestory(void){  return 0;}


/// 一直等到读到一个完整的鼠标事件才返回，否则阻塞。如果鼠标外设被拔掉或不存在，
/// 立即返回错误。出错时应用程序需要定时重试，周期不小于500ms，以实现热插拔功能。
/// 
/// \param [out] pData 指向鼠标时间结构MOUSE_DATA的指针。
/// \retval 0 读成功
/// \retval <0 读失败
int MouseGetData(MOUSE_DATA * pData){  return 0;}

/// @} end of group


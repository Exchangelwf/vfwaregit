
#include "LossDetect.h"
#include "ipc_inc.h"


/// \defgroup LossDetectAPI API Loss Detect
/// 视频丢失接口。
///	\n 调用流程图:
/// \code
///    ===========================
///                |
///        *LossDetectGetCaps
///                |
///         LossDetectCreate
///    +-----------|
///    |   LossDetectGetState
///    +-----------|
///       LossDetectDestory
///                |
///    ===========================
/// \endcode
/// @{


/// 创建视频丢失检测设备
/// 
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int LossDetectCreate(void){  return 0;}


/// 销毁视频丢失检测设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int LossDetectDestory(void){  return 0;}


/// 执行视频丢失检测。立即返回。
/// 
/// \param [out] pData 指向一个uint值的指针，uint值是各个报警输入通道电平状态
///        的掩码。低通道在低位，高通道在高位。高电平置1，低电平置0，不存在的通
///        道置0。
/// \retval 0  检测成功
/// \retval <0  检测失败
int LossDetectGetState(uint* pData)
{
	return 0;
}


/// 得到视频丢失支持的特性。
/// 
/// \param [out] pCaps 指向视频丢失特性结构LOSS_DETECT _CAPS的指针。
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int LossDetectGetCaps(LOSS_DETECT_CAPS * pCaps){  return 0;}

/// @} end of group


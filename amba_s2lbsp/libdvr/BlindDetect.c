#include "BlindDetect.h"
#include "ipc_inc.h"

/// \defgroup BlindDetectAPI API Blind Detect
/// 全屏范围按灵敏度的遮挡检测接口。
///	\n 调用流程图:
/// \code
///    ===========================
///                |
///        *BlindDetectGetCaps
///                |
///         BlindDetectCreate
///    +-----------|
///    |   BlindDetectGetState
///    |   BlindDetectSetParameter
///    +-----------|
///       BlindDetectDestory
///                |
///    ===========================
/// \endcode
/// @{

/// 创建黑屏检测设备
/// 
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int BlindDetectCreate(void)
{
	return 0; 
}


/// 销毁黑屏检测设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int BlindDetectDestory(void)
{
	return 0; 
}


/// 执行黑屏检测。立即返回。
/// 
/// \param [out] pData 指向一个uint值的指针，uint值是各个报警输入通道电平状态的
///       掩码。低通道在低位，高通道在高位。高电平置1，低电平置0，不存在的通道置0。
/// \retval 0  检测成功
/// \retval <0  检测失败
int BlindDetectGetState(uint* pData)
{
	return 0;
}


/// 设置黑屏检测捕获的参数。
/// 
/// \param [in] channel 通道号。
/// \param [in] pParam 指向黑屏检测参数结构BLIND_DETECT_ PARAM的指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int BlindDetectSetParameter(int channel, BLIND_DETECT_PARAM *pParam)
{
	return 0; 
}


/// 得到黑屏支持的特性。
/// 
/// \param [out] pCaps 指向黑屏特性结构BLIND_DETECT _CAPS的指针。
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int BlindDetectGetCaps(BLIND_DETECT_CAPS * pCaps)
{
	return 0; 
}

/// @} end of group


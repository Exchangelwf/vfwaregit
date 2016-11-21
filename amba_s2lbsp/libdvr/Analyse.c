#include "Analyse.h"
#include "ipc_inc.h"

/******************************************************************/
/// 智能分析的接口函数
/******************************************************************/

/// 获得智能分析能力。
/// 
/// \param [out] pCaps 指向智能分析能力结构体ANALYSE_DETECT_CAPS
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int AnalyseDetectGetCaps(ANALYSE_DETECT_CAPS * pCaps)
{
	return 0; 
}

/// 创建智能检测设备
/// 
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int AnalyseDetectCreate(void)
{
	return 0; 
}


/// 销毁智能检测设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int AnalyseDetectDestory(void)
{
	return 0; 
}
/// 智能检测结果
/// 
/// \param [in] channel 通道号。
/// \param [in] pResult 指向智能检测参数结构ANALYSE_DETECT_RESULT的指针。
///	type智能报警类型
/// \retval 0  获取成功
/// \retval <0  获取失败
int AnalyseDetectGetResult(int channel, INTELLIGENTTYPE type,ANALYSE_DETECT_RESULT *pResult)
{
	return 0; 
}



/// 配置智能检测通道参数
/// 
/// \param [in] channel 通道号。
/// \param [in] pParam 指向智能检测参数结构ANALYSE_DETECT_PARAM的指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int AnalyseDetectSetParameter(int channel, INTELLIGENTTYPE type,ANALYSE_DETECT_PARAM *pParam)
{
	return 0; 
}


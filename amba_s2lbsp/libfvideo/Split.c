#include "Split.h"
#include "ipc_inc.h"

/// \defgroup SplitAPI API Split
/// 画面分割接口，对未压缩的视频输入进行缩放及Z序排列后输出。
///	\n 调用流程图:
/// \code
///    ================================
///                |
///          SplitGetCaps
///                |
///           SplitCreate
///                |-------------+
///          SplitSetVstd        |
///          SplitCombine        |
///        SplitGetRect          |
///       SplitGetBlankRects     |
///       SplitGetFullRect       |
///                |-------------+
///          SplitDestory
///                |
///    ================================
/// \endcode
/// @{



/// 创建画面分割设备
/// 
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int SplitCreate(void){  return 0;}
int Split2Create(int index){  return 0;}


/// 销毁画面分割设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int SplitDestory(void){  return 0;}
int Split2Destroy(int index){  return 0;}


/// 设置视频制式。
/// 
/// \param [in] dwStandard 视频制式，取video_standard_t类型的值。
/// \retval 0  设置成功
/// \retval <0  设置失败
int SplitSetVstd(uint dwStandard){  return 0;}	
int Split2SetVstd(int index, uint dwStandard){  return 0;}	


/// 对监视通道的输出进行排列组合。可以由多个通道的视频输出组合成一个输出画面。
/// 
/// \param [in] dwMode 组合中的通道数目，取splitt类型的值。
/// \param [in] dwIndex 当前通道数目下组合方式的序号。
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int SplitCombine(uint dwMode, uint dwIndex){  return 0;}
int Split2Combine(int index,  uint dwMode, uint dwIndex){  return 0;}


/// 得到各个通道画面在当前分割模式下的输出区域。
/// 
/// \param [in] channel 通道号。
/// \param [out] pRect 指向视频区域结构VIDEO_RECT的指针，采用图形坐标体系。
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int SplitGetRect(int channel, VIDEO_RECT * pRect){  return 0;}
int Split2GetRect(int index,  int channel, VIDEO_RECT * pRect){  return 0;}


/// 得到空闲的画面区域， 一次性返回全部的空闲区域。比如9画面下只有8个画面是需要
/// 显示视频的， 还有一个是空的。又比如两路的机器切换到4画面时， 有两个画面也是
/// 空的。
/// 
/// \param [out] pRect 指向视频区域结构VIDEO_RECT的数组。应用程序需要开足够的空间
///        来接收输出的区域，可以把大小为当前分割模式下所有画面数的数组指针传进来，
/// 	   采用图形坐标体系。
/// \retval <0   获取失败。
/// \retval >=0  获得的空闲画面数。
int SplitGetBlankRects(VIDEO_RECT * pRect){  return 0;}
int Split2GetBlankRects(int index,  VIDEO_RECT * pRect){  return 0;}


/// 得到当前分割模式下整个视频的区域, 如果是单画面, 得到的就是单画面区域, 如果是
/// 9画面,得到的就是9画面的区域, 等等。
/// 
/// \param [out] pRect 指向视频区域结构VIDEO_RECT的指针，采用图形坐标体系。
/// \retval <0   获取失败。
/// \retval >=0  获得的空闲画面数。
int SplitGetFullRect(VIDEO_RECT * pRect){  return 0;}
int Split2GetFullRect(int index,  VIDEO_RECT * pRect){  return 0;}


/// 得到分割支持的特性。
/// 
/// \param [out] pCaps 指向分割特性结构SPLIT_CAPS的指针。
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int SplitGetCaps(SPLIT_CAPS * pCaps){  return 0;}
int Split2GetCaps(int index, SPLIT_CAPS * pCaps){  return 0;}
//同种分割模式下，图片位置对调显示
//Channel1，Channel2分别是待对调位置的视频输入通道
//注意这里的视频输入通道以0开始计数
#ifdef _SURPORT_SWITCH_
 int SplitSwitch(uint Channel1,uint Channel2){  return 0;}
#endif

 /*
  * @brief 控制预览是否显示
  * @param show-true:显示， false-不显示，默认为true
  */
int SplitShow(int channel, VD_BOOL bShow){  return 0;}
VD_BOOL SplitGetShowFlag(int channel){  return 0;}
/// @} end of group

int SplitPause(int channel){  return 0;}
int SplitResum(int channel){  return 0;}
int SplitZoom(int channel,VIDEO_RECT *pSourceRect, VIDEO_RECT *pDestRect){  return 0;}


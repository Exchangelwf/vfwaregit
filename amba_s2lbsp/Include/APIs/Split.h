

#ifndef __SPLIT_API_H__
#define __SPLIT_API_H__

#include "DVRDEF.H"
#include "Video.h"

#ifdef __cplusplus
extern "C" {
#endif

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

/// 画面分割设备特性
typedef struct SPLIT_CAPS
{
	/// 支持的通道组合方式的掩码，位序号对应枚举类型splitt的每一个值。置1表示
	/// 支持该枚举值对应的特性，置0表示不支持。
	uint dwCombine;
}SPLIT_CAPS;

/// 画面组合类型
enum split_combine_t
{
	/// 只输出一个通道，组合序号对应的就是输出的通道号。
	SPLIT1,

	/// 同时输出两个通道，组合序号0输出0-1通道，组合序号1输出2-3通道，依次类推。
	///	\n 示意图（矩形内数字表示视屏输入通道号）:
	/// \code
	/// +-------------+
	/// |------+------|
	/// | 1    |   2  |
	/// |------+------|
	/// +-------------+
	/// \endcode
	SPLIT2,	
	
	/// 同时输出2*2四个通道，组合序号0输出0-3通道，组合序号1输出4-7通道，依次类推。
	SPLIT4,
	
	/// 同时输出八个通道，组合序号0-7都是输出0-7通道，只是组合序号对应的通道大
	/// 画面显示，其余的小画面显示。组合序号8-15都是输出8-15通道，只是组合序号
	/// 减8对应的通道大画面显示，其余的小画面显示，依次类推。
	///	\n 示意图（矩形内数字表示视屏输入通道号）:
	/// \code
	/// +--------------------+------+
	/// |                    |  2   |
	/// |                    |------|
	/// |         1          |  3   |
	/// |                    |------|
	/// |                    |  4   |
	/// |------+------+------|------|
	/// |  8   |  7   |  6   |  5   |
	/// +------+------+------+------+
	/// \endcode
	SPLIT8,

	/// 同时输出3*3九个通道，画面上九个网格中央的那个保留，组合序号0输出0-7通道，
	/// 组合序号1输出8-15通道，依次类推。
	SPLIT9,	

	/// 同时输出4*4十六个通道，组合序号0输出0-15通道，组合序号1输出16-31通道，依次
	/// 类推。
	SPLIT16,

	/// 画中画模式，一个全屏大画面，加一个小画面窗口。
	///	\n 示意图（矩形内数字表示视屏输入通道号）:
	/// \code
	/// +-------------+
	/// | 1           |
	/// |      +------|
	/// |      |   2  |
	/// +------+------+
	/// \endcode
	SPLITPIP,

	/// 同时输出6个画面，一个大画面，5个小画面。参考8画面示意图
	SPLIT6,

	SPLIT12,
	//#ifdef _SPLIT_VR98XX_EXTENDED_
	SPLIT13,	
	SPLIT20,
	SPLIT25,
	SPLIT33,
	SPLIT5,
	//#endif
	/// 枚举的分割方式数目。
	SPLIT_NR,		
};


/// 创建画面分割设备
/// 
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int SplitCreate(void);
int Split2Create(int index);


/// 销毁画面分割设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int SplitDestory(void);
int Split2Destroy(int index);


/// 设置视频制式。
/// 
/// \param [in] dwStandard 视频制式，取video_standard_t类型的值。
/// \retval 0  设置成功
/// \retval <0  设置失败
int SplitSetVstd(uint dwStandard);	
int Split2SetVstd(int index, uint dwStandard);	


/// 对监视通道的输出进行排列组合。可以由多个通道的视频输出组合成一个输出画面。
/// 
/// \param [in] dwMode 组合中的通道数目，取splitt类型的值。
/// \param [in] dwIndex 当前通道数目下组合方式的序号。
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int SplitCombine(uint dwMode, uint dwIndex);
int Split2Combine(int index,  uint dwMode, uint dwIndex);


/// 得到各个通道画面在当前分割模式下的输出区域。
/// 
/// \param [in] channel 通道号。
/// \param [out] pRect 指向视频区域结构VIDEO_RECT的指针，采用图形坐标体系。
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int SplitGetRect(int channel, VIDEO_RECT * pRect);
int Split2GetRect(int index,  int channel, VIDEO_RECT * pRect);


/// 得到空闲的画面区域， 一次性返回全部的空闲区域。比如9画面下只有8个画面是需要
/// 显示视频的， 还有一个是空的。又比如两路的机器切换到4画面时， 有两个画面也是
/// 空的。
/// 
/// \param [out] pRect 指向视频区域结构VIDEO_RECT的数组。应用程序需要开足够的空间
///        来接收输出的区域，可以把大小为当前分割模式下所有画面数的数组指针传进来，
/// 	   采用图形坐标体系。
/// \retval <0   获取失败。
/// \retval >=0  获得的空闲画面数。
int SplitGetBlankRects(VIDEO_RECT * pRect);
int Split2GetBlankRects(int index,  VIDEO_RECT * pRect);


/// 得到当前分割模式下整个视频的区域, 如果是单画面, 得到的就是单画面区域, 如果是
/// 9画面,得到的就是9画面的区域, 等等。
/// 
/// \param [out] pRect 指向视频区域结构VIDEO_RECT的指针，采用图形坐标体系。
/// \retval <0   获取失败。
/// \retval >=0  获得的空闲画面数。
int SplitGetFullRect(VIDEO_RECT * pRect);
int Split2GetFullRect(int index,  VIDEO_RECT * pRect);


/// 得到分割支持的特性。
/// 
/// \param [out] pCaps 指向分割特性结构SPLIT_CAPS的指针。
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int SplitGetCaps(SPLIT_CAPS * pCaps);
int Split2GetCaps(int index, SPLIT_CAPS * pCaps);
//同种分割模式下，图片位置对调显示
//Channel1，Channel2分别是待对调位置的视频输入通道
//注意这里的视频输入通道以0开始计数
#ifdef _SURPORT_SWITCH_
 int SplitSwitch(uint Channel1,uint Channel2);
#endif

 /*
  * @brief 控制预览是否显示
  * @param show-true:显示， false-不显示，默认为true
  */
int SplitShow(int channel, VD_BOOL bShow);
VD_BOOL SplitGetShowFlag(int channel);
/// @} end of group

int SplitPause(int channel);
int SplitResum(int channel);
int SplitZoom(int channel,VIDEO_RECT *pSourceRect, VIDEO_RECT *pDestRect);

#ifdef __cplusplus
}
#endif

#endif //__SPLIT_API_H__


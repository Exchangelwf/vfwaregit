
#ifndef __VN_ANALYSE_API_H__
#define __VN_ANALYSE_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************/
//智能报警类型---最多支持32中智能算法，够多了
/******************************************************************/
typedef enum _IntelligentType
{
	INTELLIGENT_RED,//红色报警							
	INTELLIGENT_LINE ,	// 触线警报
	INTELLIGENT_DIRECT ,	// 逆向警报
	INTELLIGENT_ZOOM_RECT ,// 矩形周界入侵警报
	INTELLIGENT_ZOOM_CIRCLE 	,// 圆形周界入侵警报
	INTELLIGENT_ZOOM_POLYGON ,	// 多边形周界入侵警报
	INTELLIGENT_ZOOM_POLY_PT ,// 多边形周界入侵警报(对象中心点)
	INTELLIGENT_VECTOR_CROSS_DIRECT ,// 矢量相交方向
	INTELLIGENT_LINGER ,// 运动徘徊警报		
	INTELLIGENT_NR=32
}INTELLIGENTTYPE;

/******************************************************************/
/// 芯片智能分析能力集
/******************************************************************/
//这里应该反应智能算法的能力，如支持何种智能分析算法， 可以分析几路视频
typedef struct ANALYSE_DETECT_CAPS
{
	//支持的智能算法掩码，按位取INTELLIGENTTYPE值
	uint	intelligent;
	//每块芯片可分析的视频路数
	uchar chnnum[INTELLIGENT_NR];
	/// 保留
	uint	reserve;
}ANALYSE_DETECT_CAPS;

/******************************************************************/
/// 智能分析参数
/******************************************************************/
//各种智能算法共用这个参数，使用联合结构体，有需要就一直扩充
//------------------红色报警---------------------------------//
//红色报警
#define MAX_RED_RIGION_NUM  8		///< 单个通道最大智能检测区域数
#define  MAX_RED_ALARM_LEVEL 8	///< 报警等级数
#define MAX_POINT_NUM 	4			///< 确定一个区域所需点数
typedef struct _ANALYSE_REGION
{
	VD_POINT point[MAX_POINT_NUM];
}ANALYSE_REGION;
typedef struct _RED_PARA
{
	//	红色报警使能
	int 		enable;
	//   采集通道分析
	int 		channel;
	///红色报警档次
	int		iLevel;	
	/// 灵敏度的具体值
	int		sensitiveness;
	///实际区域个数
	int		 num;
	/// 智能检测的区域
	ANALYSE_REGION region[MAX_RED_RIGION_NUM];
	/// 保留值
	int		reserve[4];
}RED_PARA;
//------------------触线报警---------------------------------//
//触线报警
typedef struct _LINE_PARA
{
	//	触线报警使能
	int 		enable;
	//   采集通道分析
	int 		channel;
	//两点确定一条直线
	VD_POINT point[2];
	//  保留值
	int		reserve[4];
}LINE_PARA;
//------------------逆向报警---------------------------------//
//逆向报警
typedef struct _DIRECT_PARA
{
}DIRECT_PARA;
//------------------矩形区域入侵报警---------------------//
//矩形区域入侵
typedef struct _RECT_PARA
{
}RECT_PARA;
//------------------矢量相交报警----------------------------//
//矢量相交
typedef struct _CROSS_PARA
{
}CROSS_PARA;
//------------------运动徘徊报警----------------------------//
//运动徘徊
typedef struct _LINGER_PARA
{
}LINGER_PARA;
//-------------------结束----------------------------------------//
union ANALYSE_PARA
{
	RED_PARA red_param;
	LINE_PARA line_param;
	DIRECT_PARA direct_param;
	RECT_PARA rect_param;
	CROSS_PARA cross_param;
	LINGER_PARA linger_param;
};
typedef struct _ANALYSE_DETECT_PARAM
{
	union ANALYSE_PARA param;
}ANALYSE_DETECT_PARAM;

/******************************************************************/
/// 智能分析结果
/******************************************************************/
typedef struct _RED_RESULT
{
//按位表示采集通道的红色报警状态
	uint red_state;
	uint warning_level;
}RED_RESULT;
//------------------触线报警---------------------------------//
//触线报警
typedef struct _LINE_RESULT
{
	uint line_state;
}LINE_RESULT;
//------------------逆向报警---------------------------------//
//逆向报警
typedef struct _DIRECT_RESULT
{
	uint direct_state;
}DIRECT_RESULT;
//------------------矩形区域入侵报警---------------------//
//矩形区域入侵
typedef struct _RECT_RESULT
{
	uint rect_state;
}RECT_RESULT;
//------------------矢量相交报警----------------------------//
//矢量相交
typedef struct _CROSS_RESULT
{
	uint cross_state;
}CROSS_RESULT;
//------------------运动徘徊报警----------------------------//
//运动徘徊
typedef struct _LINGER_RESULT
{
	uint linger_state;
}LINGER_RESULT;
//-------------------结束----------------------------------------//
union ANALYSE_RESULT
{
	RED_RESULT red_result;
	LINE_RESULT line_result;
	DIRECT_RESULT direct_result;
	RECT_RESULT rect_result;
	CROSS_RESULT cross_result;
	LINGER_RESULT linger_result;
};
typedef struct _ANALYSE_DETECT_RESULT
{
	uint 		intelligent_type;
	union ANALYSE_RESULT result;
}ANALYSE_DETECT_RESULT;


/******************************************************************/
/// 智能分析的接口函数
/******************************************************************/

/// 获得智能分析能力。
/// 
/// \param [out] pCaps 指向智能分析能力结构体ANALYSE_DETECT_CAPS
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int AnalyseDetectGetCaps(ANALYSE_DETECT_CAPS * pCaps);

/// 创建智能检测设备
/// 
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int AnalyseDetectCreate(void);


/// 销毁智能检测设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int AnalyseDetectDestory(void);

/// 智能检测结果
/// 
/// \param [in] channel 通道号。
/// \param [in] pResult 指向智能检测参数结构ANALYSE_DETECT_RESULT的指针。
///	type智能报警类型
/// \retval 0  获取成功
/// \retval <0  获取失败
int AnalyseDetectGetResult(int channel, INTELLIGENTTYPE type,ANALYSE_DETECT_RESULT *pResult);


/// 配置智能检测通道参数
/// 
/// \param [in] channel 通道号。
/// \param [in] pParam 指向智能检测参数结构ANALYSE_DETECT_PARAM的指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int AnalyseDetectSetParameter(int channel, INTELLIGENTTYPE type,ANALYSE_DETECT_PARAM *pParam);

#ifdef __cplusplus
}
#endif

#endif //__VN_ANALYSE_API_H__


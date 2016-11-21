
#ifndef __VIDEO_API_H__
#define __VIDEO_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup VideoAPI API Video
/// 视频设置API。主要是视频输入参数，输出参数的设置，还有一些辅助的设置功能。
///	\n 调用流程图:
/// \code
///    =======================================================
///                |                            
///	      *Video2GetOutChannels       
///          VideoGetCaps            
///                |                            
///           VideoCreate                      
///      +---------|--------------+----------------------+     
///      |         |(In)          |(Out)                 |(Others)
///      |   VideoSetColor   VideoSwitch         VideoWriteRegister         
///      |   VideoSetCover VideoSetBkColor        VideoReadRegister
///      |   VideoSetVstd   VideoSetTVMargin        VideoMatrix
///      |         |        VideoSetTVColor      VideoSwitchTVMonitor
///      |         |       VideoSetTVAntiDither VideoSwitchVGAMonitor
///      |         |      VideoSetOutputAdapter          |
///      |         |      VideoSetOutputAdapter          |
///      |         |              |                      |
///      +---------|--------------+----------------------+          
///            VideoDestory   
///                |                            
///    =======================================================
/// \endcode
/// @{

/// 视频制式类型
enum video_standard_t {
	VIDEO_STANDARD_PAL = 0,	///< PAL制式。
	VIDEO_STANDARD_NTSC,	///< NTSC制式。
	VIDEO_STANDARD_SECAM	///< SECAM制式。
};
enum {
    ENUM_COLOR_RUILI,
    ENUM_COLOR_YANLI,    
    ENUM_COLOR_BIAOZHUN,
    ENUM_COLOR_ROUHE,
    ENUM_COLOR_ZHENGCHANG,
    ENUM_COLOR_ZIDINGYI
};
/// 视频颜色格式
/// 视频颜色格式
typedef struct VIDEO_COLOR{
	uchar	Brightness;		///< 亮度，取值0-100。
	uchar	Contrast;		///< 对比度，取值0-100。
	uchar 	Saturation;		///< 饱和度，取值0-100。
	uchar 	Hue;			///< 色调，取值0-100。
	uchar 	Gain;			///< 增益，取值0-100。bit7置位表示自动增益，其他位被忽略。
	uchar	WhiteBalance;	///< 自动白电平控制，bit7置位表示开启自动控制.0x0,0x1,0x2分别代表低,中,高等级
	uchar	reserved[2];
}VIDEO_COLOR;

/// 视频区域结构
/// - 当表示整个视频区域的坐标时, 使用图形坐标体系, 也就是是指在图形坐标体系下视频边界
/// 的坐标, 结构成员的取值与图形分辨率, 图形和视频叠加时的偏移和缩放参数, 当前分割模
/// 式等都有关系;
/// - 当表示的是整个视频区域中的一个子区域, 则使用相对坐标, 相对于一个虚拟的分辨率
/// (0, 0, RELATIVE_MAX_X, RELATIVE_MAX_Y), 使用相对坐标可以在不知道实际视频分辨率的
/// 情况下设定视频的子区域, 可以保证一次转化误差在1象素以内. 转换公式如下(假设绝对分
/// 辨率为aw*ah):
/// 绝对坐标(ax, ay)转换成相对坐标(rx, ry):
/// \code
/// rx = ax * RELATIVE_MAX_X / aw;
/// ry = ay * RELATIVE_MAX_Y / ah;
/// \endcode
/// 相对坐标(rx, ry)转换成绝对坐标(ax, ay):
/// \code
/// ax = rx * aw / RELATIVE_MAX_X;
/// ay = ry * ah / RELATIVE_MAX_Y;
/// \endcode
/// 相对分辨率定义如下:
/// \code
/// #define RELATIVE_MAX_X 8192
/// #define RELATIVE_MAX_Y 8192
/// \endcode
///
typedef struct VIDEO_RECT
{
	ushort	left;			///< 左上角x坐标。
	ushort	top;			///< 左上角y坐标。
	ushort	right;			///< 右下角x坐标。
	ushort	bottom;			///< 右下角y坐标。
}VIDEO_RECT;

/// 视频覆盖操作类型
enum video_cover_t {
	VIDEO_COVER_WINDOW_CLEAR = 0,	///< 清除窗口覆盖区域。
	VIDEO_COVER_WINDOW_DRAW,		///< 绘制窗口覆盖区域。
	VIDEO_COVER_FULLSCREEN_DRAW,	///< 绘制全屏覆盖区域。
	VIDEO_COVER_FULLSCREEN_CLEAR	///< 清楚全屏覆盖区域。
};

/// 视频覆盖参数
typedef struct VIDEO_COVER_PARAM
{
	/// 覆盖的矩形区域，采用相对坐标体系。
	VIDEO_RECT	rect;

	/// 被覆盖区域显示的颜色。16进制表示为0x00BBGGRR，最高字节保留。
	uint		color;

	/// 区域操作的类型， 取video_cover_t类型的值。窗口和全屏操作应该是独立的， 
	/// 即窗口内的操作不影响全屏，全屏内的操作也不影响窗口。 清除区域时，其他
	/// 参数被忽略。
	int			enable;
}VIDEO_COVER_PARAM;

/// 视频切换类型
enum video_switch_t{
	VIDEO_SWITCH_MONITOR = 0,	///< 切换到监视输出。
	VIDEO_SWITCH_PLAY			///< 切换到回放输出。
};

/// 视频输出适配器类型
enum video_output_t{
	VIDEO_OUTPUT_AUTO = 0,	///< 自动检测设备。
	VIDEO_OUTPUT_VGA,		///< 切换到VGA输出。
	VIDEO_OUTPUT_TV,		///< 切换到TV输出。
	VIDEO_OUTPUT_VGA_LCD	///< 切换到LCD VGA输出。
};

/// 视屏设备特性结构
typedef struct VIDEO_CAPS
{
	/// 是否支持TV输出颜色设置。置1表示支持该枚举值对应的特性，置0表示不支持。
	uchar TVColor;
	/// 图像属性掩码
	/// \note 掩码位定义参考pic_attr
} VIDEO_CAPS;				

#if (16==CHANNEL)
/* maozheng-add-20090709 S */
/* 16路时用于PCIV消息传递的结构体 */
typedef struct VIDEO_COVER_PCIV_S
{
    int					channel;
	int					index;
    VIDEO_COVER_PARAM	coverAttr;
} VIDEO_COVER_PCIV_S;

/* maozheng-add-20090709 E */
#endif

enum WHITE_BALANCE_MODE
{
	 WHITE_BALANCE_OFF, //关闭white balance
	 WHITE_BALANCE_AUTO,  //自动white balance
	 WHITE_BALANCE_DAYTIME, //室外白天
	 WHITE_BALANCE_EVENING, //室外黄昏
	 WHITE_BALANCE_CLOUDY, //室外多云
	 WHITE_BALANCE_OFFICE, //室内办公室
	 WHITE_BALANCE_FLUORESCENT,//荧光灯
	 WHITE_BALANCE_INCANDESCENT,// 白炽灯
	 WHITE_BALANCE_HAND,  //手动白平衡
	 WHITE_BALANCE_NR,
};

typedef struct _VIDEOIN_CAPS{
 uint uiCamAbility;  //按位处理（总32项目） 
                  //第0位 支持水平翻转HorReverse，1为支持该项功能，0为不支持该项功能
                  //第1位 支持竖直翻转VerReverse，1为支持该项功能，0为不支持该项功能
                  //第2位 支持曝光模式设置Exposure，1为支持该项功能，0为不支持该项功能
                  //第3位 支持感光度设置AESensitivity，1为支持该项功能，0为不支持该项功能
                  //第4位 支持背光补偿Backlight，1为支持该项功能，0为不支持该项功能
                  //第5位 支持白平衡模式WhiteBalance，1为支持该功能，0为不支持该项功能。
                  //第6位 支持阴影修正ShadeCorr，1为支持该功能，0为不支持该项功能。
                  //第7位 支持坏点修正BadCorr，1为支持该功能，0为不支持该项功能。
                  //第8位 支持防闪烁Anti-Flicker，1为支持该功能，0为不支持该项功能
                  //第9位 支持色调调节，1为支持该功能，0为不支持该功能
                  //第10位支持YUV输出范围控制，1为支持该功能，0为不支持该功能
                  //第11位支持IR-CUT控制，1为支持该功能，0为不支持该功能
                  //第12位支持去噪控制，1为支持该功能，0为不支持该功能
                  //第13位支持黑白/彩色/自动转换，1为支持该功能，0为不支持该功能
                   //第14位支持光敏电阻检查
                   //第15位支持isp数字宽动态 
                   //第16位多帧采集
                   //第17位WDR功能(SVAC中使用)
                   //第18位背光补偿
                   //第19位开关标清回环输出功能
                   //第20位帧累积, 参考EnFRAME_ACCUMULATE_MODE
                   //第21位超宽动态, 参考EnULTRA_DYNAMIC_RANGE_MODE
                   //第22位图像冻结, 参考EnFROZEN_IMAGE_MODE
                   //第23位最短对焦距离, 参考EnMIN_FOCUS_DISTANCE
                   //第24位GAMMA校正模式: 参考EnGAMMA_ADJUST_MODE
                   //第25位强光抑制                   
                   //第26位支持sensor宽动态
 uint uiRes;//保留
 uint uiSenSpan; //感光度范围最大值 如值255，则标识范围为0~255
 uint uiBLightSpan;//背光补偿支持上限Backlight,如值15，则范围0~15
 uint uiShutterL;//手动曝光 若值为6000则支持最小曝光时间为1/6000秒
 uint uiShutterH; //手动曝光，若值为50则支持最小曝光时间为1/50秒
 uint uiAntiFlicker;//防闪烁支持模式，按位处理
   //第0位 是否支持50Hz indoor模式
   //第1位 是否支持50Hz outdoor模式
   //第2位 是否支持50Hz auto模式
   //第3位 是否支持60Hz indoor模式
   //第4位 是否支持60Hz outdoor模式
   //第5位 是否支持60Hz auto模式
   //第6位 是否支持50Hz Theater模式
   //第7位 是否支持50Hz Fast模式
   //第8位 是否支持60Hz Theater模式
   //第9位 是否支持60Hz Fast模式
   //第10位 是否支持防闪关闭模式
 uint uiWhiteBlance; //如果支持白平衡模式，此项有效，按位表示白平衡支持的模式
   //每一位对应WHITE_BALANCE_MODE枚举中的相应位，比如0xc0表示支持色温4650k和色温6500k两种模式
 uint uiRes2[2];//保留
}VIDEOIN_CAPS;

//CONFIG_CAMERA和CAMERA_FORMAT结构体一样，修改时必须注意
typedef struct _CAMERA_FORMAT_
{
	 uchar HorReverse; //支持水平翻转时取值范围为枚举IMAGE_MODE_MIRROE中的值
	 uchar VerReverse; //支持竖直翻转时取值范围为枚举IMAGE_MODE_MIRROE中的值
	 uchar ExposureType;
	 //支持曝光模式时取值范围为枚举EXPOSURE_MODE中的值1:自动曝光 2:手动
	 uchar AESensitivity; 
	 //支持感光度设置时取值范围为0到ABILITY_CAMERA_T.uiSenSpan
	 uchar Backlight; //支持背光补偿时取值范围为0到ABILITY_CAMERA_T.uiBLightSpan
	 uchar WhiteBalanceMode; //支持白平衡时取值范围为能力集ABILITY_CAMERA_T.uiWhiteBlance中按位表示的值
	 uchar ShadeCorr; //暂不支持，取值范围未知
	 uchar BadCorr;  //暂不支持，取值范围未知
	 uint shutter;  //曝光模式设为手动时取值范围为uiShutterL到uiShutterH之间
	 uchar AntiFlicker; //支持防闪烁时，取值范围为能力集ABILITY_CAMERA_T.uiAntiFlicker中按位表示的值
	 uchar ColorTemp; //支持色调调节时，取值范围为枚举COLOR_TEMP_MODE 中的值
	 uchar YUVCut; //支持yuv输出范围控制时，取值范围为枚举YUV_RANGE_MODE 中的值
	 uchar IRCut; //支持IR-CUT时，取值范围为枚举IR_CUT_MODE中的值
	 uchar PAA; //支持去噪功能时，取值范围为枚举PAA_MODE中的值
	 uchar Color2WhiteBlack; //支持黑白/彩色/自动转换时，取值范围为枚举COLOR_2_WHITEBLACK_MODE中的值
	 uchar SensorType;   //光敏电阻 高电平有效低电平有效 0 高电平 1低电平  
	 uchar res[5];
}CAMERA_FORMAT;

typedef struct _CAMERA_WHITEBALANCE_  //手动设置白平衡是的RGB
{
	 uchar WhiteBalanceMode; //支持白平衡时取值范围为能力集ABILITY_CAMERA_T.uiWhiteBlance中按位表示的值
	 uchar wbr; //自动白平衡R分量
	 uchar wbg; //自动白平衡R分量
	 uchar wbb; //自动白平衡R分量
	 uchar res[4];
}CAMERA_WHITEBALANCE;

typedef struct _CAMERA_FORMAT_EXT_  // 用于设置cam的 扩张功能
{
	 uint ispWDRenable; //DRC
	 uint ispWDRStrength; //0 - 255  DRC str
	 uchar SlowFrameRate;  // 0 -15 多帧采集	 低照度情况提升图像质量
	 uchar snrWDRenable;//sensor 宽动态
	 uchar res[14];
}CAMERA_FORMAT_EXT;

/// 得到视频输出的通道数目。
/// 
/// \param 无
/// \return视频输出通道数目。
int Video2GetOutChannels(void);


/// 创建视频设备
/// 
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int VideoCreate(void);
int Video2Create(int index);


/// 销毁视频设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int VideoDestory(void);
int Video2Destroy(int index);


/// 设置视频的颜色参数。
/// 
/// \param [in] channel 通道号。
/// \param [in] pColor 指向颜色结构VIDEO _ COLOR的指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int VideoSetColor(int channel, VIDEO_COLOR * pColor);

/// 图像支持的特性列表掩码
/// \note 作为图像功能列表属性mask 1:可设置 0:不可设置
enum pic_attr
{
 PIC_COLOR, ///< 亮度,对比度,色度,增益,饱和度
 PIC_BACKLIGHT,///< 背光补偿
 PIC_EXPOSAL, 	///< 曝光选择
 PIC_AUTOC2BW, ///< 自动彩黑转换
 PIC_MIRROR,		//镜像
 PIC_FLIP,		//翻转
 PIC_MASKNUM,  ///< 属性选项数
};
 
/// 视频控制结构
typedef struct VIDEO_CONTROL{
 uchar  Exposure; ///< 曝光模式 1-6:手动曝光等级; 0:自动曝光
 uchar  Backlight; ///< 背光补偿 1:打开补偿; 0:停止补偿
 uchar  AutoColor2BW; ///< 自动彩黑转换 1:打开转换; 0:停止转换
  uchar  Mirror;		//< 镜像  1 开， 0关

}VIDEO_CONTROL;

typedef struct tagWHITE_LEVEL_CAPS //Added by wangjun2-20080805
{
	int	 enable;		// 为1表示使能，为0时表示禁用
}WHITE_LEVEL_CAPS;


/// 得到视频输出的通道数目。
/// 
/// \param 无
/// \return视频输出通道数目。
int Video2GetOutChannels(void);


/// 创建视频设备
/// 
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int VideoCreate(void);
int Video2Create(int index);


/// 销毁视频设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int VideoDestory(void);
int Video2Destroy(int index);


/// 设置视频的颜色参数。
/// 
/// \param [in] channel 通道号。
/// \param [in] pColor 指向颜色结构VIDEO _ COLOR的指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int VideoSetColor(int channel, VIDEO_COLOR * pColor);
 
/// 设置视频的控制参数。
/// 
/// \param [in] channel 通道号。
/// \param [in] pColor 指向视频控制结构VIDEO_CONTROL的指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int VideoSetControl(int channel, VIDEO_CONTROL * pColor);


/// 设置覆盖区域。
/// 
/// \param [in] channel 通道号。
/// \param [in] index 遮挡区域号
/// \param [in] pParam 指向视频覆盖结构VIDEO_COVER_PARAM的指针。
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int VideoSetCover(int channel, int index, VIDEO_COVER_PARAM *pParam);


/// 设置视频制式。
/// 
/// \param [in] channel 通道号。
/// \param [in] dwStandard 视频制式，取video_standard_t类型的值。
/// \retval 0  设置成功
/// \retval <0  设置失败
int VideoSetVstd(int channel,uint dwStandard);	


/// 写一个字节到视频设备寄存器。
/// 
/// \param [in] Chip 芯片序号。
/// \param [in] Page 页面序号。
/// \param [in] Register 寄存器序号。
/// \param [in] Data 要写入的值。
/// \return 无
void VideoWriteRegister(uchar Chip, uchar Page, uchar Register, uchar Data);


/// 从视频设备寄存器读一个字节。
/// 
/// \param [in] Chip 芯片序号。
/// \param [in] Page 页面序号。
/// \param [in] Register 寄存器序号。
/// \return读到的值。
uchar VideoReadRegister(uchar Chip, uchar Page, uchar Register);


/// 切换视频输出的类型。
/// \param [in] dwType 输出的类型，取下表中的值。
/// \retval 0  切换成功。
/// \retval <0  切换失败。
int VideoSwitch(uint dwType);
int Video2Switch(int index, uint dwType);


/// 设置TV输出的边界。视频显示在某些监视器上时， 4个边上会有损失。这个函数的目的
/// 就是设置4个边界压缩的比例，使视频落在监视器的可视区域内。调节范围0-100是一个
/// 示意值，实际上对应缩小的尺寸需要在底层做转换和限制。
/// 
/// \param [in] left 左边界比率，取值0-100， 默认值为0。
/// \param [in] top 上边界比率，取值0-100， 默认值为0。
/// \param [in] right 右边界比率，取值0-100， 默认值为0。
/// \param [in] bottom 下边界比率，取值0-100， 默认值为0。
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int VideoSetTVMargin(uchar left, uchar top, uchar right, uchar bottom);


/// 设置TV输出的颜色。
/// 
/// \param [in] brightness 亮度，取值0-100， 默认值为50。
/// \param [in] contrast 对比度，取值0-100， 默认值为50。
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int VideoSetTVColor(uchar brightness, uchar contrast);


/// 设置TV输出防抖动参数，部分老硬件会有抖动的问题。
/// 
/// \param [in] level 防抖动参数，抖动程度与取值不是比例关系，而是一个经验值。
///        取值0-100， 默认值为50。
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int VideoSetTVAntiDither(uchar level);


/// 设置视频输出的适配器类型。
/// 
/// \param [in] type 适配器类型，取video_output_t类型的值。
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int VideoSetOutputAdapter(uchar type);


/// 获取视频输出的适配器类型。
/// 
/// \param [out] ptype 适配器类型，取video_output_t类型的值。
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int VideoGetOutputAdapter(uchar *ptype);


/// 连接视频矩阵的输出通道和输入通道。
/// 
/// \param [in]  channelIn 视频输入通道
/// \param [in]  channelOut 视频输出通道
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int VideoMatrix(uchar channelIn,uchar channelOut);


/// 开关TV监视器
/// 
/// \param [in] open TRUE-开TV监视器，FALSE-关TV监视器
/// \retval 0  操作成功。
/// \retval <0  操作失败。
int VideoSwitchTVMonitor(VD_BOOL open);


/// 开关VGA监视器
/// 
/// \param [in] open TRUE-开VGA监视器，FALSE-关VGA监视器
/// \retval 0  操作成功。
/// \retval <0  操作失败。
int VideoSwitchVGAMonitor(VD_BOOL open);
/// 配置采集芯片的采集区域
/// 
/// \param [in] 对应通道的水平垂直偏移，以及宽高属性
/// \retval 0  操作成功。
/// \retval <0  操作失败。
int VideoSetCropRegion(int channel,int Hoffset,int Voffset,int Width,int Height);
/// 得到视频支持的特性。
/// 
/// \param [out] pCaps 指向视频特性结构VIDEO_CAPS的指针。
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int VideoGetCaps(VIDEO_CAPS * pCaps);
int Video2GetCaps(int index, VIDEO_CAPS * pCaps);


/// 设置没有视频的监视画面的背景颜色.
/// 
/// \param [in] color 要设置的颜色，16进制表示为0x00BBGGRR，最高字节保留。
/// \retval 0  设置成功
/// \retval <0  设置失败
int VideoSetBkColor(uint color);
int Video2SetBkColor(int index, uint color);


/// 设置视频捕获分辨率
int VideoRecordSetSize(int channel, int size);
	/*
说明	开始自动检测

参数	无

返回值	=0  ：	  成功。
	其他值：  失败。

*/
int VideoAutoDetectStart(void);
/*
说明	停止自动检测


参数	无

返回值	=0  ：	  成功。
	其他值：  失败。
	*/
int VideoAutoDetectStop(void);

/// @} end of group

/*
	说明:获取白电平控制能力值
	返回值=0:成功
	其他值:失败
*/
int WhiteLevelGetCaps(WHITE_LEVEL_CAPS * pCaps);

/*
	set or get video mode
	SystemSetVideoMode() return 0 success
*/
enum video_standard_t SystemGetVideoMode(void);
int SystemSetVideoMode(enum video_standard_t videostand);

/*
	说明: 		设置
	参数in: 	iChannel 通道
				pFormat设置结构体
	参数out: 	无
	返回值	=0  ：	  成功。
		<0：  失败。

*/
int VideoInSetCamera(uint iChannel, CAMERA_FORMAT *pFormat);

int VideoInSetWhiteBalance(uint iChannel, CAMERA_WHITEBALANCE *pWhiteBalance);

int VideoInSetCameraExt(uint iChannel,CAMERA_FORMAT_EXT *pFormatExt);
/*
	说明: 		获取镜头能力结构体
	参数in: 	ich 通道
	参数out: 	pCaps，镜头能力结构体
	返回值	=0  ：	  成功。
		<0：  失败。

*/
int VideoInGetChCaps(int ich , VIDEOIN_CAPS* pCaps);

int VideoBadPixelDetect(void);

//!ipc sensor type
typedef enum tagVd_sensor_type_e
{
    APTINA_9P031_DC_1080P_CONTINUES = 0,
    APTINA_9P031_DC_1080P_DEFAULT =1,
    ALTASENS_DC_1080P = 2,
    OV_2715_DC_1080P = 3,
    OV_10630_DC_800P = 4,
    PANASONIC_34041_1080P = 5,
    SONY_IMX036_DC_1080P = 6,
    APTINA_9M034_DC_720P_CONTINUES = 7,

    OV_2715_BT601_1080P = 8,
    BT656_D1P = 9,
    BT1120_720P = 10,
    VISIONDIGI_HITACHI_110_720P25 = 11,
    VISIONDIGI_HITACHI_110_720P30 = 12,
    VISIONDIGI_HITACHI_110_960P20 = 13,
    VISIONDIGI_SONY_EH4300_1080P = 14,
    VISIONDIGI_SONY_EH4300_720P = 15,
    VISIONDIGI_SONY_EH4300_1080I = 16,
    VISIONDIGI_SAMSUNG_5190_SXGA = 17,
    PANASONIC_34041_1080P_60FPS =18,
    SONY_IMX122_DC_1080P =19,
    APTINA_AR0331_DC_1080P_30FPS = 20,
    APTINA_AR0331_DC_3M_20FPS = 21,
    APTINA_AR0331_DC_1080P_60FPS =22, 
    VISIONDIGI_CS_110_720P30 =23,
    BT1120_1080P=24,                                              //tvp7002
    VISIONDIGI_SAMSUNG_6200 = 25,                     //1080P
    PANASONIC_34041_720P = 26, 
    VISIONDIGI_SAMSUNG_5190_720P = 27,
    SENSOR_S532_720P = 28,   
    VISIONDIGI_LG_2810=29,                                  //720P
    APTINA_9M034 =30,
    VISIONDIGI_HITACHI_220_1080P = 31,
    VISIONDIGI_LG_1220=32,                                  //1080P
    YOKO_RYK_H231=33,    
    VISIONDIGI_NUM,

}VD_SENSOR_TYPE_E;

typedef enum tagVd_sensor_mode
{
       // 1080P 机芯   1080P 编码, 720P 机芯     720P  编码
	VD_SENSOR_NORM =0,
	VD_SENSOR_1080P_720P,   // 1080P 机芯 720P 编码
}VD_SENSOR_MOED;

//!按位表示对应VD_SENSOR_TYPE_E 的值，最多支持256种
typedef struct tagVD_SENSOR_CAP_T
{
	uint sensor_cap;
	uint res[7];
}VD_SENSOR_CAP_T;

//!设置Sensor type, 以供ipc使用
int VideoInSetSensorType(int ich, VD_SENSOR_TYPE_E type);
VD_SENSOR_TYPE_E VideoInGetSensorType(int ich);
//!设置Sensor 使用类型 1080P  用作720P 编码, 以供ipc使用
int VideoInSetSensorMode(int ich, VD_SENSOR_MOED type);
VD_SENSOR_MOED VideoInGetSensorMode(int ich);


int VideoInGetSensorCap(VD_SENSOR_CAP_T* pCaps);
int GetProfileType();
int SetProfileType(int type);

int SetCameraVal(int id,int id2,int value);

int SetLinearMode();
int SetHdrMode();

#ifdef __cplusplus
}
#endif

#endif //__VIDEO_API_H__


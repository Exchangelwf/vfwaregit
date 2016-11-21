
#ifndef NETWORKSERVICE_BASETYPEDEF_H
#define NETWORKSERVICE_BASETYPEDEF_H

#include "common/Types.h"
#define IP4_STR_LEN 15

#ifdef WIN32
#else
    #include "common/DVRDEF.H"
    #ifndef trace
        #define trace printf
    #endif//trace
#endif

#include <time.h>

#ifndef __trip
    #define __trip trace("-W-%d::%s(%d)\n", (int)time(NULL), __FILE__, __LINE__);
#endif
#ifndef __fline
    #define __fline trace("%s(%d)--", __FILE__, __LINE__);
#endif


namespace NETWORKSERVICE
{

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
extern int g_nAlarmIn;
extern int g_nAlarmOut;
extern int g_nCapture;
extern int g_nPlay;
extern int g_nLogicNum;
// not valid int
const int DEFAULT_NOT_VALID_INT        = -1;

typedef union 
{
    uchar    c[4];
    ushort    s[2];
    uint    l;
}IPDEF;


/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

const int INT_NOTVALID = -1;

const int SUCCESS_RET =  0;
const int FAILURE_RET = -1;

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#if defined(WIN32)
    #ifndef LITTLE_ENDIAN
        #define LITTLE_ENDIAN 1234
        #define BIG_ENDIAN 4321
        #define BYTE_ORDER LITTLE_ENDIAN 
    #endif
#endif

#ifdef LINUX
    #include <endian.h>
#endif

	/* 通讯协议 */

#define    DVRIP_VERSION    5

	typedef struct dvrip
	{

		unsigned char head_token[8]; 	/*="WJAPRIVA"  ,包头表示*/
		unsigned char    dvrip_cmd;        /* command  命令 */
		unsigned char    dvrip_r0;        /* reserved 保留*/
		unsigned char    dvrip_r1;        /* reserved */
#if BYTE_ORDER == LITTLE_ENDIAN 
		unsigned char    dvrip_hl:4,        /* header length */
		                dvrip_v :4;        /* version */
#endif
#if BYTE_ORDER == BIG_ENDIAN 
		unsigned char    dvrip_v :4,        /* version */
		                dvrip_hl:4;        /* header length */
#endif
		unsigned int    dvrip_extlen;    /* ext data length  扩展长度 */
		unsigned char    dvrip_p[24];    /* inter dvrip.dvrip_p 参数 */
	}DVRIP;

#define DVRIP_HEAD_T_SIZE sizeof(DVRIP)

#define ZERO_DVRIP_HEAD_T(X) memset((X), 0, DVRIP_HEAD_T_SIZE);            \
	                            (X)->dvrip_hl = DVRIP_HEAD_T_SIZE/4;    \
	                            (X)->dvrip_v = DVRIP_VERSION;

	//
	// 查询系统参数信息
	// 注意顺序不能改变
	//
	typedef enum __cfg_index_t {
	    CFG_GENERAL =  0,    // 普通
	    CFG_COMM           ,    // 串口
	    CFG_NET            ,    // 网络
	    CFG_RECORD         ,    // 录像        /*!< 2.42以后废弃不用，使用CFG_NEWRECORD */
	    CFG_CAPTURE        ,    // 图像设置    /*!< 2.42以后废弃不用，使用CFG_CURRENTCAPTURE */
	    CFG_PTZ            ,    // 云台
	    CFG_DETECT         ,    // 动态检测    /*!< 2.42以后废弃不用，使用CFG_EVENT */ 
	    CFG_ALARM          ,    // 报警        /*!< 2.42以后废弃不用，使用CFG_EVENT */ 
	    CFG_DISPLAY        ,    // 显示
	    CFG_TITLE        = 10,       // 通道标题
	    CFG_MAIL         = 11,       // 邮件功能
	    CFG_EXCAPTURE    = 12,       // 预览图像设置
	    CFG_PPPOE        = 13,       // pppoe设置
	    CFG_DDNS         = 14,       // DDNS设置
	    CFG_SNIFFER      = 15,       // 网络监视捕获设置
	    CFG_DSPINFO      = 16,      // 编码能力信息
#ifdef DEF_MATRIX_INPUT
	    CFG_MATRIXINPUT = 41, //矩阵输入20100825
#endif
	    CFG_NET_NTP = 19, /*ntp配置*/
	    CFG_NET_UPNP     = 21,
	    CFG_FTP          = 37,//17,       //FTP配置
	    CFG_AUTOMAINTAIN = 18,      //自动维护
	    CFG_DSPBITRATE   = 24,    // DSP最大码流
	    CFG_MATRIX       = 30,
	    CFG_VIDEOCOVER   = 34,    //区域遮挡
	    CFG_DHCP         = 35,
	    CFG_WEB          = 36,
	    CFG_WLANIP       = 39,    //无线模块IP配置
	    CFG_DNS          = 40,
	    CFG_RECDDOWNLOAD = 42,
	    CFG_AUDIOFORMAT  = 43,        //音频编码格式
		CFG_SYSTEMLOG  = 44,		  //日志配置

#ifdef FUNC_MUTILINE_TITLE
	    CFG_MUTITLE        = 45,   //多行OSD叠加
#endif 
#ifdef DEF_MATRIX_INPUT
	    DEV_SYSTEM_INFO_XH_CAMERID =58, /*星火矩阵摄像头通道控制*/
#endif
	    CFG_DIGI_CHANNEL = 61,
	    CFG_IPCONFICT =62,
	    CFG_NEWRECORD    = 123,        /*!< 新的录像设置 */
	    CFG_EVENT        = 124,       //事件配置
	    CFG_WORKSHEET    = 125,     //时间表配置信息
	    CFG_COLOR        = 126,     // 颜色配置信息
	    CFG_CURRENTCAPTURE = 127,  // 当前最新配置,为增加双码流功能后增加的
	    CFG_NEWCAPTURE     = 128,  // 采用新的图像配置结构
	    CFG_CAMERA = 129,
	     CFG_WHITEBALANCE = 130,
	    CFG_WLAN           = 131,    //wifi config
	    CFG_TRANSFER_POLICY  = 133,//网络传输策略配置 
	    CFG_WLANSCAN         = 135, //wifi scanning
	    CFG_NEWDDNS        = 140,        //新的ddns
	    CFG_DECODE         = 0x8d, //141, 解码器
	    CFG_VSP            = 190,   //第三方介入配置
	    CFG_CAMIPC         = 200,   //CAMIPC配置
	    CFG_LOCATION       = 201,    /*!< 区域配置 */
	    CFG_GUISET         = 202,    /*!< GUI配置 */
	    CFG_RECWORKSHEET   = 203,     /*!< 录像工作表配置 */
	    CFG_PTZALARM       = 204,    /*!< 云台报警设备协议配置 */
	    CFG_NETALARM       = 205,    /*!< 网络报警 */
	    CFG_ALMWORKSHEET   = 206,    /*!< 报警工作表配置 */
	    CFG_MTDWORKSHEET   = 207,    /*!< 动检工作表配置 */
	    CFG_BLDWORKSHEET   = 208,    /*!< 遮挡工作表配置 */
	    CFG_VLTWORKSHEET   = 209,    /*!< 视频丢失工作表配置 */
	    CFG_PRESETNAME     = 210,    /*!< 云台预知点名 */
	    CFG_PTZTOUR        = 211,    /*!< 巡航配置 */
	    CFG_LOSS           = 212,    /*!< 视频丢失 */
	    CFG_BLIND          = 213,   /*!< 视频遮挡 */
	    CFG_PLAY           = 214,    /*!< 回放配置 */
	    CFG_USER           = 215,    /*!< 用户配置 */
	    CFG_GROUP          = 216,    /*!< 用户组配置 */
	    CFG_MONITORTOUR    = 217,    /*!< 监视轮训 */
	    CFG_TVADJUST       = 218,    /*!< TV调节 */
	    CFG_ATM            = 219,    /*!< ATM配置 */
	    CFG_STNOTEXIST     = 220,    /*!< 无硬盘 */
	    CFG_STFAILURE      = 221,    /*!< 硬盘出错 */
	    CFG_STLOWSPACE     = 222,    /*!< 硬盘空间不足 */
	    CFG_NETABORT       = 223,    /*!< 断网事件配置 */
	    CFG_VIDEOWIDGET    = 224,    /*!< 视频装饰 */
	    CFG_VIDEOMATRIX    = 225,    /*!< 视频矩阵 */
#ifdef FUNCTION_SAVIA_INTELLIGENT  //add langzi 2009-12-17 savia智能宏定义
	    CFG_INTELLIGENT    = 226,    /*!< 智能报警 add langzi 2009-12-10*/ 
	    CFG_INTWORKSHEET   = 227,    /*!< 智能报警工作表配置 add langzi 2009-12-10*/ 
#endif //end langzi
	    CFG_SNAPWORKSHEET = 228,
	    CFG_NETALMWORKSHEET =229,
	    CFG_NET_COMMON = 230, /*通用网络配置*/
	    CFG_NET_FTPSERVER =231, /*FTP服务器配置*/
	    CFG_NET_FTPAPPLICATION =232, /*ftp应用配置*/
	    CFG_NET_IPFILTER =233, /*IP过滤配置*/
	    CFG_NET_MULTICAST =234, /*组播配置*/
	    CFG_NET_PPPOE =235, /*pppoe配置*/
	    CFG_NET_DDNS =236, /*ddns配置*/
	    CFG_NET_ALARMSERVER =237, /*报警中心配置*/
	    CFG_NET_EMAIL =238, /*email配置*/
	    CFG_NET_SNIFFER =239, /*抓包配置*/

#ifdef VSERVER   //add by kyle xu in 20160107    
	    CFG_NET_VSERVER = 240, /*星谷平台配置*/
#endif
	    
	    CFG_REG_SERVER = 241,/*主动注册服务器配置*/
	    CFG_GOOLINK = 242, /*GOOLINK */
	    CFG_RECState = 245, /*报警动检录像状态*/

	    //改成通用
	    CFG_NET_PRESET=246,/* 预置点配置信息 */
	    CFG_NET_TOUR=247,/* 巡航配置信息 */
	    CFG_CONFIG_PTZREGRESS=248, /* 预置点回归信息 */
	    CFG_OSD_PARAM = 249, /*OSD缩放*/
	    
#if defined SHBELL || defined ZXBELL|| defined ZXBELL30 || defined VIMICRO || defined SHBELL29 || defined HTTP_VS|| defined GAYS ||defined ZXNVR
	    CFG_NET_SHBELL=249,/* 上海BELL电信的配置信息 */
	    #ifdef __BELL_PICSHOOT__
	    CFG_CONFIG_UPLOAD_PICTURE=250, /* 报警图片上传配置信息 */
	    #endif
#ifdef __BELL_BJUNICOM__
	    CFG_CONFIG_VSSETUP=275,
	    CFG_CONFIG_QSERVER=276,
#endif
#endif

#if defined(MEGA_DISPLAY) || defined(MEGA_RF) 
	    CFG_NET_MEGA=251,
	    CFG_MEGA_FTP=252,
	    CFG_MEGA_MOTION_ZONE=253,
	    CFG_NET_MEGAEYES_PRESET=254,/* 互信互通的预置点配置信息 */
	    CFG_NET_MEGAEYES_TOUR=255,/* 互信互通的巡航配置信息 */
	    CFG_MEGA_CAMERAID=256,
	    CFG_FTP_TIME=257,
	    CFG_USER_ID=258,
#endif

#ifdef ZTE_NETVIEW
	    CFG_NET_NETVIEW=259,/*中兴力维接入的配置*/
	    CFG_OSD_NETVIEW=260,/*中兴力维附加OSD的配置*/
	    CFG_NET_NETVIEW_PRESET=261,/*中兴力维的预置点配置信息 */
	    CFG_NET_NETVIEW_TOUR,/* 中兴力维的巡航配置信息 */
#if defined(_NETVIEW_EXTEND_)
	    CFG_ELSE_NETVIEW=262,
#endif
#if defined(CNC_SJZ)
	    CFG_PPWORKSHEET=263,
#endif
#endif

#if defined(_ZTE_APP_)
		CFG_NET_ZTENJ = 264,		/*第三方接入中兴南研所配置*/
		CFG_NET_ZTENJ_TR069 =265,
		CFG_ZTE_ALARMIN = 266,
		CFG_ZTE_ALARMOUT = 267,
		CFG_ELSE_ZTENJ = 268,
		CFG_ZTE_CHANNEL = 269,
#endif

#ifdef SHENYAN
		CFG_SHENYAN = 270,
#endif

#ifdef YUANYANG
		CFG_YUANYANG = 271,
		CFG_AREA = 272,
#endif

#ifdef KOTI
		CFG_KOTI = 273,
#endif

#ifdef _PRODUCT_2900
		CFG_SVAC = 274,
#endif
#ifdef YUANYANG
		CFG_YYMAINCOM = 275,
		CFG_YYEXTCOM = 276,
#endif

#ifdef FUN_SNIFFER 
		CFG_ATM_FRAME = 278,
#endif	

#ifdef _SUNELL_ 
		CFG_SUNELL_ALARM = 279,
#endif	

#ifdef BLUESTAR
	CFG_BLUESTAR = 280,
#endif

#ifdef HW_DVR
	CFG_HAOWEI = 281,

#endif

#ifdef SHREG
	CFG_SHREG = 282,
#endif

#ifdef GAYS
	CFG_CONFIG_GAYS = 283,
#endif

#if defined(GUIZHOU_G_PROJECT) || defined(POLICE_PROJECT)
		CFG_GUIZHOU_G_PROJECT = 285,
#endif
#ifdef POLICE_PROJECT 
		CFG_POLICE_PROJECT = 286,	
#endif
#if defined(GUIZHOU_G_PROJECT) || defined(POLICE_PROJECT)
			CFG_WLAN_INFO = 287,
#endif

	    CFG_AUTO_REGSERVER=300,


#ifdef FUNCTION_SAVIA_INTELLIGENT 
	    CFG_INTEL_REDALARM   = 301,    
	    CFG_INTEL_REDWORKSHEET   = 302,    
#endif //end langzi
	    CFG_DECALM = 303,
	    CFG_DecAlmWORKSHEET = 304,
#ifdef _CAR_DVR
	    CFG_CARDVR   = 305,
#endif
#if defined SHBELL || defined ZXBELL|| defined ZXBELL30 || defined VIMICRO || defined SHBELL29 || defined HTTP_VS|| defined GAYS  ||defined ZXNVR
	   CFG_CONFIG_IPCDECODER=306,
#endif

		CFG_PRESET_TITLE = 307,  //视频叠加与指点名称


		CFG_MJ_CAMERA,
		CFG_MJ_ADV_CAMERA,
		CFG_MJ_AUTOHOME,
		CFG_MJ_BLIND,
		CFG_MJ_PTZ,
		CFG_SUVA_DETECT,
		CFG_SUVA_WORKSHEET,
		CFG_RRCORD_MULTI_STREAM,

	    CFG_COLOR_EX        = 320, /* 图像颜色扩展配置 */
	    CFG_SENSOR = 321,
	    CFG_DRC = 322, 
#ifdef SHREG
	    CFG_EVENT_SNAP = 323,
#endif

#ifdef ONVIF  //modified by kyle xu in 201603151700
	    CFG_ONVIF_SERVER = 324,
#endif

#ifdef _2761_EXCEPTION_CHK_
		CFG_ENCODER_DETECT = 325,
#endif
#ifdef  MEGA_DISPLAY
	    CFG_MEGA_DISPLAY_DECODE_CFG=325,
	    CFG_MEGA_MEGA_FULLSCREEN= 326,
#endif
		CFG_OUT_COlOR_SET = 327,    /*!< TV调节 */
#ifdef LANGTAO
		CFG_LANGTAO_MOBILE = 328 //浪涛手机协议
#endif
	}CFG_INDEX;

	//!产品标记 
	enum product_t {
	    /* 视毫 */
	    DH_DVR_H    =        0,
	    /* 视通 */
	    DH_DVR_G    =        1,
	    /* 视新 */
	    DH_DVR_L    =        2,
	    /* 非实时 */
	    DH_DVR_N    =        3,
	    /* ATM机器 */
	    DH_DVR_ATM    =        4,
	    /* 网络视频服务器 */
	    DH_NVS        =        5,
	    /* 视频服务器0404e */
	    DH_NVS_E    =        6,
	    /*B系列NVS*/
	    DH_NVS_B    =        10,
	    /*H系列NVS*/
	    DH_NVS_C    =        11,

	    DH_CAM_SDIP    =        50,
	    DH_CAM_IPC    =        51,
	    DH_HISI_IPC    =        55,

	    /*A6机型*/
	    DH_DVR_A6   =       60,

		//威乾产品标记 
		VD_DVR      =       100,
		VD_IPC      =       101,
		VD_DVS      =       102,
		VD_DECODER  =       103,
		VD_DOME    =    104

	};

#define MAX_FILE_NUM_ONCE        16        //网络查询，一次最多16个文件

	typedef enum audio_forat_t
	{
	    AUDIO_FORMAT_PCM8 = 1,
	    AUDIO_FORMAT_G711a = 2,
	    AUDIO_FORMAT_AMR = 3,
	    AUDIO_FORMAT_G711u = 4,
	    AUDIO_FORMAT_G726 = 5,
	    AUDIO_FORMAT_IMAADPCM = 6
	}Em_AUDIO_FORMAT;

	enum EnDVRCommand
	{
	    //请求
	    REQ_SYSTEM_CONFIG     = 0x00,
	    ACK_REQ_SYSTEM_CONFIG = 0x00,
	    SET_SYSTEM_CONFIG     = 0x01,
	    ACK_SET_SYSTEM_CONFIG = 0x01,
	    REQ_REMOTE_SYSTEM_CONFIG     = 0x02,
	    SET_REMOTE_SYSTEM_CONFIG     = 0x03,
	    
	    REQ_UPDATE_LOGIN  = 0x10,            //升级客户端登录
	    REQ_CHAN_SWTCH    = 0x11,            //视频切换
	    REQ_CTRL_PANTZ    = 0x12,            //云台控制, 预置点设置, 灯光控制
	    REQ_MODE_FOCUS    = 0x13,
	    REQ_CTRL_FOCUS    = 0x14,
	    REQ_CTRL_ZOOM     = 0x15,
	    REQ_CTRL_IMAGE    = 0x16,
	    REQ_CTRL_MONSW    = 0x17,            //画面风格(分割)切换
	    REQ_CTRL_IRIS     = 0x18,
	    REQ_CTRL_PRESET   = 0x19,
	    REQ_CTRL_ALARM    = 0x1a,
	    REQ_CTRL_LAMP     = 0x1b,
	    REQ_CTRL_MENU     = 0x1c,
	    REQ_SEND_AUDIO    = 0x1d,            //要求发送音频
	    REQ_MODE_XCHGE    = 0x1e,            //设置传输方式
	    REQ_SEND_TXMOD    = 0x1f,            //发送传输方式
	    REQ_COMM_SEND     = 0x20,            //透明串口传送
	    REQ_CTRL_RESET    = 0x21,
	    REQ_DISK_PARAM    = 0x22,            //要求传送硬盘状态信息
	    REQ_ALARM_INFO    = 0x23,            //请求报警输入输出状态
	    REQ_SET_DTIME     = 0x24,            //DVR时间设置
	    REQ_EXT_FUNCTION  = 0x25,
	    REQ_NET_KEYBOARD_CTRL = 0x98,   /*!< 新的网络键盘控制 */
	    REQ_BOARD_CTRL        = 0x99,            //键盘控制

	    REQ_USER_LOGIN    = 0x0a,
	    REQ_STAT_QUERY    = 0xa1,            //a1 请求状态查询
	    REQ_LOGS_PARAM    = 0xa2,            //请求日志信息
	    REQ_CONF_PARAM    = 0xa3,            //请求系统配置
	    REQ_INFO_SYSTM    = 0xa4,            //请求系统信息
	    REQ_RECD_QUERY    = 0xa5,            //查询录像文件
	    REQ_USER_PARAM    = 0xa6,            //查询用户信息
	    REQ_FILE_ALARM    = 0xa7,            //查询报警文件
	    REQ_CHAN_TITLE    = 0xa8,            //请求发送汉字
	    REQ_CTRL_SWTCH    = 0xa9,            //请求改变控制权

	    REQ_AUDIO_DATA    = 0xc0,            //对讲数据
	    ACK_REQ_AUDIO_DATA    = 0x1d,            //对讲数据

	    REQ_CONF_UPDAT    = 0xc1,            //请求配置更新
	    REQ_RECD_PLAY     = 0xc2,            //录像文件回放
	    REQ_PLAY_ALERT    = 0xc3,            //播放报警文件
	    REQ_USER_UPDAT    = 0xc4,            //请求用户更新
	    REQ_CTRL_RECRD    = 0xc5,            //请求录像控制
	    REQ_CHAN_UPDAT    = 0xc6,            //请求通道标题
	    REQ_BIOS_UPDAT    = 0xc7,            //刷新BIOS
	    REQ_FONT_UPDAT    = 0xc8,            //请求字库更新
	    REQ_PLAY_STOP     = 0xc9,            //停止播放文件
	    REQ_CTRL_PLAY     = 0xca,            //录像文件时间控制
	    REQ_RECD_DOWN     = 0xcb,            //录像文件下载
	    REQ_PLAY_PAUSE    = 0xcc,            //录像回放暂停
	    REQ_PROT_TITLE    = 0xcd,            //请求协议标题

	    REQ_CHAN_COLOR    = 0xcf,            //请求通道颜色

	    REQ_MODIFY_MAC = 0xd0,            //请求修改Mac地址
	    ACK_MODIFY_MAC = 0xd0,            //应答修改Mac地址

	    REQ_DEVICE_CTRL    = 0x60,              //请求设备控制,
	    ACK_DEVICE_CTRL    = 0x60,              //应答设备控制请求,
	    ACK_SEND_MESSAGE   = 0x66,              //发送录像消息,

	    REQ_TEST_DEVICE    = 0x62,            //测试设备, 第9字节为测试类型, 0为空,1表示测试smtp配置
	    REQ_INFO_PROC   = 0x61,            //信息处理, 把来自POS机的商品信息叠加到视频上
	    ACK_INFO_PROC   = 0x61,            //信息处理, 把来自POS机的商品信息叠加到视频上
	    
	    REQ_DEVICE_QUERY= 0x82,         //设备轮巡
	    ACK_DEVICE_QUERY= 0x82,         //返回设备状态或通道状态

	    REQ_USER_FORCEIFRAME = 0x80,    //强制I-FRAME的功能
	    ACK_USER_FORCEIFRAME = 0x80,    
	    REQ_USER_SETNETRATE  = 0x81,    //设置网络流量
	    ACK_USER_SETNETRATE  = 0x81,   

	    ACK_INFO_NORMAL      = 0x64,    //普通应答消息
	    REQ_SYSTEM_OPTQUERY  = 0x83,    //查询通道可选项配置
	    ACK_SYSTEM_OPTQUERY  = 0x83,    //应答通道可选项配置

	    REQ_ALARM_STAT  = 0x68,         //报警信息请求
	    ACK_ALARM_STAT  = 0x69,         //报警信息应答

	    REQ_UPLOAD_PIC_STAT  = 0x6a,         //上传定时抓拍图片
	    ACK_UPLOAD_PIC_STAT  = 0x6b,         //上传定时抓拍图片应答
	    


	    REQ_AUTO_REGISTER = 0xd1,
	    ACK_AUTO_REGISTER = 0xd1,
	    //应答
	    ACK_USER_LOGIN    = 0xb0,            //应答用户登陆
	    ACK_STAT_QUERY    = 0xb1,            //应答状态查询
	    ACK_LOGS_PARAM    = 0xb2,
	    ACK_CONF_PARAM    = 0xb3,            //应答系统配置
	    ACK_INFO_SYSTM    = 0xb4,            //应答系统信息
	    ACK_USER_PARAM    = 0xb5,            //应答用户配置
	    ACK_FILE_RECRD    = 0xb6,
	    ACK_FILE_ALARM    = 0xb7,            //应答报警文件
	    ACK_CHAN_TITLE    = 0xb8,            //应答通道标题
	    ACK_FONT_UPDAT    = 0xb9,            //应答字库更新
	    ACK_BIOS_UPDAT    = 0xba,
	    ACK_RECD_PLAY     = 0xbb,            //发送下载和回放结束命令字
	    ACK_CHAN_MONIT    = 0xbc,            //应答通道监视
	    ACK_SOCK_TEST     = 0xbd,            //用于测试
	    ACK_CTRL_SWTCH    = 0xbe,            //更改控制权
	    ACK_CTRL_ROBED    = 0xbf,            //强行获得控制权
	    ACK_SEND_AUDIO    = 0xc0,            //应答发送音频
	    ACK_CHAN_COLOR    = 0xCF,            //返回通道颜色
	    ACK_SEND_TXMOD    = 0xe0,            //传送方式
	    ACK_DISK_PARAM    = 0xe1,            //应答传送硬盘状态信息
	    ACK_FILE_PARAM    = 0xe2,            //发送文件信息
	    ACK_STAT_ALARM    = 0xe3,            //报警布防状态
	    ACK_PROT_TITLE    = 0xe4,            //应答协议标题

	    ACK_USER_UPDAT    = 0xe5,
	    ACK_MANUL_SNAP    = 0xe6,
	    
	    ACK_ERROR_RET     = 0xee,            // 应答一般错误消息

	    ACK_USER_STATE    = 0xf0,            //应答用户状态查询

	    //added by wyf on 091123
	    REQ_CONF_EXPORT = 0xf8,            //请求配置导出 
	    ACK_CONF_EXPORT = 0xf6,            //应答配置导出
	    REQ_CONF_IMPORT = 0xf5,            //请求配置导入
	    ACK_CONF_IMPORT = 0xfb,            //应答配置导入
	    //end added by wyf on 091123
	   
	    REQ_SUB_CONN      = 0Xf1,                //子连接请求
	    ACK_CHAN_MONIT_EX = 0xf2,//多画面预览，用于内部区分
	    REQ_TEST_ONLY    = 0xfe,            //测试用
	    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
	    /*-+-+-+-+-+-+-+-+         模拟DVR用         +-+-+-+-+-+-+-+-+-+-+-+-*/
	    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
	    SIMULATE_FILE_DOWNLOAD,            // 文件下载，模拟DVR专用

	    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
	    /*-+-+-+-+-+-+-+-+         内部命令         -+-+-+-+-+-+-+-+-+-+-+-+-*/
	    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
	    INTERNAL_DEFAULT_OPERATION    = 0xff01,    // 默认操作类
	    INTERNAL_RECORD_DOWNLOAD            ,    // 记录数据下载（回放&数据下载）
	    INTERNAL_CONNECTION_BROKEN            ,    // 连接断开处理
	    INTERNAL_SYSTEM_END_TASK            ,    //线程结束的任务


	    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
	};
#define TxStEnd -100
#define C_MAX_TCP_CONN_NUM 51
	/* 操作处理状态（预定的）*/
	enum    EnOpStatus
	{
	    OpStBegin =  101,
	    OpStBeginDataSafe,
	    /* 验证通过后做事 */
	    OpStEndDataSafe,
	    /* 检查参数等是否合法 */
	    OpStCheckValid,
	    /* 将这个弄的特殊些 */
	    OpStEnd = -1,
	};

	enum TLV_CMD
	{
	    REQ_NET_COMMON_CONFIG = 0x02,
	    SET_NET_COMMON_CONFIG = 0x03,
	    REQ_NET_APP_CONFIG =  0x04,
	    SET_NET_APP_CONFIG  = 0x05,
	    REQ_DEVALARM_CONFIG  = 0x06,
	    SET_DEVALARM_CONFIG  = 0x07,
	    REQ_DEVCHANNEL_CONFIG  = 0x08,
	    SET_DEVCHANNEL_CONFIG  = 0x09,
	    REQ_PTZCHANNEL_CONFIG  = 0x70,
	    SET_PTZCHANNEL_CONFIG  = 0x71,
	};

	typedef enum _enm_error_type_t
	{
	    ENM_SUCESS=0,//成功
	    Error_PasswdNotValid = 1, // 密码不正确
	    Error_AccountNotValid=2,// 帐号不存在
	    Error_AccountLogined = 3,//已经登录
	    Error_AccountLocked,//被锁定
	    Error_AccountInBlackList,//黑名单
	    Error_IDInValid,//ID 无效，服务器未分配
	}ENM_ERROR_TYPE;

	typedef enum _tlv_msg_type_t
	{
	    // 保留子类型
	    DEV_CHANNAL_TYPE = 0, /*通道，统一固定被占用*/
		DEV_SYSTEM_INFO_DEV_TYPE = 1, /* 2010-05-20 设备类型,为了配合web端由原来的20改为1。如果有需要
												可以跟web端商量改回*/
	    // 系统信息配置的子类型
	    DEV_SYSTEM_INFO_GENERAL = 10,        /* 普通信息 */
	    DEV_SYSTEM_INFO_DEV_ATTR,            /* 设备属性信息 */
	    DEV_SYSTEM_INFO_DISK_DRIVER,        /* 硬盘信息 */
	    DEV_SYSTEM_INFO_FILE_SYSTEM,        /* 文件系统信息 */
	    DEV_SYSTEM_INFO_VIDEO_ATTR,            /* 视频属性 */
	    DEV_SYSTEM_INFO_CHARACTER_SET,        /* 字符集信息 */
	    DEV_SYSTEM_INFO_OPTICS_STORAGE,        /* 光存储设备信息 */
	    DEV_SYSTEM_INFO_DEV_ID,                /* 设备序列号(ID) */
	    DEV_SYSTEM_INFO_DEV_VER,            /* 设备版本，字符串形式 */
	    DEV_SYSTEM_INFO_TALK_ATTR,    /* 语音对讲属性 */

	    //DEV_SYSTEM_INFO_DEV_TYPE= 20,        /* 设备类型 */
	    DEV_SYSTEM_INFO_PLATFORM=21,        /* 查询设备特殊协议(平台接入)支持信息 */
	    DEV_SYSTEM_INFO_SD_CARD,        /* SD卡信息 */
	    DEV_SYSTEM_INFO_MOTIONDETECT,    /* 设备视频动态检测属性信息 */
	    DEV_SYSTEM_INFO_VIDEOBLIND,    /* 视频区域遮挡属性信息 */
	    DEV_SYSTEM_INFO_CAMERA,        /* 摄像头属性信息 */
	    DEV_SYSTEM_INFO_WATERMARK ,        /* 查询图象水印能力 */
	    DEV_SYSTEM_INFO_WIRELESS,        /* 查询Wireless能力 */
	    DEV_SYSTEM_INFO_Language,        /* 查询支持的语言列表 */
	    DEV_SYSTEM_INFO_PICTURE,    /* 是否支持新的录像及图片列表查询方式 */

	    DEV_SYSTEM_INFO_DEV_ALL = 30,        /* 设备功能列表 */
	    DEV_SYSTEM_INFO_INFRARED,        /* 查询无线报警能力 */
	    DEV_SYSTEM_INFO_NEWLOGTYPE,    /* 是否支持新的日志格式 */
	    DEV_SYSTEM_INFO_OEM_INFO,        /* OEM信息 */
	    DEV_SYSTEM_INFO_NET_STATE,        /* 网络状态 */
	    DEV_SYSTEM_INFO_DEV_SNAP,        /* 设备抓图功能能力查询 */
	    DEV_SYSTEM_INFO_VIDEO_CAPTURE,    /* 视频前端采集能力查询 */
	    DEV_SYSTEM_INFO_SAVIA_COUNT,        /* SAVIA数目*/
	    DEV_SYSTEM_INFO_USER_AUTH = 39, /*登陆后返回该用户权限列表*/

	    DEV_SYSTEM_INFO_DSPCAP=40,            /*编码能力信息*/
	    DEV_SYSTEM_INFO_VIDEO_MATRIX,    /*本地矩阵控制配置*/
	    DEV_SYSTEM_INFO_DSP_MASK = 42,

	    DEV_SYSTEM_INFO_DISPLAY_SPLIT = 43,/*多画面预览分割配置*/
	    DEV_SYSTEM_INFO_DISPLAY_TOUR = 44,/*多画面轮训配置*/

	    DEV_SYSTEM_INFO_SPLIT_MODE = 45,
	    DEV_SYSTEM_INFO_SPLIT_COMBINE = 46,
	    DEV_SYSTEM_INFO_TOUR_CFG = 47,

	    DEV_SYSTEM_INFO_VIDEOUT_CFG = 48,    //!输出信息配置
	    DEV_SYSTEM_CUR_TIME=49,//时间

	    DEV_SYSTEM_INFO_DISKINFO =50,    //硬盘信息

	    DEV_SYSTEM_CTRL_DISKINFO =51,    //操作硬盘处理

	    DEV_SYSTEM_CTRL_DISKPARTION=52,   //操作硬盘分区处理
	    DEV_SYSTEM_DST_TIME = 53,         //夏令时时间调整
	    DEV_SYSTEM_VIDEO_OUT_OSD_CFG =54, //前端输出OSD叠加
	    DEV_SYSTEM_RECCTL_CFG =55,    //录像控制配置
	    DEV_SYSTEM_CHANNAME_CFG =56,                       //通道名
	    DEV_SYSTEM_AUDIO_CFG =57,//音频配置
	    DEV_SYSTEM_QUERYLOG =59,/*系统日志查询*/
	    DEV_SYSTEM_OPRLOG=60,/*前端日志操作规则*/
	    DEV_SYSTEM_QUERY_REC_PIC=61,/* 录像图片查询操作*/
	    // 能力集
	    DEV_ABILITY_DECODER = 100,//解码器
	    DEV_ABILITY_SAVIA,//智能模块
	    DEV_ABILITY_AUDIO_IN,//音频输入(有该能力则表示每个通道都有音频输入)
	    DEV_ABILITY_TALK,//对讲（对讲音频输入+音频输出）
	    DEV_ABILITY_WIRELESS,//WFI
	    DEV_ABILITY_3G,//3G
	    DEV_ABILITY_INFRARED,//红外
	    DEV_ABILITY_AUTO_REGISTER=107,//主动注册
	    DEV_ABILITY_PTZ_TOUR=108,//云台软巡航
	    DEV_ABILITY_SPLIT_TYPE = 109, //多画面预览模式，单画面，4画面等
	    //注意多画面巡航，只要在DEV_ ABILITY_SPLIT_TYPE返回的能力集合中DEV_ SYSTEM_INFO_SPLIT_COMBINE大于1，就存在多画面巡航操作
	    DEV_ABILITY_SPLIT_COMBINE = 110,
	    //多画面组合能力列表，每种模式下的组合方式

	    //抓拍能力集
	    DEV_ABILITY_SNAP=111,

	    //!报警联动能力集
	    DEV_ABILITY_AlarmLink=112,
	    //平台接入能力集
	    DEV_ABILITY_IVIDEO = 113,
	    DEV_ABILITY_ENCODE = 114,//设备编码能力
	    DEV_ABILITY_DEV_TIME = 115,//时间能力，夏令时，ntp等
	    DEV_ABILITY_NET_FTP =116,//ftp能力集
	    //黑白名单能力集added by wyf on 20100415
	    DEV_ABILITY_DEV_BLACKWHITEIP = 117,
	    //end added by wyf on 20100415
	    DEV_ABILITY_DEV_RTSP = 118,//rtsp 能力集
	    DEV_ABILITY_DEV_AUDIO = 119,//声音音量等的能力集，
	    DEV_ABILITY_DEV_RECORD = 120,//录像控制能力信集
	    DEV_ABILITY_KBD = 121,//控制键盘能力集
	    DEV_ABILITY_DEV_MATRIX = 123, //支持矩阵能力集
		DEV_ABILITY_OSD_ZOOM = 124,//OSD缩放能力集
		DEV_ABILITY_OSD_WATERMARK = 125,//数字水印功能
	    DEV_ABILITY_CAMERA = 129, //摄像头能力集
	    DEV_ABILITY_NEW_NET_OPR = 130,//整理新的网络控制操作能力集
	    DEV_ABILITY_ENCODE2 = 131,//设备编码能力2
	    DEV_ABILITY_OEM_FUN = 132, //OEM厂商特殊功能	
	    DEV_ABILITY_OSD_MULINE = 135,//多行OSD叠加
	    DEV_ABILITY_SUPPORT_ALARM = 136,//设备支持的告警类型
	  
	    DEV_SYSTEM_INFO_SUPERUSER_AUTH = 150, 	

	    // 网络配置的子类型
	    DEV_CONFIG_TYPE_NET       = 200,            /*网络配置*/
	    DEV_CONFIG_TYPE_MAIL      = 201,            /*邮件配置*/
	    DEV_CONFIG_TYPE_PPPOE     = 202,            /*PPPoE配置*/
	    DEV_CONFIG_TYPE_DDNS      = 203,            /*DDNS配置*/
	    DEV_CONFIG_TYPE_SNIFFER   = 204,        /*网络抓包配置*/
	    DEV_CONFIG_TYPE_FTP       = 205,            /*FTP配置*/
	    DEV_CONFIG_TYPE_NTP       = 206,            /*NTP配置*/
	    DEV_CONFIG_TYPE_LIMIT_BR  = 207,        /*限码流配置*/
	    DEV_CONFIG_TYPE_UPNP      = 208,
	    DEV_CONFIG_TYPE_BLACKWHITE = 209,

	    DEV_CONFIG_TYPE_DHCP = 210,      /*DHCP配置*/
	    DEV_CONFIG_TYPE_WEB_URL ,        /*抓图保存web路径配置*/
	    DEV_CONFIG_TYPE_FTP_PRO,         /*FTP配置*/

	    DEV_CONFIG_TYPE_ETHERNET,        /*网卡配置*/
	    DEV_CONFIG_TYPE_DNS,            /*DNS服务器配置*/
	    DEV_CONFIG_TYPE_WIRELESS_ADDR,    //无线报警配置,输出地址和遥控器地址
	    DEV_CONFIG_TYPE_WLAN_DEVICE,        //搜索无线设备
	    DEV_CONFIG_TYPE_MULTI_DDNS,        //多DDNS服务器配置
	    DEV_CFG_TYPE_DECODE,
	    DEV_CONFIG_TYPE_INTERVIDEO,        //平台接入配置

	    DEV_CONFIG_TYPE_WLAN = 220,                //查询无线配置
	    DEV_CONFIG_TYPE_AUTO_REGISTER,    // 主动注册参数配置
	    DEV_CONFIG_TYPE_MAC= 222,   //MAC地址
	    DEV_CFG_TYPE_DECODE_EX=224,   //解码器配置扩展
	    DEV_CFG_TYPE_NET_FTP_SERVER = 225, //ftp服务器配置
	    DEV_CFG_TYPE_NET_FTP_APP_TIME =226, // ftp应用时间段配置
	    DEV_CFG_TYPE_NET_RTSP = 227, //rtsp配置
	    DEV_CFG_TYPE_NET_LOCAL_CH_CFG =228, // 本地通道属性配置
	    DEV_CFG_TYPE_NET_REMOTE_CH_CFG =229,// 远程通道列表配置
	    DEV_CONFIG_TYPE_AUTOREG_EX=230, 
	    DEV_CONFIG_CAMERA=231, //2762使用，视频采集配置信息
	    DEV_CONFIG_CAMERA_EXT=232, //2762使用，视频采集配置信息
	    DEV_CONFIG_PTZ = 233,
	    DEV_CONFIG_PTZ_PROT = 234, //查询支持的云台协议    
	    DEV_CONFIG_LT_MOBILE = 235, //支持浪涛手机协议
	    // 告警配置的子类型
	    DEV_ALARM_ALARMIN_TYPE = 300, //本地告警输入
	    DEV_ALARM_NETIN_TYP = 301,//网络模拟告警输入
	    DEV_ALARM_MOTION_TYPE = 302,
	    DEV_ALARM_LOSS_TYPE = 303,
	    DEV_ALARM_BLIND_TYPE = 304,

	    DEV_ALARM_NODISK_TYPE = 305,
	    DEV_ALARM_DISKERROR_TYPE = 306,
	    DEV_ALARM_DISKFULL_TYPE = 307,

	    DEV_ALARM_NETBROKEN_TYPE = 308,
	    DEV_ALARM_DECODE_TYPE    = 309,
	    DEV_ALARM_WIRELESS_TYPE  = 310,
	    DEV_ALARM_SAVIA_TYPE     = 311,
	    DEV_ALARM_IPCONFICT_TYPE = 312,//ip冲突
	    DEV_ALARM_INT_REDON_TYPE = 313,//智能红色告警
	    DEV_ALARM_NET_ALARMIN    = 314,
	    
	    ALARM_ENABLE_TYPE        = 350,     //布防撤防
	    ALARM_WORKSTREET_TYPE    = 351, //告警工作表
	    ALARM_EVENT_TYPE         = 352,         //联动表
	    ALARM_MOTION_REGION_TYPE = 353, //动检灵敏度与区域
	    ALARM_REDON_REGION_TYPE = 354, //智能红色报警区域
		
	    // 通道配置的子类型
	    DEV_CHANNEL_ENCODE_TYPE = 400, //通道编码
	    DEV_CONFIG_TYPE_RECORD,            /*定时录象配置*/
	    DEV_CONFIG_TYPE_SNAP = 402,//抓拍参数配置    
	    DEV_CHANNEL_SVAC_TYPE = 403,//SVAC特色功能

	    ENCODE_CHANNEL_NAME_TYPE = 410, //通道名称
	    ENCODE_MAIN_STREAM_TYPE, //主码流参数
	    ENCODE_SUB_STREAM_TYPE,  //副码流参数
	    ENCODE_SNAP_STREAM_TYPE, //捉拍码流参数
	    ENCODE_COVER_TYPE,     //编码与预览遮挡
	    ENCODE_COLOR_TYPE,    //颜色
	    ENCODE_TIMEOSD_TYPE,   //OSD叠加
	    ENCODE_TITLEOSD_TYPE = 418,   //OSD叠加
	    ENCODE_OSD_SIZE = 419,//by ilena OSD大小控制
		ENCODE_SVAC_FOCUS_AREA_TYPE = 420,//SVAC聚焦区域
		ENCODE_OSD_WATERMARK = 421,

	    DEV_MANUAL_SNAP = 422,//手动抓拍功能
	    ENCODE_COLOR_TYPE_EX = 423,    //颜色扩展
	    ENCODE_COLOR_TYPE_EX_TABLE = 424,    //颜色扩展 
	    ENCODE_CHANNEL_MUTILE_TITLE = 425, //多行OSD叠加
	    
	    // 云台配置的子类型
	    DEV_PTZ_PRESET_TYPE = 500, //云台预置点
	    DEV_PTZ_TOUR_TYPE =501,//云台巡航轨迹
	    DEV_CONFIG_TYPE_SERIAL_COMM,    /*串口配置*/

	    PTZ_QUERY_PRESET_TYPE =510, //云台预置点
		PTZ_QUERY_TOUR_TYPE,//云台巡航轨迹
	    PTZ_ADD_PRESET_TYPE, //添加预置点
	    PTZ_CLEAR_PRESET_TYPE, //清除预置点
	    PTZ_ADD_TOUR_PRESET_TYPE, //添加预置点到巡航轨迹

	    PTZ_CLEAR_TOUR_PRESET_TYPE, //清除预置点到巡航轨迹
	    PTZ_CLEAR_TOUR_TYPE = 516, //清除某个巡航轨迹


	    VIDEOUT_CFG_TV = 517, //!tv配置
	    PTZ_RAW_DATA  = 518,//透明通道控制
	    DEV_CONFIG_TYPE_KBD_TYPE  = 519,//控制键盘配置

	    
	    //平台接入子类型
	    INTERVIDEO_TYPE_ZXBELL =550 ,  // 中星BELL  
	    INTERVIDEO_TYPE_MEGA =551 ,  //互信
	    INTERVIDEO_TYPE_VS300 = 552,//VS300
	    INTERVIDEO_TYPE_SHENYAN = 553,//神眼
	    INTERVIDEO_TYPE_KOTI = 554,//KOTI
	    INTERVIDEO_TYPE_BJUN =555 ,  // 北京网通
		INTERVIDEO_TYPE_GAYS =556 ,  //公安一所
		INTERVIDEO_TYPE_HTTP =557 ,  //http
		INTERVIDEO_TYPE_VISS30 =558 , 

		INTERVIDEO_TYPE_BLUESTAR =559 ,  //bluestar
		INTERVIDEO_TYPE_HAOWEI = 560,   //豪威 
		INTERVIDEO_TYPE_SHSS = 562,   //三所检测
		//视频参数设置子类型
		CAMERA_WHITEBALANCE_TYPE = 600,
		
		//明景球机参数配置
		MJ_CAMERA_TYPE = 601,
		MJ_CAMERA_ADVANCE_TYPE = 602,  //高级
		MJ_CAMERA_AUTOHOME_TYPE = 603,
		MJ_CAMERA_BLIND_TYPE = 604,
		MJ_CAMERA_POSTION_TYPE = 605,
		MJ_CAMERA_PTZ_TYPE = 606,
		CAMERA_RESOLUTION_TYPE = 607,
		CAMERA_DRC_TYPE = 608,	
		CAMERA_SUPPER_CONFIG_TYPE =609,
		CAMERA_BADPIXEL_DETECT = 610 ,
		
		KOTI_FLOOR_TYPE =650,/* 楼层参数 */
		KOTI_ROOM_TYPE =651,/* 房间参数 */
		KOTI_IP_CAM_NAME_TYPE =652,/* 摄像头参数 */
		KOTI_APARTMENT_CFG_TYPE =653,/* 防区位置参数 */
		KOTI_ALARM_AREA_TYPE =654,/* 防区设置参数 */
		KOTI_MOBILE_NUM_TYPE =655,/* 报警电话参数 */
		KOTI_SYSINFO_TYPE =656,/* 系统参数 */
		KOTI_ALARM_TIME_TYPE =657,/* 安防参数 */
		KOTI_PASSWORD_TYPE = 658,/* 安防密码参数 */
		KOTI_RTSP_CONG_TYPE = 659,/* 平台对接参数 */
	  
		DEV_CONIFG_CARD_RECORD =660,	/*卡号录像*/

		ENCODE_3IRD_STREAM_TYPE = 680,  //副码流参数-对应CHL_3IRD_T
		ENCODE_4RTH_STREAM_TYPE = 618, //副码流参数-对应CHL_4RTH_T
		ENCODE_5FTH_STREAM_TYPE = 682,
		
	    MINGJING_CONFIG_TYPE = 690, //明景参数配置
	    DEV_CONFIG_TYPE_WRIELESS = 691,  //无线配置
	    SUVA_CONFIG_TYPE = 700,         //智能报警配制
		SUVA_AREA_CONFIG_TYPE = 701,    //智能区域设置信息
	    DEV_ABILITY_SUVA = 702 ,         //智能能力集   
	    DEV_ABILITY_SUPPORT_COLOR_TYPE = 703,          //图像颜色风格能力集
	    DEV_ABILITY_SUPPORT_SENSOR_TYPE = 704,   //sensor 支持的 分别率 和制式
	    DEV_ABILITY_SUPPORT_EXSTREAM_TYPE = 705, //辅码流录像能力集
	    DEV_EXSTREAM_CFG = 706,                  //辅码流录像配制
	    DEV_ENCODER_DETECT_CFG = 707,       

#if defined(GUIZHOU_G_PROJECT) || defined(POLICE_PROJECT)
		DEV_MUSIC_FILE_SEARCH_TYPE = 710,			//获取文件列表
		DEV_MUSIC_PLAY_PROJECT_TYPE = 711,			//获取播放计划
		DEV_MUSIC_PLAY_STATUS_TYPE  = 713,
#endif
#ifdef POLICE_PROJECT 
		DEV_LED_STATUS_TYPE  = 714,
#endif
#if defined(GUIZHOU_G_PROJECT) || defined(POLICE_PROJECT)
		DEV_WLAN_INFO_TYPE = 715,	
#endif
		DEV_GOOLINK_TYPE = 716,
	//  750  段为NVR  远程通道配置使用 
	    NVR_IPC_LOGIN_TYPE = 750,  //查询远程通道 是否登录    
	    
	}ENM_MSG_TYPE;
}
#endif // BASETYPEDEF_H


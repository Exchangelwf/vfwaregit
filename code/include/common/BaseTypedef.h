
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

	/* ͨѶЭ�� */

#define    DVRIP_VERSION    5

	typedef struct dvrip
	{

		unsigned char head_token[8]; 	/*="WJAPRIVA"  ,��ͷ��ʾ*/
		unsigned char    dvrip_cmd;        /* command  ���� */
		unsigned char    dvrip_r0;        /* reserved ����*/
		unsigned char    dvrip_r1;        /* reserved */
#if BYTE_ORDER == LITTLE_ENDIAN 
		unsigned char    dvrip_hl:4,        /* header length */
		                dvrip_v :4;        /* version */
#endif
#if BYTE_ORDER == BIG_ENDIAN 
		unsigned char    dvrip_v :4,        /* version */
		                dvrip_hl:4;        /* header length */
#endif
		unsigned int    dvrip_extlen;    /* ext data length  ��չ���� */
		unsigned char    dvrip_p[24];    /* inter dvrip.dvrip_p ���� */
	}DVRIP;

#define DVRIP_HEAD_T_SIZE sizeof(DVRIP)

#define ZERO_DVRIP_HEAD_T(X) memset((X), 0, DVRIP_HEAD_T_SIZE);            \
	                            (X)->dvrip_hl = DVRIP_HEAD_T_SIZE/4;    \
	                            (X)->dvrip_v = DVRIP_VERSION;

	//
	// ��ѯϵͳ������Ϣ
	// ע��˳���ܸı�
	//
	typedef enum __cfg_index_t {
	    CFG_GENERAL =  0,    // ��ͨ
	    CFG_COMM           ,    // ����
	    CFG_NET            ,    // ����
	    CFG_RECORD         ,    // ¼��        /*!< 2.42�Ժ�������ã�ʹ��CFG_NEWRECORD */
	    CFG_CAPTURE        ,    // ͼ������    /*!< 2.42�Ժ�������ã�ʹ��CFG_CURRENTCAPTURE */
	    CFG_PTZ            ,    // ��̨
	    CFG_DETECT         ,    // ��̬���    /*!< 2.42�Ժ�������ã�ʹ��CFG_EVENT */ 
	    CFG_ALARM          ,    // ����        /*!< 2.42�Ժ�������ã�ʹ��CFG_EVENT */ 
	    CFG_DISPLAY        ,    // ��ʾ
	    CFG_TITLE        = 10,       // ͨ������
	    CFG_MAIL         = 11,       // �ʼ�����
	    CFG_EXCAPTURE    = 12,       // Ԥ��ͼ������
	    CFG_PPPOE        = 13,       // pppoe����
	    CFG_DDNS         = 14,       // DDNS����
	    CFG_SNIFFER      = 15,       // ������Ӳ�������
	    CFG_DSPINFO      = 16,      // ����������Ϣ
#ifdef DEF_MATRIX_INPUT
	    CFG_MATRIXINPUT = 41, //��������20100825
#endif
	    CFG_NET_NTP = 19, /*ntp����*/
	    CFG_NET_UPNP     = 21,
	    CFG_FTP          = 37,//17,       //FTP����
	    CFG_AUTOMAINTAIN = 18,      //�Զ�ά��
	    CFG_DSPBITRATE   = 24,    // DSP�������
	    CFG_MATRIX       = 30,
	    CFG_VIDEOCOVER   = 34,    //�����ڵ�
	    CFG_DHCP         = 35,
	    CFG_WEB          = 36,
	    CFG_WLANIP       = 39,    //����ģ��IP����
	    CFG_DNS          = 40,
	    CFG_RECDDOWNLOAD = 42,
	    CFG_AUDIOFORMAT  = 43,        //��Ƶ�����ʽ
		CFG_SYSTEMLOG  = 44,		  //��־����

#ifdef FUNC_MUTILINE_TITLE
	    CFG_MUTITLE        = 45,   //����OSD����
#endif 
#ifdef DEF_MATRIX_INPUT
	    DEV_SYSTEM_INFO_XH_CAMERID =58, /*�ǻ��������ͷͨ������*/
#endif
	    CFG_DIGI_CHANNEL = 61,
	    CFG_IPCONFICT =62,
	    CFG_NEWRECORD    = 123,        /*!< �µ�¼������ */
	    CFG_EVENT        = 124,       //�¼�����
	    CFG_WORKSHEET    = 125,     //ʱ���������Ϣ
	    CFG_COLOR        = 126,     // ��ɫ������Ϣ
	    CFG_CURRENTCAPTURE = 127,  // ��ǰ��������,Ϊ����˫�������ܺ����ӵ�
	    CFG_NEWCAPTURE     = 128,  // �����µ�ͼ�����ýṹ
	    CFG_CAMERA = 129,
	     CFG_WHITEBALANCE = 130,
	    CFG_WLAN           = 131,    //wifi config
	    CFG_TRANSFER_POLICY  = 133,//���紫��������� 
	    CFG_WLANSCAN         = 135, //wifi scanning
	    CFG_NEWDDNS        = 140,        //�µ�ddns
	    CFG_DECODE         = 0x8d, //141, ������
	    CFG_VSP            = 190,   //��������������
	    CFG_CAMIPC         = 200,   //CAMIPC����
	    CFG_LOCATION       = 201,    /*!< �������� */
	    CFG_GUISET         = 202,    /*!< GUI���� */
	    CFG_RECWORKSHEET   = 203,     /*!< ¼���������� */
	    CFG_PTZALARM       = 204,    /*!< ��̨�����豸Э������ */
	    CFG_NETALARM       = 205,    /*!< ���籨�� */
	    CFG_ALMWORKSHEET   = 206,    /*!< �������������� */
	    CFG_MTDWORKSHEET   = 207,    /*!< ���칤�������� */
	    CFG_BLDWORKSHEET   = 208,    /*!< �ڵ����������� */
	    CFG_VLTWORKSHEET   = 209,    /*!< ��Ƶ��ʧ���������� */
	    CFG_PRESETNAME     = 210,    /*!< ��̨Ԥ֪���� */
	    CFG_PTZTOUR        = 211,    /*!< Ѳ������ */
	    CFG_LOSS           = 212,    /*!< ��Ƶ��ʧ */
	    CFG_BLIND          = 213,   /*!< ��Ƶ�ڵ� */
	    CFG_PLAY           = 214,    /*!< �ط����� */
	    CFG_USER           = 215,    /*!< �û����� */
	    CFG_GROUP          = 216,    /*!< �û������� */
	    CFG_MONITORTOUR    = 217,    /*!< ������ѵ */
	    CFG_TVADJUST       = 218,    /*!< TV���� */
	    CFG_ATM            = 219,    /*!< ATM���� */
	    CFG_STNOTEXIST     = 220,    /*!< ��Ӳ�� */
	    CFG_STFAILURE      = 221,    /*!< Ӳ�̳��� */
	    CFG_STLOWSPACE     = 222,    /*!< Ӳ�̿ռ䲻�� */
	    CFG_NETABORT       = 223,    /*!< �����¼����� */
	    CFG_VIDEOWIDGET    = 224,    /*!< ��Ƶװ�� */
	    CFG_VIDEOMATRIX    = 225,    /*!< ��Ƶ���� */
#ifdef FUNCTION_SAVIA_INTELLIGENT  //add langzi 2009-12-17 savia���ܺ궨��
	    CFG_INTELLIGENT    = 226,    /*!< ���ܱ��� add langzi 2009-12-10*/ 
	    CFG_INTWORKSHEET   = 227,    /*!< ���ܱ������������� add langzi 2009-12-10*/ 
#endif //end langzi
	    CFG_SNAPWORKSHEET = 228,
	    CFG_NETALMWORKSHEET =229,
	    CFG_NET_COMMON = 230, /*ͨ����������*/
	    CFG_NET_FTPSERVER =231, /*FTP����������*/
	    CFG_NET_FTPAPPLICATION =232, /*ftpӦ������*/
	    CFG_NET_IPFILTER =233, /*IP��������*/
	    CFG_NET_MULTICAST =234, /*�鲥����*/
	    CFG_NET_PPPOE =235, /*pppoe����*/
	    CFG_NET_DDNS =236, /*ddns����*/
	    CFG_NET_ALARMSERVER =237, /*������������*/
	    CFG_NET_EMAIL =238, /*email����*/
	    CFG_NET_SNIFFER =239, /*ץ������*/

#ifdef VSERVER   //add by kyle xu in 20160107    
	    CFG_NET_VSERVER = 240, /*�ǹ�ƽ̨����*/
#endif
	    
	    CFG_REG_SERVER = 241,/*����ע�����������*/
	    CFG_GOOLINK = 242, /*GOOLINK */
	    CFG_RECState = 245, /*��������¼��״̬*/

	    //�ĳ�ͨ��
	    CFG_NET_PRESET=246,/* Ԥ�õ�������Ϣ */
	    CFG_NET_TOUR=247,/* Ѳ��������Ϣ */
	    CFG_CONFIG_PTZREGRESS=248, /* Ԥ�õ�ع���Ϣ */
	    CFG_OSD_PARAM = 249, /*OSD����*/
	    
#if defined SHBELL || defined ZXBELL|| defined ZXBELL30 || defined VIMICRO || defined SHBELL29 || defined HTTP_VS|| defined GAYS ||defined ZXNVR
	    CFG_NET_SHBELL=249,/* �Ϻ�BELL���ŵ�������Ϣ */
	    #ifdef __BELL_PICSHOOT__
	    CFG_CONFIG_UPLOAD_PICTURE=250, /* ����ͼƬ�ϴ�������Ϣ */
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
	    CFG_NET_MEGAEYES_PRESET=254,/* ���Ż�ͨ��Ԥ�õ�������Ϣ */
	    CFG_NET_MEGAEYES_TOUR=255,/* ���Ż�ͨ��Ѳ��������Ϣ */
	    CFG_MEGA_CAMERAID=256,
	    CFG_FTP_TIME=257,
	    CFG_USER_ID=258,
#endif

#ifdef ZTE_NETVIEW
	    CFG_NET_NETVIEW=259,/*������ά���������*/
	    CFG_OSD_NETVIEW=260,/*������ά����OSD������*/
	    CFG_NET_NETVIEW_PRESET=261,/*������ά��Ԥ�õ�������Ϣ */
	    CFG_NET_NETVIEW_TOUR,/* ������ά��Ѳ��������Ϣ */
#if defined(_NETVIEW_EXTEND_)
	    CFG_ELSE_NETVIEW=262,
#endif
#if defined(CNC_SJZ)
	    CFG_PPWORKSHEET=263,
#endif
#endif

#if defined(_ZTE_APP_)
		CFG_NET_ZTENJ = 264,		/*������������������������*/
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

		CFG_PRESET_TITLE = 307,  //��Ƶ������ָ������


		CFG_MJ_CAMERA,
		CFG_MJ_ADV_CAMERA,
		CFG_MJ_AUTOHOME,
		CFG_MJ_BLIND,
		CFG_MJ_PTZ,
		CFG_SUVA_DETECT,
		CFG_SUVA_WORKSHEET,
		CFG_RRCORD_MULTI_STREAM,

	    CFG_COLOR_EX        = 320, /* ͼ����ɫ��չ���� */
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
		CFG_OUT_COlOR_SET = 327,    /*!< TV���� */
#ifdef LANGTAO
		CFG_LANGTAO_MOBILE = 328 //�����ֻ�Э��
#endif
	}CFG_INDEX;

	//!��Ʒ��� 
	enum product_t {
	    /* �Ӻ� */
	    DH_DVR_H    =        0,
	    /* ��ͨ */
	    DH_DVR_G    =        1,
	    /* ���� */
	    DH_DVR_L    =        2,
	    /* ��ʵʱ */
	    DH_DVR_N    =        3,
	    /* ATM���� */
	    DH_DVR_ATM    =        4,
	    /* ������Ƶ������ */
	    DH_NVS        =        5,
	    /* ��Ƶ������0404e */
	    DH_NVS_E    =        6,
	    /*Bϵ��NVS*/
	    DH_NVS_B    =        10,
	    /*Hϵ��NVS*/
	    DH_NVS_C    =        11,

	    DH_CAM_SDIP    =        50,
	    DH_CAM_IPC    =        51,
	    DH_HISI_IPC    =        55,

	    /*A6����*/
	    DH_DVR_A6   =       60,

		//��Ǭ��Ʒ��� 
		VD_DVR      =       100,
		VD_IPC      =       101,
		VD_DVS      =       102,
		VD_DECODER  =       103,
		VD_DOME    =    104

	};

#define MAX_FILE_NUM_ONCE        16        //�����ѯ��һ�����16���ļ�

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
	    //����
	    REQ_SYSTEM_CONFIG     = 0x00,
	    ACK_REQ_SYSTEM_CONFIG = 0x00,
	    SET_SYSTEM_CONFIG     = 0x01,
	    ACK_SET_SYSTEM_CONFIG = 0x01,
	    REQ_REMOTE_SYSTEM_CONFIG     = 0x02,
	    SET_REMOTE_SYSTEM_CONFIG     = 0x03,
	    
	    REQ_UPDATE_LOGIN  = 0x10,            //�����ͻ��˵�¼
	    REQ_CHAN_SWTCH    = 0x11,            //��Ƶ�л�
	    REQ_CTRL_PANTZ    = 0x12,            //��̨����, Ԥ�õ�����, �ƹ����
	    REQ_MODE_FOCUS    = 0x13,
	    REQ_CTRL_FOCUS    = 0x14,
	    REQ_CTRL_ZOOM     = 0x15,
	    REQ_CTRL_IMAGE    = 0x16,
	    REQ_CTRL_MONSW    = 0x17,            //������(�ָ�)�л�
	    REQ_CTRL_IRIS     = 0x18,
	    REQ_CTRL_PRESET   = 0x19,
	    REQ_CTRL_ALARM    = 0x1a,
	    REQ_CTRL_LAMP     = 0x1b,
	    REQ_CTRL_MENU     = 0x1c,
	    REQ_SEND_AUDIO    = 0x1d,            //Ҫ������Ƶ
	    REQ_MODE_XCHGE    = 0x1e,            //���ô��䷽ʽ
	    REQ_SEND_TXMOD    = 0x1f,            //���ʹ��䷽ʽ
	    REQ_COMM_SEND     = 0x20,            //͸�����ڴ���
	    REQ_CTRL_RESET    = 0x21,
	    REQ_DISK_PARAM    = 0x22,            //Ҫ����Ӳ��״̬��Ϣ
	    REQ_ALARM_INFO    = 0x23,            //���󱨾��������״̬
	    REQ_SET_DTIME     = 0x24,            //DVRʱ������
	    REQ_EXT_FUNCTION  = 0x25,
	    REQ_NET_KEYBOARD_CTRL = 0x98,   /*!< �µ�������̿��� */
	    REQ_BOARD_CTRL        = 0x99,            //���̿���

	    REQ_USER_LOGIN    = 0x0a,
	    REQ_STAT_QUERY    = 0xa1,            //a1 ����״̬��ѯ
	    REQ_LOGS_PARAM    = 0xa2,            //������־��Ϣ
	    REQ_CONF_PARAM    = 0xa3,            //����ϵͳ����
	    REQ_INFO_SYSTM    = 0xa4,            //����ϵͳ��Ϣ
	    REQ_RECD_QUERY    = 0xa5,            //��ѯ¼���ļ�
	    REQ_USER_PARAM    = 0xa6,            //��ѯ�û���Ϣ
	    REQ_FILE_ALARM    = 0xa7,            //��ѯ�����ļ�
	    REQ_CHAN_TITLE    = 0xa8,            //�����ͺ���
	    REQ_CTRL_SWTCH    = 0xa9,            //����ı����Ȩ

	    REQ_AUDIO_DATA    = 0xc0,            //�Խ�����
	    ACK_REQ_AUDIO_DATA    = 0x1d,            //�Խ�����

	    REQ_CONF_UPDAT    = 0xc1,            //�������ø���
	    REQ_RECD_PLAY     = 0xc2,            //¼���ļ��ط�
	    REQ_PLAY_ALERT    = 0xc3,            //���ű����ļ�
	    REQ_USER_UPDAT    = 0xc4,            //�����û�����
	    REQ_CTRL_RECRD    = 0xc5,            //����¼�����
	    REQ_CHAN_UPDAT    = 0xc6,            //����ͨ������
	    REQ_BIOS_UPDAT    = 0xc7,            //ˢ��BIOS
	    REQ_FONT_UPDAT    = 0xc8,            //�����ֿ����
	    REQ_PLAY_STOP     = 0xc9,            //ֹͣ�����ļ�
	    REQ_CTRL_PLAY     = 0xca,            //¼���ļ�ʱ�����
	    REQ_RECD_DOWN     = 0xcb,            //¼���ļ�����
	    REQ_PLAY_PAUSE    = 0xcc,            //¼��ط���ͣ
	    REQ_PROT_TITLE    = 0xcd,            //����Э�����

	    REQ_CHAN_COLOR    = 0xcf,            //����ͨ����ɫ

	    REQ_MODIFY_MAC = 0xd0,            //�����޸�Mac��ַ
	    ACK_MODIFY_MAC = 0xd0,            //Ӧ���޸�Mac��ַ

	    REQ_DEVICE_CTRL    = 0x60,              //�����豸����,
	    ACK_DEVICE_CTRL    = 0x60,              //Ӧ���豸��������,
	    ACK_SEND_MESSAGE   = 0x66,              //����¼����Ϣ,

	    REQ_TEST_DEVICE    = 0x62,            //�����豸, ��9�ֽ�Ϊ��������, 0Ϊ��,1��ʾ����smtp����
	    REQ_INFO_PROC   = 0x61,            //��Ϣ����, ������POS������Ʒ��Ϣ���ӵ���Ƶ��
	    ACK_INFO_PROC   = 0x61,            //��Ϣ����, ������POS������Ʒ��Ϣ���ӵ���Ƶ��
	    
	    REQ_DEVICE_QUERY= 0x82,         //�豸��Ѳ
	    ACK_DEVICE_QUERY= 0x82,         //�����豸״̬��ͨ��״̬

	    REQ_USER_FORCEIFRAME = 0x80,    //ǿ��I-FRAME�Ĺ���
	    ACK_USER_FORCEIFRAME = 0x80,    
	    REQ_USER_SETNETRATE  = 0x81,    //������������
	    ACK_USER_SETNETRATE  = 0x81,   

	    ACK_INFO_NORMAL      = 0x64,    //��ͨӦ����Ϣ
	    REQ_SYSTEM_OPTQUERY  = 0x83,    //��ѯͨ����ѡ������
	    ACK_SYSTEM_OPTQUERY  = 0x83,    //Ӧ��ͨ����ѡ������

	    REQ_ALARM_STAT  = 0x68,         //������Ϣ����
	    ACK_ALARM_STAT  = 0x69,         //������ϢӦ��

	    REQ_UPLOAD_PIC_STAT  = 0x6a,         //�ϴ���ʱץ��ͼƬ
	    ACK_UPLOAD_PIC_STAT  = 0x6b,         //�ϴ���ʱץ��ͼƬӦ��
	    


	    REQ_AUTO_REGISTER = 0xd1,
	    ACK_AUTO_REGISTER = 0xd1,
	    //Ӧ��
	    ACK_USER_LOGIN    = 0xb0,            //Ӧ���û���½
	    ACK_STAT_QUERY    = 0xb1,            //Ӧ��״̬��ѯ
	    ACK_LOGS_PARAM    = 0xb2,
	    ACK_CONF_PARAM    = 0xb3,            //Ӧ��ϵͳ����
	    ACK_INFO_SYSTM    = 0xb4,            //Ӧ��ϵͳ��Ϣ
	    ACK_USER_PARAM    = 0xb5,            //Ӧ���û�����
	    ACK_FILE_RECRD    = 0xb6,
	    ACK_FILE_ALARM    = 0xb7,            //Ӧ�𱨾��ļ�
	    ACK_CHAN_TITLE    = 0xb8,            //Ӧ��ͨ������
	    ACK_FONT_UPDAT    = 0xb9,            //Ӧ���ֿ����
	    ACK_BIOS_UPDAT    = 0xba,
	    ACK_RECD_PLAY     = 0xbb,            //�������غͻطŽ���������
	    ACK_CHAN_MONIT    = 0xbc,            //Ӧ��ͨ������
	    ACK_SOCK_TEST     = 0xbd,            //���ڲ���
	    ACK_CTRL_SWTCH    = 0xbe,            //���Ŀ���Ȩ
	    ACK_CTRL_ROBED    = 0xbf,            //ǿ�л�ÿ���Ȩ
	    ACK_SEND_AUDIO    = 0xc0,            //Ӧ������Ƶ
	    ACK_CHAN_COLOR    = 0xCF,            //����ͨ����ɫ
	    ACK_SEND_TXMOD    = 0xe0,            //���ͷ�ʽ
	    ACK_DISK_PARAM    = 0xe1,            //Ӧ����Ӳ��״̬��Ϣ
	    ACK_FILE_PARAM    = 0xe2,            //�����ļ���Ϣ
	    ACK_STAT_ALARM    = 0xe3,            //��������״̬
	    ACK_PROT_TITLE    = 0xe4,            //Ӧ��Э�����

	    ACK_USER_UPDAT    = 0xe5,
	    ACK_MANUL_SNAP    = 0xe6,
	    
	    ACK_ERROR_RET     = 0xee,            // Ӧ��һ�������Ϣ

	    ACK_USER_STATE    = 0xf0,            //Ӧ���û�״̬��ѯ

	    //added by wyf on 091123
	    REQ_CONF_EXPORT = 0xf8,            //�������õ��� 
	    ACK_CONF_EXPORT = 0xf6,            //Ӧ�����õ���
	    REQ_CONF_IMPORT = 0xf5,            //�������õ���
	    ACK_CONF_IMPORT = 0xfb,            //Ӧ�����õ���
	    //end added by wyf on 091123
	   
	    REQ_SUB_CONN      = 0Xf1,                //����������
	    ACK_CHAN_MONIT_EX = 0xf2,//�໭��Ԥ���������ڲ�����
	    REQ_TEST_ONLY    = 0xfe,            //������
	    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
	    /*-+-+-+-+-+-+-+-+         ģ��DVR��         +-+-+-+-+-+-+-+-+-+-+-+-*/
	    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
	    SIMULATE_FILE_DOWNLOAD,            // �ļ����أ�ģ��DVRר��

	    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
	    /*-+-+-+-+-+-+-+-+         �ڲ�����         -+-+-+-+-+-+-+-+-+-+-+-+-*/
	    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
	    INTERNAL_DEFAULT_OPERATION    = 0xff01,    // Ĭ�ϲ�����
	    INTERNAL_RECORD_DOWNLOAD            ,    // ��¼�������أ��ط�&�������أ�
	    INTERNAL_CONNECTION_BROKEN            ,    // ���ӶϿ�����
	    INTERNAL_SYSTEM_END_TASK            ,    //�߳̽���������


	    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
	};
#define TxStEnd -100
#define C_MAX_TCP_CONN_NUM 51
	/* ��������״̬��Ԥ���ģ�*/
	enum    EnOpStatus
	{
	    OpStBegin =  101,
	    OpStBeginDataSafe,
	    /* ��֤ͨ�������� */
	    OpStEndDataSafe,
	    /* ���������Ƿ�Ϸ� */
	    OpStCheckValid,
	    /* �����Ū������Щ */
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
	    ENM_SUCESS=0,//�ɹ�
	    Error_PasswdNotValid = 1, // ���벻��ȷ
	    Error_AccountNotValid=2,// �ʺŲ�����
	    Error_AccountLogined = 3,//�Ѿ���¼
	    Error_AccountLocked,//������
	    Error_AccountInBlackList,//������
	    Error_IDInValid,//ID ��Ч��������δ����
	}ENM_ERROR_TYPE;

	typedef enum _tlv_msg_type_t
	{
	    // ����������
	    DEV_CHANNAL_TYPE = 0, /*ͨ����ͳһ�̶���ռ��*/
		DEV_SYSTEM_INFO_DEV_TYPE = 1, /* 2010-05-20 �豸����,Ϊ�����web����ԭ����20��Ϊ1���������Ҫ
												���Ը�web�������Ļ�*/
	    // ϵͳ��Ϣ���õ�������
	    DEV_SYSTEM_INFO_GENERAL = 10,        /* ��ͨ��Ϣ */
	    DEV_SYSTEM_INFO_DEV_ATTR,            /* �豸������Ϣ */
	    DEV_SYSTEM_INFO_DISK_DRIVER,        /* Ӳ����Ϣ */
	    DEV_SYSTEM_INFO_FILE_SYSTEM,        /* �ļ�ϵͳ��Ϣ */
	    DEV_SYSTEM_INFO_VIDEO_ATTR,            /* ��Ƶ���� */
	    DEV_SYSTEM_INFO_CHARACTER_SET,        /* �ַ�����Ϣ */
	    DEV_SYSTEM_INFO_OPTICS_STORAGE,        /* ��洢�豸��Ϣ */
	    DEV_SYSTEM_INFO_DEV_ID,                /* �豸���к�(ID) */
	    DEV_SYSTEM_INFO_DEV_VER,            /* �豸�汾���ַ�����ʽ */
	    DEV_SYSTEM_INFO_TALK_ATTR,    /* �����Խ����� */

	    //DEV_SYSTEM_INFO_DEV_TYPE= 20,        /* �豸���� */
	    DEV_SYSTEM_INFO_PLATFORM=21,        /* ��ѯ�豸����Э��(ƽ̨����)֧����Ϣ */
	    DEV_SYSTEM_INFO_SD_CARD,        /* SD����Ϣ */
	    DEV_SYSTEM_INFO_MOTIONDETECT,    /* �豸��Ƶ��̬���������Ϣ */
	    DEV_SYSTEM_INFO_VIDEOBLIND,    /* ��Ƶ�����ڵ�������Ϣ */
	    DEV_SYSTEM_INFO_CAMERA,        /* ����ͷ������Ϣ */
	    DEV_SYSTEM_INFO_WATERMARK ,        /* ��ѯͼ��ˮӡ���� */
	    DEV_SYSTEM_INFO_WIRELESS,        /* ��ѯWireless���� */
	    DEV_SYSTEM_INFO_Language,        /* ��ѯ֧�ֵ������б� */
	    DEV_SYSTEM_INFO_PICTURE,    /* �Ƿ�֧���µ�¼��ͼƬ�б��ѯ��ʽ */

	    DEV_SYSTEM_INFO_DEV_ALL = 30,        /* �豸�����б� */
	    DEV_SYSTEM_INFO_INFRARED,        /* ��ѯ���߱������� */
	    DEV_SYSTEM_INFO_NEWLOGTYPE,    /* �Ƿ�֧���µ���־��ʽ */
	    DEV_SYSTEM_INFO_OEM_INFO,        /* OEM��Ϣ */
	    DEV_SYSTEM_INFO_NET_STATE,        /* ����״̬ */
	    DEV_SYSTEM_INFO_DEV_SNAP,        /* �豸ץͼ����������ѯ */
	    DEV_SYSTEM_INFO_VIDEO_CAPTURE,    /* ��Ƶǰ�˲ɼ�������ѯ */
	    DEV_SYSTEM_INFO_SAVIA_COUNT,        /* SAVIA��Ŀ*/
	    DEV_SYSTEM_INFO_USER_AUTH = 39, /*��½�󷵻ظ��û�Ȩ���б�*/

	    DEV_SYSTEM_INFO_DSPCAP=40,            /*����������Ϣ*/
	    DEV_SYSTEM_INFO_VIDEO_MATRIX,    /*���ؾ����������*/
	    DEV_SYSTEM_INFO_DSP_MASK = 42,

	    DEV_SYSTEM_INFO_DISPLAY_SPLIT = 43,/*�໭��Ԥ���ָ�����*/
	    DEV_SYSTEM_INFO_DISPLAY_TOUR = 44,/*�໭����ѵ����*/

	    DEV_SYSTEM_INFO_SPLIT_MODE = 45,
	    DEV_SYSTEM_INFO_SPLIT_COMBINE = 46,
	    DEV_SYSTEM_INFO_TOUR_CFG = 47,

	    DEV_SYSTEM_INFO_VIDEOUT_CFG = 48,    //!�����Ϣ����
	    DEV_SYSTEM_CUR_TIME=49,//ʱ��

	    DEV_SYSTEM_INFO_DISKINFO =50,    //Ӳ����Ϣ

	    DEV_SYSTEM_CTRL_DISKINFO =51,    //����Ӳ�̴���

	    DEV_SYSTEM_CTRL_DISKPARTION=52,   //����Ӳ�̷�������
	    DEV_SYSTEM_DST_TIME = 53,         //����ʱʱ�����
	    DEV_SYSTEM_VIDEO_OUT_OSD_CFG =54, //ǰ�����OSD����
	    DEV_SYSTEM_RECCTL_CFG =55,    //¼���������
	    DEV_SYSTEM_CHANNAME_CFG =56,                       //ͨ����
	    DEV_SYSTEM_AUDIO_CFG =57,//��Ƶ����
	    DEV_SYSTEM_QUERYLOG =59,/*ϵͳ��־��ѯ*/
	    DEV_SYSTEM_OPRLOG=60,/*ǰ����־��������*/
	    DEV_SYSTEM_QUERY_REC_PIC=61,/* ¼��ͼƬ��ѯ����*/
	    // ������
	    DEV_ABILITY_DECODER = 100,//������
	    DEV_ABILITY_SAVIA,//����ģ��
	    DEV_ABILITY_AUDIO_IN,//��Ƶ����(�и��������ʾÿ��ͨ��������Ƶ����)
	    DEV_ABILITY_TALK,//�Խ����Խ���Ƶ����+��Ƶ�����
	    DEV_ABILITY_WIRELESS,//WFI
	    DEV_ABILITY_3G,//3G
	    DEV_ABILITY_INFRARED,//����
	    DEV_ABILITY_AUTO_REGISTER=107,//����ע��
	    DEV_ABILITY_PTZ_TOUR=108,//��̨��Ѳ��
	    DEV_ABILITY_SPLIT_TYPE = 109, //�໭��Ԥ��ģʽ�������棬4�����
	    //ע��໭��Ѳ����ֻҪ��DEV_ ABILITY_SPLIT_TYPE���ص�����������DEV_ SYSTEM_INFO_SPLIT_COMBINE����1���ʹ��ڶ໭��Ѳ������
	    DEV_ABILITY_SPLIT_COMBINE = 110,
	    //�໭����������б�ÿ��ģʽ�µ���Ϸ�ʽ

	    //ץ��������
	    DEV_ABILITY_SNAP=111,

	    //!��������������
	    DEV_ABILITY_AlarmLink=112,
	    //ƽ̨����������
	    DEV_ABILITY_IVIDEO = 113,
	    DEV_ABILITY_ENCODE = 114,//�豸��������
	    DEV_ABILITY_DEV_TIME = 115,//ʱ������������ʱ��ntp��
	    DEV_ABILITY_NET_FTP =116,//ftp������
	    //�ڰ�����������added by wyf on 20100415
	    DEV_ABILITY_DEV_BLACKWHITEIP = 117,
	    //end added by wyf on 20100415
	    DEV_ABILITY_DEV_RTSP = 118,//rtsp ������
	    DEV_ABILITY_DEV_AUDIO = 119,//���������ȵ���������
	    DEV_ABILITY_DEV_RECORD = 120,//¼����������ż�
	    DEV_ABILITY_KBD = 121,//���Ƽ���������
	    DEV_ABILITY_DEV_MATRIX = 123, //֧�־���������
		DEV_ABILITY_OSD_ZOOM = 124,//OSD����������
		DEV_ABILITY_OSD_WATERMARK = 125,//����ˮӡ����
	    DEV_ABILITY_CAMERA = 129, //����ͷ������
	    DEV_ABILITY_NEW_NET_OPR = 130,//�����µ�������Ʋ���������
	    DEV_ABILITY_ENCODE2 = 131,//�豸��������2
	    DEV_ABILITY_OEM_FUN = 132, //OEM�������⹦��	
	    DEV_ABILITY_OSD_MULINE = 135,//����OSD����
	    DEV_ABILITY_SUPPORT_ALARM = 136,//�豸֧�ֵĸ澯����
	  
	    DEV_SYSTEM_INFO_SUPERUSER_AUTH = 150, 	

	    // �������õ�������
	    DEV_CONFIG_TYPE_NET       = 200,            /*��������*/
	    DEV_CONFIG_TYPE_MAIL      = 201,            /*�ʼ�����*/
	    DEV_CONFIG_TYPE_PPPOE     = 202,            /*PPPoE����*/
	    DEV_CONFIG_TYPE_DDNS      = 203,            /*DDNS����*/
	    DEV_CONFIG_TYPE_SNIFFER   = 204,        /*����ץ������*/
	    DEV_CONFIG_TYPE_FTP       = 205,            /*FTP����*/
	    DEV_CONFIG_TYPE_NTP       = 206,            /*NTP����*/
	    DEV_CONFIG_TYPE_LIMIT_BR  = 207,        /*����������*/
	    DEV_CONFIG_TYPE_UPNP      = 208,
	    DEV_CONFIG_TYPE_BLACKWHITE = 209,

	    DEV_CONFIG_TYPE_DHCP = 210,      /*DHCP����*/
	    DEV_CONFIG_TYPE_WEB_URL ,        /*ץͼ����web·������*/
	    DEV_CONFIG_TYPE_FTP_PRO,         /*FTP����*/

	    DEV_CONFIG_TYPE_ETHERNET,        /*��������*/
	    DEV_CONFIG_TYPE_DNS,            /*DNS����������*/
	    DEV_CONFIG_TYPE_WIRELESS_ADDR,    //���߱�������,�����ַ��ң������ַ
	    DEV_CONFIG_TYPE_WLAN_DEVICE,        //���������豸
	    DEV_CONFIG_TYPE_MULTI_DDNS,        //��DDNS����������
	    DEV_CFG_TYPE_DECODE,
	    DEV_CONFIG_TYPE_INTERVIDEO,        //ƽ̨��������

	    DEV_CONFIG_TYPE_WLAN = 220,                //��ѯ��������
	    DEV_CONFIG_TYPE_AUTO_REGISTER,    // ����ע���������
	    DEV_CONFIG_TYPE_MAC= 222,   //MAC��ַ
	    DEV_CFG_TYPE_DECODE_EX=224,   //������������չ
	    DEV_CFG_TYPE_NET_FTP_SERVER = 225, //ftp����������
	    DEV_CFG_TYPE_NET_FTP_APP_TIME =226, // ftpӦ��ʱ�������
	    DEV_CFG_TYPE_NET_RTSP = 227, //rtsp����
	    DEV_CFG_TYPE_NET_LOCAL_CH_CFG =228, // ����ͨ����������
	    DEV_CFG_TYPE_NET_REMOTE_CH_CFG =229,// Զ��ͨ���б�����
	    DEV_CONFIG_TYPE_AUTOREG_EX=230, 
	    DEV_CONFIG_CAMERA=231, //2762ʹ�ã���Ƶ�ɼ�������Ϣ
	    DEV_CONFIG_CAMERA_EXT=232, //2762ʹ�ã���Ƶ�ɼ�������Ϣ
	    DEV_CONFIG_PTZ = 233,
	    DEV_CONFIG_PTZ_PROT = 234, //��ѯ֧�ֵ���̨Э��    
	    DEV_CONFIG_LT_MOBILE = 235, //֧�������ֻ�Э��
	    // �澯���õ�������
	    DEV_ALARM_ALARMIN_TYPE = 300, //���ظ澯����
	    DEV_ALARM_NETIN_TYP = 301,//����ģ��澯����
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
	    DEV_ALARM_IPCONFICT_TYPE = 312,//ip��ͻ
	    DEV_ALARM_INT_REDON_TYPE = 313,//���ܺ�ɫ�澯
	    DEV_ALARM_NET_ALARMIN    = 314,
	    
	    ALARM_ENABLE_TYPE        = 350,     //��������
	    ALARM_WORKSTREET_TYPE    = 351, //�澯������
	    ALARM_EVENT_TYPE         = 352,         //������
	    ALARM_MOTION_REGION_TYPE = 353, //����������������
	    ALARM_REDON_REGION_TYPE = 354, //���ܺ�ɫ��������
		
	    // ͨ�����õ�������
	    DEV_CHANNEL_ENCODE_TYPE = 400, //ͨ������
	    DEV_CONFIG_TYPE_RECORD,            /*��ʱ¼������*/
	    DEV_CONFIG_TYPE_SNAP = 402,//ץ�Ĳ�������    
	    DEV_CHANNEL_SVAC_TYPE = 403,//SVAC��ɫ����

	    ENCODE_CHANNEL_NAME_TYPE = 410, //ͨ������
	    ENCODE_MAIN_STREAM_TYPE, //����������
	    ENCODE_SUB_STREAM_TYPE,  //����������
	    ENCODE_SNAP_STREAM_TYPE, //׽����������
	    ENCODE_COVER_TYPE,     //������Ԥ���ڵ�
	    ENCODE_COLOR_TYPE,    //��ɫ
	    ENCODE_TIMEOSD_TYPE,   //OSD����
	    ENCODE_TITLEOSD_TYPE = 418,   //OSD����
	    ENCODE_OSD_SIZE = 419,//by ilena OSD��С����
		ENCODE_SVAC_FOCUS_AREA_TYPE = 420,//SVAC�۽�����
		ENCODE_OSD_WATERMARK = 421,

	    DEV_MANUAL_SNAP = 422,//�ֶ�ץ�Ĺ���
	    ENCODE_COLOR_TYPE_EX = 423,    //��ɫ��չ
	    ENCODE_COLOR_TYPE_EX_TABLE = 424,    //��ɫ��չ 
	    ENCODE_CHANNEL_MUTILE_TITLE = 425, //����OSD����
	    
	    // ��̨���õ�������
	    DEV_PTZ_PRESET_TYPE = 500, //��̨Ԥ�õ�
	    DEV_PTZ_TOUR_TYPE =501,//��̨Ѳ���켣
	    DEV_CONFIG_TYPE_SERIAL_COMM,    /*��������*/

	    PTZ_QUERY_PRESET_TYPE =510, //��̨Ԥ�õ�
		PTZ_QUERY_TOUR_TYPE,//��̨Ѳ���켣
	    PTZ_ADD_PRESET_TYPE, //���Ԥ�õ�
	    PTZ_CLEAR_PRESET_TYPE, //���Ԥ�õ�
	    PTZ_ADD_TOUR_PRESET_TYPE, //���Ԥ�õ㵽Ѳ���켣

	    PTZ_CLEAR_TOUR_PRESET_TYPE, //���Ԥ�õ㵽Ѳ���켣
	    PTZ_CLEAR_TOUR_TYPE = 516, //���ĳ��Ѳ���켣


	    VIDEOUT_CFG_TV = 517, //!tv����
	    PTZ_RAW_DATA  = 518,//͸��ͨ������
	    DEV_CONFIG_TYPE_KBD_TYPE  = 519,//���Ƽ�������

	    
	    //ƽ̨����������
	    INTERVIDEO_TYPE_ZXBELL =550 ,  // ����BELL  
	    INTERVIDEO_TYPE_MEGA =551 ,  //����
	    INTERVIDEO_TYPE_VS300 = 552,//VS300
	    INTERVIDEO_TYPE_SHENYAN = 553,//����
	    INTERVIDEO_TYPE_KOTI = 554,//KOTI
	    INTERVIDEO_TYPE_BJUN =555 ,  // ������ͨ
		INTERVIDEO_TYPE_GAYS =556 ,  //����һ��
		INTERVIDEO_TYPE_HTTP =557 ,  //http
		INTERVIDEO_TYPE_VISS30 =558 , 

		INTERVIDEO_TYPE_BLUESTAR =559 ,  //bluestar
		INTERVIDEO_TYPE_HAOWEI = 560,   //���� 
		INTERVIDEO_TYPE_SHSS = 562,   //�������
		//��Ƶ��������������
		CAMERA_WHITEBALANCE_TYPE = 600,
		
		//���������������
		MJ_CAMERA_TYPE = 601,
		MJ_CAMERA_ADVANCE_TYPE = 602,  //�߼�
		MJ_CAMERA_AUTOHOME_TYPE = 603,
		MJ_CAMERA_BLIND_TYPE = 604,
		MJ_CAMERA_POSTION_TYPE = 605,
		MJ_CAMERA_PTZ_TYPE = 606,
		CAMERA_RESOLUTION_TYPE = 607,
		CAMERA_DRC_TYPE = 608,	
		CAMERA_SUPPER_CONFIG_TYPE =609,
		CAMERA_BADPIXEL_DETECT = 610 ,
		
		KOTI_FLOOR_TYPE =650,/* ¥����� */
		KOTI_ROOM_TYPE =651,/* ������� */
		KOTI_IP_CAM_NAME_TYPE =652,/* ����ͷ���� */
		KOTI_APARTMENT_CFG_TYPE =653,/* ����λ�ò��� */
		KOTI_ALARM_AREA_TYPE =654,/* �������ò��� */
		KOTI_MOBILE_NUM_TYPE =655,/* �����绰���� */
		KOTI_SYSINFO_TYPE =656,/* ϵͳ���� */
		KOTI_ALARM_TIME_TYPE =657,/* �������� */
		KOTI_PASSWORD_TYPE = 658,/* ����������� */
		KOTI_RTSP_CONG_TYPE = 659,/* ƽ̨�ԽӲ��� */
	  
		DEV_CONIFG_CARD_RECORD =660,	/*����¼��*/

		ENCODE_3IRD_STREAM_TYPE = 680,  //����������-��ӦCHL_3IRD_T
		ENCODE_4RTH_STREAM_TYPE = 618, //����������-��ӦCHL_4RTH_T
		ENCODE_5FTH_STREAM_TYPE = 682,
		
	    MINGJING_CONFIG_TYPE = 690, //������������
	    DEV_CONFIG_TYPE_WRIELESS = 691,  //��������
	    SUVA_CONFIG_TYPE = 700,         //���ܱ�������
		SUVA_AREA_CONFIG_TYPE = 701,    //��������������Ϣ
	    DEV_ABILITY_SUVA = 702 ,         //����������   
	    DEV_ABILITY_SUPPORT_COLOR_TYPE = 703,          //ͼ����ɫ���������
	    DEV_ABILITY_SUPPORT_SENSOR_TYPE = 704,   //sensor ֧�ֵ� �ֱ��� ����ʽ
	    DEV_ABILITY_SUPPORT_EXSTREAM_TYPE = 705, //������¼��������
	    DEV_EXSTREAM_CFG = 706,                  //������¼������
	    DEV_ENCODER_DETECT_CFG = 707,       

#if defined(GUIZHOU_G_PROJECT) || defined(POLICE_PROJECT)
		DEV_MUSIC_FILE_SEARCH_TYPE = 710,			//��ȡ�ļ��б�
		DEV_MUSIC_PLAY_PROJECT_TYPE = 711,			//��ȡ���żƻ�
		DEV_MUSIC_PLAY_STATUS_TYPE  = 713,
#endif
#ifdef POLICE_PROJECT 
		DEV_LED_STATUS_TYPE  = 714,
#endif
#if defined(GUIZHOU_G_PROJECT) || defined(POLICE_PROJECT)
		DEV_WLAN_INFO_TYPE = 715,	
#endif
		DEV_GOOLINK_TYPE = 716,
	//  750  ��ΪNVR  Զ��ͨ������ʹ�� 
	    NVR_IPC_LOGIN_TYPE = 750,  //��ѯԶ��ͨ�� �Ƿ��¼    
	    
	}ENM_MSG_TYPE;
}
#endif // BASETYPEDEF_H


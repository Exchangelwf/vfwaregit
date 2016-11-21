
#ifndef __VIDEO_API_H__
#define __VIDEO_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup VideoAPI API Video
/// ��Ƶ����API����Ҫ����Ƶ���������������������ã�����һЩ���������ù��ܡ�
///	\n ��������ͼ:
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

/// ��Ƶ��ʽ����
enum video_standard_t {
	VIDEO_STANDARD_PAL = 0,	///< PAL��ʽ��
	VIDEO_STANDARD_NTSC,	///< NTSC��ʽ��
	VIDEO_STANDARD_SECAM	///< SECAM��ʽ��
};
enum {
    ENUM_COLOR_RUILI,
    ENUM_COLOR_YANLI,    
    ENUM_COLOR_BIAOZHUN,
    ENUM_COLOR_ROUHE,
    ENUM_COLOR_ZHENGCHANG,
    ENUM_COLOR_ZIDINGYI
};
/// ��Ƶ��ɫ��ʽ
/// ��Ƶ��ɫ��ʽ
typedef struct VIDEO_COLOR{
	uchar	Brightness;		///< ���ȣ�ȡֵ0-100��
	uchar	Contrast;		///< �Աȶȣ�ȡֵ0-100��
	uchar 	Saturation;		///< ���Ͷȣ�ȡֵ0-100��
	uchar 	Hue;			///< ɫ����ȡֵ0-100��
	uchar 	Gain;			///< ���棬ȡֵ0-100��bit7��λ��ʾ�Զ����棬����λ�����ԡ�
	uchar	WhiteBalance;	///< �Զ��׵�ƽ���ƣ�bit7��λ��ʾ�����Զ�����.0x0,0x1,0x2�ֱ�����,��,�ߵȼ�
	uchar	reserved[2];
}VIDEO_COLOR;

/// ��Ƶ����ṹ
/// - ����ʾ������Ƶ���������ʱ, ʹ��ͼ��������ϵ, Ҳ������ָ��ͼ��������ϵ����Ƶ�߽�
/// ������, �ṹ��Ա��ȡֵ��ͼ�ηֱ���, ͼ�κ���Ƶ����ʱ��ƫ�ƺ����Ų���, ��ǰ�ָ�ģ
/// ʽ�ȶ��й�ϵ;
/// - ����ʾ����������Ƶ�����е�һ��������, ��ʹ���������, �����һ������ķֱ���
/// (0, 0, RELATIVE_MAX_X, RELATIVE_MAX_Y), ʹ�������������ڲ�֪��ʵ����Ƶ�ֱ��ʵ�
/// ������趨��Ƶ��������, ���Ա�֤һ��ת�������1��������. ת����ʽ����(������Է�
/// ����Ϊaw*ah):
/// ��������(ax, ay)ת�����������(rx, ry):
/// \code
/// rx = ax * RELATIVE_MAX_X / aw;
/// ry = ay * RELATIVE_MAX_Y / ah;
/// \endcode
/// �������(rx, ry)ת���ɾ�������(ax, ay):
/// \code
/// ax = rx * aw / RELATIVE_MAX_X;
/// ay = ry * ah / RELATIVE_MAX_Y;
/// \endcode
/// ��Էֱ��ʶ�������:
/// \code
/// #define RELATIVE_MAX_X 8192
/// #define RELATIVE_MAX_Y 8192
/// \endcode
///
typedef struct VIDEO_RECT
{
	ushort	left;			///< ���Ͻ�x���ꡣ
	ushort	top;			///< ���Ͻ�y���ꡣ
	ushort	right;			///< ���½�x���ꡣ
	ushort	bottom;			///< ���½�y���ꡣ
}VIDEO_RECT;

/// ��Ƶ���ǲ�������
enum video_cover_t {
	VIDEO_COVER_WINDOW_CLEAR = 0,	///< ������ڸ�������
	VIDEO_COVER_WINDOW_DRAW,		///< ���ƴ��ڸ�������
	VIDEO_COVER_FULLSCREEN_DRAW,	///< ����ȫ����������
	VIDEO_COVER_FULLSCREEN_CLEAR	///< ���ȫ����������
};

/// ��Ƶ���ǲ���
typedef struct VIDEO_COVER_PARAM
{
	/// ���ǵľ������򣬲������������ϵ��
	VIDEO_RECT	rect;

	/// ������������ʾ����ɫ��16���Ʊ�ʾΪ0x00BBGGRR������ֽڱ�����
	uint		color;

	/// ������������ͣ� ȡvideo_cover_t���͵�ֵ�����ں�ȫ������Ӧ���Ƕ����ģ� 
	/// �������ڵĲ�����Ӱ��ȫ����ȫ���ڵĲ���Ҳ��Ӱ�촰�ڡ� �������ʱ������
	/// ���������ԡ�
	int			enable;
}VIDEO_COVER_PARAM;

/// ��Ƶ�л�����
enum video_switch_t{
	VIDEO_SWITCH_MONITOR = 0,	///< �л������������
	VIDEO_SWITCH_PLAY			///< �л����ط������
};

/// ��Ƶ�������������
enum video_output_t{
	VIDEO_OUTPUT_AUTO = 0,	///< �Զ�����豸��
	VIDEO_OUTPUT_VGA,		///< �л���VGA�����
	VIDEO_OUTPUT_TV,		///< �л���TV�����
	VIDEO_OUTPUT_VGA_LCD	///< �л���LCD VGA�����
};

/// �����豸���Խṹ
typedef struct VIDEO_CAPS
{
	/// �Ƿ�֧��TV�����ɫ���á���1��ʾ֧�ָ�ö��ֵ��Ӧ�����ԣ���0��ʾ��֧�֡�
	uchar TVColor;
	/// ͼ����������
	/// \note ����λ����ο�pic_attr
} VIDEO_CAPS;				

#if (16==CHANNEL)
/* maozheng-add-20090709 S */
/* 16·ʱ����PCIV��Ϣ���ݵĽṹ�� */
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
	 WHITE_BALANCE_OFF, //�ر�white balance
	 WHITE_BALANCE_AUTO,  //�Զ�white balance
	 WHITE_BALANCE_DAYTIME, //�������
	 WHITE_BALANCE_EVENING, //����ƻ�
	 WHITE_BALANCE_CLOUDY, //�������
	 WHITE_BALANCE_OFFICE, //���ڰ칫��
	 WHITE_BALANCE_FLUORESCENT,//ӫ���
	 WHITE_BALANCE_INCANDESCENT,// �׳��
	 WHITE_BALANCE_HAND,  //�ֶ���ƽ��
	 WHITE_BALANCE_NR,
};

typedef struct _VIDEOIN_CAPS{
 uint uiCamAbility;  //��λ������32��Ŀ�� 
                  //��0λ ֧��ˮƽ��תHorReverse��1Ϊ֧�ָ���ܣ�0Ϊ��֧�ָ����
                  //��1λ ֧����ֱ��תVerReverse��1Ϊ֧�ָ���ܣ�0Ϊ��֧�ָ����
                  //��2λ ֧���ع�ģʽ����Exposure��1Ϊ֧�ָ���ܣ�0Ϊ��֧�ָ����
                  //��3λ ֧�ָй������AESensitivity��1Ϊ֧�ָ���ܣ�0Ϊ��֧�ָ����
                  //��4λ ֧�ֱ��ⲹ��Backlight��1Ϊ֧�ָ���ܣ�0Ϊ��֧�ָ����
                  //��5λ ֧�ְ�ƽ��ģʽWhiteBalance��1Ϊ֧�ָù��ܣ�0Ϊ��֧�ָ���ܡ�
                  //��6λ ֧����Ӱ����ShadeCorr��1Ϊ֧�ָù��ܣ�0Ϊ��֧�ָ���ܡ�
                  //��7λ ֧�ֻ�������BadCorr��1Ϊ֧�ָù��ܣ�0Ϊ��֧�ָ���ܡ�
                  //��8λ ֧�ַ���˸Anti-Flicker��1Ϊ֧�ָù��ܣ�0Ϊ��֧�ָ����
                  //��9λ ֧��ɫ�����ڣ�1Ϊ֧�ָù��ܣ�0Ϊ��֧�ָù���
                  //��10λ֧��YUV�����Χ���ƣ�1Ϊ֧�ָù��ܣ�0Ϊ��֧�ָù���
                  //��11λ֧��IR-CUT���ƣ�1Ϊ֧�ָù��ܣ�0Ϊ��֧�ָù���
                  //��12λ֧��ȥ����ƣ�1Ϊ֧�ָù��ܣ�0Ϊ��֧�ָù���
                  //��13λ֧�ֺڰ�/��ɫ/�Զ�ת����1Ϊ֧�ָù��ܣ�0Ϊ��֧�ָù���
                   //��14λ֧�ֹ���������
                   //��15λ֧��isp���ֿ�̬ 
                   //��16λ��֡�ɼ�
                   //��17λWDR����(SVAC��ʹ��)
                   //��18λ���ⲹ��
                   //��19λ���ر���ػ��������
                   //��20λ֡�ۻ�, �ο�EnFRAME_ACCUMULATE_MODE
                   //��21λ����̬, �ο�EnULTRA_DYNAMIC_RANGE_MODE
                   //��22λͼ�񶳽�, �ο�EnFROZEN_IMAGE_MODE
                   //��23λ��̶Խ�����, �ο�EnMIN_FOCUS_DISTANCE
                   //��24λGAMMAУ��ģʽ: �ο�EnGAMMA_ADJUST_MODE
                   //��25λǿ������                   
                   //��26λ֧��sensor��̬
 uint uiRes;//����
 uint uiSenSpan; //�й�ȷ�Χ���ֵ ��ֵ255�����ʶ��ΧΪ0~255
 uint uiBLightSpan;//���ⲹ��֧������Backlight,��ֵ15����Χ0~15
 uint uiShutterL;//�ֶ��ع� ��ֵΪ6000��֧����С�ع�ʱ��Ϊ1/6000��
 uint uiShutterH; //�ֶ��ع⣬��ֵΪ50��֧����С�ع�ʱ��Ϊ1/50��
 uint uiAntiFlicker;//����˸֧��ģʽ����λ����
   //��0λ �Ƿ�֧��50Hz indoorģʽ
   //��1λ �Ƿ�֧��50Hz outdoorģʽ
   //��2λ �Ƿ�֧��50Hz autoģʽ
   //��3λ �Ƿ�֧��60Hz indoorģʽ
   //��4λ �Ƿ�֧��60Hz outdoorģʽ
   //��5λ �Ƿ�֧��60Hz autoģʽ
   //��6λ �Ƿ�֧��50Hz Theaterģʽ
   //��7λ �Ƿ�֧��50Hz Fastģʽ
   //��8λ �Ƿ�֧��60Hz Theaterģʽ
   //��9λ �Ƿ�֧��60Hz Fastģʽ
   //��10λ �Ƿ�֧�ַ����ر�ģʽ
 uint uiWhiteBlance; //���֧�ְ�ƽ��ģʽ��������Ч����λ��ʾ��ƽ��֧�ֵ�ģʽ
   //ÿһλ��ӦWHITE_BALANCE_MODEö���е���Ӧλ������0xc0��ʾ֧��ɫ��4650k��ɫ��6500k����ģʽ
 uint uiRes2[2];//����
}VIDEOIN_CAPS;

//CONFIG_CAMERA��CAMERA_FORMAT�ṹ��һ�����޸�ʱ����ע��
typedef struct _CAMERA_FORMAT_
{
	 uchar HorReverse; //֧��ˮƽ��תʱȡֵ��ΧΪö��IMAGE_MODE_MIRROE�е�ֵ
	 uchar VerReverse; //֧����ֱ��תʱȡֵ��ΧΪö��IMAGE_MODE_MIRROE�е�ֵ
	 uchar ExposureType;
	 //֧���ع�ģʽʱȡֵ��ΧΪö��EXPOSURE_MODE�е�ֵ1:�Զ��ع� 2:�ֶ�
	 uchar AESensitivity; 
	 //֧�ָй������ʱȡֵ��ΧΪ0��ABILITY_CAMERA_T.uiSenSpan
	 uchar Backlight; //֧�ֱ��ⲹ��ʱȡֵ��ΧΪ0��ABILITY_CAMERA_T.uiBLightSpan
	 uchar WhiteBalanceMode; //֧�ְ�ƽ��ʱȡֵ��ΧΪ������ABILITY_CAMERA_T.uiWhiteBlance�а�λ��ʾ��ֵ
	 uchar ShadeCorr; //�ݲ�֧�֣�ȡֵ��Χδ֪
	 uchar BadCorr;  //�ݲ�֧�֣�ȡֵ��Χδ֪
	 uint shutter;  //�ع�ģʽ��Ϊ�ֶ�ʱȡֵ��ΧΪuiShutterL��uiShutterH֮��
	 uchar AntiFlicker; //֧�ַ���˸ʱ��ȡֵ��ΧΪ������ABILITY_CAMERA_T.uiAntiFlicker�а�λ��ʾ��ֵ
	 uchar ColorTemp; //֧��ɫ������ʱ��ȡֵ��ΧΪö��COLOR_TEMP_MODE �е�ֵ
	 uchar YUVCut; //֧��yuv�����Χ����ʱ��ȡֵ��ΧΪö��YUV_RANGE_MODE �е�ֵ
	 uchar IRCut; //֧��IR-CUTʱ��ȡֵ��ΧΪö��IR_CUT_MODE�е�ֵ
	 uchar PAA; //֧��ȥ�빦��ʱ��ȡֵ��ΧΪö��PAA_MODE�е�ֵ
	 uchar Color2WhiteBlack; //֧�ֺڰ�/��ɫ/�Զ�ת��ʱ��ȡֵ��ΧΪö��COLOR_2_WHITEBLACK_MODE�е�ֵ
	 uchar SensorType;   //�������� �ߵ�ƽ��Ч�͵�ƽ��Ч 0 �ߵ�ƽ 1�͵�ƽ  
	 uchar res[5];
}CAMERA_FORMAT;

typedef struct _CAMERA_WHITEBALANCE_  //�ֶ����ð�ƽ���ǵ�RGB
{
	 uchar WhiteBalanceMode; //֧�ְ�ƽ��ʱȡֵ��ΧΪ������ABILITY_CAMERA_T.uiWhiteBlance�а�λ��ʾ��ֵ
	 uchar wbr; //�Զ���ƽ��R����
	 uchar wbg; //�Զ���ƽ��R����
	 uchar wbb; //�Զ���ƽ��R����
	 uchar res[4];
}CAMERA_WHITEBALANCE;

typedef struct _CAMERA_FORMAT_EXT_  // ��������cam�� ���Ź���
{
	 uint ispWDRenable; //DRC
	 uint ispWDRStrength; //0 - 255  DRC str
	 uchar SlowFrameRate;  // 0 -15 ��֡�ɼ�	 ���ն��������ͼ������
	 uchar snrWDRenable;//sensor ��̬
	 uchar res[14];
}CAMERA_FORMAT_EXT;

/// �õ���Ƶ�����ͨ����Ŀ��
/// 
/// \param ��
/// \return��Ƶ���ͨ����Ŀ��
int Video2GetOutChannels(void);


/// ������Ƶ�豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int VideoCreate(void);
int Video2Create(int index);


/// ������Ƶ�豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int VideoDestory(void);
int Video2Destroy(int index);


/// ������Ƶ����ɫ������
/// 
/// \param [in] channel ͨ���š�
/// \param [in] pColor ָ����ɫ�ṹVIDEO _ COLOR��ָ�롣
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int VideoSetColor(int channel, VIDEO_COLOR * pColor);

/// ͼ��֧�ֵ������б�����
/// \note ��Ϊͼ�����б�����mask 1:������ 0:��������
enum pic_attr
{
 PIC_COLOR, ///< ����,�Աȶ�,ɫ��,����,���Ͷ�
 PIC_BACKLIGHT,///< ���ⲹ��
 PIC_EXPOSAL, 	///< �ع�ѡ��
 PIC_AUTOC2BW, ///< �Զ��ʺ�ת��
 PIC_MIRROR,		//����
 PIC_FLIP,		//��ת
 PIC_MASKNUM,  ///< ����ѡ����
};
 
/// ��Ƶ���ƽṹ
typedef struct VIDEO_CONTROL{
 uchar  Exposure; ///< �ع�ģʽ 1-6:�ֶ��ع�ȼ�; 0:�Զ��ع�
 uchar  Backlight; ///< ���ⲹ�� 1:�򿪲���; 0:ֹͣ����
 uchar  AutoColor2BW; ///< �Զ��ʺ�ת�� 1:��ת��; 0:ֹͣת��
  uchar  Mirror;		//< ����  1 ���� 0��

}VIDEO_CONTROL;

typedef struct tagWHITE_LEVEL_CAPS //Added by wangjun2-20080805
{
	int	 enable;		// Ϊ1��ʾʹ�ܣ�Ϊ0ʱ��ʾ����
}WHITE_LEVEL_CAPS;


/// �õ���Ƶ�����ͨ����Ŀ��
/// 
/// \param ��
/// \return��Ƶ���ͨ����Ŀ��
int Video2GetOutChannels(void);


/// ������Ƶ�豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int VideoCreate(void);
int Video2Create(int index);


/// ������Ƶ�豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int VideoDestory(void);
int Video2Destroy(int index);


/// ������Ƶ����ɫ������
/// 
/// \param [in] channel ͨ���š�
/// \param [in] pColor ָ����ɫ�ṹVIDEO _ COLOR��ָ�롣
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int VideoSetColor(int channel, VIDEO_COLOR * pColor);
 
/// ������Ƶ�Ŀ��Ʋ�����
/// 
/// \param [in] channel ͨ���š�
/// \param [in] pColor ָ����Ƶ���ƽṹVIDEO_CONTROL��ָ�롣
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int VideoSetControl(int channel, VIDEO_CONTROL * pColor);


/// ���ø�������
/// 
/// \param [in] channel ͨ���š�
/// \param [in] index �ڵ������
/// \param [in] pParam ָ����Ƶ���ǽṹVIDEO_COVER_PARAM��ָ�롣
/// \retval 0  ���óɹ���
/// \retval <0  ����ʧ�ܡ�
int VideoSetCover(int channel, int index, VIDEO_COVER_PARAM *pParam);


/// ������Ƶ��ʽ��
/// 
/// \param [in] channel ͨ���š�
/// \param [in] dwStandard ��Ƶ��ʽ��ȡvideo_standard_t���͵�ֵ��
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int VideoSetVstd(int channel,uint dwStandard);	


/// дһ���ֽڵ���Ƶ�豸�Ĵ�����
/// 
/// \param [in] Chip оƬ��š�
/// \param [in] Page ҳ����š�
/// \param [in] Register �Ĵ�����š�
/// \param [in] Data Ҫд���ֵ��
/// \return ��
void VideoWriteRegister(uchar Chip, uchar Page, uchar Register, uchar Data);


/// ����Ƶ�豸�Ĵ�����һ���ֽڡ�
/// 
/// \param [in] Chip оƬ��š�
/// \param [in] Page ҳ����š�
/// \param [in] Register �Ĵ�����š�
/// \return������ֵ��
uchar VideoReadRegister(uchar Chip, uchar Page, uchar Register);


/// �л���Ƶ��������͡�
/// \param [in] dwType ��������ͣ�ȡ�±��е�ֵ��
/// \retval 0  �л��ɹ���
/// \retval <0  �л�ʧ�ܡ�
int VideoSwitch(uint dwType);
int Video2Switch(int index, uint dwType);


/// ����TV����ı߽硣��Ƶ��ʾ��ĳЩ��������ʱ�� 4�����ϻ�����ʧ�����������Ŀ��
/// ��������4���߽�ѹ���ı�����ʹ��Ƶ���ڼ������Ŀ��������ڡ����ڷ�Χ0-100��һ��
/// ʾ��ֵ��ʵ���϶�Ӧ��С�ĳߴ���Ҫ�ڵײ���ת�������ơ�
/// 
/// \param [in] left ��߽���ʣ�ȡֵ0-100�� Ĭ��ֵΪ0��
/// \param [in] top �ϱ߽���ʣ�ȡֵ0-100�� Ĭ��ֵΪ0��
/// \param [in] right �ұ߽���ʣ�ȡֵ0-100�� Ĭ��ֵΪ0��
/// \param [in] bottom �±߽���ʣ�ȡֵ0-100�� Ĭ��ֵΪ0��
/// \retval 0  ���óɹ���
/// \retval <0  ����ʧ�ܡ�
int VideoSetTVMargin(uchar left, uchar top, uchar right, uchar bottom);


/// ����TV�������ɫ��
/// 
/// \param [in] brightness ���ȣ�ȡֵ0-100�� Ĭ��ֵΪ50��
/// \param [in] contrast �Աȶȣ�ȡֵ0-100�� Ĭ��ֵΪ50��
/// \retval 0  ���óɹ���
/// \retval <0  ����ʧ�ܡ�
int VideoSetTVColor(uchar brightness, uchar contrast);


/// ����TV���������������������Ӳ�����ж��������⡣
/// 
/// \param [in] level �����������������̶���ȡֵ���Ǳ�����ϵ������һ������ֵ��
///        ȡֵ0-100�� Ĭ��ֵΪ50��
/// \retval 0  ���óɹ���
/// \retval <0  ����ʧ�ܡ�
int VideoSetTVAntiDither(uchar level);


/// ������Ƶ��������������͡�
/// 
/// \param [in] type ���������ͣ�ȡvideo_output_t���͵�ֵ��
/// \retval 0  ���óɹ���
/// \retval <0  ����ʧ�ܡ�
int VideoSetOutputAdapter(uchar type);


/// ��ȡ��Ƶ��������������͡�
/// 
/// \param [out] ptype ���������ͣ�ȡvideo_output_t���͵�ֵ��
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int VideoGetOutputAdapter(uchar *ptype);


/// ������Ƶ��������ͨ��������ͨ����
/// 
/// \param [in]  channelIn ��Ƶ����ͨ��
/// \param [in]  channelOut ��Ƶ���ͨ��
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int VideoMatrix(uchar channelIn,uchar channelOut);


/// ����TV������
/// 
/// \param [in] open TRUE-��TV��������FALSE-��TV������
/// \retval 0  �����ɹ���
/// \retval <0  ����ʧ�ܡ�
int VideoSwitchTVMonitor(VD_BOOL open);


/// ����VGA������
/// 
/// \param [in] open TRUE-��VGA��������FALSE-��VGA������
/// \retval 0  �����ɹ���
/// \retval <0  ����ʧ�ܡ�
int VideoSwitchVGAMonitor(VD_BOOL open);
/// ���òɼ�оƬ�Ĳɼ�����
/// 
/// \param [in] ��Ӧͨ����ˮƽ��ֱƫ�ƣ��Լ��������
/// \retval 0  �����ɹ���
/// \retval <0  ����ʧ�ܡ�
int VideoSetCropRegion(int channel,int Hoffset,int Voffset,int Width,int Height);
/// �õ���Ƶ֧�ֵ����ԡ�
/// 
/// \param [out] pCaps ָ����Ƶ���ԽṹVIDEO_CAPS��ָ�롣
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int VideoGetCaps(VIDEO_CAPS * pCaps);
int Video2GetCaps(int index, VIDEO_CAPS * pCaps);


/// ����û����Ƶ�ļ��ӻ���ı�����ɫ.
/// 
/// \param [in] color Ҫ���õ���ɫ��16���Ʊ�ʾΪ0x00BBGGRR������ֽڱ�����
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int VideoSetBkColor(uint color);
int Video2SetBkColor(int index, uint color);


/// ������Ƶ����ֱ���
int VideoRecordSetSize(int channel, int size);
	/*
˵��	��ʼ�Զ����

����	��

����ֵ	=0  ��	  �ɹ���
	����ֵ��  ʧ�ܡ�

*/
int VideoAutoDetectStart(void);
/*
˵��	ֹͣ�Զ����


����	��

����ֵ	=0  ��	  �ɹ���
	����ֵ��  ʧ�ܡ�
	*/
int VideoAutoDetectStop(void);

/// @} end of group

/*
	˵��:��ȡ�׵�ƽ��������ֵ
	����ֵ=0:�ɹ�
	����ֵ:ʧ��
*/
int WhiteLevelGetCaps(WHITE_LEVEL_CAPS * pCaps);

/*
	set or get video mode
	SystemSetVideoMode() return 0 success
*/
enum video_standard_t SystemGetVideoMode(void);
int SystemSetVideoMode(enum video_standard_t videostand);

/*
	˵��: 		����
	����in: 	iChannel ͨ��
				pFormat���ýṹ��
	����out: 	��
	����ֵ	=0  ��	  �ɹ���
		<0��  ʧ�ܡ�

*/
int VideoInSetCamera(uint iChannel, CAMERA_FORMAT *pFormat);

int VideoInSetWhiteBalance(uint iChannel, CAMERA_WHITEBALANCE *pWhiteBalance);

int VideoInSetCameraExt(uint iChannel,CAMERA_FORMAT_EXT *pFormatExt);
/*
	˵��: 		��ȡ��ͷ�����ṹ��
	����in: 	ich ͨ��
	����out: 	pCaps����ͷ�����ṹ��
	����ֵ	=0  ��	  �ɹ���
		<0��  ʧ�ܡ�

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
       // 1080P ��о   1080P ����, 720P ��о     720P  ����
	VD_SENSOR_NORM =0,
	VD_SENSOR_1080P_720P,   // 1080P ��о 720P ����
}VD_SENSOR_MOED;

//!��λ��ʾ��ӦVD_SENSOR_TYPE_E ��ֵ�����֧��256��
typedef struct tagVD_SENSOR_CAP_T
{
	uint sensor_cap;
	uint res[7];
}VD_SENSOR_CAP_T;

//!����Sensor type, �Թ�ipcʹ��
int VideoInSetSensorType(int ich, VD_SENSOR_TYPE_E type);
VD_SENSOR_TYPE_E VideoInGetSensorType(int ich);
//!����Sensor ʹ������ 1080P  ����720P ����, �Թ�ipcʹ��
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


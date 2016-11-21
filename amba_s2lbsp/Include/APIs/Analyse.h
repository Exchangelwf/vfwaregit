
#ifndef __VN_ANALYSE_API_H__
#define __VN_ANALYSE_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************/
//���ܱ�������---���֧��32�������㷨��������
/******************************************************************/
typedef enum _IntelligentType
{
	INTELLIGENT_RED,//��ɫ����							
	INTELLIGENT_LINE ,	// ���߾���
	INTELLIGENT_DIRECT ,	// ���򾯱�
	INTELLIGENT_ZOOM_RECT ,// �����ܽ����־���
	INTELLIGENT_ZOOM_CIRCLE 	,// Բ���ܽ����־���
	INTELLIGENT_ZOOM_POLYGON ,	// ������ܽ����־���
	INTELLIGENT_ZOOM_POLY_PT ,// ������ܽ����־���(�������ĵ�)
	INTELLIGENT_VECTOR_CROSS_DIRECT ,// ʸ���ཻ����
	INTELLIGENT_LINGER ,// �˶��ǻ�����		
	INTELLIGENT_NR=32
}INTELLIGENTTYPE;

/******************************************************************/
/// оƬ���ܷ���������
/******************************************************************/
//����Ӧ�÷�Ӧ�����㷨����������֧�ֺ������ܷ����㷨�� ���Է�����·��Ƶ
typedef struct ANALYSE_DETECT_CAPS
{
	//֧�ֵ������㷨���룬��λȡINTELLIGENTTYPEֵ
	uint	intelligent;
	//ÿ��оƬ�ɷ�������Ƶ·��
	uchar chnnum[INTELLIGENT_NR];
	/// ����
	uint	reserve;
}ANALYSE_DETECT_CAPS;

/******************************************************************/
/// ���ܷ�������
/******************************************************************/
//���������㷨�������������ʹ�����Ͻṹ�壬����Ҫ��һֱ����
//------------------��ɫ����---------------------------------//
//��ɫ����
#define MAX_RED_RIGION_NUM  8		///< ����ͨ��������ܼ��������
#define  MAX_RED_ALARM_LEVEL 8	///< �����ȼ���
#define MAX_POINT_NUM 	4			///< ȷ��һ�������������
typedef struct _ANALYSE_REGION
{
	VD_POINT point[MAX_POINT_NUM];
}ANALYSE_REGION;
typedef struct _RED_PARA
{
	//	��ɫ����ʹ��
	int 		enable;
	//   �ɼ�ͨ������
	int 		channel;
	///��ɫ��������
	int		iLevel;	
	/// �����ȵľ���ֵ
	int		sensitiveness;
	///ʵ���������
	int		 num;
	/// ���ܼ�������
	ANALYSE_REGION region[MAX_RED_RIGION_NUM];
	/// ����ֵ
	int		reserve[4];
}RED_PARA;
//------------------���߱���---------------------------------//
//���߱���
typedef struct _LINE_PARA
{
	//	���߱���ʹ��
	int 		enable;
	//   �ɼ�ͨ������
	int 		channel;
	//����ȷ��һ��ֱ��
	VD_POINT point[2];
	//  ����ֵ
	int		reserve[4];
}LINE_PARA;
//------------------���򱨾�---------------------------------//
//���򱨾�
typedef struct _DIRECT_PARA
{
}DIRECT_PARA;
//------------------�����������ֱ���---------------------//
//������������
typedef struct _RECT_PARA
{
}RECT_PARA;
//------------------ʸ���ཻ����----------------------------//
//ʸ���ཻ
typedef struct _CROSS_PARA
{
}CROSS_PARA;
//------------------�˶��ǻ�����----------------------------//
//�˶��ǻ�
typedef struct _LINGER_PARA
{
}LINGER_PARA;
//-------------------����----------------------------------------//
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
/// ���ܷ������
/******************************************************************/
typedef struct _RED_RESULT
{
//��λ��ʾ�ɼ�ͨ���ĺ�ɫ����״̬
	uint red_state;
	uint warning_level;
}RED_RESULT;
//------------------���߱���---------------------------------//
//���߱���
typedef struct _LINE_RESULT
{
	uint line_state;
}LINE_RESULT;
//------------------���򱨾�---------------------------------//
//���򱨾�
typedef struct _DIRECT_RESULT
{
	uint direct_state;
}DIRECT_RESULT;
//------------------�����������ֱ���---------------------//
//������������
typedef struct _RECT_RESULT
{
	uint rect_state;
}RECT_RESULT;
//------------------ʸ���ཻ����----------------------------//
//ʸ���ཻ
typedef struct _CROSS_RESULT
{
	uint cross_state;
}CROSS_RESULT;
//------------------�˶��ǻ�����----------------------------//
//�˶��ǻ�
typedef struct _LINGER_RESULT
{
	uint linger_state;
}LINGER_RESULT;
//-------------------����----------------------------------------//
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
/// ���ܷ����Ľӿں���
/******************************************************************/

/// ������ܷ���������
/// 
/// \param [out] pCaps ָ�����ܷ��������ṹ��ANALYSE_DETECT_CAPS
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int AnalyseDetectGetCaps(ANALYSE_DETECT_CAPS * pCaps);

/// �������ܼ���豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int AnalyseDetectCreate(void);


/// �������ܼ���豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int AnalyseDetectDestory(void);

/// ���ܼ����
/// 
/// \param [in] channel ͨ���š�
/// \param [in] pResult ָ�����ܼ������ṹANALYSE_DETECT_RESULT��ָ�롣
///	type���ܱ�������
/// \retval 0  ��ȡ�ɹ�
/// \retval <0  ��ȡʧ��
int AnalyseDetectGetResult(int channel, INTELLIGENTTYPE type,ANALYSE_DETECT_RESULT *pResult);


/// �������ܼ��ͨ������
/// 
/// \param [in] channel ͨ���š�
/// \param [in] pParam ָ�����ܼ������ṹANALYSE_DETECT_PARAM��ָ�롣
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int AnalyseDetectSetParameter(int channel, INTELLIGENTTYPE type,ANALYSE_DETECT_PARAM *pParam);

#ifdef __cplusplus
}
#endif

#endif //__VN_ANALYSE_API_H__


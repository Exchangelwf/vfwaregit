

#ifndef __SPLIT_API_H__
#define __SPLIT_API_H__

#include "DVRDEF.H"
#include "Video.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup SplitAPI API Split
/// ����ָ�ӿڣ���δѹ������Ƶ����������ż�Z�����к������
///	\n ��������ͼ:
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

/// ����ָ��豸����
typedef struct SPLIT_CAPS
{
	/// ֧�ֵ�ͨ����Ϸ�ʽ�����룬λ��Ŷ�Ӧö������splitt��ÿһ��ֵ����1��ʾ
	/// ֧�ָ�ö��ֵ��Ӧ�����ԣ���0��ʾ��֧�֡�
	uint dwCombine;
}SPLIT_CAPS;

/// �����������
enum split_combine_t
{
	/// ֻ���һ��ͨ���������Ŷ�Ӧ�ľ��������ͨ���š�
	SPLIT1,

	/// ͬʱ�������ͨ����������0���0-1ͨ����������1���2-3ͨ�����������ơ�
	///	\n ʾ��ͼ�����������ֱ�ʾ��������ͨ���ţ�:
	/// \code
	/// +-------------+
	/// |------+------|
	/// | 1    |   2  |
	/// |------+------|
	/// +-------------+
	/// \endcode
	SPLIT2,	
	
	/// ͬʱ���2*2�ĸ�ͨ����������0���0-3ͨ����������1���4-7ͨ�����������ơ�
	SPLIT4,
	
	/// ͬʱ����˸�ͨ����������0-7�������0-7ͨ����ֻ�������Ŷ�Ӧ��ͨ����
	/// ������ʾ�������С������ʾ��������8-15�������8-15ͨ����ֻ��������
	/// ��8��Ӧ��ͨ��������ʾ�������С������ʾ���������ơ�
	///	\n ʾ��ͼ�����������ֱ�ʾ��������ͨ���ţ�:
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

	/// ͬʱ���3*3�Ÿ�ͨ���������ϾŸ�����������Ǹ�������������0���0-7ͨ����
	/// ������1���8-15ͨ�����������ơ�
	SPLIT9,	

	/// ͬʱ���4*4ʮ����ͨ����������0���0-15ͨ����������1���16-31ͨ��������
	/// ���ơ�
	SPLIT16,

	/// ���л�ģʽ��һ��ȫ�����棬��һ��С���洰�ڡ�
	///	\n ʾ��ͼ�����������ֱ�ʾ��������ͨ���ţ�:
	/// \code
	/// +-------------+
	/// | 1           |
	/// |      +------|
	/// |      |   2  |
	/// +------+------+
	/// \endcode
	SPLITPIP,

	/// ͬʱ���6�����棬һ�����棬5��С���档�ο�8����ʾ��ͼ
	SPLIT6,

	SPLIT12,
	//#ifdef _SPLIT_VR98XX_EXTENDED_
	SPLIT13,	
	SPLIT20,
	SPLIT25,
	SPLIT33,
	SPLIT5,
	//#endif
	/// ö�ٵķָʽ��Ŀ��
	SPLIT_NR,		
};


/// ��������ָ��豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int SplitCreate(void);
int Split2Create(int index);


/// ���ٻ���ָ��豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int SplitDestory(void);
int Split2Destroy(int index);


/// ������Ƶ��ʽ��
/// 
/// \param [in] dwStandard ��Ƶ��ʽ��ȡvideo_standard_t���͵�ֵ��
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int SplitSetVstd(uint dwStandard);	
int Split2SetVstd(int index, uint dwStandard);	


/// �Լ���ͨ�����������������ϡ������ɶ��ͨ������Ƶ�����ϳ�һ��������档
/// 
/// \param [in] dwMode ����е�ͨ����Ŀ��ȡsplitt���͵�ֵ��
/// \param [in] dwIndex ��ǰͨ����Ŀ����Ϸ�ʽ����š�
/// \retval 0  ���óɹ���
/// \retval <0  ����ʧ�ܡ�
int SplitCombine(uint dwMode, uint dwIndex);
int Split2Combine(int index,  uint dwMode, uint dwIndex);


/// �õ�����ͨ�������ڵ�ǰ�ָ�ģʽ�µ��������
/// 
/// \param [in] channel ͨ���š�
/// \param [out] pRect ָ����Ƶ����ṹVIDEO_RECT��ָ�룬����ͼ��������ϵ��
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int SplitGetRect(int channel, VIDEO_RECT * pRect);
int Split2GetRect(int index,  int channel, VIDEO_RECT * pRect);


/// �õ����еĻ������� һ���Է���ȫ���Ŀ������򡣱���9������ֻ��8����������Ҫ
/// ��ʾ��Ƶ�ģ� ����һ���ǿյġ��ֱ�����·�Ļ����л���4����ʱ�� ����������Ҳ��
/// �յġ�
/// 
/// \param [out] pRect ָ����Ƶ����ṹVIDEO_RECT�����顣Ӧ�ó�����Ҫ���㹻�Ŀռ�
///        ��������������򣬿��԰Ѵ�СΪ��ǰ�ָ�ģʽ�����л�����������ָ�봫������
/// 	   ����ͼ��������ϵ��
/// \retval <0   ��ȡʧ�ܡ�
/// \retval >=0  ��õĿ��л�������
int SplitGetBlankRects(VIDEO_RECT * pRect);
int Split2GetBlankRects(int index,  VIDEO_RECT * pRect);


/// �õ���ǰ�ָ�ģʽ��������Ƶ������, ����ǵ�����, �õ��ľ��ǵ���������, �����
/// 9����,�õ��ľ���9���������, �ȵȡ�
/// 
/// \param [out] pRect ָ����Ƶ����ṹVIDEO_RECT��ָ�룬����ͼ��������ϵ��
/// \retval <0   ��ȡʧ�ܡ�
/// \retval >=0  ��õĿ��л�������
int SplitGetFullRect(VIDEO_RECT * pRect);
int Split2GetFullRect(int index,  VIDEO_RECT * pRect);


/// �õ��ָ�֧�ֵ����ԡ�
/// 
/// \param [out] pCaps ָ��ָ����ԽṹSPLIT_CAPS��ָ�롣
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int SplitGetCaps(SPLIT_CAPS * pCaps);
int Split2GetCaps(int index, SPLIT_CAPS * pCaps);
//ͬ�ַָ�ģʽ�£�ͼƬλ�öԵ���ʾ
//Channel1��Channel2�ֱ��Ǵ��Ե�λ�õ���Ƶ����ͨ��
//ע���������Ƶ����ͨ����0��ʼ����
#ifdef _SURPORT_SWITCH_
 int SplitSwitch(uint Channel1,uint Channel2);
#endif

 /*
  * @brief ����Ԥ���Ƿ���ʾ
  * @param show-true:��ʾ�� false-����ʾ��Ĭ��Ϊtrue
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


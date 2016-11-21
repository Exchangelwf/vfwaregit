#include "Split.h"
#include "ipc_inc.h"

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



/// ��������ָ��豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int SplitCreate(void){  return 0;}
int Split2Create(int index){  return 0;}


/// ���ٻ���ָ��豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int SplitDestory(void){  return 0;}
int Split2Destroy(int index){  return 0;}


/// ������Ƶ��ʽ��
/// 
/// \param [in] dwStandard ��Ƶ��ʽ��ȡvideo_standard_t���͵�ֵ��
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int SplitSetVstd(uint dwStandard){  return 0;}	
int Split2SetVstd(int index, uint dwStandard){  return 0;}	


/// �Լ���ͨ�����������������ϡ������ɶ��ͨ������Ƶ�����ϳ�һ��������档
/// 
/// \param [in] dwMode ����е�ͨ����Ŀ��ȡsplitt���͵�ֵ��
/// \param [in] dwIndex ��ǰͨ����Ŀ����Ϸ�ʽ����š�
/// \retval 0  ���óɹ���
/// \retval <0  ����ʧ�ܡ�
int SplitCombine(uint dwMode, uint dwIndex){  return 0;}
int Split2Combine(int index,  uint dwMode, uint dwIndex){  return 0;}


/// �õ�����ͨ�������ڵ�ǰ�ָ�ģʽ�µ��������
/// 
/// \param [in] channel ͨ���š�
/// \param [out] pRect ָ����Ƶ����ṹVIDEO_RECT��ָ�룬����ͼ��������ϵ��
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int SplitGetRect(int channel, VIDEO_RECT * pRect){  return 0;}
int Split2GetRect(int index,  int channel, VIDEO_RECT * pRect){  return 0;}


/// �õ����еĻ������� һ���Է���ȫ���Ŀ������򡣱���9������ֻ��8����������Ҫ
/// ��ʾ��Ƶ�ģ� ����һ���ǿյġ��ֱ�����·�Ļ����л���4����ʱ�� ����������Ҳ��
/// �յġ�
/// 
/// \param [out] pRect ָ����Ƶ����ṹVIDEO_RECT�����顣Ӧ�ó�����Ҫ���㹻�Ŀռ�
///        ��������������򣬿��԰Ѵ�СΪ��ǰ�ָ�ģʽ�����л�����������ָ�봫������
/// 	   ����ͼ��������ϵ��
/// \retval <0   ��ȡʧ�ܡ�
/// \retval >=0  ��õĿ��л�������
int SplitGetBlankRects(VIDEO_RECT * pRect){  return 0;}
int Split2GetBlankRects(int index,  VIDEO_RECT * pRect){  return 0;}


/// �õ���ǰ�ָ�ģʽ��������Ƶ������, ����ǵ�����, �õ��ľ��ǵ���������, �����
/// 9����,�õ��ľ���9���������, �ȵȡ�
/// 
/// \param [out] pRect ָ����Ƶ����ṹVIDEO_RECT��ָ�룬����ͼ��������ϵ��
/// \retval <0   ��ȡʧ�ܡ�
/// \retval >=0  ��õĿ��л�������
int SplitGetFullRect(VIDEO_RECT * pRect){  return 0;}
int Split2GetFullRect(int index,  VIDEO_RECT * pRect){  return 0;}


/// �õ��ָ�֧�ֵ����ԡ�
/// 
/// \param [out] pCaps ָ��ָ����ԽṹSPLIT_CAPS��ָ�롣
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int SplitGetCaps(SPLIT_CAPS * pCaps){  return 0;}
int Split2GetCaps(int index, SPLIT_CAPS * pCaps){  return 0;}
//ͬ�ַָ�ģʽ�£�ͼƬλ�öԵ���ʾ
//Channel1��Channel2�ֱ��Ǵ��Ե�λ�õ���Ƶ����ͨ��
//ע���������Ƶ����ͨ����0��ʼ����
#ifdef _SURPORT_SWITCH_
 int SplitSwitch(uint Channel1,uint Channel2){  return 0;}
#endif

 /*
  * @brief ����Ԥ���Ƿ���ʾ
  * @param show-true:��ʾ�� false-����ʾ��Ĭ��Ϊtrue
  */
int SplitShow(int channel, VD_BOOL bShow){  return 0;}
VD_BOOL SplitGetShowFlag(int channel){  return 0;}
/// @} end of group

int SplitPause(int channel){  return 0;}
int SplitResum(int channel){  return 0;}
int SplitZoom(int channel,VIDEO_RECT *pSourceRect, VIDEO_RECT *pDestRect){  return 0;}


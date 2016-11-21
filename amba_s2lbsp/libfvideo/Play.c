
#include "Play.h"
#include "ipc_inc.h"

/// \defgroup PlayAPI API Play
/// ��·����Ƶ�طŽ���ӿڡ�
///	\n ��������ͼ:
/// \code
///    =======================================================
///                   |                            
///	         *PlayGetChannels       
///             PlayGetCaps            
///                   |                            
///              PlayCreate                      
///       +-----------|      
///       |      PlaySetFPS
///       |      PlaySetVstd                 	
///       |           |           	 
///       |       PlayStart       
///       | +---------|-------------+--------------+     
///       | |         |(Data)       |(Ctrl)        |(Ctrl)
///       | |   PlayGetBuffer   PlayPause     PlaySetRect          
///       | |         |             |        PlaySetVolume
///       | |    PlayPutBuffer  PlayStep      PlayGetTime
///       | |         |             |       PlaySetDirection
///       | |    PlaySetEof     PlayResume   PlaySetSpeed
///       | |     PlayFlush         |              |
///       | +---------|-------------+--------------+          
///       |        PlayStop   
///       +-----------|      
///              PlayDestory                       
///                   |                            
///    =======================================================
/// \endcode
/// @{

/// �õ��طŵ�ͨ����Ŀ�������������طŵ�ͨ���������ط�ͨ�������ط�ͨ���ż�1��
/// ���������ÿ����Ƶ������ϵĻط�ͨ����һ�£���PlayGetChannels����2��������
/// ��Ƶ����ˣ��������0�ϵĻط�ͨ�����Ϊ0��1�������1�ϵĻط�ͨ�����Ϊ2��3��
/// ����ϵͳ�������������ƣ����ֻ���������ط�ͨ����������ͬһ������˿ڣ�Ҳ
/// �����ڲ�ͬ�Ķ˿ڡ�
/// 
/// \param ��
/// \returnӳ�䵽��һ��Ƶ����˿��ϵ����طŵ�ͨ����Ŀ��
int PlayGetChannels(void){  return 0;}


/// �����ط��豸
/// 
/// \param [in] channel ͨ���š�
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int PlayCreate(int channel){  return 0;}


/// ���ٻط��豸
/// 
/// \param [in] channel ͨ���š�
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int PlayDestory(int channel){  return 0;}


/// ��ʼ����, �ײ���ջط�EOF��־, ���TOF��־.
/// 
/// \param [in] channel ͨ���š�
/// \retval 0  ��ʼ�ɹ�
/// \retval <0  ��ʼʧ��
int PlayStart(int channel){  return 0;}


/// ֹͣ���룬��ǰ���ڽ��е���������Ӧ�������ء�
/// 
/// \param [in] channel ͨ���š�
/// \retval 0  ֹͣ�ɹ�
/// \retval <0  ֹͣʧ��
int PlayStop(int channel){  return 0;}


/// ��ͣ���롣
/// 
/// \param [in] channel ͨ���š�
/// \retval 0  ��ͣ�ɹ�
/// \retval <0  ��ͣʧ��
int PlayPause(int channel){  return 0;}


/// �ָ����롣
/// 
/// \param [in] channel ͨ���š�
/// \retval 0  �ָ��ɹ�
/// \retval <0  �ָ�ʧ��
int PlayResume(int channel){  return 0;}


/// ��ս��뻺�壬������ѰI֡��
/// 
/// \param [in] channel ͨ���š�
/// \retval 0  ��ճɹ�
/// \retval <0  ���ʧ��
int PlayFlush(int channel){  return 0;}


/// ��ûط���Ҫ�����ݿ����Ϣ�����طŵ�EOF��־����ʱ, ����ײ㻺���Ѿ�ȫ������
/// ��ɣ�Ӧ�ó����´ε���PlayGetBufferʱ�᷵�ط��سɹ�, ����pBlock�еĻ��峤��
/// ����Ϊ0, Ӧ�ó���ݴ�ֹͣ�ط�, �ﵽ�طŽ���ͬ����Ŀ��.
///
/// \param [in] channel ͨ���š�
/// \param [out] pBlock ָ��ط����ݿ���Ϣ�ṹPLAY_BLOCK��ָ�롣Ӧ�ó���������
///        �ṹ��ƫ���ļ�ָ�룬��׼����Ӧ��С�����ݣ�Ȼ�����PlayPutBuffer�ӿ���
/// 	   �����ݡ��൱�ڻط�ʱ�ļ��ķ��ʲ�����API�������ƣ�Ӧ�ó���ֻ�����ļ���
///        ��ȡ��
/// \retval 0  ��óɹ��������峤�ȿ���Ϊ0
/// \retval <0  �طŻ��������� Ӧ�ó��������Ҫ˯��һ���ٳ��ԡ�/// \see   PlaySetEof, PlaySetTof, PlayPutBuffer
int PlayGetBuffer(int channel, PLAY_BLOCK *pBlock){  return 0;}


/// ������ݵ��طŻ��壬�첽��ʽ���ײ�������ʱ, ��Ҫ��ջ����ڵ����ݡ�
/// 
/// \param [in] channel ͨ���š�
/// \param [in] pBuffer ��д������ݻ���ָ�롣
/// \param [in] dwCount ��д������ݳ��ȡ�
/// \retval 0  д�ɹ�
/// \retval <0  дʧ��
int PlayPutBuffer(int channel, uchar * pBuffer, uint dwCount){  return 0;}


/// ���ûطŵ��ٶȡ�
/// 
/// \param [in] channel ͨ���š�
/// \param [in] dwSpeed �ٶȴ�С��ȡ\a  play_speed_t ���͵�ֵ�����ٵ�����Խ��
///       ��ʾԽ�������ٵ�����Խ�󣬱�ʾԽ�졣
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int PlaySetSpeed(int channel, uint dwSpeed){  return 0;}


/// ���ûطŵ�������ʵ�ֶ�·�طźͻطŻ������ŵ�Ŀ�ġ������õ���ʵ�ʻ�������
/// �ɵײ�����ײ���ڷֱ���ҲӦ���Զ�ʶ���ת����
/// 
/// \param [in] channel ͨ���š�
/// \param [in] pSourceRect ��ƵԴ���򣬲������������ϵ��
/// \param [in] pDestRect ��ƵĿ�����򣬲���ͼ��������ϵ��
/// \retval 0  д�ɹ�
/// \retval <0  дʧ��
int PlaySetRect(int channel, VIDEO_RECT *pSourceRect, VIDEO_RECT *pDestRect){  return 0;}

int PlaySyncTime(int channel, SYSTEM_TIME* pSyncTime, uint* millseconds){  return 0;}

int PlaySplitCombine( uint dwMode, uint dwIndex ){  return 0;}


/// ���ûطŵ���ʽ��
/// 
/// \param [in] channel ͨ���š�
/// \param [in] dwStandard �ط���ʽ
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ�ܡ�
int PlaySetVstd(int channel,uint dwStandard){  return 0;}


/// ���ûطŵķ���
/// 
/// \param [in] channel ͨ���š�
/// \param [in] dwDirection �طŷ���ȡplay_direction_t���͵�ֵ��
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
/// \see play_direction_t
int PlaySetDirection(int channel, uint dwDirection){  return 0;}


/// ���ûطŵ�������
/// 
/// \param [in] channel ͨ���š�
/// \param [in] dwLVolume ������������ȡֵ0-100��
/// \param [in] dwRVolume ������������ȡֵ0-100��
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int PlaySetVolume(int channel, uint dwLVolume, uint dwRVolume){  return 0;}


/// �ֶ��������ţ���Ҫ�ڲ�����ͣ����ã�������������Ӧ֡�����Զ��ٴ���ͣ������
/// Ϊ��������ʽ������˺�������ʱ�ϴε������Ż�û����ɣ���ȡʣ��֡����������
/// ֡�������ֵ��Ϊ��Ҫ�������ŵ�֡����
/// 
/// \param [in] channel ͨ���š�
/// \param [in] dwFrames ÿ�β������ŵ�֡����
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int PlayStep(int channel, uint dwFrames){  return 0;}

/// ȡ��ǰ���ڻط���Ƶ�����ڵ�ʱ���롣ʱ����Ҫ�ڱ���ʱ��ӵ���Ƶ�����У��Ա�֤
/// �ط�ʱ׼ȷ��ʾʱ�䡣ͬʱ�طŽ���Ľ�����Ҳ�Ǹ������ʱ����ȷ�������, �ڴ�
/// �������ش���ʱ,�ط�ģ������ʱ��, ��ȷ���طŽ������������.
///
/// \param [in] channel ͨ���š�
/// \param [out] pTime �ط�ʱ�䣬ָ��ϵͳʱ��ṹSYSTEM_TIME��ָ�롣
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int PlayGetTime(int channel, SYSTEM_TIME * pTime){  return 0;}

/// ȡ��ǰ���ڻط���Ƶ�����ڵ�ʱ���롣ʱ����Ҫ�ڱ���ʱ��ӵ���Ƶ�����У��Ա�֤
/// �ط�ʱ׼ȷ��ʾʱ�䡣ͬʱ�طŽ���Ľ�����Ҳ�Ǹ������ʱ����ȷ�������, �ڴ�
/// �������ش���ʱ,�ط�ģ������ʱ��, ��ȷ���طŽ������������.
/// 
/// \param [in] channel ͨ���š�
/// \param [out] pTime �ط�ʱ�䣬ָ��ϵͳʱ��ṹSYSTEM_TIME��ָ�롣
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int PlayGetTimeEx(int channel, SYSTEM_TIME * pTime, uint* millseconds){  return 0;}


/// ���ûطŵĻ��ʡ�
/// 
/// \param [in] channel ͨ���š�
/// \param [in] iLevel ���ʵȼ�1-4
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int PlaySetQuality(int channel, int iLevel){  return 0;}


/// �õ��ط�֧�ֵ����ԡ�
/// 
/// \param [out] pCaps ָ��ط����ԽṹPLAY_CAPS��ָ�롣
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int PlayGetCaps(PLAY_CAPS * pCaps){  return 0;}


/// ���������ļ���ȡ��ϱ�־��End of File����
/// �ײ�ط�ģ���ڻ��������ȫ��������ɺ���������־�� 
/// 
/// \param [in] iChannel ͨ���š�
/// \param [in] bIsEof TRUE-���ñ�־��FALST-��ձ�־��
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
/// \see	PlayGetBuffer, PlayStart
void PlaySetEof(int iChannel, VD_BOOL bIsEof)
{
	 
}


/// ���ûطŵ�֡�ʡ�
/// 
/// \param [in] channel ͨ���š�
/// \param [in] fps ¼�����ݵ�ʵ�ʱ���֡�ʡ�
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
void PlaySetFPS(int channel, int fps)
{
	 
}


/// ���ûط�ʱ��Ƶ����֮��Ļط��������ɫ.
/// 
/// \param [in] color Ҫ���õ���ɫ��16���Ʊ�ʾΪ0x00BBGGRR������ֽڱ�����
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int PlaySetBkColor(uint color){  return 0;}


/// ���������ļ�����ĩβ��־��Tail of File����
/// �ײ�ط�ģ���ڽ��������������־������Tof��ʱ����Ӧ�ó�������� һ����
/// �ļ��������һ��ʱ���ã� �������Ա�֤�ļ��м�����ݴ��󱻺��ԣ����Ժ������
/// �����ˣ������ļ�ĩβ�����ݴ���ᴥ���ط�ֹͣ������ػ��ļ�ĩβ���������⣩��
///
/// \param [in] channel ͨ���š�
/// \param [in] bIsTof TRUE-���ñ�־��FALST-��ձ�־��
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��/// \see	PlayGetBuffer, PlayStart
int PlaySetTof(int channel, VD_BOOL bIsTof){  return 0;}

//!����¼���ļ��ķֱ��ʣ��ײ����ֻ�ǿ���D1�Ľ���ͨ���������¼�����ݷֱ���������ӿ�����
//!height��16λΪ¼���ļ��߶ȣ���16λΪ¼���ļ�֡��
int PlaySetResolution(int channel, uint width,uint height){  return 0;}

int PlaySetReal(int channel){  return 0;}

//!������Ӧͨ���Ľ���ط�����, �����enum play_dec_type_t ö��
int PlaySetDecType(int channel, uint dec_type){  return 0;}

/// @} end of group



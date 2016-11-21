#include "Garphics.h"
#include "ipc_inc.h"

/// \defgroup GraphicsAPI API Graphics
/// ��ͼ���豸�ӿڣ���ȡFrameBuffer��֡���壩����������Overlay�����ӣ������ȵȡ�
/// ��Ⱦ�������ϲ�ʵ�֣���ʱ��֧��Ӳ�����١�
/// \n ��������ͼ:
/// \code
///    =========================================
///                   |                         
///             GraphicsCreate                  
///         +---------|------------------+      
///         |   GraphicsSetVstd          |
///         |         |            GraphicsSetAlpha
///         |  GraphicsGetBuffer  GraphicsSetColorKey
///         |         |                  |   
///         +---------|------------------+  
///            GraphicsDestory                  
///                   |                         
///    =========================================
/// \endcode
/// @{


/// ����ͼ���豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int GraphicsCreate(void){  return 0;}
int Graphics2Create(int index){  return 0;}


/// ����ͼ���豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int GraphicsDestory(void){  return 0;}
int Graphics2Destroy(int index){  return 0;}


/// ������Ƶ��ʽ��
/// 
/// \param [in]  dwStandard ��Ƶ��ʽ��ȡvideo_standard_t���͵�ֵ��
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int GraphicsSetVstd(uint dwStandard){  return 0;}	
int Graphics2SetVstd(int index, uint dwStandard){  return 0;}	


/// ��ȡͼ�λ��ƻ��塣��Ƶ��ʽ�ı�ʱ���ܻ�ı䣬Ӧ�ó�����Ҫ���»�ȡ��
/// 
/// \param [out] pBuffer ָ����ʾ���������ṹGRAPHICS_SURFACE��ָ�롣GDIֱ��ͨ��
///        �Դ��ָ������Դ档ͼ���豸ֻ��ѡ��GDI֧�ֵ���ɫ��ʽ�е�һ�֣����GDI
///        ����֧�֣�����Ҫ�����µ���ɫ��ʽ��������GDI���ܡ�
/// \retval 0  ��ȡ�ɹ�
/// \retval <0  ��ȡʧ��
int GraphicsGetBuffer(GRAPHICS_SURFACE * pBuffer){  return 0;}
int Graphics2GetSurface(int index, GRAPHICS_SURFACE * pBuffer){  return 0;}


/// ����ͼ�ε�͸���ȡ�ͼ����ͼ����ȫ����Χ�ڰ���͸���ȵ��ӡ�
/// 
/// \param [in] alpha Ҫ���õĵ�ǰ͸���ȣ�ֵԽ�ͱ�ʾ����ʱͼ����ɫ�ķ���Խ�ͣ�Ҳ��
///        Խ͸����ȡֵ0-255��
/// \param [in] delta ͸�����Զ�����ֵ��ÿ֡ʱ��͸���ȼ��ϴ�ֵ��ֱ��͸���ȱ�����
///        ����СΪֹ��ֵΪ0��ʾ�����䡣
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int GraphicsSetAlpha(int alpha, int delta){  return 0;}
int Graphics2SetAlpha(int index, int alpha, int delta){  return 0;}


/// ����������ɫ����ͼ����ͼ��������ɫ����ʱ��ͼ����Դ�㣬ͼ����Ŀ�Ĳ㣬ͼ�ε���
/// ��ɫ���򽫱���Ƶ�滻��
/// 
/// \param [in] dwKeyLow ��Ӧ����ɫ�������ɫ������Сֵ��16���Ʊ�ʾΪ0x00BBGGRR������ֽڱ�����
/// \param [in] dwKeyHigh ��Ӧ����ɫ�������ɫ�������ֵ��16���Ʊ�ʾΪ0x00BBGGRR������ֽڱ�����
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int GraphicsSetColorKey(uint dwKeyLow, uint dwKeyHigh){  return 0;}
int Graphics2SetColorKey(int index, uint dwKeyLow, uint dwKeyHigh){  return 0;}


/// ���ù��λ�ã����ú����ȵ����ڸ����ꡣ����ģ�����ʹ�á�
/// 
/// \param [in] x ���x���ꡣ
/// \param [in] y ���y���ꡣ
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int GraphicsSetCursorPos(int x, int y){  return 0;}

/// ����GUI�ֱ���
/// \param [in] w �ֱ����е�x����
/// \param [in[ h �ֱ����е�y����
/// \return 0 ���óɹ�
/// \retval < 0 ����ʧ��
int GraphicsSetResolution(int w, int h){  return 0;}

/// @} end of group

int GraphicsShowCursor(void){  return 0;}
int GraphicsHideCursor(void){  return 0;}
int GraphicsGetCursorPos(VD_PPOINT pPoint){  return 0;}
int GraphicsGetCursorBuf(GRAPHICS_SURFACE *pBuffer){  return 0;}

/// ����OSD����������
/// \param [in] rect Ҫ������������
/// \param [in] resv ����
/// \return 0 ���óɹ�
/// \retval < 0 ����ʧ��
int GraphicsDeFlicker(VD_RECT *rect, int resv){  return 0;}

int GraphicsSetOutputDev(int iDevType){  return 0;}


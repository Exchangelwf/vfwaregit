#include "Garphics.h"
#include "ipc_inc.h"

/// \defgroup GraphicsAPI API Graphics
/// 简单图形设备接口，获取FrameBuffer（帧缓冲）描述，设置Overlay（叠加）参数等等。
/// 渲染部分由上层实现，暂时不支持硬件加速。
/// \n 调用流程图:
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


/// 创建图形设备
/// 
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int GraphicsCreate(void){  return 0;}
int Graphics2Create(int index){  return 0;}


/// 销毁图形设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int GraphicsDestory(void){  return 0;}
int Graphics2Destroy(int index){  return 0;}


/// 设置视频制式。
/// 
/// \param [in]  dwStandard 视频制式，取video_standard_t类型的值。
/// \retval 0  设置成功
/// \retval <0  设置失败
int GraphicsSetVstd(uint dwStandard){  return 0;}	
int Graphics2SetVstd(int index, uint dwStandard){  return 0;}	


/// 获取图形绘制缓冲。视频制式改变时可能会改变，应用程序需要重新获取。
/// 
/// \param [out] pBuffer 指向显示缓冲特征结构GRAPHICS_SURFACE的指针。GDI直接通过
///        显存块指针访问显存。图形设备只能选择GDI支持的颜色格式中的一种，如果GDI
///        都不支持，就需要增加新的颜色格式，并扩充GDI功能。
/// \retval 0  获取成功
/// \retval <0  获取失败
int GraphicsGetBuffer(GRAPHICS_SURFACE * pBuffer){  return 0;}
int Graphics2GetSurface(int index, GRAPHICS_SURFACE * pBuffer){  return 0;}


/// 设置图形的透明度。图形与图像在全屏范围内按此透明度叠加。
/// 
/// \param [in] alpha 要设置的当前透明度，值越低表示叠加时图像颜色的分量越低，也就
///        越透明，取值0-255。
/// \param [in] delta 透明度自动渐变值，每帧时间透明度加上此值，直到透明度变成最大
///        或最小为止。值为0表示不渐变。
/// \retval 0  设置成功
/// \retval <0  设置失败
int GraphicsSetAlpha(int alpha, int delta){  return 0;}
int Graphics2SetAlpha(int index, int alpha, int delta){  return 0;}


/// 设置锁结颜色区域。图形与图像按锁结颜色叠加时，图像是源层，图形是目的层，图形的锁
/// 结色区域将被视频替换。
/// 
/// \param [in] dwKeyLow 对应锁结色区域的颜色分量最小值，16进制表示为0x00BBGGRR，最高字节保留。
/// \param [in] dwKeyHigh 对应锁结色区域的颜色分量最大值，16进制表示为0x00BBGGRR，最高字节保留。
/// \retval 0  设置成功
/// \retval <0  设置失败
int GraphicsSetColorKey(uint dwKeyLow, uint dwKeyHigh){  return 0;}
int Graphics2SetColorKey(int index, uint dwKeyLow, uint dwKeyHigh){  return 0;}


/// 设置光标位置，设置后光标热点落在该坐标。仅供模拟程序使用。
/// 
/// \param [in] x 光标x坐标。
/// \param [in] y 光标y坐标。
/// \retval 0  设置成功
/// \retval <0  设置失败
int GraphicsSetCursorPos(int x, int y){  return 0;}

/// 设置GUI分辨率
/// \param [in] w 分辨率中的x坐标
/// \param [in[ h 分辨率中的y坐标
/// \return 0 设置成功
/// \retval < 0 设置失败
int GraphicsSetResolution(int w, int h){  return 0;}

/// @} end of group

int GraphicsShowCursor(void){  return 0;}
int GraphicsHideCursor(void){  return 0;}
int GraphicsGetCursorPos(VD_PPOINT pPoint){  return 0;}
int GraphicsGetCursorBuf(GRAPHICS_SURFACE *pBuffer){  return 0;}

/// 设置OSD抗抖动区域
/// \param [in] rect 要抗抖动的区域
/// \param [in] resv 保留
/// \return 0 设置成功
/// \retval < 0 设置失败
int GraphicsDeFlicker(VD_RECT *rect, int resv){  return 0;}

int GraphicsSetOutputDev(int iDevType){  return 0;}


#include "ExCapture.h"
#include "ipc_inc.h"

/// \defgroup ExcaptureAPI API Excapture
/// 扩展捕获接口，比如预览数据的编码。
///	\n 调用流程图:
/// \code
///    =================================
///                   |                 
///            *ExCaptureGetCaps        
///                   |                 
///             ExCaptureCreate         
///       +-----------|     
///       |      ExCaptureSetVstd       
///       |     ExCaptureSetEncode      
///       |      ExCaptureSetSize       
///       |    ExCaptureSetQuality      
///       |      ExCaptureSetFrame      
///       |           |          	 
///       |     ExCaptureStart    
///       | +---------|          
///       | |  ExCaptureGetBuffer 
///       | | ExCaptureGetBufferSize 
///       | +---------|                 
///       |      ExCaptureStop          
///       +-----------|
///           ExCaptureDestory          
///                   |                 
///    =================================
/// \endcode
/// @{


/// 创建扩展捕获设备
/// 
/// \param [in] channel 通道号。
/// \retval <0 创建失败
/// \retval 0 创建成功
int ExCaptureCreate(int channel){  return 0;}


/// 销毁扩展捕获设备
/// 
/// \param [in] channel 通道号。
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int ExCaptureDestory(int channel){  return 0;}


/// 启动扩展捕获。
/// 
/// \param [in] dwFlags 参数表示启动捕获的选项，是capture_start_t类型的值的组合。
/// \retval 0  启动成功。
/// \retval <0  启动失败。
int ExCaptureStart(uint dwFlags){  return 0;}


/// 停止扩展捕获。
/// 
/// \param 无
/// \retval 0  停止成功。
/// \retval <0  停止失败。
int ExCaptureStop(void){  return 0;}


/// 设置扩展捕获格式。
/// 
/// \param [in]  dwStandard 视频制式，取video_standard_t类型的值。
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int ExCaptureSetVstd(uint dwStandard){  return 0;}	


/// 得到捕获数据块的大小，下次读数据时接收数据的缓冲大小一定不能比这个值小。
/// 
/// \param [out]  pCount 传出的值是实际读到的字节数。
/// \retval 0  回放成功。
/// \retval <0  回放失败。
int ExCaptureGetBufferSize (uint * pCount){  return 0;}


/// 得到捕获数据块。
/// 
/// \param [in] pBuffer 用来存放捕获数据的缓冲指针。
/// \param [in] dwCount 传入的值是要读到的字节数。
/// \retval 0  回放成功。
/// \retval <0  回放失败。
int ExCaptureGetBuffer (uchar * pBuffer, uint dwCount){  return 0;}


/// 得到扩展捕获支持的特性。
/// 
/// \param [out]  pCaps 指向回放特性结构EXCAPTURE_CAPS的指针。
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int ExCaptureGetCaps(EXCAPTURE_CAPS * pCaps){  return 0;}


/// 强制I帧
int ExCaptureForceIFrame(int channel){  return 0;}


/// 编码方式
int ExCaptureSetEncode(int iChannel, int iEncode){  return 0;}
 

/// 大小
int ExCaptureSetSize(int iChannel, int iSize){  return 0;}


/// 画质  	1-6 档
int ExCaptureSetQuality(int iChannel, int iQuality){  return 0;}


/// 帧率  	1 2 3 6 12 25
int ExCaptureSetFrame(int iChannel, int iFrame){  return 0;}

/// @} end of group


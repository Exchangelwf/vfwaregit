#include "Comm.h"
#include "ipc_inc.h"

/// \defgroup CommAPI API Comm
/// 标准的串口通讯接口。
/// \n 调用流程图:
/// \code
///    =========================================
///                   |                         
///              CommCreate                  
///       +-----------|	          
///       |     CommSetAttribute  	
///       |     CommAsConsole     	
///       |           |           	 
///       |      CommOpen    
///       | +---------|      
///       | |    CommRead    
///       | |    CommWrite   
///       | |    CommPurge   
///       | +---------|      	
///       |      CommClose   
///       +-----------|
///             CommDestory                  
///                   |                         
///    =========================================
/// \endcode
/// @{

/// 创建串口设备
/// 
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int CommCreate(void){  return 0;}


/// 销毁串口设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int CommDestory(void){  return 0;}


/// 打开串口设备。
/// 
/// \retval >0  打开成功
/// \retval <=0  打开失败
int CommOpen(void){  return 0;}


/// 关闭串口设备，当前读写操作立即返回。
/// 
/// \retval 0  关闭成功
/// \retval <0  关闭失败
int CommClose(void){  return 0;}


/// 设置串口属性。
/// 
/// \param [in] pAttr 指向串口属性结构COMM_ATTR的指针。
/// \retval 0  关闭成功
/// \retval <0  关闭失败
int CommSetAttribute(COMM_ATTR * pAttr)
{
	return 0; 
}


/// 读串口数据，一直等到读出指定的字节或出错时才返回，否则阻塞。
/// 
/// \param [out] pData 数据缓冲的指针。
/// \param [in] nBytes 要读出的字节数
/// \retval >0  读出的字节数
/// \retval <=0 读失败
int CommRead(void* pData, uint nBytes)
{
	return 0; 
}


/// 写串口数据，一直等到写入指定的字节或出错时才返回，否则阻塞。
/// 
/// \param [in] pData 数据缓冲的指针。
/// \param [in] nBytes 要写入的字节数
/// \retval >0  写入的字节数
/// \retval <=0 写失败
int CommWrite(void* pData, uint nBytes)
{
	return 0; 
}


/// 中止串口读写操作或清空输入输出缓冲。
/// 
/// \param [in] dwFlags 要执行的操作,可以是下表中一个或多个值。
/// \retval 0  操作成功
/// \retval <0  操作失败
int CommPurge(uint dwFlags)
{
	return 0; 
}


/// 串口作为控制台
/// 
/// \param [in] flag 标志，0表示不作为串口，1表示作为串口。
/// \retval 0  操作成功
/// \retval <0  操作失败
int CommAsConsole(int flag)
{
	return 0; 
}


/// 得到串口是否作为控制台使用
void CommGetConsol(int* console)
{
}

/// @} end of group


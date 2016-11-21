#include "Comm.h"
#include "IUart.h"
#include "ipc_inc.h"


/// 创建串口设备
/// 
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int UARTCreate(char *strDev){  return 0;}


/// 创建串口设备
/// 
//iFlag 1 创建非阻塞的UART,阻塞0 
//
int UARTCreateNew(char *strDev,int iFlag){  return 0;}


/// 销毁串口设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int UARTDestory(int dev_fd){  return 0;}


/// 设置串口属性。
/// 
/// \param [in] pAttr 指向串口属性结构COMM_ATTR的指针。
/// \retval 0  关闭成功
/// \retval <0  关闭失败
int UARTSetAttribute(int dev_fd, COMM_ATTR *pattr){  return 0;}



/// 获取串口属性。
/// 
/// \param [in] pAttr 指向串口属性结构COMM_ATTR的指针。
/// \retval 0  关闭成功
/// \retval <0  关闭失败

int UARTGetAttribute(int dev_fd){  return 0;}


/// 读串口数据，一直等到读出指定的字节或出错时才返回，否则阻塞。
/// 
/// \param [out] pData 数据缓冲的指针。
/// \param [in] nBytes 要读出的字节数
/// \retval >0  读出的字节数
/// \retval <=0 读失败
int UARTRead(void *pdata, uint nbytes, int dev_fd){  return 0;}


/// 写串口数据，一直等到写入指定的字节或出错时才返回，否则阻塞。
/// 
/// \param [in] pData 数据缓冲的指针。
/// \param [in] nBytes 要写入的字节数
/// \retval >0  写入的字节数
/// \retval <=0 写失败
int UARTWrite(void *pdata, uint nbytes, int dev_fd){  return 0;}


/// 中止串口读写操作或清空输入输出缓冲。
/// 
/// \param [in] dwFlags 要执行的操作,可以是下表中一个或多个值。
/// \retval 0  操作成功
/// \retval <0  操作失败
int UARTPurge(uint dw_flags ,int dev_fd){  return 0;}

/// @} end of group


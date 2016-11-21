
#ifndef __SLAVE_UART_API_H__
#define __SLAVE_UART_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif


/// 串口属性结构
typedef struct SLAVEUART_ATTR
{
    uint    baudrate;    ///< 实际的波特率值。        
    uchar    databits;    ///< 实际的数据位数。    
    uchar    parity;        ///< 奇偶校验选项，取comm_parity_t类型的枚举值。    
    uchar    stopbits;    ///< 停止位数，取comm_stopbits_t类型的枚举值。    
    uchar    reserved;    ///< 保留    
} SLAVEUART_ATTR;


//在UART2Create 调用之前调用有效
//默认为阻塞模式
//iFlag 1 非阻塞0,阻塞
int SLAVEUARTSetNonBlock(int iFlag);

/// 创建前面板设备
/// 
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int SLAVEUARTCreate(void);


/// 销毁前面板设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int SLAVEUARTDestory(void);


/// 设置串口属性。
/// 
/// \param [in] pAttr 指向串口属性结构COMM_ATTR的指针。
/// \retval 0  关闭成功
/// \retval <0  关闭失败
int SLAVEUARTSetAttribute(SLAVEUART_ATTR *pAttr);

/// 读前面板数据，一直等到读出指定的字节或出错时才返回，否则阻塞。
/// 
/// \param [out] pData 数据缓冲的指针。
/// \param [in] nBytes 要读出的字节数
/// \return>0  读出的字节数
/// \retval <=0 读失败
int SLAVEUARTRead(void* pData, uint nBytes);


/// 写前面板数据，一直等到写入指定的字节或出错时才返回，否则阻塞。
/// 
/// \param [in] pData 数据缓冲的指针。
/// \param [in] nBytes 要写入的字节数
/// \return>0  写入的字节数
/// \retval <=0  写失败
int SLAVEUARTWrite(void* pData, uint nBytes);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__UART2_API_H__


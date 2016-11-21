
#ifndef __COMM_API_H__
#define __COMM_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif

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

#define COMM_PURGE_TXABORT			0x0001	///< 中止写操作
#define COMM_PURGE_RXABORT			0x0002	///< 中止读操作
#define COMM_PURGE_TXCLEAR			0x0004	///< 清空输出缓冲
#define COMM_PURGE_RXCLEAR			0x0008	///< 清空输入缓冲

/// 串口属性结构
typedef struct COMM_ATTR
 {
	uint	baudrate;	///< 实际的波特率值。		
	uchar	databits;	///< 实际的数据位数。	
	uchar	parity;		///< 奇偶校验选项，取comm_parity_t类型的枚举值。	
	uchar	stopbits;	///< 停止位数，取comm_stopbits_t类型的枚举值。	
	uchar	reserved;	///< 保留	
} COMM_ATTR;

/// 串口停止位类型
enum comm_stopbits_t 
{
	COMM_ONESTOPBIT,	///< 1 stop bit
	COMM_ONE5STOPBITS,	///< 1.5 stop bit
	COMM_TWOSTOPBITS	///< 2 stop bit
};

/// 串口校验位类型
enum comm_parity_t 
{
	COMM_NOPARITY,	///< No parity
	COMM_ODDPARITY,	///< Odd parity
	COMM_EVENPARITY,///< Even parity
	COMM_MARK,		///< 
	COMM_SPACE		///< 
};


/// 创建串口设备
/// 
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int CommCreate(void);


/// 销毁串口设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int CommDestory(void);


/// 打开串口设备。
/// 
/// \retval >0  打开成功
/// \retval <=0  打开失败
int CommOpen(void);


/// 关闭串口设备，当前读写操作立即返回。
/// 
/// \retval 0  关闭成功
/// \retval <0  关闭失败
int CommClose(void);


/// 设置串口属性。
/// 
/// \param [in] pAttr 指向串口属性结构COMM_ATTR的指针。
/// \retval 0  关闭成功
/// \retval <0  关闭失败
int CommSetAttribute(COMM_ATTR * pAttr);


/// 读串口数据，一直等到读出指定的字节或出错时才返回，否则阻塞。
/// 
/// \param [out] pData 数据缓冲的指针。
/// \param [in] nBytes 要读出的字节数
/// \retval >0  读出的字节数
/// \retval <=0 读失败
int CommRead(void* pData, uint nBytes);


/// 写串口数据，一直等到写入指定的字节或出错时才返回，否则阻塞。
/// 
/// \param [in] pData 数据缓冲的指针。
/// \param [in] nBytes 要写入的字节数
/// \retval >0  写入的字节数
/// \retval <=0 写失败
int CommWrite(void* pData, uint nBytes);


/// 中止串口读写操作或清空输入输出缓冲。
/// 
/// \param [in] dwFlags 要执行的操作,可以是下表中一个或多个值。
/// \retval 0  操作成功
/// \retval <0  操作失败
int CommPurge(uint dwFlags);


/// 串口作为控制台
/// 
/// \param [in] flag 标志，0表示不作为串口，1表示作为串口。
/// \retval 0  操作成功
/// \retval <0  操作失败
int CommAsConsole(int flag);


/// 得到串口是否作为控制台使用
void CommGetConsol(int* console);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__COMM_API_H__


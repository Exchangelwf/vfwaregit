

#ifndef __PTZ_API_H__
#define __PTZ_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup PTZAPI API PTZ
/// 云台设备通讯接口。
/// \n 调用流程图:
/// \code
///    =========================================
///                   |                         
///              PtzCreate                  
///       +-----------|	          
///       |     PtzSetAttribute  	
///       |           |           	 
///       |      PtzOpen    
///       | +---------|      
///       | |    PtzRead    
///       | |    PtzWrite   
///       | +---------|      	
///       |      PtzClose   
///       +-----------|
///             PtzDestory                  
///                   |                         
///    =========================================
/// \endcode
/// @{

/// 云台设备属性
typedef struct PTZ_ATTR
 {
	uint	baudrate;		
	uchar	databits;
	uchar	parity;
	uchar	stopbits;	
	uchar	reserved;
} PTZ_ATTR;


//在PtzCreate 调用之前调用有效
//默认为阻塞模式
//iFlag 1 非阻塞0,阻塞
int PtzSetNonBlock(int iFlag);

/// 创建云台设备
/// 
/// \param	无
/// \retval <0 创建失败
/// \retval 0 创建成功
int PtzCreate(void);


/// 销毁云台设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int PtzDestory(void);

	
/// 打开云台设备。
/// 
/// \param 无
/// \retval 0  打开成功
/// \retval <0  打开失败
int PtzOpen(void);


/// 设置云台设备属性。
/// 
/// \param [in] pattr 云台属性结构PTZ_ATTR指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int PtzSetAttribute(PTZ_ATTR *pattr);


/// 关闭云台设备，当前读写操作立即返回。
/// 
/// \param 无
/// \retval 0  关闭成功
/// \retval <0  关闭失败
int PtzClose(void);


/// 写云台控制命令，一直等到写入指定的字节或出错时才返回，否则阻塞。
/// 
/// \param [in] pData 数据缓冲的指针。
/// \param [in] len 要写入的字节数
/// \retval >0  写入的字节数
/// \retval <=0 写失败
int PtzWrite(void* pData, uint len);


/// 读云台数据，一直等到读到指定的字节或出错时才返回，否则阻塞。
/// 
/// \param [in] pData 数据缓冲的指针。
/// \param [in] len 要读出的字节数
/// \return >0  读出的字节数
/// \retval <=0 读失败
int PtzRead(void* pData, uint len);

// 明景串口升级使用
int PtzUpdate(void* pData,unsigned long len);

/// 串口异步通讯扩展
typedef struct UartExtendOperate
{
	int (*Open)();
	int (*Close)();
	int (*SetAttribute)(PTZ_ATTR *);
	int (*Read)(void *, uint);
	int (*Write)(void *, uint);
	int (*Ioctl)();
}UART_EXTEND;

//编译错误 参数名称省略
//int UartExtendWrite(UART_EXTEND *, void *pdata, uint len);
//int UartExtendRead(UART_EXTEND *, void *pdata, uint len);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__PTZ_API_H__



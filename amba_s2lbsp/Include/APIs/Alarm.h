
#ifndef __ALARM_API_H__
#define __ALARM_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup AlarmAPI API Alarm
/// 报警输入输出端口（串口）的使用，接受外部电平输入或者触发继电器控制外部电路。
///	\n 调用流程图:
/// \code
///    ===========================
///                |
///   *AlarmGetInSlots,AlarmGetOutSlots
///                |
///           AlarmCreate
///                |------------+
///       AlarmRead,AlarmWrite  |
///                |------------+
///           AlarmDestory
///                |
///    ===========================
/// \endcode
/// @{


/// 得到报警端口的输入通道数目。
/// \return 报警端口的输入通道数目
int AlarmGetInSlots(void);

/// 得到报警端口的输出通道数目。
/// \return 报警端口的输出通道数目
int AlarmGetOutSlots(void);

/// 创建报警设备
/// \retval <0 创建失败
/// \retval 0 创建成功
int AlarmCreate(void);

/// 销毁报警设备
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int AlarmDestory(void);

/// 读报警输入状态，状态改变时或出错时返回，否则阻塞。
/// \param [out] pData 指向一个uint值的指针，uint值是各个报警输入通道电平状态的掩码。
///	       低通道在低位，高通道在高位。高电平置1，低电平置0，不存在的通道置0。
/// \retval >0  读出的字节数
/// \retval <=0 读失败
int AlarmRead(uint* pData);

/// 写读取报警输出状态。
/// \param [in] pData 指向一个uint值的指针，uint值是各个报警输出通道的掩码。低通道在
///	       低位，高通道在高位。置1输出高电平，置0输出低电平，不存在的通道位无效。
/// \return >0  写入的字节数
/// \retval <=0 写失败
//---------------------------//
//tiger//start
int ResetRead(uint* pData);
int C2BRead(uint * pData);
int reset_in_read(unsigned int * pdata);
int c2b_in_read(unsigned int * pdata);
int tiger_reset_in_read(void);
//tiger//stop
//---------------------------//
int AlarmWrite(uint* pData);
/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__ALARM_API_H__


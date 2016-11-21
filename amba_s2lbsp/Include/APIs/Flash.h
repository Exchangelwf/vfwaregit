

#ifndef __FLASH_API_H__
#define __FLASH_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup FlashAPI API Flash
/// Flash直接访问API。有些应用程序升级是使用lua控制的，lua脚本通过直接访问flash，
///	更新映像文件。
/// \note 函数FlashGetSectorSize和FlashGetClientArea暂时未使用。
/// @{


/// 读出Flash上的数据
/// 
/// \param [out] buf 用来接收读出数据的缓冲。
/// \param [in] addr 读出点的扇区偏移。
/// \param [in] cnt 要读出数据的扇区数。
/// \retval 0  读成功
/// \retval <0  读失败
int	FlashRead(unsigned char *buf, unsigned long addr, unsigned long cnt);


/// 将输入写入Flash
/// 
/// \param [out] buf 待写入的数据的缓冲。
/// \param [in] addr 写入点的扇区偏移。
/// \param [in] cnt 要写入数据的扇区数。
/// \retval 0  写成功
/// \retval <0  写失败
int	FlashWrite(unsigned char *buf, unsigned long addr, unsigned long cnt);


/// 得到Flash的物理扇区大小。
/// 
/// \param [out] size 一个扇区包含的字节数
/// \retval 0  操作成功
/// \retval <0  操作失败
int FlashGetSectorSize(unsigned long *size);


/// 得到为应用程序保留的区域的起止地址, 这部分区域由应用程序来管理。
/// 
/// \param [out] from 起始扇区偏移
/// \param [out] to 截止扇区偏移
/// \retval 0  操作成功
/// \retval <0  操作失败
int FlashGetClientArea(unsigned long *from, unsigned long *to);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__FLASH_API_H__


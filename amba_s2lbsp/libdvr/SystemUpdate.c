#include "Types.h"
#include "SystemUpdate.h"
#include "ipc_inc.h"


/// 获取当前系统中的mtd*个数,bin文件升级情况下使用到
int UpdategetMtdCount(){  return 0;}

/// 获取flash相关信息，供上层应用使用
/// \return 成功返回0 失败返回-1
int UpdategetCaps(int mtdCnt, RawFlashCaps* caps){  return 0;}

/// 读取数据
/// \praram mtdname
/// \return 成功返回0 失败返回 < 0
int UpdateRead(const char *mtdname, char *buf, ulong addr, int len){  return 0;}

/// 写入数据
/// \praram mtdname
/// \return 成功返回0 失败返回 < 0
int UpdateWrite(const char *mtdname, char*buf, ulong addr, int len){  return 0;}

/// 擦除数据
/// \praram mtdname
/// \return 成功返回0 失败返回 < 0
int UpdateErase(const char *mtdname, ulong addr, int len){  return 0;}



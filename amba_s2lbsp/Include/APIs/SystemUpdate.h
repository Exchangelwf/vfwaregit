
#ifndef __UPDATE_34254235423625415425432_H
#define __UPDATE_34254235423625415425432_H

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*update_cb)(int process);
/// 升级功能重构后相关接口 2010-06-02
/// ===================================================
typedef struct RawFlashCaps
{
	int type;			///< Flash类型，0-NAND，1-NOR
	int totalSize;		///< 单个mtd*的总大小，字节为单位
	int sectorSize;		///< 扇区大小
	int reserved[29];	///< 保留
}RawFlashCaps;

/// 获取当前系统中的mtd*个数,bin文件升级情况下使用到
int UpdategetMtdCount();

/// 获取flash相关信息，供上层应用使用
/// \return 成功返回0 失败返回-1
int UpdategetCaps(int mtdCnt, RawFlashCaps* caps);

/// 读取数据
/// \praram mtdname
/// \return 成功返回0 失败返回 < 0
int UpdateRead(const char *mtdname, char *buf, ulong addr, int len);

/// 写入数据
/// \praram mtdname
/// \return 成功返回0 失败返回 < 0
int UpdateWrite(const char *mtdname, char*buf, ulong addr, int len);

/// 擦除数据
/// \praram mtdname
/// \return 成功返回0 失败返回 < 0
int UpdateErase(const char *mtdname, ulong addr, int len);

#ifdef __cplusplus
}
#endif
#endif


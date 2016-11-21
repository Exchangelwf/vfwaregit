#include "Backup.h"
#include "ipc_inc.h"

/// 得到所有备份设备的信息。
/// 
/// \param [out] pDevList 用来存放设备信息的一个BACKUP_DEVICE数组。
/// \param [in,out]  iMax 传入的值是数组的元素个数，传出的值是实际取到的个数。
///        如果实际的设备数大于数组长度，则只取传入的长度。
/// \retval 0  成功
/// \retval <0  失败
int BackupGetDevices(BACKUP_DEVICE *pDevList, int *iMax)
{
	return 0; 
}



/// 设置文件列表，并开始备份。
/// 
/// \param [in] pDev 本地或远程备份设备的指针。
/// \param [in] pFileList 存放待备份文件信息的BACKUP_FILE数组。
/// \param [in] dwCount 待备份文件的个数。
/// \retval 0  开始成功
/// \retval <0  开始失败
int BackupStart(BACKUP_DEVICE * pDev, BACKUP_FILE *pFileList, uint dwCount)
{
	return 0; 
}



/// 停止备份，如果正在备份则中止备份。
/// 
/// \param [in] pDev 本地或远程备份设备的指针。
/// \param [in] iEjectFlag 对光驱设备有效，1弹出盘片，0不弹出；非光驱设备无效
/// \retval 0  停止成功
/// \retval <0  停止失败
int BackupStop(BACKUP_DEVICE * pDev, int iEjectFlag)
{
	return 0; 
}



/// 格式化备份设备。
/// 
/// \param [in] pDev 本地或远程备份设备的指针。 
/// \retval 0  清除成功
/// \retval <0  清除失败
int BackupFormat(BACKUP_DEVICE * pDev)
{
	return 0; 
}


/// 得到设备的剩余空间。
/// 
/// \param [in] pDev 本地或远程备份设备的指针。
/// \param [out] pLength 剩余大小，以Kilo-Bytes为单位。
/// \retval 0  停止成功
/// \retval <0  停止失败
int BackupGetRemainSpace(BACKUP_DEVICE * pDev, uint *pLength)
{
	return 0; 
}



/// 打开备份文件。
/// 
/// \param [in] pDev 本地或远程备份设备的指针。
/// \param [in] pFile 待文件的信息
/// \retval 0  停止成功
/// \retval <0  停止失败
int BackupFileOpen(BACKUP_DEVICE * pDev, BACKUP_FILE * pFile)
{
	return 0; 
}

int BackupFileTell(BACKUP_DEVICE *pdev)
{
	return 0; 
}

int BackupFileSeek(BACKUP_DEVICE *pdev, uint pos)
{
	return 0; 
}

/// 将数据写到备份设备。各个文件的数据是连续写入的，以iFile来区分。
/// 
/// \param [in] pDev 本地或远程备份设备的指针。
/// \param [in] pBuffer 待写入的数据缓冲指针。
/// \param [in] dwLength 待写入的数据长度。
/// \retval 0  写入成功
/// \retval -1　 写入失败
int BackupFileStuff(BACKUP_DEVICE * pDev, uchar * pBuffer, uint dwLength)
{
	return 0; 
}



/// 关闭备份文件。
/// 
/// \param [in] pDev 本地或远程备份设备的指针。
/// \retval 0  停止成功
/// \retval <0  停止失败
int BackupFileClose(BACKUP_DEVICE * pDev)
{
	return 0; 
}


/// 挂载备份设备。
/// 
/// \param [in] pDev 本地或远程备份设备的指针。
/// \retval 0  停止成功
/// \retval <0  停止失败
int BackupPlayMount(BACKUP_DEVICE *pDev)
{
	return 0; 
}


/// 卸载备份设备。
/// 
/// \param [in] pDev 本地或远程备份设备的指针。
/// \retval 0  停止成功
/// \retval <0  停止失败
int BackupPlayUmount(BACKUP_DEVICE *pDev)
{
	return 0; 
}

int BackupUmountDir(const char *dir)
{
	return 0; 
}



/// 获取备份设备上的文件
/// 
/// \param [in] pDev 本地或远程备份设备的指针。
/// \param [out] pList 文件列表首地址
/// \param [in,out] pNum 传入的设备文件最大个数，返回搜索到的文件个数
/// \param [in] condition 搜索条件
/// \retval 0  停止成功
/// \retval <0  停止失败
int BackupPlayGetFiles(VD_IN BACKUP_DEVICE *pDev, VD_OUT BACKUP_FILE *pList, VD_IN_OUT int *pNum, void *condition)
{
	return 0; 
}

/// 得到备份设备目录列表
/// 
/// \param [in]	pdev 设备结构指针
/// \param [out] plist 文件列表首地址
/// \param [in,out] pnum 传入所能存放的最大文件个数，传出实际搜索到的文件个数
/// \retval 0 成功
/// \retval -1 失败
int BackupPlayGetDir(VD_IN BACKUP_DEVICE *pdev, VD_OUT BACKUP_FILE *plist, VD_IN_OUT int *pnum)
{
	return 0; 
}

/// 获取备份设备上的文件
/// 
/// \param [in] pDev 本地或远程备份设备的指针。
/// \param [out] pList 文件列表首地址
/// \param [in,out] pNum 传入的设备文件最大个数，返回搜索到的文件个数
/// \param [in] condition 搜索条件
/// \retval 0  停止成功
/// \retval <0  停止失败
int BackupGetFiles(VD_IN char *directory, VD_OUT BACKUP_FILE *pList, VD_IN_OUT int *pNum, void *condition)
{
	return 0; 
}


/// 得到备份设备目录列表
/// 
/// \param [in]	pdev 设备结构指针
/// \param [out] plist 文件列表首地址
/// \param [in,out] pnum 传入所能存放的最大文件个数，传出实际搜索到的文件个数
/// \retval 0 成功
/// \retval -1 失败
int BackupGetDir(VD_IN char *directory, VD_OUT BACKUP_FILE *plist, VD_IN_OUT int *pnum)
{
	return 0; 
}

/// 加载备份设备盘片，主要指刻录机
/// 
/// \param [in] pdev 本地或远程备份设备的指针。
/// \retval 0  停止成功
/// \retval <0  停止失败
int BackupMediaLoad(BACKUP_DEVICE *pdev)
{
	return 0; 
}


/// 弹出备份设备盘片，主要指刻录机。
/// 
/// \param [in] pdev 本地或远程备份设备的指针。
/// \retval 0  停止成功
/// \retval <0  停止失败
int BackupMediaEject(BACKUP_DEVICE *pdev)
{
	return 0; 
}


/// 备份信息头转换
/// \param [in] BACKUP_DEVICE备份设备结构指针
/// \retval 0 失败
/// \retval BACKUP_DEVICE_INFO* 备份结构指针
BACKUP_DEVICE_INFO *BackupDevInfo(BACKUP_DEVICE *pdev)
{
	return 0; 
}

int BuckupCdrwInit(void *dev, BACKUP_FILE *fileList, int num)
{
	return 0; 
}

int BuckupCdrwRelease(void *dev)
{
	return 0; 
}

/// @} end of group



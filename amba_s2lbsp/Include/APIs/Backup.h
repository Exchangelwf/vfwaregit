

#ifndef __BACKUP_API_H__
#define __BACKUP_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif

#define VD_OUT
#define VD_IN const
#define VD_IN_OUT
/// \defgroup BackupAPI API Backup
/// 统一备份接口，支持到本地或远程标准文件系统的拷贝或者光盘刻录。
/// @{

/// 备份设备类型
typedef enum BDEV_TYTE
{
	BT_DHFS,			///< 睿威文件系统。
	BT_DISK,			///< 移动硬盘。
	BT_CDRW,			///< 普通的光盘刻录
	BT_CDRW_UDF,        ///< 采用UDF方式的光盘刻录
} BDEV_TYPE;

enum BKTYPE
{
	BK_NONE = 0,	/*!< 不备份 */
	BK_BACKUP = 1,	/*!< 备份不同的数据 */
	BK_REALTIME = 2,	/*!< 刻录相同的数据 */
	BK_PLAY = 3,	/*!< 回放光盘 */
	BK_TYPE_NUM,
};

/// 备份设备总线类型
typedef enum BDEV_BUS
{
	BB_USB,				///< USB接口
	BB_1394,			///< 1394接口。
	BB_IDE,				///< IDE接口。
	BB_SATA,            ///< SATA
} BDEV_BUS;

/// 备份设备结构
typedef struct BACKUP_DEVICE
{
	struct BACKUP_DEVICE		*prev;	///< 指向上一个设备指针
	struct BACKUP_DEVICE		*next;	///< 指向下一个设备指针

	char		name[32];		///< 备份设备的名称。

	int			type;			///< 备份设备的介质类型，取枚举类型BDEV_TYTE的值。
	int			bus;			///< 备份设备的接口类型，取枚举类型BDEV_BUS的值。

	unsigned int	capability;	///< 总容量，以Kilo-Bytes为单位。
	unsigned int	remain;		///< 剩余容量，以Kilo-Bytes为单位。
	char		directory[128];	///< 远程备份的目录 
	int			flag;			///< 标志1，仅供底层使用。

	void		*priv;			///< 私有数据，仅供底层使用。
} BACKUP_DEVICE;

// 备份信息头
typedef struct __BACKUP_DEVICE_INFO
{
	char name[32];	 // 设备文件名

 	char vendor[12]; // 厂商
	char model[20];	 // 型号
	char rev[8];	 // 版本
	char type[36];	 // 类型
	char ansi[4];	 // ANSI SCSI revision

	char resv[32];	 // reserved
} BACKUP_DEVICE_INFO;

/// 备份文件结构
typedef struct BACKUP_FILE
{
	char				name[128];	///< 待备份的文件名。	
	uint64				length;		///< 文件名长度
	//	unsigned long		length_high;		< 文件长度的高位 
} BACKUP_FILE;

/// 备份返回值
enum backup_ret_t 
{
	BACKUP_RET_OK,					///< 
	BACKUP_RET_DEVERR,				///< 
	BACKUP_RET_OVER					///< 
};

/// 得到所有备份设备的信息。
/// 
/// \param [out] pDevList 用来存放设备信息的一个BACKUP_DEVICE数组。
/// \param [in,out]  iMax 传入的值是数组的元素个数，传出的值是实际取到的个数。
///        如果实际的设备数大于数组长度，则只取传入的长度。
/// \retval 0  成功
/// \retval <0  失败
int BackupGetDevices(BACKUP_DEVICE *pDevList, int *iMax);


/// 设置文件列表，并开始备份。
/// 
/// \param [in] pDev 本地或远程备份设备的指针。
/// \param [in] pFileList 存放待备份文件信息的BACKUP_FILE数组。
/// \param [in] dwCount 待备份文件的个数。
/// \retval 0  开始成功
/// \retval <0  开始失败
int BackupStart(BACKUP_DEVICE * pDev, BACKUP_FILE *pFileList, uint dwCount);


/// 停止备份，如果正在备份则中止备份。
/// 
/// \param [in] pDev 本地或远程备份设备的指针。
/// \param [in] iEjectFlag 对光驱设备有效，1弹出盘片，0不弹出；非光驱设备无效
/// \retval 0  停止成功
/// \retval <0  停止失败
int BackupStop(BACKUP_DEVICE * pDev, int iEjectFlag);


/// 格式化备份设备。
/// 
/// \param [in] pDev 本地或远程备份设备的指针。 
/// \retval 0  清除成功
/// \retval <0  清除失败
int BackupFormat(BACKUP_DEVICE * pDev);


/// 得到设备的剩余空间。
/// 
/// \param [in] pDev 本地或远程备份设备的指针。
/// \param [out] pLength 剩余大小，以Kilo-Bytes为单位。
/// \retval 0  停止成功
/// \retval <0  停止失败
int BackupGetRemainSpace(BACKUP_DEVICE * pDev, uint *pLength);


/// 打开备份文件。
/// 
/// \param [in] pDev 本地或远程备份设备的指针。
/// \param [in] pFile 待文件的信息
/// \retval 0  停止成功
/// \retval <0  停止失败
int BackupFileOpen(BACKUP_DEVICE * pDev, BACKUP_FILE * pFile);

int BackupFileTell(BACKUP_DEVICE *pdev);

int BackupFileSeek(BACKUP_DEVICE *pdev, uint pos);

/// 将数据写到备份设备。各个文件的数据是连续写入的，以iFile来区分。
/// 
/// \param [in] pDev 本地或远程备份设备的指针。
/// \param [in] pBuffer 待写入的数据缓冲指针。
/// \param [in] dwLength 待写入的数据长度。
/// \retval 0  写入成功
/// \retval -1　 写入失败
int BackupFileStuff(BACKUP_DEVICE * pDev, uchar * pBuffer, uint dwLength);


/// 关闭备份文件。
/// 
/// \param [in] pDev 本地或远程备份设备的指针。
/// \retval 0  停止成功
/// \retval <0  停止失败
int BackupFileClose(BACKUP_DEVICE * pDev);


/// 挂载备份设备。
/// 
/// \param [in] pDev 本地或远程备份设备的指针。
/// \retval 0  停止成功
/// \retval <0  停止失败
int BackupPlayMount(BACKUP_DEVICE *pDev);


/// 卸载备份设备。
/// 
/// \param [in] pDev 本地或远程备份设备的指针。
/// \retval 0  停止成功
/// \retval <0  停止失败
int BackupPlayUmount(BACKUP_DEVICE *pDev);

int BackupUmountDir(const char *dir);


/// 获取备份设备上的文件
/// 
/// \param [in] pDev 本地或远程备份设备的指针。
/// \param [out] pList 文件列表首地址
/// \param [in,out] pNum 传入的设备文件最大个数，返回搜索到的文件个数
/// \param [in] condition 搜索条件
/// \retval 0  停止成功
/// \retval <0  停止失败
int BackupPlayGetFiles(VD_IN BACKUP_DEVICE *pDev, VD_OUT BACKUP_FILE *pList, VD_IN_OUT int *pNum, void *condition);


/// 得到备份设备目录列表
/// 
/// \param [in]	pdev 设备结构指针
/// \param [out] plist 文件列表首地址
/// \param [in,out] pnum 传入所能存放的最大文件个数，传出实际搜索到的文件个数
/// \retval 0 成功
/// \retval -1 失败
int BackupPlayGetDir(VD_IN BACKUP_DEVICE *pdev, VD_OUT BACKUP_FILE *plist, VD_IN_OUT int *pnum);

/// 获取备份设备上的文件
/// 
/// \param [in] pDev 本地或远程备份设备的指针。
/// \param [out] pList 文件列表首地址
/// \param [in,out] pNum 传入的设备文件最大个数，返回搜索到的文件个数
/// \param [in] condition 搜索条件
/// \retval 0  停止成功
/// \retval <0  停止失败
int BackupGetFiles(VD_IN char *directory, VD_OUT BACKUP_FILE *pList, VD_IN_OUT int *pNum, void *condition);


/// 得到备份设备目录列表
/// 
/// \param [in]	pdev 设备结构指针
/// \param [out] plist 文件列表首地址
/// \param [in,out] pnum 传入所能存放的最大文件个数，传出实际搜索到的文件个数
/// \retval 0 成功
/// \retval -1 失败
int BackupGetDir(VD_IN char *directory, VD_OUT BACKUP_FILE *plist, VD_IN_OUT int *pnum);

/// 加载备份设备盘片，主要指刻录机
/// 
/// \param [in] pdev 本地或远程备份设备的指针。
/// \retval 0  停止成功
/// \retval <0  停止失败
int BackupMediaLoad(BACKUP_DEVICE *pdev);


/// 弹出备份设备盘片，主要指刻录机。
/// 
/// \param [in] pdev 本地或远程备份设备的指针。
/// \retval 0  停止成功
/// \retval <0  停止失败
int BackupMediaEject(BACKUP_DEVICE *pdev);


/// 备份信息头转换
/// \param [in] BACKUP_DEVICE备份设备结构指针
/// \retval 0 失败
/// \retval BACKUP_DEVICE_INFO* 备份结构指针
BACKUP_DEVICE_INFO *BackupDevInfo(BACKUP_DEVICE *pdev);


int BuckupCdrwInit(void *dev, BACKUP_FILE *fileList, int num);
int BuckupCdrwRelease(void *dev);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__BACKUP_API_H__


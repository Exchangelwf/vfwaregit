

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
/// ͳһ���ݽӿڣ�֧�ֵ����ػ�Զ�̱�׼�ļ�ϵͳ�Ŀ������߹��̿�¼��
/// @{

/// �����豸����
typedef enum BDEV_TYTE
{
	BT_DHFS,			///< ����ļ�ϵͳ��
	BT_DISK,			///< �ƶ�Ӳ�̡�
	BT_CDRW,			///< ��ͨ�Ĺ��̿�¼
	BT_CDRW_UDF,        ///< ����UDF��ʽ�Ĺ��̿�¼
} BDEV_TYPE;

enum BKTYPE
{
	BK_NONE = 0,	/*!< ������ */
	BK_BACKUP = 1,	/*!< ���ݲ�ͬ������ */
	BK_REALTIME = 2,	/*!< ��¼��ͬ������ */
	BK_PLAY = 3,	/*!< �طŹ��� */
	BK_TYPE_NUM,
};

/// �����豸��������
typedef enum BDEV_BUS
{
	BB_USB,				///< USB�ӿ�
	BB_1394,			///< 1394�ӿڡ�
	BB_IDE,				///< IDE�ӿڡ�
	BB_SATA,            ///< SATA
} BDEV_BUS;

/// �����豸�ṹ
typedef struct BACKUP_DEVICE
{
	struct BACKUP_DEVICE		*prev;	///< ָ����һ���豸ָ��
	struct BACKUP_DEVICE		*next;	///< ָ����һ���豸ָ��

	char		name[32];		///< �����豸�����ơ�

	int			type;			///< �����豸�Ľ������ͣ�ȡö������BDEV_TYTE��ֵ��
	int			bus;			///< �����豸�Ľӿ����ͣ�ȡö������BDEV_BUS��ֵ��

	unsigned int	capability;	///< ����������Kilo-BytesΪ��λ��
	unsigned int	remain;		///< ʣ����������Kilo-BytesΪ��λ��
	char		directory[128];	///< Զ�̱��ݵ�Ŀ¼ 
	int			flag;			///< ��־1�������ײ�ʹ�á�

	void		*priv;			///< ˽�����ݣ������ײ�ʹ�á�
} BACKUP_DEVICE;

// ������Ϣͷ
typedef struct __BACKUP_DEVICE_INFO
{
	char name[32];	 // �豸�ļ���

 	char vendor[12]; // ����
	char model[20];	 // �ͺ�
	char rev[8];	 // �汾
	char type[36];	 // ����
	char ansi[4];	 // ANSI SCSI revision

	char resv[32];	 // reserved
} BACKUP_DEVICE_INFO;

/// �����ļ��ṹ
typedef struct BACKUP_FILE
{
	char				name[128];	///< �����ݵ��ļ�����	
	uint64				length;		///< �ļ�������
	//	unsigned long		length_high;		< �ļ����ȵĸ�λ 
} BACKUP_FILE;

/// ���ݷ���ֵ
enum backup_ret_t 
{
	BACKUP_RET_OK,					///< 
	BACKUP_RET_DEVERR,				///< 
	BACKUP_RET_OVER					///< 
};

/// �õ����б����豸����Ϣ��
/// 
/// \param [out] pDevList ��������豸��Ϣ��һ��BACKUP_DEVICE���顣
/// \param [in,out]  iMax �����ֵ�������Ԫ�ظ�����������ֵ��ʵ��ȡ���ĸ�����
///        ���ʵ�ʵ��豸���������鳤�ȣ���ֻȡ����ĳ��ȡ�
/// \retval 0  �ɹ�
/// \retval <0  ʧ��
int BackupGetDevices(BACKUP_DEVICE *pDevList, int *iMax);


/// �����ļ��б�����ʼ���ݡ�
/// 
/// \param [in] pDev ���ػ�Զ�̱����豸��ָ�롣
/// \param [in] pFileList ��Ŵ������ļ���Ϣ��BACKUP_FILE���顣
/// \param [in] dwCount �������ļ��ĸ�����
/// \retval 0  ��ʼ�ɹ�
/// \retval <0  ��ʼʧ��
int BackupStart(BACKUP_DEVICE * pDev, BACKUP_FILE *pFileList, uint dwCount);


/// ֹͣ���ݣ�������ڱ�������ֹ���ݡ�
/// 
/// \param [in] pDev ���ػ�Զ�̱����豸��ָ�롣
/// \param [in] iEjectFlag �Թ����豸��Ч��1������Ƭ��0���������ǹ����豸��Ч
/// \retval 0  ֹͣ�ɹ�
/// \retval <0  ֹͣʧ��
int BackupStop(BACKUP_DEVICE * pDev, int iEjectFlag);


/// ��ʽ�������豸��
/// 
/// \param [in] pDev ���ػ�Զ�̱����豸��ָ�롣 
/// \retval 0  ����ɹ�
/// \retval <0  ���ʧ��
int BackupFormat(BACKUP_DEVICE * pDev);


/// �õ��豸��ʣ��ռ䡣
/// 
/// \param [in] pDev ���ػ�Զ�̱����豸��ָ�롣
/// \param [out] pLength ʣ���С����Kilo-BytesΪ��λ��
/// \retval 0  ֹͣ�ɹ�
/// \retval <0  ֹͣʧ��
int BackupGetRemainSpace(BACKUP_DEVICE * pDev, uint *pLength);


/// �򿪱����ļ���
/// 
/// \param [in] pDev ���ػ�Զ�̱����豸��ָ�롣
/// \param [in] pFile ���ļ�����Ϣ
/// \retval 0  ֹͣ�ɹ�
/// \retval <0  ֹͣʧ��
int BackupFileOpen(BACKUP_DEVICE * pDev, BACKUP_FILE * pFile);

int BackupFileTell(BACKUP_DEVICE *pdev);

int BackupFileSeek(BACKUP_DEVICE *pdev, uint pos);

/// ������д�������豸�������ļ�������������д��ģ���iFile�����֡�
/// 
/// \param [in] pDev ���ػ�Զ�̱����豸��ָ�롣
/// \param [in] pBuffer ��д������ݻ���ָ�롣
/// \param [in] dwLength ��д������ݳ��ȡ�
/// \retval 0  д��ɹ�
/// \retval -1�� д��ʧ��
int BackupFileStuff(BACKUP_DEVICE * pDev, uchar * pBuffer, uint dwLength);


/// �رձ����ļ���
/// 
/// \param [in] pDev ���ػ�Զ�̱����豸��ָ�롣
/// \retval 0  ֹͣ�ɹ�
/// \retval <0  ֹͣʧ��
int BackupFileClose(BACKUP_DEVICE * pDev);


/// ���ر����豸��
/// 
/// \param [in] pDev ���ػ�Զ�̱����豸��ָ�롣
/// \retval 0  ֹͣ�ɹ�
/// \retval <0  ֹͣʧ��
int BackupPlayMount(BACKUP_DEVICE *pDev);


/// ж�ر����豸��
/// 
/// \param [in] pDev ���ػ�Զ�̱����豸��ָ�롣
/// \retval 0  ֹͣ�ɹ�
/// \retval <0  ֹͣʧ��
int BackupPlayUmount(BACKUP_DEVICE *pDev);

int BackupUmountDir(const char *dir);


/// ��ȡ�����豸�ϵ��ļ�
/// 
/// \param [in] pDev ���ػ�Զ�̱����豸��ָ�롣
/// \param [out] pList �ļ��б��׵�ַ
/// \param [in,out] pNum ������豸�ļ����������������������ļ�����
/// \param [in] condition ��������
/// \retval 0  ֹͣ�ɹ�
/// \retval <0  ֹͣʧ��
int BackupPlayGetFiles(VD_IN BACKUP_DEVICE *pDev, VD_OUT BACKUP_FILE *pList, VD_IN_OUT int *pNum, void *condition);


/// �õ������豸Ŀ¼�б�
/// 
/// \param [in]	pdev �豸�ṹָ��
/// \param [out] plist �ļ��б��׵�ַ
/// \param [in,out] pnum �������ܴ�ŵ�����ļ�����������ʵ�����������ļ�����
/// \retval 0 �ɹ�
/// \retval -1 ʧ��
int BackupPlayGetDir(VD_IN BACKUP_DEVICE *pdev, VD_OUT BACKUP_FILE *plist, VD_IN_OUT int *pnum);

/// ��ȡ�����豸�ϵ��ļ�
/// 
/// \param [in] pDev ���ػ�Զ�̱����豸��ָ�롣
/// \param [out] pList �ļ��б��׵�ַ
/// \param [in,out] pNum ������豸�ļ����������������������ļ�����
/// \param [in] condition ��������
/// \retval 0  ֹͣ�ɹ�
/// \retval <0  ֹͣʧ��
int BackupGetFiles(VD_IN char *directory, VD_OUT BACKUP_FILE *pList, VD_IN_OUT int *pNum, void *condition);


/// �õ������豸Ŀ¼�б�
/// 
/// \param [in]	pdev �豸�ṹָ��
/// \param [out] plist �ļ��б��׵�ַ
/// \param [in,out] pnum �������ܴ�ŵ�����ļ�����������ʵ�����������ļ�����
/// \retval 0 �ɹ�
/// \retval -1 ʧ��
int BackupGetDir(VD_IN char *directory, VD_OUT BACKUP_FILE *plist, VD_IN_OUT int *pnum);

/// ���ر����豸��Ƭ����Ҫָ��¼��
/// 
/// \param [in] pdev ���ػ�Զ�̱����豸��ָ�롣
/// \retval 0  ֹͣ�ɹ�
/// \retval <0  ֹͣʧ��
int BackupMediaLoad(BACKUP_DEVICE *pdev);


/// ���������豸��Ƭ����Ҫָ��¼����
/// 
/// \param [in] pdev ���ػ�Զ�̱����豸��ָ�롣
/// \retval 0  ֹͣ�ɹ�
/// \retval <0  ֹͣʧ��
int BackupMediaEject(BACKUP_DEVICE *pdev);


/// ������Ϣͷת��
/// \param [in] BACKUP_DEVICE�����豸�ṹָ��
/// \retval 0 ʧ��
/// \retval BACKUP_DEVICE_INFO* ���ݽṹָ��
BACKUP_DEVICE_INFO *BackupDevInfo(BACKUP_DEVICE *pdev);


int BuckupCdrwInit(void *dev, BACKUP_FILE *fileList, int num);
int BuckupCdrwRelease(void *dev);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__BACKUP_API_H__


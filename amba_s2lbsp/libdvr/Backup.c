#include "Backup.h"
#include "ipc_inc.h"

/// �õ����б����豸����Ϣ��
/// 
/// \param [out] pDevList ��������豸��Ϣ��һ��BACKUP_DEVICE���顣
/// \param [in,out]  iMax �����ֵ�������Ԫ�ظ�����������ֵ��ʵ��ȡ���ĸ�����
///        ���ʵ�ʵ��豸���������鳤�ȣ���ֻȡ����ĳ��ȡ�
/// \retval 0  �ɹ�
/// \retval <0  ʧ��
int BackupGetDevices(BACKUP_DEVICE *pDevList, int *iMax)
{
	return 0; 
}



/// �����ļ��б�����ʼ���ݡ�
/// 
/// \param [in] pDev ���ػ�Զ�̱����豸��ָ�롣
/// \param [in] pFileList ��Ŵ������ļ���Ϣ��BACKUP_FILE���顣
/// \param [in] dwCount �������ļ��ĸ�����
/// \retval 0  ��ʼ�ɹ�
/// \retval <0  ��ʼʧ��
int BackupStart(BACKUP_DEVICE * pDev, BACKUP_FILE *pFileList, uint dwCount)
{
	return 0; 
}



/// ֹͣ���ݣ�������ڱ�������ֹ���ݡ�
/// 
/// \param [in] pDev ���ػ�Զ�̱����豸��ָ�롣
/// \param [in] iEjectFlag �Թ����豸��Ч��1������Ƭ��0���������ǹ����豸��Ч
/// \retval 0  ֹͣ�ɹ�
/// \retval <0  ֹͣʧ��
int BackupStop(BACKUP_DEVICE * pDev, int iEjectFlag)
{
	return 0; 
}



/// ��ʽ�������豸��
/// 
/// \param [in] pDev ���ػ�Զ�̱����豸��ָ�롣 
/// \retval 0  ����ɹ�
/// \retval <0  ���ʧ��
int BackupFormat(BACKUP_DEVICE * pDev)
{
	return 0; 
}


/// �õ��豸��ʣ��ռ䡣
/// 
/// \param [in] pDev ���ػ�Զ�̱����豸��ָ�롣
/// \param [out] pLength ʣ���С����Kilo-BytesΪ��λ��
/// \retval 0  ֹͣ�ɹ�
/// \retval <0  ֹͣʧ��
int BackupGetRemainSpace(BACKUP_DEVICE * pDev, uint *pLength)
{
	return 0; 
}



/// �򿪱����ļ���
/// 
/// \param [in] pDev ���ػ�Զ�̱����豸��ָ�롣
/// \param [in] pFile ���ļ�����Ϣ
/// \retval 0  ֹͣ�ɹ�
/// \retval <0  ֹͣʧ��
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

/// ������д�������豸�������ļ�������������д��ģ���iFile�����֡�
/// 
/// \param [in] pDev ���ػ�Զ�̱����豸��ָ�롣
/// \param [in] pBuffer ��д������ݻ���ָ�롣
/// \param [in] dwLength ��д������ݳ��ȡ�
/// \retval 0  д��ɹ�
/// \retval -1�� д��ʧ��
int BackupFileStuff(BACKUP_DEVICE * pDev, uchar * pBuffer, uint dwLength)
{
	return 0; 
}



/// �رձ����ļ���
/// 
/// \param [in] pDev ���ػ�Զ�̱����豸��ָ�롣
/// \retval 0  ֹͣ�ɹ�
/// \retval <0  ֹͣʧ��
int BackupFileClose(BACKUP_DEVICE * pDev)
{
	return 0; 
}


/// ���ر����豸��
/// 
/// \param [in] pDev ���ػ�Զ�̱����豸��ָ�롣
/// \retval 0  ֹͣ�ɹ�
/// \retval <0  ֹͣʧ��
int BackupPlayMount(BACKUP_DEVICE *pDev)
{
	return 0; 
}


/// ж�ر����豸��
/// 
/// \param [in] pDev ���ػ�Զ�̱����豸��ָ�롣
/// \retval 0  ֹͣ�ɹ�
/// \retval <0  ֹͣʧ��
int BackupPlayUmount(BACKUP_DEVICE *pDev)
{
	return 0; 
}

int BackupUmountDir(const char *dir)
{
	return 0; 
}



/// ��ȡ�����豸�ϵ��ļ�
/// 
/// \param [in] pDev ���ػ�Զ�̱����豸��ָ�롣
/// \param [out] pList �ļ��б��׵�ַ
/// \param [in,out] pNum ������豸�ļ����������������������ļ�����
/// \param [in] condition ��������
/// \retval 0  ֹͣ�ɹ�
/// \retval <0  ֹͣʧ��
int BackupPlayGetFiles(VD_IN BACKUP_DEVICE *pDev, VD_OUT BACKUP_FILE *pList, VD_IN_OUT int *pNum, void *condition)
{
	return 0; 
}

/// �õ������豸Ŀ¼�б�
/// 
/// \param [in]	pdev �豸�ṹָ��
/// \param [out] plist �ļ��б��׵�ַ
/// \param [in,out] pnum �������ܴ�ŵ�����ļ�����������ʵ�����������ļ�����
/// \retval 0 �ɹ�
/// \retval -1 ʧ��
int BackupPlayGetDir(VD_IN BACKUP_DEVICE *pdev, VD_OUT BACKUP_FILE *plist, VD_IN_OUT int *pnum)
{
	return 0; 
}

/// ��ȡ�����豸�ϵ��ļ�
/// 
/// \param [in] pDev ���ػ�Զ�̱����豸��ָ�롣
/// \param [out] pList �ļ��б��׵�ַ
/// \param [in,out] pNum ������豸�ļ����������������������ļ�����
/// \param [in] condition ��������
/// \retval 0  ֹͣ�ɹ�
/// \retval <0  ֹͣʧ��
int BackupGetFiles(VD_IN char *directory, VD_OUT BACKUP_FILE *pList, VD_IN_OUT int *pNum, void *condition)
{
	return 0; 
}


/// �õ������豸Ŀ¼�б�
/// 
/// \param [in]	pdev �豸�ṹָ��
/// \param [out] plist �ļ��б��׵�ַ
/// \param [in,out] pnum �������ܴ�ŵ�����ļ�����������ʵ�����������ļ�����
/// \retval 0 �ɹ�
/// \retval -1 ʧ��
int BackupGetDir(VD_IN char *directory, VD_OUT BACKUP_FILE *plist, VD_IN_OUT int *pnum)
{
	return 0; 
}

/// ���ر����豸��Ƭ����Ҫָ��¼��
/// 
/// \param [in] pdev ���ػ�Զ�̱����豸��ָ�롣
/// \retval 0  ֹͣ�ɹ�
/// \retval <0  ֹͣʧ��
int BackupMediaLoad(BACKUP_DEVICE *pdev)
{
	return 0; 
}


/// ���������豸��Ƭ����Ҫָ��¼����
/// 
/// \param [in] pdev ���ػ�Զ�̱����豸��ָ�롣
/// \retval 0  ֹͣ�ɹ�
/// \retval <0  ֹͣʧ��
int BackupMediaEject(BACKUP_DEVICE *pdev)
{
	return 0; 
}


/// ������Ϣͷת��
/// \param [in] BACKUP_DEVICE�����豸�ṹָ��
/// \retval 0 ʧ��
/// \retval BACKUP_DEVICE_INFO* ���ݽṹָ��
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



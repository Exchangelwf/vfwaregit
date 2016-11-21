
#ifndef __UPDATE_34254235423625415425432_H
#define __UPDATE_34254235423625415425432_H

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*update_cb)(int process);
/// ���������ع�����ؽӿ� 2010-06-02
/// ===================================================
typedef struct RawFlashCaps
{
	int type;			///< Flash���ͣ�0-NAND��1-NOR
	int totalSize;		///< ����mtd*���ܴ�С���ֽ�Ϊ��λ
	int sectorSize;		///< ������С
	int reserved[29];	///< ����
}RawFlashCaps;

/// ��ȡ��ǰϵͳ�е�mtd*����,bin�ļ����������ʹ�õ�
int UpdategetMtdCount();

/// ��ȡflash�����Ϣ�����ϲ�Ӧ��ʹ��
/// \return �ɹ�����0 ʧ�ܷ���-1
int UpdategetCaps(int mtdCnt, RawFlashCaps* caps);

/// ��ȡ����
/// \praram mtdname
/// \return �ɹ�����0 ʧ�ܷ��� < 0
int UpdateRead(const char *mtdname, char *buf, ulong addr, int len);

/// д������
/// \praram mtdname
/// \return �ɹ�����0 ʧ�ܷ��� < 0
int UpdateWrite(const char *mtdname, char*buf, ulong addr, int len);

/// ��������
/// \praram mtdname
/// \return �ɹ�����0 ʧ�ܷ��� < 0
int UpdateErase(const char *mtdname, ulong addr, int len);

#ifdef __cplusplus
}
#endif
#endif


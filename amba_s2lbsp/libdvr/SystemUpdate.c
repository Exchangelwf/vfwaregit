#include "Types.h"
#include "SystemUpdate.h"
#include "ipc_inc.h"


/// ��ȡ��ǰϵͳ�е�mtd*����,bin�ļ����������ʹ�õ�
int UpdategetMtdCount(){  return 0;}

/// ��ȡflash�����Ϣ�����ϲ�Ӧ��ʹ��
/// \return �ɹ�����0 ʧ�ܷ���-1
int UpdategetCaps(int mtdCnt, RawFlashCaps* caps){  return 0;}

/// ��ȡ����
/// \praram mtdname
/// \return �ɹ�����0 ʧ�ܷ��� < 0
int UpdateRead(const char *mtdname, char *buf, ulong addr, int len){  return 0;}

/// д������
/// \praram mtdname
/// \return �ɹ�����0 ʧ�ܷ��� < 0
int UpdateWrite(const char *mtdname, char*buf, ulong addr, int len){  return 0;}

/// ��������
/// \praram mtdname
/// \return �ɹ�����0 ʧ�ܷ��� < 0
int UpdateErase(const char *mtdname, ulong addr, int len){  return 0;}



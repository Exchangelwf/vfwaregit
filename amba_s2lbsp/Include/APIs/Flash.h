

#ifndef __FLASH_API_H__
#define __FLASH_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup FlashAPI API Flash
/// Flashֱ�ӷ���API����ЩӦ�ó���������ʹ��lua���Ƶģ�lua�ű�ͨ��ֱ�ӷ���flash��
///	����ӳ���ļ���
/// \note ����FlashGetSectorSize��FlashGetClientArea��ʱδʹ�á�
/// @{


/// ����Flash�ϵ�����
/// 
/// \param [out] buf �������ն������ݵĻ��塣
/// \param [in] addr �����������ƫ�ơ�
/// \param [in] cnt Ҫ�������ݵ���������
/// \retval 0  ���ɹ�
/// \retval <0  ��ʧ��
int	FlashRead(unsigned char *buf, unsigned long addr, unsigned long cnt);


/// ������д��Flash
/// 
/// \param [out] buf ��д������ݵĻ��塣
/// \param [in] addr д��������ƫ�ơ�
/// \param [in] cnt Ҫд�����ݵ���������
/// \retval 0  д�ɹ�
/// \retval <0  дʧ��
int	FlashWrite(unsigned char *buf, unsigned long addr, unsigned long cnt);


/// �õ�Flash������������С��
/// 
/// \param [out] size һ�������������ֽ���
/// \retval 0  �����ɹ�
/// \retval <0  ����ʧ��
int FlashGetSectorSize(unsigned long *size);


/// �õ�ΪӦ�ó��������������ֹ��ַ, �ⲿ��������Ӧ�ó���������
/// 
/// \param [out] from ��ʼ����ƫ��
/// \param [out] to ��ֹ����ƫ��
/// \retval 0  �����ɹ�
/// \retval <0  ����ʧ��
int FlashGetClientArea(unsigned long *from, unsigned long *to);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__FLASH_API_H__


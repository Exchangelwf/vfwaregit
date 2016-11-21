
#ifndef __CMOS_ACCESS_API_H__
#define __CMOS_ACCESS_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup CMOSAPI API CMOS
/// CMOS����API��Ӧ�ó���ʹ��CMOS��������ʷǳ�Ƶ������ʵʱ�Խϸߵ����ݡ�
/// ռ��32�ֽ�, һ��ʹ��RTCоƬ�Ĵ洢����
/// @{


/// ��CMOS�ж����ݡ�
/// 
/// \param [in] offs ��������ֽ�ƫ�ơ�
/// \param [out] pdat �������ն������ݵĻ��塣
/// \param [in] len Ҫ�������ֽ�����
/// \retval 0  �ɹ�
/// \retval <0  ʧ��
int CMOSRead(int offs, void * pdat, int len);


/// д���ݵ�CMOS��
/// 
/// \param [in] offs д�����ֽ�ƫ�ơ�
/// \param [in] pdat ��д������ݵĻ��塣
/// \param [in] len Ҫд����ֽ�����
/// \retval 0  �ɹ�
/// \retval <0  ʧ��
int CMOSWrite(int offs, void * pdat, int len);

/// @} end of group

/// ��Eeprom�ж����ݡ�
/// 
/// \param [in] offs ��������ֽ�ƫ�ơ�
/// \param [out] pdat �������ն������ݵĻ��塣
/// \param [in] len Ҫ�������ֽ�����
/// \retval 0  �ɹ�
/// \retval <0  ʧ��
int EepromRead(int offs, void * pdat, int len);
/// д���ݵ�Eeprom��
/// 
/// \param [in] offs д�����ֽ�ƫ�ơ�
/// \param [in] pdat ��д������ݵĻ��塣
/// \param [in] len Ҫд����ֽ�����
/// \retval 0  �ɹ�
/// \retval <0  ʧ��
int EepromWrite(int offs, void * pdat, int len);

#ifdef __cplusplus
}
#endif

#endif //__CMOS_ACCESS_API_H__


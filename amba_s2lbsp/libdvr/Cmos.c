#include "Cmos.h"
#include "ipc_inc.h"


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
int CMOSRead(int offs, void * pdat, int len)
{
	return 0; 
}


/// д���ݵ�CMOS��
/// 
/// \param [in] offs д�����ֽ�ƫ�ơ�
/// \param [in] pdat ��д������ݵĻ��塣
/// \param [in] len Ҫд����ֽ�����
/// \retval 0  �ɹ�
/// \retval <0  ʧ��
int CMOSWrite(int offs, void * pdat, int len)
{
	return 0; 
}

/// @} end of group

/// ��Eeprom�ж����ݡ�
/// 
/// \param [in] offs ��������ֽ�ƫ�ơ�
/// \param [out] pdat �������ն������ݵĻ��塣
/// \param [in] len Ҫ�������ֽ�����
/// \retval 0  �ɹ�
/// \retval <0  ʧ��
int EepromRead(int offs, void * pdat, int len)
{
	return 0; 
}
/// д���ݵ�Eeprom��
/// 
/// \param [in] offs д�����ֽ�ƫ�ơ�
/// \param [in] pdat ��д������ݵĻ��塣
/// \param [in] len Ҫд����ֽ�����
/// \retval 0  �ɹ�
/// \retval <0  ʧ��
int EepromWrite(int offs, void * pdat, int len)
{
	return 0; 
}



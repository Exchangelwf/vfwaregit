#include "Mouse.h"
#include "ipc_inc.h"

/// \defgroup MouseAPI API Mouse
/// ���ʹ������豸API��
///	\n ��������ͼ:
/// \code
///    ===========================
///                |
///           MouseCreate
///                |--------+
///           MouseGetData  |
///                |--------+
///           MouseDestory
///                |
///    ===========================
/// \endcode
/// @{



/// ��������豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int MouseCreate(void){  return 0;}


/// ��������豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int MouseDestory(void){  return 0;}


/// һֱ�ȵ�����һ������������¼��ŷ��أ��������������������豻�ε��򲻴��ڣ�
/// �������ش��󡣳���ʱӦ�ó�����Ҫ��ʱ���ԣ����ڲ�С��500ms����ʵ���Ȳ�ι��ܡ�
/// 
/// \param [out] pData ָ�����ʱ��ṹMOUSE_DATA��ָ�롣
/// \retval 0 ���ɹ�
/// \retval <0 ��ʧ��
int MouseGetData(MOUSE_DATA * pData){  return 0;}

/// @} end of group


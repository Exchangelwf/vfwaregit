

#ifndef __NETWORKSERVICE_SEMAPHORE_API_H__
#define __NETWORKSERVICE_SEMAPHORE_API_H__

#include "common/DVRDEF.H"
namespace NETWORKSERVICE
{

//#ifdef __cplusplus
//	extern "C" {
//#endif

	/// \defgroup SemaphoreAPI API Semaphore
	/// �ź���API����װ�˲�ͬ����ϵͳ���ź�������
	///	\n ��������ͼ:
	/// \code
	///    ================================
	///                |
	///         SemaphoreCreate
	///                |------------------+
	///    SemaphorePost,SemaphorePend    |
	///                |------------------+
	///        SemaphoreDestory
	///                |
	///    ================================
	/// \endcode
	/// @{


	/// �����ź�����
	/// 
	/// \param [out] phSemaphore �ź��������ɹ�ʱ���������ź��������
	/// \param [in] dwInitialCount �ź��������ĳ�ʼֵ��
	/// \retval 0  �����ɹ�
	/// \retval <0  ����ʧ��
	int SemaphoreCreate(VD_HANDLE *phSemaphore, uint dwInitialCount);


	/// �����ź�����
	/// 
	/// \param [in] hSemaphore �ź��������
	/// \retval 0  ���ٳɹ�
	/// \retval <0 ����ʧ��
	int SemaphoreDestory(VD_HANDLE hSemaphore);


	/// �����ź���������
	/// 
	/// \param [in] hSemaphore �ź��������
	/// \retval 0  �����ɹ�
	/// \retval <0  ����ʧ��
	int SemaphorePend(VD_HANDLE hSemaphore);


	/// �����ź���������
	/// 
	/// \param [in] hSemaphore �ź��������
	/// \retval 0  �����ɹ�
	/// \retval <0  ����ʧ��
	int SemaphorePost(VD_HANDLE hSemaphore);

	/// @} end of group

//#ifdef __cplusplus
//	}
//#endif
}
#endif //__SEMAPHORE_API_H__

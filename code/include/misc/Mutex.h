

#ifndef __NETWORKSERVICE_MUTEX_API_H__
#define __NETWORKSERVICE_MUTEX_API_H__

#include "common/DVRDEF.H"
namespace NETWORKSERVICE
{

//#ifdef __cplusplus
//	extern "C" {
//#endif

	/// \defgroup MutexAPI API Mutex
	/// ������API����װ�˲�ͬ����ϵͳ�Ļ���������
	///	\n ��������ͼ:
	/// \code
	///    ===========================
	///                |
	///           MutexCreate
	///                |------------+
	///      MutexEnter,MutexLeave  |
	///                |------------+
	///           MutexDestory
	///                |
	///    ===========================
	/// \endcode
	/// @{

	/// ����������
	enum mutex_type
	{
		/// ���ٻ�����������ٽ����Ѿ���һ���̻߳�ȡ�������̻߳��߸��߳��ٴλ�ȡʱ��
		/// �����߳�ֱ�ӱ�����
		MUTEX_FAST = 0,

		/// �ݹ黥�������Ѿ���ȡ�ٽ������߳̿��Զ�λ�ȡ�������������ڲ�Ӧ�л�ȡ����
		/// ���������Ҫ��ȫ�ͷ��ٽ�����һ��Ҫ��֤��ȡ�������ͷŴ���һ�¡�
		MUTEX_RECURSIVE,
	};


	/// ������������
	/// 
	/// \param [out] phMutex �����������ɹ�ʱ�������Ļ����������
	/// \param [in] nType �����������ͣ�ȡmutex_typeö������ֵ
	/// \retval 0  �����ɹ�
	/// \retval <0  ����ʧ��
	int MutexCreate(VD_HANDLE *phMutex, int nType);


	/// ���ٻ�������
	/// 
	/// \param [in] hMutex �����������
	/// \retval 0  ���ٳɹ�
	/// \retval <0 ����ʧ��
	int MutexDestory(VD_HANDLE hMutex);


	/// �����ٽ�����
	/// 
	/// \param [in] hMutex �����������
	/// \retval 0  �����ɹ�
	/// \retval <0  ����ʧ��
	int MutexEnter(VD_HANDLE hMutex);


	/// �뿪�ٽ�����
	/// 
	/// \param [in] hMutex �����������
	/// \retval 0  �����ɹ�
	/// \retval <0  ����ʧ��
	int MutexLeave(VD_HANDLE hMutex);

	/// @} end of group

//#ifdef __cplusplus
//	}
//#endif
}
#endif //__MUTEX_API_H__


#include "Thread.h"
#include "ipc_inc.h"

#define TSK_DEF_STACK_SIZE		16384
static int m_thread_priority = 0;

/// \defgroup ThreadAPI API Thread
/// �߳�API����װ�˲�ͬ����ϵͳ���̶߳���
///	\n ��������ͼ:
/// \code
///    ===========================
///                |
///           ThreadCreate
///                |------------+
///        ThreadShareSocket    |
///           ThreadGetID       |
///                |------------+
///           ThreadExit
///          ThreadDestory
///                |
///    ===========================
/// \endcode
/// @{


/// �����̡߳�
/// 
/// \param [out] phThread �̴߳����ɹ�ʱ���������߳̾����
/// \param [in] pStartAddress �̺߳����ĵ�ַ��
/// \param [in] pParameter �̺߳����Ĳ�����
/// \param [in] nPriority �߳����ȼ���ȡֵ0-127��ֵԽС��ʾ���ȼ�Խ�ߣ��ᱻת��
///        �ɶ�Ӧ����ϵͳ�����ȼ���
/// \param [in] dwStatckSize Ϊ�߳�ָ���Ķ�ջ��С���������0����С�ڱ����ֵ��
///        ��ʹ��ȱʡֵ��
/// \param [in] pName �߳������ַ�����
/// \retval 0  �����ɹ�
/// \retval <0  ����ʧ��
int ThreadCreate(VD_HANDLE *phThread,
				 void(*pStartAddress)(void *p),
				 void *pParameter,
				 int nPriority,
				 uint dwStatckSize,
				 const char*pName)
{
	int			policy;
	int			priority_min;
	int			priority_max;
	const char *		failure;
	int			min_stack_size = TSK_DEF_STACK_SIZE;
	pthread_attr_t		attr;
	
	
	for (;;)
	{
		if ((nPriority > 0) && (nPriority < 30))
		{
			
#ifdef HI_SDK3515_103
			policy = SCHED_RR;
#else
			if( m_thread_priority == 2 )
			{
				policy = SCHED_FIFO;
			}
			else if( m_thread_priority == 1 )
			{
				policy = SCHED_RR;
			}
			else
			{
				policy = SCHED_OTHER;
			}
#endif
		}
		else
		{
			policy = SCHED_OTHER;
		}
	
		if ((priority_min = sched_get_priority_min(policy)) == -1)
		{
			failure = "sched_get_priority_min";
			ERR_PRINT("ThreadCreate failed(%s)\n", failure);
			break;
		}
	
		if ((priority_max = sched_get_priority_max(policy)) == -1)
		{
			failure = "sched_get_priority_max";
			ERR_PRINT("ThreadCreate failed(%s)\n", failure);
			break;
		}
	
		if (nPriority > priority_max)	nPriority = priority_max;
		if (nPriority < priority_min)	nPriority = priority_min;
		
		if (policy != SCHED_OTHER)
		{
			nPriority = priority_max - nPriority;
		}
		else
		{
			nPriority = 50;
		}
	
		if( dwStatckSize < min_stack_size )
		{
			dwStatckSize = min_stack_size;
		}
	
		if (pthread_attr_init(&attr) != 0)
		{
			failure = "pthread_attr_init";
			break;
		}
	       
		if (pthread_create((pthread_t *)phThread, NULL,
		     	(void* (*)(void *))pStartAddress, pParameter) != 0)
		{
			ERR_PRINT("ThreadCreate Failed\n");
			break;
		}
	
		if (pthread_detach((pthread_t)(*phThread)) != 0)
		{
			failure = "pthread detached";
			ERR_PRINT("ThreadCreate failed(%s)\n", failure);
			break;
		}
		
		if (policy != SCHED_OTHER)
		{
			if (pthread_setschedparam((pthread_t)(*phThread),
			 policy, (struct sched_param *)(&nPriority)) != 0)
			{
				failure = "pthread detached";
				ERR_PRINT("ThreadCreate failed(%s)\n", failure);
				break;
			}
		}
	
		return 0;
	}
	
	return -1;
}


/// �����̡߳���ָ�����߳��˳�ǰ�����������̡߳�
/// 
/// \param [in] hThread ָ���������̵߳ľ����
/// \retval 0  ���ٳɹ�
/// \retval <0 ����ʧ��
int ThreadDestory(VD_HANDLE hThread)
{
	int		ret;
	
	ret = pthread_join((pthread_t)hThread, NULL);
	if (ret != 0)
	{
		errno = ret;
		ERR_PRINT("Pthread_join Failed\n");
		return -ret;
	}
	else
	{
		return 0;
	}

} 


/// ����̵߳Ĺ����׽��֡�ĳЩ����ϵͳ��ͬ���̲߳���ʹ��ͬһ���׽��֣���Ҫ����ת����
/// �������ʹ��ͬһ���׽��֣���ֱ�ӷ��ش������������
/// 
/// \param [in] hThread ָ��Ҫʹ���׽��ֵ��߳̾����
/// \param [in] nSocket �������߳��д������׽��ֵ���������
/// \return �������׽���������
int ThreadShareSocket(VD_HANDLE hThread, int nSocket)
{
	return 0;
}


/// �˳������̡߳�
void ThreadExit(void)
{
	pthread_exit(NULL);
}


/// ���ص����̵߳�ID��
/// 
/// \return �߳�ID
int ThreadGetID(void)
{
	return getpid();
}

/// @} end of group


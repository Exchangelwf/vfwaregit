#include "Thread.h"
#include "ipc_inc.h"

#define TSK_DEF_STACK_SIZE		16384
static int m_thread_priority = 0;

/// \defgroup ThreadAPI API Thread
/// 线程API，封装了不同操作系统的线程对象。
///	\n 调用流程图:
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


/// 创建线程。
/// 
/// \param [out] phThread 线程创建成功时，传出的线程句柄。
/// \param [in] pStartAddress 线程函数的地址。
/// \param [in] pParameter 线程函数的参数。
/// \param [in] nPriority 线程优先级，取值0-127，值越小表示优先级越高，会被转化
///        成对应操作系统的优先级。
/// \param [in] dwStatckSize 为线程指定的堆栈大小，如果等于0或者小于必须的值，
///        则使用缺省值。
/// \param [in] pName 线程名称字符串。
/// \retval 0  创建成功
/// \retval <0  创建失败
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


/// 销毁线程。在指定的线程退出前，阻塞调用线程。
/// 
/// \param [in] hThread 指定待销毁线程的句柄。
/// \retval 0  销毁成功
/// \retval <0 销毁失败
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


/// 获得线程的共享套结字。某些操作系统不同的线程不能使用同一个套结字，需要经过转换。
/// 如果可以使用同一个套结字，则直接返回传入的描述符。
/// 
/// \param [in] hThread 指定要使用套接字的线程句柄。
/// \param [in] nSocket 在其他线程中创建的套结字的描述符。
/// \return 共享后的套结字描述符
int ThreadShareSocket(VD_HANDLE hThread, int nSocket)
{
	return 0;
}


/// 退出调用线程。
void ThreadExit(void)
{
	pthread_exit(NULL);
}


/// 返回调用线程的ID。
/// 
/// \return 线程ID
int ThreadGetID(void)
{
	return getpid();
}

/// @} end of group


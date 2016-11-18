//#include "VDproduct.h"

#define	MODULE_DEBUG		0

//#ifdef HI_SDK3515_103
//#define	SCHED_FIFO_SUPPORT	1
//#else
//#define	SCHED_FIFO_SUPPORT	0
//#endif

#include "common/DVRDEF.H"

#include <sched.h>
#include <errno.h>
#include <unistd.h>


#define TSK_DEF_STACK_SIZE		16384
namespace NETWORKSERVICE
{

	static int m_thread_priority = 0;

	//!设置线程优先级模式
	//!0-SCHED_OTHER, 1-SCHED_RR, 2-SCHED_FIFO
	int ThreadSetPriortyMode( int mode )
	{
		if( mode < 0 || mode > 2 )
		{
			return -1;
		}
		m_thread_priority = mode;
		return 0;
	}

	int ThreadCreate
	(
	 VD_HANDLE *	phthread,
	 void		(* tsk)(void* p),
	 void *		pdat,
	 int		prio,
	 uint		stk_sz,
	 const char *	name
	)
	{
		int			policy;
		int			priority_min;
		int			priority_max;
		const char *		failure;
		int			min_stack_size = TSK_DEF_STACK_SIZE;
		pthread_attr_t		attr;
		
		
		for (;;)
		{
			if ((prio > 0) && (prio < 30))
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
		
			if (prio > priority_max)	prio = priority_max;
			if (prio < priority_min)	prio = priority_min;
			
			if (policy != SCHED_OTHER)
			{
				prio = priority_max - prio;
			}
			else
			{
				prio = 50;
			}
		
			if( stk_sz < min_stack_size )
			{
				stk_sz = min_stack_size;
			}
		
			if (pthread_attr_init(&attr) != 0)
			{
				failure = "pthread_attr_init";
				break;
			}
		       
			if (pthread_create((pthread_t *)phthread, NULL,
			     	(void* (*)(void *))tsk, pdat) != 0)
			{
				ERR_PRINT("ThreadCreate Failed\n");
				break;
			}
		
			if (pthread_detach((pthread_t)(*phthread)) != 0)
			{
				failure = "pthread detached";
				ERR_PRINT("ThreadCreate failed(%s)\n", failure);
				break;
			}
			
			if (policy != SCHED_OTHER)
			{
				if (pthread_setschedparam((pthread_t)(*phthread),
				 policy, (struct sched_param *)(&prio)) != 0)
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


	int ThreadDestory(VD_HANDLE hthread)
	{
		int		ret;
		
		ret = pthread_join((pthread_t)hthread, NULL);
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


	int ThreadShareSocket(VD_HANDLE hthread, int nSocket)
	{
	 return 0;
	} 


	void ThreadExit(void)
	{
		pthread_exit(NULL);
	} 


	int ThreadGetID(void)
	{
		return getpid();
	} 
}

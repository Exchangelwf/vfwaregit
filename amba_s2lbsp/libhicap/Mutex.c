#include "Mutex.h"

#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>  //ENOMEM
#if 0
/// \defgroup MutexAPI API Mutex
/// 互斥量API，封装了不同操作系统的互斥量对象。
///	\n 调用流程图:
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


/// 创建互斥量。
/// 
/// \param [out] phMutex 互斥量创建成功时，传出的互斥量句柄。
/// \param [in] nType 互斥量的类型，取mutex_type枚举类型值
/// \retval 0  创建成功
/// \retval <0  创建失败
int MutexCreate(VD_HANDLE *phMutex, int nType){  return 0;}


/// 销毁互斥量。
/// 
/// \param [in] hMutex 互斥量句柄。
/// \retval 0  销毁成功
/// \retval <0 销毁失败
int MutexDestory(VD_HANDLE hMutex){  return 0;}


/// 进入临界区。
/// 
/// \param [in] hMutex 互斥量句柄。
/// \retval 0  操作成功
/// \retval <0  操作失败
int MutexEnter(VD_HANDLE hMutex){  return 0;}


/// 离开临界区。
/// 
/// \param [in] hMutex 互斥量句柄。
/// \retval 0  操作成功
/// \retval <0  操作失败
int MutexLeave(VD_HANDLE hMutex){  return 0;}

/// @} end of group
#endif 


#define TS_LOCK(lock)	pthread_cleanup_push((void(*)(void*))pthread_mutex_unlock, (void*)lock );	pthread_mutex_lock( lock )
#define TS_UNLOCK(lock)	pthread_cleanup_pop( 1 )

void *ts_malloc(size_t s)
{
	void *			addr;
	
	static pthread_mutex_t	malloc_lock = PTHREAD_MUTEX_INITIALIZER;
	
	TS_LOCK(&malloc_lock);
	addr = malloc(s);
	TS_UNLOCK(&malloc_lock);
	
	return(addr);

} 
    

void ts_free(void *addr)
{
	static pthread_mutex_t	free_lock = PTHREAD_MUTEX_INITIALIZER;
	
	TS_LOCK(&free_lock);
	free(addr);
	TS_UNLOCK(&free_lock);
 
} 

int MutexCreate(VD_HANDLE *phMutex,int nType)
{
	int ret;	
	pthread_mutex_t *lock;
	pthread_mutexattr_t attr;


	lock = ts_malloc(sizeof(pthread_mutex_t));
	if (lock == NULL)
	{
		ERR_PRINT("No Mem\n");
		return -ENOMEM;
	}
	
	switch (nType) 
	{
		case MUTEX_FAST:
		{
			ret = pthread_mutex_init(lock, NULL);
			if( ret != 0 )
			{
				ts_free(lock);
				errno = ret;
				ERR_PRINT("Pthread Mutex Init: %s\n", strerror(ret));
				*phMutex = (VD_HANDLE)(NULL);
				return -ret;
			}
			break;
		}
		case MUTEX_RECURSIVE:
		{

    		if ((ret = pthread_mutexattr_init(&attr)) != 0)
    		{
     			ts_free(lock);
    			ERR_PRINT("Pthread_mutexattr_init Failed\n");	
    			return -ENOMEM;
    		}
     		
		    if ((ret = pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE_NP)) != 0)
		    {
		    	ts_free(lock);
		    	ERR_PRINT("Can't Set Recursive Type\n");
		    	*phMutex = (VD_HANDLE)(NULL);
		    	return -ret;
		    }
		    
		    if ((ret = pthread_mutex_init(lock, &attr)) != 0)
		    {
		    	ERR_PRINT("Can't Create Recursive Mutex\n");
		    	*phMutex = (VD_HANDLE)(NULL);
		    	return -ret;
		    }
		}
		break;
		default:
		{
			ret = pthread_mutex_init(lock, NULL);
			if( ret != 0 )
			{
				ts_free(lock);
				errno = ret;
				ERR_PRINT("Pthread Mutex Init: %s\n", strerror(ret));
				*phMutex = (VD_HANDLE)(NULL);
				return -ret;
			}
			break;
		}
			
	}
	
	*phMutex = (VD_HANDLE)lock;

	return 0;
}


int MutexDestory(VD_HANDLE hmutex)
{
	pthread_mutex_destroy((pthread_mutex_t *)hmutex);

	free((void*)hmutex);
	return 0;
} 


int MutexEnter(VD_HANDLE hmutex)
{
  return -pthread_mutex_lock((pthread_mutex_t *)hmutex);
} 


int MutexLeave(VD_HANDLE hmutex)
{
  return -pthread_mutex_unlock((pthread_mutex_t *)hmutex);
} 



int RWMutexCreate(VD_HANDLE* phmutex)
{
	int			ret;
	pthread_rwlock_t *lock;
	
	lock = (pthread_rwlock_t *)malloc(sizeof(pthread_rwlock_t));
	if (lock)
	{
		ret = pthread_rwlock_init(lock, NULL);
		if (ret != 0)
		{
			free(lock);
			errno	= ret;
			ERR_PRINT("Pthread Mutex Init\n");
			*phmutex	= (VD_HANDLE)(NULL);
			return -ret;
		}
	
		*phmutex = (VD_HANDLE)lock;
	
		return 0;
	}
	
	return -1;
} 


int RWMutexDestory(VD_HANDLE hmutex)
{
	pthread_rwlock_destroy((pthread_rwlock_t *)hmutex);

	free((void*)hmutex);
	return 0;
} 


int WriteMutexEnter(VD_HANDLE hmutex)
{
  return -pthread_rwlock_wrlock((pthread_rwlock_t *)hmutex);
} 

int ReadMutexEnter(VD_HANDLE hmutex)
{
  return -pthread_rwlock_rdlock((pthread_rwlock_t *)hmutex);
} 


int WriteMutexLeave(VD_HANDLE hmutex)
{
  return -pthread_rwlock_unlock((pthread_rwlock_t *)hmutex);
} 

int ReadMutexLeave(VD_HANDLE hmutex)
{
  return -pthread_rwlock_unlock((pthread_rwlock_t *)hmutex);
} 


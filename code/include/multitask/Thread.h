#ifndef _NETWORKSERVICE_THREAD_H_
#define _NETWORKSERVICE_THREAD_H_

#include "misc/Thread.h"
#include "system/Object.h"
#include "Guard.h"
#include <string>
#include "multitask/Mutex.h"
#include "multitask/Semaphore.h"

namespace NETWORKSERVICE
{
	typedef void (CObject:: * ASYNPROC)(uint wParam);

	class CThread : public CObject
	{
		friend void ThreadBody(void *pdat);
		friend class CThreadManager;

	public:
		CThread(const char*pName, int nPriority, int nMsgQueSize = 0, uint dwStackSize = 0);
		virtual ~CThread();
		VD_BOOL CreateThread();
		VD_BOOL DestroyThread(VD_BOOL bWaited = FALSE);
		VD_BOOL IsThreadOver();
		int	GetThreadID();
		virtual void ThreadProc(){} //= 0;
		int SetThreadName(const char*pName);
		int ShareSocket(int nSocket);
		void SetTimeout(uint milliSeconds);
		VD_BOOL IsTimeout();

	protected:
		volatile VD_BOOL	m_bLoop;
		volatile VD_BOOL	m_bWaitThreadExit;
	private:
		int		m_nPriority;
		uint	m_dwStackSize;
		VD_HANDLE	m_hThread;
		int		m_ID;
		char	m_Name[32];
		CThread* m_pPrev;		//上一个线程
		CThread* m_pNext;		//下一个线程
		CSemaphore m_cSemaphore;	// 该信号量用来防止同一个对象的线程同时被创建多次；
		uint	m_expectedTime;	//预计执行结束时的时间，0表示不预计
		CSemaphore m_desSemaphore;
		CMutex m_Mutex;
		volatile bool m_bDesSemForbidPend; //!指示m_desSemaphore禁止再pend了
	};

	class Threadlet;

	class PooledThread : public CThread
	{
		friend class CThreadManager;
		friend class Threadlet;

	public:
		PooledThread();
		~PooledThread();
		void ThreadProc();

	private:
		CObject *m_obj;
		ASYNPROC m_asynFunc;
		uint m_param;
		CSemaphore m_semaphore;
		PooledThread* m_nextPooled;
		Threadlet* m_caller;
	};

	class Threadlet
	{
		friend class PooledThread;

	public:
		Threadlet();
		~Threadlet();
		VD_BOOL run(VD_PCSTR name, CObject * obj, ASYNPROC asynProc, uint param, uint timeout);
		VD_BOOL isRunning();

	private:
		PooledThread* m_thread;
		static CMutex m_mutex;	///< 保证Threadlet和PooledThread的关联关系
	};

	class CThreadManager : public CObject
	{
		friend class CThread;
		friend class PooledThread;
		friend class Threadlet;
		friend void ThreadBody(void *pdat);

	public:
		PATTERN_SINGLETON_DECLARE(CThreadManager);
		CThreadManager();
		~CThreadManager();

		void RegisterMainThread(int id);
		VD_BOOL HasThread(CThread *pThread);
		void DumpThreads();
		VD_BOOL GetTimeOutThread(std::string  &threadName);

	private:
		VD_BOOL AddThread(CThread *pThread);
		VD_BOOL RemoveThread(CThread *pThread);
		PooledThread * GetPooledThread();
		void ReleasePooledThread(PooledThread* pThread);

	private:
		CThread* m_pHead;
		PooledThread* m_headPooled;
		CMutex m_Mutex;
		int m_mainThreadID;
	};

#define g_ThreadManager (*CThreadManager::instance())
}
#endif //_THREAD_H_



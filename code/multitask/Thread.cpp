
#include <string.h>
#include <sys/prctl.h>
#include "multitask/Timer.h"
#include "multitask/Thread.h"
#ifdef SUPPORT_DEBUG
#include "nipdebug.h"
#endif

namespace NETWORKSERVICE
{
	void ThreadBody(void *pdat)
	{
		CThread *pThread = (CThread *)pdat;

	/************************************************************************
		线程通用回调函数：
		1、获得线程ID；
		2、执行特定对象的线程回调函数；
		3、当特定对象的线程回调函数执行完毕，把该线程从线程管理队列中删除；
		4、释放同步信号量；
		5、把线程句柄置为空；
	************************************************************************/
		pThread->m_ID = ThreadGetID();
#ifdef SUPPORT_DEBUG
		init_nipdebug(0, 0, 0, 0);
#endif
		g_ThreadManager.AddThread(pThread);

		 prctl(PR_SET_NAME, (unsigned long)pThread->m_Name, 0, 0, 0);
		//trace("ThreadBody Enter name = %s, id = %d, handle = 0x%x, stack = 0x%0x\n", pThread->m_Name, pThread->m_ID, (int)pThread->m_hThread, &pThread);
		pThread->ThreadProc();
		//printf("ThreadBody Leave name = %s, id = %d, handle = 0x%x\n", pThread->m_Name, pThread->m_ID, (int)pThread->m_hThread);
		fflush(stdout);
		pThread->m_cSemaphore.Post();
		g_ThreadManager.RemoveThread(pThread);

		pThread->m_Mutex.Enter();
		if (pThread->m_bWaitThreadExit)
		{
			pThread->m_desSemaphore.Post();
		}
		else
		{
			pThread->m_bDesSemForbidPend = true;
		}
		pThread->m_Mutex.Leave();

		ThreadExit();
	}

	CThread::CThread(const char*pName, int nPriority, int nMsgQueSize /* = 0 */, uint dwStackSize /* = 0 */) 
	: m_cSemaphore(1), m_desSemaphore(0)
	,m_Mutex(MUTEX_RECURSIVE)
	,m_bDesSemForbidPend(false)
	{
	/************************************************************************
		线程对象初始化：
		1、信号量初始化；
		2、优先级变量；
		3、堆栈大小变量；
		4、线程名；
		5、线程句柄；
		6、消息队列对象指针；
	************************************************************************/
		m_nPriority = nPriority;
		m_dwStackSize = dwStackSize;
		if(pName)
		{
			strcpy(m_Name, pName);
		}
		else
		{
			strcpy(m_Name, "Nonamed");
		}
		m_hThread = NULL;
		m_ID = -1;
		m_bLoop = FALSE;
		if(nMsgQueSize)
		{
		}
		else
		{
		}
		m_expectedTime = 0;
		m_bWaitThreadExit = FALSE;
	}

	CThread::~CThread()
	{
		if (m_hThread)
		{
			DestroyThread();
		}
		
	}

	int CThread::SetThreadName(const char*pName)
	{
		if(pName)
		{
			strcpy(m_Name, pName);
		}
		else
		{
			strcpy(m_Name, "Nonamed");
		}

		return 0;
	}

	VD_BOOL CThread::CreateThread()
	{
	/************************************************************************
		创建线程：
		1、设置信号量和线程回调函数中的循环标志；
		2、调用创建线程API；
		3、如果线程创建成功，把该线程对象加入到线程管理队列，并返回TURE，
		   否则返回FALSE；
	************************************************************************/

		m_cSemaphore.Pend();

		m_bLoop = TRUE;
		return  ThreadCreate(&m_hThread, ThreadBody, this, m_nPriority, m_dwStackSize, m_Name) == 0 ? TRUE : FALSE;
	}

	VD_BOOL CThread::DestroyThread(VD_BOOL bWaited)
	{
	/************************************************************************
		结束线程：
		1、直接把用于控制主循环的变量置为FALSE，让它退出主循环；
	************************************************************************/
		if (!IsThreadOver())
		{
			m_Mutex.Enter();
			if ( m_ID != ThreadGetID() )
			{
				m_bWaitThreadExit = bWaited;
			}
			m_Mutex.Leave();
			m_bLoop = FALSE;

			// 不是自己关自己的时候才采用阻塞式
			if (m_ID != ThreadGetID())
			{
				m_Mutex.Enter();
				bool bDesSemForbidPend = m_bDesSemForbidPend;
				m_Mutex.Leave();
				if (m_bWaitThreadExit && !bDesSemForbidPend )
				{
					m_desSemaphore.Pend();
				}
			}
			
		}
		return TRUE;
	}

	VD_BOOL CThread::IsThreadOver()
	{
	/************************************************************************
		判断线程回调函数是否已经停止：
		1、如果已经停止，则返回TRUE，否则返回FALSE；
	************************************************************************/
		return !m_bLoop;
		
	}

	int CThread::GetThreadID()
	{
		return m_ID;
	}

	int CThread::ShareSocket(int nSocket)
	{
		return ThreadShareSocket(m_hThread, nSocket);
	}



	void CThread::SetTimeout(uint milliSeconds)
	{
		if(milliSeconds == 0) // 清空预期时间
		{
			m_expectedTime = 0;
		}
		else
		{
			m_expectedTime = SystemGetMSCount() + milliSeconds;

			if(m_expectedTime < milliSeconds) // 计数溢出， 暂时不设置预期时时间
			{
				m_expectedTime = 0;
			}
		}
	}

	VD_BOOL CThread::IsTimeout()
	{
		return (m_expectedTime != 0 && m_expectedTime < SystemGetMSCount());
	}

	////////////////////////////////////////////////////////////////////////////////////
	// PooledThread
	////////////////////////////////////////////////////////////////////////////////////
	PooledThread::PooledThread() : CThread("Pooled", TP_DEFAULT)
	{
	}

	PooledThread::~PooledThread()
	{
	}

	void PooledThread::ThreadProc()
	{
		while(m_bLoop)
		{
			m_semaphore.Pend();
			(m_obj->*m_asynFunc)(m_param); 
			SetThreadName("Pooled");
			SetTimeout(0);
			Threadlet::m_mutex.Enter();
			if(m_caller)
			{
				m_caller->m_thread = NULL;
				m_caller = NULL;
			}
			Threadlet::m_mutex.Leave();
			g_ThreadManager.ReleasePooledThread(this);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////
	// Threadlet
	////////////////////////////////////////////////////////////////////////////////////
	CMutex Threadlet::m_mutex;

	Threadlet::Threadlet()
	{
		m_thread = NULL;
	}

	Threadlet::~Threadlet()
	{
		CGuard guard(m_mutex);

		if(m_thread)
		{
			m_thread->m_caller = NULL;
		}
	}

	VD_BOOL Threadlet::run(VD_PCSTR name, CObject * obj, ASYNPROC asynProc, uint param, uint timeout)
	{
		CGuard guard(m_mutex);

		if(m_thread) //正在运行
		{
			return FALSE;
		}

		m_thread = g_ThreadManager.GetPooledThread();
		m_thread->SetThreadName(name);
		m_thread->SetTimeout(timeout);
		m_thread->m_obj = obj;
		m_thread->m_asynFunc = asynProc;
		m_thread->m_param = param;
		m_thread->m_caller = this;
		m_thread->m_semaphore.Post();
		return TRUE;
	}

	VD_BOOL Threadlet::isRunning()
	{
		return (m_thread != NULL);
	}

	////////////////////////////////////////////////////////////////////////////////////
	// CThreadManager
	////////////////////////////////////////////////////////////////////////////////////

	PATTERN_SINGLETON_IMPLEMENT(CThreadManager);

	CThreadManager::CThreadManager() : m_Mutex(MUTEX_RECURSIVE)
	{
		printf("CThreadManager::CThreadManager()>>>>>>>>>\n");
		m_pHead = NULL;
		m_headPooled = NULL;
	}

	CThreadManager::~CThreadManager()
	{
	}

	void CThreadManager::RegisterMainThread(int id)
	{
		m_mainThreadID = id;
	}

	VD_BOOL CThreadManager::AddThread(CThread *pThread)
	{
		CGuard guard(m_Mutex);

		pThread->m_pPrev = NULL;
		pThread->m_pNext = m_pHead;

		if(m_pHead)
		{
			m_pHead->m_pPrev = pThread;
		}

		m_pHead = pThread;

		return TRUE;
	}

	VD_BOOL CThreadManager::RemoveThread(CThread *pThread)
	{
		CGuard guard(m_Mutex);

		if(pThread->m_pPrev == NULL)
		{
			m_pHead = pThread->m_pNext;
			if(m_pHead)
			{
				m_pHead->m_pPrev = NULL;
			}

			return TRUE;
		}

		pThread->m_pPrev->m_pNext = pThread->m_pNext;
		if(pThread->m_pNext != NULL)
			pThread->m_pNext->m_pPrev = pThread->m_pPrev;

		return TRUE;
	}

	VD_BOOL CThreadManager::HasThread(CThread *pThread)
	{
		CGuard guard(m_Mutex);
		CThread * p = m_pHead;

		while (p)
		{
			if (p == pThread)
			{
				return TRUE;
			}
			p = p->m_pNext;
		}

		return FALSE;
	}

#ifdef FUNC_THREAD_DETECT
	VD_BOOL CThreadManager::GetTimeOutThread(std::string &threadName)
	{
		CThread *p = m_pHead;
		VD_BOOL ret = FALSE;
		threadName.clear();

		while(p)
		{
			if (p->IsTimeout())
			{
				threadName += p->m_Name;		
				ret = TRUE;
			}
			p = p->m_pNext;
		}
		
		return ret;
	}
#endif

	PooledThread * CThreadManager::GetPooledThread()
	{
		CGuard guard(m_Mutex);
		PooledThread * p = m_headPooled;

		if(p) 
		{
			m_headPooled = p->m_nextPooled;
		}
		else 
		{
			p = new PooledThread();
			p->CreateThread();
		}

		return p;
	}

	void CThreadManager::ReleasePooledThread(PooledThread* pThread)
	{
		CGuard guard(m_Mutex);
		assert(pThread);

		pThread->m_nextPooled = m_headPooled;
		m_headPooled = pThread;
	}

	void CThreadManager::DumpThreads()
	{
		CGuard guard(m_Mutex);
		CThread * p = m_pHead;

		printf("Threads:\n");
		printf("               Name            PID  Prior State\n");
		printf("_______________________________________________________\n");
		printf("%24s   %8d  %3d  %s\n", "Main", m_mainThreadID, TP_DEFAULT, "Normal");
		while (p)
		{
			printf("%24s   %8d  %3d  %s\n", p->m_Name, p->m_ID, p->m_nPriority, p->IsTimeout() ? "Timeout" : "Normal");
			p = p->m_pNext;
		}
	}
}

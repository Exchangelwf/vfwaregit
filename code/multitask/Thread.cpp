
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
		�߳�ͨ�ûص�������
		1������߳�ID��
		2��ִ���ض�������̻߳ص�������
		3�����ض�������̻߳ص�����ִ����ϣ��Ѹ��̴߳��̹߳��������ɾ����
		4���ͷ�ͬ���ź�����
		5�����߳̾����Ϊ�գ�
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
		�̶߳����ʼ����
		1���ź�����ʼ����
		2�����ȼ�������
		3����ջ��С������
		4���߳�����
		5���߳̾����
		6����Ϣ���ж���ָ�룻
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
		�����̣߳�
		1�������ź������̻߳ص������е�ѭ����־��
		2�����ô����߳�API��
		3������̴߳����ɹ����Ѹ��̶߳�����뵽�̹߳�����У�������TURE��
		   ���򷵻�FALSE��
	************************************************************************/

		m_cSemaphore.Pend();

		m_bLoop = TRUE;
		return  ThreadCreate(&m_hThread, ThreadBody, this, m_nPriority, m_dwStackSize, m_Name) == 0 ? TRUE : FALSE;
	}

	VD_BOOL CThread::DestroyThread(VD_BOOL bWaited)
	{
	/************************************************************************
		�����̣߳�
		1��ֱ�Ӱ����ڿ�����ѭ���ı�����ΪFALSE�������˳���ѭ����
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

			// �����Լ����Լ���ʱ��Ų�������ʽ
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
		�ж��̻߳ص������Ƿ��Ѿ�ֹͣ��
		1������Ѿ�ֹͣ���򷵻�TRUE�����򷵻�FALSE��
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
		if(milliSeconds == 0) // ���Ԥ��ʱ��
		{
			m_expectedTime = 0;
		}
		else
		{
			m_expectedTime = SystemGetMSCount() + milliSeconds;

			if(m_expectedTime < milliSeconds) // ��������� ��ʱ������Ԥ��ʱʱ��
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

		if(m_thread) //��������
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

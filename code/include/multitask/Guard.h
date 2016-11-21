

#ifndef __NETWORKSERVICE_GUARD_H__
#define __NETWORKSERVICE_GUARD_H__

#include "Mutex.h"
namespace NETWORKSERVICE
{

	class CMutex;
	class CGuard
	{
	public:
		inline CGuard(CMutex& Mutex)
			:m_Mutex(Mutex)
		{
			m_Mutex.Enter();
		};
		
		inline ~CGuard()
		{
			m_Mutex.Leave();
		};
	protected:
	private:
		CMutex &m_Mutex;
	};
}
#endif //__GUARD_H__
//
//

#include <linux/watchdog.h>
#include "WatchDog.h"
#include "ipc_inc.h"

#define DEV_WATCHDOG "/dev/watchdog"

/// \defgroup WatchDogAPI API Watch Dog
/// ���Ź�API����Ӳ�������ʵ��ϵͳ�Ĺ��ϴ���.
/// @{

/// �������Ź��豸
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�

static int AmBaWdtFd = 0;
static int flags;
int WatchdogCreate(void)
{//return 0;
	int timeout = 10;
	if (0 >= (AmBaWdtFd = open(DEV_WATCHDOG, O_RDWR))){
		perror("open wdt");
		_printd("/dev/watchdog open Error\n");
		return -1;
	}
	flags = WDIOS_ENABLECARD;		
	ioctl(AmBaWdtFd, WDIOC_SETOPTIONS, &flags);
	ioctl(AmBaWdtFd, WDIOC_SETTIMEOUT, &timeout);
	_printd("Watchdog card enabled.\n");		
	return 0;
}

/// ���ٿ��Ź��豸
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int WatchdogDestory(void)
{  //return 0;
	flags = WDIOS_DISABLECARD;		
	ioctl(AmBaWdtFd, WDIOC_SETOPTIONS, &flags);		
	_printd("Watchdog card disabled.\n");		
	close(AmBaWdtFd);
	return 0;
}

/// 
int WatchdogClear(void)
{
	return 0;
}

/// 
int WatchdogRead(void* pData, uint nBytes){  return 0;}

///
int WatchdogWrite(void* pdata, uint nBytes)
{//return 0;
	int dummy;	
	ioctl(AmBaWdtFd, WDIOC_KEEPALIVE, &dummy);
	return 0;
}

/// @} end of group



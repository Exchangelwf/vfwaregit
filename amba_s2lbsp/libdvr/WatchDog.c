#include <linux/watchdog.h>
#include "WatchDog.h"
#include "ipc_inc.h"

#define DEV_WATCHDOG "/dev/watchdog"

/// \defgroup WatchDogAPI API Watch Dog
/// 看门狗API，软硬件配合来实现系统的故障处理.
/// @{

/// 创建看门狗设备
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功

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

/// 销毁看门狗设备
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
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



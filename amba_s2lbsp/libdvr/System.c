
#include <ctype.h>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>  //ENOMEM

#include <unistd.h>
#include <linux/rtc.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/reboot.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#include "System.h"
#include "ipc_inc.h"
#include "Net.h"


#define RTC_DEV "/dev/rtc0"

/// 得到当前系统时间。这个函数可能调用的很频繁, 所有调用时应该是从内存中取得
/// 时间,而不是直接从硬件, 在定时器里更新内存中的值.
/// 
/// \param [out] pTime 指向系统时间结构SYSTEM_TIME的指针。
/// \retval 0  成功
/// \retval <0  失败
int SystemGetCurrentTime(SYSTEM_TIME * pTime)
{

	time_t			t = time(NULL);
	struct tm		time;
	struct timeval		tv;
	struct timezone	tz;

	gettimeofday(&tv, &tz);
	localtime_r(&t, &time);

	pTime->second	= time.tm_sec; 			
	pTime->minute	= time.tm_min;			
	pTime->hour		= time.tm_hour;			
	pTime->wday		= time.tm_wday;			
	pTime->day		= time.tm_mday;			
	pTime->month	= time.tm_mon + 1;		
	pTime->year		= time.tm_year + 1900;	
	if(time.tm_year < 70){
		pTime->year = 2014;
	}

	return 0;
}


/// 设置当前系统时间。
/// 
/// \param [in] pTime 指向系统时间结构SYSTEM_TIME的指针。
/// \retval 0  成功
/// \retval <0  失败
int SystemSetCurrentTime(SYSTEM_TIME * pTime)
{
	_printd("set time %4d-%02d-%02d %02d:%02d:%02d\n", pTime->year,
		pTime->month, pTime->day, pTime->hour, pTime->minute, pTime->second);
	time_t			t;
	struct tm		newtime;
	struct timezone	tz;
	struct timeval	tv;
	newtime.tm_year	= pTime->year - 1900;
	newtime.tm_mon	= pTime->month - 1;		
	newtime.tm_mday	= pTime->day;			
	newtime.tm_hour	= pTime->hour;
	newtime.tm_min	= pTime->minute;		
	newtime.tm_sec	= pTime->second;

	gettimeofday(NULL, &tz);
	t = mktime(&newtime) + tz.tz_minuteswest * 60L;
	newtime.tm_isdst	= 0;	
	t = mktime(&newtime);
	tv.tv_sec = (long)t;
	settimeofday(&tv,NULL);

	int 		rtcfd 	= -1;
	if(0 >= (rtcfd = open(RTC_DEV, O_RDWR))){
		perror("open rtc");
		return -1;
	}
	if(0 > ioctl(rtcfd, RTC_SET_TIME, &newtime)){
		perror("RTC_SET_TIME");
		close(rtcfd);
		return -1;
	}
	close(rtcfd);
	
	return 0;
}


/// 得到系统的毫秒计数。两次调用之间的间隔时间就是得到的计数的差值。
/// 
/// \param 无
/// \return 当前计数。
/// \note 一般情况下，49.7天后得到计数会溢出，使用到的地方要做溢出后处理！
uint SystemGetMSCount(void)
{
	uint t;
	t = (times(NULL) & 0xffffffff) * 10;
	return	t;
}

/// 得到系统的毫秒计数。两次调用之间的间隔时间就是得到的计数的差值。
/// 
/// \param 无
/// \return 当前计数。
/// \note 一般情况下，497天后得到计数会溢出，使用到的地方要做溢出后处理！
static uint64 g_uiTickOffs = 0;	//tick计数校正
#ifndef _SC_CLK_TCK
#define sysconf
#define _SC_CLK_TCK 100
#endif
uint64 SystemGetMSCount64(void)
{
	uint64 tick = (uint64)times(NULL); // 产生从0开始的tick计数
	if(tick == (uint64)-1)
	{
		tick = (uint64)-errno;
	}
	if(g_uiTickOffs == 0)
	{
		g_uiTickOffs = 0 - tick;
	}

	return (tick + g_uiTickOffs) * (1000 / sysconf(_SC_CLK_TCK));

}



/// 得到系统的微秒计数, 此函数主要用于模块性能的测试。溢出的更快。
/// 
/// \param 无
/// \return 当前计数。
uint SystemGetUSCount(void)
{
	struct timeval		tv;
	struct timezone	tz;
	int			ret;

	gettimeofday(&tv, &tz);
	ret = tv.tv_sec * 1000 * 1000 + tv.tv_usec;

	return ret;
} 


/// 得到CPU占用率。
/// 
/// \param 无
/// \return CPU占用百分比。
int SystemGetCPUUsage(void)
{
	FILE *file = NULL;
	char		str[128] ={0};
	int		u_ticks = 0;
	int		n_ticks = 0;
	int		s_ticks = 0;
	int		i_ticks = 0;
	int		t_ticks = 0;
	int		ret = 0;

	file = fopen("/proc/stat", "r");
	if (file == NULL)
	{
		return 20;
	}

	if (fgets(str, 128, file))
	{
		if (fscanf(file, "cpu%*d %d %d %d %d\n",
	    	 &u_ticks, &n_ticks, &s_ticks, &i_ticks) != 4)
		{
			ret = 20;	
		}
		else
		{
		      printf("SystemGetCPUUsage: %d ,%d ,%d ,%d\r\n", u_ticks, s_ticks, i_ticks, n_ticks);
			  
			t_ticks = (u_ticks + s_ticks + i_ticks + n_ticks);
			uint64 iTmp = ((uint64)i_ticks) * 100 /t_ticks;
			ret = 100 - (uint)iTmp;
		}
	}

	fclose(file);
	return ret;
} 


/// 得到系统特性。
/// 
/// \param [out] pCaps 指向系统特性结构SYSTEM_CAPS的指针。
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int SystemGetCaps(SYSTEM_CAPS * pCaps)
{
	unsigned long		total;
	unsigned long		used;
	unsigned long		mfree;
	unsigned long		shared;
	unsigned long		buffers;
	unsigned long		cached;

	FILE			*fp;
	char			buf[256];
	//unsigned int		needs_conversion = 1;

	fp = fopen("/proc/meminfo", "r");
	if (fp == NULL)
	{
		return 0;
	}	
	if (fscanf(fp, "MemTotal: %lu %s\n", &total, buf) != 2)
	{
		fgets(buf, sizeof(buf), fp);		
		fscanf(fp, "Mem: %lu %lu %lu %lu %lu %lu",
	    	 &total, &used, &mfree, &shared, &buffers, &cached);
		used	/= 1024;
		mfree	/= 1024;
		shared	/= 1024;
		buffers	/= 1024;
		cached	/= 1024;
		total	/= 1024;
		
	}
	else
	{
		//needs_conversion = 0;
		fscanf(fp, "MemFree: %lu %s\n", &mfree, buf);
		if (fscanf(fp, "MemShared: %lu %s\n", &shared, buf) != 2)
		{
			shared = 0;
		}
		fscanf(fp, "Buffers: %lu %s\n", &buffers, buf);
		fscanf(fp, "Cached: %lu %s\n", &cached, buf);
		used = total - mfree;
	}
	fclose(fp);
	printf("MemoryTotal = %ld MemoryLeft = %ld", total, mfree);
	pCaps->MemoryTotal	= total;
	pCaps->MemoryLeft	= mfree;
	return 0;
}


/// 
int SystemGetCapsEx(SYSTEM_CAPS_EX * pCaps)
{
	memset(pCaps, 0,sizeof(SYSTEM_CAPS_EX));
	pCaps->HasAudioBoard = 1;
	return 0;
}


/// 挂起当前线程，时间到后再唤醒。
/// 
/// \param [in] dwMilliSeconds 挂起的时间。
void SystemSleep(uint dwMilliSeconds)
{
	usleep(dwMilliSeconds * 1000);
} 


/// 
void SystemUSleep(uint dwMicroSeconds)
{
	usleep(dwMicroSeconds);
} 


/// 让系统蜂鸣器发出单音调的声音，是同步模式，时间到才返回。
/// 
/// \param [in] dwFrequence 音调频率。
/// \param [in] dwDuration 持续的时间。
void SystemBeep(uint dwFrequence, uint dwDuration ){ }

 
// 静音接口
// 当isSilence为1时，表示系统需要静音，关闭声音的IO口；
// 当isSilence为0时，表示系统需要取消静音，打开声音的IO口；
void SystemSilence(int isSilence){ }
 

/// 系统复位。
void SystemReboot(void)
{
	LIBDVR_PRINT("Sys Will Reboot\n");
	sync();
	sleep(1);
#if 1
	system("killall ipcam");
	sleep(11);
	system("reboot -f");
#else
	int iRet = reboot(RB_AUTOBOOT);
	if (0 == iRet){
		LIBDVR_PRINT("reboot func return true");
	}else{
		ERR_PRINT("reboot func return false");
	}
#endif
	return;
}

/// 系统关闭。
void SystemShutdown(void)
{
	LIBDVR_PRINT("Sys Will Halt\n");
	reboot(RB_POWER_OFF);
}

/// 应用程序收到升级包后，让系统解压。是同步调用模式，直到解压完成才返回。
/// 
/// \param [in] pData 数据包在内存中的指针。
/// \param [in] dwLength 数据长度。
/// \param [out] pProgress 当前进度百分比，函数调用过程时会被定时更新，应用程序
///        不应该修改这个值。
/// \retval 0  成功
/// \retval <0  失败
/// \note 不同的系统数据打包的格式不同，对应的完成数据打包和解压的的程序也不同。
int SystemUpgrade(uchar* pData, uint dwLength, uint *pProgress){  return 0;}

/// 系统日志初始化。系统日志用来记录应用程序运行时所有的标准输出。
/// 
/// \retval 0  成功
/// \retval <0  失败
int SystemLogInit(void){  return 0;}


/// 记录系统日志， 同时将内容写到到标准输出缓冲。
/// 
/// \retval 0  成功
/// \retval <0  失败
int SystemLogWrite(char * format, ...){  return 0;}


/// 获取系统序列号
int SystemGetSerialNumber(char *pData, int len)
{
	if (pData == NULL ){
        return 0;
	}
	  
 	NetGetMAC("eth0", pData, len);
	return 0;
}


/// 获取系统硬件版本
uint SystemGetHWVersion(void){  return 0;}


/// 获取设备名称设备类型是一个字符串，长度小于8
///字符串根据设备不同分别为："IPCF4x5"，"IPCA6"，"IPCA8"，"IPCA7"，"IPVM"，"IPSD"
int SystemGetDeviceType(char* type)
{
    if(NULL == type)
    {
        ERR_PRINT("SystemGetDeviceType ERROR!\n");
        return 0;
    }
	return 0;
} 

/// 获取版本号, 格式例如: r9508:2.15:12:13:14:16, 最大长度128字节
int SystemGetVersion(char *version)
{
	if(NULL == version)
	{
		ERR_PRINT("SystemGetVersion ERROR!\n");
		return 0;
	}
	strcpy(version,"3.2");
	return 0;
}

/// 获取UBOOT过程中检测到的config_resume 状态，方便上层决定是否需要恢复默认设置
CONFIG_RESUME_OPR SystemGetConfigResume(void)
{
	return CONFIG_RESUME_NO;
}


enum ui_language_t SystemGetUILanguage(void){  return 0;}
int SystemSetUILanguage(enum ui_language_t lang){  return 0;}
		     
int SystemGetVGAPixelCaps(int *pCaps)
{    
    if(NULL == pCaps)
    {
        printf("%s - param err!\r\n", __func__);
        return -1;
    }

    return -1;
}
int SystemGetVGAPixel(){  return 0;}
int SystemSetVGAPixel(int vgapixel){  return 0;}
int SystemSetVGAResolution(int type){  return 0;}

int SystemAuth(){  return 0;}




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

/// �õ���ǰϵͳʱ�䡣����������ܵ��õĺ�Ƶ��, ���е���ʱӦ���Ǵ��ڴ���ȡ��
/// ʱ��,������ֱ�Ӵ�Ӳ��, �ڶ�ʱ��������ڴ��е�ֵ.
/// 
/// \param [out] pTime ָ��ϵͳʱ��ṹSYSTEM_TIME��ָ�롣
/// \retval 0  �ɹ�
/// \retval <0  ʧ��
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


/// ���õ�ǰϵͳʱ�䡣
/// 
/// \param [in] pTime ָ��ϵͳʱ��ṹSYSTEM_TIME��ָ�롣
/// \retval 0  �ɹ�
/// \retval <0  ʧ��
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


/// �õ�ϵͳ�ĺ�����������ε���֮��ļ��ʱ����ǵõ��ļ����Ĳ�ֵ��
/// 
/// \param ��
/// \return ��ǰ������
/// \note һ������£�49.7���õ������������ʹ�õ��ĵط�Ҫ���������
uint SystemGetMSCount(void)
{
	uint t;
	t = (times(NULL) & 0xffffffff) * 10;
	return	t;
}

/// �õ�ϵͳ�ĺ�����������ε���֮��ļ��ʱ����ǵõ��ļ����Ĳ�ֵ��
/// 
/// \param ��
/// \return ��ǰ������
/// \note һ������£�497���õ������������ʹ�õ��ĵط�Ҫ���������
static uint64 g_uiTickOffs = 0;	//tick����У��
#ifndef _SC_CLK_TCK
#define sysconf
#define _SC_CLK_TCK 100
#endif
uint64 SystemGetMSCount64(void)
{
	uint64 tick = (uint64)times(NULL); // ������0��ʼ��tick����
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



/// �õ�ϵͳ��΢�����, �˺�����Ҫ����ģ�����ܵĲ��ԡ�����ĸ��졣
/// 
/// \param ��
/// \return ��ǰ������
uint SystemGetUSCount(void)
{
	struct timeval		tv;
	struct timezone	tz;
	int			ret;

	gettimeofday(&tv, &tz);
	ret = tv.tv_sec * 1000 * 1000 + tv.tv_usec;

	return ret;
} 


/// �õ�CPUռ���ʡ�
/// 
/// \param ��
/// \return CPUռ�ðٷֱȡ�
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


/// �õ�ϵͳ���ԡ�
/// 
/// \param [out] pCaps ָ��ϵͳ���ԽṹSYSTEM_CAPS��ָ�롣
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
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


/// ����ǰ�̣߳�ʱ�䵽���ٻ��ѡ�
/// 
/// \param [in] dwMilliSeconds �����ʱ�䡣
void SystemSleep(uint dwMilliSeconds)
{
	usleep(dwMilliSeconds * 1000);
} 


/// 
void SystemUSleep(uint dwMicroSeconds)
{
	usleep(dwMicroSeconds);
} 


/// ��ϵͳ��������������������������ͬ��ģʽ��ʱ�䵽�ŷ��ء�
/// 
/// \param [in] dwFrequence ����Ƶ�ʡ�
/// \param [in] dwDuration ������ʱ�䡣
void SystemBeep(uint dwFrequence, uint dwDuration ){ }

 
// �����ӿ�
// ��isSilenceΪ1ʱ����ʾϵͳ��Ҫ�������ر�������IO�ڣ�
// ��isSilenceΪ0ʱ����ʾϵͳ��Ҫȡ����������������IO�ڣ�
void SystemSilence(int isSilence){ }
 

/// ϵͳ��λ��
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

/// ϵͳ�رա�
void SystemShutdown(void)
{
	LIBDVR_PRINT("Sys Will Halt\n");
	reboot(RB_POWER_OFF);
}

/// Ӧ�ó����յ�����������ϵͳ��ѹ����ͬ������ģʽ��ֱ����ѹ��ɲŷ��ء�
/// 
/// \param [in] pData ���ݰ����ڴ��е�ָ�롣
/// \param [in] dwLength ���ݳ��ȡ�
/// \param [out] pProgress ��ǰ���Ȱٷֱȣ��������ù���ʱ�ᱻ��ʱ���£�Ӧ�ó���
///        ��Ӧ���޸����ֵ��
/// \retval 0  �ɹ�
/// \retval <0  ʧ��
/// \note ��ͬ��ϵͳ���ݴ���ĸ�ʽ��ͬ����Ӧ��������ݴ���ͽ�ѹ�ĵĳ���Ҳ��ͬ��
int SystemUpgrade(uchar* pData, uint dwLength, uint *pProgress){  return 0;}

/// ϵͳ��־��ʼ����ϵͳ��־������¼Ӧ�ó�������ʱ���еı�׼�����
/// 
/// \retval 0  �ɹ�
/// \retval <0  ʧ��
int SystemLogInit(void){  return 0;}


/// ��¼ϵͳ��־�� ͬʱ������д������׼������塣
/// 
/// \retval 0  �ɹ�
/// \retval <0  ʧ��
int SystemLogWrite(char * format, ...){  return 0;}


/// ��ȡϵͳ���к�
int SystemGetSerialNumber(char *pData, int len)
{
	if (pData == NULL ){
        return 0;
	}
	  
 	NetGetMAC("eth0", pData, len);
	return 0;
}


/// ��ȡϵͳӲ���汾
uint SystemGetHWVersion(void){  return 0;}


/// ��ȡ�豸�����豸������һ���ַ���������С��8
///�ַ��������豸��ͬ�ֱ�Ϊ��"IPCF4x5"��"IPCA6"��"IPCA8"��"IPCA7"��"IPVM"��"IPSD"
int SystemGetDeviceType(char* type)
{
    if(NULL == type)
    {
        ERR_PRINT("SystemGetDeviceType ERROR!\n");
        return 0;
    }
	return 0;
} 

/// ��ȡ�汾��, ��ʽ����: r9508:2.15:12:13:14:16, ��󳤶�128�ֽ�
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

/// ��ȡUBOOT�����м�⵽��config_resume ״̬�������ϲ�����Ƿ���Ҫ�ָ�Ĭ������
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



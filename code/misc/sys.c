
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
#include <sys/reboot.h>
#include <sys/time.h>

#include "misc/System.h"
//#include "Cmdpopen.h"
//#include "../update/norflash.h"
/*
*彩煌出现IPC重启失败，使用popen代替system解决
*/
namespace NETWORKSERVICE
{

	int cmdpopen(char *cmd)
	{
	    char result_buf[1024]={0}, command[1024]={0};
	    int rc = 0; // 用于接收命令返回值
	    FILE *fp = NULL;

		if (NULL == cmd)
		{
			printf("\r Parameter is invalid.\n");
			return -1;
		}
	    /*将要执行的命令写入buf*/
	    snprintf(command, sizeof(command), cmd);
	    /*执行预先设定的命令，并读出该命令的标准输出*/
	    fp = popen(command, "r");
	    if(NULL == fp)
	    {
	        printf("\r %s popen fail\n", __func__);
	        return 1;
	    }
	   
	    while(fgets(result_buf, sizeof(result_buf), fp) != NULL)
	    {
			usleep(10);
	    }
	  
	    /*等待命令执行完毕并关闭管道及文件指针*/
	    rc = pclose(fp);
	   
	    if(-1 == rc)
	    {
			perror("command close file fail");
	        return 1;
	    }

	    return 0;
	}

	int SystemGetCurrentTime(WJA_SYSTEM_TIME *ptime)
	{
		time_t			t = time(NULL);
		struct tm		time;
		struct timeval		tv;
		struct timezone	tz;

		gettimeofday(&tv, &tz);
		localtime_r(&t, &time);

		ptime->second	= time.tm_sec; 			
		ptime->minute	= time.tm_min;			
		ptime->hour	= time.tm_hour;			
		ptime->wday	= time.tm_wday;			
		ptime->day	= time.tm_mday;			
		ptime->month	= time.tm_mon + 1;		
		ptime->year	= time.tm_year + 1900;		

		return 0;
	} 

	int SystemSetCurrentTime(WJA_SYSTEM_TIME *ptime)
	{
		int			moffset[] = { 0, 31, 59, 90, 120, 151,
					      181, 212, 243, 273, 304, 334	};
		time_t		t;
		struct tm	newtime;
		struct timezone	tz;
		newtime.tm_sec	= ptime->second;		
		newtime.tm_min	= ptime->minute;		
		newtime.tm_hour	= ptime->hour;			
		newtime.tm_wday	= ptime->wday;			
		newtime.tm_mday	= ptime->day;			
		newtime.tm_mon	= ptime->month - 1;		
		newtime.tm_year	= ptime->year - 1900;
		newtime.tm_yday	= moffset[newtime.tm_mon] + newtime.tm_mday - 1;

		gettimeofday(NULL, &tz);
		t = mktime(&newtime) + tz.tz_minuteswest * 60L;
		newtime.tm_isdst	= 0;	
		t = mktime(&newtime);	
		if (t < 0)
		{
			ERR_PRINT("Format Error\n");
		}
		else
		{
		
			/// 先更新rtc
			#if 0
			if (SystemTimeToRtcTimeSet(&newtime) < 0)
			{
				ERR_PRINT ("SystemSetCurrentTime Failed\n");
	//			return -1;
			}
			
			if (stime(&t) < 0)
			{
				ERR_PRINT("Cannot Set Date");
			}
			#endif
		}

		return 0;
	}

	uint SystemGetMSCount(void)
	{
		uint t;
		//t = (times(NULL) & 0xffffffff) * 10;
		return	t;
	}

	static uint64 g_uiTickOffs = 0;	//tick计数校正
#ifndef _SC_CLK_TCK
#define sysconf
#define _SC_CLK_TCK 100
#endif

#if 0
	//需要独立开发一个驱动eeeeee
	uint64 SystemGetMSCount64(void)
	{
		uint64 tick = 0;//(uint64)times(NULL); // 产生从0开始的tick计数
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
#else
	uint64 SystemGetMSCount64(void)
	{
		struct timeval		tv;
		struct timezone	tz;
		int			ret;

		gettimeofday(&tv, &tz);
		ret = tv.tv_sec * 1000  + tv.tv_usec/1000;

		return ret;
	} 
#endif
	uint SystemGetUSCount(void)
	{
		struct timeval		tv;
		struct timezone	tz;
		int			ret;

		gettimeofday(&tv, &tz);
		ret = tv.tv_sec * 1000 * 1000 + tv.tv_usec;

		return ret;
	} 

	//CPU
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

	//MEMORY

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
		unsigned int		needs_conversion = 1;

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
			needs_conversion = 0;
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
		printf("MemoryTotal = %d MemoryLeft = %d", total, mfree);
		pCaps->MemoryTotal	= total;
		pCaps->MemoryLeft	= mfree;
		return 0;
	}

	void SystemSleep(uint ms)
	{
		usleep(ms * 1000);
	} 

	void SystemUSleep(uint ms)
	{
		usleep(ms);
	} 

	//void Remote_SystemReboot();
	//void Remote_SystemShutdown();
	//void Remote_ApplicationRestart();
	//void Remote_ApplicationExit();
#ifdef HI_SDK3515_103
	void SystemReboot(void)
	{
		LIBDVR_PRINT("Sys Will Reboot\n");
		reboot(RB_AUTOBOOT);
		return;
	}
#else
	void SystemReboot(void)
	{
		LIBDVR_PRINT("Sys Will Reboot\n");

	    char any[32];
	    //!此后，要关闭喂狗功能, 不允许再喂狗, 让看门狗复位
	    //!解决从片重启不动问题，多片级联功能时使用
#if 0
		WatchdogSetTimeOut(1);		// set watchdog feed 1 second, wyt, 2011.1.17
	    WatchdogWrite(any, 4);
	    int  heartbeat = 5;//设置20秒
	     set_watchdog_heartbeat(heartbeat);
	    extern int WatchdogDiableWrite(uint enable);
	   WatchdogDiableWrite(1);
#endif
		sync();
		int iRet = reboot(0x1234567);
		if (0 == iRet){
			LIBDVR_PRINT("reboot func return true");
		}else{
			ERR_PRINT("reboot func return false");
		}
	    //sleep(2);
		return;
	}
#endif
}
#if 0

void SystemShutdown(void)
{
	LIBDVR_PRINT("Sys Will Halt\n");
	//Remote_SystemShutdown();
#if defined(SDK_3516)
#else
	extern	int HiSplitDisable(void);
	HiSplitDisable();
#endif
	cmdpopen("poweroff");
}


//回复默认功能
#include <misc/System.h>
#define BOOTARGS_DEV "/proc/cmdline"
static int fd_cmd=-1;
static unsigned char bootargs_str[1024];
static unsigned char configresume_str[20];
#define CONFIG_MATCH_STR "configresume"
#define CONFIG_RESUME_VALUE_OFFSET 13
static int bootargs_init(void)
{
	int ret=0;
	fd_cmd=open(BOOTARGS_DEV, O_RDWR);
	if (fd_cmd < 0)
	{
		ERR_PRINT("Open /proc/cmdline device Failed\n");
		return -1;
	}

	ret = read(fd_cmd, bootargs_str, 1024);
	if(ret<=0)
	{
		printf("\n read bootargs fail....\n");
		return -1;
	}
	
	return 0;
}

static int bootargs_match(void)
{
	char *pAddr=strstr(bootargs_str,CONFIG_MATCH_STR);
	if(NULL==pAddr)
	{
		printf("\nFind no configresume string.....\n");
		return -1;
	}
	strcpy(configresume_str,pAddr);
	return 0;
}

CONFIG_RESUME_OPR SystemGetConfigResume(void)
{
#ifdef USE_3516_ISP
	unsigned char *env=0, *nxt = 0;
	unsigned char *env_data = bootargs_str;
	int oldval = -1;
	int ret=0;

	ret = bootargs_init();
	if(0!=ret)
	{
		printf("\nSystemGetConfigResume fail,return CONFIG_RESUME_NO....\n");
		return CONFIG_RESUME_NO;
	}
	ret=bootargs_match();
	if(0!=ret)
	{
		printf("\nSystemGetConfigResume fail,return CONFIG_RESUME_NO....\n");
		return CONFIG_RESUME_NO;
	}

	if('0'==configresume_str[CONFIG_RESUME_VALUE_OFFSET])
	{
		return CONFIG_RESUME_NO;
	}
	else if('1'==configresume_str[CONFIG_RESUME_VALUE_OFFSET])
	{
		return CONFIG_RESUME_YES;
	}
#else	
	return CONFIG_RESUME_INVALID;
#endif
	
}


void ApplicationRestart(void)
{
	//Remote_ApplicationRestart();

	sync();
	reboot(RB_AUTOBOOT);
}

void ApplicationExit(void)
{
	//Remote_ApplicationExit();

	sync();
	_exit(0);
}



//!从片控制操作
#include "pciv_msg.h"
#include "chip.h"
#include "Slave.h"
/*
 *  软重启，4个接口都需要主从通讯
 *  主片：依次通知每个从片再自身操作
 *  从片：收到主片命令，立即执行（不需要sleep（3））
 */

int Remote_SystemInform(PCIV_MSGTYPE_E msg_type, PCIV_DEVTYPE_E dev_type, const char* pstr)
{
	HI_S32 s32Ret;
	PCIV_MSGHEAD_S stMsg;

	CHIP_MANAGER_T* pChip = Chip_GetIns();

	if( pChip->LocalChipID != 0
		|| pChip->m_ChipNum <= 1 )
	{
		//!不是主片不通知从片, 或没有从片
		return -1;
	}

	LIBDVR_PRINT("%s\n", pstr);

	int i = 0;
	//!没有通道概念，要全部从片全初始化一次
	for( i = 0; i < PCIV_MAX_CHIPNUM ; i++ )
	{
		if( pChip->m_RemoteChipID[i] == -1 )
		{
			continue;
		}

		stMsg.u32Target = pChip->m_RemoteChipID[i];
		stMsg.enMsgType = msg_type;
		stMsg.enDevType = dev_type;
		stMsg.u32MsgLen = 0;
		pciv_port_usercmd_lock((char*)__func__);
		printf("u32Target:%d\n", stMsg.u32Target);
		s32Ret = PCIV_SendMsg(stMsg.u32Target, PCIV_MSGPORT_USERCMD, &stMsg);
		if(s32Ret != 0)
		{
			ERR_PRINT(" Error !!! \n");
		}
		//Wait for the echo message. stMsg will be changed
		//s32Ret = PCIV_ReadMsg(stMsg.u32Target, PCIV_MSGPORT_USERCMD, &stMsg);
		pciv_port_usercmd_unlock((char*)__func__);
		printf("2-u32Target:%d\n", stMsg.u32Target);
	}
	return 0;
}

void Remote_SystemReboot()
{
	Remote_SystemInform(PCIV_MSGTYPE_System, PCIV_DEVTYPE_SystemReboot, "Remote_SystemReboot");
}

void Remote_SystemShutdown()
{
	Remote_SystemInform(PCIV_MSGTYPE_System, PCIV_DEVTYPE_SystemShutdown, "Remote_SystemShutdown");
}

void Remote_ApplicationRestart()
{
	Remote_SystemInform(PCIV_MSGTYPE_System, PCIV_DEVTYPE_ApplicationRestart, "Remote_ApplicationRestart");
}

void Remote_ApplicationExit()
{
	Remote_SystemInform(PCIV_MSGTYPE_System, PCIV_DEVTYPE_ApplicationExit, "Remote_ApplicationExit");
}

int Slave_System(SLAVE_T* pThis, PCIV_MSGHEAD_S* pstMsg)
{
	LIBDVR_PRINT("Slave_System\n");
	sleep(1);
	switch (pstMsg->enDevType)
	{
	case PCIV_DEVTYPE_SystemReboot:
		SystemReboot();
		break;
	case PCIV_DEVTYPE_SystemShutdown:
		SystemShutdown();
			break;
	case PCIV_DEVTYPE_ApplicationRestart:
		ApplicationRestart();
			break;
	case PCIV_DEVTYPE_ApplicationExit:
		ApplicationExit();
			break;
	default:
		break;
	}
	return -2;
}
#endif



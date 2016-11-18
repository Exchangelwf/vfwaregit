#include <iostream>
#include <cstring>
#include <unistd.h>
#include <stdio.h>

#include "networkapi/wjalink_api.h"
#include "networkapi/wjalink_callback.h"

int wja_RecordDownLoadTimeSet(int chn,WJA_RECORDTIME time,unsigned int clientid)
{
	printf("wja_RecordDownLoadTimeSet carryout\n");
	return 0;
}
int wja_RecordDownLoadFileOpen(int chn, char recordname[80],unsigned int clientid)
{
	printf("wja_RecordDownLoadFileOpen carryout\n");
	return 0;
}

int wja_RecordSendStart(int chn)
{
	printf("wja_RecordSendStart carryout\n");
	return 0;
}

int wja_RecordSendStop(int chn)
{
	printf("wja_RecordSendStart carryout\n");
	return 0;
}
int wja_RecordSendStopAll()
{
	printf("wja_RecordSendStopAll carryout\n");
	return 0;
}

int wja_VideoFileSearch_CallBack(WJA_NET_FILEINFO *SearchFileInfo, WJA_NET_FILEINFO **ptr, int *size)
{
	printf("wja_VideoFileSearch_CallBack carryout\n");
	return 0;
}

int wja_PlayBackSendStreamOpen(int chn, char recordname[80],WJA_VideoDataFormat* videoinfo)
{
	printf("wja_PlayBackSendStreamOpen carryout\n");
	return 0;
}

int wja_PlayBackPointTime(int chn, WJA_RECORDTIME time,WJA_VideoDataFormat* videoinfo)
{
	printf("wja_PlayBackPointTime carryout\n");
	return 0;
}

int wja_PlayBackSendStart(int chn)
{
	printf("wja_PlayBackSendStart carryout\n");
	return 0;
}
int wja_PlayBackSendStop(int chn)
{
	printf("wja_PlayBackSendStop carryout\n");
	return 0;
}

int wja_PlayBackSendStopAll()
{
	printf("wja_PlayBackSendStopAll carryout\n");
	return 0;
}

int wja_PlayBackSendControl(int chn,int cmd,int value)
{
	printf("wja_PlayBackSendControl carryout\n");
	return 0;
}

int wja_RealTimePlayOpen(int chn,int stream,WJA_VideoDataFormat* videoinfo)
{
	printf("wja_RealTimePlayOpen carryout\n");
	return 0;
}

int wja_RealTimePlayClose(int chn,int stream)
{
	printf("wja_RealTimePlayClose carryout\n");
	return 0;
}

int wja_RealTimePlayCloseAll()
{
	printf("wja_RealTimePlayCloseAll carryout\n");
	return 0;
}

int wja_GetDevStatus(int chn,WJA_DevStatus* Status)
{
	printf("wja_GetDevStatus carryout\n");
	return 0;
}
int wja_GetWifiInfo(WJA_WifiInfo* WifiInfo)
{
	printf("wja_GetWifiInfo carryout\n");
	return 0;
}

int wja_SetWifiInfo(WJA_WifiInfo WifiInfo)
{
	printf("wja_SetWifiInfo carryout\n");
	return 0;
}

bool wja_FirmwareUpload(WJA_CHIP_TYPE type)
{
	printf("wja_FirmwareUpload carryout\n");
	return false;
}
int wja_SnapStart(int chn)
{
	printf("wja_SnapStart carryout\n");
	return 0;
}

int InitNetworkParam(Wja_InitNetworkParam* parm)
{
	if(parm ==NULL)
	{
		printf("InitNetworkParam parm is NULL\n");
		return -1;
	}
	strcpy(parm->remotehost,"172.16.5.41");
	strcpy(parm->dir,"/tmp");
	strcpy(parm->filename,"uploadfile");
	strcpy(parm->uuid,"12456789-21456987");
	parm->remoteport=5555;
	parm->channelnum=1;
	return 0;
}


int main()
{
	Wja_InitNetworkParam para;
	int cnt=0;
	
	std::cout<<"main begining"<<std::endl;
	InitNetworkParam(&para);
	wja_netservice_init(para);
	while(cnt < 15)
	{
		sleep(1);
	}
	wja_netservice_destroy();
	std::cout<<"main end"<<std::endl;
	return 0;
}

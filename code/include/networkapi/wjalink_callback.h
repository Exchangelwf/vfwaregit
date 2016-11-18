#ifndef __WJALINK_CALLBACK_H__
#define __WJALINK_CALLBACK_H__

typedef struct SYSTEM_TIME_WJA{
	int  year;///< �ꡣ   
	int  month;///< �£�January = 1, February = 2, and so on.	
	int  day;///< �ա�	 
	int  wday;///< ���ڣ�Sunday = 0, Monday = 1, and so on	 
	int  hour;///< ʱ��   
	int  minute;///< �֡�	
	int  second;///< �롣	
	int  isdst;///< ����ʱ��ʶ��   
}WJA_SYSTEM_TIME;

typedef struct RecordTime_WJA{
	WJA_SYSTEM_TIME StartTime;
	WJA_SYSTEM_TIME EndTime;
}WJA_RECORDTIME;

typedef enum{
 	 ALL_TYPE,                //ȫ��
	 SYSTEM_TYPE,            //ϵͳ����
	 CONIFG_TYPE,            //���ò���
	 DATAMANAGER_TYPE,      //���ݹ���
	 ALARM_TYPE,             //�����¼�
     RECORD_OPERRATION_TYPE, //¼�����
     USERMANAGER_TYPE,      //�û�����
	 CLEAR_TYPE,             //��־���
     File_operate              //�ļ�����
 }WJA_Log_Type;

typedef struct tagQueryLog_WJA
{
	WJA_Log_Type  ucLogType;/*��ѯ����־����*/
		/* 0 ȫ��1 ϵͳ����2 ���ò���3 ���ݹ���
		4 �����¼�5 ¼�����6 �û�����7 ��־���8 �ļ�����*/
	WJA_SYSTEM_TIME  stStartTime;/*��ʼʱ��*/
	WJA_SYSTEM_TIME  stEndTime;/*����ʱ��*/
	unsigned char  ucRes[16];
}WJA_NET_QUERY_SYSYTEM_LOG;

typedef struct tagNetLogInfo_WJA
{
    WJA_Log_Type      usType;         /*��־����*/
	WJA_SYSTEM_TIME  stTime;			/* ��־ʱ�� */
	unsigned char     ucContext[64]; 		/*����*/
	unsigned char  	 ucRes1[4];
}WJA_NET_LOG_INFO;

int wja_GetLogInfo_CallBack(WJA_NET_QUERY_SYSYTEM_LOG* LogInfo,WJA_NET_LOG_INFO** ptr ,int *LogSize);

/* Functionality:
   set Record  DownLoadTime,the first step of Record  DownLoad,sum twp steps
 Parameters:
 	chn:for IPC,the value is zero
 		for nvr .the value is nvr channel for IPC
 	time:start time and End Time
 Return:
   1 have Record on storage , 0 no Record for DownLoad
 */
int wja_RecordDownLoadTimeSet(int chn,WJA_RECORDTIME time,unsigned int clientid);

int wja_RecordDownLoadFileOpen(int chn, char recordname[80],unsigned int clientid);

/* Functionality:
   start DownLoad Record,the second step of Record  DownLoad,sum twp steps
 Parameters:
 	chn:for IPC,the value is zero
 		for nvr .the value is nvr channel for IPC
 Return:
   0 on success, otherwise fail.
 */
int wja_RecordSendStart(int chn);

/* Functionality:
   stop DownLoad Record
 Parameters:
 	chn:for IPC,the value is zero
 		for nvr .the value is nvr channel for IPC
 Return:
   0 on success, otherwise fail.
 */
int wja_RecordSendStop(int chn);

/* Functionality:
   stop all DownLoad Record of device
 Return:
   0 on success, otherwise fail.
 */
int wja_RecordSendStopAll();


typedef struct 
{
	unsigned char ucFileType; /* 0 ¼��  1 ͼƬ */
	unsigned char ucSubType; /* ������0 ȫ��1 �ⲿ����2 ���죨������Ƶ�ڵ���Ƶ��ʧ��̬��⣩3 ���б���������1��2�����⻹�����쳣������Ӳ�����ȣ�4 �ֶ�¼�� 5��ʱ¼�� 6����¼��*/
	unsigned char ucChannel; /* ¼��ͨ�� */
	unsigned char ucRes;
	unsigned char ucFileName[80];/* �ļ�����ʽ:ͨ����_��ʼʱ��(yyyy-MM-dd-HH-mm-ss)_����ʱ��(yyyy-MM-dd-HH-mm-ss)_�̺�_������_�غ�_�ļ����_�ļ�����(00¼�� 01ͼƬ)_�ļ�����*/  
	WJA_SYSTEM_TIME stStartTime;
	WJA_SYSTEM_TIME stEndTime;
	unsigned int	FileLen;
	unsigned char ucRes1[16];
}WJA_NET_FILEINFO;

/* Functionality:
   get VideoFile list infoation between stStartTime and stEndTime,use for video PlayBack or Search
 Parameters:
 	SearchFileInfo:include chn ,stStartTime stEndTime and so on
 	ptr:			output file list for 	VideoFile search,include VideoFile name
 	size			output for VideoFile count
 Return:
   0 on success, otherwise fail.
 */
int wja_VideoFileSearch_CallBack(WJA_NET_FILEINFO *SearchFileInfo, WJA_NET_FILEINFO **ptr, int *size);

typedef struct _DEV_VideoDataFormat_WJA
{
	unsigned int codec;			//���뷽ʽ
	unsigned int bitrate;        //������, bps
	unsigned short width;			//ͼ����
	unsigned short height;			//ͼ��߶�
	unsigned char framerate;		//֡��, fps
	unsigned char colorDepth;		//should be 24 bits
	unsigned char frameInterval;   //I֡���
	unsigned char reserve;

}WJA_VideoDataFormat;

/* Functionality:
   open VideoFile  for PlayBack,and also get stream parameter
 Parameters:
 	recordname:PlayBack video file
 	chn:			input
 	videoinfo			output for VideoFile stream parameter
 Return:
   0 on success, otherwise fail.
 */
int wja_PlayBackSendStreamOpen(int chn, char recordname[80],WJA_VideoDataFormat* videoinfo);
int wja_PlayBackPointTime(int chn, WJA_RECORDTIME time,WJA_VideoDataFormat* videoinfo);

/* Functionality:
   after open VideoFile  for PlayBack,then inform the IPC to send record data
 Parameters:
 	chn:			input
 Return:
   0 on success, otherwise fail.
 */
int wja_PlayBackSendStart(int chn);



/* Functionality:
   after  IPC send record data,manual control PlayBack speed 
 Parameters:
 	chn:			input
 Return:
   0 on success, otherwise fail.
 */
int wja_PlayBackSendControl(int chn,int cmd,int value);

/* Functionality:
   after open VideoFile  for PlayBack,need close PlayBack
 Parameters:
 	chn:			input
 Return:
   0 on success, otherwise fail.
 */
int wja_PlayBackSendStop(int chn);

/* Functionality:
   stop all PlayBack for device
 Return:
   0 on success, otherwise fail.
 */
int wja_PlayBackSendStopAll();

/* Functionality:
  start RealTime video Play
 Parameters:
 	chn:			input
 	stream:	main stream or sub stream
 	videoinfo: output for VideoFile stream parameter
 Return:
   0 on success, otherwise fail.
 */
int wja_RealTimePlayOpen(int chn,int stream,WJA_VideoDataFormat* videoinfo);


/* Functionality:
  stop RealTime video Play
 Parameters:
 	chn:			input
 	stream:	main stream or sub stream
 Return:
   0 on success, otherwise fail.
 */
int wja_RealTimePlayClose(int chn,int stream);

/* Functionality:
  stop all RealTime video Play
 Parameters:
 Return:
   0 on success, otherwise fail.
 */
int wja_RealTimePlayCloseAll();


int wja_SnapStart(int chn);

typedef struct{
	int iDevStatus;    //�豸����״̬
	int iDiskStatus;    //Ӳ��״̬
	int iDiskCap;       //Ӳ������
	int iDiskReCap;     //Ӳ��ʣ������
	int iDev4gStatus;   //4G״̬
	int iDevWifiStatus; //WIFI״̬
	char verdata[64];   //�汾��Ϣ
	WJA_SYSTEM_TIME StartTime; //����ʱ��
}WJA_DevStatus;
int wja_GetDevStatus(int chn,WJA_DevStatus* Status);

typedef struct{
	int iEnable;
	char szSsid[32];
	char szPswd[32];
}WJA_WifiInfo;

int wja_GetWifiInfo(WJA_WifiInfo* WifiInfo);
int wja_SetWifiInfo(WJA_WifiInfo WifiInfo);

typedef enum{
	MAINCHIP=0,
}WJA_CHIP_TYPE;

bool wja_FirmwareUpload(WJA_CHIP_TYPE type);

#endif

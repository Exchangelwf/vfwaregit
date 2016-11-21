#ifndef __WJALINK_CALLBACK_H__
#define __WJALINK_CALLBACK_H__

typedef struct SYSTEM_TIME_WJA{
	int  year;///< Äê¡£   
	int  month;///< ÔÂ£¬January = 1, February = 2, and so on.	
	int  day;///< ÈÕ¡£	 
	int  wday;///< ÐÇÆÚ£¬Sunday = 0, Monday = 1, and so on	 
	int  hour;///< Ê±¡£   
	int  minute;///< ·Ö¡£	
	int  second;///< Ãë¡£	
	int  isdst;///< ÏÄÁîÊ±±êÊ¶¡£   
}WJA_SYSTEM_TIME;

typedef struct RecordTime_WJA{
	WJA_SYSTEM_TIME StartTime;
	WJA_SYSTEM_TIME EndTime;
}WJA_RECORDTIME;

typedef enum{
 	 ALL_TYPE,                //È«²¿
	 SYSTEM_TYPE,            //ÏµÍ³²Ù×÷
	 CONIFG_TYPE,            //ÅäÖÃ²Ù×÷
	 DATAMANAGER_TYPE,      //Êý¾Ý¹ÜÀí
	 ALARM_TYPE,             //±¨¾¯ÊÂ¼þ
     RECORD_OPERRATION_TYPE, //Â¼Ïñ²Ù×÷
     USERMANAGER_TYPE,      //ÓÃ»§¹ÜÀí
	 CLEAR_TYPE,             //ÈÕÖ¾Çå³ý
     File_operate              //ÎÄ¼þ²Ù×÷
 }WJA_Log_Type;

typedef struct tagQueryLog_WJA
{
	WJA_Log_Type  ucLogType;/*²éÑ¯µÄÈÕÖ¾ÀàÐÍ*/
		/* 0 È«²¿1 ÏµÍ³²Ù×÷2 ÅäÖÃ²Ù×÷3 Êý¾Ý¹ÜÀí
		4 ±¨¾¯ÊÂ¼þ5 Â¼Ïñ²Ù×÷6 ÓÃ»§¹ÜÀí7 ÈÕÖ¾Çå³ý8 ÎÄ¼þ²Ù×÷*/
	WJA_SYSTEM_TIME  stStartTime;/*ÆðÊ¼Ê±¼ä*/
	WJA_SYSTEM_TIME  stEndTime;/*½áÊøÊ±¼ä*/
	unsigned char  ucRes[16];
}WJA_NET_QUERY_SYSYTEM_LOG;

typedef struct tagNetLogInfo_WJA
{
    WJA_Log_Type      usType;         /*ÈÕÖ¾ÀàÐÍ*/
	WJA_SYSTEM_TIME  stTime;			/* ÈÕÖ¾Ê±¼ä */
	unsigned char     ucContext[64]; 		/*ÄÚÈÝ*/
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
	unsigned char ucFileType; /* 0 Â¼Ïñ  1 Í¼Æ¬ */
	unsigned char ucSubType; /* ×ÓÀàÐÍ0 È«²¿1 Íâ²¿±¨¾¯2 ¶¯¼ì£¨°üÀ¨ÊÓÆµÕÚµ²ÊÓÆµ¶ªÊ§¶¯Ì¬¼ì²â£©3 ËùÓÐ±¨¾¯£¨°üÀ¨1£¬2ÀàÐÍÍâ»¹°üÀ¨Òì³£±¨¾¯£¬Ó²ÅÌÂúµÈ£©4 ÊÖ¶¯Â¼Ïñ 5¶¨Ê±Â¼Ïñ 6¿¨ºÅÂ¼Ïñ*/
	unsigned char ucChannel; /* Â¼ÏñÍ¨µÀ */
	unsigned char ucRes;
	unsigned char ucFileName[80];/* ÎÄ¼þÃû¸ñÊ½:Í¨µÀºÅ_¿ªÊ¼Ê±¼ä(yyyy-MM-dd-HH-mm-ss)_½áÊøÊ±¼ä(yyyy-MM-dd-HH-mm-ss)_ÅÌºÅ_·ÖÇøºÅ_´ØºÅ_ÎÄ¼þÐòºÅ_ÎÄ¼þÀàÐÍ(00Â¼Ïñ 01Í¼Æ¬)_ÎÄ¼þ³¤¶È*/  
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
	unsigned int codec;			//ï¿½ï¿½ï¿½ë·½Ê½
	unsigned int bitrate;        //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½, bps
	unsigned short width;			//Í¼ï¿½ï¿½ï¿½ï¿½
	unsigned short height;			//Í¼ï¿½ï¿½ß¶ï¿½
	unsigned char framerate;		//Ö¡ï¿½ï¿½, fps
	unsigned char colorDepth;		//should be 24 bits
	unsigned char frameInterval;   //IÖ¡ï¿½ï¿½ï¿½
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
	int iDevStatus;    //Éè±¸ÔËÐÐ×´Ì¬
	int iDiskStatus;    //Ó²ÅÌ×´Ì¬
	int iDiskCap;       //Ó²ÅÌÈÝÁ¿
	int iDiskReCap;     //Ó²ÅÌÊ£ÓàÈÝÁ¿
	int iDev4gStatus;   //4G×´Ì¬
	int iDevWifiStatus; //WIFI×´Ì¬
	char verdata[64];   //°æ±¾ÐÅÏ¢
	WJA_SYSTEM_TIME StartTime; //¿ª»úÊ±¼ä
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

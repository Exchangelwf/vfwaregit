//    文 件 名： NetCore.h
//    描    述: 用于实现网络视频实时传输

//                               删除原有的网络自适应算法
//
#ifndef __NETWORKSERVICE_NET_CORE_H__
#define __NETWORKSERVICE_NET_CORE_H__

#ifndef WIN32    
#include <time.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include "common/BaseTypedef.h"
#include "common/DVRDEF.H"

#ifndef IO_MAX
#define    IO_MAX        (1024*50)//16384        /* Max request size        */
#endif /* IO_MAX */

//-----------------------------------------------------------------------------
// Forward declare and defines
//-----------------------------------------------------------------------------
namespace NETWORKSERVICE
{


	struct conn;
	/*
	* Net Core context
	*/
	struct NetCoreContext 
	{
	    time_t          start_time;        /* Start time */
	    int             nactive;        /* # of connections now */
	    unsigned int    nrequests;        /* Requests made */
	    unsigned int    kb_in, kb_out;        /* IN/OUT traffic counters */
	    struct conn    *connections;        /* List of connections */

	    int        port;            /* Listening port */
	};

#define MSG_FLAGS_MORE         0x01
#define PKT_FULL_FRAME          0x01

#define  NET_PRE_MALLOC_CNT 200
	//-----------------------------------------------------------------------------
	// type declare
	//-----------------------------------------------------------------------------
	typedef struct tagMsgHdr {
	    void *buf;
	    size_t buf_len;
	    void (*callback)(void*);
	    unsigned short msg_flags;
	    void *context;
	    void *msgAddr;
	    struct timeval timestamp;    /* 时间戳 */
	    struct tagMsgHdr *next;
	    int malloc_flags;  //0 :system malloc  1: my malloc_main
	    int msgCmd;
	    int chn;
	    int iFrameFlag ;
	} msghdr_t;

	//----------------------------------------------------------------
	//Malloc Unit
	//-----------------------------------------------------------------------

#define msgbufsize sizeof(msghdr_t)

	typedef struct tagPreMallocNode{
	    
	       void *pMallocNode;
	    tagPreMallocNode *pNext;
	}PreMallocNode_t;


	typedef struct tagMsgHdrMalloc{
	    msghdr_t msg;
	    char buf[72];//72+msgbufsize+8 = 128，128*8=1024
	    tagMsgHdrMalloc *pre;
	    tagMsgHdrMalloc *next;
	}msghdrmalloc_t;

#define msgbufunit sizeof(msghdrmalloc_t)-8

	typedef enum{
	    malloc_msg = 1,
	    malloc_malloc,
	}enum_malloc_method;

	/*
	 * Connection descriptor
	 */
	typedef void (*shttpd_watch_t)(struct conn *);

	/*
	 * Unified socket address
	 */
	struct usa {
	    socklen_t len;
	    union {
	        struct sockaddr    sa;
	        struct sockaddr_in sin;
	    } u;
	};

	typedef struct NetCoreIO {
	    int (*init)    (struct conn *);
	    void (*fini)   (struct conn *);
	    void (*process)(struct conn *);
	    int port;
	    int udp_port;
	    void *context;
	} NetCoreIO_t;

	/*
	 * I/O buffer
	 */
	struct io {
	    char    buf[IO_MAX*2];    /* Buffer        */
	    int    done;                /* IO finished        */
	    size_t    head;            /* Bytes read        */
	    size_t    tail;            /* Bytes written    *//*实际上代码中是已经处理的数据的便宜量*/
	};
#define    IO_SPACELEN(io)        (sizeof((io)->buf) - (io)->head - 1)
#define    IO_DATALEN(io)        ((io)->head - (io)->tail)

	enum 
	{
	    FLAG_FINISHED        =1,     /* Connection to be closed    */
	    FLAG_SOCK_READABLE    =2,
	    FLAG_SOCK_WRITABLE    =4,
	    FLAG_SSLACCEPTED    =8,    /* SSL_accept() succeeded    */
	    FLAG_ALWAYS_READY    =16    ,/* Local channel always ready for IO */
	    FLAG_IO_READY       = (FLAG_SOCK_WRITABLE | FLAG_SOCK_READABLE)
	};

	typedef struct tagNetQueue
	{
	    msghdr_t         *head;        /* 发送队列的头部 */
	    msghdr_t         *tail;        /* 发送队列的尾部 */
	    struct timeval startTime;    /* 队列的开始时间 */
	    struct timeval endTime;      /* 队列的结束时间 */
	    unsigned long  totalSize;   /* 队列中的数据总长度 */
	    int SocketSendFailFlag;     //!<这个队列的数据曾经发送失败, 
	                                //!所以再次发送的时候要过一段时间才会再
	                                //!次发送, 具体等待的时间和dsp的当前速率
	                                //!有关系
	    int SocketSendFailCnt;
	    struct timeval SocetSendSleepTime;//!<记录上一次数据发送失败的时间点
	    int            CurrentDspRate;
	} NetQueue_t;

	typedef struct tagNode
	{
	    void *pMallocAddress;
	    tagMsgHdrMalloc         *head;        /* buf head*/
	    tagMsgHdrMalloc         *cur;        /* free head*/
	    tagMsgHdrMalloc         *tail;        /* buf tail*/
	    int nodeuse;                        /*已经使用的节点个数*/
	    int buf_size;                       /*已经申请的内存大小*/
	} Node_t;

	//网络运行状态监控
	struct tagNetSTtateMonitor 
	{
	    int ch_no;                  //打开的通道数
	    int ch_no_func[N_SYS_CH+1]; //打开的通道执行的功能 0 表示该通道没打开 1 监控 ，2 下载 , 3回放 add by yang_shukui
	};
	enum monitorFunc
	{
	    NOUSE=0, //0 表示该通道没打开
	    MONITOR, //1 监控 
	    DOWNLOAD,//2 下载 
	    PLAYBACK,//3 回放 
	    PREVIEW  //4 预览
	     
	};
	struct conn 
	{
	    struct conn    *next;                	/* Connections chain        */
	    struct NetCoreContext *ctx;        		/* Context this conn belongs to */
	    struct usa    sa;                    	/* Remote socket address    */
	    time_t        birth;                	/* Creation time        */
	    int           sock;                		/* Remote socket        */
	    shttpd_watch_t    watch;            	/* IO readiness callback    */
	    void        *watch_data;        		/* Callback data        */
	    struct io    remote;                	/* Remote IO buffer        */
	    
	    NetQueue_t  TransferQueue;  			/*保持以前兼容，只用他的长度了*/ //!好像不再使用了
	    NetQueue_t  DataTransferQueue[2];        /* 传输队列            */
	    Node_t      NodeFree;

	    void (*fini)(struct conn *);    		/* 用于执行连接断开时的清除工作  */
	    int (*init)(struct conn *);        		/* 用于执行连接建立时的初始话工作*/
	    unsigned int    flags;            		/* Flags            */
	    void *context;
	    void *hMutex;
	    
	    unsigned long ulConnSequenceFlag;		/*用户连接的序列号，以标识不同的用户*/
	    int iSubConnChn;    					//!子连接通道
	    int iSubConnType;    					//!子连接类型1--实时监控, 2--多画面预览, 3--主意对讲, 4--下载回放
	    int iSubConnFlag;
	    int iChangMainToExtraFlag;
	    int iSocketSendFlag;        			//!,上次的数据没有发完, 或发送出错, 或改变缓冲区大小出错,都会被置1
	    int iSocketNeedChgFlag;					/*用户的子连接的SENDBUFFER需要改变*/
	    int iExtraCfgChangFlag;					//!主辅码流类型改变标志
	    int iClientFlag;    					//!标识客户端版本号,为1的话,支持多连接的,0--老式单连接方式
	    int iRecDownNormalEndFlag;
	    int iOnlineFlag;

	    uint64 iUserBookAlarm;					//订阅告警掩码
	    
	    struct tagNetSTtateMonitor netSTtateMonitor;

	    //录像回放结束标志
	    bool bVodFlag;
	    int iVodChn;
		
	    int iUpPicMsk;	    //订阅定时图片上传
	    bool bUpPicFlag;  //是能手动抓拍上传
	    bool bSend;   	//检测到 开始帧，防止只上传尾帧
		//int iBellFlag;
	};


	//-----------------------------------------------------------------------------
	// function declare
	//-----------------------------------------------------------------------------


	void   NetDelSubUser(struct conn *c);
	int NetOperateSubConn(struct conn *c,int iType,int iChn,unsigned long ulSequencFlag);

	 int NetInsertDataToQueue(struct conn *c, msghdr_t *msg);
	// 下面三个函数针对不同的应用情况使用不同的方式往网络上发送数据

	int DvrNetSendMsg(struct conn *c, msghdr_t *msg);
	int DvrNetSendChain(struct conn *c, msghdr_t *chain);


	void NetCorePoll(struct NetCoreContext *ctx, int milliseconds);

	struct NetCoreContext * NetCoreInit(int argc, char *argv[]);
	void   NetCoreFini(struct NetCoreContext *ctx);
	int    NetCoreOpenPort(int port);
	int    NetCoreRecv(struct conn *c);

	int NetCoreAddHandler(struct NetCoreContext *ctx, NetCoreIO_t *io);


	int NetDvrSendOtherQueue(struct conn *c,int iChn, int iType);
	int NetDvrSendQueue(struct conn *c,int iChn, int iType);

	int NetDvrFreeQueueData(struct conn *c,int cmd,int chn,int flag);
	int NetQueueOverLimit(struct conn *c);
	void *malloc_manager(struct conn *c, size_t nBytes,enum_malloc_method flag);
	void free_manager(struct conn *c, void *ptr,enum_malloc_method flag);
}

#endif


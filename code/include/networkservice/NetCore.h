//    �� �� ���� NetCore.h
//    ��    ��: ����ʵ��������Ƶʵʱ����

//                               ɾ��ԭ�е���������Ӧ�㷨
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
	    struct timeval timestamp;    /* ʱ��� */
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
	    char buf[72];//72+msgbufsize+8 = 128��128*8=1024
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
	    size_t    tail;            /* Bytes written    *//*ʵ���ϴ��������Ѿ���������ݵı�����*/
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
	    msghdr_t         *head;        /* ���Ͷ��е�ͷ�� */
	    msghdr_t         *tail;        /* ���Ͷ��е�β�� */
	    struct timeval startTime;    /* ���еĿ�ʼʱ�� */
	    struct timeval endTime;      /* ���еĽ���ʱ�� */
	    unsigned long  totalSize;   /* �����е������ܳ��� */
	    int SocketSendFailFlag;     //!<������е�������������ʧ��, 
	                                //!�����ٴη��͵�ʱ��Ҫ��һ��ʱ��Ż���
	                                //!�η���, ����ȴ���ʱ���dsp�ĵ�ǰ����
	                                //!�й�ϵ
	    int SocketSendFailCnt;
	    struct timeval SocetSendSleepTime;//!<��¼��һ�����ݷ���ʧ�ܵ�ʱ���
	    int            CurrentDspRate;
	} NetQueue_t;

	typedef struct tagNode
	{
	    void *pMallocAddress;
	    tagMsgHdrMalloc         *head;        /* buf head*/
	    tagMsgHdrMalloc         *cur;        /* free head*/
	    tagMsgHdrMalloc         *tail;        /* buf tail*/
	    int nodeuse;                        /*�Ѿ�ʹ�õĽڵ����*/
	    int buf_size;                       /*�Ѿ�������ڴ��С*/
	} Node_t;

	//��������״̬���
	struct tagNetSTtateMonitor 
	{
	    int ch_no;                  //�򿪵�ͨ����
	    int ch_no_func[N_SYS_CH+1]; //�򿪵�ͨ��ִ�еĹ��� 0 ��ʾ��ͨ��û�� 1 ��� ��2 ���� , 3�ط� add by yang_shukui
	};
	enum monitorFunc
	{
	    NOUSE=0, //0 ��ʾ��ͨ��û��
	    MONITOR, //1 ��� 
	    DOWNLOAD,//2 ���� 
	    PLAYBACK,//3 �ط� 
	    PREVIEW  //4 Ԥ��
	     
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
	    
	    NetQueue_t  TransferQueue;  			/*������ǰ���ݣ�ֻ�����ĳ�����*/ //!������ʹ����
	    NetQueue_t  DataTransferQueue[2];        /* �������            */
	    Node_t      NodeFree;

	    void (*fini)(struct conn *);    		/* ����ִ�����ӶϿ�ʱ���������  */
	    int (*init)(struct conn *);        		/* ����ִ�����ӽ���ʱ�ĳ�ʼ������*/
	    unsigned int    flags;            		/* Flags            */
	    void *context;
	    void *hMutex;
	    
	    unsigned long ulConnSequenceFlag;		/*�û����ӵ����кţ��Ա�ʶ��ͬ���û�*/
	    int iSubConnChn;    					//!������ͨ��
	    int iSubConnType;    					//!����������1--ʵʱ���, 2--�໭��Ԥ��, 3--����Խ�, 4--���ػط�
	    int iSubConnFlag;
	    int iChangMainToExtraFlag;
	    int iSocketSendFlag;        			//!,�ϴε�����û�з���, ���ͳ���, ��ı仺������С����,���ᱻ��1
	    int iSocketNeedChgFlag;					/*�û��������ӵ�SENDBUFFER��Ҫ�ı�*/
	    int iExtraCfgChangFlag;					//!�����������͸ı��־
	    int iClientFlag;    					//!��ʶ�ͻ��˰汾��,Ϊ1�Ļ�,֧�ֶ����ӵ�,0--��ʽ�����ӷ�ʽ
	    int iRecDownNormalEndFlag;
	    int iOnlineFlag;

	    uint64 iUserBookAlarm;					//���ĸ澯����
	    
	    struct tagNetSTtateMonitor netSTtateMonitor;

	    //¼��طŽ�����־
	    bool bVodFlag;
	    int iVodChn;
		
	    int iUpPicMsk;	    //���Ķ�ʱͼƬ�ϴ�
	    bool bUpPicFlag;  //�����ֶ�ץ���ϴ�
	    bool bSend;   	//��⵽ ��ʼ֡����ֹֻ�ϴ�β֡
		//int iBellFlag;
	};


	//-----------------------------------------------------------------------------
	// function declare
	//-----------------------------------------------------------------------------


	void   NetDelSubUser(struct conn *c);
	int NetOperateSubConn(struct conn *c,int iType,int iChn,unsigned long ulSequencFlag);

	 int NetInsertDataToQueue(struct conn *c, msghdr_t *msg);
	// ��������������Բ�ͬ��Ӧ�����ʹ�ò�ͬ�ķ�ʽ�������Ϸ�������

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


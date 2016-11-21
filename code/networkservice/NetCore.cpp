//    文 件 名： NetCore.cpp
//    描    述: 用于实现网络视频实时传输

// 包括动态检测网络状态，根据不同网络传输策略进行调整
// 包括用户的低延时，画面质量和流畅性，带宽利用率等

//-----------------------------------------------------------------------------
// Standard include files:
//-----------------------------------------------------------------------------
#include "networkservice/CNetWorkClient.h"
#include "networkservice/NetCore.h"
#include "misc/Mutex.h"
#include "misc/System.h"

#ifdef WIN32    
#define PF_INET         AF_INET
#define    ERRNO            WSAGetLastError()
#define MSG_NOSIGNAL  0
#define MSG_MORE      0
#define    O_NONBLOCK        0
#define    EWOULDBLOCK        WSAEWOULDBLOCK

#else
#include <sys/wait.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <sys/time.h>    
#include <linux/sockios.h>
#include <unistd.h>
#include <dirent.h>
#define    closesocket(a)            close(a)
#define    ERRNO                errno

#endif    /* _WIN32 */


#include <sys/types.h>    
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stddef.h>
#include <fcntl.h>


namespace NETWORKSERVICE
{

	/*
	 * Global variables
	 */
	struct timeval current_time;
	unsigned long  g_ulUserConnSequenceFlag = 1;

	void add_conn_to_ctx(struct NetCoreContext *ctx, struct conn *c)
	{
	    if((NULL == ctx) ||(NULL == c))
	    {
	        return;
	    }
	    
	    tracef("$$$$$$$$connect count is %d\n",ctx->nactive);    

	    c->next = ctx->connections;
	    ctx->connections = c;
	    ctx->nactive++;
	    c->ctx = ctx;

	}

	void del_conn_from_ctx(struct NetCoreContext *ctx, struct conn *c)
	{
	    struct conn    *tmp;
	    if((NULL == ctx) ||(NULL == c))
	    {
	        return;
	    }
	    

	    if (c == ctx->connections)
	        ctx->connections = c->next;
	    else
	        for (tmp = ctx->connections; tmp != NULL; tmp = tmp->next)
	            if (tmp->next == c)
	            {
	                tmp->next = c->next;
	                break;
	            }

	    ctx->nactive--;
	    assert(ctx->nactive >= 0);

	}

	//目前该函数中的动态申请存在问题，会导致图像不同步，暂时隐掉不用
	//等待下一阶段重写，请不要随便打开注掉部分
	void *malloc_manager(struct conn *c, size_t nBytes,enum_malloc_method flag)
	{
	    if(NULL == c)
	    {
	        return NULL;
	    }
	    if( flag == malloc_msg)
	    {
	         if(nBytes < msgbufunit)
	        {
	            msghdrmalloc_t *mallocAddr  = NULL;    
	            if(c->NodeFree.buf_size == 0)
	            {
	                 c->NodeFree.buf_size  = (NET_PRE_MALLOC_CNT*N_SYS_CH)*sizeof(msghdrmalloc_t);
	                 c->NodeFree.head  =(msghdrmalloc_t *)malloc(c->NodeFree.buf_size );
	                if(c->NodeFree.head )
	                {
	                    memset(c->NodeFree.head ,0,c->NodeFree.buf_size );
	                    c->NodeFree.pMallocAddress =  c->NodeFree.head ;
	                
	                    msghdrmalloc_t *tmpNode = NULL;
	                    c->NodeFree.head->pre = NULL;
	                    c->NodeFree.tail  =(msghdrmalloc_t *)((char *)c->NodeFree.head+(  (NET_PRE_MALLOC_CNT*N_SYS_CH) -1)*sizeof(msghdrmalloc_t));
	                    c->NodeFree.tail->next = NULL;
	                    tmpNode = c->NodeFree.head;
	                    
	                    while(tmpNode && (tmpNode < c->NodeFree.tail))
	                    {
	                        tmpNode->next = (msghdrmalloc_t *)((char *)tmpNode+sizeof(msghdrmalloc_t));
	                        tmpNode->next->pre = tmpNode;
	                        tmpNode = tmpNode->next;
	                    }
	                    
	                    c->NodeFree.cur = c->NodeFree.head;
	                }
	            }

	            if(c->NodeFree.cur)
	            {
	               mallocAddr = c->NodeFree.cur;
	                c->NodeFree.nodeuse++;
	                c->NodeFree.cur = c->NodeFree.cur->next;   
	            }
			else
			{
				printf("malloc_manager fail,c->NodeFree.cur is NULL,c->NodeFree.nodeuse=%d,fail\n",c->NodeFree.nodeuse);
			}
	            
	            return mallocAddr;
	         }
			else
			{
				printf("malloc_manager need malloc nBytes=%d, large msgbufunit=%d,fail\n",nBytes,msgbufunit);
			}
	    }
	    else
	    {
	        void *malloc_addr = malloc( nBytes);
	        return malloc_addr;
	    }
	    
	    return NULL;
	}
	void free_manager(struct conn *c, void *ptr,enum_malloc_method flag)
	{
	    if((NULL == c) ||(NULL == ptr ))
	    {
	        return;
	    }   
	    
	    switch(flag)
	    {
	        case malloc_msg:
	        {
	            msghdrmalloc_t *tmpNode = (msghdrmalloc_t *)ptr;
	            if(tmpNode == c->NodeFree.head)
	            {
	                c->NodeFree.head = tmpNode->next;
	                c->NodeFree.head->pre = NULL;
	                c->NodeFree.tail->next = tmpNode;
	                tmpNode->pre = c->NodeFree.tail;
	                c->NodeFree.tail = tmpNode;
	                c->NodeFree.tail->next = NULL;
	            }
	            else if(tmpNode == c->NodeFree.tail)
	            {
	            }
	            else
	            {
	                tmpNode->pre->next = tmpNode->next;
	                tmpNode->next->pre = tmpNode->pre;
	                
	                c->NodeFree.tail->next = tmpNode;
	                tmpNode->pre = c->NodeFree.tail;
	                c->NodeFree.tail = tmpNode;
	                c->NodeFree.tail->next = NULL;
	            }

	            c->NodeFree.nodeuse--;
	            if(c->NodeFree.cur == NULL)
	            {
	                c->NodeFree.cur = tmpNode;
	            }
	            break;
	        }
	        case malloc_malloc:
	        {
	            free(ptr);
	            break;
	        }
	    }
	}

	/*
	 * 返回发送出去的字节数,返回0表示没有数据发送
	 */
	static inline unsigned long conn_send(struct conn *c, msghdr_t *msg)
	{
	    long n;

	    int flags = MSG_FLAGS_MORE & msg->msg_flags ? MSG_MORE : 0; 

	    if(1 == c->iClientFlag )
	    {
	        if((ACK_CHAN_MONIT != msg->msgCmd)
	            &&(ACK_REQ_AUDIO_DATA != msg->msgCmd)
	            &&(ACK_RECD_PLAY != msg->msgCmd)
	             &&(ACK_MANUL_SNAP != msg->msgCmd)
	            &&(ACK_CHAN_MONIT_EX!= msg->msgCmd))
	        {
	            n = send(c->sock, (const char *)msg->buf, msg->buf_len, flags | MSG_NOSIGNAL);
	            //tracef("Cmd send len %d\n",n);
	        }
	        else
	        {
	          
	        }
	    }
	    else
	    {
	        n = send(c->sock, (const char *)msg->buf, msg->buf_len, flags | MSG_NOSIGNAL);
	    }

	    printf("send n is %d,chn is %d n is %d toal is %d\n",n,msg->chn,n,msg->buf_len);
	    if(n<0)
	    {
	         c->iSocketSendFlag = 1;
	         n = 0;
	        perror("send error\n");
	    }    

	    if(n< (long)msg->buf_len)
	    {
	        c->iSocketSendFlag = 1;
	    }

	    return n;
	}

	/*
	*该函数是网络发送函数，为网络调整基础函数
	*窄带网络传输策略处理核心函数，请不要随意修改
	*/
	static inline unsigned long  NetSendDataToSocketBuffer(struct conn *c, msghdr_t *chain, unsigned long nMaxSize = 640 *1024)
	{
	    msghdr_t *msg;

	    int  nSendBuf = 16*1024;        ///< 默认16K为开启网络传输策略情况下的buf大小
	    bool bSetSocketBufFailed = false;

	    if(1 == c->iSocketNeedChgFlag )
	    {
	        if(1 == c->iClientFlag )
	        {
	          // if (0 == g_stNetTransPolicy.bNetTransPolicy)    ///< 未开启策略,设置为32k
	            {
	                nSendBuf = 32*1024;
	            }


	            if (bSetSocketBufFailed)
	            {
	                c->iSocketSendFlag = 1;
	                __trip;
	                return 0;
	            }
	        }
	        else
	        {
	            __trip;

	        }
	        
	        c->iSocketNeedChgFlag  = 0;
	        
	    }
	    
	    c->iSocketSendFlag = 0;    
	    
#ifdef WIN32

	    int nCount = 0;
	    for(msg = chain; msg != NULL; msg = msg->next) 
	    {
	        int nRet = conn_send(c, msg);
	        if(nRet <= 0)
	        {
	            break;
	        }
	        nCount += nRet;
	    }
	    return nCount;
	    
#else
	    unsigned long n, total_size =0;
	    struct iovec iov[200];
	    msg = chain;

	  
	    if (msg->buf_len > nMaxSize)
	    {
	        printf("msg->buf_len = %d\r\n",msg->buf_len);
	    }
	    
	    for(n = 0;n < (sizeof(iov) / sizeof(iov[0]))&& (msg != NULL)&& ((total_size + msg->buf_len) <= nMaxSize);n ++) 
	    {
	        iov[n].iov_base = msg->buf;
	        iov[n].iov_len  = msg->buf_len;
	        total_size     += msg->buf_len;
	         
	        msg = msg->next;
	    }

	    if(0 == n) 
	    {
	    __trip;
	        c->iSocketSendFlag = 1;
	        return 0;
	    }
	    
	    long nRet  = 0;
	    if(1 == c->iClientFlag )
	    {
	        if((ACK_CHAN_MONIT != chain->msgCmd)
	            &&(ACK_REQ_AUDIO_DATA != chain->msgCmd)
	            &&(ACK_RECD_PLAY != chain->msgCmd)
	             &&(ACK_MANUL_SNAP != chain->msgCmd)
	            &&(ACK_CHAN_MONIT_EX != chain->msgCmd))
	        {
	            nRet = writev(c->sock, iov, n);
	        }
	        else
	        {
	            
	        }
	    }
	    else
	    {
	         nRet = writev(c->sock, iov, n);
	    }
	    
	//    tracef("writev nret is %d,chn is %d n is %d toal is %d\n",nRet,chain->chn,n,total_size);
	    if(nRet < 0 ) 
	    { 
	        nRet = 0;
	    }

	    if(nRet < (long)total_size)
	    {
	        c->iSocketSendFlag = 1;
	    }
	    
	    return nRet;
#endif

	}

	 int NetInsertDataToQueue(struct conn *c, msghdr_t *msg)
	{
	    msghdr_t *node = NULL;
	    int nBufSize = 0;

	    NetQueue_t *queue = &c->TransferQueue;

	    if((ACK_CHAN_MONIT != msg->msgCmd)
	        &&(ACK_REQ_AUDIO_DATA != msg->msgCmd)
	        &&(ACK_RECD_PLAY != msg->msgCmd)
	         &&(ACK_MANUL_SNAP != msg->msgCmd)
	        &&(ACK_CHAN_MONIT_EX != msg->msgCmd))
	    {
	         queue = &c->DataTransferQueue[0];    
	    }
	    else
	    {
	        if(ACK_CHAN_MONIT == msg->msgCmd
				||ACK_MANUL_SNAP == msg->msgCmd)
	        {
	            queue = &c->DataTransferQueue[msg->chn +1];
	        }
	        else
	        {
	        }   
	    }

	    if(msg->callback == NULL)
	    {
	        nBufSize = msg->buf_len;
	    }
	    
	    void *tmp = malloc_manager(c,sizeof(msghdr_t) + nBufSize,malloc_msg);
	    msg->malloc_flags = malloc_msg;
	    msg->msgAddr = tmp;
	    
	    node = (msghdr_t *)tmp;
	    if(node) 
	    {
	        msg->timestamp   = current_time;
	        queue->endTime   = current_time;
	        queue->totalSize+= msg->buf_len;
	        memcpy((void *)node, msg, sizeof(msghdr_t));
	        node->next  = NULL;
	        
	        if(nBufSize)
	        {
	            node->buf = (void *)(node + 1);
	            memcpy(node->buf, msg->buf, nBufSize);
	        }
	        /*
	         * 下面我们把这一个节点加入到队列的相应位置
	         */
	        if(queue->head) 
	        {
	            queue->tail->next = node;
	            queue->tail       = node;
	        } 
	        else
	        {
	            queue->head     = queue->tail = node;
	            queue->startTime= msg->timestamp;
	        }    
	    }
	    else
	    {
	    	printf("NetInsertDataToQueue insert sizeof(msghdr_t)=%d ,nBufSize=%d,malloc_manager malloc error,queue too long\n",sizeof(msghdr_t) , nBufSize);
	        if(msg->callback)//申请不到内存，需要释放packet资源
	        {
	            msg->callback(msg->context);
	        }
	    }
	    return node == NULL ? -1: 0;
	}


	/*
	*该函数是网络发送函数，为网络调整基础函数
	*窄带网络传输策略处理核心函数，请不要随意修改
	*/
	static int NetSendChnDataUnlock(struct conn *c,int iChn,int iCmd)
	{
	    NetQueue_t *queue = NULL;    

	    msghdr_t *p = NULL , *q;
	    msghdr_t *msg;
	    int max_send_buf = 640* 1024; //最大的发送缓冲,需要通过计算得出
	    unsigned long n;
	    int iDataFlag = 0;//!标识传送视频数据标识
	    struct timeval time_now;
#ifdef LINUX
	    gettimeofday(&time_now, NULL);
#else
	    time_now.tv_sec = 0;
	    time_now.tv_usec = SystemGetMSCount()*1000; 
#endif

	    if((ACK_CHAN_MONIT !=iCmd)
	        &&(ACK_REQ_AUDIO_DATA != iCmd)
	        &&(ACK_RECD_PLAY != iCmd)
	        &&(ACK_MANUL_SNAP != iCmd)
	        &&(ACK_CHAN_MONIT_EX != iCmd))
	        
	    {
	        if(0 == c->DataTransferQueue[0].CurrentDspRate)
	        {
	            c->DataTransferQueue[0].CurrentDspRate = 3*1024;
	        }
	        
	        queue = &c->DataTransferQueue[0];    
	    }
	    else
	    {
	        if(ACK_CHAN_MONIT == iCmd ||ACK_MANUL_SNAP == iCmd)
	        {
	            queue = &c->DataTransferQueue[iChn +1];
	            iDataFlag = 1;
	        }
	        else
	        {
	        
	        }   

	    }

	    if(0  == queue->totalSize)
	    {
	    __trip;
	        return 0;
	    }



	    n = NetSendDataToSocketBuffer(c, queue->head, max_send_buf);
	    queue->totalSize -= n ;   // 队列的总长度做相应的更改
	    c->TransferQueue.totalSize -= n;/*这是总的数据长度，保持向前兼容*/

	    for(msg = queue->head;msg && (n >=msg->buf_len); )
	    {
	        /*
	         * 数据已经发送成功,如果设置了回调函数在这里处理
	         */
	        n -= msg->buf_len;

	        q = msg;
	        msg = msg->next;

	        if(q->callback) 
	        {
	            q->callback(q->context);
	        }
	        if(q->msgAddr)
	        {
	            free_manager(c,q->msgAddr,(enum_malloc_method)q->malloc_flags);
	        }
	    }
	    
	    if(n > 0) 
	    {
	        assert(msg != NULL);
	        msg->buf      = (void *)((char *)msg->buf + n);
	        msg->buf_len -= n;
	        
	        /*
	         * 把优先级提到最高,不允许后面的数据插到它的前面,避免破坏数据包的完整性
	         */
	        for(p = msg, q = msg->next;
	            q && p->msg_flags & MSG_FLAGS_MORE;) 
	        {
	            p = p->next;
	            q = p == NULL ? NULL : p->next;
	        }
	    } 

	    queue->head    = msg;
	    if(msg)  
	    {
	        /*
	         * 这里我们不考虑队列中数据经过排序的问题,避免算法复杂化
	         */
	        queue->startTime = msg->timestamp;
	    }

	    if(NULL == queue->head)
	    {
	        queue->totalSize = 0;
	    }

	    
	    return 0;     
	}

	     
	int DvrNetSendMsg(struct conn *c, msghdr_t *msg)
	{
	    unsigned long n =0;
	    const NetQueue_t *queue = NULL;    
		//printf("111cmd = [%x]\n", msg->msgCmd);
	    MutexEnter(c->hMutex);    

	    /* 信令传输*/
	    if((ACK_CHAN_MONIT != msg->msgCmd)
	        &&(ACK_REQ_AUDIO_DATA != msg->msgCmd)
	        &&(ACK_RECD_PLAY != msg->msgCmd)
	        &&(ACK_MANUL_SNAP != msg->msgCmd)
	        &&(ACK_CHAN_MONIT != msg->msgCmd))
	    {
	    	
	        queue = &c->DataTransferQueue[0];
	    }
	    else
	    {
	    printf("cmd = [%x]", msg->msgCmd);
	        if(ACK_CHAN_MONIT == msg->msgCmd ||ACK_MANUL_SNAP == msg->msgCmd)
	        {
	            queue = &c->DataTransferQueue[msg->chn +1];
	        }
	        else
	        {
	        }
	    }
	    
	    /*
	     * 更新当前的时间
	     */        
#ifdef LINUX
	    gettimeofday(&current_time, NULL);
#else
	    current_time.tv_sec = 0;
	    current_time.tv_usec = SystemGetMSCount()*1000; 
#endif
	     
	    if(!queue->head) 
	    {
	        n = conn_send(c, msg);
	    //    tracef("*******n is %d\n",n);
	        if(n == msg->buf_len ) 
	        {
	            /*
	            * 数据已经发送成功,如果设置了回调函数在这里处理
	            */
	            if(msg->callback)
	            {
	                msg->callback(msg->context);
	            }
	        }
	        else
	        {
	            /* 
	             * 如果数据发送了一部分,必须把该数据放到队列的最前面
	             */
	            msg->buf      = (void *)((char *)msg->buf + n);
	            msg->buf_len -= n;
	            
	            NetInsertDataToQueue(c, msg);
	        }
	    } 
	    else 
	    {
	        NetInsertDataToQueue(c, msg);
	        NetSendChnDataUnlock(c,msg->chn ,msg->msgCmd);
	    }
	    MutexLeave(c->hMutex);
	    return 0;        
	}


	/*
	*该函数是网络发送函数，为网络调整基础函数
	*窄带网络传输策略处理核心函数，请不要随意修改
	*/
	int DvrNetSendChain(struct conn *c, msghdr_t *chain)
	{
	    msghdr_t *msg;
	    size_t n =0;
	     NetQueue_t *queue = &c->TransferQueue;    
	    int iDataFlag = 0;
	    struct timeval time_now;
#ifdef LINUX
	    gettimeofday(&time_now, NULL);
#else
	    time_now.tv_sec = 0;
	    time_now.tv_usec = SystemGetMSCount()*1000; 
#endif
	    MutexEnter(c->hMutex);    

	    if((ACK_CHAN_MONIT != chain->msgCmd)
	        &&(ACK_REQ_AUDIO_DATA != chain->msgCmd)
	        &&(ACK_RECD_PLAY != chain->msgCmd)
	        &&(ACK_MANUL_SNAP != chain->msgCmd)
	        &&(ACK_CHAN_MONIT_EX  != chain->msgCmd))
	    {
	        if(0 == c->DataTransferQueue[0].CurrentDspRate)
	        {
	            c->DataTransferQueue[0].CurrentDspRate = 3*1024;
	        }

	        queue = &c->DataTransferQueue[0];    
	       //     tracef("******DvrNetSendChain Video Queue Size %d ,Cmd size %d\n",c->TransferQueue.totalSize,c->CmdTransferQueue.totalSize);
	    }
	    else
	    {
	        if(ACK_CHAN_MONIT == chain->msgCmd ||ACK_MANUL_SNAP == chain->msgCmd)
	        {
	            queue = &c->DataTransferQueue[chain->chn + 1];
	            iDataFlag = 1;
	        }
	        else
	        {
	        }
	    }
	    
#ifdef LINUX
	    gettimeofday(&current_time, NULL);
#else
	    current_time.tv_sec = 0;
	    current_time.tv_usec = SystemGetMSCount()*1000; 
#endif
		
	    if(!queue->head)
	    {        
			
	        n = NetSendDataToSocketBuffer(c, chain);
	        
			//if(chain->next)
			//	printf("DvrNetSendChain queue->head is NULL, need send iLen =%d,has sent n=%d\n",chain->next->buf_len,n);
	        for(msg = chain;msg && (n >= msg->buf_len); msg = msg->next )
	        {
	            /*
	            * 数据已经发送成功,如果设置了回调函数在这里处理
	            */
	            n -= msg->buf_len;
	            if(msg->callback) 
	            {
	            msg->callback(msg->context);
	            }
	        }

	        if(msg)
	        {
	            /*
	            * 如果数据发送了一部分,必须把该数据放到队列的最前面
	            */

	            msg->buf      = (void *)((char *)msg->buf + n);
	            msg->buf_len -= n;
	            for(;msg; msg = msg->next) 
	            {
	                
	                NetInsertDataToQueue(c, msg);
	            }
	        }
	    } 
	    else 
	    {
	    	if(chain->next)
				printf("DvrNetSendChain queue->head is not NULL, need send iLen =%d\n",chain->next->buf_len);
	        for(msg = chain;msg; msg = msg->next) 
	        {
	            NetInsertDataToQueue(c, msg);
	        }
	        n =NetSendChnDataUnlock(c,chain->chn,chain->msgCmd);
		printf("DvrNetSendChain queue->head is not NULL, has send n =%d\n",n);
	    } 

	    MutexLeave(c->hMutex);

	    return 0;        
	}

	int NetOperateSubConn(struct conn *c,int iType,int iChn,unsigned long ulSequencFlag)
	{
	    if(NULL == c)
	    {
	        return 0;
	    }
	    
	    struct conn    *tmp;
	    struct NetCoreContext *ctx = c->ctx;
	    int iFind = 0;
	        
	    //EnterCriticalSection(&ctx->mutex);
	        
	    for (tmp = ctx->connections; tmp != NULL; tmp = tmp->next)
	    {
	        if((1 == tmp->iSubConnFlag)
	           ||(1 == tmp->ulConnSequenceFlag))
	        {
	           continue;
	        }
	        
	        /*找到对应的主连接*/
	        if (tmp->ulConnSequenceFlag == ulSequencFlag) 
	        {
	            iFind = 1;
	            if(1 ==  iType)
	            {
	              
	            }
	            else 
	            {
	            }

	            c->ulConnSequenceFlag = 0;

	            c->flags &= ~FLAG_IO_READY;
	            tracef("sub connect sock is %d chn is %d suquen is %lu,type is %d mainsocket %d\n",c->sock,iChn,ulSequencFlag,iType,tmp->sock);
	              
	            break;
	        }
	    }

	    //LeaveCriticalSection(&ctx->mutex);

	     return iFind;

	}

	void  NetDelSubUser(struct conn *c)
	{
	    if(NULL == c)
	    {
	        return;
	    }
	    
	    tracef("net dl sub user %p seq[%d]\n", c, c->ulConnSequenceFlag);
	    
	    if(c->fini)
	    {
	        c->fini(c);
	    }


	    if(c->hMutex) 
	    {
	        MutexEnter(c->hMutex);
	    }     
	    
	    msghdr_t *msg, *q;
	    int iChn = 0;    
	    NetQueue_t *queue = &c->TransferQueue;
	    
	    for(iChn = 0; iChn <2;iChn ++)
	    {
	        queue = &c->DataTransferQueue[iChn];

	        for(msg = queue->head;msg ; ) 
	        {
	            q = msg;
	            msg = msg->next;

	            if(q->callback) 
	            {
	                q->callback(q->context);
	            }

	            {
	                free_manager(c,q,(enum_malloc_method)q->malloc_flags);
	            }

	        }    
	    }

	    if(c->NodeFree.pMallocAddress 
	        && c->NodeFree.buf_size > 0)
	    {
	        free(c->NodeFree.pMallocAddress);
	        c->NodeFree.pMallocAddress = NULL;
	    }

	    if(c->hMutex) 
	    {
	        MutexLeave(c->hMutex);
	    }     
	    
	    del_conn_from_ctx(c->ctx, c);
	    
	    if(c->hMutex) 
	    {
	        MutexDestory(c->hMutex);
	    }
	    free(c);

	}

	/*
	 * Disconnect from remote side, free resources
	 */
	void disconnect(struct conn *c)
	{
	    if(NULL == c)
	    {
	        return;
	    }
	    NetQueue_t *queue = &c->TransferQueue;
	    msghdr_t *msg, *q;
	    int iChn = 0;

	    tracef("disconnecting %p\r\n", c);

	    c->flags |= FLAG_FINISHED; 
	    if(c->fini)
	    {
	        c->fini(c);
	    }

	    if (c->hMutex)
	    {
	        MutexEnter(c->hMutex);
	    }    

	    for(iChn = 0; iChn <2;iChn ++)
	    {
	        queue = &c->DataTransferQueue[iChn];
	        
	        for(msg = queue->head;msg ; ) 
	        {
	            q = msg;
	            msg = msg->next;

	            if(q->callback) 
	            {
	                q->callback(q->context);
	            }
	            if(q->msgAddr)
	            {
	                free_manager(c,q->msgAddr,(enum_malloc_method)q->malloc_flags);
	            }
	        }    
	    }

	    if(c->NodeFree.pMallocAddress 
	        && c->NodeFree.buf_size > 0)
	    {
	        free(c->NodeFree.pMallocAddress);
	        c->NodeFree.pMallocAddress = NULL;
	    }

	    
	    if (c->hMutex)
	    {
	        MutexLeave(c->hMutex);
	    }   

	    del_conn_from_ctx(c->ctx, c);
	    (void) shutdown(c->sock, 2);
	    (void) closesocket(c->sock);
	    c->sock = INVALID_SOCKET;
	    if(c->hMutex) 
	    {
	        MutexDestory(c->hMutex);
	    }
	    free(c);
	    c = NULL;
	}


	int NetCoreRecv(struct conn *c)
	{

	    if(NULL == c)
	    {
	        return 0;
	    }
	    
	    int n = 0;

	    if (c->flags & FLAG_SOCK_READABLE) 
	    {
	        const unsigned long len = IO_SPACELEN(&c->remote);

	        n = recv(c->sock, c->remote.buf + c->remote.head, len, 0);

	        if (n == 0 || (n < 0 && ERRNO != EWOULDBLOCK))
	        {
	            perror("readremote");
	            c->remote.done = 1;
	            
	            tracef("$$$$$$$$$$$ socket is %d, ret: %d\n",c->sock, n);
	        }
	        
	        if (n > 0)
	        {
	            c->remote.head += n;
	            c->remote.buf[c->remote.head] = '\0';
	            c->iOnlineFlag = 1;
	        } 
	        else if (c->remote.done) 
	        {
	            c->flags |= FLAG_FINISHED;
	            c->flags &= ~FLAG_IO_READY;
	        }
	    }    
	    return n;
	}

	        
	/*
	 * One iteration of server loop.
	 */
	void NetCorePoll(struct NetCoreContext *ctx, int milliseconds)
	{
	    assert(ctx != NULL);
	    
	    struct conn    *c, *nc;
	    struct timeval    tv;            /* Timeout for select() */
	    fd_set        read_set, write_set;
	    int        max_fd = 0, msec = milliseconds;
	    const NetQueue_t *queue;
	    
#ifdef LINUX
	    gettimeofday(&current_time, NULL);
#else
	    current_time.tv_sec = 0;
	    current_time.tv_usec = SystemGetMSCount()*1000; 
#endif

	    FD_ZERO(&read_set);
	    FD_ZERO(&write_set);

	    for (c = ctx->connections; c != NULL; c = nc) 
	    {
	        nc = c->next;
	        int error=-1, len;
	        len = sizeof(error);
	        int     ret = getsockopt(c->sock, SOL_SOCKET, SO_ERROR, (char*)&error, (socklen_t *)&len);
	        if( (ret != 0) || (error == EPIPE) )
	        {
	            printf("NetCorePoll():ret= %d error = %d\r\n",ret,error);
	            disconnect(c);
	            c = NULL;
	            continue;
	         }
	     }

	    for (c = ctx->connections; c != NULL; c = c->next) 
	    {
	        c->flags &= ~FLAG_IO_READY;

	        if(1 == c->iSubConnFlag)
	        {
	            continue;
	        }
	        if (c->flags & FLAG_FINISHED) 
	        {
	            continue;    
	        }
	        queue = &c->DataTransferQueue[0];

#define    MERGEFD(fd,set)    \
	    do {FD_SET(fd, set); if (fd > max_fd) max_fd = fd; } while (0)
	    
	        /* If there is space to read, do it */
	        if (IO_SPACELEN(&c->remote))
	            {MERGEFD(c->sock, &read_set);}
	        
	        /* If there is data in output queue, add to write set */
	        if (queue->head)
	            {MERGEFD(c->sock, &write_set);}

	            
	    }

	    tv.tv_sec = msec / 1000;
	    tv.tv_usec = msec % 1000;

	    /* Check IO readiness */
	    if (select(max_fd + 1, &read_set, &write_set, NULL, &tv) < 0) 
	    {
#ifdef WIN32
	        SystemSleep(milliseconds);
#else
	        tracef("select: %s\r\n", strerror(errno));
#endif
	        return;
	    }

	    /* Set IO readiness flags */
	    for (c = ctx->connections; c != NULL; c = c->next)
	    {
	        if (FD_ISSET(c->sock, &read_set)) 
	        {
	            c->flags |= FLAG_SOCK_READABLE;
	        }
	        if (FD_ISSET(c->sock, &write_set))
	        {
	            c->flags |= FLAG_SOCK_WRITABLE;
	        }
	    }

	    /* Loop through all connections, handle if IO ready */
	    for (c = ctx->connections; c != NULL; c = nc) 
	    {
	        nc = c->next;
	        if (c->flags & FLAG_SOCK_READABLE) 
	        {
	            c->watch(c);
	        }
	        if (c->flags & FLAG_SOCK_WRITABLE) 
	        {
	            MutexEnter(c->hMutex);    

	            NetSendChnDataUnlock(c,-1, 0);

	            //if(c->DataTransferQueue[ICaptureManager::instance()->GetLogicChnNum() + 1].totalSize != 0)
	            {
	                NetSendChnDataUnlock(c,-1, ACK_RECD_PLAY);
	            }
	            MutexLeave(c->hMutex);
	        }
	  
	        if ((c->flags & FLAG_FINISHED) || c->iOnlineFlag == -1)
	        {
	            disconnect(c);
	            c = NULL;
	        }
	  
	        if(NULL != c)
	        {
	            if(1 == c->iSubConnFlag)
	            {
	                int find = NetOperateSubConn(c,c->iSubConnType,c->iSubConnChn,c->ulConnSequenceFlag);
					if (1 != find)
					{
					    (void) shutdown(c->sock, 2);
					    (void) closesocket(c->sock);
						c->sock = INVALID_SOCKET;
						//printf("_______________________%lu, cSCount [%d], [%d]\n", c->ulConnSequenceFlag, ++cSCount,  c->sock);
					}
	                NetDelSubUser(c);
	                c = NULL;
	            }
	        }
	    }
	}

	/*
	 * Initialize shttpd
	 */
	struct NetCoreContext *NetCoreInit(int argc, char *argv[])
	{
	    struct NetCoreContext    *ctx;

#ifdef LINUX
	    gettimeofday(&current_time, NULL);
#else
	    current_time.tv_sec = 0;
	    current_time.tv_usec = SystemGetMSCount()*1000; 
#endif

	    if ((ctx = (struct NetCoreContext *)calloc(1, sizeof(*ctx))) == NULL)
	        tracef( "do_init: cannot allocate context");

	    assert(ctx != NULL);
	    

#ifdef WIN32
	    {WSADATA data;    WSAStartup(MAKEWORD(2,2), &data);}
#endif /* _WIN32 */

	    tracef( "do_init: initialized context %p", ctx);
	    
	    return (ctx);
	}

	/*
	 * Setup listening socket on given port, return socket
	 */
	int NetCoreOpenPort(int port)
	{
	    int        sock, on = 1;
	    struct usa    sa;

#ifdef WIN32
	    {WSADATA data;    WSAStartup(MAKEWORD(2,2), &data);}
#endif /* _WIN32 */

	    sa.len                = sizeof(sa.u.sin);
	    sa.u.sin.sin_family        = AF_INET;
	    sa.u.sin.sin_port        = htons((ushort) port);
	    sa.u.sin.sin_addr.s_addr    = htonl(INADDR_ANY);

	    if ((sock = socket(PF_INET, SOCK_STREAM, 6)) == -1)
	        tracef( "shttpd_open_port: socket: %s",strerror(ERRNO));
	    else 
	    {
#if defined(WIN32)
	        unsigned long    ulON = 1;
	        ioctlsocket(sock, FIONBIO, &ulON);
#else
	        int    flags = fcntl(sock, F_GETFL, 0);
	        if (flags != -1)
	        {
	            fcntl(sock, F_SETFL, flags | O_NONBLOCK) ;
	        }
#endif /* _WIN32 */

	        setsockopt(sock, SOL_SOCKET,  SO_REUSEADDR,(char *) &on, sizeof(on));

	    if (bind(sock, &sa.u.sa, sa.len) < 0)
	        tracef( "shttpd_open_port: bind(%d): %s",   port, strerror(ERRNO));
	    else if (listen(sock, 128) != 0)
	        tracef( "shttpd_open_port: listen: %s",strerror(ERRNO));
	    
#ifdef LINUX
	    (void) fcntl(sock, F_SETFD, FD_CLOEXEC);
#endif /* !_WIN32 */

	    }

	    return (sock);
	}


	int NetDvrSendOtherQueue(struct conn *c,int iChn,int iType)
	{
	  
	    MutexEnter(c->hMutex);    

	    //for(int iTmpChn = 0 ; iTmpChn < ICaptureManager::instance()->GetLogicChnNum() ; iTmpChn ++)
	    for(int iTmpChn = 0 ; iTmpChn <1 ; iTmpChn ++)
	    {
	        if(iTmpChn == iChn)
	        {
	            continue;
	        }

	        if(c->DataTransferQueue[iTmpChn + 1].totalSize != 0)
	        {
	            NetSendChnDataUnlock(c,iTmpChn,iType) ;
	        }
	    }

	    MutexLeave(c->hMutex);

	    return 0;
	}

	int NetDvrSendQueue(struct conn *c,int iChn,int iType)
	{
	    MutexEnter(c->hMutex);    
	     
	    NetSendChnDataUnlock(c,iChn,iType) ;

	    MutexLeave(c->hMutex);
	    return 0;
	}


	/*
	 * Deallocate shttpd object, free up the resources
	 */
	void NetCoreFini(struct NetCoreContext *ctx)
	{
	    assert(ctx != NULL);

	    tracef("NetCoreFini=========>>>>>>>>>>>>>>>>>>>>\n");
	    struct conn    *c, *nc;

	    /* Free all connections */
	    for (c = ctx->connections; c != NULL; c = nc) 
	    {
	        nc = c->next;
	        disconnect(c);
	        c = NULL;
	    }


	    free(ctx);
	    ctx = NULL;
	    
	}

	static void do_accept(struct conn *conn)
	{
	    if(NULL == conn)
	    {
	        return;
	    }
	    
	    int    sock = INVALID_SOCKET;
	    struct usa    sa;
	    const int optval = 1;

	    struct NetCoreContext *ctx = conn->ctx;

	    sa.len = sizeof(sa.u.sin);

	    sock = accept(conn->sock, &sa.u.sa, &sa.len);
	    
	    if (sock == INVALID_SOCKET)
	    {
	        tracef( "do_accept(%d): %s", (int) conn->sock, strerror(ERRNO));
	        return ;
	    }
	    
	    struct conn    *c;
#ifdef LINUX

	    int keepAlive = 1; // 开启keepalive属性
	    int keepIdle = 60; // 如该连接在60秒内没有任何数据往来,则进行探测
	    int keepInterval = 5; // 探测时发包的时间间隔为5 秒
	    int keepCount = 3; // 探测尝试的次数.如果第1次探测包就收到响应了,则后2次的不再发.
	    if( setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepAlive, sizeof(keepAlive)) < 0)
	    {
	        tracef("setsockopt keepalive failed..\n");
	    }    
	    else if(  setsockopt(sock, SOL_TCP, TCP_KEEPIDLE, (void*)&keepIdle, sizeof(keepIdle)) < 0 )
	    {
	        tracef("setsockopt keepidle failed..\n");
	    }
	    else if( setsockopt(sock, SOL_TCP, TCP_KEEPINTVL, (void *)&keepInterval, sizeof(keepInterval)) < 0 )
	    {
	        tracef("setsockopt keepintvl failed..\n");
	    }
	    else if( setsockopt(sock, SOL_TCP, TCP_KEEPCNT, (void *)&keepCount, sizeof(keepCount)) < 0 )
	    {
	        tracef("setsockopt keepcnt failed..\n");
	    }

	    gettimeofday(&current_time, NULL);
#else
	    current_time.tv_sec = 0;
	    current_time.tv_usec = SystemGetMSCount()*1000; 
#endif

	    sa.len = sizeof(sa.u.sin);
#if defined(WIN32)
	        unsigned long    ulON = 1;
	        ioctlsocket(sock, FIONBIO, &ulON);
#else
	        int    flags = fcntl(sock, F_GETFL, 0);
	        if (flags != -1)
	        {
	            fcntl(sock, F_SETFL, flags | O_NONBLOCK) ;
	        }
#endif /* _WIN32 */

	    if (getpeername(sock, &sa.u.sa, &sa.len)) 
	    {
	        tracef( "shttpd_add: %s\r\n", strerror(errno));
	        (void) closesocket(sock);
	        sock = INVALID_SOCKET;
	    } 
	    else if ((c = (struct conn *)malloc(sizeof(*c))) == NULL)
	    {
	        (void) closesocket(sock);
	        sock = INVALID_SOCKET;
	        tracef( "shttpd_add: calloc: %s", strerror(ERRNO));
	    } 
	    else if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY,
	                   (char *)&optval, sizeof(optval)) < 0)
	    {
	        tracef("error setsockopt nodelay");
	        (void) closesocket(sock);
	        sock = INVALID_SOCKET;        
	    }
	    else 
	    {
	        /*一个新的用户连接上来，加1，以标识不同的连接*/
	        g_ulUserConnSequenceFlag ++;

	        int nSendBuf=64*1024;
	        printf("NetCore do_accept Send buffer is %d.\n",nSendBuf);
	        
	        if(setsockopt(sock,SOL_SOCKET,SO_SNDBUF,( char*)&nSendBuf,sizeof(int))<0)
	        {
	            tracef("NetCore do_accept set send buffer error!\n");
	        }

	        NetCoreIO_t *io = (NetCoreIO_t *)conn->context;
	        
	        memset(c, 0, sizeof(*c));        
	    
	                
	        MutexCreate(&c->hMutex,MUTEX_FAST);
	        c->sa        = sa;
	        c->sock        = sock;


	        int iTemp;
	        for(iTemp = 0; iTemp< 2; iTemp ++)
	        {
	            memset(&c->DataTransferQueue[iTemp] , 0, sizeof(NetQueue_t));
	        }

	        c->watch    = io->process;
	        c->fini     = io->fini;
	        c->context  = io->context;
	        c->watch_data    = c;
	        c->birth    = current_time.tv_sec;
	        c->iOnlineFlag = 1;
	        c->iUserBookAlarm = 0;

	        c->ulConnSequenceFlag  = g_ulUserConnSequenceFlag;
	        c->iExtraCfgChangFlag = 1;
	        c->iSocketNeedChgFlag = 0;
	        c->iClientFlag = 0;
	        memset( &c->NodeFree,0,sizeof(Node_t));

	        c->iSubConnFlag = 0;
	        c->iRecDownNormalEndFlag = 0;
		 c->iUpPicMsk = 0;
		 c->bUpPicFlag =0;
		 c->bSend =0;
	        
	        add_conn_to_ctx(ctx, c);
	        
	        if(io->init)
	        {
	            if(io->init(c) != 0)
	            {
	                disconnect(c);
	                return ;
	            }
	        }                
#ifdef LINUX
	        (void) fcntl(sock, F_SETFD, FD_CLOEXEC);
#endif /* _WIN32 */
	        tracef( "shttpd_add: %lu ctx %p, sock %d, conn %p\n", c->ulConnSequenceFlag, ctx, sock, c);
	    }        
	}



	int NetCoreAddHandler(struct NetCoreContext *ctx, NetCoreIO_t *io )
	{
	    if((NULL == ctx)||(NULL == io))
	    {
	        return -1;
	    }
	    
	    struct conn    *c = 0;
	    int sock = NetCoreOpenPort(io->port);
	    if(sock != -1) 
	    {
	        if ((c = (struct conn *)calloc(1, sizeof(*c) + sizeof(NetCoreIO_t))) == NULL)
	        {
	            tracef( "NetCoreAddHandler: cannot allocate connection");
	            return 0;
	        }
	        
	        c->watch        = do_accept;
	        c->watch_data   = (void *) c;
	        c->sock         = sock;
	        
	        int iTemp;

	        for(iTemp = 0; iTemp< 2; iTemp ++)
	        {
	            memset(&c->DataTransferQueue[iTemp] , 0, sizeof(NetQueue_t));
	        }

	        c->ulConnSequenceFlag  = g_ulUserConnSequenceFlag;
	        
	        c->iOnlineFlag    = 1;
	        c->context        = (void *)(c + 1);
	        c->ctx            = ctx;
	        memcpy(c->context, io, sizeof(NetCoreIO_t));
	        
	        add_conn_to_ctx(ctx, c);
	        return 0;
	    } 
	    return -1;
	}



	int NetDvrFreeQueueData(struct conn *c,int cmd,int chn,int flag)
	{
	    if(NULL == c)
	    {
	        return 0;
	    }
	    
	    NetQueue_t *queue = NULL;  
	    
	    unsigned long n;
	    msghdr_t *pre = NULL;
	    msghdr_t *tmp = NULL;

	    bool bRelease = false;
	    msghdr_t *next  = NULL;

	    
	    MutexEnter(c->hMutex);    

	    tracef("!!!!!!!!!!!!!!!!!!!!!!!1cmd %d ,chn %d\n",cmd,chn);

	    if (c->flags & FLAG_FINISHED) 
	    {
	        flag = 0;
	    }
	    if((ACK_CHAN_MONIT !=cmd)
	        &&(ACK_REQ_AUDIO_DATA != cmd)
	        &&(ACK_RECD_PLAY != cmd)
	        &&(ACK_MANUL_SNAP != cmd)
	        &&(ACK_CHAN_MONIT_EX  != cmd))
	    {
	        queue = &c->DataTransferQueue[0];    
	    }
	    else
	    {
	       
	    
	    }
	    
	//    trace("########1######total is %lu ,head=0x%8x,tail=0x%8x\n",queue->totalSize,queue->head,queue->tail);
	 
	    bRelease = false;
	    n = 0;
	    next = NULL;
	    
	    tmp = queue->head;
	   
	    while(tmp)
	    {
	        n = 0;
	        next = tmp->next;
	        if(bRelease == true)
	        {
	            n = tmp->buf_len;
	            bRelease = false;
	        }
	        else if((tmp->msgCmd == cmd)&&(tmp->chn == chn))
	        {
	            if(1 == flag )
	            {
	                if(PKT_FULL_FRAME == tmp->iFrameFlag )
	                {                
	                    tracef("\n@@@@@@@@@@@@@@@@ find full pkt\n");
	                    flag = 0;
	                }

	                pre = tmp;
	                tmp = next;
	                continue;    
	            }
	            else
	            {
	                n = tmp->buf_len;
	                bRelease = true;
	            }
	        }
	        else 
	        {
	            pre = tmp;
	            tmp = next;
	            continue;
	        }
	        queue->totalSize-= n ;   // 队列的总长度做相应的更改
	        c->TransferQueue.totalSize -= n;

	        if(pre)
	        {
	            pre->next = next;
	        }
	        
	        if(tmp == queue->head)
	        {
	            queue->head    = next;
	            if(next)  
	            {
	                queue->startTime = next->timestamp;
	            }
	        }
	        if(tmp == queue->tail)
	        {
	            queue->tail = pre;
	            if(pre)  
	            {
	                queue->endTime = pre->timestamp;
	            }
	        }
	        
	        if(tmp->callback) 
	        {
	            tmp->callback(tmp->context);
	        }

	        /*
	         * 释放节点占用的资源
	         */
	        if(tmp->msgAddr)
	        {
	            free_manager(c,tmp->msgAddr,(enum_malloc_method)tmp->malloc_flags);
	        }
	        tmp = next;
	        
	    }
	    // trace("#######2#######total is %lu ,head=0x%8x,tail=0x%8x\n",queue->totalSize,queue->head,queue->tail);
	    
	    MutexLeave(c->hMutex);    
	    return 0;    
	}

	int NetQueueOverLimit(struct conn *c)
	{
	    NetQueue_t *queue = NULL;  
	    //queue = &c->DataTransferQueue[ICaptureManager::instance()->GetLogicChnNum()+ 1];
		queue = &c->DataTransferQueue[0];
	    if(NULL ==queue)
	    {
	         return 0;    
	    }
	    if(queue->totalSize > 100*1024)
	    {
	        return 1;
	    }
	    else
	    {
	        return 0;
	    }
	}

}



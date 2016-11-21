#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "CaptureList.h"
#include "ipc_inc.h"



int CapListCreate(BLOCK_LIST *list)
{
	int s32Ret;
	memset(list,0,sizeof(BLOCK_LIST));
	s32Ret = pthread_mutex_init( &(list->lock), NULL );
	assert( s32Ret == 0 );
	list->head = NULL;
	list->tail = NULL;
	return 0;
}

int CapListDestory(BLOCK_LIST *list)
{
	struct CAPTURE_LIST  *list_tmp;
	while(list->tail != NULL)
	{
		MemoryRelease(list->tail->avenc_buf.handle);
		list_tmp = list->tail->pre;
		free(list->tail);
		list->tail = list_tmp;
	}
	list->head = NULL;
	return 0;
}
struct CAPTURE_LIST *CapListAlloc(BLOCK_LIST *list, int len)
{
	int s32Ret;
	struct CAPTURE_LIST  *list_tmp;
	if(list == NULL || len > 2*1024*1024)
	{
		return NULL;
	}
	if(list->head == NULL)
	{
		list->head = malloc(sizeof(struct CAPTURE_LIST));
		if(list->head == NULL)
		{
			return NULL;
		}
		memset(list->head, 0, sizeof(struct CAPTURE_LIST));
		list->tail = list->head;
	}
	else
	{
		list->head->pre = malloc(sizeof(struct CAPTURE_LIST));
		if(list->head->pre == NULL)
		{
			return NULL;
		}
		memset(list->head->pre, 0, sizeof(struct CAPTURE_LIST));
		list_tmp = list->head;
		list->head = list->head->pre;
		list->head->next = list_tmp;

	}
	if(list->head->avenc_buf.ptr == NULL)
	{
		s32Ret = MemoryAlloc(&(list->head->avenc_buf), len);
		if (s32Ret < 0)
		{
			_printd("MemoryAlloc return < 0\n");
			return NULL;
		}
		gettimeofday(&(list->head->times),NULL);
		list->head->start_length = list->head->length;
		list->head->pack_head.FrameInfo[list->head->count].FramePos = list->head->length;
	}

	return list->head;
}

struct CAPTURE_LIST *CapListDetect(BLOCK_LIST *list, int len)
{
	int s32Ret;
	if(list->head == NULL)
	{
		list->head = malloc(sizeof(struct CAPTURE_LIST));
		if(list->head == NULL)
		{
			_printd("malloc error \n");
			return NULL;
		}
		memset(list->head, 0, sizeof(struct CAPTURE_LIST));
		list->tail = list->head;
	}

	if(list->head->avenc_buf.ptr == NULL)
	{
		s32Ret = MemoryAlloc(&(list->head->avenc_buf), len);
		if (s32Ret < 0)
		{
			_printd("MemoryAlloc error len = %d\n", len);
			return NULL;
		}
		gettimeofday(&(list->head->times),NULL);
		list->head->start_length = list->head->length;
		list->head->pack_head.FrameInfo[list->head->count].FramePos = list->head->length;
	}
	return list->head;
}

int CapListFree(BLOCK_LIST *list, struct CAPTURE_LIST *node)
{
	if(list->tail == node || list->tail != NULL)
	{
		list->tail = list->tail->pre;
		if(list->tail == NULL)
		{
			list->head = NULL;
		}
		free(node);
	}
	return 0;
}

struct CAPTURE_LIST *CapListHead(BLOCK_LIST *list)
{
	struct CAPTURE_LIST *list_tmp;
	if(list->head == NULL)
	{
		return NULL;
	}
	else
	{
		list_tmp = list->head;
		return list_tmp;
	}
	return NULL;
}


struct CAPTURE_LIST *CapListGet(BLOCK_LIST *list)
{
	struct CAPTURE_LIST *list_tmp;
	if(list->tail == NULL)
	{
		list->head = NULL;
		return NULL;
	}
	else
	{
		list_tmp = list->tail;
		return list_tmp;
	}
	return NULL;
}

int CapListInfo(BLOCK_LIST *list, BLOCK_INFO *info)
{
	struct CAPTURE_LIST  *list_tmp;
	if(info == NULL)
	{
		return -1;
	}
	info->block_len = 0;
	info->packet_count = 0;
	if(list == NULL)
	{
		return 0;
	}
	list_tmp = list->tail;
	while(list_tmp != NULL)
	{
		info->packet_count ++;
		info->block_len += list_tmp->length;
		list_tmp = list_tmp->pre;
	}
	return 0;
}


void CapList2Next(struct CAPTURE_LIST *pbuf)
{
	if(NULL == pbuf)
	{
		printf("%s - %d: err\r\n", __func__, __LINE__);
		return ;
	}

	int count = pbuf->count;
	if(	pbuf->pack_head.FrameInfo[count].FrameType==0&&
  		pbuf->pack_head.FrameInfo[count].FrameFlag==0&&
  		pbuf->pack_head.FrameInfo[count].FramePos==0&&
  		pbuf->pack_head.FrameInfo[count].FrameLength==0&&
  		pbuf->pack_head.FrameInfo[count].DataLength==0)
	{
		//printf("%s:0-0-0-0-0\r\n", __func__);
		return;
	}
	pbuf->start_length = pbuf->length;
	pbuf->count++;
}


struct CAPTURE_LIST * StreamCopy(BLOCK_LIST *pList, struct CAPTURE_LIST *pbuf,
		uchar * SrcAddr, int Length,STREAM_INFO *pStreamInfo)
{

	if(pList == NULL||pbuf == NULL||SrcAddr == NULL||pStreamInfo == NULL)
	{
		return NULL;
	}
	if(Length < 0 || Length >2 *1024*1024)//mosaic????
	{
		ERR_PRINT("Error : mosaic!!!!!!!!!\n");
		return NULL;
	}

	while(Length > 0)
	{
		if(pbuf == NULL)
		{
			return NULL;
		}
		if(pbuf->count >= 8)
		{

			if(pStreamInfo->HeadTail & PACK_CONTAIN_FRAME_HEAD)
			{
				pbuf->ready = 1;
			}
			else
			{
				pbuf->count ++;//TODO:此处走到会导致越界,不过，应该永远走不到这儿
				pbuf->ready = 1;
				printf("file:%s, line:%d\n", __FILE__, __LINE__);exit(-1);
			}

			pbuf = CapListAlloc(pList, pStreamInfo->PacketLen);
			if(pbuf == NULL)
			{
				return NULL;
			}
			pbuf->pack_head.FrameInfo[pbuf->count].FramePos = pbuf->length;
		}
		else if( (pbuf->avenc_buf.size - pbuf->length) <= Length
			&& pbuf->count < 8)
		{

			if(pStreamInfo->HeadTail & PACK_CONTAIN_FRAME_HEAD)
			{

				pbuf->ready = 1;
			}
			else
			{
				if( ( (pbuf->avenc_buf.size - pbuf->length)==Length ) &&
						(pStreamInfo->HeadTail& PACK_CONTAIN_FRAME_TRAIL) )
				{
					pbuf->pack_head.FrameInfo[pbuf->count].FrameFlag |= pStreamInfo->HeadTail;
				}

				pbuf->pack_head.FrameInfo[pbuf->count].FrameLength = pStreamInfo->FrameLength;
				pbuf->pack_head.FrameInfo[pbuf->count].FrameType = pStreamInfo->FrameType;
				memcpy(pbuf->avenc_buf.ptr, SrcAddr, (pbuf->avenc_buf.size - pbuf->length));

				Length -= (pbuf->avenc_buf.size - pbuf->length);
				SrcAddr += (pbuf->avenc_buf.size - pbuf->length);
				pbuf->avenc_buf.ptr += (pbuf->avenc_buf.size - pbuf->length);
				pbuf->length += (pbuf->avenc_buf.size - pbuf->length);
				pbuf->pack_head.FrameInfo[pbuf->count].DataLength = pbuf->length - pbuf->start_length;

				pbuf->ready = 1;
				pbuf->count ++;
			}


			pbuf = CapListAlloc(pList, pStreamInfo->PacketLen);
			if(pbuf == NULL)
			{
				return NULL;
			}
			pbuf->pack_head.FrameInfo[pbuf->count].FramePos = pbuf->length;
		}
		//安帧上传时只会进这里
		else
		{
			if(pStreamInfo->HeadTail & PACK_CONTAIN_FRAME_HEAD )
			{
				pbuf->pack_head.FrameInfo[pbuf->count].FramePos = pbuf->length;
				pbuf->pack_head.FrameInfo[pbuf->count].FrameFlag |= pStreamInfo->HeadTail;
			}

			pbuf->pack_head.FrameInfo[pbuf->count].FrameLength = pStreamInfo->FrameLength;
			pbuf->pack_head.FrameInfo[pbuf->count].FrameType = pStreamInfo->FrameType;

			memcpy(pbuf->avenc_buf.ptr, SrcAddr, Length);
			pbuf->avenc_buf.ptr += Length;
			pbuf->length += Length;
			pbuf->pack_head.FrameInfo[pbuf->count].DataLength = pbuf->length - pbuf->start_length;

			if(pStreamInfo->HeadTail & PACK_CONTAIN_FRAME_TRAIL)
			{
				pbuf->pack_head.FrameInfo[pbuf->count].FrameFlag |= pStreamInfo->HeadTail;
				pbuf->start_length = pbuf->length;
				pbuf->ready = 1;
			}
			Length = 0;
		}
	}

	return pbuf;
}



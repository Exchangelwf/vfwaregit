#include "Memory.h"

#ifndef HI_SDK3515_103
#define SIZE_IN_PAGE		(32 * 1024 * 1024)	//32M
#else
#define SIZE_IN_PAGE		(16 * 1024 * 1024)	//16M
#endif

void *ExtraMemoryAlloc(int nSize)
{
	uchar * pImageBuf;

	printf("-------------%s----------%d   nsize=%d\r\n", __func__, __LINE__, nSize);
	if ( nSize > SIZE_IN_PAGE || nSize <= 0)
	{
		printf("ExtraMemoryAlloc: Error , nSize > MaxSize)");
		return NULL;
	}
	
	pImageBuf = (uchar*)malloc(nSize);

	return pImageBuf;
}


void ExtraMemoryFree(void *pBuf)
{
	free(pBuf);
	return ;
}
void *ExtraMemoryCopy(void *pDest, const void *pSrc, int nCount)
{
	return memcpy(pDest, pSrc, nCount);
}
void *ExtraMemorySet(void *pDest, int c, int nCount)
{
	return memset(pDest, c, nCount);
}


#include "DVRDEF.H"
#include <errno.h>
#include <stdlib.h>
#include <semaphore.h>  //sem_t

#include "CSemaphore.h"

int SemaphoreCreate(VD_HANDLE *semId, uint cnt)
{
	sem_t *	sem;
	
	sem = (sem_t *)malloc(sizeof(sem_t));
	if (sem == NULL)
	{
		ERR_PRINT("SemaphoreCreate Malloc Fail\n");
		return -1;
	}
	if (sem_init(sem, 0, cnt) == 0)
	{
		*semId = (VD_HANDLE)sem;
		return 0;
	}
	
	ERR_PRINT("Sem_init Fail\n");
	return -1;
} 


int SemaphoreDestory(VD_HANDLE semId)
{
	if (sem_destroy((sem_t *)semId) == -1)
	{
		perror("sem destroy");
		return -errno;
	}
	else
	{
		free((void*)semId);
		return 0;
	}
} 


int SemaphorePost(VD_HANDLE semId)
{
  return sem_post((sem_t *)semId);
} 


int SemaphorePend(VD_HANDLE semId)
{
  return sem_wait((sem_t *)semId);
} 



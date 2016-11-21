#ifndef __COLOR2GREY_H__
#define __COLOR2GREY_H__

typedef enum{
	IRCUT_TIMER		= 0,
	IRCUT_FULLCOLOR,
	IRCUT_BWCOLOR,
	IRCUT_AUTO,
}ENU_IPC_IRCUTMODE;

int GetIrCutStatus();
int SetIrCutStatus(int status/*0 auto 1 open 2 close*/);

void* ColorGreyTask(void *noargs);
#endif

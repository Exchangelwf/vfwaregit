#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <asm/types.h>                
#include <net/if.h>
#include <net/if_arp.h>
#include <net/route.h>
#include <sys/errno.h>
#include <stdio.h>
#include <linux/if_ether.h>
#include <linux/sockios.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "DVRDEF.H"
#include "Net.h"

#define DEFAULTE_RESOLVE_CONFIG "/mnt/mtd/Config/resolv.conf"

int NetSetDNSAddress(const char *pprimary, const char *psecondary)
{
    FILE *fp;

    if ((fp = fopen(DEFAULTE_RESOLVE_CONFIG, "w+")) == NULL)
    {
        ERR_PRINT("Fopen Resolv.conf Failed\n");
        return -1;
    }

    if (pprimary != NULL)
    {
        fprintf(fp, "nameserver %s\n", pprimary);
    }
    if (psecondary != NULL)
    {
        fprintf(fp, "nameserver %s\n", psecondary);
    }

    fclose(fp);

    return 0;
}


int NetGetDNSAddress(char *pprimary,const int iPlen ,char *psecondary, const int iSlen)
{
    FILE *fp;
    char line[64] = {0};

    if ((iPlen < 16) || (iSlen < 16)) 
    {
        ERR_PRINT("IPlen = %d\n",iPlen);
        return -1;
    }
    if ((fp = fopen(DEFAULTE_RESOLVE_CONFIG, "r")) != NULL)
    {
        if((fgets(line, sizeof(line), fp)) != NULL)
        {
            if (strstr(line, "nameserver") != NULL)
            {
                sscanf(line, "nameserver %s", pprimary);
            }
        }
        if((fgets(line, sizeof(line), fp)) != NULL)
        {
            if (strstr(line, "nameserver") != NULL)
            {
                sscanf(line, "nameserver %s", psecondary);
            }
        }

        fclose(fp);
    }
    else
    {
        strcpy(pprimary, "202.101.172.35");
        strcpy(psecondary, "202.101.172.35");
    }
    
    return 0;
}

int NetSetDNSDomain(const char * pDomain)
{
    return 0;
}

int NetSetDNSHostName(const char * pHostName)
{
	pid_t child_pid;
	if((child_pid = vfork()) < 0){
		perror("vfork");
		return -1;
	}else if(child_pid == 0){
		execl("hostname", "hostname", pHostName, (char *) 0); 
	}else{
		waitpid(child_pid, NULL, 0);
	}
    return 0;
}


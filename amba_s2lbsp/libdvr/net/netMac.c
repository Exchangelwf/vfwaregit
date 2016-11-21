#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <netinet/if_ether.h>
#include <netinet/in.h>

#include <arpa/inet.h>
#include <net/if.h>
#include <errno.h>
#include "ipc_inc.h"

int NetGetMAC(const char *pEthName, char *pEthMac, const int iEthMacLen)
{
	unsigned char mac[7];
	int sockfd;
	struct ifreq req;
	if ( ( sockfd = socket (PF_INET,SOCK_DGRAM,0 ) ) ==-1 )
	{
		fprintf ( stderr,"NetGetMAC:Sock Error:%s\n\a",strerror ( errno ) );
		return ( -1 );
	}

	memset ( &req,0,sizeof ( req ) );
	strcpy ( req.ifr_name, pEthName);
	if ( ioctl ( sockfd,SIOCGIFHWADDR, ( char * ) &req ) ==-1 )
	{
		fprintf ( stderr,"NetGetMAC ioctl SIOCGIFHWADDR:%s\n\a",strerror ( errno ) );
		close ( sockfd );
		return ( -1 );
	}

	memcpy ( mac,req.ifr_hwaddr.sa_data,6 );
	close ( sockfd );

	snprintf(pEthMac, iEthMacLen, "%02x:%02x:%02x:%02x:%02x:%02x", 
	mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
	_printd("MAC======================%s\n",pEthMac);
	return 0;
}

int NetSetMAC(const char *pEthName, const char *pEthMac)
{
    char ethname[8];
    char str[16] = {0};
    
    if (!pEthMac)
    {
        _printd("pEthMac Is NULL\n");
        return -1;
    }
    
    if (NULL == pEthName)
    {
	strncpy(str, "eth0", sizeof(str));
    }
    else
    {
        strncpy(str, pEthName, sizeof(str));
    }
    
    if ((strcmp("eth0", str) == 0) ||
        (strcmp("HWADDR0", str) == 0))
    {
        strcpy(ethname, "ethaddr");
    }
    else
    {
        _printd("Not Found Device %s\n", str);
        return -1;
    }
    
	pid_t child_pid;
	if((child_pid = vfork()) < 0){
		perror("vfork");
		return -1;
	}else if(child_pid == 0){
		execl("/usr/local/bin/upgrade_partition", "upgrade_partition", "-E", pEthMac, (char *) 0); 
	}else{
		waitpid(child_pid, NULL, 0);
	}

    return 0;
}


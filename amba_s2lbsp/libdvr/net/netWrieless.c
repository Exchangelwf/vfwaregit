#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/types.h>
#include <linux/netlink.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/if_arp.h>
#include "Cmos.h"
#include "Net.h"

#define WRIELESS_CONFIG "/mnt/mtd/Config/3g/accountinfo"
#define WRIELESS_ENABLE "/mnt/mtd/Config/3g/3g-enable"
#define WRIELESS_TYPE	"/mnt/mtd/Config/3g/3g-type"

static int get_ip(char *ip,char *ifname)
{
	int fd;
    struct ifreq ifr;
	struct sockaddr_in *s_in;

	if(ip == NULL || ifname == NULL)
		return -1;
	fd = socket(AF_INET,SOCK_DGRAM,0);
	if(fd < 0){

		perror("sock");
		return -1;
	}

	memset(&ifr,0,sizeof(struct ifreq));
    memcpy(ifr.ifr_name, ifname,strlen(ifname));
    ifr.ifr_addr.sa_family = AF_INET;
    if (ioctl(fd, SIOCGIFADDR, &ifr) == 0) {

		s_in = (struct sockaddr_in *)&ifr.ifr_addr;
		memcpy(ip,inet_ntoa(s_in->sin_addr),16);
		close(fd);
		return 0;
		#if 0
        memcpy(ifr.ifr_name, ifname,strlen(ifname));
        if (ioctl(fd, SIOCGIFDSTADDR, &ifr) < 0){
			memcpy(ip,"0.0.0.0",16);
			close(fd);
			return -1;
        }
        else{

			s_in = (struct sockaddr_in *)&ifr.ifr_dstaddr;
			memcpy(ip,inet_ntoa(s_in->sin_addr),16);
			close(fd);
			return 0;
		}
		#endif
    }
	
	memcpy(ip,"0.0.0.0",16);
	close(fd);
    return -1;
}

extern int get_3g_signal(char *dev_name) ;
extern int NetGetDialIpAddress(char *pDailIp);
int NetGetWrielessConfig(NET_WRIELESS_CONFIG *m_pConfig)
{
	FILE *fp;
	char *dev = "/dev/ttyUSB1";
	char pppoeip[16];
	
	if(m_pConfig == NULL){

		ERR_PRINT("NULL m_pConfig\n");
		return -1;
	}

	memset((void *)m_pConfig,0,sizeof(NET_WRIELESS_CONFIG));
	
	fp = fopen(WRIELESS_CONFIG,"rb");

	if(fp == NULL)
	{
		ERR_PRINT("%s open failed\n",WRIELESS_CONFIG);
		return -1;
	}

	fscanf(fp,"%s %s\n",m_pConfig->username,m_pConfig->password);
	fclose(fp);
	
	fp = fopen(WRIELESS_ENABLE,"rb");
	
	if(fp == NULL)
	{
		ERR_PRINT("%s open failed\n",WRIELESS_ENABLE);
		return -1;
	}

	fscanf(fp,"%d\n",&(m_pConfig->enable));
	fclose(fp);
	fp = fopen(WRIELESS_TYPE,"rb");
	if(fp == NULL)
	{
		ERR_PRINT("%s open failed\n",WRIELESS_TYPE);
		return -1;
	}

	fscanf(fp,"%d\n",&(m_pConfig->type)); 
	fclose(fp);

	memset(pppoeip,0,16);
	memset(m_pConfig->ip,0,16);
#if 0
	if(NetGetDialIpAddress(pppoeip) < 0){
		get_ip(m_pConfig->ip,"ppp0");
	}
	else{

		get_ip(m_pConfig->ip,"ppp0");
		if(strcmp(m_pConfig->ip,pppoeip) == 0){
			memset(m_pConfig->ip,0,16);
			get_ip(m_pConfig->ip,"ppp1");
		}
	}
#else
	get_ip(m_pConfig->ip,"ppp0");
#endif
	if(m_pConfig->type == 0)
		dev = "/dev/ttyUSB2";
	//printf("dev = %s\n",dev);
	m_pConfig->signal = get_3g_signal(dev);
	if(m_pConfig->signal == -1)
		m_pConfig->signal = 0;
	return 0;
}

int NetSetWrielessConfig(NET_WRIELESS_CONFIG *m_pConfig)
{
	FILE *fp;

	if(m_pConfig == NULL){

		ERR_PRINT("NULL m_pConfig\n");
		return -1;
	}
	
	fp = fopen(WRIELESS_CONFIG,"w+");

	if(fp == NULL)
	{
		ERR_PRINT("%s open failed\n",WRIELESS_CONFIG);
		return -1;
	}

	fprintf(fp,"%s %s\n",m_pConfig->username,m_pConfig->password);
	fclose(fp);

	fp = fopen(WRIELESS_ENABLE,"w+");

	if(fp == NULL)
	{
		ERR_PRINT("%s open failed\n",WRIELESS_ENABLE);
		return -1;
	}

	fprintf(fp,"%d\n",m_pConfig->enable); 
	fclose(fp);

	fp = fopen(WRIELESS_TYPE,"w+");
	if(fp == NULL)
	{
		ERR_PRINT("%s open failed\n",WRIELESS_TYPE);
		return -1;
	}

	fprintf(fp,"%d\n",m_pConfig->type); 
	fclose(fp);
	return 0;
}


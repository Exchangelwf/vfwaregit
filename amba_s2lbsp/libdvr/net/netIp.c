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
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>

#include "DVRDEF.H"
#include "Net.h"
#include "Cmos.h"

#define NET_WORK         "/mnt/mtd/Config/network"
#define DEFAULTE_NETCARD "/mnt/mtd/Config/default_netcard"
#define DIR_DEV_NET      "/proc/net/dev"

#define CMOS_OFFSET      40
#define DATA_LEN         (4 * 1024)

enum
{
    TYPE_IP      = 0,
    TYPE_NETMASK = 1,
    TYPE_GATEWAY = 2,
    TYPE_PRIO    = 3,
    TYPE_TCPPORT = 4,
    TYPE_MAX     = 5
};

#define ifreq_offsetof(x)  offsetof(struct ifreq, x)
#define SET_SA_FAMILY(addr, family)                        \
    memset ((char *) &(addr), '\0', sizeof(addr));        \
    addr.sa_family = (family);

#define SIN_ADDR(x)        (((struct sockaddr_in *) (&(x)))->sin_addr.s_addr)


#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define    MAC_BCAST_ADDR        "\xff\xff\xff\xff\xff\xff"

int NetSendarp(const char *pEthName);

int GetEthAttrFromFile(const char *ifname, const int type, char *str, const int str_length);

char *IP2Str(const unsigned int iIP, char *pStr)
{
    unsigned char i;
    unsigned char cIP[4];

    for (i = 0; i < 4; i++)
    {
        cIP[i] = (unsigned char)((iIP>>(i*8)) & (0xff));
    }

    sprintf(pStr, "%d.%d.%d.%d\n", cIP[0], cIP[1], cIP[2], cIP[3]);
    return pStr;
}

int Str2IP(const char *pStr)
{
    unsigned int uiIP[4];
    int val;

    val = sscanf(pStr, "%d.%d.%d.%d\n", &uiIP[0], &uiIP[1], &uiIP[2], &uiIP[3]);
    
    if (val != 4)
    {
        return -1;
    }
    
    if ((uiIP[0]>0xff)||(uiIP[1]>0xff)||(uiIP[2]>0xff)||(uiIP[3]>0xff))
    {
        return -1;
    }
    
    val = ((uiIP[3]) << 24) | (uiIP[2] << 16) | (uiIP[1] << 8) | (uiIP[0] << 0);
    return val;
}

/* Set eth attibute to the ifreq*/
int SetEthAttr(const char *ifname, const int type, const char *str)
{
    struct ifreq ifr;
    struct sockaddr_in sai;
    int sockfd;
    int ioctl_cmd;
    char err_str[24] = {0};
    
    if ((type == TYPE_IP) || (type == TYPE_NETMASK))
    {
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
            ERR_PRINT("Socket Failed\n");
            return -1;
        }
        strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));
        
        sai.sin_family = AF_INET;
        sai.sin_port = 0;
        sai.sin_addr.s_addr = inet_addr(str);
         
        if (sai.sin_addr.s_addr == -1)
        {
            ERR_PRINT("Express Error\n");
            close(sockfd);
            return -1;
        }

        if (type == TYPE_IP)
        {
            strcpy(err_str, "set ip error\n");
            ioctl_cmd = SIOCSIFADDR;
            memcpy((((char *)(&ifr)) + ifreq_offsetof(ifr_addr)), 
                &sai, sizeof(struct sockaddr));
        }
        else
        {
            strcpy(err_str, "set netmask error\n");
            ioctl_cmd = SIOCSIFNETMASK;
            memcpy((((char *)(&ifr)) + ifreq_offsetof(ifr_netmask)),
                &sai, sizeof(struct sockaddr));
        }

        if (ioctl(sockfd, ioctl_cmd, &ifr) < 0)
        {
            ERR_PRINT("%s\n", err_str);
            close(sockfd);
            return -1;
        }
        
        if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) < 0)
        {
            ERR_PRINT("SIOCGIFFLAGS Failed\n");
            close(sockfd);
            return -1;
        }

        ifr.ifr_flags |= (IFF_UP | IFF_RUNNING);
        if (ioctl(sockfd, SIOCSIFFLAGS, &ifr) < 0)
        {
            ERR_PRINT("SIOCSIFFLAGS Failed\n");
            close(sockfd);
            return -1;
        }

        if (type == TYPE_IP)
        {
            LIBDVR_PRINT("Set Ip Successful\n");
        }
        else
        {
            LIBDVR_PRINT("Set NetMask Successful\n");
        }
        close(sockfd);
        
    }
    else if(type == TYPE_GATEWAY)
    {

	const char* pEthName = ifname;
	const char* pGageway= str;

	struct rtentry rt;
	struct sockaddr_in *s_in;

	    NetDevice eth_device[10];
	    char HostIP[32];
	    char Gateway[32];
	    char Netmask[32];

	    int eth_num;
	    int i;

	    if(pEthName == NULL || pGageway ==NULL)
	    {
	        ERR_PRINT("pEthName == NULL || pGageway == NULL\n");
	        return -1;
	    }

	    eth_num = NetGetEthDevice(eth_device, 10);
	    if (eth_num == 0)
	    {
	        ERR_PRINT("NetGetEthDevice Failed\n");
	        return -1;
	    }
	    
	    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	    {
	        ERR_PRINT("Socket Failed\n");
	        return -1;
	    }

	    //DEL ALL INTERFACE's ROUTE
	    memset(&rt, 0, sizeof(struct rtentry));
	    s_in = (struct sockaddr_in *)&(rt.rt_dst);
	    s_in->sin_family = AF_INET;
	    s_in->sin_port = 0;
	    s_in->sin_addr.s_addr = INADDR_ANY;
	    rt.rt_flags = RTF_UP | RTF_MODIFIED;
	    rt.rt_flags |= RTF_GATEWAY;
	    
	    for(i = 0; i < eth_num; i++)
	    {
	        rt.rt_dev = eth_device[i].name;
	        ioctl(sockfd, SIOCDELRT, &rt);
	    }
	    
	    if (GetEthAttrFromFile(pEthName, TYPE_IP, HostIP, 32) < 0)
	    {
	        ERR_PRINT("Get Default Gateway Error\n");
	        return -1;
	    }
	    if (GetEthAttrFromFile(pEthName, TYPE_GATEWAY, Gateway, 32) < 0)
	    {
	        ERR_PRINT("Get Default Gateway Error\n");
	        return -1;
	    }
	    if (GetEthAttrFromFile(pEthName, TYPE_NETMASK, Netmask, 32) < 0)
	    {
	        ERR_PRINT("Get Default Gateway Error\n");
	        return -1;
	    }
	    
	    //DEL ROUTE
	    memset(&rt, 0, sizeof(struct rtentry));
	    rt.rt_flags = RTF_UP | RTF_MODIFIED;
	    rt.rt_dev = (char *)pEthName;
	    
	    s_in = (struct sockaddr_in *)&(rt.rt_genmask);
	    s_in->sin_family = AF_INET;
	    s_in->sin_port = 0;
	    s_in->sin_addr.s_addr = inet_addr(Netmask);
	    s_in = (struct sockaddr_in *)&(rt.rt_dst);
	    s_in->sin_family = AF_INET;
	    s_in->sin_port = 0;
	    s_in->sin_addr.s_addr = inet_addr(HostIP)&inet_addr(Netmask);
	    ioctl(sockfd, SIOCDELRT, &rt);
		
	    //ADD ROUTE
	    memset(&rt, 0, sizeof(struct rtentry));
	    rt.rt_flags = RTF_UP | RTF_MODIFIED;
	    rt.rt_dev = (char *)pEthName;
	    rt.rt_metric = 0;
		
	    s_in = (struct sockaddr_in *)&(rt.rt_genmask);
	    s_in->sin_family = AF_INET;
	    s_in->sin_port = 0;
	    s_in->sin_addr.s_addr = inet_addr(Netmask);
	    s_in = (struct sockaddr_in *)&(rt.rt_dst);
	    s_in->sin_family = AF_INET;
	    s_in->sin_port = 0;
	    s_in->sin_addr.s_addr = inet_addr(HostIP)&inet_addr(Netmask);

	    if (ioctl(sockfd, SIOCADDRT, &rt) < 0)
	    {
	        ERR_PRINT("SetGateWay SIOCADDRT Failed\n");
	        close(sockfd);
	        return -1;
	    }
		
	    //ADD SET ROUTE
	    memset(&rt, 0, sizeof(struct rtentry));
	    rt.rt_flags = RTF_UP | RTF_MODIFIED;
	    rt.rt_dev = (char *)pEthName;
	    rt.rt_metric = 0;
	    
	    s_in = (struct sockaddr_in *)&(rt.rt_gateway);
	    s_in->sin_family = AF_INET;
	    s_in->sin_port = 0;
	    s_in->sin_addr.s_addr = inet_addr(pGageway);
	    s_in = (struct sockaddr_in *)&(rt.rt_dst);
	    s_in->sin_family = AF_INET;
	    s_in->sin_port = 0;
	    s_in->sin_addr.s_addr = INADDR_ANY;
	    rt.rt_flags |= RTF_GATEWAY;
	    
	    if (ioctl(sockfd, SIOCADDRT, &rt) < 0)
	    {
	        ERR_PRINT("SetGateWay SIOCADDRT Failed\n");
	        close(sockfd);
	        return -1;
	    }
	    
	    close(sockfd);
	    LIBDVR_PRINT("SetGateWay Successful\n");
    }
    else
    {
        ERR_PRINT("Type Error\n");
        return -1;
    }

    return 0;
}

/* Save To NET_WORK File*/
int SaveEthAttr2File(const char *ifname, const int type, const char *str)
{
	if (str == NULL)
	{
		return 0;
	}

	FILE *fp = NULL;
	char str_type[10];
	char line[32];
	char *buf = NULL;
	//int offset[3] = {CMOS_IP, CMOS_MASK, CMOS_GATE};
	int found = 0;

	switch(type)
	{
		case TYPE_IP:
		{
			 strcpy(str_type, "HOSTIP");
			 break;
		}
		case TYPE_NETMASK:
		{
			 strcpy(str_type, "SUBMASK");
			 break;
		}
		case TYPE_GATEWAY:
		{
			 strcpy(str_type, "GATEWAYIP");
			 break;
		}
		case TYPE_TCPPORT:
		{
			 strcpy(str_type, "TCPPORT");
			 break;
		}
		case TYPE_PRIO:
			{
			 strcpy(str_type, "PRIO");
			 break;
		}
		default:
			return -1;
	}

    if ((fp = fopen(NET_WORK, "r+")) != NULL)
    {
        buf = (char *)malloc(DATA_LEN);
        if (buf == NULL)
        {
            ERR_PRINT("Malloc Failed\n");
            return -1;
        }
        memset(buf, 0, DATA_LEN);
        
        while(fgets(line, sizeof(line), fp))
        {
            if(strstr(line, str_type) != NULL)
            {
                
                sprintf(line, "%s = %s\n", str_type, str);
                
                found = 1;
            }
			
            strcat(buf, line);
        }

        if (!found)
        {
            sprintf(line, "%s = %s\n", str_type, str);
            strcat(buf, line);
        }
        
        fclose(fp);
        
        if ((fp = fopen(NET_WORK, "w+")) != NULL)
        {
            fprintf(fp, "%s", buf);
            fclose(fp);
        }
        
        free(buf);
		
    }
    else
    {
        if ((fp = fopen(NET_WORK, "w+")) != NULL)
        {
            fprintf(fp, "%s = %s\n", str_type, str);
            fclose(fp);
        }
    }

    return 0;
}

/* Read From NET_WORK File*/
int GetEthAttrFromFile(const char *ifname, const int type, char *str, const int str_length)
{
	
    FILE *fp = NULL;
    int eth_type = -1;
    char str_type[10] = {0};
    char filename[32] = {0};
    char line[80];
    //int offset[3] = {CMOS_IP, CMOS_MASK, CMOS_GATE};
    char ip_table[3][4][16] = 
/*        {{"192.168.1.108","255.255.255.0","192.168.1.1","eth0"},
        {"192.168.1.108","255.255.255.0","192.168.1.1","eth1"},
        {"192.168.0.108","255.255.0.0","192.168.0.1","eth2"}};
		*/
		 {{"192.168.1.88","255.255.255.0","192.168.1.1","eth0"},
	    {"192.168.1.88","255.255.255.0","192.168.1.1","eth1"},
	     {"192.168.0.88","255.255.0.0","192.168.0.1","eth2"}};
 	strncpy(filename, NET_WORK, sizeof(filename));
	eth_type = 0;
    
	switch(type)
	{
		case TYPE_IP:
		{
			 strcpy(str_type, "HOSTIP");
			 break;
		}
		case TYPE_NETMASK:
		{
			 strcpy(str_type, "SUBMASK");
			 break;
		}
		case TYPE_GATEWAY:
		{
			 strcpy(str_type, "GATEWAYIP");
			 break;
		}
		case TYPE_PRIO:
		{
			 strcpy(str_type, "PRIO");
			 strncpy(filename, DEFAULTE_NETCARD, sizeof(filename));
			 break;
		}		
	}

	if ((fp = fopen(filename, "r")) != NULL)
    {
        while(fgets(line, sizeof(line), fp))
        {
            if(strstr(line, str_type) != NULL)
            {
                sscanf(line, "%s = %s", str_type, str);
                fclose(fp);
                return 0;
            }
        }
        fclose(fp);    
    }
    
    if (type == TYPE_PRIO)
    {
        return -1;
    }
    
    strcpy(str, ip_table[eth_type][type]);
    return 0;
}

int NetGetEthDevice(NetDevice *pNetDev, int iMax)
{
    int ethnum = 0;
    char line[255];
    char ethname[10][30];
    int i = 0;
    FILE *fp;
    char *p = NULL;
    
    if(pNetDev == NULL)
    {
        return 0;
    }
    
    memset(pNetDev,0,iMax*sizeof(NetDevice));
    
    fp = fopen(DIR_DEV_NET, "r");
    if(fp == NULL)
    {
        ERR_PRINT("Open File %s Failed!\n", DIR_DEV_NET);
        strcpy(&ethname[10][0], "eth0");
        ethnum ++;
    }
    else
    {
        while (fgets(line, sizeof(line), fp))
        {
            p = NULL;
            if ((p = strstr(line, "eth")) != NULL)
            {
                i = 0; 
                while(p[i] != ':')
                {
                    ethname[ethnum][i] = p[i];
                    i++;
                }
                ethname[ethnum][i] = '\0';
                ethnum++;
            }
        }
     
        fclose(fp);
    }
	
    for(i = 0; i < iMax && i < ethnum;i++)
    {
        strcpy(pNetDev[i].name,&ethname[i][0]);
        if(strcmp(pNetDev[i].name,"eth0")==0)
        {
            pNetDev[i].valid = 1;
            pNetDev[i].trans_media = NM_WIRED;
        }
        else if(strcmp(pNetDev[i].name,"eth1")==0)
        {
            pNetDev[i].valid = 0;
            pNetDev[i].trans_media = NM_WIRED;
        }
        else if(strcmp(pNetDev[i].name,"eth2")==0)
        {
            pNetDev[i].valid = 1;
            pNetDev[i].trans_media = NM_WIRELESS;
        }
    }
    return i;
}


int NetSetDefaultEth(const char *pEthName)
{
    return -1;
}


int NetGetDefaultEth(char *pEthName)
{
    return 0;
}

int NetGetEthName( char *pEthName, int EthNameLen, int *iEthnum)
{
    return 0;
}

/////////////////////////////////////////////////APIs
int NetGetHostIP(const char *pEthName, char *pHostIP, const int iHLen, 
                                char *pNetmask, const int iNLen)
{
    char str[16] = {0};
    
    if (NULL == pEthName)
    {
	strncpy(str, "eth0", sizeof(str));
    }
    else
    {
        strncpy(str, pEthName, sizeof(str));
    }
    
    if (GetEthAttrFromFile(str, TYPE_IP, pHostIP, iHLen) < 0)
    {
        ERR_PRINT("Get Default Ip Error\n");
        return -1;
    }
    
    if (GetEthAttrFromFile(str, TYPE_NETMASK, pNetmask, iNLen) < 0)
    {
        ERR_PRINT("Get Default Netmask Error\n");
        return -1;
    }    
	printf ("#########################################################\n[%s][%d][%s]\n", __FILE__, __LINE__, __func__);
    return 0;
}

int NetGetGateway(const char *pEthName, char *pGateway, const int iGLen)
{
    char str[16] = {0};
    
    if (NULL == pEthName)
    {
	strncpy(str, "eth0", sizeof(str));
    }
    else
    {
        strncpy(str, pEthName, sizeof(str));
    }
    
    if (GetEthAttrFromFile(str, TYPE_GATEWAY, pGateway, iGLen) < 0)
    {
        ERR_PRINT("Get Default Gateway Error\n");
        return -1;
    }
    
    return 0;
}


int NetSetHostIP(const char *pEthName, const char *pHostIP, const char *pNetmask)
{
    char str[16] = {0};
    
    if (NULL == pEthName)
    {
	strncpy(str, "eth0", sizeof(str));
    }
    else
    {
        strncpy(str, pEthName, sizeof(str));
    }
    
    if (SetEthAttr(str, TYPE_IP, pHostIP) == 0)
    {
        SaveEthAttr2File(str, TYPE_IP, pHostIP);
    }
    
    if (SetEthAttr(str, TYPE_NETMASK, pNetmask) == 0)
    {
        SaveEthAttr2File(str, TYPE_NETMASK, pNetmask);
    }
    
    return NetSendarp(str);
    
}

int NetSetGateway(const char *pEthName, const char *pGateway)
{
    char str[16] = {0};
    
    if (NULL == pEthName)
    {
	strncpy(str, "eth0", sizeof(str));
    }
    else
    {
        strncpy(str, pEthName, sizeof(str));
    }
    
    if (SetEthAttr(str, TYPE_GATEWAY, pGateway) == 0)
    {
        SaveEthAttr2File(str, TYPE_GATEWAY, pGateway);
		
        NetSendarp(str);
        return 0;
    }
    else
    {
        NetSendarp(str);
        return -1;
    }
    return 0;
}

//提供给ULK使用，将TCP端口保存到network中，只做保存用
int NetSetTCPPort(const char *pEthName, const char *TCPPort)
{
    char str[16] = {0};
    
    if (NULL == pEthName)
    {
	strncpy(str, "eth0", sizeof(str));
    }
    else
    {
        strncpy(str, pEthName, sizeof(str));
    }
    
    SaveEthAttr2File(str, TYPE_TCPPORT, TCPPort);
	
    NetSendarp(str);
    return 0;
}

/// NetSendArp函数
struct arpMsg
{
    /* 前面是ethernet头 */
    struct  ethhdr ethhdr; 
    /* 下面是arp协议 */	
    u_short htype;        
    u_short ptype;        
    u_char  hlen;        
    u_char  plen;        
    u_short operation;  
	
    u_char  sHaddr[6];    
    u_char  sInaddr[4];    
    u_char  tHaddr[6];    
    u_char  tInaddr[4];    
    u_char  pad[18];    
};

static void llip_mkArpMsg(int opcode, u_long tInaddr, u_char *tHaddr,
        u_long sInaddr, u_char *sHaddr, struct arpMsg *msg)
{
    int    i;
    char    t_Haddr[6];

    for (i = 0; i < 6; i++)
    {
        t_Haddr[i] = 0;
    }

    bzero(msg, sizeof(*msg));
    bcopy(tHaddr, msg->ethhdr.h_dest, 6);        
    bcopy(sHaddr, msg->ethhdr.h_source, 6);        

    msg->ethhdr.h_proto = htons(ETH_P_ARP);        
    msg->htype = htons(1);                        
    msg->ptype = htons(ETH_P_IP);                
    msg->hlen = 6;                                
    msg->plen = 4;                                
    msg->operation = htons(opcode);                

    *((u_int *)msg->sInaddr) = sInaddr;            
    bcopy(sHaddr, msg->sHaddr, 6);                
    bcopy(tHaddr, t_Haddr, 6);
    memcpy((char *)msg->tInaddr, (char *)&sInaddr, 4);
}

static int llip_openRawSocket (int *s, u_short type)
{
    int optval = 1;

    if((*s = socket (AF_INET, SOCK_PACKET, htons (type))) == -1)
    {
        ERR_PRINT("SOCK_PACKET\n");
        return -1;
    }

    if(setsockopt (*s, SOL_SOCKET, SO_BROADCAST,
                &optval, sizeof (optval)) == -1)
    {
        ERR_PRINT("setsockopt\n");
        close(*s);
        return -1;
    }

    return 0;
}

int llip_sendGratuitousArps(char* device_name, u_long address,
        unsigned char source_hw_addr[6])
{
    int s;                    
    struct sockaddr addr;    
    struct arpMsg    arp;    

    if (llip_openRawSocket(&s, ETH_P_ARP) == -1)
    {
        return -1;
    }

    
    llip_mkArpMsg(ARPOP_REPLY, 0xffffffffU, (u_char*)MAC_BCAST_ADDR,
            address, (u_char*)source_hw_addr, &arp);
    bzero(&addr, sizeof(addr));
    strncpy(addr.sa_data, device_name, sizeof(addr.sa_data));

    

    if (sendto(s, &arp, sizeof(arp), 0, &addr, sizeof(addr)) < 0 )
    {
        ERR_PRINT("sendto arp wrong");
        close(s);
        return -1;
    }

    close(s);

    return 0;
}

/* IP从配置文件获取,MAC */
int NetSendarp(const char *pEthName)
{
    unsigned int ip;
    char host_ip[20], ethmactmp[20];
    char ethname[32];
    static unsigned char ethmac[6];
    
    int i = 0;
    
    strcpy(ethname, pEthName);
    if (GetEthAttrFromFile(ethname, TYPE_IP, host_ip, sizeof(host_ip)) < 0)
    {
        ERR_PRINT("Get Ip Error\n");
        return -1;
    }
    
    ip = inet_addr(host_ip);    
    if (NetGetMAC(ethname, ethmactmp, sizeof(ethmactmp)) == -1)
    {
        ERR_PRINT("Can Not Get Host Mac\n");
        return -1;
    }
    
    for (i = 0; i < 6; i++)
    {
        ethmactmp[(i+1)*3 - 1] = 0;
        ethmac[i] = strtoul(ethmactmp + i*3, 0, 16);
    }        

    llip_sendGratuitousArps(ethname, ip, ethmac);
    return 0;
}



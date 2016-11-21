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
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define ifreq_offsetof(x)  offsetof(struct ifreq, x)
#define SET_SA_FAMILY(addr, family)                        \
    memset ((char *) &(addr), '\0', sizeof(addr));        \
    addr.sa_family = (family);

#define SIN_ADDR(x)        (((struct sockaddr_in *) (&(x)))->sin_addr.s_addr)

#define    MAC_BCAST_ADDR        "\xff\xff\xff\xff\xff\xff"
#define    GRATUITOUS_ARPS        2
#define    GRATUITOUS_ARP_SPACING    2    
#define    ARPOP_REPLY        2
#define    offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

struct arpMsg
{
    struct  ethhdr ethhdr;    
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

const char ETH0[]="eth0";

#ifndef SIOCGMIIPHY
#define SIOCGMIIPHY (SIOCDEVPRIVATE)        
#define SIOCGMIIREG (SIOCDEVPRIVATE+1)         
#define SIOCSMIIREG (SIOCDEVPRIVATE+2)         
#define SIOCGPARAMS (SIOCDEVPRIVATE+3)         
#define SIOCSPARAMS (SIOCDEVPRIVATE+4)         
#endif

#include <netinet/if_ether.h>
#include <net/if_arp.h>
#include <net/if.h>
#include <netinet/in.h>


struct arpMsg1 {
    /* Ethernet header */
    u_char h_dest[6]; /* destination ether addr */
    u_char h_source[6]; /* source ether addr */
    u_short h_proto; /* packet type ID field */

    /* ARP packet */
    uint16_t htype; /* hardware type (must be ARPHRD_ETHER) */
    uint16_t ptype; /* protocol type (must be ETH_P_IP) */
    uint8_t hlen; /* hardware address length (must be 6) */
    uint8_t plen; /* protocol address length (must be 4) */
    uint16_t operation; /* ARP opcode */
    uint8_t sHaddr[6]; /* sender's hardware address */
    uint8_t sInaddr[4]; /* sender's IP address */
    uint8_t tHaddr[6]; /* target's hardware address */
    uint8_t tInaddr[4]; /* target's IP address */
    uint8_t pad[18]; /* pad for min. Ethernet payload (60 bytes) */
} ATTRIBUTE_PACKED;

/* Include our own copy of struct sysinfo to avoid binary compatability
 * problems with Linux 2.4, which changed things.  Grumble, grumble. */
struct sysinfo {
    long uptime;            /* Seconds since boot */
    unsigned long loads[3];        /* 1, 5, and 15 minute load averages */
    unsigned long totalram;        /* Total usable main memory size */
    unsigned long freeram;        /* Available memory size */
    unsigned long sharedram;    /* Amount of shared memory */
    unsigned long bufferram;    /* Memory used by buffers */
    unsigned long totalswap;    /* Total swap space size */
    unsigned long freeswap;        /* swap space still available */
    unsigned short procs;        /* Number of current processes */
    unsigned short pad;            /* Padding needed for m68k */
    unsigned long totalhigh;    /* Total high memory size */
    unsigned long freehigh;        /* Available high memory size */
    unsigned int mem_unit;        /* Memory unit size in bytes */
    char _f[20-2*sizeof(long)-sizeof(int)];    /* Padding: libc5 uses this.. */
};
extern int sysinfo (struct sysinfo* info);

/* function prototypes */
int arpping(uint32_t yiaddr, uint32_t ip, uint8_t *arp, char *interface);

long uptime(void) {
    struct sysinfo info;
    sysinfo(&info);
    return info.uptime;
}

//static uint8_t myMAC[6];
static struct in_addr myself, mymask;
static u_char *ptr;
static struct ifreq ifr; /*   ifr   structure   */

int getIPMAC(void)
{
    int ret = 0;
    int fd_arp; /*   socket   fd   for   receive   packets   */
    
    char device[32]; /*   ethernet   device   name   */
    struct sockaddr_in *sin_ptr;

    memset(device, 0, 32);
    strcpy(device, "eth0");

    if ((fd_arp = socket(AF_INET, SOCK_PACKET, htons(0x0806))) < 0)
    {
        return -1;
    }
    
    strcpy(ifr.ifr_name, device);
    /*   ifr.ifr_addr.sa_family   =   AF_INET;   */

    /*   get   ip   address   of   my   interface   */
    if (ioctl(fd_arp, SIOCGIFADDR, &ifr) < 0)
    {
        ret = -1;
        goto err_label;
    }
    sin_ptr = (struct sockaddr_in *) &ifr.ifr_addr;
    myself = sin_ptr->sin_addr;

    /*   get   network   mask   of   my   interface   */
    if (ioctl(fd_arp, SIOCGIFNETMASK, &ifr) < 0) {
        ret = -1;
        goto err_label;
    }
    sin_ptr = (struct sockaddr_in *) &ifr.ifr_addr;
    mymask = sin_ptr->sin_addr;

    /*   get   mac   address   of   the   interface   */
    if (ioctl(fd_arp, SIOCGIFHWADDR, &ifr) < 0)
    {
        ret = -1;
        goto err_label;
    }
    ptr = (u_char *) &ifr.ifr_ifru.ifru_hwaddr.sa_data[0];

err_label:
    close(fd_arp);
  
    return ret;
}

char *mac_ntoa ( const unsigned char *mac )
{
    /* Linux 下有 ether_ntoa(),不过我们重新写一个也很简单 */
    static char buffer[18];
    memset ( buffer,0,sizeof ( buffer ) );
    sprintf ( buffer,"%02X:%02X:%02X:%02X:%02X:%02X",
              mac[0],mac[1],mac[2],mac[3],mac[4],mac[5] );
    return ( buffer );
}

int ArpCheck(void)
{
    if (-1 == getIPMAC())
    {
        return 0;
	}

    return !arpping(myself.s_addr,myself.s_addr,ptr,"eth0");
}

int arpping(uint32_t yiaddr, uint32_t ip, uint8_t *mac, char *interface)
{
    int timeout = 2;
    int optval = 1;
    int s; /* socket */
    int rv = 1; /* return value */
    struct sockaddr addr; /* for interface name */
    struct arpMsg1 arp;
    fd_set fdset;
    struct timeval tm;
    time_t prevTime;

    if ((s = socket(PF_PACKET, SOCK_PACKET, htons(ETH_P_ARP))) == -1)
	{
        return -1;
    }

    if (setsockopt(s, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) == -1)
	{
        close(s);
        return -1;
    }

    /* send arp request */
    memset(&arp, 0, sizeof(arp));
    memcpy(arp.h_dest, (uint8_t *)MAC_BCAST_ADDR, 6); /* MAC DA */
    memcpy(arp.h_source, mac, 6); /* MAC SA */
    arp.h_proto = htons(ETH_P_ARP); /* protocol type (Ethernet) */
    arp.htype = htons(ARPHRD_ETHER); /* hardware type */
    arp.ptype = htons(ETH_P_IP); /* protocol type (ARP message) */
    arp.hlen = 6; /* hardware address length */
    arp.plen = 4; /* protocol address length */
    arp.operation = htons(ARPOP_REQUEST); /* ARP op code */
    memcpy(arp.sInaddr, &ip, sizeof(ip)); /* source IP address */
    memcpy(arp.sHaddr, mac, 6); /* source hardware address */
    memcpy(arp.tInaddr, &yiaddr, sizeof(yiaddr)); /* target IP address */

    memset(&addr, 0, sizeof(addr));
    strcpy(addr.sa_data, interface);
    if (sendto(s, &arp, sizeof(arp), 0, &addr, sizeof(addr)) < 0)
    {
        rv = 0;
	}

    /* wait arp reply, and check it */
    tm.tv_usec = 0;
    prevTime = uptime();
    while (timeout > 0) {
        FD_ZERO(&fdset);
        FD_SET(s, &fdset);
        tm.tv_sec = timeout;
        if (select(s + 1, &fdset, (fd_set *) NULL, (fd_set *) NULL, &tm) < 0) {
            if (errno != EINTR)
                rv = 0;
        } else if (FD_ISSET(s, &fdset)) {
            if (recv(s, &arp, sizeof(arp), 0) < 0)
                rv = 0;

            //printf("\r\n op:%d,ip:%s,mac:%s\r\n",ntohs(arp.operation),arp.sInaddr,arp.tHaddr);

            if (arp.operation == htons(ARPOP_REPLY) && bcmp(arp.tHaddr, mac, 6)
                    == 0 && *((uint32_t *) arp.sInaddr) == yiaddr) {
                //printf("\r\n find the same ip.\r\n");
                rv = 0;
                break;
            }
            else if (arp.operation == htons(ARPOP_REQUEST) &&  *((uint32_t *) arp.sInaddr) == yiaddr) 
            {
                rv = 0;
                break;
            }
        }
        timeout -= uptime() - prevTime;
        prevTime = uptime();
    }
    close(s);
    return rv;
}



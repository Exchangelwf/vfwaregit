#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    "Cmos.h"
#include    "Net.h"

#define PPPoE_ENABLE_FNAME "/mnt/mtd/Config/ppp/pppoe-enable"
#define PPPoE_CFG_FNAME "/mnt/mtd/Config/ppp/pap-secrets"
#define DDNS_CFG_FNAME "/mnt/mtd/Config/ddns-server"
#define PPPoE_DIAL_IP  "/mnt/mtd/Config/dial-ip"
#define DHCP_CFG_FNAME  "/mnt/mtd/Config/dhcp.cfg"

#ifndef trace
#define trace printf
#endif

int NetSetDDNSConfig(NET_DDNS_CONFIG *pConfig)
{
    FILE *fp = NULL;

    if (NULL == pConfig)
    {
        return -1;
    }
    
    fp = fopen(DDNS_CFG_FNAME, "w+");
    if (NULL == fp)
    {
        trace("open file fail: %s\n", DDNS_CFG_FNAME);
        return -1;
    }

    fprintf(fp, "%d\n", pConfig->enable);
    
    fprintf(fp, "%s\n", pConfig->serverip);
    
    fprintf(fp, "%s\n", pConfig->hostname);

    fprintf(fp, "%d", pConfig->serverport);
    
    fclose(fp);
    
    return 0;
}

int NetGetDDNSConfig(NET_DDNS_CONFIG *pConfig)
{
    FILE *fp = NULL;
    char cTemp[8] = {0};
    
    if (NULL == pConfig)
    {
        return -1;
    }

    fp = fopen(DDNS_CFG_FNAME, "rb");
    if (NULL == fp)
    {
        trace("open file fail: %s\n", DDNS_CFG_FNAME);
        return -1;
    }

    fgets(cTemp, sizeof(cTemp), fp);
    cTemp[strlen(cTemp) - 1] = 0;
    pConfig->enable = atoi(cTemp);
    
    memset(pConfig->serverip, 0, sizeof(pConfig->serverip));
    fgets(pConfig->serverip, sizeof(pConfig->serverip), fp);
    pConfig->serverip[strlen(pConfig->serverip) - 1] = 0;

    memset(pConfig->hostname, 0, sizeof(pConfig->hostname));
    fgets(pConfig->hostname, sizeof(pConfig->hostname), fp);
    pConfig->hostname[strlen(pConfig->hostname) - 1] = 0;
    
    memset(cTemp, 0, sizeof(cTemp));
    fgets(cTemp, sizeof(cTemp), fp);
    pConfig->serverport = atoi(cTemp);
    
    fclose(fp);
    
    return 0;
}

int NetSetPPPoEConfig(NET_PPPOE_CONFIG *pConfig)
{
    FILE *fp = NULL;
    
    if (NULL == pConfig)
    {
        return -1;
    }
    
    fp = fopen(PPPoE_CFG_FNAME, "w+");
    if (NULL == fp)
    {
        trace("open file fail: %s\n", PPPoE_CFG_FNAME);
        return -1;
    }

    fprintf(fp, "%s\n", pConfig->username);
    fprintf(fp, "%s\n", pConfig->password);
    fclose(fp);
    
    fp = fopen(PPPoE_ENABLE_FNAME, "w+");
    if (NULL == fp)
    {
        trace("open file fail: %s\n", PPPoE_ENABLE_FNAME);
        return -1;
    }

    fprintf(fp, "%d", pConfig->enable);
    fclose(fp);
    return 0;
    
}

int NetGetPPPoEConfig(NET_PPPOE_CONFIG *pConfig)
{
    FILE *fp = NULL;
    char cTemp[4] = {0};
    
    if (NULL == pConfig)
    {
        return -1;
    }

    memset(pConfig->username, 0, sizeof(pConfig->username));
    memset(pConfig->password, 0, sizeof(pConfig->password));
    
    fp = fopen(PPPoE_CFG_FNAME, "rb");
    if (NULL == fp)
    {
        trace("open file fail: %s, get from cmos\n", PPPoE_CFG_FNAME);
        NetSetPPPoEConfig(pConfig);
        return 0;
    }
    
    fgets(pConfig->username, sizeof(pConfig->username), fp);
    pConfig->username[strlen(pConfig->username) - 1] = 0;
    fgets(pConfig->password, sizeof(pConfig->password), fp);
    pConfig->password[strlen(pConfig->password) -1 ] = 0;
    
    fclose(fp);

    fp = fopen(PPPoE_ENABLE_FNAME, "rb");
    if (NULL == fp)
    {
        trace("open file fail: %s\n", PPPoE_ENABLE_FNAME);
        return -1;
    }
    
    fgets(cTemp, sizeof(cTemp), fp);
    pConfig->enable = atoi(cTemp);

    fclose(fp);
    
    return 0;
}

int NetGetDialIpAddress(char *pDailIp)
{
    FILE *fp = NULL;
    
    if (NULL == pDailIp)
    {
        return -1;
    }

    fp = fopen(PPPoE_DIAL_IP, "rb");

    if (NULL == fp)
    {
        trace("open file fail: %s\n",PPPoE_DIAL_IP);
         return -1;
    }
    //!与Sofia保持一致,应用代码中pDailIp大小为16
    fgets(pDailIp, 16, fp);
   
    pDailIp[strlen(pDailIp) - 1] = 0;
 
    fclose(fp);

    //by wmy 091105
    if(0== strcmp("", pDailIp))
        return -2;
    
    if(0== strcmp("0.0.0.0", pDailIp))
        return -3;
    
    return 0;
}

int NetSetDhcpEnable(int enable)
{
    FILE *fp = NULL;
    
    fp = fopen(DHCP_CFG_FNAME, "w+");
    if (NULL == fp)
    {
        trace("open file fail: %s\n", DHCP_CFG_FNAME);
        return -1;
    }

    fprintf(fp, "%d", enable);
    CMOSWrite(CMOS_DHCP, &enable, 1);
    
    fclose(fp);
    return 0;
}

int NetGetDhcpEnable(int *pEnable)
{
    FILE *fp = NULL;
    char cTemp[4] = {0};
    
    if (NULL == pEnable)
    {
        return -1;
    }
    
    fp = fopen(DHCP_CFG_FNAME, "rb");
    if (NULL == fp)
    {
        trace("open file fail: %s, get from cmos\n", DHCP_CFG_FNAME);
        if (CMOSRead(CMOS_DHCP, pEnable, 1))
        {
            trace("read dhcp info from cmos fail\n");
            return -1;
        }
        
        NetSetDhcpEnable(*pEnable);
        return 0;
    }

    fgets(cTemp, sizeof(cTemp), fp);
    *pEnable = atoi(cTemp);
    
    fclose(fp);
    return 0;
}



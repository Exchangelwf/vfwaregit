#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    "Cmos.h"

#include "DVRDEF.H"

//  设置UPNP使能标志
int NetSetUpnpEnable(char enable)
{
    int ret=0;
    
    enable &= 0x01;
    ret=CMOSWrite(CMOS_UPNP_ENABLE, &enable, 1);
    if(0!=ret)
    {
        printf("\n set upnp enable status fail.......\n");
        return -1;
    }
    return 0;
}

//  获得UPNP使能标志
int NetGetUpnpEnable(char * pEnable)
{
    int ret=0;

    if(NULL == pEnable)
    {
        printf("%s - param is NULL\r\n", __func__);
        return -1;
    }
    
    ret=CMOSRead(CMOS_UPNP_ENABLE, pEnable, 1);
    if(0!=ret)
    {
        printf("\n get upnp enable status fail.....\n");
        return -1;
    }
    return 0;
}


#include "networkapi/wjalink_api.h"
#include "networkapi/wjalink_callback.h"
#include "networkservice/CNetWorkClient.h"

int wja_netservice_init( Wja_InitNetworkParam netparam)
{
	g_NetworkServiceClient.Run(netparam);
	return 0;
}

int wja_netservice_destroy()
{
	g_NetworkServiceClient.Stop();
	return 0;
}

int wja_sendvideodata( unsigned char  channel,
						unsigned char ismainorsub,
			                    char isIFrame, 
			                    void* Data, 
			                    unsigned  DataLen,callback fun,void* arg)
{
	g_NetworkServiceClient.RealTimevideodataSend(channel,ismainorsub,isIFrame,Data,DataLen,fun,arg);
	return 0;
}

int wja_sendplaybackdata(	unsigned char channel, 
							char isIFrame, 
							void* Data, 
						unsigned DataLen,callback fun,void* arg)
{
	g_NetworkServiceClient.RecordPlayBackSend(channel,isIFrame,Data,DataLen,fun,arg);
	return 0;
}

int wja_sendrecorddata(unsigned char channel,                            
							void* Data, 
						unsigned DataLen,unsigned int clientid,callback fun,void* arg)
{
	g_NetworkServiceClient.RecordDownLoadSend(channel,Data,DataLen,clientid,fun,arg);
	return 0;
}

int wja_sendsnapdata(unsigned char channel,                            
							void* Data, 
						unsigned DataLen,callback fun,void* arg)
{
	g_NetworkServiceClient.SnapvideodataSend(channel,Data,DataLen,fun,arg);
	return 0;
}


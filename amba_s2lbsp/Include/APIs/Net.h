

#ifndef __NET_API_H__
#define __NET_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup NetAPI API Net
/// 网络接口、DDNS与PPPoE的参数设置与获取。
/// @{

#if 0
 reset net card
 *  ifname always is "eth0"
 
int reset_Ncard(const char *ifname);
#endif


/// 设置主机的IP地址
/// 
/// \param [in] pEthName 网卡名，NULL表示当前使用的网卡
/// \param [in] pHostIp 主机IPpHostIp
/// \param [in] pNetMask 子网掩码点分字符串
/// \retval 0  设置成功
/// \retval <0  设置失败
int NetSetHostIP(const char *pEthName,
				 const char *pHostIp,
                 const char *pNetMask);


/// 设置网关地址
/// 
/// \param [in] pEthName 网卡名，NULL表示当前使用的网卡
/// \param [in] pGateway 网关地址点分字符串
/// \retval 0  设置成功
/// \retval <0  设置失败
int NetSetGateway(const char *pEthName,
				  const char *pGateway);


/// 设置TCP端口，给ULK使用
/// 
/// \param [in] pEthName 网卡名，NULL表示当前使用的网卡
/// \param [in] pGateway TCP端口
/// \retval 0  设置成功
/// \retval <0  设置失败
int NetSetTCPPort(const char *pEthName, 
				const char *TCPPort);

/// 获取主机的IP地址
/// 
/// \param [in] pEthName 网卡名，NULL表示当前使用的网卡
/// \param [out] pHostIp 主机IP地址点分字符串
/// \param [in] iHLen 主机IP地址字符串缓冲长度
/// \param [out] pNetMask 子网掩码点分字符串
/// \param [in] iNLen 子网掩码字符串缓冲长度
/// \retval 0  获取成功
/// \retval <0  获取失败
int NetGetHostIP(const char *pEthName,
				 char *pHostIp,
                 const int iHLen,
                 char *pNetMask,
                 const int iNLen);


/// 获取网关地址
/// 
/// \param [in] pEthName 网卡名，NULL表示当前使用的网卡
/// \param [out] pGateway 网关地址点分字符串
/// \param [in] iGLen 网关地址字符串缓冲长度
/// \retval 0  获取成功
/// \retval <0  获取失败
int NetGetGateway(const char *pEthName,
				  char *pGateway,
                  const int iGLen);


/// 取得本机的Mac地址
/// 
/// \param [in] pEthName 接口名称，如eth0,eth1。，NULL表示当前使用的网卡
/// \param [out] pEthMac Mac16进制地址的字符串，形如00:05:0e:11:12:15。
/// \param [in] iEthMacLen MAC地址掩码长度。
/// \retval 0  获取成功
/// \retval <0  获取失败
int NetGetMAC(const char *pEthName,
                 char *pEthMac,
                 const int iEthMacLen);


/// 设置本机的Mac地址
/// 
/// \param [in] pEthName 接口名称，如eth0,eth1。，NULL表示当前使用的网卡
/// \param [in] pEthMac Mac16进制地址的字符串，形如00:05:0e:11:12:15
/// \retval 0  设置成功
/// \retval <0  设置失败
int NetSetMAC(const char *pEthName, const char *pEthMac);

/// 获取DNS地址
/// 
/// \param [in] pPrimary 首选的DNS服务器地址字符串。
/// \param [in] pSecondary 备选的DNS服务器地址字符串。
/// \param [in] iPlen 长度,要大于16
/// \retval 0  设置成功
/// \retval <0  设置失败
//modified by debugone 090323
int NetGetDNSAddress(char *pprimary,const int iPlen ,char *psecondary, const int iSlen);
/// 设置DNS地址
/// 
/// \param [in] pPrimary 首选的DNS服务器地址字符串。
/// \param [in] pSecondary 备选的DNS服务器地址字符串。
/// \retval 0  设置成功
/// \retval <0  设置失败
int NetSetDNSAddress(const char * pPrimary, const char * pSecondary);

/// 设置DNS域名
/// 
/// \param [in] pDomain DNS域名字符串。
/// \retval 0  设置成功
/// \retval <0  设置失败
int NetSetDNSDomain(const char * pDomain);


/// 设置计算机名
/// 
/// \param [in] pHostName 计算机名字符串。
/// \retval 0  设置成功
/// \retval <0  设置失败
int NetSetDNSHostName(const char * pHostName);

//int NetSetMac(const char * pMac);

/// 初始化DVR web控件
/// 
/// \param [in] iWebPort web服务端口号
/// \param [in] iAppPort 应用程序端口号
/// \param [in] iLanguage web控件语言序号
/// \retval 0  设置成功
/// \retval <0  设置失败

//int	WebInitialize(int iWebPort, int iAppPort, int iLanguage);
int WebStart(int iWebPort, int iAppPort, int iLanguage, const char* strWebRoot, const char* webFile);
int WebStop(void);
int WebRestart(void);
int WebStatus(void);
int WebDebug(int level);
/////////////////////////////////////////////////////
/// 设置主机的IP/掩码/网关地址
///此组函数用于设置多网卡，多网卡时通过参数pEthName区分网卡，如"eth0","eth2"
///其它参数意义参见上面对应函数
int NetSetHostIP2(const char *pEthName,  
		   const char *pHostIp,
                 const char *pNetMask);


/// 设置网关地址
int NetSetGateway2(const char *pEthName,
				const char *pGateway);


/// 获取主机的IP地址
int NetGetHostIP2(const char *pEthName,
		   char *pHostIp,
                 const int iHLen,
                 char *pNetMask,
                 const int iNLen);


/// 获取网关地址
int NetGetGateway2(const char *pEthName,
		    char *pGateway,
                  const int iGLen);
//////////////////////////////////////////////


/// DDNS配置结构
typedef struct NET_DDNS_CONFIG
{
	int		enable;			///< 是否启动DDNS，0-不启动，1-启动
	char	hostname[32];	///< 主机名，带0结束符的字符串。
	char	username[32];	///< DDNS服务用户名，带0结束符的字符串。
	char	password[32];	///< DDNS服务密码，带0结束符的字符串。
	char	serverip[32];	///< DDNS服务器IP地址，带0结束符的字符串，点分形式，如192.168.0.6。
	int		serverport;		///< DDNS服务端口。
} NET_DDNS_CONFIG;

/// iscsi 配置结构
typedef struct NET_ISCSI_CONFIG
{
	int enable;
	int auth;
	char username[64];
	char password[64];
	char ip[16];
	char port[10];
}NET_ISCSI_CONFIG;

int NetGetIscsiConfig(NET_ISCSI_CONFIG *m_pConfig);
int NetSetIscsiConfig(NET_ISCSI_CONFIG *m_pConfig);


/// 设置DDNS（动态域名解析）配置。
/// 
/// \param [in] pConfig 配置结构NET_DDNS_CONFIG指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int NetSetDDNSConfig(NET_DDNS_CONFIG *pConfig);


/// 获取DDNS（动态域名解析）配置， 如果没有配置，应当返回一个初始化的结构。
/// 
/// \param [out] pConfig 配置结构NET_DDNS_CONFIG指针。
/// \retval 0  获取成功
/// \retval <0  获取失败
int NetGetDDNSConfig(NET_DDNS_CONFIG *pConfig);

/// PPPoE配置结构
typedef struct NET_PPPOE_CONFIG
{
	int		enable;			///< 是否启动PPPoE，0-不启动，1-启动
	char	username[64];	///< 帐户用户名，带0结束符的字符串。
	char	password[64];	///< 帐户密码，带0结束符的字符串, 因为CMOS空间限制密码最多为8字节。
} NET_PPPOE_CONFIG;


/// 设置PPPoE（以太网点对点）配置。
/// 
/// \param [in] pConfig 配置结构NET_PPPOE_CONFIG指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int NetSetPPPoEConfig(NET_PPPOE_CONFIG *pConfig);


/// 获取PPPoE（以太网点对点）配置，如果没有配置，应当返回一个初始化的结构。
/// 
/// \param [out] pConfig 配置结构NET_PPPOE_CONFIG指针。
/// \retval 0  获取成功
/// \retval <0  获取失败
int NetGetPPPoEConfig(NET_PPPOE_CONFIG *pConfig);


/// 获取PPPoE拨号后分配的IP地址
int NetGetDialIpAddress(char *pDailIp);


/// 设置DHCP使能标志。
/// 
/// \param [in] enable Bit0~bit31分别代表对应第几块网卡的DHCP使能标志 0： 不使能 1： 使能
/// \retval 0  设置成功
/// \retval <0  设置失败
int NetSetDhcpEnable(int enable);

/// 获取DHCP的使能标志
/// 
/// \param [out] pEnable Bit0~bit31分别代表对应第几块网卡的DHCP使能标志 0： 不使能 1： 使能
/// \retval 0  获取成功
/// \retval <0  获取失败
int NetGetDhcpEnable(int *pEnable);

/// 获取网线连接状态
/// 
/// \param [out] ethname 网口名, 一般为"eth0"
/// \retval 0 连接正常
/// \retval other 异常
int mii_diag(char *ethname);


/// 检测IP地址是否冲突
/// 
/// \retval 0 正常
/// \retval other 冲突
int ArpCheck();


/// @} end of group


#define IW_ENCODING_TOKEN_MAX 128 //64

#define ENCRYPTION_OFF	0
#define ENCRYPTION_ON	1
#define ENCRYPTION_WEB64 2
#define ENCRYPTION_WEB128 3

typedef struct
{
	char isWirelessEnable;
	char reserved[3];
}ENABLE_T_WLAN;

typedef struct 
{
       int    Enable;
	char	 SSID[36];
	int	LinkMode;	//0:auto 1:adhoc 2:Infrastructure
	int   Encryption;	//0: off  1:on 2:WEP64bit 3: WEP128bit
	int   KeyType;  //0:Hex 1:ASCII
       int   KeyID;
	char   Keys[IW_ENCODING_TOKEN_MAX]; 
	int KeyFlag;
	int Reserved[3];
}CONFIG_WLAN;

typedef struct 
{
	char	SSID[36];
	int	LinkMode;	//0:adhoc 1:Infrastructure
	int 	Encryption;	//0: off 1:on  2:WEP64bit 3: WEP128bit
}WLAN_DEVICE;


int WLanOpen();
int WLanClose();
//必须首先找到可用的无线网卡
int WLanSetConfig(CONFIG_WLAN* pconfig);
int WLanGetConfig(CONFIG_WLAN* pconfig);
//int WLanScanDevices(WLanDeviceEnumProc proc);
WLAN_DEVICE* WLanScanDevices(char * num);

//此组函数用于多网卡配置
int NetSetHostIPEx(const char *pEthName,  
		   			const char *pHostIp,
                 			const char *pNetmask);
/// 设置网关地址
int NetSetGatewayEx(const char *pEthName,
					     const char *pGateway);


/// 获取主机的IP地址
int NetGetHostIPEx(const char *pEthName,
		   			 char *pHostIp,
                 			 const int iHLen,
                 			 char *pNetmask,
                 			 const int iNLen);
/// 获取网关地址
int NetGetGatewayEx(const char *pEthName,
		    			     char *pGateway,
                  			     const int iGLen);
//获取EthName，供应用层调用。可从系统获取，现写死
//int NetGetEthName(int EthNo, char *pEthName, int len_EthName);

//*pEthName :	存入支持的网卡名,以::分隔,如eth0::eth2::
//len_EthName:	存入支持的网卡名BUF长度,一定要够长,否则失败
//*Ethnum:		实际找到的网卡数量

int NetGetEthName( char *pEthName, int len_EthName, int *iEthnum);
//modified by debugone 090323
int NetGetDefaultEth(char *pEthName);
int NetSetDefaultEth(const char *pEthName);


// by wmy
//  设置UPNP使能标志
int NetSetUpnpEnable(char enable);

//  获得UPNP使能标志
int NetGetUpnpEnable(char * pEnable);

enum network_media {
 NM_WIRED = 0,  //有线网卡
 NM_WIRELESS,  //无线网卡
 NM_NR,    //种类
};

typedef struct tagNetDevice
{
	uchar valid;			//当前网卡是否连接
	uchar trans_media;	//传输媒介
	uchar reserved[6];	//保留
	char name[32];		//网卡名称
	
}NetDevice;
/********************************************************************
**
**	函数名	:	NetGetEthDevice
**	入参	:	
**				1.	pNetDev:指向网卡属性结构NetDevice的指针数组。
**				2.	iMax   :最大支持的网卡个数(可以设置为10,目前实际为3个)
**	返回	:	> 0 支持的实际网卡个数
**				= 0 支持网卡个数为0,即失败
**	功能说明:	此函数用于应用层获取网卡支持的个数,以及对应网卡的种类和当前是否可用
**
*********************************************************************/
int NetGetEthDevice(NetDevice *pNetDev, int iMax);

///3g配置结构
typedef struct NET_WRIELESS_CONFIG
{
	int enable;
	char username[64];
	char password[64];
	char ip[16];
	int type;
	int	signal;
}NET_WRIELESS_CONFIG;

int NetGetWrielessConfig(NET_WRIELESS_CONFIG *m_pConfig);
int NetSetWrielessConfig(NET_WRIELESS_CONFIG *m_pConfig);

typedef struct _RTSP_CONFIG
{
    ushort    usEnable;           //使能 
    ushort    usListernPort;     //RTSP侦听端口
    ushort    usUdpStartPort;  //UDP起始端口
    ushort    usUdpPortNum;   //UDP端口数量

    IPADDR   stMulticastIp;//组播地址，用掉4字节保留位 
    ushort    usPort; //组播端口，用掉2字节保留位

    ushort    usReserve[33];   //保留
} RTSP_CONFIG;

int SetRtspPort(RTSP_CONFIG *pConfig);
int SartVencRtsp(void);

#ifdef __cplusplus
}
#endif

#endif //__NET_API_H__


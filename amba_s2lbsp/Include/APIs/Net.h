

#ifndef __NET_API_H__
#define __NET_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup NetAPI API Net
/// ����ӿڡ�DDNS��PPPoE�Ĳ����������ȡ��
/// @{

#if 0
 reset net card
 *  ifname always is "eth0"
 
int reset_Ncard(const char *ifname);
#endif


/// ����������IP��ַ
/// 
/// \param [in] pEthName ��������NULL��ʾ��ǰʹ�õ�����
/// \param [in] pHostIp ����IPpHostIp
/// \param [in] pNetMask �����������ַ���
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int NetSetHostIP(const char *pEthName,
				 const char *pHostIp,
                 const char *pNetMask);


/// �������ص�ַ
/// 
/// \param [in] pEthName ��������NULL��ʾ��ǰʹ�õ�����
/// \param [in] pGateway ���ص�ַ����ַ���
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int NetSetGateway(const char *pEthName,
				  const char *pGateway);


/// ����TCP�˿ڣ���ULKʹ��
/// 
/// \param [in] pEthName ��������NULL��ʾ��ǰʹ�õ�����
/// \param [in] pGateway TCP�˿�
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int NetSetTCPPort(const char *pEthName, 
				const char *TCPPort);

/// ��ȡ������IP��ַ
/// 
/// \param [in] pEthName ��������NULL��ʾ��ǰʹ�õ�����
/// \param [out] pHostIp ����IP��ַ����ַ���
/// \param [in] iHLen ����IP��ַ�ַ������峤��
/// \param [out] pNetMask �����������ַ���
/// \param [in] iNLen ���������ַ������峤��
/// \retval 0  ��ȡ�ɹ�
/// \retval <0  ��ȡʧ��
int NetGetHostIP(const char *pEthName,
				 char *pHostIp,
                 const int iHLen,
                 char *pNetMask,
                 const int iNLen);


/// ��ȡ���ص�ַ
/// 
/// \param [in] pEthName ��������NULL��ʾ��ǰʹ�õ�����
/// \param [out] pGateway ���ص�ַ����ַ���
/// \param [in] iGLen ���ص�ַ�ַ������峤��
/// \retval 0  ��ȡ�ɹ�
/// \retval <0  ��ȡʧ��
int NetGetGateway(const char *pEthName,
				  char *pGateway,
                  const int iGLen);


/// ȡ�ñ�����Mac��ַ
/// 
/// \param [in] pEthName �ӿ����ƣ���eth0,eth1����NULL��ʾ��ǰʹ�õ�����
/// \param [out] pEthMac Mac16���Ƶ�ַ���ַ���������00:05:0e:11:12:15��
/// \param [in] iEthMacLen MAC��ַ���볤�ȡ�
/// \retval 0  ��ȡ�ɹ�
/// \retval <0  ��ȡʧ��
int NetGetMAC(const char *pEthName,
                 char *pEthMac,
                 const int iEthMacLen);


/// ���ñ�����Mac��ַ
/// 
/// \param [in] pEthName �ӿ����ƣ���eth0,eth1����NULL��ʾ��ǰʹ�õ�����
/// \param [in] pEthMac Mac16���Ƶ�ַ���ַ���������00:05:0e:11:12:15
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int NetSetMAC(const char *pEthName, const char *pEthMac);

/// ��ȡDNS��ַ
/// 
/// \param [in] pPrimary ��ѡ��DNS��������ַ�ַ�����
/// \param [in] pSecondary ��ѡ��DNS��������ַ�ַ�����
/// \param [in] iPlen ����,Ҫ����16
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
//modified by debugone 090323
int NetGetDNSAddress(char *pprimary,const int iPlen ,char *psecondary, const int iSlen);
/// ����DNS��ַ
/// 
/// \param [in] pPrimary ��ѡ��DNS��������ַ�ַ�����
/// \param [in] pSecondary ��ѡ��DNS��������ַ�ַ�����
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int NetSetDNSAddress(const char * pPrimary, const char * pSecondary);

/// ����DNS����
/// 
/// \param [in] pDomain DNS�����ַ�����
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int NetSetDNSDomain(const char * pDomain);


/// ���ü������
/// 
/// \param [in] pHostName ��������ַ�����
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int NetSetDNSHostName(const char * pHostName);

//int NetSetMac(const char * pMac);

/// ��ʼ��DVR web�ؼ�
/// 
/// \param [in] iWebPort web����˿ں�
/// \param [in] iAppPort Ӧ�ó���˿ں�
/// \param [in] iLanguage web�ؼ��������
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��

//int	WebInitialize(int iWebPort, int iAppPort, int iLanguage);
int WebStart(int iWebPort, int iAppPort, int iLanguage, const char* strWebRoot, const char* webFile);
int WebStop(void);
int WebRestart(void);
int WebStatus(void);
int WebDebug(int level);
/////////////////////////////////////////////////////
/// ����������IP/����/���ص�ַ
///���麯���������ö�������������ʱͨ������pEthName������������"eth0","eth2"
///������������μ������Ӧ����
int NetSetHostIP2(const char *pEthName,  
		   const char *pHostIp,
                 const char *pNetMask);


/// �������ص�ַ
int NetSetGateway2(const char *pEthName,
				const char *pGateway);


/// ��ȡ������IP��ַ
int NetGetHostIP2(const char *pEthName,
		   char *pHostIp,
                 const int iHLen,
                 char *pNetMask,
                 const int iNLen);


/// ��ȡ���ص�ַ
int NetGetGateway2(const char *pEthName,
		    char *pGateway,
                  const int iGLen);
//////////////////////////////////////////////


/// DDNS���ýṹ
typedef struct NET_DDNS_CONFIG
{
	int		enable;			///< �Ƿ�����DDNS��0-��������1-����
	char	hostname[32];	///< ����������0���������ַ�����
	char	username[32];	///< DDNS�����û�������0���������ַ�����
	char	password[32];	///< DDNS�������룬��0���������ַ�����
	char	serverip[32];	///< DDNS������IP��ַ����0���������ַ����������ʽ����192.168.0.6��
	int		serverport;		///< DDNS����˿ڡ�
} NET_DDNS_CONFIG;

/// iscsi ���ýṹ
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


/// ����DDNS����̬�������������á�
/// 
/// \param [in] pConfig ���ýṹNET_DDNS_CONFIGָ�롣
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int NetSetDDNSConfig(NET_DDNS_CONFIG *pConfig);


/// ��ȡDDNS����̬�������������ã� ���û�����ã�Ӧ������һ����ʼ���Ľṹ��
/// 
/// \param [out] pConfig ���ýṹNET_DDNS_CONFIGָ�롣
/// \retval 0  ��ȡ�ɹ�
/// \retval <0  ��ȡʧ��
int NetGetDDNSConfig(NET_DDNS_CONFIG *pConfig);

/// PPPoE���ýṹ
typedef struct NET_PPPOE_CONFIG
{
	int		enable;			///< �Ƿ�����PPPoE��0-��������1-����
	char	username[64];	///< �ʻ��û�������0���������ַ�����
	char	password[64];	///< �ʻ����룬��0���������ַ���, ��ΪCMOS�ռ������������Ϊ8�ֽڡ�
} NET_PPPOE_CONFIG;


/// ����PPPoE����̫����Ե㣩���á�
/// 
/// \param [in] pConfig ���ýṹNET_PPPOE_CONFIGָ�롣
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int NetSetPPPoEConfig(NET_PPPOE_CONFIG *pConfig);


/// ��ȡPPPoE����̫����Ե㣩���ã����û�����ã�Ӧ������һ����ʼ���Ľṹ��
/// 
/// \param [out] pConfig ���ýṹNET_PPPOE_CONFIGָ�롣
/// \retval 0  ��ȡ�ɹ�
/// \retval <0  ��ȡʧ��
int NetGetPPPoEConfig(NET_PPPOE_CONFIG *pConfig);


/// ��ȡPPPoE���ź�����IP��ַ
int NetGetDialIpAddress(char *pDailIp);


/// ����DHCPʹ�ܱ�־��
/// 
/// \param [in] enable Bit0~bit31�ֱ�����Ӧ�ڼ���������DHCPʹ�ܱ�־ 0�� ��ʹ�� 1�� ʹ��
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int NetSetDhcpEnable(int enable);

/// ��ȡDHCP��ʹ�ܱ�־
/// 
/// \param [out] pEnable Bit0~bit31�ֱ�����Ӧ�ڼ���������DHCPʹ�ܱ�־ 0�� ��ʹ�� 1�� ʹ��
/// \retval 0  ��ȡ�ɹ�
/// \retval <0  ��ȡʧ��
int NetGetDhcpEnable(int *pEnable);

/// ��ȡ��������״̬
/// 
/// \param [out] ethname ������, һ��Ϊ"eth0"
/// \retval 0 ��������
/// \retval other �쳣
int mii_diag(char *ethname);


/// ���IP��ַ�Ƿ��ͻ
/// 
/// \retval 0 ����
/// \retval other ��ͻ
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
//���������ҵ����õ���������
int WLanSetConfig(CONFIG_WLAN* pconfig);
int WLanGetConfig(CONFIG_WLAN* pconfig);
//int WLanScanDevices(WLanDeviceEnumProc proc);
WLAN_DEVICE* WLanScanDevices(char * num);

//���麯�����ڶ���������
int NetSetHostIPEx(const char *pEthName,  
		   			const char *pHostIp,
                 			const char *pNetmask);
/// �������ص�ַ
int NetSetGatewayEx(const char *pEthName,
					     const char *pGateway);


/// ��ȡ������IP��ַ
int NetGetHostIPEx(const char *pEthName,
		   			 char *pHostIp,
                 			 const int iHLen,
                 			 char *pNetmask,
                 			 const int iNLen);
/// ��ȡ���ص�ַ
int NetGetGatewayEx(const char *pEthName,
		    			     char *pGateway,
                  			     const int iGLen);
//��ȡEthName����Ӧ�ò���á��ɴ�ϵͳ��ȡ����д��
//int NetGetEthName(int EthNo, char *pEthName, int len_EthName);

//*pEthName :	����֧�ֵ�������,��::�ָ�,��eth0::eth2::
//len_EthName:	����֧�ֵ�������BUF����,һ��Ҫ����,����ʧ��
//*Ethnum:		ʵ���ҵ�����������

int NetGetEthName( char *pEthName, int len_EthName, int *iEthnum);
//modified by debugone 090323
int NetGetDefaultEth(char *pEthName);
int NetSetDefaultEth(const char *pEthName);


// by wmy
//  ����UPNPʹ�ܱ�־
int NetSetUpnpEnable(char enable);

//  ���UPNPʹ�ܱ�־
int NetGetUpnpEnable(char * pEnable);

enum network_media {
 NM_WIRED = 0,  //��������
 NM_WIRELESS,  //��������
 NM_NR,    //����
};

typedef struct tagNetDevice
{
	uchar valid;			//��ǰ�����Ƿ�����
	uchar trans_media;	//����ý��
	uchar reserved[6];	//����
	char name[32];		//��������
	
}NetDevice;
/********************************************************************
**
**	������	:	NetGetEthDevice
**	���	:	
**				1.	pNetDev:ָ���������ԽṹNetDevice��ָ�����顣
**				2.	iMax   :���֧�ֵ���������(��������Ϊ10,Ŀǰʵ��Ϊ3��)
**	����	:	> 0 ֧�ֵ�ʵ����������
**				= 0 ֧����������Ϊ0,��ʧ��
**	����˵��:	�˺�������Ӧ�ò��ȡ����֧�ֵĸ���,�Լ���Ӧ����������͵�ǰ�Ƿ����
**
*********************************************************************/
int NetGetEthDevice(NetDevice *pNetDev, int iMax);

///3g���ýṹ
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
    ushort    usEnable;           //ʹ�� 
    ushort    usListernPort;     //RTSP�����˿�
    ushort    usUdpStartPort;  //UDP��ʼ�˿�
    ushort    usUdpPortNum;   //UDP�˿�����

    IPADDR   stMulticastIp;//�鲥��ַ���õ�4�ֽڱ���λ 
    ushort    usPort; //�鲥�˿ڣ��õ�2�ֽڱ���λ

    ushort    usReserve[33];   //����
} RTSP_CONFIG;

int SetRtspPort(RTSP_CONFIG *pConfig);
int SartVencRtsp(void);

#ifdef __cplusplus
}
#endif

#endif //__NET_API_H__


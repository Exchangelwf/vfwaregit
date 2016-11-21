#ifndef _NETWORKSERVICE_VINPDU_H_
#define _NETWORKSERVICE_VINPDU_H_

#pragma once

#include <list>
//#include "system/Packet.h"
#include "common/BaseTypedef.h"


namespace NETWORKSERVICE
{

	struct dvrip;

	typedef struct _IE_HEADER
	{
		int IeType; //IE的具体类型，详见参数表 
		int IeLengh; //IE的长度，不含IE头的长度 
	}IE_HEADER;

	typedef struct _tagTlvPtr
	{
		IE_HEADER stHdr;
		char* pData;
	} IVN_TLV_NODE;

#define IVN_TLV_HEADER 8
	/* 
	1）进行分析处理
		new CIVnPdu(); parseBody(char* pBuff = NULL, int iLen = 0)；m_pIpHdr头部信息,包体数据可以通过GetPacket偏移获取
	2）产生pdu或者根据请求作响应
	   new CIVnPdu(); packetBody(char* pBuff = NULL, int iLen = 0); 再修改头数据 m_pIpHdr；GetPacket（），GetPacketLen（）为最后的包内容

	--------------------------------------------------------------------------------------------
	3）TLV 分析处理
	    new CIVnPdu(); parseBody(char* pBuff = NULL, int iLen = 0)；
	   单TLV 
	       包体数据为GetPacket获取到
	       采取bool parseTlv(char* pBuff, int iLen, bool bMainType = true);
	       获取数据直接使用IVN_TLV_NODE* getTlvNode(int ieType, bool bMainType = true)

	   两层TLV
	      首先分析主TLV
		  包体数据为GetPacket获取到
		  采取bool parseTlv(char* pBuff, int iLen, bool bMainType = true);

		  副TLV  可以通过IVN_TLV_NODE* getTlvNode(int ieType, bool bMainType = true)获取数据地址与长度
		         再进行bool parseTlv(char* pBuff, int iLen, bool bMainType = false);
				 依次获取完所需要的副TLV数据 IVN_TLV_NODE* getTlvNode(int ieType, bool bMainType = false)


	4）TLV打包处理
		new CIVnPdu(); packetBody(char* pBuff = NULL, int iLen > 0)已经知道总长度大小（可以估计下大小）

		单TLV 
		     多次 addTlvNode(int ieType, int ieLength, char* pValue)，最后修正下头部扩展数据的长度changeLength(bool bMainType = true)

		两层TLV 
			 先  主TLV addTlvNode(int ieType)，
			     多次 副TLV addTlvNode(int ieType, int ieLength, char* pValue),添加完后修正下主TLV对应的长度changeLength()

			 最后修正头部的长度changeLength(bool bMainType = true)
	*/
	class CIVnPdu
	{
	public:
		CIVnPdu(void);
		~CIVnPdu(void);

		/* 分析缓冲由应用层传入，拷贝保存*/
		bool parseBody(char* pBuff, int iLen);

		/* 打包缓冲pdu传入包体数据 */
		int packetBody(char* pBuff = NULL, int iLen = 0);

		/* 
		获取包体数据  
		GetPacket进行包头偏移，获取到的包体长度为m_iTotalLen-32或者头中扩展长度
			设置包体数据
		packetbody
		*/
		char* GetPacket(){return m_pBuffer;}
		int GetPacketLen(){return m_iTotalLen;}

		struct dvrip* m_pIpHdr;
		void* m_pConn;

		/* 获取TLV */

		/* 添加TLV */

	protected:
		char* m_pBuffer;/* 内部分析和打包缓冲，内部申请以及内部释放*/
		bool mallocflag;
		int m_iTotalLen;
		//CPacket* m_pPacker;
		char m_stDVRIPHdr[32];


		unsigned int m_MainTypeLen;
		char* m_strMainTypePtr;
	};
}

#endif

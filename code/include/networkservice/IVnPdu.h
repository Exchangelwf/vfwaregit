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
		int IeType; //IE�ľ������ͣ���������� 
		int IeLengh; //IE�ĳ��ȣ�����IEͷ�ĳ��� 
	}IE_HEADER;

	typedef struct _tagTlvPtr
	{
		IE_HEADER stHdr;
		char* pData;
	} IVN_TLV_NODE;

#define IVN_TLV_HEADER 8
	/* 
	1�����з�������
		new CIVnPdu(); parseBody(char* pBuff = NULL, int iLen = 0)��m_pIpHdrͷ����Ϣ,�������ݿ���ͨ��GetPacketƫ�ƻ�ȡ
	2������pdu���߸�����������Ӧ
	   new CIVnPdu(); packetBody(char* pBuff = NULL, int iLen = 0); ���޸�ͷ���� m_pIpHdr��GetPacket������GetPacketLen����Ϊ���İ�����

	--------------------------------------------------------------------------------------------
	3��TLV ��������
	    new CIVnPdu(); parseBody(char* pBuff = NULL, int iLen = 0)��
	   ��TLV 
	       ��������ΪGetPacket��ȡ��
	       ��ȡbool parseTlv(char* pBuff, int iLen, bool bMainType = true);
	       ��ȡ����ֱ��ʹ��IVN_TLV_NODE* getTlvNode(int ieType, bool bMainType = true)

	   ����TLV
	      ���ȷ�����TLV
		  ��������ΪGetPacket��ȡ��
		  ��ȡbool parseTlv(char* pBuff, int iLen, bool bMainType = true);

		  ��TLV  ����ͨ��IVN_TLV_NODE* getTlvNode(int ieType, bool bMainType = true)��ȡ���ݵ�ַ�볤��
		         �ٽ���bool parseTlv(char* pBuff, int iLen, bool bMainType = false);
				 ���λ�ȡ������Ҫ�ĸ�TLV���� IVN_TLV_NODE* getTlvNode(int ieType, bool bMainType = false)


	4��TLV�������
		new CIVnPdu(); packetBody(char* pBuff = NULL, int iLen > 0)�Ѿ�֪���ܳ��ȴ�С�����Թ����´�С��

		��TLV 
		     ��� addTlvNode(int ieType, int ieLength, char* pValue)�����������ͷ����չ���ݵĳ���changeLength(bool bMainType = true)

		����TLV 
			 ��  ��TLV addTlvNode(int ieType)��
			     ��� ��TLV addTlvNode(int ieType, int ieLength, char* pValue),��������������TLV��Ӧ�ĳ���changeLength()

			 �������ͷ���ĳ���changeLength(bool bMainType = true)
	*/
	class CIVnPdu
	{
	public:
		CIVnPdu(void);
		~CIVnPdu(void);

		/* ����������Ӧ�ò㴫�룬��������*/
		bool parseBody(char* pBuff, int iLen);

		/* �������pdu����������� */
		int packetBody(char* pBuff = NULL, int iLen = 0);

		/* 
		��ȡ��������  
		GetPacket���а�ͷƫ�ƣ���ȡ���İ��峤��Ϊm_iTotalLen-32����ͷ����չ����
			���ð�������
		packetbody
		*/
		char* GetPacket(){return m_pBuffer;}
		int GetPacketLen(){return m_iTotalLen;}

		struct dvrip* m_pIpHdr;
		void* m_pConn;

		/* ��ȡTLV */

		/* ���TLV */

	protected:
		char* m_pBuffer;/* �ڲ������ʹ�����壬�ڲ������Լ��ڲ��ͷ�*/
		bool mallocflag;
		int m_iTotalLen;
		//CPacket* m_pPacker;
		char m_stDVRIPHdr[32];


		unsigned int m_MainTypeLen;
		char* m_strMainTypePtr;
	};
}

#endif

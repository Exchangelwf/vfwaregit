
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "networkservice/IVnPdu.h"
#include "common/BaseTypedef.h"

namespace NETWORKSERVICE
{

	CIVnPdu::CIVnPdu(void)
	{
		m_pBuffer = 0;
		m_iTotalLen = 0;
		//m_pPacker = 0;
		memset(m_stDVRIPHdr, 0 , sizeof(m_stDVRIPHdr));
		m_pIpHdr = (DVRIP *)m_stDVRIPHdr;
		m_pBuffer = NULL;
		mallocflag=false;

		m_MainTypeLen = 0;
		m_strMainTypePtr = 0;
		m_pConn = 0;
	}

	CIVnPdu::~CIVnPdu(void)
	{
		if (mallocflag)
		{
			free(m_pBuffer);
		}
	}

	bool CIVnPdu::parseBody(char* pBuff, int iLen)
	{
		if ( NULL == pBuff || iLen < 32 )
		{
			return false;
		}

#if 0
		///////协议外已经处理
		DVRIP* pTmp = (DVRIP* )pBuff;
		if (pTmp->dvrip_extlen + DVRIP_HEAD_T_SIZE < iLen)
		{
			return false;
		}
#endif
		if (iLen == DVRIP_HEAD_T_SIZE)
		{
			memcpy(m_stDVRIPHdr, pBuff, DVRIP_HEAD_T_SIZE);
			m_pBuffer = (char *)m_stDVRIPHdr;
			m_pIpHdr = (DVRIP* )m_pBuffer;
			m_iTotalLen = DVRIP_HEAD_T_SIZE;
		}
		else
		{
			

			if(m_pBuffer=(char*)malloc(iLen+1))
			{
				mallocflag=true;
				memcpy(m_pBuffer, pBuff, iLen);
				m_pIpHdr = (DVRIP* )m_pBuffer;
				m_pBuffer[iLen] = '\0';
				m_iTotalLen = iLen;
			}
			else
			{
				memcpy(m_stDVRIPHdr, pBuff, DVRIP_HEAD_T_SIZE);
				m_pBuffer = (char *)m_stDVRIPHdr;
				m_pIpHdr = (DVRIP* )m_pBuffer;
				m_iTotalLen = DVRIP_HEAD_T_SIZE;
			}
		}

		return true;
	}

	int CIVnPdu::packetBody(char* pData, int iLen)
	{
		if (iLen > 0)
		{
		   //有扩展数据
		  //  if (!m_pPacker)
		    {
		 //       m_pPacker = g_PacketManager.GetPacket(iLen+DVRIP_HEAD_T_SIZE);
			//	if (!m_pPacker)
			//	{
			//		return -1;
			//	}
		    }
			
		//	m_pPacker->SetLength(iLen+DVRIP_HEAD_T_SIZE);
		//	m_pBuffer = (char *)m_pPacker->GetBuffer();
		//	memset(m_pBuffer,0,m_pPacker->GetLength());
			if(m_pBuffer=(char*)malloc(iLen+DVRIP_HEAD_T_SIZE))
			{
				mallocflag=true;
				memcpy(m_pBuffer, m_pIpHdr, DVRIP_HEAD_T_SIZE);
				m_pIpHdr = (DVRIP* )m_pBuffer;
				m_pIpHdr->dvrip_extlen = iLen;
				m_iTotalLen = DVRIP_HEAD_T_SIZE+iLen;
		        // 针对有扩展数据填充时直接拷贝
				if (pData)
				{
					memcpy(m_pBuffer+DVRIP_HEAD_T_SIZE, pData, iLen);
				}
				else//在没有填充扩展数据前只存在有效长度为32字节，后续进行数据添加
				{
				//	m_pPacker->SetLength(DVRIP_HEAD_T_SIZE); 
				}
			}
			else
			{
				m_pBuffer = (char *)m_stDVRIPHdr;
				m_pIpHdr = (DVRIP* )m_pBuffer;
				m_iTotalLen = DVRIP_HEAD_T_SIZE;
			}
		}
		else
		{
			//只有头部32字节
			m_pBuffer = (char *)m_stDVRIPHdr;
			m_pIpHdr = (DVRIP* )m_pBuffer;
			m_iTotalLen = DVRIP_HEAD_T_SIZE;
		}

		//添加版本信息
		m_pIpHdr->dvrip_hl = DVRIP_HEAD_T_SIZE/4;
		m_pIpHdr->dvrip_v = DVRIP_VERSION;		
		return m_iTotalLen;
	}

}







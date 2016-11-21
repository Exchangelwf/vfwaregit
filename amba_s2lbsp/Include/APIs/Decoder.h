//
//  "$Id$"
//
//  Copyright (c)1992-2007, Shanghai VisionDigi Technology CO.LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		2010-01-12 eric.yu created.
//

#ifndef __DECODER_API_H__
#define __DECODER_API_H__

#include "Types.h"

#ifdef __cplusplus
extern "C" {
#endif
	/// \defgroup DecoderAPI API Capture
	/// ��·��������Ƶ����ӿڣ�������Memory�ӿڡ�
	///	\n ��������ͼ:
	/// \code
	///    ============================================
	///                   	|   
	///			    DecoderGetChannels
	///									       |                         
	///                DecoderCreate           |  
	///       +-------------|------------------+         
	///       | DecoderSetAudioVideoEnable     |
	///       |           	|           	   |	 
	///       |        DecoderStart     	   |   
	///       | +-----------|                  |   
	///       | |  DigitCaputurePutBuffer  	   |  
	///       | +-----------|                  |         
	///       |        DecoderStop             |         
	///       +-------------|------------------+         
	///               DecoderDestroy                       
	///                     |                            
	///    ============================================	


/// �������,ֵԽ���ʾԽ�������ӳ�Խ��
enum DECODER_POLICY
{
	decoderPolicyMoreRealTime	= -2,	///< ��ʵʱ
	decoderPolicyRealTime		= -1,	///< ʵʱ
	decoderPolicyDefault		= 0,	///< Ĭ�����
	decoderPolicyFluency		= 1,	///< ����
	decoderPolicyMoreFluency	= 2,	///< ������
};


/// dsp ������Ϣ�ṹ
typedef struct DECODER_DSPINFO 
{
		int   iChannel;					//����ͨ����
		char sVideoType[8];				//��Ƶ���ͣ�        ��MPG4�� �� H.264" ����4�ֽ��ı�
		int   iVideoWidth;				//��Ƶ��
		int   iVideoHeight;				//��Ƶ��
		int   iVideoFramesPerSecond;    //��Ƶƽ��֡��
		int   iFramesType;				//������֡   ��ЩD1����������Щ��֡��֡Ϊ0����Ϊ1
		char sAudioType[8];				//��Ƶ���ͣ����ַ�����ʾ       "NONE" "ADPM" "G711"  �ȵ�
		int   iAudioSampleKRate;		//��Ƶ������        8K�� 16K��,����ֵ8��16
		int   iKBPS;

}DECODER_DSPINFO, *PDECODER_DSPINFO;

//��ȡ����dsp��Ϣ��0��ʾ�ɹ�����0��ʾ��ȡʧ��
int DecoderGetDspInfo(int iChannel, DECODER_DSPINFO *pDspInfo);

/// ���֧�ֵ����ֽ���ͨ��
/// \retval ���ͨ����0��ʾ��֧�֡�
int DecoderGetChannels(void);

///�����������豸
/// \param [in] iChannel ͨ����
/// \retval 0  �����ɹ���
/// \retval !=0  ����ʧ�ܡ�
int DecoderCreate(int iChannel);

/// ���ٽ����豸
int DecoderDestroy(int iChannel);

/// ���ý������
/// \param [in] iChannel ͨ����
/// \param[in] iPolicy ����
/// \		   Ŀǰһ����5��(-2 -1 0 1 2),ֵԽ���ʾԽ�������ӳ�Խ��
/// \		   -2��ʾʵʱ����ã�2��ʾ��������ã�0Ĭ��
/// \retval 0  ���óɹ���
/// \retval !=0  ����ʧ�ܡ�
int DecoderSetPolicy(int iChannel, int iPolicy);

/// ���ý���
///int DecoderSetFormat(int iChannel, int imagesize);

/// ��ȡ����ķֱ��ʣ�ע�⣺�˺����ķ��غ͵�ǰ�ָ��й�
/// \param [in] iChannel ͨ����
/// \retval �ֱ��ʣ�����capture_size_t��
int DecoderGetImageSize(int iChannel);

/// ���뿪ʼ
/// \param [in] iChannel ͨ����
/// \retval 0  �ɹ���
/// \retval !=0  ʧ�ܡ�
int DecoderStart(int iChannel);

///������ֹͣ
/// \param [in] iChannel ͨ����
/// \retval 0  �ɹ���
/// \retval !=0  ʧ�ܡ�
int DecoderStop(int iChannel);

/// �������ݸ����������
/// \param [in] iChannel ͨ����
/// \param [in] pBuffer	����ָ��
/// \param [in] iLength	���ݳ���
/// \param [in] iTimeOut �������ݵĳ�ʱʱ�䣬��msΪ��λ
/// \retval 0  �ɹ���
/// \retval !=0  ʧ�ܡ�
int DecoderPutBuffer(int iChannel, void* pBuffer, int iLength, int iTimeOut);

typedef struct DECODER_BUF_GROUP
{
    uchar   *pu8Addr[32];
    uint   u32Len[32];
    int 	count;	//!����������
} DECODER_BUF_GROUP;

//!�Ŷ����������ȥ������0�ɹ���ȫ��ʹ�ã�����һ����������������
int DecodePutMultiBuf(int iChannel, DECODER_BUF_GROUP* pDecoderBuf, int iTimeOut);

/// �������ݰ��ṹ
typedef struct DECODER_BLOCK
{
	/// ��Ҫ�����ݿ��ֽ�����
	uint		length;
}DECODER_BLOCK;

int DecoderGetBuffer(int channel, DECODER_BLOCK *pBlock);

/// ��������ͨ����Ӧ��Ԥ��ͨ����
/// \param[in] iChannel ��ֵͨ���ţ�ȡֵ0~DecoderGetChannels()
/// \param[in] targetDisplayChannel����ʾͨ���ţ�ȡֵ0~15
/// \retval 0  �ɹ���
/// \retval !=0  ʧ�ܡ�
int DecoderMapChannel(int iChannel, int targetDisplayChannel);

/// ȡ������ͨ������ʾͨ���Ķ�Ӧ��ϵ
/// \param[in] iChannel ��ֵͨ���ţ�ȡֵ0~DecoderGetChannels()
/// \retval 0  �ɹ���
/// \retval !=0  ʧ�ܡ�
int DecoderUnmapChannel(int iChannel);

/// ��������ͨ����Ӧ����ʾͨ�����������ͨ��δ��map�������������󣬷���-1
/// ���򷵻���ʾͨ����(>=0)
/// \param[in] iChannel ��ֵͨ���ţ�ȡֵ0~DecoderGetChannels()
/// \return �ɹ�������ʾͨ���ţ�ʧ�ܷ���-1
int DecoderGetMappedChannel(int iChannel);

//���ý�������Ƶʹ��
/// \param [in] iChannel ͨ���� ȡֵ0~DecoderGetChannels()-1
/// \param [in] iAudioEnable	��Ƶ����ʹ��
/// \param [in] iVideoEnable	��Ƶ����ʹ��
/// \retval 0  �ɹ���
/// \retval !=0  ʧ�ܡ�
////createʱĬ��ȫ����
int  DecoderSetAVEnable(int iChannel,int iAudioEnable,int iVideoEnable);

//���ý�������Ƶʹ��
/// \param [in] iChannel ͨ���� ȡֵ0~DecoderGetChannels()-1
/// \param [out] piAEnable	��Ƶ����ʹ��
/// \param [out] piVEnable	��Ƶ����ʹ��
/// \retval 0  �ɹ���
/// \retval !=0  ʧ�ܡ�
int  DecoderGetAVEnable(int iChannel,int *piAEnable,int *piVEnable);

/// ��ս��뻺�壬������ѰI֡���ϲ㶪����ʱ�����
/// 
/// \param [in] channel ͨ���š�
/// \retval 0  ��ճɹ�
/// \retval <0  ���ʧ��
int DecoderFlush(int iChannel);

#ifdef __cplusplus
}
#endif
#endif//__DECODER_API_H__


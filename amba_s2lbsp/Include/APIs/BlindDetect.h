

#ifndef __BLIND_DETECT_API_H__
#define __BLIND_DETECT_API_H__

#include "DVRDEF.H"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup BlindDetectAPI API Blind Detect
/// ȫ����Χ�������ȵ��ڵ����ӿڡ�
///	\n ��������ͼ:
/// \code
///    ===========================
///                |
///        *BlindDetectGetCaps
///                |
///         BlindDetectCreate
///    +-----------|
///    |   BlindDetectGetState
///    |   BlindDetectSetParameter
///    +-----------|
///       BlindDetectDestory
///                |
///    ===========================
/// \endcode
/// @{

/// ��������豸���Խṹ
typedef struct BLIND_DETECT_CAPS
{
	uint Enabled;			///< ��1��ʾ֧�ֺ�����⣬��0��ʾ��֧�ֺ�����⡣
}BLIND_DETECT_CAPS;

/// ����������
typedef struct BLIND_DETECT_PARAM
{
	uchar	Level;            	///< �ȼ�	0 - 3 // 0 - 30
	int		Enable;				///< Ϊ1��ʾʹ�ܣ�Ϊ0ʱ��ʾ����
}BLIND_DETECT_PARAM;


/// ������������豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int BlindDetectCreate(void);


/// ���ٺ�������豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int BlindDetectDestory(void);


/// ִ�к�����⡣�������ء�
/// 
/// \param [out] pData ָ��һ��uintֵ��ָ�룬uintֵ�Ǹ�����������ͨ����ƽ״̬��
///       ���롣��ͨ���ڵ�λ����ͨ���ڸ�λ���ߵ�ƽ��1���͵�ƽ��0�������ڵ�ͨ����0��
/// \retval 0  ���ɹ�
/// \retval <0  ���ʧ��
int BlindDetectGetState(uint* pData);


/// ���ú�����Ⲷ��Ĳ�����
/// 
/// \param [in] channel ͨ���š�
/// \param [in] pParam ָ������������ṹBLIND_DETECT_ PARAM��ָ�롣
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int BlindDetectSetParameter(int channel, BLIND_DETECT_PARAM *pParam);


/// �õ�����֧�ֵ����ԡ�
/// 
/// \param [out] pCaps ָ��������ԽṹBLIND_DETECT _CAPS��ָ�롣
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int BlindDetectGetCaps(BLIND_DETECT_CAPS * pCaps);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__BLIND_DETECT_API_H__


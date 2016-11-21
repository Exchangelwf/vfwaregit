
#include "MotionDetect.h"
#include "ipc_inc.h"
#include "Motion.h"
#include <time.h>
#include <string.h>
/// \defgroup MotionDetectAPI API Motion Detect
/// ������������ȵĶ�̬���ӿڡ�
///	\n ��������ͼ:
/// \code
///    ============================
///                  |
///         *MotionDetectGetCaps
///                  |
///          MotionDetectCreate
///     +------------|
///     |   MotionDetectGetState
///     | MotionDetectSetParameter
///     |   MotionDetectGetResult
///     |   MotionDetectShowHint
///     +------------|
///         MotionDetectDestory
///                  |
///    ============================
/// \endcode
/// @{

typedef struct _MotionParamStatus
{
	unsigned char 	 	levelParam;
	unsigned char 		enable;//1enable 0 disable

	unsigned int 	ResultMoarea[18];
	unsigned short 	ResultareaNum;

	time_t 			ResultTime;
}MotionParamStatus;
static MotionParamStatus gMoParamStatus = {0};

static int total_det_num = 0;

int print_objdata(unsigned int *pArea)
{
	int i = 0;
	int j = 0;
	for (i = 0; i < 23; i++){
			printf ("%02d",i - 1);
	}
	printf ("\n");
	for (i = 0; i < 18; i++){
		printf ("%c ", (char)('a' + i));
		for (j = 0; j < 22; j++){
			if (1 << (31 - j)/*�������Ҵ�ӡ*/ & pArea[i]) printf ("1 ");
			else printf ("0 ");
		}
		printf ("\n");
	}

	printf ("#####################################\n");
	return 0;
	
}

static void stMotionDetectGetResultTask(const u8 *md_event)
{
	int j;
	int total = 0;
	int i = 0;
	int bit = 0;
	unsigned int area[18] = {0};
	md_motbuf_evt_t *md_evt = (md_motbuf_evt_t *)md_event;
	if (gMoParamStatus.enable)
	{
		for (j=0; j<MD_MAX_ROI_N; j++) {

			if ((j != 0) && (j%22 == 0))
			{
				bit = 0;
				i++;
			}
		
			if (md_evt[j].valid == 1) {
				//printf("motion_flag ========== %d\n", md_evt[j].motion_flag);
				#if 0
				bit = (31-j%22);
				area[i] |= (1<<bit);
				#endif
				if (md_evt[j].motion_flag == MOT_START)
				{
					total++;
					#if 1
					bit = (31-j%22);
					area[i] |= (1<<bit);
					#endif
					//printf("\tROI#%d Motion Start!\n", j);
					//printf("motion_pos  left %d  top %d  width %d height %d\n",
					//		md_evt[j].x, md_evt[j].y,
					//		md_evt[j].width,  md_evt[j].height);					
				}
				else if (md_evt[j].motion_flag == MOT_END)
				{
					//printf("\tROI#%d Motion End!\n", j);		
				}
				//else if (md_evt[j].motion_flag == MOT_HAS_STARTED)
				//{
					//total++;
					//bit = (31-j%22);
					//area[i] |= (1<<bit);
					//printf("\tROI#%d Motion Has Start!\n", j);
					//printf("motion_pos  left %d  top %d  width %d height %d\n",
					//		md_evt[j].x, md_evt[j].y,
					//		md_evt[j].width,  md_evt[j].height);					
				//}
		   }
		}
	}

	if (total >= 1)
	{	
		//print_objdata(area);
		printf("#######detect total num = %d#########\n", total);
		gMoParamStatus.ResultareaNum = total;
		memcpy(gMoParamStatus.ResultMoarea, area, sizeof(area));
		time(&gMoParamStatus.ResultTime);
	}

}

/// ������̬����豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int MotionDetectCreate(void)
{
	printf ("[%s][%d][%s]\n", __FILE__, __LINE__, __func__); 
	//return 0;
	gMoParamStatus.enable 			= 0;
	gMoParamStatus.levelParam 		= 0;
	memset(gMoParamStatus.ResultMoarea, 0, sizeof(gMoParamStatus.ResultMoarea));
	gMoParamStatus.ResultareaNum 	= 0;
	
	set_rois_config();

	return md_motbuf_callback_setup(stMotionDetectGetResultTask);
}


/// ���ٶ�̬����豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int MotionDetectDestory(void)
{
	md_motbuf_thread_stop();
	md_motbuf_deinit();
	printf("\nDeinit........Yes!\n");
	return 0;

}


/// ִ�ж�̬��⡣�������ء�
/// 
/// \param [out] pData ָ��һ��uintֵ��ָ�룬uintֵ�Ǹ�����������ͨ����ƽ״̬
///        �����롣��ͨ���ڵ�λ����ͨ���ڸ�λ���ߵ�ƽ��1���͵�ƽ��0�������ڵ�ͨ
///        ����0��
/// \retval 0  ���ɹ�
/// \retval <0  ���ʧ��
int MotionDetectGetState(uint* pData)
{
	static time_t 	stgetsignaltime = 0;
	static uint   	stLastGetSignal = 0x00;
	//static time_t 	stsendtime	  = 0;
	//time_t curtime = {0};
	
	if (gMoParamStatus.ResultTime != stgetsignaltime)
	{
		if ((total_det_num > 5) && (gMoParamStatus.ResultareaNum >= total_det_num/80) ) //SuperJoki 10 10
		{
			*pData = 0x01;
		}
		else if((total_det_num <= 5) && (gMoParamStatus.ResultareaNum >= 1)) //SuperJoki 10 1
		{
			*pData = 0x01;
		}
		else
		{
			*pData = 0x00;
		}
		stgetsignaltime = gMoParamStatus.ResultTime;
	}
	else
	{
		*pData = 0x00;
	}

	if (stLastGetSignal == 0x01 && *pData == 0x01)
	{
		_printd("last singal = %d, *pdata = %d\n", stLastGetSignal, *pData);
		*pData = 0x00;
	}

	stLastGetSignal = *pData;

	if (*pData == 0x01)
	{
		_printd("Get a MotiohnDetect signal value = %d, ResultareaNum = %d last=%d\n", *pData, gMoParamStatus.ResultareaNum, stLastGetSignal);
	}

	return 0;
}


/// ��̬��Ⲷ�����ϸ�����
/// 
/// \param [in] channel ͨ���š�
/// \param [in] pResult ָ��̬�������ṹMOTION_DETECT_RESULT��ָ�롣
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int MotionDetectGetResult(int channel, MOTION_DETECT_RESULT *pResult)
{
	return 0;
}


/// �����Ƿ��ڶ�̬���ɹ������������ʾ��
/// 
/// \param [in] channel ͨ���š�
/// \param [in] enable Ϊ1��ʾ��ʾ��Ϊ0ʱ��ʾ����ʾ��
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int MotionDetectShowHint(int channel, int enable)
{
	return 0;
}


/// ִ�ж�̬��⡣�������ء�
/// 
/// \param [in] channel ͨ���š�
/// \param [in] pParam ָ��̬�������ṹMOTION_DETECT_PARAM��ָ�롣
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int MotionDetectSetParameter(int channel, MOTION_DETECT_PARAM *pParam)
{
	printf ("[%s][%d][%s] iLevel == %d\n", __FILE__, __LINE__, __func__, pParam->iLevel); 

	//return 0;
	int i, j;
	int total_num = 0;
	int threshold = 50;	//���Ȳ���ķ�ֵ  the algo dont't use the parameter
	int sensitivity = 50;
	int detect_num = 0;

	if (pParam->enable == 0x00)
	{
		gMoParamStatus.enable = 0;
	}
	else
	{
		gMoParamStatus.levelParam = pParam->iLevel;

		switch(pParam->iLevel)
		{
			case 1://��
				//threshold = 5;
				sensitivity = 5; 
				break;
			case 2:
				//threshold = 20;
				sensitivity = 25;
				break;
			case 3:
				//threshold = 35;
				sensitivity = 50;
				break;
			case 4:
				//threshold = 50;
				sensitivity = 65;
				break;
			case 5:
				//threshold = 75;
				sensitivity = 70;
				break;
			case 6:
				//threshold = 90;
				sensitivity = 73; //10
				break;

			default :
				break;
		}


		for (i = 0; i < MD_MAX_ROI_ROWS; ++i)
		{
			printf("-----win == %x\n", pParam->win[i]);
			for (j = 0; j < MD_MAX_ROI_LINES; ++j)
			{
				if (pParam->win[i]&0x01)
				{
					md_motbuf_set_roi_validity(total_num, 1);
					detect_num++;
				}
				else
				{
					md_motbuf_set_roi_validity(total_num, 0);
				}

				if (threshold != 50)
				{
					// Set threshold ֵԽ��Խ����
					md_motbuf_set_roi_threshold(total_num, threshold);
				}

				//if (sensitivity != 50)
				//{
					// Set sensitivity ֵԽСԽ����
					md_motbuf_set_roi_sensitivity(total_num, sensitivity);
				//}

				total_num++;
				pParam->win[i] = pParam->win[i] >> 1;
			}
			
		}
		gMoParamStatus.enable = 1;
		total_det_num = detect_num;
		printf("total_det_num = %d\n", total_det_num);
	}
	return 0;
}



/// �õ���̬���֧�ֵ����ԡ�
/// 
/// \param [out] pCaps ָ��̬������ԽṹMOTION_DETECT_CAPS��ָ�롣
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int MotionDetectGetCaps(MOTION_DETECT_CAPS * pCaps)
{
	printf ("[%s][%d][%s]\n", __FILE__, __LINE__, __func__);

	if(pCaps == NULL)
	{
		return -1;
	}
	#if 1
	pCaps->GridRows = 18;
	pCaps->GridLines = 22;
	#else
	pCaps->GridRows 	= 18;
	pCaps->GridLines 	= 32;
	#endif
	pCaps->Result = 1;
	pCaps->Hint = 0;
	pCaps->Enabled = 1;
	return 0; 
}

/// @} end of group



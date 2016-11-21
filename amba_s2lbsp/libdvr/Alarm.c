#include "Alarm.h"
#include "ipc_inc.h"
#include "Gpio.h"

#define LOW_LEVEL 0
#define HIGH_LEVEL 1

/// \defgroup AlarmAPI API Alarm
/// ������������˿ڣ����ڣ���ʹ�ã������ⲿ��ƽ������ߴ����̵��������ⲿ��·��
///	\n ��������ͼ:
/// \code
///    ===========================
///                |
///   *AlarmGetInSlots,AlarmGetOutSlots
///                |
///           AlarmCreate
///                |------------+
///       AlarmRead,AlarmWrite  |
///                |------------+
///           AlarmDestory
///                |
///    ===========================
/// \endcode
/// @{


/// �õ������˿ڵ�����ͨ����Ŀ��
/// \return �����˿ڵ�����ͨ����Ŀ
int AlarmGetInSlots(void)
{ 
	_printd("Test\n");
	return 2; 
}

/// �õ������˿ڵ����ͨ����Ŀ��
/// \return �����˿ڵ����ͨ����Ŀ
int AlarmGetOutSlots(void)
{ 
	_printd("Test\n"); 
	return 1; 
}

/// ���������豸
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int AlarmCreate(void)
{// return 0;
	_printd("Test\n");
	gpio_init();
	return 0; 
}

/// ���ٱ����豸
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int AlarmDestory(void)
{ 
	_printd("Test\n");
	return 0; 
}

/// ����������״̬��״̬�ı�ʱ�����ʱ���أ�����������
/// \param [out] pData ָ��һ��uintֵ��ָ�룬uintֵ�Ǹ�����������ͨ����ƽ״̬�����롣
///	       ��ͨ���ڵ�λ����ͨ���ڸ�λ���ߵ�ƽ��1���͵�ƽ��0�������ڵ�ͨ����0��
/// \retval >0  �������ֽ���
/// \retval <=0 ��ʧ��
int AlarmRead(uint* pData)
{//return 0;
	//_printd("Test\n");
	const uint GpioState1 = gpio_read(ConvertGpio(GPIO_ALARM_IN1));

	//Gpio_Read �ڵ͵�ƽ����0 �ߵ�ƽ����1 Ĭ���Ǹߵ�ƽ �����̽Ӻ�͵�ƽ
	if (LOW_LEVEL == GpioState1){
		printf("GPIO_ALM_IN read %d return %d\n", ConvertGpio(GPIO_ALARM_IN1), GpioState1);
		*pData |= 1;
		return 1;
	}
	
	const uint GpioState2 = gpio_read(ConvertGpio(GPIO_ALARM_IN2));
	if (LOW_LEVEL == GpioState2){
		printf("GPIO_ALM_IN read %d return %d\n", ConvertGpio(GPIO_ALARM_IN2), GpioState2);
		*pData |= 2;
		return 2;
	}
	
	*pData = 0;
	return 0;
}

/// д��ȡ�������״̬��
/// \param [in] pData ָ��һ��uintֵ��ָ�룬uintֵ�Ǹ����������ͨ�������롣��ͨ����
///	       ��λ����ͨ���ڸ�λ����1����ߵ�ƽ����0����͵�ƽ�������ڵ�ͨ��λ��Ч��
/// \return >0  д����ֽ���
/// \retval <=0 дʧ��
//---------------------------//
//tiger//start
int ResetRead(uint* pData)
{//return 0;//��Ӳ����λ����Ҫ�������
	const uint GpioResetState = gpio_read(ConvertGpio(GPIO_SYS_RST));
	//Gpio_Read �ڵ͵�ƽ����0 �ߵ�ƽ����1 Ĭ���Ǹߵ�ƽ ���¸�λ����͵�ƽ
	if (HIGH_LEVEL== GpioResetState){
		//_printd("GPIO_SYS_RST read %d return %d\n", GPIO_SYS_RST, GpioResetState);
		return 0;
	}
	if (LOW_LEVEL == GpioResetState){
		_printd("GPIO_SYS_RST read %d return %d\n", ConvertGpio(GPIO_SYS_RST), GpioResetState);
		*pData = 1;
		return 1;
	}
	*pData = 0;
    return 0;
}
int C2BRead(uint * pData){ _printd("Test\n"); return 0; }
int reset_in_read(unsigned int * pdata){ _printd("Test\n"); return 0; }
int c2b_in_read(unsigned int * pdata){ _printd("Test\n"); return 0; }
int tiger_reset_in_read(void){ _printd("Test\n"); return 0; }
//tiger//stop
//---------------------------//
int AlarmWrite(uint* pData)
{//return 0;
	_printd("*pdata=[%x]\n", *pData);
	if (*pData & 0x01) {
		printf("Set GPIO_ALM_OUT = 0\n");
		gpio_set0(ConvertGpio(GPIO_ALARM_OUT));
	}else{
		printf("Set GPIO_ALM_OUT = 1\n");
		gpio_set1(ConvertGpio(GPIO_ALARM_OUT));
	}

return 0; 
}
/// @} end of group


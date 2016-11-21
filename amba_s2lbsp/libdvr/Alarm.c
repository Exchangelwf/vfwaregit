#include "Alarm.h"
#include "ipc_inc.h"
#include "Gpio.h"

#define LOW_LEVEL 0
#define HIGH_LEVEL 1

/// \defgroup AlarmAPI API Alarm
/// 报警输入输出端口（串口）的使用，接受外部电平输入或者触发继电器控制外部电路。
///	\n 调用流程图:
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


/// 得到报警端口的输入通道数目。
/// \return 报警端口的输入通道数目
int AlarmGetInSlots(void)
{ 
	_printd("Test\n");
	return 2; 
}

/// 得到报警端口的输出通道数目。
/// \return 报警端口的输出通道数目
int AlarmGetOutSlots(void)
{ 
	_printd("Test\n"); 
	return 1; 
}

/// 创建报警设备
/// \retval <0 创建失败
/// \retval 0 创建成功
int AlarmCreate(void)
{// return 0;
	_printd("Test\n");
	gpio_init();
	return 0; 
}

/// 销毁报警设备
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int AlarmDestory(void)
{ 
	_printd("Test\n");
	return 0; 
}

/// 读报警输入状态，状态改变时或出错时返回，否则阻塞。
/// \param [out] pData 指向一个uint值的指针，uint值是各个报警输入通道电平状态的掩码。
///	       低通道在低位，高通道在高位。高电平置1，低电平置0，不存在的通道置0。
/// \retval >0  读出的字节数
/// \retval <=0 读失败
int AlarmRead(uint* pData)
{//return 0;
	//_printd("Test\n");
	const uint GpioState1 = gpio_read(ConvertGpio(GPIO_ALARM_IN1));

	//Gpio_Read 在低电平返回0 高电平返回1 默认是高电平 报警短接后低电平
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

/// 写读取报警输出状态。
/// \param [in] pData 指向一个uint值的指针，uint值是各个报警输出通道的掩码。低通道在
///	       低位，高通道在高位。置1输出高电平，置0输出低电平，不存在的通道位无效。
/// \return >0  写入的字节数
/// \retval <=0 写失败
//---------------------------//
//tiger//start
int ResetRead(uint* pData)
{//return 0;//老硬件复位不需要软件控制
	const uint GpioResetState = gpio_read(ConvertGpio(GPIO_SYS_RST));
	//Gpio_Read 在低电平返回0 高电平返回1 默认是高电平 按下复位键后低电平
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


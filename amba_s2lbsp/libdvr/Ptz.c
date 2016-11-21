
#include "Ptz.h"
#include "ipc_inc.h"
#include "Gpio.h"
#include <sys/termios.h>

#define	PTZ_DISABLE	0

#define COM_DEV_PATH       "/dev/ttyS0"
static int gComFd = 0;
pthread_mutex_t   serial_mutex = PTHREAD_MUTEX_INITIALIZER;

//在PtzCreate 调用之前调用有效
//默认为阻塞模式
//iFlag 1 非阻塞0,阻塞
int PtzSetNonBlock(int iFlag)
{
	_printd("iFlag = %d\n", iFlag);
	return 0;
}

/// 创建云台设备
/// 
/// \param	无
/// \retval <0 创建失败
/// \retval 0 创建成功
int PtzCreate(void)
{
	_printd("PtzCreate\n");
	if(PTZ_DISABLE)
	{
		return 0;
	}
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//简易版
	{
		return 0;
	}
	gpio_set1(ConvertGpio(GPIO_RS485));//设置为高，收发控制
	if(0>(gComFd = open(COM_DEV_PATH, O_RDWR|O_NOCTTY|O_NONBLOCK))){
		perror("Can't open serial port");
		gComFd = 0;
		return -1;
	}
	return 0;
}


/// 销毁云台设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int PtzDestory(void)
{
	_printd("PtzDestory\n");
	if(PTZ_DISABLE)
	{
		return 0;
	}
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//简易版
	{
		return 0;
	}
	
	if (0 != gComFd)	close(gComFd);
	return 0;
}

	
/// 打开云台设备。
/// 
/// \param 无
/// \retval 0  打开成功
/// \retval <0  打开失败
int PtzOpen(void)
{
	_printd("PtzOpen\n");
	return 0;
}


/// 设置云台设备属性。
/// 
/// \param [in] pattr 云台属性结构PTZ_ATTR指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int PtzSetAttribute(PTZ_ATTR *pattr)
{
	if(PTZ_DISABLE)
	{
		return 0;
	}
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//简易版
	{
		return 0;
	}
		
	struct termios	Termios = {0};
	int speed = 0;
	_printd("PtzSetAttribute\n");
	if(gComFd == 0){
		_printd("gComFd == 0\n");
		return -1;
	}

	tcflush(gComFd, TCIFLUSH);
	if (tcgetattr(gComFd, &Termios) < 0) {
		perror("Can't get port settings");
		return -1;
	}

	cfmakeraw(&Termios);

	///设置波特率
	switch (pattr->baudrate)
	{
		case 50:
			speed = B50;
		break;
		case 75:
			speed = B75;
		break;
		case 110:
			speed = B110;
		break;
		case 134:
			speed = B134;
		break;
		case 150:
			speed = B150;
		break;
		case 200:
			speed = B200;
		break;
		case 300:
			speed = B300;
		break;
		case 600:
			speed = B600;
		break;
		case 1200:
			speed = B1200;
		break;
		case 2400:
			speed = B2400;
		break;
		case 4800:
			speed = B4800;
		break;
		case 9600:
			speed = B9600;
		break;
		case 19200:
			speed = B19200;
		break;
		case 38400:
			speed = B38400;
		break;
		case 57600:
			speed = B57600;
		break;
		case 115200:
			speed = B115200;
		break;
		case 230400:
			speed = B230400;
		break;
		case 460800:
			speed = B460800;
		break;
		case 500000:
			speed = B500000;
		break;
		case 576000:
			speed = B576000;
		break;
		case 921600:
			speed = B921600;
		break;
		case 1000000:
			speed = B1000000;
		break;
		case 1152000:
			speed = B1152000;
		break;
		case 1500000:
			speed = B1500000;
		break;
		default:
			_printd("unsupported baud rate %d\n", pattr->baudrate);
		return FALSE;
   	}

	///设置数据位
	switch (pattr->databits)
	{
		case 5:
			Termios.c_cflag |= CS5;
		break;
		case 6:
			Termios.c_cflag |= CS6;
		break;
		case 7:
			Termios.c_cflag |= CS7;
		break;
		case 8:
			Termios.c_cflag |= CS8;
		break; 
		default:
			break;
			return FALSE;
   	}

	//设置奇偶位
	switch (toupper(pattr->parity))
	{
		case 'N':
		break;
		case 'E':
			Termios.c_cflag |= PARENB;
		break;
		case 'O':
			Termios.c_cflag |= PARODD;
		break;
		
		default:	
			break;
			
		return FALSE;
   	}

	//停止位
	switch (pattr->stopbits)
	{
		case 1:
		break;
		case 2:
			Termios.c_cflag |= CSTOPB;
		break;
		default: 			
			break;
		return FALSE;
	}

	Termios.c_cflag |= CLOCAL;
	Termios.c_cflag &= ~CRTSCTS;
	cfsetospeed(&Termios, speed);
	cfsetispeed(&Termios, speed);

	if(tcsetattr(gComFd,TCSANOW,&Termios)<0){
		perror("tcsetattr");
		return -1;
	}
	tcflush(gComFd, TCIFLUSH);
		
	return 0;
}


/// 关闭云台设备，当前读写操作立即返回。
/// 
/// \param 无
/// \retval 0  关闭成功
/// \retval <0  关闭失败
int PtzClose(void)
{
	_printd("PtzClose\n");
	return 0;
}


/// 写云台控制命令，一直等到写入指定的字节或出错时才返回，否则阻塞。
/// 
/// \param [in] pData 数据缓冲的指针。
/// \param [in] len 要写入的字节数
/// \retval >0  写入的字节数
/// \retval <=0 写失败
int PtzWrite(void* pData, uint len)
{	
	if(PTZ_DISABLE)
	{
		return 0;
	}

	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//简易版
	{
		return 0;
	}
	
	int nWrite = 0;
	int i = 0;
	_printd("PtzWrite:");
	for (i = 0; i < len;i++){
		printf("[%x] ", *((char *)pData + i));
	}
	printf("\n");
  	pthread_mutex_lock(&serial_mutex);
	nWrite = write (gComFd, pData, len);
	pthread_mutex_unlock(&serial_mutex);
	if (nWrite < len){
		_printd("write len = %d, need write len = %u", nWrite, len);
	}
	return nWrite;
}

/// 读云台数据，一直等到读到指定的字节或出错时才返回，否则阻塞。
/// 
/// \param [in] pData 数据缓冲的指针。
/// \param [in] len 要读出的字节数
/// \return >0  读出的字节数
/// \retval <=0 读失败
int PtzRead(void* pData, uint len)
{
	_printd("PtzRead\n");
	return 0;
}

// 明景串口升级使用
int PtzUpdate(void* pData,unsigned long len)
{
	_printd("PtzUpdate\n");
	return 0;
}


//int UartExtendWrite(UART_EXTEND *, void *pdata, uint len){  return 0;}
//int UartExtendRead(UART_EXTEND *, void *pdata, uint len){  return 0;}

/// @} end of group



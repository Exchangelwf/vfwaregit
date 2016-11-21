
#include "Ptz.h"
#include "ipc_inc.h"
#include "Gpio.h"
#include <sys/termios.h>

#define	PTZ_DISABLE	0

#define COM_DEV_PATH       "/dev/ttyS0"
static int gComFd = 0;
pthread_mutex_t   serial_mutex = PTHREAD_MUTEX_INITIALIZER;

//��PtzCreate ����֮ǰ������Ч
//Ĭ��Ϊ����ģʽ
//iFlag 1 ������0,����
int PtzSetNonBlock(int iFlag)
{
	_printd("iFlag = %d\n", iFlag);
	return 0;
}

/// ������̨�豸
/// 
/// \param	��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int PtzCreate(void)
{
	_printd("PtzCreate\n");
	if(PTZ_DISABLE)
	{
		return 0;
	}
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//���װ�
	{
		return 0;
	}
	gpio_set1(ConvertGpio(GPIO_RS485));//����Ϊ�ߣ��շ�����
	if(0>(gComFd = open(COM_DEV_PATH, O_RDWR|O_NOCTTY|O_NONBLOCK))){
		perror("Can't open serial port");
		gComFd = 0;
		return -1;
	}
	return 0;
}


/// ������̨�豸
/// 
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int PtzDestory(void)
{
	_printd("PtzDestory\n");
	if(PTZ_DISABLE)
	{
		return 0;
	}
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//���װ�
	{
		return 0;
	}
	
	if (0 != gComFd)	close(gComFd);
	return 0;
}

	
/// ����̨�豸��
/// 
/// \param ��
/// \retval 0  �򿪳ɹ�
/// \retval <0  ��ʧ��
int PtzOpen(void)
{
	_printd("PtzOpen\n");
	return 0;
}


/// ������̨�豸���ԡ�
/// 
/// \param [in] pattr ��̨���ԽṹPTZ_ATTRָ�롣
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int PtzSetAttribute(PTZ_ATTR *pattr)
{
	if(PTZ_DISABLE)
	{
		return 0;
	}
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//���װ�
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

	///���ò�����
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

	///��������λ
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

	//������żλ
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

	//ֹͣλ
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


/// �ر���̨�豸����ǰ��д�����������ء�
/// 
/// \param ��
/// \retval 0  �رճɹ�
/// \retval <0  �ر�ʧ��
int PtzClose(void)
{
	_printd("PtzClose\n");
	return 0;
}


/// д��̨�������һֱ�ȵ�д��ָ�����ֽڻ����ʱ�ŷ��أ�����������
/// 
/// \param [in] pData ���ݻ����ָ�롣
/// \param [in] len Ҫд����ֽ���
/// \retval >0  д����ֽ���
/// \retval <=0 дʧ��
int PtzWrite(void* pData, uint len)
{	
	if(PTZ_DISABLE)
	{
		return 0;
	}

	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//���װ�
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

/// ����̨���ݣ�һֱ�ȵ�����ָ�����ֽڻ����ʱ�ŷ��أ�����������
/// 
/// \param [in] pData ���ݻ����ָ�롣
/// \param [in] len Ҫ�������ֽ���
/// \return >0  �������ֽ���
/// \retval <=0 ��ʧ��
int PtzRead(void* pData, uint len)
{
	_printd("PtzRead\n");
	return 0;
}

// ������������ʹ��
int PtzUpdate(void* pData,unsigned long len)
{
	_printd("PtzUpdate\n");
	return 0;
}


//int UartExtendWrite(UART_EXTEND *, void *pdata, uint len){  return 0;}
//int UartExtendRead(UART_EXTEND *, void *pdata, uint len){  return 0;}

/// @} end of group



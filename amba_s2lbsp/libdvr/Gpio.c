#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <basetypes.h>
#include <amba_debug.h>
#include "Gpio.h"

int gpio_fd = -1;

static unsigned char GpioTable2[13]={33,63,255,54,2,33,4,91,92,113,37,255,24};//new board
static unsigned char GpioTable1[13]={33,255,255,54,65,255,66,255,67,255,37,113,255};
//static unsigned char GpioTable[7]={33,255,54,65,66,67,113};//v2
//static unsigned char GpioTable[6]={33,255,255,255,255,38};//v1

char *GetVersioninfo()
{
		static char verdata[64];
		int iRet = 0;
		FILE *fpVer = NULL;
		fpVer = fopen("/etc/version", "r");
		if (NULL == fpVer) {
			sprintf (verdata, "open ver file error");
			goto ReadOver;
		}
		iRet = fscanf(fpVer, "%s", verdata);
		if(iRet <= 0) {
			sprintf (verdata, "fscanf return var <= 0");
			fclose(fpVer);
			goto ReadOver;
		}
		fclose(fpVer);
ReadOver:
		return verdata;

}

int ConvertGpio(unsigned char Gpiofun)
{
	if(NULL != strcasestr(GetVersioninfo(), "V10")){
		return GpioTable2[Gpiofun];
	}else
		return GpioTable1[Gpiofun];
}

int gpio_init(void)
{
	if(gpio_fd < 0)
	{
		if ((gpio_fd = open("/dev/ambad", O_RDWR, 0)) < 0) {
			perror("/dev/ambad");
			return -1;
		}
	}
	return 0;
}

int gpio_set0(int gpio_id)
{
	if(gpio_id == 255){
		return -1;
	}
	struct amba_vin_test_gpio	gpio;
	gpio.id = gpio_id;
	gpio.data = 0;

	if (ioctl(gpio_fd, AMBA_DEBUG_IOC_SET_GPIO, &gpio) < 0) {
		perror("AMBA_DEBUG_IOC_SET_GPIO");
		return -1;
	}
	return 0;
}

int gpio_set1(int gpio_id)
{
	if(gpio_id == 255){
		return -1;
	}
	struct amba_vin_test_gpio	gpio;
	gpio.id = gpio_id;
	gpio.data = 1;

	if (ioctl(gpio_fd, AMBA_DEBUG_IOC_SET_GPIO, &gpio) < 0) {
		perror("AMBA_DEBUG_IOC_SET_GPIO");
		return -1;
	}
	return 0;
}

int gpio_read(int gpio_id)
{
	if(gpio_id == 255){
		return -1;
	}
	struct amba_vin_test_gpio	gpio;
	gpio.id = gpio_id;
	if (ioctl(gpio_fd, AMBA_DEBUG_IOC_GET_GPIO, &gpio) < 0) {
		perror("AMBA_DEBUG_IOC_GET_GPIO");
		return -1;
	}

	return gpio.data;
}


#if 0
int main(int argc, char const *argv[])
{
	/* code */
	int fd = 0;

	int gpio_id = 52;

	if ((fd = open("/dev/ambad", O_RDWR, 0)) < 0) {
		perror("/dev/ambad");
		return -1;
	}

	gpio_init(fd);
	int data = -1;
	//gpio_set0(gpio_id);
	while (1)
	{

		gpio_id = 52;
		gpio_set0(gpio_id);
		//data = gpio_read(gpio_id);
		//printf("data 0x%08x\n", data);
		usleep(200*1000);
		gpio_set1(gpio_id);
		//data = gpio_read(gpio_id);
		//printf("data 0x%08x\n", data);
		
		gpio_id = 50;
		gpio_set0(gpio_id);
		//data = gpio_read(gpio_id);
		//printf("data 0x%08x\n", data);
		usleep(200*1000);
		gpio_set1(gpio_id);
		//data = gpio_read(gpio_id);
		//printf("data 0x%08x\n", data);
		gpio_id = 46;
		gpio_set0(gpio_id);
		//data = gpio_read(gpio_id);
		//printf("data 0x%08x\n", data);
		usleep(200*1000);
		gpio_set1(gpio_id);
		//data = gpio_read(gpio_id);
		//printf("data 0x%08x\n", data);

		gpio_id = 54;
		gpio_set0(gpio_id);
		//data = gpio_read(gpio_id);
		//printf("data 0x%08x\n", data);
		usleep(200*1000);
		gpio_set1(gpio_id);
		//data = gpio_read(gpio_id);
		//printf("data 0x%08x\n", data);

		gpio_id = 13;
		gpio_set0(gpio_id);
		//data = gpio_read(gpio_id);
		//printf("data 0x%08x\n", data);
		usleep(200*1000);
		gpio_set1(gpio_id);
		//data = gpio_read(gpio_id);
		//printf("data 0x%08x\n", data);

		gpio_id = 16;
		gpio_set0(gpio_id);
		//data = gpio_read(gpio_id);
		//printf("data 0x%08x\n", data);
		usleep(200*1000);
		gpio_set1(gpio_id);
		//data = gpio_read(gpio_id);
		//printf("data 0x%08x\n", data);

		gpio_id = 45;
		gpio_set0(gpio_id);
		//data = gpio_read(gpio_id);
		//printf("data 0x%08x\n", data);
		usleep(200*1000);
		gpio_set1(gpio_id);
		//data = gpio_read(gpio_id);
		//printf("data 0x%08x\n", data);

	}
	return 0;
}
#endif

#include "Ide.h"
#include "Types.h"
#include "ipc_inc.h"

#include "DVRDEF.H"
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/types.h>
#include <linux/string.h>	
#include <linux/hdreg.h>
#include <linux/fs.h>
#include <sys/io.h>
#include <linux/unistd.h>
#include<unistd.h>  //F_OK
#include <fcntl.h>   //O_RDWR
#include <errno.h>

#include <scsi/scsi.h>
#include <scsi/sg.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef uint64 u64;
typedef uint u32;
typedef uchar u8;

#define SECTOR_SIZE 512

IDE_INFO64 g_ide_info;

#define IDLE_IMMED			0xE1
#define STANDBY				0xE2

#define MAX_DISK_NUM		2

#define SCSI_DEV_HEAD	"/dev/hhda"
#define USB_PROC_FILE	"/proc/scsi/usb-storage/"
#define PROC_PORT		"/proc/sata_ports"

int dev_has_inited = 0;
int g_sata_ports = 8;
static struct esata_config g_esata_config;

void esata_read_config(struct esata_config *config);

typedef struct hdio_ioctl_command 
{
	unsigned char cmd;
	unsigned char nsect;
	unsigned char feature;
	unsigned char sectors;
} Hdio_Ioctl_Command;

struct ide_dev_info
{
	char		name[20];
	int		fd;

	unsigned long	err_cnt;
	char		mode[10];
	char		product[30];
	int			ide_msk;
	int			ide_bad;
	char		present;
	char		members;
	char		order;
	char		channel;
	char		smart_status;
	char		wakeupByIdle;
	char		nwakeupByIdle;
	
	unsigned long	capacity;
	unsigned long	sectors;
} g_dev_info[MAX_DISK_NUM];

//规避坏盘，3515芯片存在	
#ifdef HI_SDK3515_103
#define PROC_NAME "/proc/ata_error"
struct ide_disk_err_info
{
	int num;
	int error;
};
static struct ide_disk_err_info g_err_info[MAX_DISK_NUM];
#endif

#define 	ATA_DRQ			(1 << 3)	/* data request i/o */
#define		ATA_ERR			(1 << 0)  /* have an error */

#define 	ATA_UNC			(1 << 6)	/*media error */
#define 	ATA_ICRC			(1 << 7) /* ata bus error */
#define 	ATA_TIMEOUT		(1 << 0) /* timeout */


#define 	DRIVER_SENSE 					0x08
#define 	SAM_STAT_CHECK_CONDITION 	0x02
#define 	ATA_PASS_THRU_12				12
#define 	ATA_12							0xa1
#define 	ATA_PASS_THRU_16				16
#define 	ATA_16							0x85
#define 	DMA_CMD						(6 << 1)
#define	NO_DATA_CMD					(3 << 1)
#define 	FOR_SENSE_DATA				0x20
#define 	LBA_48							(1 << 0)
#define   LBA_ADDR						(1 << 6)

#define NA								0x00
#define READ_LOG_EXT					0x2f

#define SECURITY_SET_PASSWD_CMD		0xf1
#define SECURITY_UNLOCK_CMD			0xf2
#define SECURITY_ERASE_PREPARE_CMD	0xf3
#define SECURITY_ERASE_UNIT_CMD		0xf4
#define SECURITY_FREEZE_LOCK_CMD		0xf5
#define SECURITY_DISABLE_PASSWD_CMD	0xf6

#define DEVICE_RESET_CMD				0x08
#define IDENTITY_DEVICE_CMD			0xec

#define LOG_DIRECTORY					0x00
#define SUMMARY_ERROR_LOG				0x01
#define COMPREHENSIVE_ERROR_LOG		0x02	/*  lba28 */
#define COMPREHENSIVE_ERROR_LOG_EXT	0x03	/* lba28 and lba48 */

enum smart_featur{
	SMART_READ_DATA = 0xd0,
	SMART_THRESHOLD = 0xd1,
	SMART_READ_LOG	= 0xd5,
	SMART_ENABLE_OPERATIONS = 0xd8,
	SMART_RETURN_STATUS = 0xda,
};

enum hd_error{

	BAD_TARGET = -2, /* For ata driver */
	ATA_BUS_ERROR = -3,
	TIMEOUT	= -4,
	MEDIA_ERROR	= -5,
};

enum{
	ATA_CMD_READ		= 0xC8,
	ATA_CMD_READ_EXT	= 0x25,
	ATA_CMD_WRITE		= 0xCA,
	ATA_CMD_WRITE_EXT	= 0x35,
	ATA_CMD_WRITE_FUA_EXT	= 0x3D,
	ATA_CMD_FPDMA_READ	= 0x60,
	ATA_CMD_FPDMA_WRITE	= 0x61,
	ATA_CMD_PIO_READ	= 0x20,
	ATA_CMD_PIO_READ_EXT	= 0x24,
	ATA_CMD_PIO_WRITE	= 0x30,
	ATA_CMD_PIO_WRITE_EXT	= 0x34,
	ATA_CMD_READ_MULTI	= 0xC4,
	ATA_CMD_READ_MULTI_EXT	= 0x29,
	ATA_CMD_WRITE_MULTI	= 0xC5,
	ATA_CMD_WRITE_MULTI_EXT	= 0x39,
	ATA_CMD_WRITE_MULTI_FUA_EXT = 0xCE,
	ATA_CMD_SMART = 0xb0,
	ATA_CMD_READ_LOG_EXT = 0x2f,
	ATA_CMD_WRITE_LOG = 0x3f,
	ATA_CMD_VERIFY = 0x40,
	ATA_CMD_VERIFY_EXT = 0x42,

	ATA_CMD_STANDBY = 0xe2,
	ATA_CMD_STANDBY_IMMEDIATE = 0xe0,
	ATA_CMD_SLEEP = 0xe6,
	ATA_CMD_CHECK_POWER_MODE = 0xe5,
	ATA_CMD_IDLE = 0xe3,
	ATA_CMD_IDLE_IMMEDIATE = 0xe1,
	ATA_CMD_IDENTITY = 0xec,
	ATA_CMD_SET_FEATURE = 0xef,
	ATA_CMD_FLUSH_CACHE	= 0xea,
};
enum feature{

	ENABLE_WRITE_READ_VERIFY = 0x0b,
	DISABLE_WRITE_READ_VERIFY = 0x8b,
	SPIN_UP = 0x07,
};
enum power_mode
{
//	STANDBY	=	0x00,
	IDLE	=  	0x80,
	ACTIVE		= 	0xff,
};
enum data_dirction{
	NO_DATA,
	PIO_DATA_IN,
	PIO_DATA_OUT,
	NOTHING,
};

static inline int lba_28_ok(u64 block, u32 n_block)
{
	return ((block + n_block) < ((u64)1 << 28)) && (n_block <= 256);
}

static inline int lba_48_ok(u64 block, u32 n_block)
{
	/* check the ending block number */
	return ((block + n_block) < ((u64)1 << 48)) && (n_block <= 65536);
}
static inline int ata_power_management_time(int value)
{

	switch(value){

		case 0:
			return value;
		case 1 ... 20:
			return (value*12);///
		case 21 ... 30:
			return 252;
		case 31 ... 330:
			return (value/30 + 240); ///
		default:
			return 255; ///default 21min 15sec
	}

	return 255;

}

//动态识别sd卡节点 mmcblk0p1节点则优先  zhongheyuabn 20141127
#include <dirent.h>
#define DIR_DEV "/dev"
#define AMBAS2L_MMC_DEV_FIRST "mmcblk0p1"
#define AMBAS2L_MMC_DEV_SECOUND "mmcblk0"

int discern_SDcare_node(char *SDcare_node, int str_size)
{
	memset(SDcare_node, 0, str_size);
	strcpy(SDcare_node, AMBAS2L_MMC_DEV_SECOUND);
	DIR *directory_dev;
	struct dirent *dirp;
	
	if(NULL == (directory_dev = opendir(DIR_DEV))) {
		printf("Can`t open %s\n",DIR_DEV);
		return -1;
	}
	while((dirp = readdir(directory_dev)) != NULL) {
		if(0 == strncmp(dirp->d_name, AMBAS2L_MMC_DEV_FIRST, 11)) {
			strcpy(SDcare_node, dirp->d_name);
		}
	}
	closedir(directory_dev);
	LIBDVR_PRINT("SD node is %s\n", SDcare_node);
	return 0;
}

static int sd_init(void)
{

	memset(&g_ide_info, 0, sizeof(IDE_INFO64));

	//char strName[64] = {'\0'};
	
	g_ide_info.ide_num = 0;
	g_ide_info.ide_port = 0;			
	g_ide_info.ide_msk = 0;
	g_ide_info.ide_bad = 0;

	char SDcare_node[16] = {'\0'};
	discern_SDcare_node(SDcare_node, 16);
	
	sprintf(g_dev_info[g_ide_info.ide_num].name, "/dev/%s",SDcare_node);

	LIBDVR_PRINT("Find %s\n", g_dev_info[g_ide_info.ide_num].name);
	g_dev_info[g_ide_info.ide_num].fd = open(g_dev_info[g_ide_info.ide_num].name, O_RDWR|O_SYNC);

	if (g_dev_info[g_ide_info.ide_num].fd < 0) 
	{
		ERR_PRINT("Open %s Failed!\n", g_dev_info[g_ide_info.ide_num].name);
		g_ide_info.ide_bad |= (1 << 0);
		return -1;
	}
	else
	{

		unsigned long long  v64;   
		unsigned long  longsectors=0;  

		if (ioctl(g_dev_info[g_ide_info.ide_num].fd, BLKGETSIZE64, &v64) == 0)
		{   
			/* Got bytes, convert to 512 byte sectors */  
			g_ide_info.ide_cap[g_ide_info.ide_num] = (v64>>9);
			g_dev_info[g_ide_info.ide_num].capacity = (v64>>9); ;   
		}   
		else if (ioctl(g_dev_info[g_ide_info.ide_num].fd, BLKGETSIZE, &longsectors) == 0)   
		{
			/* Needs temp of type long */  
			g_ide_info.ide_cap[g_ide_info.ide_num] = longsectors;
			g_dev_info[g_ide_info.ide_num].capacity = longsectors;	
		}else
		{
			ERR_PRINT("Get BLKGETSIZE Failed!");
			close(g_dev_info[g_ide_info.ide_num].fd);
			g_dev_info[g_ide_info.ide_num].fd = -1;
			g_ide_info.ide_bad |= (1 << 0);
		}

		
		g_ide_info.ide_num++;
	}
	
	g_ide_info.ide_msk |= (1 << 0);

	if (g_ide_info.ide_num == 0)
	{
		LIBDVR_PRINT("Can't Find Any Sata Dev\n");
	}

	return 0;
}
static int SDRW(unsigned short dev, unsigned short sectors, uint64 offs, void *pdat, int rw)
{
	loff_t		ret	= 0;
	size_t		count	= sectors << 9;		
	loff_t		addr = offs<< 9;
	int		devfd = g_dev_info[dev].fd;
	//int i = 0;
	
//	printf("\n\n===========SDRW-sectors:%d, offs:%llu\n\n", sectors, offs);
	if (devfd < 0)
	{
		ERR_PRINT("DEV INFO FD ERROR\n");
		return -1;
	}
	
	if (((unsigned int)pdat % 0x200))
	{
		ERR_PRINT("Read Address Not Align, 0x%x\n", (unsigned int)pdat);
		return -1;
	}

	ret  = lseek64(devfd, addr, SEEK_SET);

	if (ret ==-1)
	{
		
		printf("read llseek Failed %d\n",errno);
		return -1;
	}			
	
	if (rw == O_RDONLY)
	{
	 	ret = read(devfd, pdat, count);
	 	if (ret != count)
		{
			printf(" read fail...\n");

			return -1;
		}
	}
	else
	{
		ret = write(devfd, pdat, count);
		if(ret!=count)
		{
			printf("write error !\n");

			return -1;
		}
	}
	
	return 0;
}

int ide_init(void)
{
    int i = 0;

	if (dev_has_inited)
	{
		LIBDVR_PRINT("Do Init The Hard Disk One Time Enough.\n");
		return 0;
	}
	
	LIBDVR_PRINT("------ ide library : version 2.00, built at %s, %s -----\n", __DATE__, __TIME__);
    #if (0 & G_PROJECT)
	memset(&g_ide_info, 0, sizeof(IDE_INFO64));
	g_ide_info.ide_num = 0;
	g_ide_info.ide_port = 0;			
	g_ide_info.ide_msk = 0;
	g_ide_info.ide_bad = 0;
    g_dev_info[g_ide_info.ide_num].fd = -1;
    g_ide_info.ide_bad |= (1 << 0);
    #else
	sd_init();
    #endif
	dev_has_inited = 1;	
	return 0;
}


int ide_getinfo64(IDE_INFO64 *info)
{	
	memcpy(info, &g_ide_info, sizeof(IDE_INFO64));
	return info->ide_num;
}

int ide_read64(unsigned short dev, unsigned short sectors, uint64 offs, void *pdat)
{
	int ret =0;
	if (dev >= g_ide_info.ide_num)
	{
		ERR_PRINT("Select Dev = %d; Nax Ide_num = %d\n", dev, g_ide_info.ide_num);
		return -1;
	}
	ret = SDRW(dev, sectors, offs, pdat, O_RDONLY);
	return ret;
}

int ide_write64(unsigned short dev, unsigned short sectors, uint64 offs, void *pdat)
{
	int ret = 0;

	if (dev > g_ide_info.ide_num)
	{
		ERR_PRINT("Select Dev = %d; Nax Ide_num = %d\n", dev, g_ide_info.ide_num);
		return -1;
	}
	ret = SDRW(dev, sectors, offs, pdat, O_WRONLY);
	return ret;
}


int ide_setstandby(unsigned short  ide_no, int timer)
{
	return 0;
}


int ide_setstandby_multi(unsigned long ide_mask,int minutes)
{
	int dev = 0;

	for (dev = 0; dev < g_ide_info.ide_num; dev++)
	{
		ide_setstandby(dev, minutes);
	}
	
	return 0;
}

int ide_wakeup(unsigned short  ide_no)
{
	return 0;
}


void esata_read_config(struct esata_config *config)
{
	int fd,i;
    int n = 0;
	char ptr[ESATA_CONFIG_SIZE];

	if(config == NULL){

		return ;
	}
	
	config->sata_msk = 0;
	config->sata_num = -1;
	
    fd = open(ESATA_CONFIG,O_RDONLY);

    if(fd < 0){

        perror(ESATA_CONFIG"==>open");
        return ;
    }
	
	memset(ptr,0,ESATA_CONFIG_SIZE);
    n = read(fd,ptr,ESATA_CONFIG_SIZE);

	if(n < 0){

		perror("read");
		close(fd);
		return ;
	}

	for(i = 0; i < g_sata_ports;i++){

		if(ptr[i] == 'E' || ptr[i] == 'e'){

			config->sata_msk |= (1 << i);
		}
		else if(ptr[i] != 'S' && ptr[i] != 's'){
			LIBDVR_PRINT("WARNING:Some bad characters\n");
		}
	}

    config->sata_num = g_sata_ports;
	
	close(fd);

    return ;
}

struct esata_config *get_esata_config(void)
{
	return &g_esata_config;
}
int ide_get_esata(void)
{
	int i;
	struct esata_config *ec;

	ec = get_esata_config();
	
	for(i = 0; i < ec->sata_num; i++){

		if(ec->sata_num != -1){

			if(ec->sata_msk & (1 << i)){
				return i;
			}
		}
	}

	return -1;
}
void ide_set_esata(char *ptr)
{
	FILE	*fp = NULL;

	fp = fopen(ESATA_CONFIG,"w+");

	if(NULL == fp){

			perror(ESATA_CONFIG);
			return -1;
	}

	if(fwrite(ptr,strlen(ptr),1,fp) < 0){

	    perror("fwrite");
	    fclose(fp);
	    return -1;
	}

  	fclose(fp);
}


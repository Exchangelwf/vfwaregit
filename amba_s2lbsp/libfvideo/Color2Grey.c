#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include "DVRDEF.H"
#include "Gpio.h"
#include <basetypes.h>
#include "Color2Grey.h"
#include "ColorAdjustment.h"
#include "iav_ioctl.h"
#include "ipc_inc.h"

/*
* lux 	1		2		3		4		5		6		7		8		9		10		11		12		13		14		15
* adc2	0x50		0xd0		0x1c0	0x215	0x250	0x2f4	0x376	0x3f4	0x48f	0x500	0x54c	0x5f4	0x66c	0x6af	0x715
*/

static u32 adc_table[15] = {0x50,0xd0,0x1c0,0x215,0x250,0x2f4,0x376,0x3f4,
								0x48f,0x500,0x54c,0x5f4,0x66c,0x6af,0x715};
int adc_open_led_num = 5;
int adc_close_led_num = 9;

#define ADC_CHANNEL	2
#define DET_NUM	50
#define MAX_GPIO_LIGHT_DET_NUM 10
static int gIrCut = 0x00;
int DayNightFlag = 0;
int HdrFlag = 0;

static s32 get_adc_value(s32 *fd, s32 adc_ch_id, u32 *value)
{
        char value_str[128] = {0};
        char vbuf[8] = {0};
        char adc_device[128] = "/sys/devices/e8000000.apb/e801d000.adc/adcsys";
        sprintf(vbuf, "adc%d=0x", adc_ch_id);

        if (!value) {
                fprintf(stderr, "Invalid pointer!");
                return -1;
        }
        if (*fd < 0) {
                if ((*fd = open(adc_device, O_RDONLY)) < 0) {
                        fprintf(stderr, "get_adc_value open %s\n", strerror(errno));
                        return -1;
                }
        }
        if (read(*fd, value_str, sizeof(value_str)) <= 0) {
                perror("read");
                return -1;
        } else {
                sscanf(strstr(value_str, vbuf), "%*[^x]x%x", value);
                lseek(*fd, 0, SEEK_SET);
        }

return 0;
}

u32 adc_filter()
{
	int i = 0;
	u32 sum = 0;
	u32 ir_led_value = 0;
	s32 fd = -1;
	for(i=0; i<DET_NUM; i++){
		get_adc_value(&fd, ADC_CHANNEL, &ir_led_value);
		sum +=ir_led_value;		
		usleep(1000);
	}
	close(fd);
	ir_led_value = sum/DET_NUM;
	
	return ir_led_value;
}


int OpenIrLed()
{
	gpio_set1(ConvertGpio(GPIO_IR_SW));
	return 0;
}

int CloseIrLed()
{
	gpio_set0(ConvertGpio(GPIO_IR_SW));
	return 0;
}
int CloseIrCut()
{
	if(NULL != strcasestr(GetVersioninfo(), "V10")){
		gpio_set1(ConvertGpio(GPIO_IRCUT_SW));
	}else 
		gpio_set0(ConvertGpio(GPIO_IRCUT_SW));	
	return 0;
}
int OpenIrCut()
{
	if(NULL != strcasestr(GetVersioninfo(), "V10")){
		gpio_set0(ConvertGpio(GPIO_IRCUT_SW));
	}else 
		gpio_set1(ConvertGpio(GPIO_IRCUT_SW));
	return 0;
}

int GetIrCutStatus()
{
	return gIrCut;
}
int SetIrCutStatus(int status/*0 auto 1 open 2 close*/)
{
	gIrCut = status;
	return 0;
}
int IsHdrMode()
{
	struct vindev_mode vsrc_mode;
	memset(&vsrc_mode, 0, sizeof(vsrc_mode));
	vsrc_mode.vsrc_id = 0;
	AM_IOCTL(iav_fd(), IAV_IOC_VIN_GET_MODE, &vsrc_mode);
	if(vsrc_mode.hdr_mode == AMBA_VIDEO_2X_HDR_MODE)
		return 1;
	else
		return 0;
}
void* ColorGreyTask(void *noargs)
{
	int MarkIrcut = IRCUT_AUTO;
	int MarkHdr = 0;
	
	OpenIrCut();                  //设置IO口
	sleep (3);//视频刚刚启动。亮度较小
	while (1)
	{
		sleep(1);
		switch(GetIrCutStatus())
		{
			case 0:
				break;
			case 1:
			{
				if(MarkIrcut != IRCUT_FULLCOLOR){
					DayNightFlag = 1;
					CloseIrLed();
					OpenIrCut();
					if(0 == IsHdrMode()){
						DayNightMode(0);
					}
					if(MarkHdr && HdrFlag){
						SetHdrMode();
						MarkHdr = 0;
					}else if(HdrFlag == 1){
						SetHdrMode();
					}
		
					MarkIrcut = IRCUT_FULLCOLOR;
				}
				continue;
			}
			case 2:
			{
				if(MarkIrcut != IRCUT_BWCOLOR){
					if(1 == IsHdrMode()){//hdr 模式下没有黑白模式，需先切成普通模式
						SetLinearMode();
						sleep(2);
						MarkHdr = 1;
					}
					DayNightMode(1);
					sleep(1);
					OpenIrLed();
					CloseIrCut();					
					DayNightFlag = 0;
					MarkIrcut = IRCUT_BWCOLOR;
				}
				continue;
			}
		}
		
		int i=0;
		int LDR_state_on=0;
		int LDR_state_off=0;
		if(NULL != strcasestr(GetVersioninfo(), "V10")){	
            int ldr_on_adc_value=0x2af;
            int ldr_off_adc_value=0x5a8;
            char tmp_buff[16];
            FILE *fp = fopen("/appex/mtd/Config/ADC_LDR_VALUE", "r");
            if(NULL == fp){
                fp = fopen("/appex/mtd/Config/ADC_LDR_VALUE", "w");
                sprintf(tmp_buff, "%x,%x", ldr_on_adc_value, ldr_off_adc_value);
                fwrite(tmp_buff, 1, strlen(tmp_buff), fp);
            }else{
                fread(tmp_buff, 1, sizeof(tmp_buff), fp);
                sscanf(tmp_buff, "%x,%x", &ldr_on_adc_value, &ldr_off_adc_value);
            }
            fclose(fp);
			for(i=0;i<MAX_GPIO_LIGHT_DET_NUM;i++){
				if(adc_filter() <= ldr_on_adc_value){
					LDR_state_on +=1;
				}else if(adc_filter() >= ldr_off_adc_value){
					LDR_state_off +=1;
				}
				usleep(10*1000);	
				//printf("LDR_state_on=%d*****LDR_state_off=%d*****ir_led_value=0x%x\n", LDR_state_on,LDR_state_off,adc_filter());
			}
		}else{
			for(i=0;i<MAX_GPIO_LIGHT_DET_NUM;i++){
				if(gpio_read(ConvertGpio(GPIO_LIGHT_DET))){
					LDR_state_on +=1;					
				}else
					LDR_state_off +=1;		
				usleep(100*1000);
			}
		}
		if(MAX_GPIO_LIGHT_DET_NUM == LDR_state_on){
			if(MarkIrcut != IRCUT_BWCOLOR){
				if(1 == IsHdrMode()){
					SetLinearMode();
					sleep(2);
					MarkHdr = 1;
				}
				
				DayNightMode(1);
				sleep(1);
				OpenIrLed();
				CloseIrCut();					
				DayNightFlag = 0;
				MarkIrcut = IRCUT_BWCOLOR;
			}
		}else if(MAX_GPIO_LIGHT_DET_NUM == LDR_state_off){
			if(MarkIrcut != IRCUT_FULLCOLOR){
				DayNightFlag = 1;
				CloseIrLed();
				OpenIrCut();
				if(0 == IsHdrMode()){
					DayNightMode(0);
				}
				
				if(MarkHdr && HdrFlag){
					SetHdrMode();
					MarkHdr = 0;
				}else if(HdrFlag == 1){
					SetHdrMode();
				}
				
				MarkIrcut = IRCUT_FULLCOLOR;
			}
		}
	}
}


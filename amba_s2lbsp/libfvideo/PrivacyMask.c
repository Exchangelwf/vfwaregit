#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <sched.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <assert.h>
#include <basetypes.h>
#include <iav_ioctl.h>

#include <signal.h>
#include "Capture.h"
#include "ipc_inc.h"

extern int iav_fd();

static struct amba_video_info vin_info;
static u32 vin_frame_time;
static u8 *pm_addr = NULL;
static int pm_size = 0;
struct iav_privacy_mask privacy_mask = {1, 210, 16, 146, 0, 0, 0};
//privacy_mask.enable = 1;
//privacy_mask.y = 210;
//privacy_mask.u = 16;
//privacy_mask.v = 146;

//mask rect
typedef struct privacy_mask_rect_s{
	int start_x;
	int start_y;
	int width;
	int height;
} privacy_mask_rect_t;

static int mask_rect_remove = 0;
static privacy_mask_rect_t	mask_rect;

static int hdr_x_offset = 0;
static int hdr_y_offset = 0;

#ifndef ROUND_UP
#define ROUND_UP(x, n)  ( ((x)+(n)-1u) & ~((n)-1u) )
#endif

static inline int get_vin_info(){
	struct vindev_fps vsrc_fps;
	struct vindev_video_info video_info;

	video_info.vsrc_id = 0;
	video_info.info.mode = AMBA_VIDEO_MODE_CURRENT;
	vin_info.mode = AMBA_VIDEO_MODE_CURRENT;
	if (ioctl(iav_fd(), IAV_IOC_VIN_GET_VIDEOINFO, &video_info) < 0) {
		perror("IAV_IOC_VIN_GET_VIDEOINFO");
		return -1;
	}

	memcpy(&vin_info, &video_info.info, sizeof(struct amba_video_info));

	if (video_info.info.hdr_mode >= AMBA_VIDEO_2X_HDR_MODE) {
		vin_info.width = video_info.info.max_act_width;
		vin_info.height = video_info.info.max_act_height;
		// FIX ME! act win and max_act win may not have same center in the future.
		hdr_x_offset = (video_info.info.max_act_width - video_info.info.width) / 2;
		hdr_y_offset = (video_info.info.max_act_height - video_info.info.height) / 2;
	}

	vsrc_fps.vsrc_id = 0;
	if (ioctl(iav_fd(), IAV_IOC_VIN_GET_FPS, &vsrc_fps) < 0) {
		perror("IAV_IOC_VIN_GET_FPS");
		return -1;
	}

	vin_frame_time = vsrc_fps.fps;

	return 0;
}

static int map_privacymask(void)
{
	struct iav_querybuf querybuf;

	querybuf.buf = IAV_BUFFER_PM_BPC;
	if (ioctl(iav_fd(), IAV_IOC_QUERY_BUF, &querybuf) < 0) {
		perror("IAV_IOC_QUERY_BUF");
		return -1;
	}

	pm_size = querybuf.length;
	pm_addr = mmap(NULL, pm_size, PROT_WRITE, MAP_SHARED, iav_fd(), querybuf.offset);

	return 0;
}

int fill_pm_mem(privacy_mask_rect_t pm_rect, int pm_remove)
{
	int i, j;
	int pitch = ROUND_UP(vin_info.width / 8, 32);
	u8 *row = pm_addr + pitch * pm_rect.start_y;
	u8 *column;
	u8 value;
	int left = pm_rect.start_x / 8;
	int right = (pm_rect.start_x + pm_rect.width) / 8;
	int left_start = pm_rect.start_x % 8;
	int right_remain = (pm_rect.start_x + pm_rect.width) % 8;

	if (pm_remove) {
		value = 0x00;
	} else {
		value =0xff;
	}
	for (j = 0; j < pm_rect.height; j++) {

		if (left == right && (left_start + pm_rect.width < 8)) {
			/*This is for small PM width is within 8 pixel*/
			column = row + left;
			if (pm_remove) {
				value = 0xFF;
				value <<= (left_start + pm_rect.width);
				value |= (0xFF >> (8 - left_start));
				*column &= value;
			} else {
				value = 0xFF;
				value <<= left_start;
				value &= (0xFF >> (8 - left_start - pm_rect.width));
				*column |= value;
			}
		}	else {
			/* This is for PM ocupies more than one byte(8 pixel) */
			column = row + left + 1;
			for (i = 0; i < right - left - 1; i++) {
				*column = value;
				column++;
			}

			column = row + left;
			if (pm_remove) {
				*column &= (0xFF >> (8 - left_start));
			} else {
				*column |= (0xFF << left_start);
			}

			column = row + right;
			if (pm_remove) {
				*column &= (0xFF << right_remain);
			} else {
				*column |= (0xFF >> (8 - right_remain));
			}
		}

		/*Switch to next line*/
		row += pitch;
	}


	if (ioctl(iav_fd(), IAV_IOC_SET_PRIVACY_MASK, &privacy_mask) < 0) {
		perror("IAV_IOC_SET_PRIVACY_MASK");
		return -1;
	}

	return 0;

}

void set_privacy_mask_info(VIDEO_COVER_PARAM *pParam)
{
	struct iav_srcbuf_setup buf_setup;
	memset(&buf_setup, 0, sizeof(buf_setup));
	ioctl(iav_fd(), IAV_IOC_GET_SOURCE_BUFFER_SETUP, &buf_setup);

	int left = (int)buf_setup.input[0].width*pParam->rect.left/1920;
	int right = (int)buf_setup.input[0].width*pParam->rect.right/1920;
	int top = (int)buf_setup.input[0].height*pParam->rect.top/1080;
	int bottom = (int)buf_setup.input[0].height*pParam->rect.bottom/1080;
	uint color = pParam->color;
	if(pParam->color > 0)
	{
		privacy_mask.y = 0.299*(color&0xff) + 0.587*((color&0xff00)>>8) + 0.114*((color&0xff0000)>>16);
		privacy_mask.u = -0.147*(color&0xff) - 0.289*((color&0xff00)>>8) + 0.436*((color&0xff0000)>>16) + 128;
		privacy_mask.v = 0.615*(color&0xff) - 0.515*((color&0xff00)>>8) - 0.100*((color&0xff0000)>>16) + 128;	
		if ((color&0xff)==255 && ((color&0xff00)>>8)!=255)
			privacy_mask.v = 240;
	}
	
	mask_rect.start_x = left;
	mask_rect.start_y = top;
	mask_rect.width   = right -left;
	mask_rect.height  = bottom - top;
	
	if(pParam->enable == 1){
		mask_rect_remove = 0;
	}
	else{
		mask_rect_remove = 1;
	}
	//printf("x---y(%d, %d), w----h(%d, %d) \n", left, top, mask_rect.width, mask_rect.height);
	//printf("x---y(%d, %d), w----h(%d, %d) \n", pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
}


int privacy_mask_set(VIDEO_COVER_PARAM *pParam)
{
	static int map_flag = 1;
	
	/* PM type check */
	struct iav_system_resource resource;
	memset(&resource, 0, sizeof(resource));
	resource.encode_mode = DSP_CURRENT_MODE;
	if(ioctl(iav_fd(), IAV_IOC_GET_SYSTEM_RESOURCE, &resource) < 0) {
		perror("IAV_IOC_GET_SYSTEM_RESOURCE");
		return -1;
	}
	if (resource.mctf_pm_enable) {
		printf("Please switch PM type to BPC based PM as test_privacymask is always using "
				"BPC based PM!\n");
		return -1;
	}
	
	get_vin_info();
	set_privacy_mask_info(pParam);
	
	if (mask_rect.start_x + mask_rect.width > vin_info.width ||
		mask_rect.start_y + mask_rect.height > vin_info.height ||
		mask_rect.height <= 0 || mask_rect.width <= 0){
		return -1;
	}
	if (map_flag && map_privacymask() < 0) {
		printf("map privacy mask failed \n");
		return -1;
	}
		
	mask_rect.start_x += hdr_x_offset;
	mask_rect.start_y += hdr_y_offset;

	if (fill_pm_mem(mask_rect, mask_rect_remove) < 0){
		perror("fill_pm_mem");
	} 
	map_flag = 0;

	return 0;
}

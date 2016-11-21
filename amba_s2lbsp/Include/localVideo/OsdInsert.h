#ifndef			__OSD__INSERT___H
#define			__OSD__INSERT___H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <wchar.h>
#include <signal.h>
#include <getopt.h>
#include <ctype.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <pthread.h>
#include <locale.h>

#include <basetypes.h>
#include <iav_ioctl.h>
#include "text_insert.h"
#include "ipc_inc.h"

#define MAX_OVERLAY_AREA_NUM	(3)
#define OVERLAY_CLUT_NUM		(16)
#define OVERLAY_CLUT_SIZE		(1024)
#define OVERLAY_CLUT_OFFSET		(0)
#define OVERLAY_YUV_OFFSET		(OVERLAY_CLUT_NUM * OVERLAY_CLUT_SIZE)
#define CLUT_OUTLINE_OFFSET		(OVERLAY_CLUT_NUM / 2)

#define STRING_SIZE				(255)


#ifndef LINE_SPACING
#define LINE_SPACING(x)		((x) * 3 / 2)
#endif

#ifndef ROUND_UP
#define ROUND_UP(size, align) (((size) + ((align) - 1)) & ~((align) - 1))
#endif

#ifndef ROUND_DOWN
#define ROUND_DOWN(size, align) ((size) & ~((align) - 1))
#endif

#ifndef VERIFY_STREAMID
#define VERIFY_STREAMID(x)   do {		\
			if (((x) < 0) || ((x) >= MAX_ENCODE_STREAM_NUM)) {	\
				printf ("stream id wrong %d \n", (x));			\
				return -1; 	\
			}	\
		} while(0)
#endif

#ifndef VERIFY_AREAID
#define VERIFY_AREAID(x)	do {		\
			if (((x) < 0) || ((x) >= MAX_OVERLAY_AREA_NUM)) {	\
				printf("area id wrong %d, not in range [0~2].\n", (x));	\
				return -1;	\
			}	\
		} while (0)
#endif

#define	ROTATE_BIT		(0)
#define	HFLIP_BIT		(1)
#define	VFLIP_BIT		(2)
#define	SET_BIT(x)		(1 << (x))

typedef enum rotate_type_s {
	CLOCKWISE_0 = 0,
	CLOCKWISE_90 = SET_BIT(ROTATE_BIT),
	CLOCKWISE_180 = SET_BIT(HFLIP_BIT) | SET_BIT(VFLIP_BIT),
	CLOCKWISE_270 = SET_BIT(HFLIP_BIT) | SET_BIT(VFLIP_BIT) | SET_BIT(ROTATE_BIT),
	CLOCKWISE_HELIP = SET_BIT(HFLIP_BIT),
	CLOCKWISE_VFLIP = SET_BIT(VFLIP_BIT),
	AUTO_ROTATE,
} rotate_type_t;

typedef struct textbox_s {
	int enable;
	u16 x;
	u16 y;
	u16 width;
	u16 height;
	int color;
	int font_type;
	int font_size;
	int outline;
	int bold;
	int italic;
	rotate_type_t rotate;
	wchar_t content[STRING_SIZE];
} textbox_t;

typedef struct stream_text_info_s {
	int enable;
	int win_width;
	int win_height;
	rotate_type_t rotate;
	textbox_t textbox[MAX_OVERLAY_AREA_NUM];
} stream_text_info_t;

typedef struct clut_s {
	u8 v;
	u8 u;
	u8 y;
	u8 alpha;
} clut_t;

typedef struct buffer_info_s {
	u16 buf_width;
	u16 buf_height;
	u16 sub_x;
	u16 sub_y;
	u16 sub_width;
	u16 sub_height;
	u8 *buf_base;
} buffer_info_t;

int overlay_yuv_size;
u8 *overlay_clut_addr;

stream_text_info_t stream_text_info[MAX_ENCODE_STREAM_NUM];
struct iav_overlay_insert overlay_insert[MAX_ENCODE_STREAM_NUM];
pthread_rwlock_t lock_overlay;//Add by SuperJoki 20151209
int check_encode_status(void);
int map_overlay(void);
int check_param(void);
int fill_overlay_clut(void);
int set_overlay_config(void);
int text_set_font(int stream_id, int area_id);
void fill_overlay_data(const buffer_info_t *src, buffer_info_t *dst, rotate_type_t rotate);
int text_string_convert(int stream_id, int area_id);
int text_osd_init();
int char_to_wchar(const char* orig_str, wchar_t *wtext, int max_length);
int utf8_char_to_wchar(const char* orig_str, wchar_t *wtext, int max_length);



#endif
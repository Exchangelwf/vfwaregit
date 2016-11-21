#include "OsdInsert.h"

extern int iav_fd();
extern int draw_font_size; //SuperJoki

#define COLOR_NUM		2
clut_t clut[] = {
	/* alpha 0 is full transparent */
	//{ .y = 82, .u = 90, .v = 240, .alpha = 255 },	/* red */
	//{ .y = 41, .u = 240, .v = 110, .alpha = 255 },	/* blue */
	//{ .y = 12, .u = 128, .v = 128, .alpha = 255 },	/* black */
	{ .y = 235, .u = 128, .v = 128, .alpha = 255 },	/* white */
	{ .y = 235, .u = 128, .v = 128, .alpha = 255 },	/* white */
	//{ .y = 145, .u = 54, .v = 34, .alpha = 255 },	/* green */
	//{ .y = 210, .u = 16, .v = 146, .alpha = 255 },	/* yellow */
	//{ .y = 170, .u = 166, .v = 16, .alpha = 255 },	/* cyan */
	//{ .y = 107, .u = 202, .v = 222, .alpha = 255 }, /* magenta */

};


wchar_t wText[][STRING_SIZE] = {
	L"wanjiaan",
	L"wanjiaan",
	L"wanjiaan",
	L"wanjiaan"
};

#define FONTTYPE_NUM		4
#if 1
char font_type[][MAX_BYTE_SIZE] = {
	"/usr/share/fonts/Vera.ttf",
	"/usr/share/fonts/Lucida.ttf",
	"/usr/share/fonts/gbsn00lp.ttf",
	"/usr/share/fonts/UnPen.ttf",
};
#else
char font_type[][MAX_BYTE_SIZE] = {
	"/usr/share/fonts/FreeMono.ttf",
	"/usr/share/fonts/FreeMonoBold.ttf",
	"/usr/share/fonts/FreeSansBold.ttf",
	"/usr/share/fonts/FreeSerifItalic.ttf",
};
#endif
pixel_type_t pixel_type = {
	.pixel_background = 0,
	.pixel_outline = 1,
	.pixel_font = 255,
};

int char_to_wchar(const char* orig_str, wchar_t *wtext, int max_length)
{
	if (-1 == mbstowcs(wtext, orig_str, max_length - 1))
		return -1;
	if (wcscat(wtext, L"") == NULL)
		return -1;
	return 0;
}

int utf8_char_to_wchar(const char* orig_str, wchar_t *wtext, int max_length)
{
	int outputsize=0;
	memset(wtext, 0, max_length);

	while(*orig_str && (outputsize<max_length)){
		char *tmp=(char*)(wtext+outputsize);
		if (*orig_str > 0x00 && *orig_str <= 0x7F){
			*tmp = *orig_str;
			tmp++;
			*tmp = 0;
		}else if(((*orig_str) & 0xE0) == 0xC0){
			char high = *orig_str;
			orig_str++;
			char low = *orig_str;
			if ((low & 0xC0) != 0x80){
				return -1;
			}
			*tmp = (high << 6) + (low & 0x3F);
			tmp++;
			*tmp = (high >> 2) & 0x07;
		}else if(((*orig_str) & 0xF0) == 0xE0){
			char high = *orig_str;
			orig_str++;
			char middle = *orig_str;
			orig_str++;
			char low = *orig_str;
			if (((middle & 0xC0) != 0x80) || ((low & 0xC0) != 0x80)){
				return -1;
			}
			*tmp = (middle << 6) + (low & 0x7F);
			tmp++;
			*tmp = (high << 4) + ((middle >> 2) & 0x0F); 
		}else{
			return -1;
		}
		orig_str ++;
		outputsize ++;
	}
	
	return 0;
}

int check_encode_status(void)
{
	int i;
	enum iav_state status;
	struct iav_stream_format format;
	u8 rotate_clockwise, vflip, hflip;

	/* IAV must be in ENOCDE or PREVIEW state */
	if (ioctl(iav_fd(), IAV_IOC_GET_IAV_STATE, &status) < 0) {
		perror("IAV_IOC_GET_IAV_STATE");
		return -1;
	}

	if ((status != IAV_STATE_PREVIEW) &&
			(status != IAV_STATE_ENCODING)) {
		printf("IAV must be in PREVIEW or ENCODE for text OSD.\n");
		return -1;
	}

	/* Check rotate status */
	for (i = 0; i < MAX_ENCODE_STREAM_NUM; i++) {
		if (stream_text_info[i].enable) {
			format.id = i;
			if (ioctl(iav_fd(), IAV_IOC_GET_STREAM_FORMAT, &format) < 0) {
				perror("IAV_IOC_GET_STREAM_FORMAT");
				return -1;
			}

			if (stream_text_info[i].rotate == AUTO_ROTATE) {
				rotate_clockwise = format.rotate_cw;
				hflip = format.hflip;
				vflip = format.vflip;

				stream_text_info[i].rotate = CLOCKWISE_0;
				stream_text_info[i].rotate |= (rotate_clockwise ?
						SET_BIT(ROTATE_BIT) : 0);
				stream_text_info[i].rotate |= (hflip ? SET_BIT(HFLIP_BIT) : 0);
				stream_text_info[i].rotate |= (vflip ? SET_BIT(VFLIP_BIT) : 0);
			}

			if (stream_text_info[i].rotate & SET_BIT(ROTATE_BIT)) {
				stream_text_info[i].win_width = format.enc_win.height;
				stream_text_info[i].win_height = format.enc_win.width;
			} else {
				stream_text_info[i].win_width = format.enc_win.width;
				stream_text_info[i].win_height = format.enc_win.height;
			}
		}
	}
	return 0;
}

int map_overlay(void)
{
	struct iav_querybuf querybuf;
	u8* addr = NULL;

	querybuf.buf = IAV_BUFFER_OVERLAY;
	if (ioctl(iav_fd(), IAV_IOC_QUERY_BUF, &querybuf) < 0) {
		printf("IAV_IOC_QUERY_BUF: OVERLAY\n");
		return -1;
	}

	addr = (u8*)mmap(NULL, querybuf.length,
							PROT_WRITE, MAP_SHARED,
							iav_fd(), querybuf.offset);
	if (addr == MAP_FAILED) {
		printf("mmap OVERLAY failed\n");
		return -1;
	}

	//printf("\noverlay: start = %p, total size = 0x%x ( bytes)\n",addr, querybuf.length);
	overlay_clut_addr = addr + OVERLAY_CLUT_OFFSET;
	overlay_yuv_size = (querybuf.length - OVERLAY_YUV_OFFSET)/MAX_ENCODE_STREAM_NUM;

	return 0;
}

int check_param(void)
{
	int i, j, win_width, win_height;
	textbox_t *box;
	char str[STRING_SIZE];
	//static int default_fontsize = 56;
	int default_fontsize = draw_font_size;//mod by SuperJoki
	_printd("default_fontsize ====> %d\n", default_fontsize);

	for (i = 0; i < MAX_ENCODE_STREAM_NUM; i++){
		if (stream_text_info[i].enable) {
			win_width = stream_text_info[i].win_width;
			win_height = stream_text_info[i].win_height;
			for (j = 0; j < MAX_OVERLAY_AREA_NUM; j++) {
				box = &stream_text_info[i].textbox[j];
				if (box->enable) {
					sprintf(str, "Stream %c Textbox %d", 'A' + i, j);
					if (box->font_type < 0 || box->font_type >= FONTTYPE_NUM)
						box->font_type = 0;
					if (box->font_size <= 0)
						box->font_size = default_fontsize;
					if (box->color < 0 || box->color >= COLOR_NUM)
						box->color = 0;
					if (box->italic < 0)
						box->italic = 0;
					if (box->outline < 0)
						box->outline = 0;
					if (box->x < 0)
						box->x = 0;
					if (box->y < 0)
						box->y = 0;

					/* Text box's width and height cannot be ZERO */
					if (box->width <= 0|| box->height <= 0) {
						printf("!!!Error: %s width[%d] or height[%d] should be "
								"positive.\n", str, box->width, box->height);
						return -1;
					}
					/* Text box's height should be big enough for alignment.*/
					if (box->height < LINE_SPACING(box->font_size)){
						printf("!!!Error: %s height [%d] is too small for font"
								" size [%d].\nOSD height >= line_space * font "
								"size. The default line_space is 1.5x.\n", str,
								box->height, box->font_size);
						return -1;
					}
					/* Text box cannot exceed the encode size */
					if (box->x + box->width > win_width) {
						stream_text_info[i].textbox[j].x = win_width - ROUND_UP(box->width, 32);

						//printf("%s [x %d, w %d]\n", str,
						//	stream_text_info[i].textbox[j].x, stream_text_info[i].textbox[j].width);
					}

					if (box->y + box->height > win_height) {
						stream_text_info[i].textbox[j].y = win_height - ROUND_UP(box->height, 4);

						//printf("%s [y %d, h %d]\n", str,
							//stream_text_info[i].textbox[j].y, stream_text_info[i].textbox[j].height);
					}

					if (wcslen(box->content) == 0)
						wcscpy(box->content, wText[box->font_type]);

					//printf("%s [x %d, y %d, w %d, h %d], font size %d, type %d,"
						//	" color %d, bold %d, italic %d, outline %d\n", str,
						//	box->x, box->y, box->width, box->height,
						//	box->font_size, box->font_type, box->color,
						//	box->bold, box->italic, box->outline);
				}
			}
		}
	}
	return 0;
}

int fill_overlay_clut(void)
{
	static clut_t * clut_data;
	static int enter = 0;
	if (enter == 0)
	{
		clut_data = (clut_t *)overlay_clut_addr;
		enter = 1;
	}
	int i, j, outline_clut;

	for (i = 0; i < CLUT_OUTLINE_OFFSET && i < sizeof(clut); i++) {
		/* CLUT (0 ~ CLUT_OUTLINE_OFFSET) is for non outline font.*/
		for (j = 0; j < 256; j++) {
			clut_data[(i << 8) + j].y = clut[i].y;
			clut_data[(i << 8) + j].u = clut[i].u;
			clut_data[(i << 8) + j].v = clut[i].v;
			clut_data[(i << 8) + j].alpha = j;
		}

		/* CLUT (CLUT_OUTLINE_OFFSET ~ OVERLAY_CLUT_NUM) is outline font.*/
		outline_clut = i + CLUT_OUTLINE_OFFSET;
		for (j = 0; j < 256; j++) {
			clut_data[(outline_clut << 8) + j].y = clut[i].y;
			clut_data[(outline_clut << 8) + j].u = clut[i].u;
			clut_data[(outline_clut << 8) + j].v = clut[i].v;
			clut_data[(outline_clut << 8) + j].alpha = j;
		}
		clut_data[(outline_clut << 8) + pixel_type.pixel_outline].y = (
				clut[i].y > 128 ? 16 : 235);
		clut_data[(outline_clut << 8) + pixel_type.pixel_outline].u = 128;
		clut_data[(outline_clut << 8) + pixel_type.pixel_outline].v = 128;
		clut_data[(outline_clut << 8) + pixel_type.pixel_outline].alpha = 255;

	}

	return 0;
}

int set_overlay_config(void)
{
	int i, j, win_width, win_height, total_size;
	u32 overlay_data_offset;
	struct iav_overlay_area *area;
	textbox_t *box;

	for (i = 0; i < MAX_ENCODE_STREAM_NUM; i++) {
		if (stream_text_info[i].enable) {
			overlay_data_offset = OVERLAY_YUV_OFFSET + overlay_yuv_size * i;
			win_width = stream_text_info[i].win_width;
			win_height = stream_text_info[i].win_height;
			for (j = 0, total_size = 0; j < MAX_OVERLAY_AREA_NUM; j++) {
				area = &overlay_insert[i].area[j];
				box = &stream_text_info[i].textbox[j];

				if (box->enable) {
					if (stream_text_info[i].rotate &
							SET_BIT(ROTATE_BIT)) {
						area->width = box->height = ROUND_UP(box->height, 32);
						area->height = box->width = ROUND_UP(box->width, 4);
					} else {
						area->width = box->width = ROUND_UP(box->width, 32);
						area->height = box->height = ROUND_UP(box->height, 4);
					}
					switch (stream_text_info[i].rotate) {
					case CLOCKWISE_0:
						area->start_x = box->x = ROUND_DOWN(box->x, 2);
						area->start_y = box->y = ROUND_DOWN(box->y, 4);
						break;
					case CLOCKWISE_90:
						area->start_x = box->y = ROUND_DOWN(box->y, 2);
						area->start_y = ROUND_DOWN(
								win_width - box->x - box->width, 4);
						box->x = win_width - box->width - area->start_y;
						break;
					case CLOCKWISE_180:
						area->start_x = ROUND_DOWN(
								win_width - box->x - box->width, 2);
						area->start_y = ROUND_DOWN(
								win_height - box->y - box->height, 4);
						box->x = win_width - box->width - area->start_x;
						box->y = win_height - box->height -  area->start_y;
						break;
					case CLOCKWISE_270:
						area->start_x = ROUND_DOWN(
								win_height - box->y - box->height, 2);
						area->start_y = box->x = ROUND_DOWN(box->x, 4);
						box->y = win_height - box->height -  area->start_x;
						break;
					case CLOCKWISE_HELIP:
						area->start_x = ROUND_DOWN(win_width - box->x - box->width, 2);
						area->start_y = box->y = ROUND_DOWN(box->y, 4);
						break;
					case CLOCKWISE_VFLIP:
						area->start_x = box->x = ROUND_DOWN(box->x, 2);
						area->start_y = ROUND_DOWN(win_height - box->y - box->height, 4);
						break;
					default:
						printf("unknown rotate type\n");
						return -1;
					}

					area->pitch = area->width;
					area->enable = 1;
					area->total_size = area->pitch * area->height;
					area->clut_addr_offset = (box->outline == 0 ? box->color :
							box->color + CLUT_OUTLINE_OFFSET) * OVERLAY_CLUT_SIZE;
					area->data_addr_offset = overlay_data_offset + total_size;

					//printf("Stream %c Area %d [x %d, y %d, w %d, h %d,"
					//		" size %d],clut_addr_offset:%d,data_addr_offset:%d\n", 'A' + i, j, area->start_x,
					//		area->start_y, area->width, area->height,
					//		area->total_size,area->clut_addr_offset,area->data_addr_offset);

					total_size += area->total_size;
					if (total_size > overlay_yuv_size) {
						printf("The total OSD size is %d (should be <= %d).\n",
								total_size, overlay_yuv_size);
						return -1;
					}
				}
			}
		}
	}
	return 0;
}

int text_set_font(int stream_id, int area_id)
{
	font_attribute_t font;
	textbox_t *box = &stream_text_info[stream_id].textbox[area_id];

	strcpy(font.type, font_type[box->font_type]);
	font.size = box->font_size;
	font.outline_width = box->outline;
	font.hori_bold = font.vert_bold = box->bold;
	font.italic = box->italic;
	font.disable_anti_alias = 0;

	if (text2bitmap_set_font_attribute(&font) < 0)
		return -1;

	return 0;
}

/*
 * Fill the overlay data based on rotate type.
 */

void fill_overlay_data(const buffer_info_t *src, buffer_info_t *dst,
                              rotate_type_t rotate)
{
	u8 *sp, *dp, *src_base, *dst_base;
	int col, row;
	const u16 width = src->sub_width, height = src->buf_height;
	const u16 src_width = src->buf_width, src_height = src->buf_height;
	const u16 dst_width = dst->buf_width, dst_height = dst->buf_height;
	const u16 sx = src->sub_x, sy = src->sub_y;
	const u16 dx = dst->sub_x, dy = dst->sub_y;
	src_base = src->buf_base;
	dst_base = dst->buf_base;

	if (sx + width > src_width || sy + height > src_height) {
		printf("fill_overlay_data error.\n");
		return;
	}

	sp = src_base + sy * src_width + sx;

	switch (rotate) {
	case CLOCKWISE_0:
		#if 0
		_printd("\n////////////////////////////////////////////AAAAAAAA\n");
		char *p = NULL;
		char buf[23] = "superjoki";
		memcpy(p, buf, sizeof(buf));
		#endif
		
		dp = dst_base + dy * dst_width + dx;
		for (row = 0; row < height; row++) {
			memcpy(dp, sp, width);
			sp += src_width;
			dp += dst_width;
		}
		break;
	case CLOCKWISE_90:
		dp = dst_base + (dst_height - dx - 1) * dst_width + dy;
		for (row = 0; row < height; row++) {
			for (col = 0; col < width; col++) {
				*(dp - col * dst_width) = *(sp + col);
			}
			sp += src_width;
			dp++;
		}
		break;
	case CLOCKWISE_180:
		dp = dst_base + (dst_height - dy - 1) * dst_width + dst_width - dx - 1;
		for (row = 0; row < height; row++) {
			for (col = 0; col < width; col++) {
				*(dp - col) = *(sp + col);
			}
			sp += src_width;
			dp -= dst_width;
		}
		break;
	case CLOCKWISE_270:
		dp = dst_base + dx * dst_width + dst_width - dy - 1;
		for (row = 0; row < height; row++) {
			for (col = 0; col < width; col++) {
				*(dp + col * dst_width) = *(sp + col);
			}
			sp += src_width;
			dp--;
		}
		break;
	case CLOCKWISE_HELIP:
		dp = dst_base + dy * dst_width + dx;
		for (row = 0; row < height; row++) {
			for (col = 0; col < width; col++) {
				*(dp + col) = *(sp + width - col);
			}
			sp += src_width;
			dp += dst_width;
		}
		break;
	case CLOCKWISE_VFLIP:
		dp = dst_base + (dst_height - dy - 1) * dst_width + dx;
		for (row = 0; row < height; row++) {
			memcpy(dp, sp, width);
			sp += src_width;
			dp -= dst_width;
		}
		break;
	default:
		printf("Unknown rotate type.\n");
		break;
	}

	if (rotate & SET_BIT(ROTATE_BIT)) {
		dst->sub_width = src->sub_height;
		dst->sub_height = src->sub_width;
	} else {
		dst->sub_width = src->sub_width;
		dst->sub_height = src->sub_height;
	}
}

int text_string_convert(int stream_id, int area_id)
{
	int i, remain_height, offset_x, offset_y;
	buffer_info_t boxbuf, overlay;
	u8 *line_addr, *boxbuf_addr;
	bitmap_info_t bitmap_info;
	wchar_t *p;
	struct iav_overlay_area * area = &overlay_insert[stream_id].area[area_id];
	textbox_t *box = &stream_text_info[stream_id].textbox[area_id];
	const u16 line_width = box->width;
	const u16 line_height = LINE_SPACING(box->font_size);
	
	//_printd("Stream %c area->total_size ==>%d\n", 'A' + stream_id, area->total_size);//debug
	pthread_rwlock_wrlock(&lock_overlay); //Add by SuperJoki 20151209
	if (NULL == (boxbuf_addr = (u8 *)malloc(area->total_size))) {
		perror("text_string_convert: malloc\n");
		pthread_rwlock_unlock(&lock_overlay);
		return -1;
	}
	memset(boxbuf_addr, pixel_type.pixel_background, area->total_size);

	offset_x = offset_y = 0;
	line_addr = boxbuf_addr;

	for (i = 0, p = box->content;i < wcslen(box->content); i++, p++) {
		if (offset_x + box->font_size > line_width) {
			/*
			 * If there is enough memory for more lines, try to write on a new
			 * line. Otherwise, stop converting.
			 */
			offset_y += line_height;
			remain_height = box->height - offset_y;
			if (remain_height < line_height)
				break;
			line_addr += line_width * line_height;
			offset_x = 0;
		}

		/* Remember the offset of letters in time string */
		if (text2bitmap_convert_character(*p, line_addr, line_height,
				line_width, offset_x, &bitmap_info) < 0) {
			free(boxbuf_addr);
			pthread_rwlock_unlock(&lock_overlay);
			return -1;
		}
		
		offset_x += bitmap_info.width;
		
	}

	overlay.buf_base = overlay_clut_addr + area->data_addr_offset;
	overlay.buf_height = area->height;
	overlay.buf_width = area->width;
	overlay.sub_x = overlay.sub_y = 0;
	boxbuf.buf_base = boxbuf_addr;
	boxbuf.buf_height = boxbuf.sub_height = box->height;
	boxbuf.buf_width = boxbuf.sub_width = box->width;
	boxbuf.sub_x = boxbuf.sub_y = 0;
	fill_overlay_data(&boxbuf, &overlay, stream_text_info[stream_id].rotate);
	pthread_rwlock_unlock(&lock_overlay);
	free(boxbuf_addr);
	return 0;
}

int text_osd_init()
{
	if (map_overlay() < 0)
		return -1;

	//if (fill_overlay_clut() < 0)
	//	return -1;
	
	if (text2bitmap_lib_init(&pixel_type) < 0)
		return -1;
	
	return 0;
}

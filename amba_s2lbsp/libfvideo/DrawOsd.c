#include "DrawOsd.h"
#include <assert.h>
#include <locale.h>

#define   STRING_MAX		128

extern int iav_fd();

char draw_content[STRING_MAX] = "WELCOME WJAIPC";


int draw_str_flag = 1;
int draw_time_flag = 1;
int draw_str_change = 1;

int draw_color[2] = {0, 1};       //set color, white

int draw_font_type = 2;   //set font type, 0 Verdana, 1 Lucida, 2 Chinese(gbsn), 3 Korean(UnPen)

static OSD_TYPE gOsdType = {0};

int  draw_font_size   = 56;//changed by SuperJoki
extern clut_t clut[];
//osd_init_flag = FALSE;

extern pthread_rwlock_t lock_stream; 

void *OsdDrawTimeThread(void *argv)
{
	while(1)
	{	
		usleep(200*1000);
		pthread_rwlock_rdlock(&lock_stream);
		OsdDrawSetTimeStr();
		pthread_rwlock_unlock(&lock_stream);
	}
}


void OsdDrawTimeStart()
{
	pthread_t osd_time_tid;
	pthread_create(&osd_time_tid, NULL, (void *)OsdDrawTimeThread, NULL);
}

#define OSD_STREAM  4  //对应码流通道的值三路码流改为3

void OsdDrawSetTimeStr()
{
	char TimeOsdStr[30]	= {0};
	static time_t LastDrawTime = {0};
	static int time_f = 0;
	static int str_f = 0;
	time_t CurrTime		= {0};
	struct tm	timeTm 	= {0};
	int i = 0;
	
	time(&CurrTime);

	if (CurrTime == LastDrawTime)
		return;

	LastDrawTime = CurrTime;
	localtime_r(&CurrTime, &timeTm);
	OsdDrawGetTimeStr(TimeOsdStr, &timeTm);
	if (0 == draw_time_flag){

		if (time_f == 0){
			time_f = 1;
			for(i = 0; i < OSD_STREAM; i++){
				OsdDrawSetStr(i, 0, " ");
				OsdDrawRelease(i);
			}
		}	
	}
	else{
		time_f = 0;
		for(i = 0; i < OSD_STREAM; i++){
			OsdDrawSetStr(i, 0, TimeOsdStr);
		}
	}

	if (0 == draw_str_flag){
		if (str_f == 0){
			str_f = 1;
			for(i = 0; i < OSD_STREAM; i++){
				OsdDrawSetStr(i, 1, " ");
				OsdDrawRelease(i);
			}
		}
	}
	else{
		str_f = 0;
		if (draw_str_change){
			draw_str_change = 0;
			for(i = 0; i < OSD_STREAM; i++){
				OsdDrawSetStr(i, 1, draw_content);
			}
		}
	}
}

void OsdDrawGetTimeStr(char *str, struct tm *pTimeTm)
{
	char ds = 0x00;
	char AmPmFlag=0;
	switch (gOsdType.datespec){
		case DS_DOT:
			ds = '.';
			break;
		case DS_DASH:
			ds = '-';
			break;
		case DS_SLASH:
			ds = '/';
			break;
		default:
			ds = '-';
			break;
	}
	//gOsdType.timefmt == TF_12 && pTimeTm->tm_hour - 12 >= 0? pTimeTm->tm_hour -= 12:0;
	if(gOsdType.timefmt == TF_12){
		if(pTimeTm->tm_hour >= 12){
			AmPmFlag = 1;
		}
		if(pTimeTm->tm_hour == 0){
			pTimeTm->tm_hour = 12;
		}else if(pTimeTm->tm_hour > 12){
			pTimeTm->tm_hour -= 12;
		}
	}
	switch(gOsdType.datefmt){
		case DF_YYMMDD:
			if(gOsdType.timefmt == TF_12){
				if(AmPmFlag){
					sprintf(str, "%04d%c%02d%c%02d pm %02d:%02d:%02d", 
						1900 + pTimeTm->tm_year,ds, pTimeTm->tm_mon + 1, ds, pTimeTm->tm_mday, 
						pTimeTm->tm_hour, pTimeTm->tm_min, pTimeTm->tm_sec);
				}else{
					sprintf(str, "%04d%c%02d%c%02d am %02d:%02d:%02d", 
						1900 + pTimeTm->tm_year,ds, pTimeTm->tm_mon + 1, ds, pTimeTm->tm_mday, 
						pTimeTm->tm_hour, pTimeTm->tm_min, pTimeTm->tm_sec);
				}
			}else{
				sprintf(str, "%04d%c%02d%c%02d %02d:%02d:%02d", 
					1900 + pTimeTm->tm_year,ds, pTimeTm->tm_mon + 1, ds, pTimeTm->tm_mday, 
					pTimeTm->tm_hour, pTimeTm->tm_min, pTimeTm->tm_sec);
			}
			break;
		case DF_MMDDYY:
			if(gOsdType.timefmt == TF_12){
				if(AmPmFlag){
					sprintf(str, "%02d%c%02d%c%04d pm %02d:%02d:%02d", 
						pTimeTm->tm_mon + 1, ds, pTimeTm->tm_mday, ds, 1900 + pTimeTm->tm_year,
						pTimeTm->tm_hour, pTimeTm->tm_min, pTimeTm->tm_sec);
				}else{
					sprintf(str, "%02d%c%02d%c%04d am %02d:%02d:%02d", 
						pTimeTm->tm_mon + 1, ds, pTimeTm->tm_mday, ds, 1900 + pTimeTm->tm_year,
						pTimeTm->tm_hour, pTimeTm->tm_min, pTimeTm->tm_sec);
				}
			}else{
				sprintf(str, "%02d%c%02d%c%04d %02d:%02d:%02d", 
					pTimeTm->tm_mon + 1, ds, pTimeTm->tm_mday, ds, 1900 + pTimeTm->tm_year,
					pTimeTm->tm_hour, pTimeTm->tm_min, pTimeTm->tm_sec);
			}
			break;
		case DF_DDMMYY:
			if(gOsdType.timefmt == TF_12){
				if(AmPmFlag){
					sprintf(str, "%02d%c%02d%c%04d pm %02d:%02d:%02d", 
						pTimeTm->tm_mday, ds, pTimeTm->tm_mon + 1, ds, 1900 + pTimeTm->tm_year,
						pTimeTm->tm_hour, pTimeTm->tm_min, pTimeTm->tm_sec);
				}else{
					sprintf(str, "%02d%c%02d%c%04d am %02d:%02d:%02d", 
						pTimeTm->tm_mday, ds, pTimeTm->tm_mon + 1, ds, 1900 + pTimeTm->tm_year,
						pTimeTm->tm_hour, pTimeTm->tm_min, pTimeTm->tm_sec);
				}
			}else{
				sprintf(str, "%02d%c%02d%c%04d %02d:%02d:%02d", 
					pTimeTm->tm_mday, ds, pTimeTm->tm_mon + 1, ds, 1900 + pTimeTm->tm_year,
					pTimeTm->tm_hour, pTimeTm->tm_min, pTimeTm->tm_sec);
			}
			break;
		default :
			if(gOsdType.timefmt == TF_12){
				if(AmPmFlag){
					sprintf(str, "%04d%c%02d%c%02d pm %02d:%02d:%02d", 
						1900 + pTimeTm->tm_year,ds, pTimeTm->tm_mon + 1, ds, pTimeTm->tm_mday, 
						pTimeTm->tm_hour, pTimeTm->tm_min, pTimeTm->tm_sec);	
				}else{
					sprintf(str, "%04d%c%02d%c%02d am %02d:%02d:%02d", 
						1900 + pTimeTm->tm_year,ds, pTimeTm->tm_mon + 1, ds, pTimeTm->tm_mday, 
						pTimeTm->tm_hour, pTimeTm->tm_min, pTimeTm->tm_sec);	
				}
			}else{
				sprintf(str, "%04d%c%02d%c%02d %02d:%02d:%02d", 
					1900 + pTimeTm->tm_year,ds, pTimeTm->tm_mon + 1, ds, pTimeTm->tm_mday, 
					pTimeTm->tm_hour, pTimeTm->tm_min, pTimeTm->tm_sec);	
			}
			break;
	}	
	//printf("time==%s\n", str);
}

void OsdDrawRelease(int stream_id)
{
	if(stream_id == 2){
		return 0;
	}
	overlay_insert[stream_id].enable = 0;
	overlay_insert[stream_id].id = stream_id;
	if (ioctl(iav_fd(), IAV_IOC_SET_OVERLAY_INSERT,
			&overlay_insert[stream_id]) < 0) {
		perror("IAV_IOC_SET_OVERLAY_INSERT");
	}
}

void OsdDrawSetTimeStly(uchar datefmt, uchar datespec, uchar timefmt)
{
	gOsdType.datefmt  = datefmt;
	gOsdType.datespec = datespec;
	gOsdType.timefmt  = timefmt;
}

int TextStrlen(char *content)
{
	int nEngCount = 0;//短信英文字数
	int nChaCount = 0;//短信中文字数
	unsigned int i;
	for(i=0;i<strlen(content);i++)
	{
		if(content[i] > 0 && content[i] < 128)
		{
			nEngCount++;
		}
	}
	nChaCount = strlen(content) - nEngCount;
	//计数中文字数
	//中文占3个字符的
	if(nChaCount%3 == 0)
	{
		nChaCount = nChaCount/3*2;
	}
	//中文占2个字符的
	else if(nChaCount%2 == 0)
	{
		nChaCount = nChaCount;
	}

	return (nEngCount + nChaCount);
}

int OsdDrawSetPostion(int stream_id, int area_id, int x, int y)
{
	int real_x = x, real_y = y;
	int real_font_size = draw_font_size;
	int len = strlen("xxxx-xx-xx xx:xx:xx");
	int width = 0;

	if(gOsdType.timefmt == TF_12){
		len = strlen("xxxx-xx-xx xx xx:xx:xx");
	}
	stream_text_info[stream_id].enable = 1;
	stream_text_info[stream_id].textbox[area_id].enable = 1;
	stream_text_info[stream_id].rotate = AUTO_ROTATE;

	if (check_encode_status() < 0)
		return -1;
	real_x = (int)(x*stream_text_info[stream_id].win_width/1728);
	real_y = (int)(y*stream_text_info[stream_id].win_height/1034);
	
	if(gOsdType.timefmt == TF_24){
		width = ROUND_UP((int)(len * 39), 32)-10;//Mod by SuoerJoki 20151106
	}else{
		width = ROUND_UP((int)(len * 34), 32)-10;
	}
	
	switch (stream_text_info[stream_id].win_height)
	{	
		#if 1
		case 1520: //SuperJoki add 20151105
			if(gOsdType.timefmt == TF_24){
				width = ROUND_UP((int)(len * 60), 32); //Add 20151127
			} else{
				width = ROUND_UP((int)(len * 57), 32);
			}
			//width = ROUND_UP((int)(len * 60), 32); //Add 20151127
			if (real_font_size == 56) {
				real_font_size += 50;
				if (1 == area_id)
					//width = ROUND_UP((int)(TextStrlen(draw_content) * 60), 32);
					width = 1910;
					//_printd("area_id ===> %d, width ===> %d\n", area_id, width);//debug
			} else {
				real_font_size += 21; //src 2 -> 6 -> 14 -> 16 -> 18 -> 20 -> 21
				if (1 == area_id)
					width = ROUND_UP((int)(TextStrlen(draw_content) * 41), 32)+2;
			}
			break;	
		case 1296: //SuperJoki add 20151105
			if(gOsdType.timefmt == TF_24){
				width = ROUND_UP((int)(len * 60), 32); //Add 20151127
			}else{
				width = ROUND_UP((int)(len * 60), 32); //Add 20151127
			}
			
			real_font_size += 14;
			if (1 == area_id)
				width = ROUND_UP((int)(TextStrlen(draw_content) * 37), 32)+1;
			break;
		#endif
	
		case 1080:
			if(gOsdType.timefmt == TF_24){
				width = ROUND_UP((int)(len * 60), 32);
			}else{
				width = ROUND_UP((int)(len * 60), 32); 
			}
			
			real_font_size += 2; //SuperJoki add
			if (1 == area_id)
				width = ROUND_UP((int)(TextStrlen(draw_content) * 30), 32)+1;
			break;			
		case 960:
			if (1 == area_id)
				width = ROUND_UP((int)(TextStrlen(draw_content) * 29), 32)+1;
			break;				
		case 720:
			real_font_size -= 7;//SuperJoki add 6
			if (1 == area_id)
				width = ROUND_UP((int)(TextStrlen(draw_content) * 29), 32)+1;//SuperJoki src (draw_content) * 28), 32)+1
			break;
		case 576:
			real_font_size -= 19;
			if(gOsdType.timefmt == TF_24){
				width -= 220;
			}else{
				width -= 220;
			}
			//width -= 220;
			if (1 == area_id)
				width = ROUND_UP((int)(TextStrlen(draw_content)* 20), 32)+1;		
			break;
		case 480:
			real_font_size -= 20;
			if(gOsdType.timefmt == TF_24){
				width -= 240;
			}else{
				width -= 240;
			}
			//width -= 240;
			if (1 == area_id)
				width = ROUND_UP((int)(TextStrlen(draw_content) * 18), 32)+1;
			break;
		case 240:
			real_font_size -= 38;
			if(gOsdType.timefmt == TF_24){
				width -= 440; 
			}else{
				width = 250;
			}
			//width -= 440; 
			if (1 == area_id)
				width = ROUND_UP((int)(TextStrlen(draw_content) * 10), 32);
			break;
		default:
			real_font_size += 2;//SuperJoki add
			if (1 == area_id)
				width = ROUND_UP((int)(TextStrlen(draw_content) * 32), 32);
			break;
	}
	
	if (1 == area_id && TextStrlen(draw_content) <= 6)
	{
		//width += 16;
	}
	//printf("stream_id = %d font_size = %d width = %d\n", stream_id, real_font_size, width);
	stream_text_info[stream_id].textbox[area_id].x = real_x;
	stream_text_info[stream_id].textbox[area_id].y = real_y;
	pthread_rwlock_rdlock(&lock_overlay); //Add by SuperJoki 20151209
	stream_text_info[stream_id].textbox[area_id].width = width;
	stream_text_info[stream_id].textbox[area_id].font_size = real_font_size;
	//stream_text_info[stream_id].textbox[area_id].height = real_font_size * 1.5 + 6;
	stream_text_info[stream_id].textbox[area_id].height = LINE_SPACING(real_font_size);
	pthread_rwlock_unlock(&lock_overlay);
	//_printd("Height =====> %d\n", stream_text_info[stream_id].textbox[area_id].height);//debug
	stream_text_info[stream_id].textbox[area_id].font_type = draw_font_type;
	stream_text_info[stream_id].textbox[area_id].color = draw_color[area_id];
	stream_text_info[stream_id].textbox[area_id].bold = 1;
	//printf("real_x:%d, real_y:%d, width:%d,x-y(%d, %d)\n",real_x, real_y, width, x, y);

	return 0;

}

int OsdDrawCheck()
{
	if (check_param() < 0)
		return -1;

	if (set_overlay_config() < 0)
		return -1;

	return 0;
}

void OsdFlush(int stream_id)
{
	draw_str_change = 1;
	stream_text_info[stream_id].rotate = AUTO_ROTATE;
	check_encode_status();
	OsdDrawCheck();
}

void OsdDrawSetFontStly(int type)
{
	draw_font_type = type;
}

/*YUV 与 RGB 的转换关系
	Y = 0.299R + 0.587G + 0.114B
    U = -0.147R - 0.289G + 0.436B +128
    V = 0.615R - 0.515G - 0.100B+128
*/
void OsdDrawSetColor(int color, int area_id)
{
	static int last_color[2] = {0, 0};
	//printf("color->  %x\n",color);
	if ( last_color[area_id] == color)
		return;
	
	last_color[area_id] = color;
	clut[area_id].y = 0.299*(color&0xff) + 0.587*((color&0xff00)>>8) + 0.114*((color&0xff0000)>>16);
	clut[area_id].u = -0.147*(color&0xff) - 0.289*((color&0xff00)>>8) + 0.436*((color&0xff0000)>>16) + 128;
	clut[area_id].v = 0.615*(color&0xff) - 0.515*((color&0xff00)>>8) - 0.100*((color&0xff0000)>>16) + 128;	
	
	if ((color&0xff)==255 && ((color&0xff00)>>8)!=255)
		clut[area_id].v = 240;
	clut[area_id].alpha = 255;
	draw_color[area_id] = area_id;
	//printf("color->  Y:%d	U:%d	V:%d\n",clut[area_id].y, clut[area_id].u, clut[area_id].v);

	fill_overlay_clut();
}

void OsdDrawSetFontSize(int size)
{
	draw_font_size = size;
}

void OsdDrawSetContent(int channel, char *content)
{
	draw_str_change = 1;
	if (-1 == channel)
		return ;
	if (0 == strlen(content))
		strncpy(draw_content, " ", STRING_MAX);
	else
		strncpy(draw_content, content, STRING_MAX);
}

int OsdDrawInit()
{
	return text_osd_init();
}

int OsdDrawSetStr(int stream_id, int area_id, char *content)
{
	if(stream_id == 2){//cvbs 暂无osd
		return 0;
	}
	if(setlocale(LC_ALL, "zh_CN.utf8") == NULL){
		//printf("Set locale failed.\n");
		utf8_char_to_wchar(content,
			stream_text_info[stream_id].textbox[area_id].content,
			sizeof(stream_text_info[stream_id].textbox[area_id].content));
	}else if(char_to_wchar(content,
		stream_text_info[stream_id].textbox[area_id].content,
		sizeof(stream_text_info[stream_id].textbox[area_id].content)) < 0) {
		printf("Convert to wchar failed. Use default string.\n");
		memset(stream_text_info[stream_id].textbox[area_id].content, 0,
		       sizeof(stream_text_info[stream_id].textbox[area_id].content));
	}

	if (stream_text_info[stream_id].textbox[area_id].enable) {
		// Set font attribute
		if (text_set_font(stream_id, area_id) < 0)
			return -1;

		// Convert string and fill the overlay data.
		if (text_string_convert(stream_id, area_id) < 0)
			return -1;
		//printf ("[%s][%d][%s]\n", __FILE__, __LINE__, __func__);
	}

	overlay_insert[stream_id].enable = 1;
	overlay_insert[stream_id].id = stream_id;
	if (ioctl(iav_fd(), IAV_IOC_SET_OVERLAY_INSERT,
			&overlay_insert[stream_id]) < 0) {
		printf("streamid=%d\n",stream_id);
		perror("IAV_IOC_SET_OVERLAY_INSERT");
		return -1;
	}
	//printf ("end::[%s][%d][%s]\n", __FILE__, __LINE__, __func__);
	return 0;
}

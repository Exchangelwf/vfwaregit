#ifndef			__DRAW__H
#define			__DRAW__H

#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include "Capture.h"
#include "OsdInsert.h"

#ifndef uchar
#define		uchar		unsigned char
#endif

#define 	MAX_OSD_TIME_TID_NUM	4

#define		TRUE		1
#define		FALSE		0

/// \defgroup CaptureAPI API Capture
    /// ��·����������Ƶ����ӿڣ�������Memory�ӿڡ�
    /// \n ��������ͼ:
    /// \code
    ///    ============================================
    ///                     |                            
    ///             OsdDrawInit             
    ///       +-----------|------------------+      OsdDrawSetPostion   
    ///       |     OsdDrawSetContent        |
    ///       |             |                |      OsdDrawTimeStart
    ///       |             |                |      
    ///       |     OsdDrawSetPostion        |
    ///       |             |                |      OsdDrawGetTimeStr
    ///       |             |                |
    ///       |     OsdDrawSetStr            |         
    ///       |             |                |      OsdDrawSetStr   
    ///       |             |                |         
    ///       +-----------|------------------+         
    ///             OsdDrawRelease                 
    ///                   |                            
    ///    ============================================
//ʱ����ʾarea_id��Ϊ 0


//ʱ����ʾ��ʽ
typedef struct {
    uchar datefmt;
    uchar datespec;
    uchar timefmt;
}OSD_TYPE;

void *OsdDrawTimeThread(void *argv);

extern void OsdDrawTimeStart();
extern void OsdDrawSetTimeStr();
extern void OsdDrawGetTimeStr(char *str, struct tm *pTimeTm);
extern void OsdDrawSetTimeStly(uchar datefmt, uchar datespec, uchar timefmt);
extern int OsdDrawSetPostion(int stream_id, int area_id, int x, int y);
extern int OsdDrawSetStr(int stream_id, int area_id, char *content);
extern void OsdDrawSetContent(int channel, char *content);
extern void OsdDrawSetFontStly(int type);
extern void OsdDrawRelease(int stream_id);
extern void OsdDrawSetColor(int color, int area_id);
extern void OsdDrawSetFontSize(int size);
extern int OsdDrawInit();
extern int OsdDrawCheck();

#endif
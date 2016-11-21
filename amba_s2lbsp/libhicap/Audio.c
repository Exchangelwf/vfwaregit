#include <fcntl.h>
#include <stdio.h>
#include <assert.h>
#include <sys/time.h>
#include <basetypes.h>
#include <string.h>
#include <dirent.h>
#include "Audio.h"
#include "ipc_inc.h"
#include "input.h"
#include "output.h"
#include "conf.h"
#include "global.h"
#include "pcm.h"
#include "control.h"
#include "mixer.h"
#include "error.h"
#include "audio_encode.h"
#include "lame.h"

#define AUDIO_DISABLE       0
#define MAX_HI_AUDIOTYPE    3

typedef struct tagAUDIOIN_FORMAT_CAPS
{
    uint    BitRate;
    uint    SampleRate;
    uint    SampleBit;
    enum audio_encode_type EncodeType;
} AUDIOIN_FORMAT_CAPS;

static const AUDIOIN_FORMAT_CAPS Convert2EncodeType[MAX_HI_AUDIOTYPE] =
{
    {40, 8000, 16, G711_ALAW},
    {40, 8000, 16, G711_ULAW},
    {40, 8000, 8, PCM_ALAW}
};

/*
 * When DEFAULT_CHANNELS is 1 ("mono"), channel_id is used to
 * choose which channel to record (Channel 0 or Channel 1 ?)
 */
static int channel_id = 0;

static struct hwparam{
    snd_pcm_format_t format;
    unsigned int channels;
    unsigned int rate;
}in_hwparams, out_hwparams;

static size_t in_bits_per_frame, out_bits_per_frame;
static snd_pcm_uframes_t in_chunk_size, out_chunk_size;

//audio_out IPC接受外部音频数据解码回放
static snd_pcm_t *audio_out_handle;
static uchar *buf_out;
static uchar *out_tmp;

//audio_in IPC接受mic输入，音频编码
static snd_pcm_t *audio_in_handle;
static uchar *buf_in;
static uchar *in_tmp;
//开启对讲标志位
static int flag_talk = 0;
static int flag_audio_play = 0;

static snd_pcm_uframes_t set_params(snd_pcm_t *handle, snd_pcm_stream_t stream, struct hwparam hwparams, size_t *bits_per_frame)
{
    snd_pcm_hw_params_t *params;
    snd_pcm_sw_params_t *swparams;
    snd_pcm_uframes_t buffer_size = 0;
    snd_pcm_uframes_t start_threshold = 0;
    snd_pcm_uframes_t chunk_size = 0;
    size_t bits_per_sample = 0;
    unsigned period_time = 0;
    unsigned buffer_time = 0;
    size_t chunk_bytes = 0;
    int err;

    snd_pcm_hw_params_alloca(&params);
    snd_pcm_sw_params_alloca(&swparams);

    err = snd_pcm_hw_params_any(handle, params);
    if (err < 0) {
        _printd("Broken configuration for this PCM: no configurations available");
        exit(EXIT_FAILURE);
    }

    err = snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    if (err < 0) {
        _printd("Access type not available");
        exit(EXIT_FAILURE);
    }

    _printd("format = %s, channels = %d, rate = %d\n",
            snd_pcm_format_name(hwparams.format), hwparams.channels, hwparams.rate);

    err = snd_pcm_hw_params_set_format(handle, params, hwparams.format);
    if (err < 0) {
        _printd("Sample format non available");
        exit(EXIT_FAILURE);
    }

    err = snd_pcm_hw_params_set_channels(handle, params, hwparams.channels);
    if (err < 0) {
        _printd("Channels count non available");
        exit(EXIT_FAILURE);
    }

    err = snd_pcm_hw_params_set_rate(handle, params, hwparams.rate, 0);
    if (err < 0) {
        _printd("Rate non available");
        exit(EXIT_FAILURE);
    }

    err = snd_pcm_hw_params_get_buffer_time_max(params, &buffer_time, 0);
    assert(err >= 0);
    if (buffer_time > 500000)
        buffer_time = 500000;

    period_time = buffer_time / 4;
    err = snd_pcm_hw_params_set_period_time_near(handle, params, &period_time, 0);
    assert(err >= 0);

    err = snd_pcm_hw_params_set_buffer_time_near(handle, params, &buffer_time, 0);
    assert(err >= 0);

    err = snd_pcm_hw_params(handle, params);
    if (err < 0) {
        _printd("Unable to install hw params:");
        exit(EXIT_FAILURE);
    }
    snd_pcm_hw_params_get_period_size(params, &chunk_size, 0);
    snd_pcm_hw_params_get_buffer_size(params, &buffer_size);
    if (chunk_size == buffer_size) {
        _printd("Can't use period equal to buffer size (%lu == %lu)", chunk_size, buffer_size);
        exit(EXIT_FAILURE);
    }

    snd_pcm_sw_params_current(handle, swparams);

    err = snd_pcm_sw_params_set_avail_min(handle, swparams, chunk_size);
    if(stream == SND_PCM_STREAM_PLAYBACK)
        start_threshold = (buffer_size / chunk_size) * chunk_size;
    else
        start_threshold = 1;
    err = snd_pcm_sw_params_set_start_threshold(handle, swparams, start_threshold);
    assert(err >= 0);

    err = snd_pcm_sw_params_set_stop_threshold(handle, swparams, buffer_size);
    assert(err >= 0);

    if (snd_pcm_sw_params(handle, swparams) < 0) {
        printf("unable to install sw params:");
        exit(EXIT_FAILURE);
    }
    bits_per_sample = snd_pcm_format_physical_width(hwparams.format);
    *bits_per_frame = bits_per_sample * hwparams.channels;
    chunk_bytes = chunk_size * (*bits_per_frame) / 8;

    printf("chunk_size = %d,chunk_bytes = %d,buffer_size = %d\n\n",
            (int)chunk_size,chunk_bytes,(int)buffer_size);

    return chunk_size;
}


//针对音频处于xrun状态的处理 
static void xrun(snd_pcm_stream_t stream, snd_pcm_t *handle)
{
    snd_pcm_status_t *status;
    int res;

    snd_pcm_status_alloca(&status);
    if ((res = snd_pcm_status(handle, status))<0) {
        printf("status error: %s\n", snd_strerror(res));
        exit(EXIT_FAILURE);
    }

    if (snd_pcm_status_get_state(status) == SND_PCM_STATE_XRUN) {
        struct timeval now, diff, tstamp;
        gettimeofday(&now, 0);
        snd_pcm_status_get_trigger_tstamp(status, &tstamp);
        timersub(&now, &tstamp, &diff);
        fprintf(stderr, "%s!!! (at least %.3f ms long)\n",
                stream == SND_PCM_STREAM_PLAYBACK ? "underrun" : "overrun",
                diff.tv_sec * 1000 + diff.tv_usec / 1000.0);

        if ((res = snd_pcm_prepare(handle))<0) {
            printf("xrun: prepare error: %s\n", snd_strerror(res));
            exit(EXIT_FAILURE);
        }
        return; /* ok, data should be accepted again */
    } else if (snd_pcm_status_get_state(status) == SND_PCM_STATE_DRAINING) {
        printf("draining!!!\n");
        if (stream == SND_PCM_STREAM_CAPTURE) {
            fprintf(stderr, "capture stream format change? attempting recover...\n");
            if ((res = snd_pcm_prepare(handle))<0) {
                printf("xrun(DRAINING): prepare error: %s", snd_strerror(res));
                exit(EXIT_FAILURE);
            }
            return;
        }
    }

    return;
}

static size_t handle_data_cp(snd_pcm_uframes_t chunk_size, u_char *data, u_char *tmp)
{
    size_t tmp_cnt, i, bfcount, chunk_bytes;

    chunk_bytes = chunk_size * in_bits_per_frame / 8;

    if ((in_hwparams.channels == 2) && (channel_id != -1)) {
        tmp_cnt = chunk_bytes >> 1;
        if (in_hwparams.format == SND_PCM_FORMAT_U8) {
            for (i = 0;i < tmp_cnt; i++) {
                if(channel_id)
                    *(tmp + i) = *(data + i * 2 + 1);
                else
                    *(tmp + i) = *(data + i * 2);
            }
        } else {
            for (i = 0;i < tmp_cnt; i++) {
                if(channel_id)
                    *((u_short *)tmp + i) = *((u_short *)data + i * 2 + 1);
                else
                    *((u_short *)tmp + i) = *((u_short *)data + i * 2);
            }
        }
    } else if (in_hwparams.channels == 2){
        if ((in_hwparams.format == SND_PCM_FORMAT_A_LAW) ||
            (in_hwparams.format == SND_PCM_FORMAT_MU_LAW)) 
        {
            printf("MU_LAW and A_LAW only support mono\n");
            exit(EXIT_FAILURE);
        }
        tmp_cnt = chunk_bytes;
        memcpy(tmp, data, tmp_cnt);
    } else {
        tmp_cnt = chunk_bytes;
        memcpy(tmp, data, tmp_cnt);
    }

    switch (in_hwparams.format) {
        case SND_PCM_FORMAT_A_LAW:
            bfcount = ALawEncode(data, (s16 *)tmp, tmp_cnt);
            break;
        case SND_PCM_FORMAT_MU_LAW:
            bfcount = ULawEncode(data, (s16 *)tmp, tmp_cnt);
            break;
        case SND_PCM_FORMAT_S16_LE:
        case SND_PCM_FORMAT_U8:
            memcpy(data, tmp, tmp_cnt);
            bfcount = tmp_cnt;
            break;
        default:
            printf("Not supported format!\n");
            exit(EXIT_FAILURE);
            break;
    }

    return bfcount;
}

static snd_pcm_uframes_t handle_data_pb(ssize_t count, u_char *data, u_char *tmp)
{
    ssize_t tmp_cnt, i;
    size_t bfcount;

    if ((out_hwparams.channels == 2) && (channel_id != -1)) {
        if (out_hwparams.format == SND_PCM_FORMAT_S16_LE) {
            for (i = 0;i < count; i++)
                if(channel_id)
                    *((u_short *)tmp + i) = *((u_short *)data + i / 2 + 1);
                else
                    *((u_short *)tmp + i) = *((u_short *)data + i / 2);
        } else {
            for (i = 0; i < count << 1; i++)
                if(channel_id)
                    *(tmp + i) = *(data + i / 2 + 1);
                else
                    *(tmp + i) = *(data + i / 2);
        }
        tmp_cnt = count << 1;
    } else if (out_hwparams.channels == 2) {
        if ((out_hwparams.format == SND_PCM_FORMAT_A_LAW) ||
            (out_hwparams.format == SND_PCM_FORMAT_MU_LAW)) 
        {
            printf("MU_LAW and A_LAW only support mono\n");
            exit(EXIT_FAILURE);
        }
        tmp_cnt = count;
        memcpy(tmp, data, tmp_cnt);
    } else {
        tmp_cnt = count;
        memcpy(tmp, data, tmp_cnt);
    }

    switch (out_hwparams.format) {
        case SND_PCM_FORMAT_A_LAW:
            bfcount = ALawDecode((s16 *)data, tmp, tmp_cnt);
            break;
        case SND_PCM_FORMAT_MU_LAW:
            bfcount = ULawDecode((s16 *)data, tmp, tmp_cnt);
            break;
        case SND_PCM_FORMAT_S16_LE:
        case SND_PCM_FORMAT_U8:
            memcpy(data, tmp, tmp_cnt);
            bfcount = tmp_cnt;
            break;
        default:
            printf("Not supported format!\n");
            exit(EXIT_FAILURE);
            break;
    }

    return bfcount * 8 / out_bits_per_frame;
}


static ssize_t pcm_write(snd_pcm_uframes_t chunk_size, uchar *data)
{
    ssize_t r;
    ssize_t result = 0;
    snd_pcm_uframes_t count;

    count = chunk_size;

    while (count > 0) {
        r = snd_pcm_writei(audio_out_handle, data, count);
        if (r == -EAGAIN || (r >= 0 && (size_t)r < count)) {
            snd_pcm_wait(audio_out_handle, 1000);
        } else if (r == -EPIPE) {
            xrun(SND_PCM_STREAM_PLAYBACK,audio_out_handle);
        } else if (r < 0) {
            printf("write error: %s", snd_strerror(r));

            if(r == -EIO)
            printf("-EIO error!\n");
            else if(r == -EINVAL)
            printf("-EINVAL error!\n");
            else
            printf("unknown error!\n");

            exit(EXIT_FAILURE);
        }
        //printf("playback snd_pcm_writei = %d\n", r);
        if (r > 0) {
            result += r;
            count -= r;
            data += r * out_bits_per_frame / 8;
        }
    }
    return result;
}

int ForMatAudioHead(uchar *pBuffer, uint *pdwCount, uchar * pStream, uint len)
{
    if(AUDIO_DISABLE)
    {
        return 0;
    }
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//简易版
	{
		return 0;
	}
	
    pBuffer[0] = 0x00;
    pBuffer[1] = 0x00;
    pBuffer[2] = 0x01;
    pBuffer[3] = 0xF0;
    pBuffer[4] = 0x0E;
    pBuffer[5] = 0x02;

    pBuffer[6] = len & 0xFF;
    pBuffer[7] = (len >> 8) & 0xFF;

    memcpy(&pBuffer[8], pStream, len);
    *pdwCount = len + 8;
    //printf("[%s][%d][%s] ForMatAudioHead len=%d\n", __FILE__, __LINE__, __func__, len);
    
    return 0;
}

static size_t pcm_read(snd_pcm_uframes_t chunk_size, uchar *data, size_t rcount)
{
    size_t r;
    size_t result = 0;
    size_t count = rcount;

    if (count != chunk_size) {
        count = chunk_size;
    }

    while (count > 0) {
        r = snd_pcm_readi(audio_in_handle, data, count);
        //_printd("snd_pcm_readi r:%d\n", r);
        if (r == -EAGAIN || (r >= 0 && (size_t)r < count)) {
            snd_pcm_wait(audio_in_handle, 1000);
        } else if (r == -EPIPE) {
            xrun(SND_PCM_STREAM_CAPTURE, audio_in_handle);
            continue ;//yym
        } else if (r < 0) {
            printf("read error: %s", snd_strerror(r));
            exit(EXIT_FAILURE);
        }else if(r == -EBADFD) {
            printf("read error: %s", snd_strerror(r));
            return -1;
        }

        if (r > 0) {
            result += r;
            count -= r;
            data += r * in_bits_per_frame / 8;
        }else {
            return r;
        }
    }

    return result;
}

/// \defgroup AudioOutAPI API Audio Output 
/// 音频解码与输出控制设备接口，独立于回放设备。
/// \n 调用流程图:
/// \code
///    ===============================================
///               |
///          AudioCreate
///               |------------+------------------+
///               |            |                  |  
///               |      AudioOutSetFormat        | 
///               |            |                  |
///               |      AudioOutStart            |	
///        AudioSwitch         |--------------+   |
///               |      AudioOutPutBuf       |   |
///               |            |--------------+   |
///               |        AudioOutStop           |
///               |            |                  |
///               |------------+------------------+
///          AudioDestory
///               |
///    ===============================================
/// \endcode
/// @{


/// 创建音频设备
///
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int AudioCreate(void)
{
    _printd("Test\n");
    if(AUDIO_DISABLE)
    {
        return 0;
    }
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//简易版
	{
		return 0;
	}

    int err;
    size_t chunk_bytes;
    
    err = snd_pcm_open(&audio_out_handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if (err < 0) {
        printf("[%s][%d][%s] audio out open error: %s\n", __FILE__, __LINE__, __func__, snd_strerror(err));
        return 1;
    }
    
    out_hwparams.channels = 1;
    out_hwparams.format = SND_PCM_FORMAT_A_LAW;
    out_hwparams.rate = 8000;
    out_chunk_size = set_params(audio_out_handle, SND_PCM_STREAM_PLAYBACK, out_hwparams, &out_bits_per_frame);

    chunk_bytes = out_chunk_size * out_bits_per_frame / 8;
    _printd("chunk_bytes:%d\n",chunk_bytes);
    buf_out = (uchar *)malloc(chunk_bytes);
    out_tmp = (uchar *)malloc(chunk_bytes);
    snd_pcm_prepare(audio_out_handle);
    snd_pcm_start(audio_out_handle);

    #if (0 & G_PROJECT)
    char **AudioFileName;
    int i, iRet;
    AudioFileName = (char **)malloc(sizeof(char*)*5);
    for(i=0; i<5; i++)
    {
        AudioFileName[i] = (char *)malloc(sizeof(char)*255);
    }
    iRet = AudioGetAudioFiles("/media/sd", 1, AudioFileName, 255, MP3);
    if(iRet == 1)
    {
        AudioPlayAudioStart(AudioFileName[0], MP3, 1);
    }
    for(i=0; i<5; i++)
    {
        free(AudioFileName[i]);
    }
    free(AudioFileName);
    #endif
    
    return 0;
}


/// 销毁音频设备
///
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int AudioDestory(void)
{
    _printd("Test\n"); 
    
    if(AUDIO_DISABLE)
    {
        return 0;
    }
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//简易版
	{
		return 0;
	}

    snd_pcm_close(audio_out_handle);
    free(buf_out);
    free(out_tmp);

    return 0;
}


/// 切换音视频输出的类型。
///
/// \param [in] dwType 输出的类型，取下表中的值。audio_switch_t
/// \param [in] dwChannel 监视或回放的通道号。
/// \retval 0  切换成功。
/// \retval <0  切换失败。
int AudioSwitch(uint dwType, uint dwChannel){_printd("Test\n"); return 0;}

/// 开启语音对讲功能
///
/// \param 无
/// \return>0 开启失败
/// \retval 0 开启成功
int AudioOutStart(void)
{
    _printd("Test\n"); 
    if(AUDIO_DISABLE)
    {
        return 0;
    }
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//简易版
	{
		return 0;
	}

    if(!flag_talk) {
        flag_talk = 1;
    }

    return 0;
}

/// 停止语音对讲功能
///
/// \param 无
/// \return>0 停止失败
/// \retval 0 停止成功
int AudioOutStop(void)
{
    _printd("Test\n"); 
    if(AUDIO_DISABLE)
    {
        return 0;
    }
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//简易版
	{
		return 0;
	}

    //snd_pcm_drop(audio_out_handle);

    return 0;
}

/// 语音对讲解码数据填充
///
/// \param [in] src 解码数据缓冲地址
/// \param [in] size 解码数据缓冲长度
/// \return>0 填充失败
/// \retval 0 填充成功
void AudioOutPutBuf(unsigned char *src, int size)
{
    _printd("Test\n");

    if(AUDIO_DISABLE)
    {
        return 0;
    }
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//简易版
	{
		return 0;
	}

    if(flag_audio_play)
    {
        return ;
    }

    int AudioType 	= 0;
    AudioType = src[4];
    snd_pcm_uframes_t c;

    if (G711_ALAW != AudioType && G711_ULAW != AudioType)
    {
        AudioType = 0;
        printf("G711_ALAW != AudioType && G711_ULAW != AudioType\n");
    }
    else
    {
        memcpy(buf_out, &src[8], size - 8); size -= 8;
    }

    c = handle_data_pb(size, buf_out, out_tmp);
    //printf("AudioOutPutBuf c = %d\n",c);
    pcm_write(c, buf_out);

    return ;
}


int AudioOutSetVolume(int iChannel, uint dwLVolume, uint dwRVolume)
{
    if(AUDIO_DISABLE)
    {
        return 0;
    }
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//简易版
	{
		return 0;
	}

    _printd("iChannel =%d dwLVolume = %d dwRVolume = %d\n", iChannel, dwLVolume, dwRVolume);
    snd_mixer_t *mixerFd;
    snd_mixer_elem_t *elem;
    int result;
    char mixer_name[50];
    //音量修正，防止设置的音量太低听不到声音
    int vol = 155;

    if ((result = snd_mixer_open( &mixerFd, 0)) < 0){
        printf("snd_mixer_open error\n");
        mixerFd = NULL;
    }

    // Attach an HCTL to an opened mixer
    if ((result = snd_mixer_attach( mixerFd, "default")) < 0){
        printf("snd_mixer_attach error\n");
        snd_mixer_close(mixerFd);
        mixerFd = NULL;
    }

    // 注册混音器
    if ((result = snd_mixer_selem_register( mixerFd, NULL, NULL)) < 0){
        printf("snd_mixer_selem_register error\n");
        snd_mixer_close(mixerFd);
        mixerFd = NULL;
    }
    // 加载混音器
    if ((result = snd_mixer_load( mixerFd)) < 0){
        printf("snd_mixer_load error\n");
        snd_mixer_close(mixerFd);
        mixerFd = NULL;
    }
    for(elem=snd_mixer_first_elem(mixerFd); elem; elem=snd_mixer_elem_next(elem))
    {
        // 找到可以用的, 激活的elem
        if(snd_mixer_elem_get_type(elem) == SND_MIXER_ELEM_SIMPLE &&
            snd_mixer_selem_is_active(elem))
        {
            strcpy(mixer_name, snd_mixer_selem_get_name(elem));

            if(0 == strcmp(mixer_name, "Left")){
                result = snd_mixer_selem_set_playback_volume_range(elem,0,255);
                result = snd_mixer_selem_set_playback_volume_all(elem, vol+dwLVolume);
            }
            if(0 == strcmp(mixer_name, "Right")){
                result = snd_mixer_selem_set_playback_volume_range(elem,0,255);
                result = snd_mixer_selem_set_playback_volume_all(elem, vol+dwRVolume);
            }
        }
    }

    snd_mixer_close(mixerFd);

    return 0;
}

int AudioOutGetCaps(AUDIOIN_CAPS * pCaps)
{
    _printd("Test\n");
    return 0;
}

/// 设置数据的压缩格式
///
/// \param [in] coderType 压缩格式
/// \return>0 设置失败
/// \retval 0 设置成功
int AudioOutSetFormat(int coderType)
{
    _printd("Test\n");
    //音频配置写死
    return 0;
}

/*!
获得语音对讲支持的编码格式。
\param AUDIOIN_FORMAT* pFormat: 保存的格式。
\param int iMax: 最大个数
\return 支持的个数
*/
int AudioGetFormat(AUDIOIN_FORMAT* pFormat, int iMax)
{
    _printd("Test\n"); 
    if(AUDIO_DISABLE)
    {
        return 0;
    }
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//简易版
	{
		return 0;
	}

    int i;
    if(pFormat == NULL)
    {
        return -1;
    }
    if(iMax <= 0)
    {
        return -1;
    }
    for(i = 0;i < iMax && i < MAX_HI_AUDIOTYPE; i++)
    {
        pFormat[i].BitRate      = Convert2EncodeType[i].BitRate;
        pFormat[i].SampleRate   = Convert2EncodeType[i].SampleRate;
        pFormat[i].SampleBit    = Convert2EncodeType[i].SampleBit;
        pFormat[i].EncodeType   = Convert2EncodeType[i].EncodeType;
    }

    return i;
}



int AudioInModeChange(const AUDIO_INPUT_MODE sample_mode){_printd("Test\n"); return 0;}

/// 得到额外的音频输入通道数, 不包括录像通道。以下所有AudioIn均表示额外的音频输入。
///
/// \param 无
/// \return额外的音频输入通道数目。
int AudioInGetChannels(void)
{
    _printd("Test\n"); 
    //这里不能return 0，否则上层不会把音频数据发送出去
    return 1;
}


/// 以下处理语音对讲中的音频输入部分,应用程序调用相应接口获取编码后的语音数据
/// =========================================================================

void *AudioInGetStreamTask(void *args)
{
    sleep(1);
    while (1) {
        if (flag_talk) {
            //开启对讲功能
            usleep(100*1000);
            continue;
        }
        int actual_capture_size;
        snd_pcm_uframes_t c;
        //每次读取480byte音频数据
        ssize_t expected_size = 480;
        snd_pcm_uframes_t expected_frame = expected_size * 8 / in_bits_per_frame;
        actual_capture_size = pcm_read(expected_frame, buf_in, in_chunk_size);
        if (actual_capture_size <= 0) {
            printf("[%s][%d][%s]pcm_read error %d\n", __FILE__, __LINE__, __func__, actual_capture_size);
        }
        //printf("[%s][%d]pcm_read  %d\n", __FILE__, __LINE__, actual_capture_size);
        c = handle_data_cp(expected_frame, buf_in, in_tmp);
        //_printd("audio_pthread, %d\n", c);
        if(venc_chn[CHL_MAIN_T].enable == 1){
            CaptureDev_SaveAencList(c, buf_in, CHL_MAIN_T);
        }
        if(venc_chn[CHL_2END_T].enable == 1){
            CaptureDev_SaveAencList(c, buf_in, CHL_2END_T);
        }
    }
}

int AudioInGetStream(void)
{
    static int  mark_audio_task_run = 0;
    pthread_t   pidId               = -1;
    int         iRet                = 0;
    if (1 == mark_audio_task_run) {
        printf("Pthread AudioInGetStream was created\n");
        return 0;
    } else if (0 == mark_audio_task_run) {
        iRet = pthread_create(&pidId, NULL, AudioInGetStreamTask, NULL);
        if(0 == iRet){
            //gAudioParam.AiTaskRun = 1;
            mark_audio_task_run = 1;
            return 0;
        }
        return -1;
    }

    return 0;
}

/// 创建音频输入设备
///
/// \param [in] iChannel 通道号
/// \retval <0 创建失败
/// \retval 0 创建成功
int AudioInCreate(int iChannel)
{
    _printd("Test\n");
    if(AUDIO_DISABLE)
    {
        return 0;
    }
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//简易版
	{
		return 0;
	}

    int err;
    size_t chunk_bytes;
    
    err = snd_pcm_open(&audio_in_handle, "default", SND_PCM_STREAM_CAPTURE, 0);
    if (err < 0) {
        _printd("audio in open error: %s\n", snd_strerror(err));
        return 1;
    }
    
    in_hwparams.channels = 1;
    in_hwparams.format = SND_PCM_FORMAT_A_LAW;
    in_hwparams.rate = 8000;
    in_chunk_size = set_params(audio_in_handle, SND_PCM_STREAM_CAPTURE, in_hwparams, &in_bits_per_frame);

    chunk_bytes = in_chunk_size * in_bits_per_frame / 8;
    buf_in = (uchar *)malloc(chunk_bytes);
    in_tmp = (uchar *)malloc(chunk_bytes);
    //创建监听线程
    AudioInGetStream();

    return 0;
}


/// 销毁音频输入设备
///
/// \param [in] iChannel 通道号
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int AudioInDestroy(int iChannel)
{
    _printd("Test\n"); 
    if(AUDIO_DISABLE)
    {
        return 0;
    }
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//简易版
	{
		return 0;
	}

    snd_pcm_close(audio_in_handle);
    free(buf_in);
    free(in_tmp);

    return 0;
}


/// 开始音频编码
///
/// \param [in] iChannel 通道号
/// \retval <0 开始失败
/// \retval 0 开始成功
int AudioInStart(int iChannel)
{
    _printd("Test\n");
    if(AUDIO_DISABLE)
    {
        return 0;
    }
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//简易版
	{
		return 0;
	}

    snd_pcm_prepare(audio_in_handle);
    snd_pcm_start(audio_in_handle);
    //开启语言对讲，停止向链表投放音频数据
    if(!flag_talk) {
        flag_talk = 1;
    }

    return 0;
}


/// 停止音频编码
///
/// \param [in] iChannel 通道号
/// \retval <0 停止失败
/// \retval 0 停止成功
int AudioInStop(int iChannel)
{
    _printd("Test\n");
    if(AUDIO_DISABLE)
    {
        return 0;
    }
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//简易版
	{
		return 0;
	}

    //snd_pcm_drop(audio_in_handle);
    if(flag_talk) {
        flag_talk = 0;
    }

    return 0;
}


/// 读取采样数据，读为阻塞超时模式。超时时立即返回，传出实际读到的数据，将超时时间
/// 设置的较小，可以提高数据的实时性。
///
/// \param [in] iChannel 通道号，-1表示读所有通道的数据，读到的通道号作为函数返回值返回。
/// \param [in] pBuffer 用来存放音频编码数据的缓冲指针。
/// \param [in,out] pdwCount 传入的值是缓冲大小，传出的值是实际读到数据的长度。
/// \param [in] dwTimeout 读超时的时间，单位为毫秒，设为0表示一直等到读出数据或出错为止。
/// \return>=0 通道号（在输入通道号为-1时）
/// \retval 0  读成功
/// \retval <0  读失败
int AudioInGetBuffer(int iChannel, uchar* pBuffer, uint* pdwCount, uint dwTimeout)
{
    if(AUDIO_DISABLE)
    {
        return 0;
    }
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//简易版
	{
		return 0;
	}

    //这里一次获取的音频数据大小不要超过上层一个packet申请得到的内存大小(一般是1024Byte)
    unsigned int len;
    //每次获取音频的大小，A-LAW一帧数据量为1byte
    ssize_t c = 480;
    snd_pcm_uframes_t r,f = c * 8 / in_bits_per_frame;
    if(flag_talk) {
        r = pcm_read(f, buf_in, in_chunk_size);
        if (r > *pdwCount) {
            _printd("pcm_read size error %ld\n", r);
            r = 200;
        } else if (r <= 0) {
            _printd("pcm_read error %ld\n", r);
            return -1;
        }
        len = handle_data_cp(r, buf_in, in_tmp);
        ForMatAudioHead(pBuffer, pdwCount, buf_in, len);
    }

    return 0;
}


/// 设置音频数据编码的格式。
///
/// \param [in] iChannel 通道号。
/// \param [in] pFormat 指向音频编码格式结构AUDIOIN _FORMAT的指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int AudioInSetFormat(int iChannel, AUDIOIN_FORMAT * pFormat)
{
    _printd("Test\n");
    //音频配置写死
    return 0;
}


/// 设置音频输入的音量。
///
/// \param [in] iChannel 通道号。
/// \param [in] dwLVolume 左声道音量，取值0-100。
/// \param [in] dwRVolume 右声道音量，取值0-100。
/// \retval 0  设置成功
/// \retval <0  设置失败
int AudioInSetVolume(int iChannel, uint dwLVolume, uint dwRVolume)
{
    if(AUDIO_DISABLE)
    {
        return 0;
    }
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//简易版
	{
		return 0;
	}

    _printd("iChannel =%d dwLVolume = %d dwRVolume = %d\n", iChannel, dwLVolume, dwRVolume);
    snd_mixer_t *mixerFd;
    snd_mixer_elem_t *elem;
    int result;
    char mixer_name[50];

    if ((result = snd_mixer_open( &mixerFd, 0)) < 0){
        printf("snd_mixer_open error\n");
        mixerFd = NULL;
    }

    // Attach an HCTL to an opened mixer
    if ((result = snd_mixer_attach( mixerFd, "default")) < 0){
        printf("snd_mixer_attach error\n");
        snd_mixer_close(mixerFd);
        mixerFd = NULL;
    }

    // 注册混音器
    if ((result = snd_mixer_selem_register( mixerFd, NULL, NULL)) < 0){
        printf("snd_mixer_selem_register error\n");
        snd_mixer_close(mixerFd);
        mixerFd = NULL;
    }
    // 加载混音器
    if ((result = snd_mixer_load( mixerFd)) < 0){
        printf("snd_mixer_load error\n");
        snd_mixer_close(mixerFd);
        mixerFd = NULL;
    }
    for(elem=snd_mixer_first_elem(mixerFd); elem; elem=snd_mixer_elem_next(elem))
    {
        // 找到可以用的, 激活的elem
        if(snd_mixer_elem_get_type(elem) == SND_MIXER_ELEM_SIMPLE &&
            snd_mixer_selem_is_active(elem))
        {
            strcpy(mixer_name, snd_mixer_selem_get_name(elem));

            if(0 == strcmp(mixer_name, "Left")){
                result = snd_mixer_selem_set_capture_volume_range(elem,0,100);
                result = snd_mixer_selem_set_capture_volume_all(elem, dwLVolume);
            }
            if(0 == strcmp(mixer_name, "Right")){
                result = snd_mixer_selem_set_capture_volume_range(elem,0,100);
                result = snd_mixer_selem_set_capture_volume_all(elem, dwRVolume);
            }
        }
    }

    snd_mixer_close(mixerFd);

    return 0;
}


/// 得到音频输入支持的特性。
///
/// \param [out] pCaps 指向音频输入特性结构AUDIOIN_CAPS的指针。
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int AudioInGetCaps(AUDIOIN_CAPS * pCaps)
{ 
	_printd("Test\n");
    if(AUDIO_DISABLE)
    {
        return 0;
    }

	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//简易版
	{
		return 0;
	}
		
    if (NULL == pCaps)
    {
        printf("AudioInGetCaps():pCaps = NULL\r\n");
    }
    pCaps->ucAudioInType = BITMSK(0);

    return 0;
}

int AudioGetAudioFiles(char *FilePath, int MaxNum, char *AudioFileName[], int FileNameMaxLen, enum audio_encode_type type)
{
    if(AUDIO_DISABLE)
    {
        return 0;
    }
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//简易版
	{
		return 0;
	}

    DIR *TopDir = NULL;
    struct dirent *tmpDirent = NULL;
    char tmpFilePath[255];
    char FileType[16];
    char *tmpFileType;
    int iRet = 0;
    int FilesCount = 0;
    
    switch(type)
    {
        case G711_ALAW:
            strcpy(FileType, ".g711");
            break;
        case G711_ULAW:
            strcpy(FileType, ".g711");
            break;
        case MP3:
            strcpy(FileType, ".mp3");
            break;
        default:
            _printd("not support type %d\n", type);
            return -1;
            break;
    }
    if (NULL == (TopDir = opendir(FilePath)))
    {
        _printd("error dir %s\n", FilePath);
        return -2;
    }
    while (NULL != (tmpDirent = readdir(TopDir)) && FilesCount < MaxNum)
    {
        if (DT_DIR == tmpDirent->d_type)
        {
            if (0 == strcmp(tmpDirent->d_name, ".") || 0 == strcmp(tmpDirent->d_name, ".."))
            {
                continue;
            }
            if ((strlen(FilePath) + strlen(tmpDirent->d_name) + 1) < sizeof(tmpFilePath))
            {
                strcpy(tmpFilePath, FilePath);
                strcat(tmpFilePath, "/");
                strcat(tmpFilePath, tmpDirent->d_name);
                iRet = AudioGetAudioFiles(tmpFilePath, (MaxNum-FilesCount), (AudioFileName+FilesCount), FileNameMaxLen, type);
                if(iRet > 0)
                {
                    FilesCount += iRet;
                }
            }
        }
        if (DT_REG == tmpDirent->d_type)
        {
            tmpFileType = tmpDirent->d_name + (strlen(tmpDirent->d_name) - strlen(FileType));
            if (0 == strcmp(tmpFileType, FileType))
            {
                if ((strlen(FilePath) + strlen(tmpDirent->d_name) + 1) < FileNameMaxLen)
                {
                    strcpy(AudioFileName[FilesCount], FilePath);
                    strcat(AudioFileName[FilesCount], "/");
                    strcat(AudioFileName[FilesCount], tmpDirent->d_name);
                    FilesCount++;
                }
            }
        }
    }
    closedir(TopDir);
    
    return FilesCount;
}

typedef struct tagAUDIO_PLAY_FILE
{
    char loop;
    enum audio_encode_type type;
    char AudioFileName[255];
}AUDIO_PLAY_FILE;

static size_t lenOfId3v2Tag(unsigned char const* buf)
{
    unsigned int b0 = buf[0] & 127;
    unsigned int b1 = buf[1] & 127;
    unsigned int b2 = buf[2] & 127;
    unsigned int b3 = buf[3] & 127;
    return (((((b0 << 7) + b1) << 7) + b2) << 7) + b3;
}

static int lame_decode_initfile(FILE * mp3fp, mp3data_struct * mp3data, int *enc_delay, int *enc_padding)
{
    int len;
    unsigned char mp3_buf[100];
    
    memset(mp3data, 0, sizeof(mp3data_struct));
    if (4 != fread(mp3_buf, 1, 4, mp3fp)) { 
        return -1;
    }
    while (mp3_buf[0] == 'I' && mp3_buf[1] == 'D' && mp3_buf[2] == '3') { 
        if (6 != fread(&mp3_buf[4], 1, 6, mp3fp)) { 
            return -1;
        }
        len = lenOfId3v2Tag(&mp3_buf[6]);
        fseek(mp3fp, len, SEEK_CUR);
        if (4 != fread(mp3_buf, 1, 4, mp3fp)) { 
            return -1;
        }
    }
    if (0 == memcmp(mp3_buf, "AiD\1", 4)) { 
        if (2 != fread(mp3_buf, 1, 2, mp3fp)) { 
            return -1;
        }
        len = mp3_buf[0] + 256 * mp3_buf[1] - 6;
        fseek(mp3fp, len, SEEK_CUR);
        if (4 != fread(mp3_buf, 1, 4, mp3fp)) { 
            return -1;
        }
    }
    return 0;
}

void *AudioPlayFileTask(void *args)
{
    AUDIO_PLAY_FILE audio_play_file = *(AUDIO_PLAY_FILE *)args;
    FILE *fp;
    int read_bytes;
    snd_pcm_uframes_t c;

    pthread_detach(pthread_self());
    _printd("AudioPlayFileTask %s,%d,%d\n", audio_play_file.AudioFileName, audio_play_file.type, audio_play_file.loop);
    fp = fopen(audio_play_file.AudioFileName, "r");
    if(fp == NULL)
    {
        flag_audio_play = 0;
        perror("fopen");
        return NULL;
    }
    switch(audio_play_file.type)
    {
        case G711_ALAW:
        case G711_ULAW:
            while(flag_audio_play)
            {
                read_bytes = fread(buf_out, 1, 480, fp);
                if(read_bytes <= 0)
                {
                    if(audio_play_file.loop)
                    {
                        fclose(fp);
                        fp = fopen(audio_play_file.AudioFileName, "r");
                        if(fp == NULL)
                        {
                            flag_audio_play = 0;
                            perror("fopen");
                            return NULL;
                        }
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
                c = handle_data_pb(read_bytes, buf_out, out_tmp);
                pcm_write(c, buf_out);
            }
            break;
        case MP3:
            out_hwparams.channels = 1;
            out_hwparams.format = SND_PCM_FORMAT_S16_LE;
            out_hwparams.rate = 40000;
            set_params(audio_out_handle, SND_PCM_STREAM_PLAYBACK, out_hwparams, &out_bits_per_frame);
            if (0) {
                FILE *pcmfp = fopen("/appex/tmp/ll.pcm", "r");
                unsigned char mp3buf[2304];
                while ((read_bytes = fread(mp3buf, 1, 2304, pcmfp)) > 0) {
                    pcm_write(read_bytes, mp3buf);
                }
                fclose(pcmfp);
            } else {
                #if G_PROJECT
                unsigned char mp3buf[1024];
                short pcm_l[1152];
                short pcm_r[1152];
                int enc_delay = 0;
                int enc_padding = 0;
                int decode_bytes = 0;
                mp3data_struct mp3data;
                hip_t hip = hip_decode_init();
                if (0 > lame_decode_initfile(fp, &mp3data, &enc_delay, &enc_padding)) {
                    _printd("lame_decode_initfile return error\n");
                    fclose(fp);
                    break;
                }
                while (flag_audio_play) {
                    read_bytes = fread(mp3buf, 1, 1024, fp);
                    read_bytes = fread(mp3buf, 1, 1024, fp);
                    if (read_bytes <= 0) {
                        decode_bytes = hip_decode1_headers(hip, mp3buf, read_bytes, pcm_l, pcm_r, &mp3data);
                        if (decode_bytes == -1) {
                            if (audio_play_file.loop) {
                                fclose(fp);
                                fp = fopen(audio_play_file.AudioFileName, "r");
                                if(fp == NULL)
                                {
                                    flag_audio_play = 0;
                                    perror("fopen");
                                    return NULL;
                                }
                                continue;
                            } else {
                                break;
                            }
                        }
                    } else {
                        decode_bytes = hip_decode1_headers(hip, mp3buf, read_bytes, pcm_l, pcm_r, &mp3data);
                    }
                    if (decode_bytes > 0) {
                        c = handle_data_pb(2*decode_bytes, (uchar *)pcm_l, (uchar *)pcm_r);
                        pcm_write(c, (uchar *)pcm_l);
                    }
                }
                hip_decode_exit(hip);
                #endif
            }
            out_hwparams.channels = 1;
            out_hwparams.format = SND_PCM_FORMAT_A_LAW;
            out_hwparams.rate = 8000;
            set_params(audio_out_handle, SND_PCM_STREAM_PLAYBACK, out_hwparams, &out_bits_per_frame);
            break;
        default:
            _printd("not support type %d\n", audio_play_file.type);
            break;
    }
    fclose(fp);
    flag_audio_play = 0;
    
    return NULL;
}

int AudioPlayAudioStart(char *AudioFileName, enum audio_encode_type type, char loop)
{
    pthread_t pid_audio_play = 0;
    AUDIO_PLAY_FILE audio_play_file;
    strcpy(audio_play_file.AudioFileName, AudioFileName);
    audio_play_file.type = type;
    audio_play_file.loop = loop;
    
    if(1 == flag_audio_play)
    {
        _printd("audio already in play\n");
        return -1;
    }
    else
    {
        flag_audio_play = 1;
        pthread_create(&pid_audio_play, NULL, AudioPlayFileTask, (void*)&audio_play_file);
        usleep(100);
    }
    return 0;
}

int AudioPlayAudioStop()
{
    flag_audio_play = 0;
    return 0;
}

int AudioPlayAudioStatus()
{
    return flag_audio_play;
}


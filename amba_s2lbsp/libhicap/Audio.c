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

//audio_out IPC�����ⲿ��Ƶ���ݽ���ط�
static snd_pcm_t *audio_out_handle;
static uchar *buf_out;
static uchar *out_tmp;

//audio_in IPC����mic���룬��Ƶ����
static snd_pcm_t *audio_in_handle;
static uchar *buf_in;
static uchar *in_tmp;
//�����Խ���־λ
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


//�����Ƶ����xrun״̬�Ĵ��� 
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
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//���װ�
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
/// ��Ƶ��������������豸�ӿڣ������ڻط��豸��
/// \n ��������ͼ:
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


/// ������Ƶ�豸
///
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int AudioCreate(void)
{
    _printd("Test\n");
    if(AUDIO_DISABLE)
    {
        return 0;
    }
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//���װ�
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


/// ������Ƶ�豸
///
/// \param ��
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int AudioDestory(void)
{
    _printd("Test\n"); 
    
    if(AUDIO_DISABLE)
    {
        return 0;
    }
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//���װ�
	{
		return 0;
	}

    snd_pcm_close(audio_out_handle);
    free(buf_out);
    free(out_tmp);

    return 0;
}


/// �л�����Ƶ��������͡�
///
/// \param [in] dwType ��������ͣ�ȡ�±��е�ֵ��audio_switch_t
/// \param [in] dwChannel ���ӻ�طŵ�ͨ���š�
/// \retval 0  �л��ɹ���
/// \retval <0  �л�ʧ�ܡ�
int AudioSwitch(uint dwType, uint dwChannel){_printd("Test\n"); return 0;}

/// ���������Խ�����
///
/// \param ��
/// \return>0 ����ʧ��
/// \retval 0 �����ɹ�
int AudioOutStart(void)
{
    _printd("Test\n"); 
    if(AUDIO_DISABLE)
    {
        return 0;
    }
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//���װ�
	{
		return 0;
	}

    if(!flag_talk) {
        flag_talk = 1;
    }

    return 0;
}

/// ֹͣ�����Խ�����
///
/// \param ��
/// \return>0 ֹͣʧ��
/// \retval 0 ֹͣ�ɹ�
int AudioOutStop(void)
{
    _printd("Test\n"); 
    if(AUDIO_DISABLE)
    {
        return 0;
    }
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//���װ�
	{
		return 0;
	}

    //snd_pcm_drop(audio_out_handle);

    return 0;
}

/// �����Խ������������
///
/// \param [in] src �������ݻ����ַ
/// \param [in] size �������ݻ��峤��
/// \return>0 ���ʧ��
/// \retval 0 ���ɹ�
void AudioOutPutBuf(unsigned char *src, int size)
{
    _printd("Test\n");

    if(AUDIO_DISABLE)
    {
        return 0;
    }
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//���װ�
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
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//���װ�
	{
		return 0;
	}

    _printd("iChannel =%d dwLVolume = %d dwRVolume = %d\n", iChannel, dwLVolume, dwRVolume);
    snd_mixer_t *mixerFd;
    snd_mixer_elem_t *elem;
    int result;
    char mixer_name[50];
    //������������ֹ���õ�����̫������������
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

    // ע�������
    if ((result = snd_mixer_selem_register( mixerFd, NULL, NULL)) < 0){
        printf("snd_mixer_selem_register error\n");
        snd_mixer_close(mixerFd);
        mixerFd = NULL;
    }
    // ���ػ�����
    if ((result = snd_mixer_load( mixerFd)) < 0){
        printf("snd_mixer_load error\n");
        snd_mixer_close(mixerFd);
        mixerFd = NULL;
    }
    for(elem=snd_mixer_first_elem(mixerFd); elem; elem=snd_mixer_elem_next(elem))
    {
        // �ҵ������õ�, �����elem
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

/// �������ݵ�ѹ����ʽ
///
/// \param [in] coderType ѹ����ʽ
/// \return>0 ����ʧ��
/// \retval 0 ���óɹ�
int AudioOutSetFormat(int coderType)
{
    _printd("Test\n");
    //��Ƶ����д��
    return 0;
}

/*!
��������Խ�֧�ֵı����ʽ��
\param AUDIOIN_FORMAT* pFormat: ����ĸ�ʽ��
\param int iMax: ������
\return ֧�ֵĸ���
*/
int AudioGetFormat(AUDIOIN_FORMAT* pFormat, int iMax)
{
    _printd("Test\n"); 
    if(AUDIO_DISABLE)
    {
        return 0;
    }
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//���װ�
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

/// �õ��������Ƶ����ͨ����, ������¼��ͨ������������AudioIn����ʾ�������Ƶ���롣
///
/// \param ��
/// \return�������Ƶ����ͨ����Ŀ��
int AudioInGetChannels(void)
{
    _printd("Test\n"); 
    //���ﲻ��return 0�������ϲ㲻�����Ƶ���ݷ��ͳ�ȥ
    return 1;
}


/// ���´��������Խ��е���Ƶ���벿��,Ӧ�ó��������Ӧ�ӿڻ�ȡ��������������
/// =========================================================================

void *AudioInGetStreamTask(void *args)
{
    sleep(1);
    while (1) {
        if (flag_talk) {
            //�����Խ�����
            usleep(100*1000);
            continue;
        }
        int actual_capture_size;
        snd_pcm_uframes_t c;
        //ÿ�ζ�ȡ480byte��Ƶ����
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

/// ������Ƶ�����豸
///
/// \param [in] iChannel ͨ����
/// \retval <0 ����ʧ��
/// \retval 0 �����ɹ�
int AudioInCreate(int iChannel)
{
    _printd("Test\n");
    if(AUDIO_DISABLE)
    {
        return 0;
    }
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//���װ�
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
    //���������߳�
    AudioInGetStream();

    return 0;
}


/// ������Ƶ�����豸
///
/// \param [in] iChannel ͨ����
/// \retval <0 ����ʧ��
/// \retval 0 ���ٳɹ�
int AudioInDestroy(int iChannel)
{
    _printd("Test\n"); 
    if(AUDIO_DISABLE)
    {
        return 0;
    }
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//���װ�
	{
		return 0;
	}

    snd_pcm_close(audio_in_handle);
    free(buf_in);
    free(in_tmp);

    return 0;
}


/// ��ʼ��Ƶ����
///
/// \param [in] iChannel ͨ����
/// \retval <0 ��ʼʧ��
/// \retval 0 ��ʼ�ɹ�
int AudioInStart(int iChannel)
{
    _printd("Test\n");
    if(AUDIO_DISABLE)
    {
        return 0;
    }
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//���װ�
	{
		return 0;
	}

    snd_pcm_prepare(audio_in_handle);
    snd_pcm_start(audio_in_handle);
    //�������ԶԽ���ֹͣ������Ͷ����Ƶ����
    if(!flag_talk) {
        flag_talk = 1;
    }

    return 0;
}


/// ֹͣ��Ƶ����
///
/// \param [in] iChannel ͨ����
/// \retval <0 ֹͣʧ��
/// \retval 0 ֹͣ�ɹ�
int AudioInStop(int iChannel)
{
    _printd("Test\n");
    if(AUDIO_DISABLE)
    {
        return 0;
    }
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//���װ�
	{
		return 0;
	}

    //snd_pcm_drop(audio_in_handle);
    if(flag_talk) {
        flag_talk = 0;
    }

    return 0;
}


/// ��ȡ�������ݣ���Ϊ������ʱģʽ����ʱʱ�������أ�����ʵ�ʶ��������ݣ�����ʱʱ��
/// ���õĽ�С������������ݵ�ʵʱ�ԡ�
///
/// \param [in] iChannel ͨ���ţ�-1��ʾ������ͨ�������ݣ�������ͨ������Ϊ��������ֵ���ء�
/// \param [in] pBuffer ���������Ƶ�������ݵĻ���ָ�롣
/// \param [in,out] pdwCount �����ֵ�ǻ����С��������ֵ��ʵ�ʶ������ݵĳ��ȡ�
/// \param [in] dwTimeout ����ʱ��ʱ�䣬��λΪ���룬��Ϊ0��ʾһֱ�ȵ��������ݻ����Ϊֹ��
/// \return>=0 ͨ���ţ�������ͨ����Ϊ-1ʱ��
/// \retval 0  ���ɹ�
/// \retval <0  ��ʧ��
int AudioInGetBuffer(int iChannel, uchar* pBuffer, uint* pdwCount, uint dwTimeout)
{
    if(AUDIO_DISABLE)
    {
        return 0;
    }
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//���װ�
	{
		return 0;
	}

    //����һ�λ�ȡ����Ƶ���ݴ�С��Ҫ�����ϲ�һ��packet����õ����ڴ��С(һ����1024Byte)
    unsigned int len;
    //ÿ�λ�ȡ��Ƶ�Ĵ�С��A-LAWһ֡������Ϊ1byte
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


/// ������Ƶ���ݱ���ĸ�ʽ��
///
/// \param [in] iChannel ͨ���š�
/// \param [in] pFormat ָ����Ƶ�����ʽ�ṹAUDIOIN _FORMAT��ָ�롣
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int AudioInSetFormat(int iChannel, AUDIOIN_FORMAT * pFormat)
{
    _printd("Test\n");
    //��Ƶ����д��
    return 0;
}


/// ������Ƶ�����������
///
/// \param [in] iChannel ͨ���š�
/// \param [in] dwLVolume ������������ȡֵ0-100��
/// \param [in] dwRVolume ������������ȡֵ0-100��
/// \retval 0  ���óɹ�
/// \retval <0  ����ʧ��
int AudioInSetVolume(int iChannel, uint dwLVolume, uint dwRVolume)
{
    if(AUDIO_DISABLE)
    {
        return 0;
    }
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//���װ�
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

    // ע�������
    if ((result = snd_mixer_selem_register( mixerFd, NULL, NULL)) < 0){
        printf("snd_mixer_selem_register error\n");
        snd_mixer_close(mixerFd);
        mixerFd = NULL;
    }
    // ���ػ�����
    if ((result = snd_mixer_load( mixerFd)) < 0){
        printf("snd_mixer_load error\n");
        snd_mixer_close(mixerFd);
        mixerFd = NULL;
    }
    for(elem=snd_mixer_first_elem(mixerFd); elem; elem=snd_mixer_elem_next(elem))
    {
        // �ҵ������õ�, �����elem
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


/// �õ���Ƶ����֧�ֵ����ԡ�
///
/// \param [out] pCaps ָ����Ƶ�������ԽṹAUDIOIN_CAPS��ָ�롣
/// \retval 0  ��ȡ�ɹ���
/// \retval <0  ��ȡʧ�ܡ�
int AudioInGetCaps(AUDIOIN_CAPS * pCaps)
{ 
	_printd("Test\n");
    if(AUDIO_DISABLE)
    {
        return 0;
    }

	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//���װ�
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
	
	if(NULL != strcasestr(GetVersioninfo(), "0.3"))//���װ�
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


#ifndef AMBAS_IMGPROC_IOCTL_ARCH_H
#define AMBAS_IMGPROC_IOCTL_ARCH_H

#include <linux/ioctl.h>

/*
 * image processing APIs	- 'p'
 */
#define	IAVIOC_IMAGE_MAGIC		'p'
#define	IAVIMG_IO(nr)				_IO(IAVIOC_IMAGE_MAGIC, nr)
#define	IAVIMG_IOR(nr, size)			_IOR(IAVIOC_IMAGE_MAGIC, nr, size)
#define	IAVIMG_IOW(nr, size)		_IOW(IAVIOC_IMAGE_MAGIC, nr, size)
#define	IAVIMG_IOWR(nr, size)		_IOWR(IAVIOC_IMAGE_MAGIC, nr, size)

enum {
	//ISP filter control, start from 0x01
	IOC_IMG_CONFIG_STATISTICS = 0x01,
	IOC_IMG_CONFIG_FLOAT_STATISTICS = 0x02,
	IOC_IMG_AAA_SETUP_EX = 0x3,
	IOC_IMG_NOISE_FILTER_SETUP = 0x04,
	IOC_IMG_BLACK_LEVEL_GLOBAL_OFFSET = 0x05,
	IOC_IMG_BAD_PIXEL_CORRECTION = 0x06,
	IOC_IMG_CFA_LEAKAGE_FILTER_SETUP = 0x07,
	IOC_IMG_CFA_NOISE_FILTER_SETUP = 0x08,
	IOC_IMG_RGB_GAIN_ADJUST = 0x09,
	IOC_IMG_VIGNETTE_COMPENSATION = 0x0A,
	IOC_IMG_LOCAL_EXPOSURE = 0x0B,
	IOC_IMG_COLOR_CORRECTION = 0x0C,
	IOC_IMG_RGB_TO_YUV_SETUP = 0x0D,
	IOC_IMG_CHROMA_SCALE = 0x0E,
	IOC_IMG_CHROMA_MEDIAN_FILTER_SETUP = 0x0F,
	IOC_IMG_LUMA_SHARPENING = 0x10,
	IOC_IMG_LUMA_SHARPENING_FIR_CONFIG = 0x11,
	IOC_IMG_LUMA_SHARPENING_BLEND_CONFIG = 0x12,
	IOC_IMG_LUMA_SHARPENING_LEVEL_CONTROL = 0x13,
	IOC_IMG_MCTF_GMV_SETUP = 0x14,
	IOC_IMG_MCTF_MV_STABILIZER_SETUP = 0x15,
	IOC_IMG_ANTI_ALIASING_CONFIG = 0x16,
	IOC_IMG_DIGITAL_SATURATION_LEVEL = 0x17,
	IOC_IMG_DEMOSAIC_CONIFG = 0x18,
	IOC_IMG_SENSOR_CONFIG = 0x19,
	IOC_IMG_STATIC_BAD_PIXEL_CORRECTION = 0x1A,
	IOC_IMG_LUMA_SHARPENING_TONE = 0x1B,
	IOC_IMG_CHROMA_NOISE_FILTER_SETUP = 0x1C,
	IOC_IMG_LUMA_SHARPENING_LNL = 0x1D,
	IOC_IMG_STRONG_GRGB_MISMATCH = 0x1E,
	IOC_IMG_CONFIG_HISTOGRAM = 0x1F,

	// HDR related, start from 0x48
	IOC_IMG_HDR_SET_VIDEO_PROC = 0x48,
	IOC_IMG_HDR_SET_BUF_ID = 0x49,
	IOC_IMG_HDR_GET_BUF_ID = 0x4A,
	IOC_IMG_CONFIG_HDR_STATISTICS = 0x4B,

	// HISO related, start from 0x60
	IOC_IMG_LISO_CFG_NI =0x60,
	IOC_IMG_FEED_RAW_NI =0x61,
	IOC_IMG_WAIT_RAW2ENC_NI =0x62,
	IOC_IMG_RESET_RAW2ENC_NI =0x63,

	//AAA control, start from 0x80
	IOC_IMG_GET_STATISTICS = 0x80,
	IOC_IMG_WAIT_ENTER_PREVIEW = 0x81,

	//Debug, start from 0xE0
	IOC_IMG_DUMP_IDSP_SEC = 0xE0,
};

#define	IAV_IOC_IMG_CONFIG_STATISTICS			IAVIMG_IOW(IOC_IMG_CONFIG_STATISTICS, aaa_statistics_setup_t)
#define	IAV_IOC_IMG_CONFIG_HISTOGRAM			IAVIMG_IOW(IOC_IMG_CONFIG_HISTOGRAM, aaa_histogram_t)
#define	IAV_IOC_IMG_GET_STATISTICS				IAVIMG_IOR(IOC_IMG_GET_STATISTICS, struct img_statistics)
#define	IAV_IOC_IMG_CONFIG_HDR_STATISTICS		IAVIMG_IOW(IOC_IMG_CONFIG_HDR_STATISTICS, vin_stats_setup_t)
#define	IAV_IOC_IMG_NOISE_FILTER_SETUP			IAVIMG_IOW(IOC_IMG_NOISE_FILTER_SETUP, u32)
#define	IAV_IOC_IMG_BLACK_LEVEL_GLOBAL_OFFSET	IAVIMG_IOW(IOC_IMG_BLACK_LEVEL_GLOBAL_OFFSET, black_level_global_offset_t)
#define	IAV_IOC_IMG_BAD_PIXEL_CORRECTION		IAVIMG_IOW(IOC_IMG_BAD_PIXEL_CORRECTION, bad_pixel_correct_setup_t)
#define	IAV_IOC_IMG_CFA_LEAKAGE_FILTER_SETUP	IAVIMG_IOW(IOC_IMG_CFA_LEAKAGE_FILTER_SETUP, cfa_domain_leakage_filter_t)
#define	IAV_IOC_IMG_CFA_NOISE_FILTER_SETUP		IAVIMG_IOW(IOC_IMG_CFA_NOISE_FILTER_SETUP, cfa_noise_filter_t)
#define	IAV_IOC_IMG_VIGNETTE_COMPENSATION		IAVIMG_IOW(IOC_IMG_VIGNETTE_COMPENSATION, vignette_compensation_t)
#define	IAV_IOC_IMG_LOCAL_EXPOSURE				IAVIMG_IOW(IOC_IMG_LOCAL_EXPOSURE, local_exposure_t)
#define	IAV_IOC_IMG_COLOR_CORRECTION			IAVIMG_IOW(IOC_IMG_COLOR_CORRECTION, color_correction_t)
#define	IAV_IOC_IMG_RGB_TO_YUV_SETUP			IAVIMG_IOW(IOC_IMG_RGB_TO_YUV_SETUP, rgb_to_yuv_setup_t)
#define	IAV_IOC_IMG_CHROMA_SCALE				IAVIMG_IOW(IOC_IMG_CHROMA_SCALE, chroma_scale_t)
#define	IAV_IOC_IMG_CHROMA_MEDIAN_FILTER_SETUP	IAVIMG_IOW(IOC_IMG_CHROMA_MEDIAN_FILTER_SETUP, chroma_median_filter_info_t)
#define	IAV_IOC_IMG_LUMA_SHARPENING				IAVIMG_IOW(IOC_IMG_LUMA_SHARPENING, luma_sharpening_t)
#define	IAV_IOC_IMG_LUMA_SHARPENING_FIR_CONFIG		IAVIMG_IOW(IOC_IMG_LUMA_SHARPENING_FIR_CONFIG, luma_sharpening_FIR_config_t)
#define	IAV_IOC_IMG_LUMA_SHARPENING_BLEND_CONFIG	IAVIMG_IOW(IOC_IMG_LUMA_SHARPENING_BLEND_CONFIG, luma_sharpening_blend_control_t)
#define	IAV_IOC_IMG_LUMA_SHARPENING_LEVEL_CONTROL	IAVIMG_IOW(IOC_IMG_LUMA_SHARPENING_LEVEL_CONTROL, luma_sharpening_level_control_t)
//#define	IAV_IOC_IMG_MCTF_GMV_SETUP				IAVIMG_IOW(IOC_IMG_MCTF_GMV_SETUP, mctf_gmv_setup_t)
#define	IAV_IOC_IMG_MCTF_MV_STABILIZER_SETUP	IAVIMG_IOW(IOC_IMG_MCTF_MV_STABILIZER_SETUP, mctf_mv_stab_setup_t)
#define	IAV_IOC_IMG_RGB_GAIN_ADJUST				IAVIMG_IOW(IOC_IMG_RGB_GAIN_ADJUST, rgb_gain_adjust_t)
#define	IAV_IOC_IMG_AAA_SETUP_EX				IAVIMG_IOW(IOC_IMG_AAA_SETUP_EX, struct aaa_statistics_ex)
#define	IAV_IOC_IMG_ANTI_ALIASING_CONFIG		IAVIMG_IOW(IOC_IMG_ANTI_ALIASING_CONFIG, anti_aliasing_filter_t)
#define	IAV_IOC_IMG_DIGITAL_SATURATION_LEVEL	IAVIMG_IOW(IOC_IMG_DIGITAL_SATURATION_LEVEL, digital_gain_level_t)
#define	IAV_IOC_IMG_DEMOSAIC_CONIFG				IAVIMG_IOW(IOC_IMG_DEMOSAIC_CONIFG, demoasic_filter_t)
#define	IAV_IOC_IMG_SENSOR_CONFIG				IAVIMG_IOW(IOC_IMG_SENSOR_CONFIG, sensor_input_setup_t)
#define	IAV_IOC_IMG_STATIC_BAD_PIXEL_CORRECTION		IAVIMG_IOW(IOC_IMG_STATIC_BAD_PIXEL_CORRECTION, fixed_pattern_noise_correct_t)
#define	IAV_IOC_IMG_CONFIG_FLOAT_STATISTICS		IAVIMG_IOW(IOC_IMG_CONFIG_FLOAT_STATISTICS, aaa_floating_tile_config_t)
#define	IAV_IOC_IMG_CHROMA_NOISE_FILTER_SETUP		IAVIMG_IOW(IOC_IMG_CHROMA_NOISE_FILTER_SETUP, chroma_noise_filter_t)
#define	IAV_IOC_IMG_LUMA_SHARPENING_LNL			IAVIMG_IOW(IOC_IMG_LUMA_SHARPENING_LNL, luma_sharpening_LNL_t)
#define	IAV_IOC_IMG_STRONG_GRGB_MISMATCH		IAVIMG_IOW(IOC_IMG_STRONG_GRGB_MISMATCH, strong_grgb_mismatch_filter_t)
#define	IAV_IOC_IMG_LUMA_SHARPENING_TONE		IAVIMG_IOW(IOC_IMG_LUMA_SHARPENING_TONE, luma_sharpening_tone_control_t)

#define	IAV_IOC_IMG_WAIT_ENTER_PREVIEW			IAVIMG_IO(IOC_IMG_WAIT_ENTER_PREVIEW)

#define	IAV_IOC_IMG_DUMP_IDSP_SEC				IAVIMG_IOWR(IOC_IMG_DUMP_IDSP_SEC, idsp_config_info_t)

#define	IAV_IOC_IMG_HDR_SET_VIDEO_PROC    			IAVIMG_IOW(IOC_IMG_HDR_SET_VIDEO_PROC, ipcam_set_hdr_proc_control_t)
#define	IAV_IOC_IMG_HDR_SET_BUF_ID					IAVIMG_IOW(IOC_IMG_HDR_SET_BUF_ID, u8*)
#define	IAV_IOC_IMG_HDR_GET_BUF_ID					IAVIMG_IOR(IOC_IMG_HDR_GET_BUF_ID, u8*)

#define	IAV_IOC_IMG_SET_LISO_CFG_NI                            	IAVIMG_IOW(IOC_IMG_LISO_CFG_NI, u8*)
#define	IAV_IOC_IMG_UPDATE_LISO_CFG_NI				IAVIMG_IOW(IOC_IMG_LISO_CFG_NI, struct idsp_hiso_config_update_s)
#define	IAV_IOC_IMG_FEED_RAW_NI                                	IAVIMG_IOW(IOC_IMG_FEED_RAW_NI, struct raw2enc_raw_feed_info*)
#define	IAV_IOC_IMG_WAIT_RAW2ENC_NI                            	IAVIMG_IO(IOC_IMG_WAIT_RAW2ENC_NI)
#define	IAV_IOC_IMG_RESET_RAW2ENC_NI                           	IAVIMG_IO(IOC_IMG_RESET_RAW2ENC_NI)


#endif

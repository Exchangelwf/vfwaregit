/*
 * Spot.h
 *
 *  Created on: 2010-5-2
 *      Author: Administrator
 */

#ifndef SPOT_H_
#define SPOT_H_

#include "Video.h"

#ifdef __cplusplus
extern "C" {
#endif

int Spot_Create( );

int Spot_Destory( );

int Spot_SplitCombin(uint dwMode, uint dwIndex);

int Spot_SplitGetRect(int channel, VIDEO_RECT * pRect);

int Spot_SplitGetFullRect(VIDEO_RECT * pRect);

#ifdef __cplusplus
}
#endif
#endif /* SPOT_H_ */

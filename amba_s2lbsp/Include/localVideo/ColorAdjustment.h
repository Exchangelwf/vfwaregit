#ifndef __COLORADJUSTMENT_H__
#define __COLORADJUSTMENT_H__

void aaa_start();
void aaa_stop();
int DayNightMode(int mode);
void BrightnessAdjustment(int brightness);  //亮度(range -255 ~ 255)
void ContrastAdjustment(int contrast);    //对比度(range 0 ~ 128)
void SaturationAdjustment(int saturation);  //饱和度(range 0 ~ 255)
void HueAdjustment(int hue); //色度(range -15 ~ +15)
void SharpnessAdjustment(int sharpness);  //锐度(range 0 ~ 255)
void GainAdjustment(int gain);     //增益
void Set_anti_flicker_mode(int antiflicker);//防闪烁

#endif

#ifndef __COLORADJUSTMENT_H__
#define __COLORADJUSTMENT_H__

void aaa_start();
void aaa_stop();
int DayNightMode(int mode);
void BrightnessAdjustment(int brightness);  //����(range -255 ~ 255)
void ContrastAdjustment(int contrast);    //�Աȶ�(range 0 ~ 128)
void SaturationAdjustment(int saturation);  //���Ͷ�(range 0 ~ 255)
void HueAdjustment(int hue); //ɫ��(range -15 ~ +15)
void SharpnessAdjustment(int sharpness);  //���(range 0 ~ 255)
void GainAdjustment(int gain);     //����
void Set_anti_flicker_mode(int antiflicker);//����˸

#endif

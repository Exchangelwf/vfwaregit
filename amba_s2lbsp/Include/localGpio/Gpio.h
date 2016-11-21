#ifndef			__GPIO__H
#define			__GPIO__H

#if 0
#define GPIO_LDR			33
#define GPIO_IRCUT			38

#define  GPIO_ALARM_IN		65
#define  GPIO_ALARM_OUT		66
#define  GPIO_LIGHT_DET		33
#define  GPIO_IRCUT_SW		67
#define  GPIO_SEN_RST		54
#define  GPIO_PHY_RST		37
#define GPIO_WORK_LED		113
#endif

#if 0
#define GPIO_LIGHT_DET		0
#define GPIO_PHY_RST		1
#define GPIO_SEN_RST		2
#define GPIO_ALARM_IN		3
#define GPIO_ALARM_OUT		4
#define GPIO_IRCUT_SW		5
#define GPIO_WORK_LED		6
#else
#define GPIO_LIGHT_DET		0
#define GPIO_SYS_RST		1
#define GPIO_PHY_RST		2
#define GPIO_SEN_RST		3
#define GPIO_ALARM_IN1		4
#define GPIO_ALARM_IN2		5
#define GPIO_ALARM_OUT		6
#define GPIO_IR_SW			7
#define GPIO_IRCUT_SW		8
#define GPIO_PWM_IRIS		9
#define GPIO_SPI0_CSN0  	10
#define GPIO_WORK_LED		11
#define GPIO_RS485			12

#endif

extern int ConvertGpio(unsigned char Gpiofun);

extern int gpio_init(void);

extern int gpio_set0(int gpio_id);

extern int gpio_set1(int gpio_id);

extern int gpio_read(int gpio_id);

extern char *GetVersioninfo();


#endif

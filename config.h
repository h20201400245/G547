#ifndef CHAR_CONFIG_H
#define CHAR_CONFIG_H

#include <linux/ioctl.h>

#define MAGIC_NUM 158

//IOCTL interface prototypes

#define LED1_STATUS _IOWR(MAGIC_NUM, 0, int16_t *)
#define LED2_INPUT _IOWR(MAGIC_NUM, 1, int16_t *)
#define LED2_USER_INPUT _IOWR(MAGIC_NUM, 2, int16_t *)

//Device file interface
#define DEVICE_FILE_NAME "/dev/led_Device"

#endif

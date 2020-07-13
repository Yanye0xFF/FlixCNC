#ifndef _USER_MAIN_H_
#define _USER_MAIN_H_

/* stdlibs */
#include "stdint.h"
#include "stdlib.h"

/* drivers */
#include "hw_control_AM335x.h"
#include "soc_AM335x.h"
#include "interrupt.h"
#include "cache.h"
#include "antminer.h"
#include "gpio_v2.h"
#include "delay.h"
#include "hw_cm_per.h"
#include "hw_types.h"
#include "consoleUtils.h"
#include "pin_mux.h"
#include "i2c_port2.h"
#include "network.h"
#include "sdcard_conf.h"
#include "int_priority.h"
#include "mcspi_port0.h"
#include "mcspi_port1.h"
#include "mmu_conf.h"

/* FlixCNC part */
#include "interpreter.h"
#include "stepper.h"
#include "system.h"
#include "spindle.h"

#define SYSTEM_LED_PIN_ADDR    GPIO_0_20
#define SYSTEM_LED_PIN    20


/* GUI part */
#define FRAME_RATE_DEFAULT    25

#include "text.h"
#include "frame.h"
#include "view.h"
#include "textview.h"
#include "progressbar.h"
#include "imageview.h"
#include "context.h"
#include "boot_view.h"
#include "conn_view.h"
#include "app_view.h"

/* NetWork part */
#define COMMEN_GROUP_END    0x0F
#define PARAMS_GROUP_START    0x10
#define PARAMS_GROUP_END    0x2F
#define MOTION_GROUP_START    0x40
#define MOTION_GROUP_END    0x4F
#define FILE_GROUP_START    0x50
#define FILE_GROUP_END    0x5F

#include "comm_group.h"
#include "params_group.h"
#include "motion_group.h"
#include <file_group.h>
#include "file_group.h"
#include "bytestream.h"


#endif

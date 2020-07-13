/*
 * motion_group.h
 * @brief 
 * Created on: Jun 16, 2020
 * Author: Yanye
 */

#ifndef _MOTION_GROUP_H_
#define _MOTION_GROUP_H_

#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "hw_types.h"

#include "network.h"
#include "spindle.h"
#include "stepper.h"

#define GO_PHYHOME_CMD    0x40
#define GO_WORKHOME_CMD   0x41
#define JOGMOTION_CMD   0x42
#define LONGMOTION_START_CMD   0x43
#define LONGMOTION_END_CMD   0x44

// 稍做修改防止和spindle.h冲突
#define SPINDLE_ON_CMD     0x45
#define SPINDLE_OFF_CMD    0x46
#define TONE_MAKE_CMD      0x47
#define LED_CONTROL_CMD    0x48

void processMotionCmd(uint8_t *buffer);

#endif

/*
 * params_group.h
 * @brief 
 * Created on: Jun 16, 2020
 * Author: Yanye
 */

#ifndef _PARAMS_GROUP_H_
#define _PARAMS_GROUP_H_

#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "hw_types.h"

#include "network.h"
#include "spindle.h"
#include "stepper.h"

// 雕刻机部分
#define SET_WORKHOME_CMD            0x10
#define SET_CURRENT_WORKHOME_CMD    0x11
#define SET_CALLBACK_RATE_CMD       0x12

#define SET_MACHINE_HOMING_SPEED_CMD       0x13

#define SET_JOG_MOTION_SPEED_CMD    0x14
#define SET_LONG_MOTION_SPEED_CMD   0x15

// 主轴部分
#define SPINDLE_PARAM_ALL_CMD    0x16
#define SPINDLE_DUTY_CMD    0x17
#define SPINDLE_VSSTIME_CMD    0x18
#define SPINDLE_DIR_CMD    0x19
#define SPINDLE_TONETICK_CMD    0x1A
#define SPINDLE_SPEEDUP_CMD    0x1B
#define SPINDLE_SLOWDOWN_CMD    0x1C

void processParamCmd(uint8_t *package);

#endif

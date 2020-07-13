/*
 * comm_group.h
 * @brief 
 * Created on: Jun 15, 2020
 * Author: Yanye
 */

#ifndef _COMM_GROUP_H_
#define _COMM_GROUP_H_

#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "hw_types.h"

#include "network.h"
#include "spindle.h"
#include "stepper.h"
#include "config.h"

#include "text.h"
#include "frame.h"
#include "view.h"
#include "textview.h"
#include "app_view.h"
#include "conn_view.h"

#define CONNECT_CMD        0x00
#define DISCONNECT_CMD     0x01
#define QUERY_MACHINE_STATE_CMD    0x02
#define QUERY_SPINDLE_STATE_CMD    0x03

#define HARDWARE_VERSION   0x1
#define SOFTWARE_VERSION   0x2

void processCommCmd(uint8_t *package, Frame *frame);


#endif

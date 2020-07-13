/*
 * file_group.h
 * @brief 
 * Created on: Jun 19, 2020
 * Author: Yanye
 */

#ifndef _TRANS_GROUP_H_
#define _TRANS_GROUP_H_

#include "stdint.h"
#include "string.h"
#include "bytestream.h"
#include "network.h"
#include "crc16.h"

#include "interpreter.h"
#include "stepper.h"

#include "consoleUtils.h"

#define FILEHEADER_CMD    0x50
#define FILEFILL_CMD      0x51

#define PARSE_GCODE_CMD    0x52
#define RUN_GCODE_CMD    0x53

void processTransCmd(uint8_t *package, ByteStream *stream);

#endif

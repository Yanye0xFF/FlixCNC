/*
 * FileName: color.h
 * @brief 
 * Created on: Jun 1, 2020
 * Author: Yanye
 */

#ifndef _COLOR_H_
#define _COLOR_H_

#include "stdint.h"

typedef struct _color {
	uint16_t RED;
	uint16_t BLUE;
	uint16_t GREEN;
	uint16_t WHITE;
	uint16_t BLACK;
	uint16_t GRAY;
	uint16_t YELLOW;
	uint16_t SILVER;
	uint16_t (*parseColor)(uint32_t rgb);
} Color;

extern const Color COLOR;

#endif

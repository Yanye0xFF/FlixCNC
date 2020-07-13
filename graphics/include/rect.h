/*
 * FileName: rect.h
 * @brief 视图元素尺寸信息
 * Created on: Jun 1, 2020
 * Author: Yanye
 */

#ifndef _RECT_H_
#define _RECT_H_

#include "stdint.h"

typedef struct _rect {
   int16_t xStart;
   int16_t yStart;
   int16_t xEnd;
   int16_t yEnd;
} Rect;

#endif



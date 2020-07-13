/*
 * FileName: progressbar.h
 * @brief 
 * Created on: Jun 3, 2020
 * Author: Yanye
 */

#ifndef _PROGRESSBAR_H_
#define _PROGRESSBAR_H_

#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"

#include <rect.h>
#include "color.h"
#include "displayio.h"

#define PROGRESSBAR_NORMAL    0
#define PROGRESSBAR_ROUND     1


struct _progressbar;
typedef struct _progressbar    ProgressBar;

struct _progressbar {
    Rect size;
    uint16_t borderColor;
    uint16_t fillColor;
    uint16_t emptyColor;
    uint8_t max;
    uint8_t progress;
    uint8_t style;
    uint8_t drawState;
    uint8_t lastValue;
    // 字节对齐,
    uint8_t :8;
    void (*onDraw)(ProgressBar *view);
    void (*onDestory)(ProgressBar *view);
    void (*onStateChanged)(uint8_t newState, ProgressBar *view);
    void (*setProgress)(uint8_t value, ProgressBar *view);
};

ProgressBar *newProgressBar(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height);

#endif

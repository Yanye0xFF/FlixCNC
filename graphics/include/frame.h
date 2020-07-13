/*
 * FileName: frame.h
 * @brief 
 * Created on: Jun 1, 2020
 * Author: Yanye
 */

#ifndef _FRAME_H_
#define _FRAME_H_

#include "stdint.h"
#include "stdlib.h"
#include "hw_types.h"

#include <internal_font.h>


#include "color.h"
#include <view.h>
#include "displayio.h"
#include "textview.h"
#include "progressbar.h"
#include "imageview.h"


#define TITLE_ALIGN_START   0
#define TITLE_ALIGN_CENTER  1
#define TITLE_ALIGN_END     2

// framelayout
struct _frame;
typedef struct _frame   Frame;

struct _frame {
    Rect rect;
	View* viewGroup;
	uint16_t foregroundColor;
	uint16_t backgroundColor;
	uint16_t borderColor;
    uint8_t maxViews;
	uint8_t drawState;
	void (*invalidate)(Frame *frame);
	Boolean (*addView)(Frame *frame, ViewType type, uint8_t id, void *entity);
	View* (*findViewById)(Frame *frame, ViewType type, uint8_t id);
	void (*removeView)(Frame *frame, ViewType type, uint8_t id);
	void (*removeAllView)(Frame *frame);
	void (*updateViewState)(Frame *frame, ViewType type, uint8_t id, uint8_t newState);
};

Frame *newFrame(uint8_t maxViews, Rect rect);
void destoryFrame(Frame *frame);

#endif

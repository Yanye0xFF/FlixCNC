/*
 * FileName: textview.h
 * @brief 
 * Created on: Jun 1, 2020
 * Author: Yanye
 */

#ifndef _TEXTVIEW_H_
#define _TEXTVIEW_H_

#include "stdint.h"
#include "string.h"
#include "hw_types.h"

#include <internal_font.h>
#include <rect.h>
#include <size.h>
#include "font.h"
#include "color.h"
#include "displayio.h"
#include "consoleUtils.h"
#include "sdcard_conf.h"

#define ALIGN_H_START                                 (1<<0)
#define ALIGN_H_CENTER                                (1<<1)
#define ALIGN_H_END                                   (1<<2)
#define ALIGN_V_TOP                                   (1<<3)
#define ALIGN_V_CENTER                                (1<<4)
#define ALIGN_V_BOTTOM                                (1<<5)

#define TEXTVIEW_RECT_FILL          0
#define TEXTVIEW_ROUNDRECT_FILL     1
#define TEXTVIEW_BOX                2
#define TEXTVIEW_ROUNDD_BOX         3

#define ENCODE_GB2312           0
#define ENCODE_ASCII            1

#define WCHAR_SIZE    2
#define CHAR_SIZE     1

struct _textview;
typedef struct _textview    TextView;

struct _textview {
    uint8_t* text;
    Font *font;
	Rect rect;
	uint16_t foregroundColor;
	uint16_t backgroundColor;
    uint16_t borderColor;
	uint8_t encodeType;
	int8_t horSpace;
	int8_t verSpace;
	uint8_t style;
	uint8_t textAlign;
	uint8_t drawState;
	// 当text为malloc创建，将recycleText置1，textview在onDestory时将会回收text
	uint8_t recycleText;
	// 当textview占用空间固定，即保证不触发背景重绘可置1，下次绘制时仅绘制字体而不绘制背景降低闪烁
	uint8_t quickDraw;
	// placeHolder = 1 && text = NULL时，textview可当占位区，用作矩形/圆角矩形/填充/边框
	uint16_t placeHolder;
	void (*setText)(uint8_t *text, TextView *view);
	void (*setSize)(Rect rect, TextView *view);
	void (*onDraw)(TextView *view);
	void (*onDestory)(TextView *view);
	void (*onStateChanged)(uint8_t newState, TextView *view);
};

TextView *newTextView(uint8_t *str, uint8_t encode, uint16_t xStart, uint16_t yStart);

#endif

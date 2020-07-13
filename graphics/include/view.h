/*
 * FileName: view.h
 * @brief 父类View，其下实现TextView，ImageView等
 * Created on: Jun 1, 2020
 * Author: Yanye
 */

#ifndef _VIEW_H_
#define _VIEW_H_

#include <rect.h>
#include "stdint.h"
#include "frame.h"

#define VIEW_USABLE_MASK        (0x1)
#define VIEW_USABLE_SHIFT        0
#define VIEW_FREE                0
#define VIEW_OCCUPY              1

#define VIEW_VISIBILITY_MASK    (1<<1)
#define VIEW_VISIBILITY_SHIFT    1
#define VIEW_INVISIBLE           0
#define VIEW_VISIBLE             1


#define VIEW_ONLY_OCCUPY                0x1
#define VIEW_OCCUPY_AND_VISIBLE         0x3

typedef enum _viewtype {
    TEXTVIEW = 0,
    IMAGEVIEW,
    PROGRESSBAR
} ViewType;

typedef struct _view    View;

struct _view {
	uint8_t state;
	uint8_t type;
	uint8_t id;
    uint8_t :8;
    void* entity;
};

#endif

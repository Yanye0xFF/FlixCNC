/*
 * FileName: internal_font.h
 * @brief 字体管理
 * Created on: May 31, 2020
 * Author: Yanye
 */

#ifndef _FONTMANAGER_H_
#define _FONTMANAGER_H_

#include "stdint.h"
#include "hw_types.h"

#include <font.h>
#include "minifont.h"

typedef struct _internal_font {
	Font miniFont;
	Font miniIcon;
} InternalFont;

extern const InternalFont INTERNAL_FONT;

#endif

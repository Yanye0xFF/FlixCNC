/*
 * app_view.h
 * @brief 
 * Created on: Jun 9, 2020
 * Author: Yanye
 */

#ifndef _APP_VIEW_H_
#define _APP_VIEW_H_

#include "stdint.h"
#include "text.h"
#include "frame.h"
#include "view.h"
#include "textview.h"
#include "progressbar.h"
#include "imageview.h"

#define HOST_TYPE_WINDOWS    0
#define HOST_TYPE_ANDROID    1

void createAppView(uint8_t hostType, Frame *frame);


#endif

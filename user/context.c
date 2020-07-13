/*
 * context.c
 * @brief 
 * Created on: Jun 25, 2020
 * Author: Yanye
 */

#include "context.h"

static Context *context;

static Frame *prepareFrame();

Context *contextInstance() {
    if(NULL == context) {
        context = (Context *)malloc(sizeof(Context));
        memset((uint8_t *)context, 0x00, sizeof(Context));
        context->prepareFrame = prepareFrame;
    }
    return context;
}

static Frame *prepareFrame() {
    if(NULL == context->frame) {
        context->frame = newFrame(16, (Rect){0, 0, 127, 127});
    }
    return context->frame;
}



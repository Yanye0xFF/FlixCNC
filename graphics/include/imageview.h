/*
 * FileName: imageview.h
 * @brief 
 * Created on: Jun 3, 2020
 * Author: Yanye
 */

#ifndef _IMAGEVIEW_H_
#define _IMAGEVIEW_H_

#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "displayio.h"

#include "hw_types.h"

#include "consoleUtils.h"
#include "sdcard_conf.h"

#define BITMAP_DIB          0x4D42
#define BI_RGB              0
#define TRUE_COLOR_24BIT    0x18

struct _imageview;
typedef struct _imageview    ImageView;

struct _imageview {
    uint16_t x0;
    uint16_t y0;
    uint16_t width;
    uint16_t height;
    uint16_t dataOffset;
    uint16_t drawState;
    uint8_t *imageFile;
    char *fileName;
    void (*onDraw)(ImageView *view);
    void (*onDestory)(ImageView *view);
    void (*onStateChanged)(uint8_t newState, ImageView *view);
    void (*setImageFile)(char *fileName, ImageView *view);
};

ImageView *newImageView(uint16_t x0, uint16_t y0, char *fileName);


#endif

/*
 * FileName: imageview.c
 * @brief 
 * Created on: Jun 3, 2020
 * Author: Yanye
 */

#include "imageview.h"

static uint32_t getBmpWidth(uint8_t* bmpfile);
static uint32_t getBmpHeight(uint8_t* bmpfile);
static uint32_t getDataOffset(uint8_t * bmpfile);

static void setImageFile(char *fileName, ImageView *view);

static void onDraw(ImageView *view);
static void onDestory(ImageView *view);
static void onStateChanged(uint8_t newState, ImageView *view);

ImageView *newImageView(uint16_t x0, uint16_t y0, char *fileName) {

    ImageView *view = (ImageView *)malloc(sizeof(ImageView));
    memset((uint8_t *)view, 0x00, sizeof(ImageView));

    view->x0 = x0;
    view->y0 = y0;
    view->fileName = fileName;
    view->onDraw = onDraw;
    view->onDestory = onDestory;
    view->onStateChanged = onStateChanged;
    view->setImageFile = setImageFile;
    return view;
}

static void setImageFile(char *fileName, ImageView *view) {
    if(view->imageFile != NULL) {
        free(view->imageFile);
        view->imageFile = NULL;
    }
    view->fileName = fileName;
    view->drawState = 0x00;
}

static void onDestory(ImageView *view) {
    if(view->imageFile != NULL) {
        free(view->imageFile);
        view->imageFile = NULL;
        ConsoleUtilsPrintf("free(view->imageFile)\n");
    }
    ConsoleUtilsPrintf("ImageView onDestory\n");
}

static void onStateChanged(uint8_t newState, ImageView *view) {
    view->drawState = newState;
}

static void onDraw(ImageView *view) {
    // 已绘制过的Image不再重复绘制
    if(view->drawState & 0x1) {
        return;
    }
    // 加载文件
    if(view->imageFile == NULL) {
        view->imageFile = loadFile(view->fileName);
    }
    if(view->imageFile == NULL) {
        return;
    }
    uint32_t magic = 0;
    memcpy((uint8_t *)&magic, (view->imageFile + 0), 2);
    // 只支持Windows 3.1x, 95, NT... DIB(device-independent bitmap)格式
    if(BITMAP_DIB != magic) {
        return;
    }
    // biBitCount 只支持24位图
    memcpy((uint8_t *)&magic, (view->imageFile + 28), 2);
    if(TRUE_COLOR_24BIT != magic) {
        return;
    }
    // 只支持BI_RGB无压缩类型
    memcpy((uint8_t *)&magic, (view->imageFile + 30), 4);
    if(BI_RGB != magic) {
        return;
    }
    view->drawState = 0x1;
    view->width = getBmpWidth(view->imageFile);
    view->height = getBmpHeight(view->imageFile);
    view->dataOffset = getDataOffset(view->imageFile);

    uint16_t *buffer = (uint16_t *)malloc(sizeof(uint16_t) * view->width * view->height);
    uint8_t *data = (view->imageFile + view->dataOffset);
    uint16_t j, color;
    uint8_t red, green, blue;
    for(uint16_t i = 0; i < view->height; i++ ) {
        // magic复用做显示屏缓冲区游标cursor
        // windows bmp扫描方式是按从左到右、从下到上
        magic = view->width * (view->height - i - 1);
        for(j = 0; j < view->width; j++ ) {
            // 24bit BGR888
            blue = (*data) >> 3;
            green = (*(data + 1)) >> 2;
            red = (*(data + 2)) >> 3;
            data += 3;
            // 16bit RGB565
            color = (red << 11) + (green << 5) + (blue << 0);
            *(buffer + magic) = color;
            magic++;
        }
        // 4字节对齐
        for(; (j % 4) != 0; j++) {
            data += 3;
        }
    }
    GuiFillData(view->x0, view->y0, (view->x0 + view->width - 1),  (view->y0 + view->height - 1), buffer);
    free(buffer);
}


static uint32_t getBmpWidth(uint8_t* bmpfile) {
    uint32_t width;
    memcpy((uint8_t *)&width, (bmpfile + 18), 4);
    return width;
}

static uint32_t getBmpHeight(uint8_t* bmpfile ) {
    uint32_t height;
    memcpy((uint8_t *)&height, (bmpfile + 22), 4);
    return height;
}

static uint32_t getDataOffset(uint8_t * bmpfile) {
    uint32_t offset;
    memcpy((uint8_t *)&offset, (bmpfile + 10), 4);
    return offset;
}

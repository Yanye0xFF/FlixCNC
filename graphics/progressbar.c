/*
 * FileName: ProgressBar.c
 * @brief 
 * Created on: Jun 3, 2020
 * Author: Yanye
 */

#include "progressbar.h"

static void setProgress(uint8_t value, ProgressBar *view);

static void onDraw(ProgressBar *view);
static void onDestory(ProgressBar *view);
static void onStateChanged(uint8_t newState, ProgressBar *view);

ProgressBar *newProgressBar(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height) {

    ProgressBar *bar = (ProgressBar *)malloc(sizeof(ProgressBar));
    memset((uint8_t *)bar, 0x00, sizeof(ProgressBar));

    bar->size.xStart = x0;
    bar->size.yStart = y0;
    bar->size.xEnd = (x0 +  width - 1);
    bar->size.yEnd = (y0 + height - 1);
    bar->borderColor = COLOR.BLUE;
    bar->fillColor = COLOR.parseColor(0xE35C51);
    bar->emptyColor = COLOR.GRAY;
    bar->onDraw = onDraw;
    bar->onDestory = onDestory;
    bar->onStateChanged = onStateChanged;
    bar->setProgress = setProgress;
    return bar;
}

static void onDestory(ProgressBar *view) {
    ConsoleUtilsPrintf("ProgressBar onDestory\n");
}

static void onStateChanged(uint8_t newState, ProgressBar *view) {
    view->drawState = newState;
}

static void onDraw(ProgressBar *view) {
    Rect rect = view->size;
    uint16_t width = abs(rect.xEnd - rect.xStart + 1);
    uint16_t height = abs(rect.yEnd - rect.yStart + 1);
    float segment = height / 10.0;
    uint16_t circle = (uint16_t)(segment * 2.0);

    if((view->drawState & 0x1) == 0) {
        if(PROGRESSBAR_ROUND == view->style) {
            GuiFillRoundRect(rect.xStart, rect.yStart, rect.xEnd, rect.yEnd, circle, view->borderColor);
            GuiFillRoundRect(rect.xStart + circle, rect.yStart + circle, rect.xEnd - circle, rect.yEnd - circle, circle, view->emptyColor);
        }else {
            for(uint16_t i = 0; i < circle; i++) {
                GuiDrawRect(rect.xStart + i, rect.yStart + i, rect.xEnd - i, rect.yEnd - i, view->borderColor);
                GuiFillColor(rect.xStart + circle, rect.yStart + circle, rect.xEnd - circle, rect.yEnd - circle, view->emptyColor);
            }
        }
    }
    // 进度条没变化，不执行绘制
    if((view->drawState & 0x2)) {
        return;
    }

    if(view->progress < view->lastValue) {
        if(PROGRESSBAR_ROUND == view->style) {
            GuiFillRoundRect(rect.xStart + circle, rect.yStart + circle, rect.xEnd - circle, rect.yEnd - circle, circle, view->emptyColor);
        }else {
            GuiFillColor(rect.xStart + circle, rect.yStart + circle, rect.xEnd - circle, rect.yEnd - circle, view->emptyColor);
        }
    }

    view->drawState = 0x3;

    if(view->progress == 0) {
        return;
    }else if(view->progress >= view->max) {
        if(PROGRESSBAR_ROUND == view->style) {
            GuiFillRoundRect(rect.xStart + circle, rect.yStart + circle, rect.xEnd - circle, rect.yEnd - circle, circle, view->fillColor);
        }else {
            GuiFillColor(rect.xStart + circle, rect.yStart + circle, rect.xEnd - circle, rect.yEnd - circle, view->fillColor);
        }
        return;
    }

    // TODO fix some bug in PROGRESSBAR_ROUND style

    segment = view->progress * (width - (circle << 1)) / view->max * 1.0;
    uint16_t x1 = rect.xStart + circle + (uint16_t)segment - 1;
    uint16_t y1 = rect.yEnd - circle;


    if(PROGRESSBAR_ROUND == view->style) {
        GuiFillRoundRect(rect.xStart + circle, rect.yStart + circle, x1, y1, circle, view->fillColor);
    }else {
        GuiFillColor(rect.xStart + circle, rect.yStart + circle, x1, y1, view->fillColor);
    }

}

static void setProgress(uint8_t value, ProgressBar *view) {
    view->lastValue = view->progress;
    view->progress = value;
    // 清除进度条进度绘制标记位
    view->drawState &= 0x01;
}


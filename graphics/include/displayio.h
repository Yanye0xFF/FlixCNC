/*
 * FileName: displayio.h
 * @brief 
 * Created on: Jun 1, 2020
 * Author: Yanye
 */

#ifndef _DISPLAYIO_H_
#define _DISPLAYIO_H_

#include "stdint.h"
#include "font.h"
#include "string.h"

#include "consoleUtils.h"

#include "st7735s.h"
/**
 * @brief 底层绘制api，调用屏幕驱动
 * */
void GuiDrawPixel(uint16_t x, uint16_t y, uint16_t color);
void GuiFillData(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t *data);
void GuiFillColor(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t color);

/**
 * @brief 高级图形api
 * */
void GuiDrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t c );
void GuiDrawRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t c );

void GuiDrawArc(int16_t x0, int16_t y0, int16_t r, uint8_t s, uint16_t color);
void GuiDrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t c);
void GuiFillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t c);

void GuiDrawRoundRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t r, uint16_t c);
void GuiFillRoundRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t r, uint16_t c);
void GuiDrawMesh(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t gridSize, uint16_t c);

void GuiDrawChar(wchar ch, uint16_t x, uint16_t y, uint16_t foregroundColor, uint16_t backgroundColor, Font *font) ;

#endif

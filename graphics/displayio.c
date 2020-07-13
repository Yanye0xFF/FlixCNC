/*
 * FileName: displayio.c
 * @brief 
 * Created on: Jun 1, 2020
 * Author: Yanye
 */

#include "displayio.h"

/**
 * @breif 在屏幕上绘制一像素
 * @param x x坐标
 * @param y y坐标
 * @param color RGB565颜色
 * */
void GuiDrawPixel(uint16_t x, uint16_t y, uint16_t color) {
	St7735sSetPixel((uint8_t)x, (uint8_t)y, color);
}

/**
 * @breif 使用data数据源填充屏幕指定区域
 * @param xStart x左上坐标
 * @param yStart y左上坐标
 * @param xEnd y右下坐标
 * @param yEnd y右下坐标
 * @param data 数据源指针
 * */
void GuiFillData(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t *data) {
	St7735sSetRegion((uint8_t)xStart, (uint8_t)yStart, (uint8_t)xEnd, (uint8_t)yEnd);
	St7735sFillData(data);
}

/**
 * @breif 使用特定颜色填充屏幕指定区域
 * @param xStart x左上坐标
 * @param yStart y左上坐标
 * @param xEnd y右下坐标
 * @param yEnd y右下坐标
 * @param color RGB565颜色
 * */
void GuiFillColor(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t color) {
	St7735sSetRegion((uint8_t)xStart, (uint8_t)yStart, (uint8_t)xEnd, (uint8_t)yEnd);
	St7735sFillColor(color);
}

/**
 * @brief 绘制直线段，水平/垂直线将被优化直接绘制，其他带斜率的线段使用bresenham算法绘制
 * @brief 两点距离8 直线xy = 6 节点
 * */
void GuiDrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color ) {

    if((x1 == x2) || (y1 == y2)) {
        GuiFillColor(x1, y1, x2, y2, color);
        return;
    }

    int16_t dx = x2 - x1;
    int16_t dy = y2 - y1;
    int16_t dxabs = (dx > 0) ? dx : -dx;
    int16_t dyabs = (dy > 0) ? dy : -dy;
    int16_t sgndx = (dx > 0) ? 1  : -1;
    int16_t sgndy = (dy > 0) ? 1 : -1;
    int16_t x = dyabs >> 1;
    int16_t y = dxabs >> 1;
    int16_t drawx = x1;
    int16_t drawy = y1;

    GuiDrawPixel(drawx, drawy, color);

    if(dxabs >= dyabs) {
        for(int16_t n = 0; n < dxabs; n++) {
            y += dyabs;
            if(y >= dxabs) {
                y -= dxabs;
                drawy += sgndy;
            }
            drawx += sgndx;
            GuiDrawPixel(drawx, drawy, color);
        }
    }else {
        for(int16_t n = 0; n < dyabs; n++) {
            x += dxabs;
            if(x >= dyabs) {
                x -= dyabs;
                drawx += sgndx;
            }
            drawy += sgndy;
            GuiDrawPixel(drawx, drawy, color);
        }
    }
}

/**
 * @brief 绘制矩形
 * */
void GuiDrawRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t c) {
	uint16_t temp;
	if(x1 > x2) {
	    temp = x1;
		x1 = x2;
		x2 = temp;
	}
	if(y1 > y2) {
	    temp = y1;
		y1 = y2;
		y2 = temp;
	}

    GuiFillColor(x1, y1, x2, y1, c);
    GuiFillColor(x1, y2, x2, y2, c);

    GuiFillColor(x1, y1, x1, y2, c);
    GuiFillColor(x2, y1, x2, y2, c);
}

/**
 * @brief 绘制圆弧
 * @param x0 圆心x
 * @param y0 圆心y
 * @param r 圆弧半径
 * @param s 弧长
 * */
void GuiDrawArc(int16_t x0, int16_t y0, int16_t r, uint8_t s, uint16_t color) {

    if ( x0 < 0 ) return;
    if ( y0 < 0 ) return;
    if ( r <= 0 ) return;

    int16_t xd = 1 - (r << 1);
    int16_t yd = 0, e = 0, x = r,  y = 0;

    while ( x >= y ) {
        // Q1
        if ( s & 0x01 ) GuiDrawPixel(x0 + x, y0 - y, color);
        if ( s & 0x02 ) GuiDrawPixel(x0 + y, y0 - x, color);
        // Q2
        if ( s & 0x04 ) GuiDrawPixel(x0 - y, y0 - x, color);
        if ( s & 0x08 ) GuiDrawPixel(x0 - x, y0 - y, color);
        // Q3
        if ( s & 0x10 ) GuiDrawPixel(x0 - x, y0 + y, color);
        if ( s & 0x20 ) GuiDrawPixel(x0 - y, y0 + x, color);
        // Q4
        if ( s & 0x40 ) GuiDrawPixel(x0 + y, y0 + x, color);
        if ( s & 0x80 ) GuiDrawPixel(x0 + x, y0 + y, color);
        y++;
        e += yd;
        yd += 2;
        if(((e << 1) + xd) > 0 ) {
            x--;
            e += xd;
            xd += 2;
        }
    }
}

/**
 * @brief 绘制圆
 * @param x0 圆心x
 * @param y0 圆心y
 * */
void GuiDrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t c) {
    if (x0 < 0) return;
    if (y0 < 0) return;
    if (r <= 0) return;

    int16_t xd = 1 - (r << 1);
    int16_t yd = 0, e = 0, x = r, y = 0;

    while (x >= y) {
        GuiDrawPixel(x0 - x, y0 + y, c);
        GuiDrawPixel(x0 - x, y0 - y, c);
        GuiDrawPixel(x0 + x, y0 + y, c);
        GuiDrawPixel(x0 + x, y0 - y, c);
        GuiDrawPixel(x0 - y, y0 + x, c);
        GuiDrawPixel(x0 - y, y0 - x, c);
        GuiDrawPixel(x0 + y, y0 + x, c);
        GuiDrawPixel(x0 + y, y0 - x, c);
        y++;
        e += yd;
        yd += 2;
        if (((e << 1) + xd) > 0) {
            x--;
            e += xd;
            xd += 2;
        }
    }
}

void GuiFillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t c) {

    if ( x0 < 0 ) return;
    if ( y0 < 0 ) return;
    if ( r <= 0 ) return;

    int16_t xd = 3 - (r << 1);
    int16_t x = 0, y = r;

    while ( x <= y ) {
        if( y > 0 ) {
            GuiFillColor(x0 - x, y0 - y,x0 - x, y0 + y, c);
            GuiFillColor(x0 + x, y0 - y,x0 + x, y0 + y, c);
        }
        if( x > 0 ) {
            GuiFillColor(x0 - y, y0 - x,x0 - y, y0 + x, c);
            GuiFillColor(x0 + y, y0 - x,x0 + y, y0 + x, c);
        }
        if ( xd < 0 ) {
            xd += (x << 2) + 6;
        }else {
            xd += ((x - y) << 2) + 10;
            y--;
        }
        x++;
    }
    GuiDrawCircle(x0, y0, r,c);
}

void GuiDrawRoundRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t r, uint16_t c) {
    int16_t n;
    if(x2 < x1) {
        n = x2;
        x2 = x1;
        x1 = n;
    }
    if (y2 < y1) {
        n = y2;
        y2 = y1;
        y1 = n;
    }

    if(r > x2) return;
    if(r > y2) return;

    GuiFillColor(x1 + r, y1, x2 - r, y1, c);
    GuiFillColor(x1 + r, y2, x2 - r, y2, c);
    GuiFillColor(x1, y1 + r, x1, y2 - r, c);
    GuiFillColor(x2, y1 + r, x2, y2 - r, c);
    GuiDrawArc(x1+r, y1+r, r, 0x0C, c);
    GuiDrawArc(x2-r, y1+r, r, 0x03, c);
    GuiDrawArc(x1+r, y2-r, r, 0x30, c);
    GuiDrawArc(x2-r, y2-r, r, 0xC0, c);
}

void GuiFillRoundRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t r, uint16_t c) {
    int16_t  x, y, xd;

    if ( x2 < x1 ) {
        x = x2;
        x2 = x1;
        x1 = x;
    }
    if ( y2 < y1 ) {
        y = y2;
        y2 = y1;
        y1 = y;
    }

    if (r <= 0) return;

    xd = 3 - (r << 1);
    x = 0;
    y = r;

    GuiFillColor(x1 + r, y1, x2 - r, y2, c);

    while ( x <= y ) {
        if( y > 0 ) {
            GuiFillColor(x2 + x - r, y1 - y + r, x2 + x - r, y + y2 - r, c);
            GuiFillColor(x1 - x + r, y1 - y + r, x1 - x + r, y + y2 - r, c);
        }
        if( x > 0 ) {
            GuiFillColor(x1 - y + r, y1 - x + r, x1 - y + r, x + y2 - r, c);
            GuiFillColor(x2 + y - r, y1 - x + r, x2 + y - r, x + y2 - r, c);
        }
        if ( xd < 0 ) {
            xd += (x << 2) + 6;
        }else {
            xd += ((x - y) << 2) + 10;
            y--;
        }
        x++;
    }
}

/**
 * @brief 绘制格点区域
 * */
void GuiDrawMesh(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t gridSize, uint16_t c) {
   int16_t n,m;
   if(x2 < x1) {
      n = x2;
      x2 = x1;
      x1 = n;
   }
   if(y2 < y1) {
      n = y2;
      y2 = y1;
      y1 = n;
   }

   for(m = y1; m <= y2; m += gridSize) {
      for(n = x1; n <= x2; n += gridSize) {
         GuiDrawPixel(n, m, c);
      }
   }
}

/**
 * @brief 在x,y位置绘制字符，支持8位字符和16位宽字符
 * @param ch 字符编码
 * @param x 开始绘制左上x轴坐标
 * @param y 开始绘制左上y轴坐标
 * @param foregroundColor 字体颜色
 * @param backgroundColor 字体背景色
 * @param font 字体
 * */
void GuiDrawChar(wchar ch, uint16_t x, uint16_t y, uint16_t foregroundColor, uint16_t backgroundColor, Font *font) {

    if(ch < font->startChar || ch > font->endChar) return;

    uint32_t cursor = 0, bitmapline = 0, byteline;
    uint16_t data[font->width * font->height];
    uint32_t offset = font->calcOffset(font, ch);

    uint8_t heightpart;
    uint8_t *ptr = font->data + offset;

    byteline = font->width >> 3;
    byteline = ((font->width % 8) ? (byteline + 1) : byteline);

    St7735sSetRegion(x, y,  (x + font->width - 1),  (y + font->height - 1));

    for(uint32_t j = 0; j < font->height; j++) {
        memcpy((uint8_t *)&bitmapline, ptr, byteline);

        if(!font->internal) {
            // 外部字库像素倒序处理，需要对所使用字模字节序做出调整
            heightpart = (bitmapline >> 12) & 0x0F;
            bitmapline = (bitmapline << 4) | (heightpart);
        }
        for(uint32_t i = 0; i < font->width; i++) {
            data[cursor++] = ((bitmapline >> (font->width - i)) & 0x1) ? foregroundColor : backgroundColor;
        }
        ptr += byteline;
    }

    St7735sFillData(data);
}


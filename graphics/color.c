/*
 * FileName: color.c
 * @brief 颜色管理，默认支持RGB565
 * Created on: Jun 1, 2020
 * Author: Yanye
 */

#include <color.h>

static uint16_t parseColor(uint32_t rgb);

const Color COLOR = {0xF800, 0xe2ea, 0x07E0, 0xFFFF, 0x0000, 0x8410, 0xFFE0, 0xBDF7, parseColor};

static uint16_t parseColor(uint32_t rgb) {
    // 砍掉高位
	//return (uint16_t)(((rgb & 0x1F0000) >> 5) | ((rgb & 0x3F00) >> 3) | (rgb & 0x1F));
    // 砍掉低位
    uint8_t red   = (rgb & 0x00ff0000) >> 19;
    uint8_t green = (rgb & 0x0000ff00) >> 10;
    uint8_t blue  = (rgb & 0x000000ff) >> 3;
    return ((red << 11) | (green << 5) | blue);
}


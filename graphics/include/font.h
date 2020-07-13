/*
 * FileName: font.h
 * @brief 字体定义结构体
 * Created on: Jun 1, 2020
 * Author: Yanye
 */

#ifndef _FONT_H_
#define _FONT_H_

#include "stdint.h"

typedef uint16_t wchar;

struct _font;
typedef struct _font Font;

/**
 * 字体结构，仅支持双字节编码的字体
 */
struct _font {
	// 字库文件地址指针
	uint8_t *data;
	// 计算字模数据偏移量回调
	uint32_t (*calcOffset)(Font *font, uint16_t charCode);
	// 开始字符编码
	wchar startChar;
	// 结束字符编码
	wchar endChar;
	// 字体宽度(pixel)
	uint8_t width;
	// 字体高度(pixel)
	uint8_t height;
	// 单个字的字节数(byte)
	uint8_t fontBytes;
	// 是否为内置字体
	uint8_t internal;
};


#endif

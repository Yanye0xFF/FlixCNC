/*
 * FileName: internal_font.c
 * @brief 字体管理
 * Created on: May 31, 2020
 * Author: Yanye
 */

#include <internal_font.h>

/**
 * @brief 自带7*8字体计算字模偏移量实现
 * */
uint32_t calcOffsetDef(Font *font, uint16_t charCode) {
    return ((charCode -  font->startChar) * font->fontBytes);
}

const InternalFont INTERNAL_FONT = {(Font) {(uint8_t *)MiniFontArray, calcOffsetDef, 0x20, 0x7E, 7, 8, 8, TRUE},
		(Font) {(uint8_t *)MiniIconArray, calcOffsetDef, 0x00, 0x04, 7, 8, 8, TRUE}};









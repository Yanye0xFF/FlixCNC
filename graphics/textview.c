/*
 * FileName: textview.c
 * @brief 
 * Created on: Jun 1, 2020
 * Author: Yanye
 */

#include "textview.h"
// GB2312 A1A1－FEFE
// GB2312字库6763(B0A1-F7FE)+846(A1A1~A9EF)个字符 (12*12)
static Font *FontGB2312 = NULL;

static Size calcTextSize(TextView *textview);
static void onMeasure(uint16_t xStart, uint16_t yStart, TextView *view);

static void setText(uint8_t *text, TextView *view);
static void setSize(Rect rect, TextView *view);
static void onDraw(TextView *view);
static void onDestory(TextView *view);
static void onStateChanged(uint8_t newState, TextView *view);

static uint32_t calcOffset2312(Font *font, uint16_t charCode) {
    // 默认的gb2312编码大端模式
    // 区码在低字节位码在高字节
    uint8_t LSB = (charCode >> 8) & 0xFF;
    uint8_t MSB  = charCode & 0xFF;
    if(MSB >= 0xA1 && MSB <= 0xA9 && LSB >= 0xA1) {
        return ((MSB - 0xA1) * 94 + (LSB - 0xA1)) * 24;
    }else if(MSB >= 0xB0 && MSB <= 0xF7 && LSB >= 0xA1) {
        return ((MSB - 0xB0) * 94 + (LSB - 0xA1) + 846) * 24;
    }else {
        return 0;
    }
}

/**
 * @brief textview构造函数
 * @param *str 字符串指针，wchar宽字符串需要强制转换成uint8_t*类型
 * @param encode 编码类型 ENCODE_ASCII 和 ENCODE_GB2312
 * @param xStart textview 左上x轴坐标
 * @param yStart textview 左上y轴坐标
 * */
TextView *newTextView(uint8_t *str, uint8_t encode, uint16_t xStart, uint16_t yStart) {

    TextView *textview = (TextView *)malloc(sizeof(TextView));
    memset((uint8_t *)textview, 0x00, sizeof(TextView));

	textview->text = str;
	textview->encodeType = encode;
	if(ENCODE_ASCII == encode) {
	    textview->font = (Font *)&INTERNAL_FONT.miniFont;
	}else {
	    if(FontGB2312 == NULL) {
	        FontGB2312 = (Font *)malloc(sizeof(Font));
	        FontGB2312->data = loadFile("GB2312.bin");
	        FontGB2312->calcOffset = calcOffset2312;
	        FontGB2312->startChar = 0xA1A1;
	        FontGB2312->endChar = 0xFEFE;
	        FontGB2312->fontBytes = 24;
	        FontGB2312->height = 12;
	        FontGB2312->width = 12;
	        FontGB2312->internal = FALSE;
	    }
	    textview->font = FontGB2312;
	}
	textview->foregroundColor = COLOR.BLACK;
	textview->backgroundColor = COLOR.WHITE;
	textview->style = TEXTVIEW_RECT_FILL;
	textview->textAlign = (ALIGN_V_TOP | ALIGN_H_START);
	onMeasure(xStart, yStart, textview);

	textview->setText = setText;
	textview->setSize = setSize;
	textview->onDraw = onDraw;
	textview->onDestory = onDestory;
	textview->onStateChanged = onStateChanged;
	return textview;
}

/**
 * @brief 测量textview内容所需占用最小的空间
 * @param xStart textview 左上x轴坐标
 * @param yStart textview 左上y轴坐标
 * @param *view textview对象
 * */
static void onMeasure(uint16_t xStart, uint16_t yStart, TextView *view) {
    Size size = calcTextSize(view);
    view->rect.xStart = xStart;
    view->rect.yStart = yStart;
    view->rect.xEnd = xStart + size.width;
    view->rect.yEnd = yStart + size.height;
}

static void onStateChanged(uint8_t newState, TextView *view) {
    view->drawState = newState;
}

/**
 * @brief 更新文本，会触发textview重绘
 * @param *text 字符串指针
 * @param *view textview对象
 * */
static void setText(uint8_t *text, TextView *view) {
    if(view->recycleText && view->text != NULL) {
        free(view->text);
    }
    view->text = text;
    view->drawState &= (~0x1);
}

/**
 * @brief 更新尺寸，会触发textview重绘
 * @param rect 新的尺寸参数
 * @param *view textview对象
 * */
static void setSize(Rect rect, TextView *view) {
    memcpy((uint8_t *)&view->rect, (uint8_t *)&rect, sizeof(Rect));
    view->drawState = 0x00;
}

/**
 * @brief 测量textview内容所需占用最小的空间
 * @param *textview textview对象
 * @return size 尺寸信息包含长/宽
 * */
static Size calcTextSize(TextView *textview) {
    Size size = {0, 0};
    uint16_t lineWidth = 0;
    uint8_t *text = textview->text;
    size.height = (textview->encodeType == ENCODE_ASCII) ?
            (INTERNAL_FONT.miniFont.height + textview->verSpace) : (textview->font->height + textview->verSpace);

    while(*text != '\0') {
        if(*text == '\n') {
            if(size.width < lineWidth) {
                size.width = lineWidth;
            }
            lineWidth = 0;
            size.height += (textview->encodeType == ENCODE_ASCII) ?
                    (INTERNAL_FONT.miniFont.height + textview->verSpace) : (textview->font->height + textview->verSpace);
        }else if(*text >= 0x20 && *text <= 0x7E) {
            lineWidth += (INTERNAL_FONT.miniFont.width + textview->horSpace);
        }else {
            lineWidth += (textview->font->width + textview->horSpace);
        }
        text += (*text > 0x7E) ? WCHAR_SIZE : CHAR_SIZE;
    }
    size.width = (size.width < lineWidth) ? lineWidth : size.width;
    return size;
}

/**
 * @brief textview绘制实现
 * @param *view textview对象
 * */
static void onDraw(TextView *view) {
    // 已绘制过且文本/尺寸未被改变
    if(view->drawState & 0x1) {
        return;
    }
    // 参数检查
    if((view->text == NULL) && (view->placeHolder == FALSE)) return;
    // 绘制边框/背景
    if(!(view->drawState >> 1) || !view->quickDraw) {
        if(TEXTVIEW_ROUNDD_BOX == view->style) {
            GuiDrawRoundRect(view->rect.xStart - 1, view->rect.yStart - 1, view->rect.xEnd + 1,view->rect.yEnd + 1, 2, view->borderColor);
        }else if(TEXTVIEW_BOX == view->style) {
            GuiDrawRect(view->rect.xStart - 1, view->rect.yStart - 1, view->rect.xEnd + 1, view->rect.yEnd + 1, view->borderColor);
        }else if(TEXTVIEW_ROUNDRECT_FILL == view->style) {
            GuiFillRoundRect(view->rect.xStart, view->rect.yStart,view->rect.xEnd,view->rect.yEnd, 4, view->backgroundColor);
        }else {
            GuiFillColor(view->rect.xStart, view->rect.yStart,view->rect.xEnd,view->rect.yEnd, view->backgroundColor);
        }
        view->drawState |= (0x1<<1);
    }
    // 参数检查
    if(view->font->data == NULL) return;
    if(view->text == NULL) return;
    if((view->rect.xEnd - view->rect.xStart) < view->font->width) return;
    if((view->rect.yEnd - view->rect.yStart) < view->font->height) return;

    Size size;
    uint16_t startX, startY;
    uint8_t hAlignType = 0, vAlignType = 0;

    for(uint8_t i = 0; i < 6; i++) {
        if(i < 3 && ((view->textAlign >> i) & 0x1)) {
            hAlignType = i;
        }else if((view->textAlign >> i) & 0x1) {
            vAlignType = (i - 3);
        }
    }

    if(hAlignType != 0 || vAlignType != 0) {
        size = calcTextSize(view);
    }

    if(hAlignType == 0) {
        startX = view->rect.xStart;
    }else if(hAlignType == 1) {
        startX = view->rect.xStart + ((view->rect.xEnd - view->rect.xStart + 1) - size.width) / 2;
    }else {
        startX = view->rect.xStart + ((view->rect.xEnd - view->rect.xStart + 1) - size.width);
    }

    if(vAlignType == 0) {
        startY = view->rect.yStart;
    }else if(vAlignType == 1) {
        startY = view->rect.yStart + ((view->rect.yEnd - view->rect.yStart + 1) - size.height) / 2;
    }else {
        startY = view->rect.xStart + ((view->rect.yEnd - view->rect.yStart + 1) - size.height);
    }

    view->drawState |= 0x1;
    uint8_t *str = view->text;
    uint16_t x = startX, y = startY;
    wchar dispChar = 0x00;

    while(*str != '\0') {
        if(*str == '\n') {
            str++;
            x = startX;
            y = y + view->font->height + view->verSpace;
            continue;
        }
        if(*str >= 0x20 && *str <= 0x7E) {
            // ascii字符使用内置minifont
            if(ENCODE_GB2312 == view->encodeType) {
                GuiDrawChar(*str, x, y + 4, view->foregroundColor, view->backgroundColor, (Font *)&INTERNAL_FONT.miniFont);
            }else {
                GuiDrawChar(*str, x, y, view->foregroundColor, view->backgroundColor, (Font *)&INTERNAL_FONT.miniFont);
            }
            x = (x + INTERNAL_FONT.miniFont.width + view->horSpace);
            str += CHAR_SIZE;
        }else {
            // 宽字符使用gb2312字库
            memcpy((uint8_t *)&dispChar, str, 2);
            GuiDrawChar(dispChar, x, y, view->foregroundColor, view->backgroundColor, view->font);
            x = (x + view->font->width + view->horSpace);
            str += WCHAR_SIZE;
        }
    }
}

static void onDestory(TextView *view) {
    if(view->recycleText && view->text != NULL) {
        free(view->text);
        ConsoleUtilsPrintf("onDestory recycleText\n");
    }
    view->text = NULL;
    ConsoleUtilsPrintf("TextView onDestory\n");
}

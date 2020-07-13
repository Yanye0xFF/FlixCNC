/*
 * conn_view.c
 * @brief 
 * Created on: Jun 8, 2020
 * Author: Yanye
 */

#include "conn_view.h"

void createConnView(Frame *frame) {
    TextView *tvTitle = newTextView((uint8_t *)TEXT_CONN_TITLE, ENCODE_GB2312, 0, 0);
    tvTitle->backgroundColor = COLOR.BLUE;
    tvTitle->foregroundColor = COLOR.WHITE;
    tvTitle->textAlign = (ALIGN_H_CENTER | ALIGN_V_CENTER);
    tvTitle->setSize((Rect){0, 0, 127, 13}, tvTitle);

    ImageView *ivWifi = newImageView(1, 14, "wifi.bmp");
    TextView *tvWifi = newTextView((uint8_t *)TEXT_WIFI, ENCODE_ASCII, 18, 19);

    ImageView *ivPwd = newImageView(1, 30, "pwd.bmp");
    TextView *tvPwd = newTextView((uint8_t *)TEXT_PWD, ENCODE_GB2312, 18, 32);

    ImageView *ivCode = newImageView(24, 48, "qrcode.bmp");

    frame->addView(frame, TEXTVIEW, 0, (void *)tvTitle);
    frame->addView(frame, IMAGEVIEW, 0, (void *)ivWifi);
    frame->addView(frame, TEXTVIEW, 1, (void *)tvWifi);
    frame->addView(frame, IMAGEVIEW, 1, (void *)ivPwd);
    frame->addView(frame, TEXTVIEW, 2, (void *)tvPwd);
    frame->addView(frame, IMAGEVIEW, 3, (void *)ivCode);
}



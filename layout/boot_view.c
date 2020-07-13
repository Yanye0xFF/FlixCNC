/*
 * bootloader_view.c
 * @brief 
 * Created on: Jun 8, 2020
 * Author: Yanye
 */

#include "boot_view.h"

ProgressBar *createBootView(Frame *frame) {
    TextView *tvTitle = newTextView((uint8_t *)TEXT_TITLE, ENCODE_ASCII, 0, 0);
    tvTitle->backgroundColor = COLOR.BLUE;
    tvTitle->textAlign = (ALIGN_H_CENTER | ALIGN_V_CENTER);
    tvTitle->setSize((Rect){0, 0, 127, 13}, tvTitle);

    ImageView *image = newImageView(28, 18, "splash.bmp");

    ProgressBar *progressBar = newProgressBar(10, 95, 110, 10);
    progressBar->max = 100;
    progressBar->progress = 4;
    progressBar->borderColor = COLOR.parseColor(0xa3cf62);
    progressBar->fillColor = COLOR.parseColor(0x7fb80e);
    progressBar->emptyColor = COLOR.WHITE;

    TextView *tvHint = newTextView((uint8_t *)TEXT_BOOT, ENCODE_GB2312, 10, 110);
    tvHint->textAlign = (ALIGN_H_CENTER | ALIGN_V_TOP);
    tvHint->rect = (Rect){10, 110, 119, 125};

    frame->addView(frame, TEXTVIEW, 0, (void *)tvTitle);
    frame->addView(frame, IMAGEVIEW, 0, (void *)image);
    frame->addView(frame, PROGRESSBAR, 0, (void *)progressBar);
    frame->addView(frame, TEXTVIEW, 1, (void *)tvHint);
    return progressBar;
}

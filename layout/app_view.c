/*
 * app_view.c
 * @brief 
 * Created on: Jun 9, 2020
 * Author: Yanye
 */

#include "app_view.h"

void createAppView(uint8_t hostType, Frame *frame) {
    ImageView *ivHost = newImageView(0, 0, (hostType == HOST_TYPE_WINDOWS) ? "windows.bmp" : "android.bmp");
    TextView *tvHost = newTextView((uint8_t *)TEXT_HOST, ENCODE_GB2312, 15, 2);

    TextView *tvXAxis = newTextView((uint8_t *)TEXT_X_AXIS, ENCODE_GB2312, 0, 0);
    tvXAxis->quickDraw = 1;
    tvXAxis->style = TEXTVIEW_ROUNDRECT_FILL;
    tvXAxis->foregroundColor = COLOR.WHITE;
    tvXAxis->backgroundColor = COLOR.parseColor(0xd93a49);
    tvXAxis->textAlign = (ALIGN_H_START | ALIGN_V_CENTER);
    tvXAxis->setSize((Rect){0, 18, 127, 33}, tvXAxis);

    TextView *tvYAxis = newTextView((uint8_t *)TEXT_Y_AXIS, ENCODE_GB2312, 0, 0);
    tvYAxis->quickDraw = 1;
    tvYAxis->style = TEXTVIEW_ROUNDRECT_FILL;
    tvYAxis->foregroundColor = COLOR.WHITE;
    tvYAxis->backgroundColor = COLOR.parseColor(0x694d9f);
    tvYAxis->textAlign = (ALIGN_H_START | ALIGN_V_CENTER);
    tvYAxis->setSize((Rect){0, 36, 127, 51}, tvYAxis);

    TextView *tvZAxis = newTextView((uint8_t *)TEXT_Z_AXIS, ENCODE_GB2312, 0, 0);
    tvZAxis->quickDraw = 1;
    tvZAxis->style = TEXTVIEW_ROUNDRECT_FILL;
    tvZAxis->foregroundColor = COLOR.WHITE;
    tvZAxis->backgroundColor = COLOR.parseColor(0x007947);
    tvZAxis->textAlign = (ALIGN_H_START | ALIGN_V_CENTER);
    tvZAxis->setSize((Rect){0, 54, 127, 69}, tvZAxis);

    TextView *tvTask = newTextView((uint8_t *)TEXT_TASK_IDLE, ENCODE_GB2312, 0, 0);
    tvXAxis->quickDraw = 1;
    tvTask->style = TEXTVIEW_ROUNDRECT_FILL;
    tvTask->foregroundColor = COLOR.WHITE;
    tvTask->backgroundColor = COLOR.parseColor(0x33a3dc);
    tvTask->textAlign = (ALIGN_H_START | ALIGN_V_CENTER);
    tvTask->setSize((Rect){0, 72, 127, 87}, tvTask);

    ProgressBar *progress = newProgressBar(8, 94, 112, 10);
    progress->max = 100;
    progress->borderColor = COLOR.parseColor(0xa3cf62);
    progress->fillColor = COLOR.parseColor(0x7fb80e);
    progress->emptyColor = COLOR.WHITE;

    ImageView *ivSystem = newImageView(0, 110, "system.bmp");
    TextView *tvSystem = newTextView((uint8_t *)TEXT_SYS_IDLE, ENCODE_GB2312, 15, 112);
    tvSystem->foregroundColor = COLOR.parseColor(0x1d953f);

    ImageView *ivSpindle = newImageView(64, 110, "spindle.bmp");
    TextView *tvSpindle = newTextView((uint8_t *)TEXT_SP_STOP, ENCODE_GB2312, 79, 112);
    tvSpindle->foregroundColor = COLOR.parseColor(0x1d953f);

    frame->addView(frame, IMAGEVIEW, 0, (void *)ivHost);
    frame->addView(frame, TEXTVIEW, 0, (void *)tvHost);

    frame->addView(frame, TEXTVIEW, 1, (void *)tvXAxis);
    frame->addView(frame, TEXTVIEW, 2, (void *)tvYAxis);
    frame->addView(frame, TEXTVIEW, 3, (void *)tvZAxis);

    frame->addView(frame, TEXTVIEW, 4, (void *)tvTask);
    frame->addView(frame, PROGRESSBAR, 0, (void *)progress);
    frame->updateViewState(frame, PROGRESSBAR, 0, VIEW_ONLY_OCCUPY);

    frame->addView(frame, IMAGEVIEW, 1, (void *)ivSystem);
    frame->addView(frame, TEXTVIEW, 5, (void *)tvSystem);

    frame->addView(frame, IMAGEVIEW, 2, (void *)ivSpindle);
    frame->addView(frame, TEXTVIEW, 6, (void *)tvSpindle);

}




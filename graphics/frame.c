/*
 * FileName: frame.c
 * @brief 
 * Created on: Jun 1, 2020
 * Author: Yanye
 */

#include "frame.h"

static View* findEmptyView(Frame *frame);
static View* findViewById(Frame *frame, ViewType type, uint8_t id);

static void invalidate(Frame *frame);
static Boolean addView(Frame *frame, ViewType type, uint8_t id, void *entity);
static void removeView(Frame *frame, ViewType type, uint8_t id);
static void updateViewState(Frame *frame, ViewType type, uint8_t id, uint8_t newState);

static void removeAllView(Frame *frame);

Frame *newFrame(uint8_t maxViews, Rect rect) {
    Frame *frame = (Frame *)malloc(sizeof(Frame));
    View *viewlist = (View *)malloc(sizeof(View) * maxViews);

    memset((uint8_t *)frame, 0x00, sizeof(Frame));
    memset((uint8_t *)viewlist, 0x00, sizeof(View) * maxViews);

	frame->maxViews = maxViews;
	frame->viewGroup = viewlist;
	frame->rect = rect;
	frame->foregroundColor = COLOR.BLACK;
	frame->backgroundColor = COLOR.WHITE;
	frame->borderColor = COLOR.BLUE;

	frame->invalidate = invalidate;
	frame->addView = addView;
	frame->findViewById = findViewById;
    frame->removeView = removeView;
	frame->removeAllView = removeAllView;
	frame->updateViewState = updateViewState;

	return frame;
}

void destoryFrame(Frame *frame) {

}

static void invalidate(Frame *frame) {
    if((frame->drawState & 0x1) == 0) {
        frame->drawState = 0x1;
        GuiFillColor(frame->rect.xStart , frame->rect.yStart, frame->rect.xEnd,frame->rect.yEnd, frame->backgroundColor);
    }
	// 绘制内部view
	View *view;
	 for(uint8_t i = 0; i < frame->maxViews; i++) {
        view = (frame->viewGroup + i);
        if((view->state & VIEW_USABLE_MASK) && (view->state & VIEW_VISIBILITY_MASK)) {
            switch(view->type) {
                case TEXTVIEW: {
                    TextView *tv = (TextView *)view->entity;
                    tv->onDraw(tv);
                    break;
                }
                case IMAGEVIEW: {
                    ImageView *iv = (ImageView *)view->entity;
                    iv->onDraw(iv);
                    break;
                }
                case PROGRESSBAR: {
                    ProgressBar *pb = (ProgressBar *)view->entity;
                    pb->onDraw(pb);
                    break;
                }
                default:
                    break;
            }
       }
    }
}

static Boolean addView(Frame *frame, ViewType type, uint8_t id, void *entity) {
    View *view = findEmptyView(frame);
    if(view != NULL) {
        // VIEW_OCCUPY & VIEW_VISIBLE
        view->state = 0x3;
        view->type = type;
        view->id = id;
        view->entity = entity;
        return TRUE;
    }
    return FALSE;
}

/**
 * @brief 清除Frame中的View，将会触发最大范围的重绘
 * */
static void removeView(Frame *frame, ViewType type, uint8_t id) {
    View *view = findViewById(frame, type, id);
    if(view != NULL) {
        if(type == TEXTVIEW) {
            TextView *tv = (TextView *)view->entity;
            tv->onDestory(tv);
        }else if(type == IMAGEVIEW) {
            ImageView *iv = (ImageView *)view->entity;
            iv->onDestory(iv);
        }else if(type == PROGRESSBAR) {
            ProgressBar *pb = (ProgressBar *)view->entity;
            pb->onDestory(pb);
        }
        free(view->entity);
        memset((uint8_t *)view, 0x00, sizeof(View));
        for(uint8_t i = 0; i < frame->maxViews; i++) {
            view = (frame->viewGroup + i);
            if(view->state & 0x1) {
                if(view->type == TEXTVIEW) {
                    TextView *tv = (TextView *)view->entity;
                    tv->onStateChanged(0, tv);
                }else if(type == IMAGEVIEW) {
                    ImageView *iv = (ImageView *)view->entity;
                    iv->onStateChanged(0, iv);
                }else if(type == PROGRESSBAR) {
                    ProgressBar *pb = (ProgressBar *)view->entity;
                    pb->onStateChanged(0, pb);
                }
           }
        }
        frame->drawState = 0x00;
    }
}

static void removeAllView(Frame *frame) {
    View *view;
    for(uint8_t i = 0; i < frame->maxViews; i++) {
        view = (frame->viewGroup + i);
        if((VIEW_OCCUPY == (view->state & VIEW_USABLE_MASK)) && view->entity != NULL) {
            if(view->type == TEXTVIEW) {
                TextView *tv = (TextView *)view->entity;
                tv->onDestory(tv);
            }else if(view->type == IMAGEVIEW) {
                ImageView *iv = (ImageView *)view->entity;
                iv->onDestory(iv);
            }else if(view->type == PROGRESSBAR) {
                ProgressBar *pb = (ProgressBar *)view->entity;
                pb->onDestory(pb);
            }
            free(view->entity);
        }
    }
    memset((uint8_t *)frame->viewGroup, 0x00, sizeof(View) * frame->maxViews);
    frame->drawState = 0x00;
}


static void updateViewState(Frame *frame, ViewType type, uint8_t id, uint8_t newState) {
    View *view = findViewById(frame, type, id);
    if(view != NULL) {
        view->state = newState;
    }
}

static View* findEmptyView(Frame *frame) {
    View *view;
    for(uint8_t i = 0; i < frame->maxViews; i++) {
        view = (frame->viewGroup + i);
        if(VIEW_FREE == (view->state & VIEW_USABLE_MASK)) {
            return view;
        }
    }
    return NULL;
}

static View* findViewById(Frame *frame, ViewType type, uint8_t id) {
    View *view;
    for(uint8_t i = 0; i < frame->maxViews; i++) {
        view = (frame->viewGroup + i);
        if((view->type == type) && (view->id == id)) {
            return view;
       }
    }
    return NULL;
}


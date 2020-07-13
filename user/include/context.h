/*
 * context.h
 * @brief 
 * Created on: Jun 25, 2020
 * Author: Yanye
 */

#ifndef _CONTEXT_H_
#define _CONTEXT_H_


#include "stdint.h"
#include "stdlib.h"
#include "string.h"

#include "frame.h"

#include "consoleUtils.h"

typedef struct _context {
    Frame *frame;
    Frame *(*prepareFrame)();
} Context;

Context *contextInstance();

#endif

/*
 * text.h
 * @brief 
 * Created on: Jun 8, 2020
 * Author: Yanye
 */

#ifndef _TEXT_H_
#define _TEXT_H_


#include "stdint.h"

extern const char *MACHINE_NAME;

extern const char *TEXT_TITLE;
extern const uint8_t TEXT_BOOT[];
extern const uint8_t TEXT_BOOT_FINISH[];

extern const uint8_t TEXT_CONN_TITLE[];
extern const uint8_t TEXT_PWD[];
extern const char *TEXT_WIFI;

extern const uint8_t TEXT_HOST[];
extern const uint8_t TEXT_X_AXIS[];
extern const uint8_t TEXT_Y_AXIS[];
extern const uint8_t TEXT_Z_AXIS[];
extern const uint8_t TEXT_TASK_IDLE[];
extern const uint8_t TEXT_SYS_RUN[];
extern const uint8_t TEXT_SYS_IDLE[];
extern const uint8_t TEXT_SP_RUN[];
extern const uint8_t TEXT_SP_STOP[];


#endif /* USER_TEXT_H_ */

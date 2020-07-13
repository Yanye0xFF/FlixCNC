/*
 * motion_group.c
 * @brief 雕刻机运动组-编址范围0x40~4F
 * Created on: Jun 16, 2020
 * Author: Yanye
 */

#include "motion_group.h"

void processMotionCmd(uint8_t *package) {

    const uint32_t offsets[] = {2, 6, 10};
    //NetWork *network = networkInstance();
    Spindle *spindle = spindleInstanec();
    Stepper *stepper = stepperInstance();

    uint8_t command = *(package + 0);
    uint8_t longValue[4];
    long positions[3];

    switch(command) {
    case GO_PHYHOME_CMD:
        stepper->goPhyHome();
        break;
    case GO_WORKHOME_CMD:
        stepper->goWorkHome();
        break;
    case JOGMOTION_CMD:
        for(uint32_t i = 0; i < 3; i++) {
            memcpy(longValue, (package + offsets[i]), 4);
            positions[i] = *(long *)longValue;
        }
        // longValue[0] 用作轴标识
        longValue[0] = *(package + 1);
        stepper->requestJogMove(longValue[0], positions[0], positions[1], positions[2]);
        break;
    case LONGMOTION_START_CMD:
        longValue[0] = *(package + 1);
        longValue[1] = *(package + 2);
        stepper->requestLongMove(Long_Motion_START, longValue[0], longValue[1]);
        break;
    case LONGMOTION_END_CMD:
        stepper->requestLongMove(Long_Motion_STOP, 0x7, 0x00);
        break;
    case SPINDLE_ON_CMD:
        longValue[0] = *(package + 1);
        spindle->runSpindle(longValue[0]);
        break;
    case SPINDLE_OFF_CMD:
        longValue[0] = *(package + 1);
        spindle->stopSpindle(longValue[0]);
        break;
    case TONE_MAKE_CMD:
        longValue[0] = *(package + 1);
        spindle->playTone(longValue[0]);
        break;
    case LED_CONTROL_CMD:
        longValue[0] = *(package + 1);
        spindle->updateSpindleLed(longValue[0]);
        break;
    default:
        break;
    }
}

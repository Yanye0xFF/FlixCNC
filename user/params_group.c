/*
 * params_group.c
 * @brief 雕刻机/主轴参数设置组-编址范围0x10~2F
 * Created on: Jun 16, 2020
 * Author: Yanye
 */
#include "params_group.h"

void processParamCmd(uint8_t *package) {

    const uint32_t offsets[] = {1, 5, 9};

    NetWork *network = networkInstance();
    Spindle *spindle = spindleInstanec();
    Stepper *stepper = stepperInstance();

    uint8_t longValue[4];
    uint8_t doubleValue[8];
    long positions[3];

    uint8_t command = *(package + 0);
    uint8_t retBuffer[32];

    switch(command) {
        case SET_WORKHOME_CMD:
            // 设置工作区原点
            for(uint32_t i = 0; i < 3; i++) {
                memcpy(longValue, (package + offsets[i]), 4);
                positions[i] = *(long *)longValue;
            }
            stepper->setWorkHome(positions[0], positions[1], positions[2]);
            break;
        case SET_CURRENT_WORKHOME_CMD: {
            // 使用雕刻机当前位置作为工作原点
            stepper->setWorkHomeEx();
            // 返回当前位置及工作原点
            StepperState state = stepper->getStepperState();
            CountPosition phypos = stepper->getCurrentPosition();
            CountPosition homepos = stepper->getWorkHomePosition();
            retBuffer[0] = 0x11;
            memcpy((retBuffer + 1), (uint8_t *)&state, 1);
            memcpy(retBuffer + 2, (uint8_t *)&phypos, sizeof(CountPosition));
            memcpy(retBuffer + 14, (uint8_t *)&homepos, sizeof(CountPosition));
            network->tcpSend(retBuffer, 26);
            break;
        }
        case SET_CALLBACK_RATE_CMD:
            // 设置实时位置反馈速率
            memcpy(longValue, (package + offsets[0]), 4);
            positions[0] = *(long *)longValue;
            stepper->setCallbackRate(positions[0]);
            break;
        case SET_JOG_MOTION_SPEED_CMD:
            memcpy(doubleValue, (package + offsets[0]), 8);
            stepper->setMotionSpeed(Jog_Motion, (*(double *)doubleValue));
            break;
        case SET_LONG_MOTION_SPEED_CMD:
            memcpy(doubleValue, (package + offsets[0]), 8);
            stepper->setMotionSpeed(Long_Motion, (*(double *)doubleValue));
            break;
        case SPINDLE_PARAM_ALL_CMD: {
            uint16_t dutyMin, dutyMax, vssTime;
            memcpy((uint8_t *)&dutyMin, (package + 1), 2);
            memcpy((uint8_t *)&dutyMax, (package + 3), 2);
            memcpy((uint8_t *)&vssTime, (package + 5), 2);
            // longValue[0] as spindleDir typeof uint8_t
            // longValue[1] as toneTick typeof uint8_t
            memcpy(longValue, (package + 7), 2);
            spindle->setParams(dutyMin, dutyMax, vssTime, longValue[0], longValue[1]);
            break;
        }
        case SPINDLE_DUTY_CMD: {
            uint16_t dutyMin, dutyMax;
            memcpy((uint8_t *)&dutyMin, (package + 1), 2);
            memcpy((uint8_t *)&dutyMax, (package + 3), 2);
            spindle->setDuty(dutyMin, dutyMax);
            break;
        }
        case SPINDLE_VSSTIME_CMD: {
            uint16_t vssTime;
            memcpy((uint8_t *)&vssTime, (package + 1), 2);
            // longValue[0] as as force typeof uint8_t
            longValue[0] = *(package + 3);
            spindle->setVssTime(vssTime, longValue[0]);
            break;
        }
        case SPINDLE_DIR_CMD:
            longValue[0] = *(package + 1);
            spindle->setSpindleDir(longValue[0]);
            break;
        case SPINDLE_TONETICK_CMD:
            longValue[0] = *(package + 1);
            spindle->setToneTick(longValue[0]);
            break;
        case SPINDLE_SPEEDUP_CMD: {
            uint16_t dutySeg, timeSeg;
            memcpy((uint8_t *)&dutySeg, (package + 1), 2);
            memcpy((uint8_t *)&timeSeg, (package + 3), 2);
            spindle->setSpeedUpParams(dutySeg, timeSeg);
            break;
        }
        case SPINDLE_SLOWDOWN_CMD: {
            uint16_t dutySeg, timeSeg;
            memcpy((uint8_t *)&dutySeg, (package + 1), 2);
            memcpy((uint8_t *)&timeSeg, (package + 3), 2);
            spindle->setSlowDownParams(dutySeg, timeSeg);
            break;
        }
        default:
            break;
    }

}


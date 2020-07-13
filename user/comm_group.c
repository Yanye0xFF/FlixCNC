/*
 * comm_group.c
 * @brief 连接/查询通用组-编址范围0x00~0x0F
 * Created on: Jun 15, 2020
 * Author: Yanye
 */

#include "comm_group.h"

void processCommCmd(uint8_t *package, Frame *frame) {
    NetWork *network = networkInstance();
    Spindle *spindle = spindleInstanec();
    Stepper *stepper = stepperInstance();
    uint8_t command = *(package + 0);
    uint8_t retBuffer[32];

    if(CONNECT_CMD == command) {
        // 更新界面
        frame->removeAllView(frame);
        createAppView(HOST_TYPE_ANDROID, frame);
        // 取出app名称
        uint8_t *namePtr = (package + 4);
        uint8_t length = 0;
        while(*namePtr != '\0') {
            length++;
            namePtr++;
        }
        // 更新textview显示
        View *view = frame->findViewById(frame, TEXTVIEW, 0);
        if(view->entity != NULL) {
            // 1字节结束符，6字节标题
            uint8_t *strHost = (uint8_t *)malloc(sizeof(uint8_t) * (length + 1 + 6));
            memcpy(strHost, (uint8_t *)TEXT_HOST, 6);
            memcpy((strHost + 6), (package + 4), length);
            *(strHost + length + 6) = '\0';
            // 设置文字
            TextView *tvHost = (TextView *)view->entity;
            tvHost->setText(strHost, tvHost);
            tvHost->recycleText = TRUE;
        }
        stepper->updateDisplay();
        // 响铃提示
        if(*(package + 3)) {
            spindle->playTone(1);
        }
        // 返回数据
        retBuffer[0] = 0x0;
        retBuffer[1] = HARDWARE_VERSION;
        retBuffer[2] = SOFTWARE_VERSION;
        memcpy((retBuffer + 3), (uint8_t *)MACHINE_NAME, 7);
        retBuffer[11] = '\0';
        network->tcpSend(retBuffer, 12);

    }else if(DISCONNECT_CMD == command) {
        // 响铃提示
        if(*(package + 1)) {
            spindle->playTone(1);
        }
        stepper->toggleAxisENA(AXIS_CLOSE);
        frame->removeAllView(frame);
        createConnView(frame);

    }else if(QUERY_MACHINE_STATE_CMD == command) {
        // 获取数据
        StepperState state = stepper->getStepperState();
        CountPosition phypos = stepper->getCurrentPosition();
        CountPosition homepos = stepper->getWorkHomePosition();

        retBuffer[0] = 0x02;
        memcpy((retBuffer + 1), (uint8_t *)&state, 1);
        memcpy(retBuffer + 2, (uint8_t *)&phypos, sizeof(CountPosition));
        memcpy(retBuffer + 14, (uint8_t *)&homepos, sizeof(CountPosition));
        network->tcpSend(retBuffer, 26);

    }else if(QUERY_SPINDLE_STATE_CMD == command) {
        retBuffer[0] = 0x03;
        retBuffer[1] = spindle->spindleState;

        memcpy((retBuffer + 2), (uint8_t *)&spindle->dutyMin, 2);// 2.3
        memcpy((retBuffer + 4), (uint8_t *)&spindle->dutyMax, 2);// 4.5
        memcpy((retBuffer + 6), (uint8_t *)&spindle->vssTime, 2);// 6.7
        retBuffer[8] = spindle->spindleDir;// 8.9
        retBuffer[9] = spindle->toneTick;
        memcpy((retBuffer + 10), (uint8_t *)&spindle->speedUpDutySegment, 2);// 10.11
        memcpy((retBuffer + 12), (uint8_t *)&spindle->speedUpTimeSegment, 2);// 12.13
        memcpy((retBuffer + 14), (uint8_t *)&spindle->slowDownDutySegment, 2);// 14.15
        memcpy((retBuffer + 16), (uint8_t *)&spindle->slowDownTimeSegment, 2);// 16.17
        network->tcpSend(retBuffer, 18);
    }
}

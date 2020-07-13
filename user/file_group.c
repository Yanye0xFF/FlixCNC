/*
 * file_group.c
 * @brief 
 * Created on: Jun 19, 2020
 * Author: Yanye
 */

#include "file_group.h"

void processTransCmd(uint8_t *package, ByteStream *stream) {

    NetWork *network = networkInstance();
    uint8_t command = *(package + 0);
    uint8_t retBuffer[2];

    if(FILEHEADER_CMD == command) {
        uint32_t fileSize = 0x00;
        uint16_t fileCRC = 0x00;
        memcpy((uint8_t *)&fileSize, (package + 1), 4);
        memcpy((uint8_t *)&fileCRC, (package + 5), 2);

        if(stream->buffer != NULL) {
            stream->release(stream);
        }
        Boolean result = stream->prepare(fileSize, stream);
        stream->crc16 = fileCRC;

        retBuffer[0] = FILEHEADER_CMD;
        retBuffer[1] = (result ? 0x00 : 0x01);
        network->tcpSend(retBuffer, 2);

    }else if(FILEFILL_CMD == command) {
        uint8_t len = *(package + 1);
        stream->write((package + 2), 0, len, stream);

        if(0x1E == len) {
            retBuffer[1] = 0x00;
        }else if(stream->cursor != stream->size) {
            retBuffer[1] = 0x01;
        }else {
            uint16_t calcCRC = crc16_ccitt(stream->buffer, stream->size);
            retBuffer[1] = (calcCRC == stream->crc16) ? 0x03 : 0x02;
        }

        retBuffer[0] = FILEFILL_CMD;
        network->tcpSend(retBuffer, 2);

    }else if(PARSE_GCODE_CMD == command) {
        Interpreter *interpreter = interpreterInstance();
        interpreter->parseGcode(stream->buffer);

        stream->release(stream);

        retBuffer[0] = PARSE_GCODE_CMD;
        retBuffer[1] = 0x00;
        network->tcpSend(retBuffer, 2);

    }else if(RUN_GCODE_CMD == command) {
        Stepper *stepper = stepperInstance();
        stepper->wakeUp();

    }
}

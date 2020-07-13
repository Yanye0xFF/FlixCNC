/*
 * bytestream.h
 * @brief 
 * Created on: Jun 19, 2020
 * Author: Yanye
 */

#ifndef USER_INCLUDE_BYTESTREAM_H_
#define USER_INCLUDE_BYTESTREAM_H_

#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "hw_types.h"

typedef enum _seek_type {
    SEEKTYPE_START = 0,
    SEEKTYPE_END,
} SeekType;


struct _bytestream;
typedef struct _bytestream ByteStream;

struct _bytestream {
    uint8_t *buffer;
    uint32_t size;
    uint32_t cursor;
    uint32_t tag;
    uint16_t crc16;
    uint16_t isFull;
    Boolean (*prepare)(uint32_t size, ByteStream *stream);
    void (*seekTo)(SeekType seekType,  ByteStream *stream);
    void (*mark)(uint32_t position, ByteStream *stream);
    void (*reset)(ByteStream *stream);
    void (*write)(uint8_t *src, uint32_t offset, uint32_t length, ByteStream *stream);
    void (*read)(uint8_t *dest, uint32_t offset, uint32_t length, ByteStream *stream);
    void (*release)(ByteStream *stream);
    void (*destory)(ByteStream *stream);
};

ByteStream *newByteStream();


#endif

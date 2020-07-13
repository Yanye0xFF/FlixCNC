/*
 * bytestream.c
 * @brief 
 * Created on: Jun 19, 2020
 * Author: Yanye
 */

#include "bytestream.h"

static Boolean prepare(uint32_t size, ByteStream *stream);
static void release(ByteStream *stream);
static void destory(ByteStream *stream);

static void seekTo(SeekType seekType,  ByteStream *stream);
static void mark(uint32_t position, ByteStream *stream);
static void reset(ByteStream *stream);

static void write(uint8_t *src, uint32_t srcOffset, uint32_t length, ByteStream *stream);
static void read(uint8_t *dest, uint32_t destOffset, uint32_t length, ByteStream *stream);

ByteStream *newByteStream() {
    ByteStream *stream = (ByteStream *)malloc(sizeof(ByteStream));
    memset((uint8_t *)stream, 0x00, sizeof(ByteStream));

    stream->prepare = prepare;
    stream->seekTo = seekTo;
    stream->mark = mark;
    stream->reset = reset;
    stream->write = write;
    stream->read = read;
    stream->release = release;
    stream->destory = destory;
    return stream;
}

static Boolean prepare(uint32_t size, ByteStream *stream) {
    stream->buffer = (uint8_t *)malloc(sizeof(uint8_t) * size);
    if(NULL == stream->buffer) {
        return FALSE;
    }
    memset(stream->buffer, 0x00, sizeof(uint8_t) * size);
    stream->size = size;
    stream->cursor = 0;
    return TRUE;
}

static void seekTo(SeekType seekType,  ByteStream *stream) {
    if(SEEKTYPE_START == seekType) {
        stream->cursor = 0;
    }else if(SEEKTYPE_END == seekType) {
        stream->cursor = (stream->size - 1);
    }
}

static void mark(uint32_t position, ByteStream *stream) {
    if(position < stream->size) {
        stream->tag = position;
    }
}

static void reset(ByteStream *stream) {
    stream->cursor = stream->tag;
}

static void write(uint8_t *src, uint32_t srcOffset, uint32_t length, ByteStream *stream) {
    memcpy((stream->buffer + stream->cursor), (src + srcOffset), length);
    stream->cursor += length;
    stream->isFull = (stream->cursor == stream->size) ? TRUE : FALSE;
}

static void read(uint8_t *dest, uint32_t destOffset, uint32_t length, ByteStream *stream) {
    memcpy((dest + destOffset), (stream->buffer + stream->cursor), length);
    stream->cursor += length;
}

static void release(ByteStream *stream) {
    if(NULL != stream->buffer) {
        free(stream->buffer);
        stream->buffer = NULL;
    }
}

static void destory(ByteStream *stream) {
    release(stream);
    free(stream);
    stream = NULL;
}


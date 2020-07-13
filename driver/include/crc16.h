#ifndef _CRC16_H_
#define _CRC16_H_

#include "stdint.h"

uint16_t crc16_ccitt(uint8_t *buf, uint32_t len);

#endif

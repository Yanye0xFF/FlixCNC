/*
 * network.h
 * @brief TCP通讯网络通用接口，在回调函数中不要执行耗时操作
 * Created on: Apr 2, 2020
 * Author: Yanye
 */

#ifndef _NETWORK_H_
#define _NETWORK_H_

#include "stdint.h"
#include "stdlib.h"
#include "string.h"

#include "hw_types.h"
#include "antminer.h"
#include "pin_mux.h"
#include "soc_AM335X.h"
#include "gpio_v2.h"
#include "interrupt.h"
#include "mcspi.h"
#include "mcspi_port0.h"
#include "utils.h"

// esp8266从机读写命令
#define ESP8266_WRITE_CMD        0x02
#define ESP8266_READ_CMD         0x03
#define ESP8266_DUMMY_ADDRESS    0x00
// hspi缓冲区
#define HSPI_PACKAGE_SIZE        32
#define HSPI_HEADER_SIZE         2
// spi传输中断线
#define SPI_TX_INT_PIN_ADDR    GPIO_0_15
#define SPI_TX_INT_PIN         15
#define SPI_RX_INT_PIN_ADDR    GPIO_0_14
#define SPI_RX_INT_PIN         14
// ESP8266复位
#define ESP8266_RST_PIN_ADDR   GPIO_1_16
#define ESP8266_RST_PIN        16
// ESP8266片选
#define ESP8266_CS_PIN_ADDR    GPIO_0_5
#define ESP8266_CS_PIN         5

typedef void (*TcpRecvCallback)(uint8_t *data, uint32_t maxLength);
typedef void (*TcpSentCallback)();

typedef struct _network {
	void (*init)(uint32_t spiPriority, uint32_t gpioPriority);
	Boolean (*tcpSend)(uint8_t *data, uint32_t length);
	void (*registerTcpRecvCallback)(TcpRecvCallback callback);
	void (*registerTcpSentCallback)(TcpSentCallback callback);
	void (*removeTcpRecvCallback)();
	void (*removeTcpSentCallback)();
} NetWork;


NetWork *networkInstance();


#endif

/*
 * i2c_port2.h
 * @brief 
 * Created on: Apr 25, 2020
 * Author: Yanye
 */

#ifndef _I2C_PORT2_H_
#define _I2C_PORT2_H_

#include "hw_types.h"
#include "soc_AM335x.h"
#include "hw_cm_per.h"
#include "pin_mux.h"
#include "interrupt.h"
#include "hsi2c.h"
#include "gpio_v2.h"
#include "antminer.h"
#include "consoleUtils.h"

#include "int_priority.h"

#include "stdint.h"
#include "string.h"

#define CONTROL_CONF_I2C2_SDA   (0x978)
#define CONTROL_CONF_I2C2_SCL   (0x97C)

// I2C时钟配置，参见2019版手册Page4641
#define I2C_SYS_CLK_48MHZ       (48000000u)
#define I2C_INTERNAL_CLK_12MHZ  (12000000u)
#define I2C_BUS_FREQ_MAX        (400000u)
#define I2C_BUS_FREQ_MIN        (100000u)
#define I2C_BUS_SPEED           (50000u)

#define I2C_BUFFER_SIZE         (32u)

// 与I2C通讯的从机地址
#define  I2C_SLAVE_ADDR         (0x7F)

void I2C2AINTCConfigure();
void SetupI2C2();
void I2C2Send(uint8_t *data, uint32_t length);

#endif

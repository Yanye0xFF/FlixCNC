/*
 * st7735s.h
 * @brief st7735s控制器驱动
 * Created on: Feb 12, 2020
 * Author: Yanye
 */

#ifndef _ST7735S_H_
#define _ST7735S_H_

#include "stdint.h"

#include "antminer.h"
#include "soc_AM335x.h"
#include "hw_cm_per.h"
#include "hw_types.h"
#include "hw_control_AM335x.h"
#include "gpio_v2.h"
#include "mcspi.h"
#include "pin_mux.h"
#include "mcspi_port1.h"
#include "mcspi_comm.h"
#include "dmtimer.h"

#define LCD_LANDSCAPE

// lcd width and height (pixels)
#define LCD_WIDTH 128
#define LCD_HEIGHT 128

#define LCD_CONTROL_PIN_REGS SOC_GPIO_3_REGS

#define RESET_PIN_ADDR GPIO_3_21
#define RESET_PIN 21

#define A0_PIN_ADDR GPIO_3_19
#define A0_PIN 19

void St7735sSPI1Init();
void St7735sPinMuxSetup();
void St7735sReset();
void St7735sInit();

void St7735sSendCmd(uint32_t cmd);
void St7735sSendData(uint32_t data, uint8_t bits);

void St7735sInversion(Boolean isInverse);

void St7735sSetRegion(uint8_t xStart, uint8_t yStart, uint8_t xEnd, uint8_t yEnd);
void St7735sSetPixel(uint8_t x, uint8_t y, uint16_t color);
void St7735sFillColor(uint16_t color);
void St7735sFillData(uint16_t *data);

#endif

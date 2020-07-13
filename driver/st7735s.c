/*
 * st7735s.c
 * @brief st7735s控制器驱动
 * Created on: Feb 12, 2020
 * Author: Yanye
 */

#include "st7735s.h"

static uint8_t DisplayWidth = 0;
static uint8_t DisplayHeight = 0;

static void writeDataByte(uint8_t data);
static void writeDataShort(uint16_t data);

static void timerDelayMs(uint32_t miliSec) {
	unsigned int countVal = (miliSec * 0x5DC0);
	DMTimerCounterSet(SOC_DMTIMER_6_REGS, 0);
	DMTimerEnable(SOC_DMTIMER_6_REGS);
	while(DMTimerCounterGet(SOC_DMTIMER_6_REGS) < countVal);
	DMTimerDisable(SOC_DMTIMER_6_REGS);
}

static void timerDelayUs(uint32_t microSec) {
    unsigned int countVal = (microSec * 0x18);
    DMTimerCounterSet(SOC_DMTIMER_6_REGS, 0);
    DMTimerEnable(SOC_DMTIMER_6_REGS);
    while(DMTimerCounterGet(SOC_DMTIMER_6_REGS) < countVal);
    DMTimerDisable(SOC_DMTIMER_6_REGS);
}

/**
 * @brief st7735s使用的spi1初始化
 * @brief spi1 ch0, tx only, int tx empty
 * */
void St7735sSPI1Init() {
	McSPI1ModuleClkConfig();
	McSPI1PinMuxSetup();
	McSPI1CSPinMuxSetup(McSPI_Channel_0);
	McSPI1SetUp(McSPI_Channel_0);

	McSPICSAssert(SOC_SPI_1_REGS, 0);
	McSPIIntEnable(SOC_SPI_1_REGS, MCSPI_INT_TX_EMPTY(0));
	McSPIChannelEnable(SOC_SPI_1_REGS, 0);

	DMTimer6ModuleClkConfig();
    DMTimerModeConfigure(SOC_DMTIMER_6_REGS, DMTIMER_ONESHOT_NOCMP_ENABLE);

}

/**
 * @brief 设置gpio3[21]-> LCD RESET, gpio3[19]-> LCD A0, should enable GPIO3 module first.
 * */
void St7735sPinMuxSetup() {
	HWREG(SOC_CONTROL_REGS + RESET_PIN_ADDR) = PAD_FS_RXD_PD_PUPDE(7);
	HWREG(SOC_CONTROL_REGS + A0_PIN_ADDR) = PAD_FS_RXD_PD_PUPDE(7);

	GPIODirModeSet(LCD_CONTROL_PIN_REGS, RESET_PIN, GPIO_DIR_OUTPUT);
	GPIODirModeSet(LCD_CONTROL_PIN_REGS, A0_PIN, GPIO_DIR_OUTPUT);
}

/**
 * @brief 复位ST7735s，时许参考datasheet，需要先初始化DMTIMER_7
 * */
void St7735sReset() {
	// 拉高复位脚稍稍延时
	GPIOPinWrite(LCD_CONTROL_PIN_REGS, RESET_PIN, GPIO_PIN_HIGH);
	timerDelayMs(1);
	// 拉低RESET，低电平持续时间最小10us
	GPIOPinWrite(LCD_CONTROL_PIN_REGS, RESET_PIN, GPIO_PIN_LOW);
	timerDelayUs(20);
	GPIOPinWrite(LCD_CONTROL_PIN_REGS, RESET_PIN, GPIO_PIN_HIGH);
	// 执行初始化必须在RESET拉高120ms后
	timerDelayMs(120);
}

/**
 * @brief st7735s控制器初始化流程，参考数据手册
 * */
void St7735sInit() {
	//Sleep exit
	St7735sSendCmd(0x11);
	timerDelayMs(120);
	//ST7735R Frame Rate
	St7735sSendCmd(0xB1);
	writeDataByte(0x01);
	writeDataByte(0x2C);
	writeDataByte(0x2D);

	St7735sSendCmd(0xB2);
	writeDataByte(0x01);
	writeDataByte(0x2C);
	writeDataByte(0x2D);

	St7735sSendCmd(0xB3);
	writeDataByte(0x01);
	writeDataByte(0x2C);
	writeDataByte(0x2D);
	writeDataByte(0x01);
	writeDataByte(0x2C);
	writeDataByte(0x2D);

	//Column inversion
	St7735sSendCmd(0xB4);
	writeDataByte(0x07);

	//ST7735R Power Sequence
	St7735sSendCmd(0xC0);
	writeDataByte(0xA2);
	writeDataByte(0x02);
	writeDataByte(0x84);

	St7735sSendCmd(0xC1);
	writeDataByte(0xC5);

	St7735sSendCmd(0xC2);
	writeDataByte(0x0A);
	writeDataByte(0x00);

	St7735sSendCmd(0xC3);
	writeDataByte(0x8A);
	writeDataByte(0x2A);

	St7735sSendCmd(0xC4);
	writeDataByte(0x8A);
	writeDataByte(0xEE);

	//VCOM
	St7735sSendCmd(0xC5);
	writeDataByte(0x0E);

	//MX, MY, RGB mode
	St7735sSendCmd(0x36);
#ifdef LCD_LANDSCAPE
	writeDataByte(0x68); //横屏68 or A8
#else
	writeDataByte(0xC8); //竖屏C8 or 08
#endif
	//ST7735R Gamma Sequence
	St7735sSendCmd(0xe0);
	writeDataByte(0x0f);
	writeDataByte(0x1a);
	writeDataByte(0x0f);
	writeDataByte(0x18);
	writeDataByte(0x2f);
	writeDataByte(0x28);
	writeDataByte(0x20);
	writeDataByte(0x22);
	writeDataByte(0x1f);
	writeDataByte(0x1b);
	writeDataByte(0x23);
	writeDataByte(0x37);
	writeDataByte(0x00);
	writeDataByte(0x07);
	writeDataByte(0x02);
	writeDataByte(0x10);

	St7735sSendCmd(0xe1);
	writeDataByte(0x0f);
	writeDataByte(0x1b);
	writeDataByte(0x0f);
	writeDataByte(0x17);
	writeDataByte(0x33);
	writeDataByte(0x2c);
	writeDataByte(0x29);
	writeDataByte(0x2e);
	writeDataByte(0x30);
	writeDataByte(0x30);
	writeDataByte(0x39);
	writeDataByte(0x3f);
	writeDataByte(0x00);
	writeDataByte(0x07);
	writeDataByte(0x03);
	writeDataByte(0x10);

	St7735sSendCmd(0x2a);
	writeDataByte(0x00);
	writeDataByte(0x00+2);
	writeDataByte(0x00);
	writeDataByte(0x80+2);

	St7735sSendCmd(0x2b);
	writeDataByte(0x00);
	writeDataByte(0x00+3);
	writeDataByte(0x00);
	writeDataByte(0x80+3);
	//Enable test command
	St7735sSendCmd(0xF0);
	writeDataByte(0x01);
	//Disable ram power save mode
	St7735sSendCmd(0xF6);
	writeDataByte(0x00);
	//65k mode
	St7735sSendCmd(0x3A);
	writeDataByte(0x05);
	//Display on
	St7735sSendCmd(0x29);
}

/**
 * @brief 设置绘制区域，可实现局部更新
 * @param xStart 0~127
 * @param yStart 0~127
 * @param xEnd 0~127
 * @param yEnd 0~127
 * */
void St7735sSetRegion(uint8_t xStart, uint8_t yStart, uint8_t xEnd, uint8_t yEnd) {
	DisplayWidth = (xEnd - xStart) + 1;
	DisplayHeight = (yEnd - yStart) + 1;

	St7735sSendCmd(0x2a);
	writeDataByte(0x00);
	writeDataByte(xStart + 1);
	writeDataByte(0x00);
	writeDataByte(xEnd + 1);

	St7735sSendCmd(0x2b);
	writeDataByte(0x00);
	writeDataByte(yStart + 2);
	writeDataByte(0x00);
	writeDataByte(yEnd + 2);

	St7735sSendCmd(0x2c);
}


/**
 * @brief 颜色按位取反
 * @param isInverse 1:反色显示  2:正常显示
 * */
void St7735sInversion(Boolean isInverse) {
	St7735sSendCmd((isInverse) ? 0x21 : 0x20);
}

/**
 * @brief 填充颜色，需要先St7735sSetRegion()
 * @param color RGB565 16位颜色
 * */
void St7735sFillColor(uint16_t color) {
	for (uint32_t i = 0; i < (DisplayWidth * DisplayHeight); i++) {
		writeDataShort(color);
	}
}

/**
 * @brief 设置某个像素颜色
 * @param x 0~127
 * @param y 0~127
 * @param color RGB565 16位颜色
 * */
void St7735sSetPixel(uint8_t x, uint8_t y, uint16_t color) {
	St7735sSetRegion(x, y, x + 1, y + 1);
	writeDataShort(color);
}

/**
 * @brief 填充数据，需要先St7735sSetRegion()
 * @param *data 16位颜色字节集
 * */
void St7735sFillData(uint16_t *data) {
	for(uint32_t i = 0; i < (DisplayWidth * DisplayHeight); i++) {
		writeDataShort(*(data + i));
	}
}

static void writeDataByte(uint8_t data) {
	St7735sSendData(data, 8);
}

static void writeDataShort(uint16_t data) {
	St7735sSendData(data, 16);
}

void St7735sSendCmd(uint32_t cmd) {
	unsigned int intCode = McSPIIntStatusGet(SOC_SPI_1_REGS);
	GPIOPinWrite(LCD_CONTROL_PIN_REGS, A0_PIN, GPIO_PIN_LOW);

	while(MCSPI_INT_TX_EMPTY(MCSPI_CHANNEL_0) != (intCode & MCSPI_INT_TX_EMPTY(MCSPI_CHANNEL_0))) {
		intCode = McSPIIntStatusGet(SOC_SPI_1_REGS);
	}
	McSPIIntStatusClear(SOC_SPI_1_REGS, MCSPI_INT_TX_EMPTY(MCSPI_CHANNEL_0));

	McSPIWordLengthSet(SOC_SPI_1_REGS, MCSPI_WORD_LENGTH(8), MCSPI_CHANNEL_0);
	McSPITransmitData(SOC_SPI_1_REGS, cmd, MCSPI_CHANNEL_0);
	// 延时300ns, 详情需使用逻辑分析仪查看
    DMTimerCounterSet(SOC_DMTIMER_6_REGS, 0);
    DMTimerCounterGet(SOC_DMTIMER_6_REGS);
}

void St7735sSendData(uint32_t data, uint8_t bits) {
	unsigned int intCode = McSPIIntStatusGet(SOC_SPI_1_REGS);
	GPIOPinWrite(LCD_CONTROL_PIN_REGS, A0_PIN, GPIO_PIN_HIGH);

	while(MCSPI_INT_TX_EMPTY(MCSPI_CHANNEL_0) != (intCode & MCSPI_INT_TX_EMPTY(MCSPI_CHANNEL_0))) {
		intCode = McSPIIntStatusGet(SOC_SPI_1_REGS);
	}
	McSPIIntStatusClear(SOC_SPI_1_REGS, MCSPI_INT_TX_EMPTY(MCSPI_CHANNEL_0));

	McSPIWordLengthSet(SOC_SPI_1_REGS, MCSPI_WORD_LENGTH(bits), MCSPI_CHANNEL_0);
	McSPITransmitData(SOC_SPI_1_REGS, data, MCSPI_CHANNEL_0);
	// 延时300ns
	DMTimerCounterSet(SOC_DMTIMER_6_REGS, 0);
	DMTimerCounterGet(SOC_DMTIMER_6_REGS);
}


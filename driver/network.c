/*
 * network.h
 * @brief TCP通讯网络通用接口，在回调函数中不要执行耗时操作
 * Created on: Apr 2, 2020
 * Author: Yanye
 */
#include "network.h"

static void init(uint32_t spiPriority, uint32_t gpioPriority);
static Boolean tcpSend(uint8_t *data, uint32_t length);
static void registerTcpRecvCallback(TcpRecvCallback callback);
static void registerTcpSentCallback(TcpSentCallback callback);
static void removeTcpRecvCallback();
static void removeTcpSentCallback();

static void mcspiAssert();
static void mcspiTransmit(uint8_t *data, uint8_t len);
static void mcspiReceive();
static void mcspiISR();
static void txrxISR();

static NetWork *instance = NULL;

static TcpRecvCallback tcpRecvCallback = NULL;
static TcpSentCallback tcpSentCallback = NULL;

static volatile uint8_t hspi_recving = FALSE;
static volatile uint8_t hspi_sending = FALSE;

static volatile uint32_t length;
static uint8_t *spi_rx_buffer;
static uint8_t *spi_tx_buffer;
static uint8_t *ptr_rx;
static uint8_t *ptr_tx;

NetWork *networkInstance() {
    if(NULL == instance) {
        instance = (NetWork *)malloc(sizeof(NetWork));
        memset((uint8_t *)instance, 0x00, sizeof(NetWork));
        instance->init = init;
        instance->tcpSend = tcpSend;
        instance->registerTcpRecvCallback = registerTcpRecvCallback;
        instance->registerTcpSentCallback = registerTcpSentCallback;
        instance->removeTcpRecvCallback = removeTcpRecvCallback;
        instance->removeTcpSentCallback = removeTcpSentCallback;
    }
    return instance;
}

/**
 * @brief 初始化
 * @param priority MCSPI0的优先级
 * @param gpioPriority TX RX中断线优先级
 * */
static void init(uint32_t spiPriority, uint32_t gpioPriority) {
	spi_tx_buffer = (uint8_t *)malloc(sizeof(uint8_t) * (HSPI_PACKAGE_SIZE + HSPI_HEADER_SIZE + 1));
	spi_rx_buffer = (uint8_t *)malloc(sizeof(uint8_t) * (HSPI_PACKAGE_SIZE + HSPI_HEADER_SIZE + 1));

	// config gpop pinmux
	HWREG(SOC_CONTROL_REGS + SPI_TX_INT_PIN_ADDR) = PAD_SL_RXE_PU_PUPDE(7);
	GPIODirModeSet(SOC_GPIO_0_REGS, SPI_TX_INT_PIN, GPIO_DIR_INPUT);
	HWREG(SOC_CONTROL_REGS + SPI_RX_INT_PIN_ADDR) = PAD_SL_RXE_PU_PUPDE(7);
	GPIODirModeSet(SOC_GPIO_0_REGS, SPI_RX_INT_PIN, GPIO_DIR_INPUT);

	// init gpio interrupt
	IntRegister(SYS_INT_GPIOINT0A, txrxISR);
	IntPrioritySet(SYS_INT_GPIOINT0A, gpioPriority, AINTC_HOSTINT_ROUTE_IRQ);
	IntSystemEnable(SYS_INT_GPIOINT0A);

	GPIOIntTypeSet(SOC_GPIO_0_REGS, SPI_TX_INT_PIN, GPIO_INT_TYPE_NO_LEVEL);
	GPIOIntTypeSet(SOC_GPIO_0_REGS, SPI_TX_INT_PIN, GPIO_INT_TYPE_FALL_EDGE);
	GPIOPinIntEnable(SOC_GPIO_0_REGS, GPIO_INT_LINE_1, SPI_TX_INT_PIN);

	GPIOIntTypeSet(SOC_GPIO_0_REGS, SPI_RX_INT_PIN, GPIO_INT_TYPE_NO_LEVEL);
	GPIOIntTypeSet(SOC_GPIO_0_REGS, SPI_RX_INT_PIN, GPIO_INT_TYPE_FALL_EDGE);
	GPIOPinIntEnable(SOC_GPIO_0_REGS, GPIO_INT_LINE_1, SPI_RX_INT_PIN);

	// init mcspi0 channel 0
	McSPI0ModuleClkConfig();
	McSPI0PinMuxSetup();
	McSPI0CSPinMuxSetup();

	McSPI0AintcConfigure(mcspiISR, spiPriority);
	McSPI0SetUp();
}

/**
 * @brief 通过TCP发送数据，通过TcpSentCallback回调通知发送完成
 * @brief 单次最多发32字节，超出部分将丢弃
 * @param *data 数据指针
 * @param length 数据长度
 * @return TRUE 数据已发送，FALSE SPI接口忙，数据发送失败。判定数据是否送达请使用TcpSentCallback回调
 * */
static Boolean tcpSend(uint8_t *data, uint32_t length) {
	if(hspi_sending) {
		return FALSE;
	}
	uint32_t size = (length > HSPI_PACKAGE_SIZE) ? HSPI_PACKAGE_SIZE : length;
	hspi_sending = TRUE;
	mcspiTransmit(data, size);
	return TRUE;
}

static void mcspiTransmit(uint8_t *data, uint8_t len) {
	spi_tx_buffer[0] = ESP8266_WRITE_CMD;
	spi_tx_buffer[1] = ESP8266_DUMMY_ADDRESS;
	memcpy((spi_tx_buffer + 2), data, len);
    length = (HSPI_PACKAGE_SIZE + HSPI_HEADER_SIZE);
	mcspiAssert();
}

static void mcspiReceive() {
	spi_tx_buffer[0] = ESP8266_READ_CMD;
	spi_tx_buffer[1] = ESP8266_DUMMY_ADDRESS;
    length = (HSPI_PACKAGE_SIZE + HSPI_HEADER_SIZE + 1);
	mcspiAssert();
}

static void mcspiAssert() {
    ptr_tx = spi_tx_buffer;
    ptr_rx = spi_rx_buffer;
    McSPICSAssert(SOC_SPI_0_REGS, MCSPI_CHANNEL_0);
    McSPIIntEnable(SOC_SPI_0_REGS, MCSPI_INT_TX_EMPTY(MCSPI_CHANNEL_0) | MCSPI_INT_RX_FULL(MCSPI_CHANNEL_0));
    McSPIChannelEnable(SOC_SPI_0_REGS, MCSPI_CHANNEL_0);
}

static void mcspiISR(void) {
    unsigned int intCode = McSPIIntStatusGet(SOC_SPI_0_REGS);
    if(intCode) {
        if(MCSPI_INT_TX_EMPTY(MCSPI_CHANNEL_0) == (intCode & MCSPI_INT_TX_EMPTY(MCSPI_CHANNEL_0)) ||
        		MCSPI_INT_RX_FULL(MCSPI_CHANNEL_0) == (intCode & MCSPI_INT_RX_FULL(MCSPI_CHANNEL_0)))  {
            McSPIIntStatusClear(SOC_SPI_0_REGS, MCSPI_INT_TX_EMPTY(MCSPI_CHANNEL_0));
            McSPIIntStatusClear(SOC_SPI_0_REGS, MCSPI_INT_RX_FULL(MCSPI_CHANNEL_0));
            length--;
            McSPITransmitData(SOC_SPI_0_REGS, (unsigned int)(*ptr_tx++), MCSPI_CHANNEL_0);
			*ptr_rx++ = (unsigned char)McSPIReceiveData(SOC_SPI_0_REGS, MCSPI_CHANNEL_0);
            if(!length) {
            	hspi_sending = FALSE;
                McSPIIntDisable(SOC_SPI_0_REGS, MCSPI_INT_TX_EMPTY(MCSPI_CHANNEL_0));
                McSPICSDeAssert(SOC_SPI_0_REGS, MCSPI_CHANNEL_0);
				McSPIChannelDisable(SOC_SPI_0_REGS, MCSPI_CHANNEL_0);
				if(!hspi_recving) {
					return;
				}
				hspi_recving = FALSE;
				if(tcpRecvCallback != NULL) {
					tcpRecvCallback(spi_rx_buffer + 3, HSPI_PACKAGE_SIZE);
				}
            }
        }
    }
}

static void txrxISR() {
	if(GPIOPinIntStatus(SOC_GPIO_0_REGS, GPIO_INT_LINE_1, SPI_TX_INT_PIN) & bit(SPI_TX_INT_PIN)) {
		GPIOPinIntClear(SOC_GPIO_0_REGS, GPIO_INT_LINE_1, SPI_TX_INT_PIN);
		if(!GPIOPinRead(SOC_GPIO_0_REGS, SPI_TX_INT_PIN)) {
			if(tcpSentCallback != NULL) {
				tcpSentCallback();
			}
		}
	}else if(GPIOPinIntStatus(SOC_GPIO_0_REGS, GPIO_INT_LINE_1, SPI_RX_INT_PIN) & bit(SPI_RX_INT_PIN)) {
		GPIOPinIntClear(SOC_GPIO_0_REGS, GPIO_INT_LINE_1, SPI_RX_INT_PIN);
		if(!GPIOPinRead(SOC_GPIO_0_REGS, SPI_RX_INT_PIN)) {
			hspi_recving = TRUE;
			mcspiReceive();
		}
	}
}

/**
 * @brrief 注册TCP接收回调,底层接收完整数据后执行该回调函数
 * @param callback TcpRecvCallback
 * */
static void registerTcpRecvCallback(TcpRecvCallback callback) {
	tcpRecvCallback = callback;
}

/**
 * @brrief 注册TCP发送完成回调,底层发送完整数据后执行该回调函数
 * @param callback TcpRecvCallback
 * */
static void registerTcpSentCallback(TcpSentCallback callback) {
	tcpSentCallback = callback;
}

/**
 * @brief 移除TCP接收回调
 * */
static void removeTcpRecvCallback() {
	tcpRecvCallback = NULL;
}

/**
 * @brief 移除TCP发送完成回调
 * */
static void removeTcpSentCallback() {
	tcpSentCallback = NULL;
}


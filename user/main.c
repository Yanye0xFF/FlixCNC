#include "main.h"

static volatile uint8_t isRecvData = FALSE;
static uint8_t recvBuffer[HSPI_PACKAGE_SIZE];
static ByteStream *stream = NULL;

static NetWork *network;
static Frame *frame = NULL;

static void displayTimerIsr();
static void displayInit(uint32_t frameRate);
static void startDisplayLoop();

static void esp8266Reset();
static void hc89sReset();
static void openSystemLed();

static void tcpRecvCallback(uint8_t *data, uint32_t maxLength);

int main() {

	MMUConfigAndEnable();
	CacheEnable(CACHE_ICACHE);

	IntMasterIRQEnable();
	IntAINTCInit();

	ConsoleUtilsInit();
	ConsoleUtilsSetType(CONSOLE_UART);

	SysDelayTimerSetup(DEALY_TIMER_PRIORITY);
	SysdelayMs(300);
	ConsoleUtilsPrintf("Hello World\n");

    // gpio外设初始化复位
    System *system = systemInstance();
    system->gpioModelSetup();

    // 首先复位主轴控制器, 安全考虑
    hc89sReset();

    // tf卡, fatfs文件系统
    HSMMCSDPinMuxSetup();
    HSMMCSDModuleClkConfig();
    HSMMCSDInit();

    // 显示屏, 刷新定时器
	displayInit(20);
	// init frame
	frame = contextInstance()->prepareFrame();
	startDisplayLoop();

	ProgressBar *progressBar = createBootView(frame);

	// 配置主轴控制器i2c接口
	Spindle *spindle = spindleInstanec();
	spindle->setupSpindleI2CPort();
    SysdelayMs(100);
    progressBar->setProgress(20, progressBar);

	// 复位ESP8266
	esp8266Reset();
    progressBar->setProgress(60, progressBar);

    // 网络
	network = networkInstance();
	network->registerTcpRecvCallback(tcpRecvCallback);
	network->init(NETWORK_PRIORITY, NETWORK_NOTIFY_PRIORITY);
    SysdelayMs(100);
    progressBar->setProgress(80, progressBar);

	// 设置步进驱动器和限位传感器引脚复用
	system->pinmuxSetup();

    // 初始化planner和stepper
	Planner *planner = plannerInstance();
	planner->init();
	Stepper *stepper = stepperInstance();
	stepper->init();
	progressBar->setProgress(100, progressBar);

	View *view = frame->findViewById(frame, TEXTVIEW, 1);
	if((NULL != view) && (NULL != view->entity)) {
	    TextView *tvHint = (TextView *)view->entity;
	    tvHint->setText((uint8_t *)TEXT_BOOT_FINISH, tvHint);
	}

	// app连接扫码界面
    SysdelayMs(150);
    frame->removeAllView(frame);
    createConnView(frame);
    openSystemLed();

	uint8_t command = 0x00;
	stream = newByteStream();
	while(1) {
		if(isRecvData == TRUE) {
			command = *(recvBuffer + 0);
		    ConsoleUtilsPrintf("command: 0x%x\n", command);
			if(command <= COMMEN_GROUP_END) {
			    processCommCmd(recvBuffer, frame);
			}else if(command >= PARAMS_GROUP_START && command <= PARAMS_GROUP_END) {
			    processParamCmd(recvBuffer);
			}else if(command >= MOTION_GROUP_START && command <= MOTION_GROUP_END) {
			    processMotionCmd(recvBuffer);
			}else if(command >= FILE_GROUP_START && command <= FILE_GROUP_END) {
			    processTransCmd(recvBuffer, stream);
            }
            isRecvData = FALSE;
		}
	}
}

/**
 * @brief 定时器刷新显示回调
 * */
static void displayTimerIsr() {
    DMTimerIntDisable(SOC_DMTIMER_2_REGS, DMTIMER_INT_OVF_EN_FLAG);
    DMTimerIntStatusClear(SOC_DMTIMER_2_REGS, DMTIMER_INT_OVF_IT_FLAG);
    frame->invalidate(frame);
    DMTimerIntEnable(SOC_DMTIMER_2_REGS, DMTIMER_INT_OVF_EN_FLAG);
}

/**
 * @brief 初始化GUI部分
 * @param frameRate 显示屏刷新率 1~30, 不在规定范围内默认取FRAME_RATE_DEFAULT
 * */
static void displayInit(uint32_t frameRate) {
    if(frameRate <= 0 || frameRate > 30) {
        frameRate = FRAME_RATE_DEFAULT;
    }
    frameRate = (1000 / frameRate) * 0x5DC0;

    IntRegister(SYS_INT_TINT2, displayTimerIsr);
    IntPrioritySet(SYS_INT_TINT2, GUI_TIMER_PRIORITY, AINTC_HOSTINT_ROUTE_IRQ);
    IntSystemEnable(SYS_INT_TINT2);

    DMTimer2ModuleClkConfig();
    DMTimerDisable(SOC_DMTIMER_2_REGS);
    DMTimerCounterSet(SOC_DMTIMER_2_REGS, (0xFFFFFFFF - frameRate));
    DMTimerReloadSet(SOC_DMTIMER_2_REGS, (0xFFFFFFFF - frameRate));
    DMTimerModeConfigure(SOC_DMTIMER_2_REGS, DMTIMER_AUTORLD_NOCMP_ENABLE);

    St7735sSPI1Init();
    St7735sPinMuxSetup();
    St7735sReset();
    St7735sInit();
}

static void startDisplayLoop() {
    DMTimerIntEnable(SOC_DMTIMER_2_REGS, DMTIMER_INT_OVF_EN_FLAG);
    DMTimerEnable(SOC_DMTIMER_2_REGS);
}

static void esp8266Reset() {
	// 初始化复位与片选脚
	HWREG(SOC_CONTROL_REGS + ESP8266_RST_PIN_ADDR) = PAD_FS_RXD_PU_PUPDE(7);
	HWREG(SOC_CONTROL_REGS + ESP8266_CS_PIN_ADDR) = PAD_FS_RXD_PU_PUPDE(7);
	GPIODirModeSet(SOC_GPIO_1_REGS, ESP8266_RST_PIN, GPIO_DIR_OUTPUT);
	GPIODirModeSet(SOC_GPIO_0_REGS, ESP8266_CS_PIN, GPIO_DIR_OUTPUT);
	// 拉低cs->拉低rst->延时100ms->拉高rst->等待1s->拉高cs
	GPIOPinWrite(SOC_GPIO_0_REGS, ESP8266_CS_PIN, GPIO_PIN_LOW);
	GPIOPinWrite(SOC_GPIO_1_REGS, ESP8266_RST_PIN, GPIO_PIN_LOW);

	SysdelayMs(100);
	GPIOPinWrite(SOC_GPIO_1_REGS, ESP8266_RST_PIN, GPIO_PIN_HIGH);

    SysdelayMs(1000);
	GPIOPinWrite(SOC_GPIO_0_REGS, ESP8266_CS_PIN, GPIO_PIN_HIGH);
}

static void hc89sReset() {
    HWREG(SOC_CONTROL_REGS + SPINDLE_RST_PIN_ADDR) = PAD_SL_RXD_PU_PUPDE(7);
    GPIODirModeSet(SOC_GPIO_0_REGS, SPINDLE_RST_PIN, GPIO_DIR_OUTPUT);
    // 拉低复位脚100ms
    GPIOPinWrite(SOC_GPIO_0_REGS, SPINDLE_RST_PIN, GPIO_PIN_LOW);
    SysdelayMs(100);
    GPIOPinWrite(SOC_GPIO_0_REGS, SPINDLE_RST_PIN, GPIO_PIN_HIGH);
}

static void openSystemLed() {
    HWREG(SOC_CONTROL_REGS + SYSTEM_LED_PIN_ADDR) = PAD_SL_RXD_PU_PUPDE(7);
    GPIODirModeSet(SOC_GPIO_0_REGS, SYSTEM_LED_PIN, GPIO_DIR_OUTPUT);
    // 拉低复位脚100ms
    GPIOPinWrite(SOC_GPIO_0_REGS, SYSTEM_LED_PIN, GPIO_PIN_LOW);
}

static void tcpRecvCallback(uint8_t *data, uint32_t maxLength) {
	memcpy(recvBuffer, data, maxLength);
	isRecvData = TRUE;
}

/*
 * i2c_port2.c
 * @brief 
 * Created on: Apr 25, 2020
 * Author: Yanye
 */

#include "i2c_port2.h"

static void I2C2ModuleClkConfig();
static void I2C2PinMuxSetup();

static void I2C2Transmission();
static void CleanUpInterrupts();
static void I2CIsr();

// Configures AINTC to generate interrupt
void I2C2AINTCConfigure() {
    IntRegister(SYS_INT_I2C2INT, I2CIsr);
    IntPrioritySet(SYS_INT_I2C2INT, SPINDLE_CONTROLLER_PRIORITY, AINTC_HOSTINT_ROUTE_IRQ);
    IntSystemEnable(SYS_INT_I2C2INT);
}
//  1/78
//  2 /18
void SetupI2C2() {
	I2C2ModuleClkConfig();
	I2C2PinMuxSetup();
	// Put i2c in reset/disabled state
	I2CMasterDisable(SOC_I2C_2_REGS);
	// Disable auto Idle functionality
	I2CAutoIdleDisable(SOC_I2C_2_REGS);
	// Configure i2c bus speed to 50khz
	I2CMasterInitExpClk(SOC_I2C_2_REGS, I2C_SYS_CLK_48MHZ, I2C_INTERNAL_CLK_12MHZ, I2C_BUS_SPEED);
	// Set i2c slave address
	I2CMasterSlaveAddrSet(SOC_I2C_2_REGS, I2C_SLAVE_ADDR);
	// Bring I2C out of reset
	I2CMasterEnable(SOC_I2C_2_REGS);

}

static uint8_t i2c_tx_buffer[I2C_BUFFER_SIZE];
static volatile uint32_t i2c_tx_cursor;
static uint32_t i2c_tx_length;

void I2C2Send(uint8_t *data, uint32_t length) {
	length = (length > I2C_BUFFER_SIZE) ? I2C_BUFFER_SIZE : length;
	memcpy(i2c_tx_buffer, data, length);
	i2c_tx_length = length;
	I2C2Transmission();
}

/**
 * This function will configure the required clocks for I2C2 instance.
 */
static void I2C2ModuleClkConfig() {

    HWREG(SOC_PRCM_REGS + CM_PER_L3S_CLKSTCTRL) |= CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L3S_CLKSTCTRL) &
     CM_PER_L3S_CLKSTCTRL_CLKTRCTRL) != CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKSTCTRL) |= CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKSTCTRL) &
     CM_PER_L3_CLKSTCTRL_CLKTRCTRL) != CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_PRCM_REGS + CM_PER_L3_INSTR_CLKCTRL) |= CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L3_INSTR_CLKCTRL) &
                               CM_PER_L3_INSTR_CLKCTRL_MODULEMODE) !=
                                   CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE);

    HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKCTRL) |= CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKCTRL) &
        CM_PER_L3_CLKCTRL_MODULEMODE) != CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE);

    HWREG(SOC_PRCM_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) |= CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_PRCM_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
                              CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL) !=
                                CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKSTCTRL) |= CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKSTCTRL) &
                             CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL) !=
                               CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKCTRL) |= CM_PER_L4LS_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKCTRL) &
      CM_PER_L4LS_CLKCTRL_MODULEMODE) != CM_PER_L4LS_CLKCTRL_MODULEMODE_ENABLE);

    HWREG(SOC_PRCM_REGS + CM_PER_I2C2_CLKCTRL) |= CM_PER_I2C2_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_PRCM_REGS + CM_PER_I2C2_CLKCTRL) &
      CM_PER_I2C2_CLKCTRL_MODULEMODE) != CM_PER_I2C2_CLKCTRL_MODULEMODE_ENABLE);

    while(!(HWREG(SOC_PRCM_REGS + CM_PER_L3S_CLKSTCTRL) &
            CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK));

    while(!(HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKSTCTRL) &
            CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK));

    while(!(HWREG(SOC_PRCM_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
           (CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK |
            CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L4_GCLK)));

    while(!(HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKSTCTRL) &
           (CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_L4LS_GCLK |
            CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_I2C_FCLK)));
}

static void I2C2PinMuxSetup() {
	GPIO0ModuleClkConfig();
	GPIOModuleEnable(SOC_GPIO_0_REGS);
	GPIOModuleReset(SOC_GPIO_0_REGS);

	HWREG(SOC_CONTROL_REGS + CONTROL_CONF_I2C2_SDA) = PAD_SL_RXE_PU_PUPDE(3);
	HWREG(SOC_CONTROL_REGS + CONTROL_CONF_I2C2_SCL) = PAD_SL_RXE_PU_PUPDE(3);
}

static void I2C2Transmission() {
	// reset i2c transmit buffer cursor
	i2c_tx_cursor = 0;
	// Data Count specifies the number of bytes to be transmitted
	I2CSetDataCount(SOC_I2C_2_REGS, i2c_tx_length);
	// Clear status of all interrupts
	CleanUpInterrupts();
    // Configure I2C controller in Master Transmitter mode
    I2CMasterControl(SOC_I2C_2_REGS, I2C_CFG_MST_TX);
    // Transmit interrupt is enabled
	I2CMasterIntEnableEx(SOC_I2C_2_REGS, I2C_INT_TRANSMIT_READY);
	// Generate Start Condition over I2C bus
	I2CMasterStart(SOC_I2C_2_REGS);

	//ConsoleUtilsPrintf("I2C2Transmission\n");
	//while(I2CMasterBusBusy(SOC_I2C_2_REGS) == 0);
	//ConsoleUtilsPrintf("I2CMasterBusBusy == 0\n");
}

// Clear status of all interrupts
static void CleanUpInterrupts() {
    I2CMasterIntEnableEx(SOC_I2C_2_REGS, 0x7FF);
    I2CMasterIntClearEx(SOC_I2C_2_REGS,  0x7FF);
    I2CMasterIntDisableEx(SOC_I2C_2_REGS, 0x7FF);
}

/**
 * @brief I2C Interrupt Service Routine. This function will write data through I2C bus.
 * */
void I2CIsr(void) {
    // Get only Enabled interrupt status
    uint32_t status = I2CMasterIntStatus(SOC_I2C_2_REGS);

    //ConsoleUtilsPrintf("I2CIsr state: 0x%x\n", status);
    // Clear all enabled interrupt status except receive ready and transmit ready interrupt status
    I2CMasterIntClearEx(SOC_I2C_2_REGS, (status & ~(I2C_INT_RECV_READY | I2C_INT_TRANSMIT_READY)));

    if(status & I2C_INT_TRANSMIT_READY) {
        // Put data to data transmit register of i2c
        I2CMasterDataPut(SOC_I2C_2_REGS, i2c_tx_buffer[i2c_tx_cursor++]);
        // Clear Transmit interrupt status
        I2CMasterIntClearEx(SOC_I2C_2_REGS, I2C_INT_TRANSMIT_READY);
        //ConsoleUtilsPrintf("I2C_INT_TRANSMIT_READY\n");
        if(i2c_tx_cursor >= i2c_tx_length) {
            // Disable the transmit ready interrupt
            I2CMasterIntDisableEx(SOC_I2C_2_REGS, I2C_INT_TRANSMIT_READY);
            // Generate a STOP
            I2CMasterStop(SOC_I2C_2_REGS);
            //ConsoleUtilsPrintf("I2CMasterStop\n");
        }
    }

    if(status & I2C_INT_NO_ACK) {
         I2CMasterIntDisableEx(SOC_I2C_2_REGS, (I2C_INT_TRANSMIT_READY | I2C_INT_NO_ACK));
         // Generate a STOP
         I2CMasterStop(SOC_I2C_2_REGS);
         //ConsoleUtilsPrintf("I2C_INT_NO_ACK\n");
    }
}





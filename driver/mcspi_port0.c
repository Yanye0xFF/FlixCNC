#include "mcspi_port0.h"

void McSPI0AintcConfigure(IntHandler handler, unsigned int priority) {
    IntRegister(SYS_INT_SPI0INT, handler);
    IntPrioritySet(SYS_INT_SPI0INT, priority, AINTC_HOSTINT_ROUTE_IRQ);
    IntSystemEnable(SYS_INT_SPI0INT);
}

void McSPI0SetUp() {
    McSPIReset(SOC_SPI_0_REGS);
    McSPICSEnable(SOC_SPI_0_REGS);
    McSPIMasterModeEnable(SOC_SPI_0_REGS);

    McSPIMasterModeConfig(SOC_SPI_0_REGS, MCSPI_SINGLE_CH, MCSPI_TX_RX_MODE, MCSPI_DATA_LINE_COMM_MODE_1, 0);
    McSPIClkConfig(SOC_SPI_0_REGS, MCSPI_IN_CLK, MCSPI0_OUT_FREQ, 0, MCSPI_CLK_MODE_0);

    McSPIWordLengthSet(SOC_SPI_0_REGS, MCSPI_WORD_LENGTH(8), 0);
    McSPICSPolarityConfig(SOC_SPI_0_REGS, MCSPI_CS_POL_LOW, 0);

    McSPITxFIFOConfig(SOC_SPI_0_REGS, MCSPI_TX_FIFO_ENABLE, 0);
    McSPIRxFIFOConfig(SOC_SPI_0_REGS, MCSPI_RX_FIFO_ENABLE, 0);
}

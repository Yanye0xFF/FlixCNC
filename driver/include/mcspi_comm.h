#ifndef _MCSPI_COMM_H_
#define _MCSPI_COMM_H_

#define CONTROL_CONF_SPI1_SCLK  0x990
#define CONTROL_CONF_SPI1_D0    0x994
#define CONTROL_CONF_SPI1_D1    0x998

#define CONTROL_CONF_SPI1_CS0   0x99C
#define CONTROL_CONF_SPI1_CS1   0x97C
// 2019手册第4000页左右
// SPI0->ESP8266
#define MCSPI0_OUT_FREQ          (16000000u)
// SPI1->ST7735s
#define MCSPI1_OUT_FREQ          (16000000u)
#define MCSPI_IN_CLK            (48000000u)

typedef enum _mcspi_channel {
    McSPI_Channel_0 = 0,
    McSPI_Channel_1 = 1
} McSPI_Channel;

typedef void (*IntHandler)(void);

#endif

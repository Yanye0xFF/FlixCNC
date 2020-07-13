/*
 * mcspi_conf.h
 *  Created on: Feb 23, 2020
 *      Author: Yanye
 */

#ifndef _MCSPI_CONF_H_
#define _MCSPI_CONF_H_

#include "hw_control_AM335x.h"
#include "soc_AM335x.h"
#include "interrupt.h"
#include "hw_cm_per.h"
#include "hw_types.h"
#include "mcspi.h"
#include "stdint.h"

#include "../../driver/include/mcspi_comm.h"

void McSPI1ModuleClkConfig();

void McSPI1PinMuxSetup();
void McSPI1CSPinMuxSetup(McSPI_Channel channel);
void McSPI1SetUp(McSPI_Channel channel);

void McSPI1AintcConfigure(IntHandler handler, uint32_t priority);

#endif /* USER_MCSPI_CONF_H_ */

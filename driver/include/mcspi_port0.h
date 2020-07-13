/*
 * mcspi_conf.h
 * Created on: Feb 23, 2020
 *      Author: Yanye
 */

#ifndef _MCSPI0_CONF_H_
#define _MCSPI0_CONF_H_

#include "hw_control_AM335x.h"
#include "soc_AM335x.h"
#include "interrupt.h"
#include "hw_cm_per.h"
#include "hw_types.h"
#include "mcspi.h"

#include "../../driver/include/mcspi_comm.h"

void McSPI0SetUp();
void McSPI0AintcConfigure(IntHandler handler, unsigned int priority);

#endif /* USER_MCSPI_CONF_H_ */

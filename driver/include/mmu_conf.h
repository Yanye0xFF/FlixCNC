/*
 * mmu_config.h
 *
 *  Created on: Feb 3, 2020
 *      Author: Yanye
 */

#ifndef _MMU_CONF_H_
#define _MMU_CONF_H_

#include "mmu.h"

/*
 * @参考2019版技术手册 AM335x and AMIC110 Sitara™ Processors Technical Reference Manual- ARM Cortex-A8 Memory Map
 * */
// EMIF0 SDRAM 0x8000_0000 0xBFFF_FFFF 1GB 8-/16-bit External Memory (Ex/R/W)(2)
#define START_ADDR_DDR                  (0x80000000u)
// device address start from "0x4400_0000" to "0x7FFF_FFFF", some address is Reserved
#define START_ADDR_DEV                  (0x44000000u)
// L3 OCMC0 0x4030_0000 ~ 0x4030_FFFF 64KB 32-bit Ex/R/W(2) OCMC SRAM
#define START_ADDR_OCMC                 (0x40300000u)
// @brief 矿板BB_Black_V1.6 DDR3 256MB, 在此使用1024/512/256都可，外部DDR3有1G的地址空间，详情参考手册
#define NUM_SECTIONS_DDR                (512u)
#define NUM_SECTIONS_DEV                (960u)
#define NUM_SECTIONS_OCMC               (1u)

/*
** Function to setup MMU. This function Maps three regions ( 1. DDR
** 2. OCMC and 3. Device memory) and enables MMU.
*/
void MMUConfigAndEnable();



#endif /* USER_MMU_CONF_H_ */

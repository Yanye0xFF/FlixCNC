#ifndef _SDCARD_CONF_H_
#define _SDCARD_CONF_H_

#include "stdint.h"
#include "stdlib.h"
#include "delay.h"

#include "soc_AM335x.h"
#include "ff.h"
#include "hs_mmcsd.h"
#include "mmcsd_proto.h"
#include "hs_mmcsdlib.h"
#include "string.h"
#include "diskio.h"
#include "consoleUtils.h"

#define PATH_BUF_SIZE       512
#define DATA_BUF_SIZE       512
#define MMCSD_BLK_SIZE      512
#define MMCSD_IN_FREQ       96000000 /* 96MHz */
#define MMCSD_INIT_FREQ     400000   /* 400kHz */
#define MMCSD_OCR           (SD_OCR_VDD_3P0_3P1 | SD_OCR_VDD_3P1_3P2)
#define HSMMCSD_INT_STATUS_FLAG    (0xFFFFFFFF)
#define MMCSD_BASE                 SOC_MMCHS_0_REGS

uint8_t HSMMCSDInit();
uint8_t *loadFile(char *fname);

#endif

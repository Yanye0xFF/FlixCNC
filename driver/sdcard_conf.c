#include "sdcard_conf.h"

#pragma DATA_ALIGN(sdCard, 32);
mmcsdCardInfo sdCard;
#pragma DATA_ALIGN(ctrlInfo, 32);
mmcsdCtrlInfo ctrlInfo;

static FATFS g_sFatFs;

static volatile unsigned int xferPend = 0;
static volatile unsigned int cmdCompFlag = 0;
static volatile unsigned int cmdTimeout = 0;
static volatile unsigned int errFlag = 0;
static volatile unsigned int sdBlkSize = MMCSD_BLK_SIZE;
static unsigned int hsmmcsd_dataLen = 0;
static volatile unsigned char *hsmmcsd_buffer = NULL;

extern FatDevice fat_devices[2];

static void HSMMCSDControllerSetup();
static void HSMMCSDXferSetup(mmcsdCtrlInfo *ctrl, uint8_t rwFlag, void *ptr, uint32_t blkSize, uint32_t nBlks);
static unsigned int HSMMCSDCmdStatusGet(mmcsdCtrlInfo *ctrl);
static unsigned int HSMMCSDStatusGet();
static unsigned int HSMMCSDXferStatusGet(mmcsdCtrlInfo *ctrl);

static void HSMMCSDControllerSetup();
static void HSMMCSDFsMount(uint32_t driveNum, void *ptr);

/**
 * detect sdcard
 * @return 0: no sdcard, 1: card is present
 * */
uint8_t HSMMCSDInit() {
	uint32_t counter = 0;
    // Basic controller initializations
    HSMMCSDControllerSetup();
    // First check, if card is insterted
    while(1) {
    	// retry 3 times to detect sdcard
    	if(counter > 3) {
    		return 0;
    	}
        if(MMCSDCardPresent(&ctrlInfo)){
        	break;
        }
        counter++;
        SysdelayMs(500);
    }
    // Initialize the MMCSD controller
    MMCSDCtrlInit(&ctrlInfo);
    MMCSDIntEnable(&ctrlInfo);

    HSMMCSDFsMount(0, &sdCard);
    return 1;
}

static void HSMMCSDControllerSetup() {
    ctrlInfo.memBase = MMCSD_BASE;
    ctrlInfo.ctrlInit = HSMMCSDControllerInit;
    ctrlInfo.xferSetup = HSMMCSDXferSetup;
    ctrlInfo.cmdStatusGet = HSMMCSDCmdStatusGet;
    ctrlInfo.xferStatusGet = HSMMCSDXferStatusGet;
    ctrlInfo.cardPresent = HSMMCSDCardPresent;
    ctrlInfo.cmdSend = HSMMCSDCmdSend;
    ctrlInfo.busWidthConfig = HSMMCSDBusWidthConfig;
    ctrlInfo.busFreqConfig = HSMMCSDBusFreqConfig;
    ctrlInfo.intrMask = (HS_MMCSD_INTR_CMDCOMP | HS_MMCSD_INTR_CMDTIMEOUT |
                            HS_MMCSD_INTR_DATATIMEOUT | HS_MMCSD_INTR_TRNFCOMP);
    ctrlInfo.intrEnable = HSMMCSDIntEnable;
    ctrlInfo.busWidth = (SD_BUS_WIDTH_1BIT | SD_BUS_WIDTH_4BIT);
    ctrlInfo.highspeed = 1;
    ctrlInfo.ocr = MMCSD_OCR;
    ctrlInfo.card = &sdCard;
    ctrlInfo.ipClk = MMCSD_IN_FREQ;
    ctrlInfo.opClk = MMCSD_INIT_FREQ;
    sdCard.ctrl = &ctrlInfo;

    cmdCompFlag = 0;
    cmdTimeout = 0;
}

static void HSMMCSDFsMount(uint32_t driveNum, void *ptr) {

	FRESULT result = f_mount(driveNum, &g_sFatFs);
	if(result == FR_OK) {
		fat_devices[driveNum].dev = ptr;
		fat_devices[driveNum].fs = &g_sFatFs;
	}
}

static void HSMMCSDXferSetup(mmcsdCtrlInfo *ctrl, uint8_t rwFlag, void *ptr, uint32_t blkSize, uint32_t nBlks) {
    HSMMCSDIntrStatusClear(ctrl->memBase, HS_MMCSD_INTR_TRNFCOMP);
    if (rwFlag == 1) {
        HSMMCSDIntrStatusClear(ctrl->memBase, HS_MMCSD_INTR_BUFRDRDY);
        HSMMCSDIntrStatusEnable(ctrl->memBase, HS_MMCSD_INTR_BUFRDRDY);
        HSMMCSDIntrStatusDisable(ctrl->memBase, HS_MMCSD_INTR_BUFWRRDY);
    }else {
        HSMMCSDIntrStatusClear(ctrl->memBase, HS_MMCSD_INTR_BUFWRRDY);
        HSMMCSDIntrStatusEnable(ctrl->memBase, HS_MMCSD_INTR_BUFWRRDY);
        HSMMCSDIntrStatusDisable(ctrl->memBase, HS_MMCSD_INTR_BUFRDRDY);
    }
    HSMMCSDBlkLenSet(ctrl->memBase, blkSize);
    hsmmcsd_dataLen = (nBlks * blkSize);
    hsmmcsd_buffer = ptr;
    xferPend = 1;
}

static unsigned int HSMMCSDCmdStatusGet(mmcsdCtrlInfo *ctrl) {
    unsigned int status = 0;
    while ((cmdCompFlag == 0) && (cmdTimeout == 0)) {
        status = HSMMCSDStatusGet();
    }
    if(cmdCompFlag) {
        HSMMCSDIntrStatusClear(ctrlInfo.memBase, HS_MMCSD_STAT_CMDCOMP);
        status = 1;
        cmdCompFlag = 0;
    }
    if(cmdTimeout) {
        HSMMCSDIntrStatusClear(ctrlInfo.memBase, HS_MMCSD_STAT_CMDTIMEOUT);
        status = 0;
        cmdTimeout = 0;
    }
    return status;
}

static unsigned int HSMMCSDStatusGet() {
    volatile unsigned int status = 0;
    status = HSMMCSDIntrStatusGet(ctrlInfo.memBase, HSMMCSD_INT_STATUS_FLAG);
    if(status & HS_MMCSD_STAT_CMDCOMP) {
        cmdCompFlag = 1;
    }
    if(status & HS_MMCSD_STAT_ERR) {
        errFlag = status & 0xFFFF0000;
        if(status & HS_MMCSD_STAT_CMDTIMEOUT) {
            cmdTimeout = 1;
        }
    }
    return status;
}

static unsigned int HSMMCSDXferStatusGet(mmcsdCtrlInfo *ctrl) {
    volatile unsigned int status = 0;
    volatile unsigned int temp;
    unsigned int i;
    while(1) {
        status = HSMMCSDStatusGet();

        if(status & HS_MMCSD_STAT_BUFRDRDY) {
            HSMMCSDIntrStatusClear(ctrlInfo.memBase, HS_MMCSD_STAT_BUFRDRDY);

            if(hsmmcsd_buffer != NULL) {
                for (i = 0; i < hsmmcsd_dataLen; i += 4) {
                    temp = HWREG(MMCSD_BASE + MMCHS_DATA);
                    hsmmcsd_buffer[i] = *((char*)&temp);
                    hsmmcsd_buffer[i + 1] = *((char*)&temp + 1);
                    hsmmcsd_buffer[i + 2] = *((char*)&temp + 2);
                    hsmmcsd_buffer[i + 3] = *((char*)&temp + 3);
                }
            }
        }

        if (status & HS_MMCSD_STAT_BUFWRRDY) {
            HSMMCSDIntrStatusClear(ctrlInfo.memBase, HS_MMCSD_STAT_BUFRDRDY);

            if (hsmmcsd_buffer != NULL) {
                for (i = 0; i < hsmmcsd_dataLen; i+=4) {
                    *((char*)&temp) = hsmmcsd_buffer[i];
                    *((char*)&temp + 1) = hsmmcsd_buffer[i+1];
                    *((char*)&temp + 2) = hsmmcsd_buffer[i+2];
                    *((char*)&temp + 3) = hsmmcsd_buffer[i+3];
                    HWREG(MMCSD_BASE + MMCHS_DATA) = temp;
                }
            }
        }

        if (status & HS_MMCSD_STAT_DATATIMEOUT) {
            HSMMCSDIntrStatusClear(ctrlInfo.memBase, HS_MMCSD_STAT_DATATIMEOUT);
            status = 0;
            xferPend = 0;
            break;
        }

        if (status & HS_MMCSD_STAT_TRNFCOMP) {
            HSMMCSDIntrStatusClear(ctrlInfo.memBase, HS_MMCSD_STAT_TRNFCOMP);
            status = 1;
            xferPend = 0;
            break;
        }
    }
    return status;
}

uint8_t *loadFile(char *fname) {
	uint8_t *buffer = NULL, *secBuffer, *ptr;
    unsigned short bytesRead = 0;
    FIL fileObj;
    // Open the file for reading.
    FRESULT fresult = f_open(&fileObj, fname, FA_READ);
    // If there was some problem opening the file, then return an error.
    if(fresult != FR_OK) {
        return NULL;
    }else {
    	buffer = (uint8_t *)malloc(sizeof(uint8_t) * fileObj.fsize);
    	secBuffer = (uint8_t *)malloc(sizeof(uint8_t) * DATA_BUF_SIZE);
    	ptr = buffer;
    	do{
    		// 一次只能读512字节, DATA_BUF_SIZE = 512
    		fresult = f_read(&fileObj, secBuffer, DATA_BUF_SIZE, &bytesRead);
    		if(fresult != FR_OK || (bytesRead > DATA_BUF_SIZE)) {
				free(buffer);
				free(secBuffer);
				return NULL;
			}
    		memcpy(ptr, secBuffer, bytesRead);
			ptr += bytesRead;

    	}while(bytesRead == 512);
    	free(secBuffer);
    }
    // Close the file.
    fresult = f_close(&fileObj);
    return buffer;
}

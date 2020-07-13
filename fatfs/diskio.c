/*
 * diskio.c
 * @brief 
 * Created on: Apr 10, 2020
 * Author: Yanye
 */

#include "diskio.h"

FatDevice fat_devices[DRIVE_NUM_MAX];


DSTATUS disk_initialize(BYTE bValue) {
	unsigned int status;

    if(DRIVE_NUM_MAX <= bValue) {
        return STA_NODISK;
    }

    if((DRIVE_NUM_MMCSD == bValue) && (fat_devices[bValue].initDone != 1)) {
        mmcsdCardInfo *card = (mmcsdCardInfo *) fat_devices[bValue].dev;
        //SDCard init
        status = MMCSDCardInit(card->ctrl);
        if (status == 0) {
            return STA_NOINIT;
        }else {
            // set bus width
            if (card->cardType == MMCSD_CARD_SD) {
                MMCSDBusWidthSet(card->ctrl);
            }
            // transfer speed
            MMCSDTranSpeedSet(card->ctrl);
        }
		fat_devices[bValue].initDone = 1;
    }
    return 0;
}

DSTATUS disk_status(BYTE drv) {
	return (fat_devices[drv].initDone == 1) ? RES_OK : RES_NOTRDY;
}


/**
 * This function reads sector(s) from the disk drive
 * @param drv Physical drive number (0)
 * @param buff Pointer to the data buffer to store read data
 * @param sector Physical drive nmuber (0)
 * @param count Sector count (1..255)
 * */
DRESULT disk_read (BYTE drv, BYTE* buff, DWORD sector, BYTE count) {
	if (drv == DRIVE_NUM_MMCSD) {
		mmcsdCardInfo *card = fat_devices[drv].dev;
    	// READ BLOCK
		if (MMCSDReadCmdSend(card->ctrl, buff, sector, count) == 1) {
        	return RES_OK;
		}
    }
    return RES_ERROR;
}




#if _READONLY == 0
/**
 * This function write sector(s) from the disk drive
 * @param drv Physical drive number (0)
 * @param buff Pointer to the data to be written
 * @param sector Start sector number (LBA)
 * @param count Sector count (1..255)
 * */
DRESULT disk_write (BYTE ucDrive, const BYTE* buff, DWORD sector, BYTE count) {
	if (ucDrive == DRIVE_NUM_MMCSD) {
		mmcsdCardInfo *card = fat_devices[ucDrive].dev;
    	/* WRITE BLOCK */
	    if(MMCSDWriteCmdSend(card->ctrl,(BYTE*) buff, sector, count) == 1) {
        	return RES_OK;
		}
	}
    return RES_ERROR;
}
#endif /* _READONLY */

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
    BYTE drv,               /* Physical drive number (0) */
    BYTE ctrl,              /* Control code */
    void *buff)             /* Buffer to send/receive control data */
{
	return RES_OK;
}

/*---------------------------------------------------------*/
/* User Provided Timer Function for FatFs module           */
/*---------------------------------------------------------*/
/* This is a real time clock service to be called from     */
/* FatFs module. Any valid time must be returned even if   */
/* the system does not support a real time clock.          */

DWORD get_fattime (void) {
    return    ((2007UL-1980) << 25) // Year = 2007
            | (6UL << 21)           // Month = June
            | (5UL << 16)           // Day = 5
            | (11U << 11)           // Hour = 11
            | (38U << 5)            // Min = 38
            | (0U >> 1)             // Sec = 0
            ;
}

/*-----------------------------------------------------------------------
/  Low level disk interface modlue include file  R0.04a   (C)ChaN, 2007
/-----------------------------------------------------------------------*/

#ifndef _DISKIO_
#define _DISKIO_

#define _READONLY	0	/* 1: Read-only mode */

#include "integer.h"
#include "mmcsd_proto.h"
#include "hs_mmcsdlib.h"
#include "ff.h"


typedef struct _fat_device {
    /* Pointer to underlying device/controller */
    void *dev;
    /* File system pointer */
    FATFS *fs;
	/* state */
	unsigned int initDone;
}FatDevice;

#define DRIVE_NUM_MMCSD     0
#define DRIVE_NUM_MAX      2


/* Status of Disk Functions */
typedef BYTE	DSTATUS;

/* Results of Disk Functions */
typedef enum {
	RES_OK = 0,		/* 0: Successful */
	RES_ERROR,		/* 1: R/W Error */
	RES_WRPRT,		/* 2: Write Protected */
	RES_NOTRDY,		/* 3: Not Ready */
	RES_PARERR		/* 4: Invalid Parameter */
} DRESULT;


/*---------------------------------------*/
/* Prototypes for disk control functions */

DSTATUS disk_initialize (BYTE);
DSTATUS disk_status (BYTE);
DRESULT disk_read (BYTE, BYTE*, DWORD, BYTE);
#if	_READONLY == 0
DRESULT disk_write (BYTE, const BYTE*, DWORD, BYTE);
#endif
DRESULT disk_ioctl (BYTE, BYTE, void*);

DWORD get_fattime (void);


/* Disk Status Bits (DSTATUS) */

#define STA_NOINIT		0x01	/* Drive not initialized */
#define STA_NODISK		0x02	/* No medium in the drive */
#define STA_PROTECT		0x04	/* Write protected */


/* Command code for disk_ioctrl() */

#define GET_SECTOR_COUNT	1
#define GET_SECTOR_SIZE		2
#define CTRL_SYNC			3
#define CTRL_POWER			4
#define CTRL_LOCK			5
#define CTRL_EJECT			6
#define MMC_GET_CSD			10
#define MMC_GET_CID			11
#define MMC_GET_OCR			12
#define ATA_GET_REV			20
#define ATA_GET_MODEL		21
#define ATA_GET_SN			22



#endif

/*
 * mmu_conf.c
 *
 *  Created on: Feb 3, 2020
 *      Author: Yanye
 */

#include "mmu_conf.h"

#pragma DATA_ALIGN(PageTable, MMU_PAGETABLE_ALIGN_SIZE);
volatile unsigned int PageTable[MMU_PAGETABLE_NUM_ENTRY];

void MMUConfigAndEnable() {
    /*
    ** Define DDR memory region of AM3352. DDR can be configured as Normal
    ** memory with R/W access in user/privileged modes. The cache attributes
    ** specified here are,
    ** Inner - Write through, No Write Allocate
    ** Outer - Write Back, Write Allocate
    */
    REGION regionDdr = {MMU_PGTYPE_SECTION, START_ADDR_DDR, NUM_SECTIONS_DDR,
                        MMU_MEMTYPE_NORMAL_NON_SHAREABLE(MMU_CACHE_WT_NOWA, MMU_CACHE_WB_WA),
                        MMU_REGION_NON_SECURE, MMU_AP_PRV_RW_USR_RW,
                        (unsigned int*)PageTable};
    /*
    ** Define L3 OCMC0 RAM region of AM3352. Same Attributes of DDR region given.
    */
    REGION regionOcmc = {MMU_PGTYPE_SECTION, START_ADDR_OCMC, NUM_SECTIONS_OCMC,
                         MMU_MEMTYPE_NORMAL_NON_SHAREABLE(MMU_CACHE_WT_NOWA, MMU_CACHE_WB_WA),
                         MMU_REGION_NON_SECURE, MMU_AP_PRV_RW_USR_RW,
                         (unsigned int*)PageTable};

    /*
    ** Define Device Memory Region. The region between OCMC and DDR is
    ** configured as device memory, with R/W access in user/privileged modes.
    ** Also, the region is marked 'Execute Never'.
    */
    REGION regionDev = {MMU_PGTYPE_SECTION, START_ADDR_DEV, NUM_SECTIONS_DEV,
    					MMU_MEMTYPE_DEVICE_SHAREABLE, MMU_REGION_NON_SECURE,
                        MMU_AP_PRV_RW_USR_RW  | MMU_SECTION_EXEC_NEVER,
                        (unsigned int*)PageTable};

    /* Initialize the page table and MMU */
    MMUInit((unsigned int*)PageTable);

    /* Map the defined regions */
    MMUMemRegionMap(&regionDdr);
    MMUMemRegionMap(&regionOcmc);
    MMUMemRegionMap(&regionDev);

    /* Now Safe to enable MMU */
    MMUEnable((unsigned int*)PageTable);
}

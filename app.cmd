-stack  0x80000     /* SOFTWARE STACK SIZE  512KB   */
-heap   0xC000000   /* HEAP AREA SIZE       192MB */
-e Entry
/* Since we used 'Entry' as the entry-point symbol the compiler issues a    */
/* warning (#10063-D: entry-point symbol other than "_c_int00" specified:   */
/* "Entry"). The CCS Version (5.1.0.08000) stops building from command      */
/* line when there is a warning. So this warning is suppressed with the     */
/* below flag. */

--diag_suppress=10063
/* 调用ti libc的malloc实现时会出现该warning, 实际上.sysmem已经分配到堆区了, 无需手动指定*/
/* 如果手动指定了.sysmem的load地址会导致.sysmem分配到.data区*/
--diag_suppress=10247-D

/* SPECIFY THE SYSTEM MEMORY MAP */
MEMORY
{
        IRAM_MEM       : org = 0x402F0400  len = 0x1FBFF            /* IRAM */
        DDR_MEM        : org = 0x80000000  len = 0x10000000           /* DDRAM */
}

/* SPECIFY THE SECTIONS ALLOCATION INTO MEMORY */

SECTIONS
{
    .text:Entry : load > 0x80000000
    .text    : load > DDR_MEM              /* CODE                          */
    .data    : load > DDR_MEM              /* INITIALIZED GLOBAL AND STATIC VARIABLES */
    .bss     : load > DDR_MEM              /* UNINITIALIZED OR ZERO INITIALIZED */
                                           /* GLOBAL & STATIC VARIABLES */
                    RUN_START(bss_start)   /* Clear bss segment in runtime*/
                    RUN_END(bss_end)
    .const   : load > DDR_MEM              /* GLOBAL CONSTANTS              */
    .stack   : load > 0x8FF80000           /* SOFTWARE SYSTEM STACK         */
}


#include <config.h>
.global __v7_flush_dcache_all
__v7_flush_dcache_all:
	dmb				@ ensure ordering with prvs mr access
	mrc	p15, 1, r0, c0, c0, 1	@ read clidr
	ands	r3, r0, #0x7000000	@ extract loc from clidr
	mov	r3, r3, lsr #23		@ left align loc bit field
	beq	finished		@ if loc is 0, then no need to clean
	mov	r10, #0			@ start clean at cache level 0
loop1:
	add	r2, r10, r10, lsr #1	@ work out 3x current cache level
	mov	r1, r0, lsr r2		@ extract cache type bits from clidr
	and	r1, r1, #7		@ mask of the bits for current cache
	cmp	r1, #2			@ see what cache we have at this level
	blt	skip			@ skip if no cache, or just i-cache
	mcr	p15, 2, r10, c0, c0, 0	@ select current cache level in cssr
	isb				@ isb to sych the new cssr&csidr
	mrc	p15, 1, r1, c0, c0, 0	@ read the new csidr
	and	r2, r1, #7		@ extract the length of the cache lines
	add	r2, r2, #4		@ add 4 (line length offset)
	ldr	r4, =0x3ff
	ands	r4, r4, r1, lsr #3	@ find maximum number on the way size
	clz	r5, r4			@ find bit position of way size incr
	ldr	r7, =0x7fff
	ands	r7, r7, r1, lsr #13	@ extract max number of the index size
loop2:
	mov	r9, r4			@ create working copy of max way size
loop3:
	orr	r11, r10, r9, lsl r5	@ factor way and cache number into r11
 	orr	r11, r11, r7, lsl r2	@ factor index number into r11
	mcr	p15, 0, r11, c7, c14, 2	@ clean & invalidate by set/way
	subs	r9, r9, #1		@ decrement the way
	bge	loop3
	subs	r7, r7, #1		@ decrement the index
	bge	loop2
skip:
	add	r10, r10, #2		@ increment cache number
	cmp	r3, r10
	bgt	loop1
finished:
	mov	r10, #0			@ swith back to cache level 0
	mcr	p15, 2, r10, c0, c0, 0	@ select current cache level in cssr
	dsb
	isb
	mov	pc, lr

__setup_mmu:	sub	r3, r4, #16384		@ Page directory size
		bic	r3, r3, #0xff		@ Align the pointer
		bic	r3, r3, #0x3f00
/*
 * Initialise the page tables, turning on the cacheable and bufferable
 * bits for the RAM area only.
 */
		mov	r0, r3

		mov	r9, r0, lsr #18
		mov	r9, r9, lsl #18		@ start of RAM

		mov 	r9,#0			@start of RAM

#if CONFIG_DDR_MT41J64M16
		add	r10, r9, #0x10000000	@ a reasonable RAM size
#elif (CONFIG_DDR_MT41J256M8)
		add	r10, r9, #0x40000000	@ a reasonable RAM size
#endif

#if EXEC_AREA
		mov	r1, #0x12	@no exec
#else
		mov	r1, #0x02	@exec
#endif
		orr	r1, r1, #3 << 10
		add	r2, r3, #16384
1:		cmp	r1, r9			@ if virt > start of RAM
		orrhs	r1, r1, #0x0c		@ set cacheable, bufferable
		cmp	r1, r10			@ if virt > end of RAM
		bichs	r1, r1, #0x0c		@ clear cacheable, bufferable
#if INSTN_CACHE_CONTROL
		bichs	r1, r1, #0x1000
		orrls	r1,r1,#0x1000
#endif
		str	r1, [r0], #4		@ 1:1 mapping
		add	r1, r1, #1048576
		teq	r0, r2
		bne	1b
/*
 * If ever we are running from Flash, then we surely want the cache
 * to be enabled also for our execution instance...  We map 2MB of it
 * so there is no map overlap problem for up to 1 MB compressed kernel.
 * If the execution is in RAM then we would only be duplicating the above.
 */
#if FLASH_CODE_EXEC
		mov	r1, #0x1e
		orr	r1, r1, #3 << 10
		mov	r2, pc, lsr #20
		orr	r1, r1, r2, lsl #20
		add	r0, r3, r2, lsl #2
		str	r1, [r0], #4
		add	r1, r1, #1048576
		str	r1, [r0]
#endif
		mov	pc, lr

v7_invalidate_l1:
        mov     r0, #0
        mcr     p15, 2, r0, c0, c0, 0
        mrc     p15, 1, r0, c0, c0, 0

        ldr     r1, =0x7fff
        and     r2, r1, r0, lsr #13

        ldr     r1, =0x3ff

        and     r3, r1, r0, lsr #3  @ NumWays - 1
        add     r2, r2, #1          @ NumSets

        and     r0, r0, #0x7
        add     r0, r0, #4          @ SetShift

        clz     r1, r3              @ WayShift
        add     r4, r3, #1          @ NumWays
1:      sub     r2, r2, #1          @ NumSets--
        mov     r3, r4              @ Temp = NumWays
2:      subs    r3, r3, #1          @ Temp--
        mov     r5, r3, lsl r1
        mov     r6, r2, lsl r0
        orr     r5, r5, r6          @ Reg = Temp<<WayShift)|(NumSets<<SetShift)
        mcr     p15, 0, r5, c7, c6, 2
        bgt     2b
        cmp     r2, #0
        bgt     1b
        dsb
        isb
        mov     pc, lr


_TLB_TOP:
	.word  __TLB_TOP

.global __armv7_mmu_cache_on
__armv7_mmu_cache_on:
		push    {r0-r11,lr}
		bl v7_invalidate_l1
		/*
		 * Invalidate L1 I/D
		 */
		mov	r0, #0			@ set up for MCR
		mcr	p15, 0, r0, c8, c7, 0	@ invalidate TLBs
		mcr	p15, 0, r0, c7, c5, 0	@ invalidate icache
		ldr	r4, _TLB_TOP		@page table base address
		mrc	p15, 0, r11, c0, c1, 4	@ read ID_MMFR0
		tst	r11, #0xf		@ VMSA
		blne	__setup_mmu
		mov	r0, #0
		mcr	p15, 0, r0, c7, c10, 4	@ drain write buffer
		tst	r11, #0xf		@ VMSA
		mcrne	p15, 0, r0, c8, c7, 0	@ flush I,D TLBs
		mrc	p15, 0, r0, c1, c0, 0	@ read control reg
		orr	r0, r0, #0x5000		@ I-cache enable,
						@ RR cache replacement
		orr	r0, r0, #0x003c		@ write buffer
		orrne	r0, r0, #1		@ MMU enabled
		movne	r1, #-1
		mcrne	p15, 0, r3, c2, c0, 0	@ load page table pointer
		mcrne	p15, 0, r1, c3, c0, 0	@ load domain access control
		mcr	p15, 0, r0, c1, c0, 0	@ load control register
#if 0
	mrc	p15, 0, r0, c1, c0, 0	@ and read it back , giving problem !!
#endif
		mov	r0, #0
		mcr	p15, 0, r0, c7, c5, 4	@ ISB
		pop  	{r0-r11,lr}
		/*mov	pc, r12*/
		mov	pc,r14

.global __disable_dcache
__disable_dcache:
		push 	{r0}
		mrc	p15, 0, r0, c1, c0, 0	@ read control reg
		and	r0, r0, #0xfffffffB	@ disable D-cache
		mcr	p15, 0, r0, c1, c0, 0	@ load control register
		pop	{r0}
		dsb
		isb
		mov 	pc,r14

.global __disable_mmu
__disable_mmu:
		push 	{r0}
		mrc	p15, 0, r0, c1, c0, 0	@ read control reg
		and	r0, r0, #0xfffffffe	@ disable D-cache
		mcr	p15, 0, r0, c1, c0, 0	@ load control register
		pop	{r0}
		dsb
		isb
		mov 	pc,r14


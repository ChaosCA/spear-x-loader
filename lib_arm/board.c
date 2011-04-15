/*
 * (C) Copyright 2002-2006
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * To match the U-Boot user interface on ARM platforms to the U-Boot
 * standard (as on PPC platforms), some messages with debug character
 * are removed from the default U-Boot build.
 *
 * Define DEBUG here if you want additional info as shown below
 * printed upon startup:
 *
 * U-Boot code: 00F00000 -> 00F3C774 BSS: -> 00FC3274
 * IRQ Stack: 00ebff7c
 * FIQ Stack: 00ebef7c
 */

#include <common.h>
#include <asm/io.h>

#if DDR_ECC_ENABLE
#define ECC_MASK	(3<<8)
#define NO_ECC		(0<<8)
#define ECC_REPORT_ON	(1<<8)
#define ECC_CORR_ON	(3<<8)

s32
memset_long(u32 *addr, u32 val, u32 size)
{
	u32 i;
	for (i = 0; i < (size/4); i++)
		*(addr + i) = val;
	return 0;
}

void mpmc_config_ecc(u32 value)
{
	u32 *mpmc_reg_p = (u32 *)CONFIG_SPEAR_MPMCBASE;
	writel((readl(&mpmc_reg_p[18]) & (~ECC_MASK)) | (value),
						&mpmc_reg_p[18]);
	while ((readl(&mpmc_reg_p[18]) & ECC_MASK) != value)
		;
}

#if USE_C3
#define USE_C3 1
#define C3_BASE_ADDRESS						0xE1800000
#define C3_HIF_offset							((unsigned int)0x400)	/* master interface registers */
#define C3_HIF_MBAR_offset        					((unsigned int)0x304)	/* memory base address register */

#ifdef SPEAR_REVB
#define C3_CLOCK_ENABLE_BASE_ADDRESS		0xE070030C
#define C3_PERIPHERAL_RESET_BASE_ADDRESS	0xE0700318
#define C3_PERIPHERAL_CLK_CFG				0xE0700244
#define C3_CLK_SYNTH						0xE0700250
#define C3_CLK_SYNTH_X_Y					0x40010002
#define C3_CLK_SYNTH_ENA					0xC0010002
#define C3_PERIPHERAL_CLK_RNG_BIT_ENA		0x02
#define C3_PERIPHERAL_RESET_BIT_POS			29
#else
#define C3_CLOCK_ENABLE_BASE_ADDRESS		0xE0700274
#define C3_PERIPHERAL_RESET_BASE_ADDRESS	0xE070027C
#define C3_PERIPHERAL_CLK_CFG				0xE0700244
#define C3_CLK_SYNTH						0xE070024C
#define C3_CLK_SYNTH_X_Y					0x40010002
#define C3_CLK_SYNTH_ENA					0xC0010002
#define C3_PERIPHERAL_CLK_RNG_BIT_ENA		0x02
#define C3_PERIPHERAL_RESET_BIT_POS			29
#endif
#define C3_MOVE_CHANNEL_OFFSET			0x2000
#define C3_MOVE_CHANNEL_ID				(C3_BASE_ADDRESS + C3_MOVE_CHANNEL_OFFSET + 0x3FC)
#define C3_MOVE_CHANNEL_ID_VAL			0x00000102

/* ID0 Registers definition */
#define C3_ID0_OFFSET						0x1000
#define C3_ID0_SCR						(C3_BASE_ADDRESS + C3_ID0_OFFSET)
#	define C3_ID0_SCR_RESET				BIT_SET(16)
#define C3_ID0_IP							(C3_BASE_ADDRESS + C3_ID0_OFFSET + 0x10)
#define C3_ID0_DEFAULT_READY_VALUE			0x80002AAA
#define C3_ID0_STATE_MASK					0xC0000000
#define C3_ID0_STATE_RUN					0xC0000000
#define C3_ID0_STATE_IDLE					0x80000000

#define BIT_SET(a)								(1<<a)
#define C3_CLOCK_ENABLE_BIT					BIT_SET(29)
#define C3_HID_OFFSET							0x400
#define C3_HIF_MCR							(C3_BASE_ADDRESS + C3_HID_OFFSET + 0x308)
#define C3_HIF_MCR_ENABLE_INTERNAL_MEM		0x01
#define MEM_INTERNAL_MEMORY_BASE_ADDRESS		(C3_BASE_ADDRESS+0x400)
#	define C3_LOCAL_MEMORY_ADDRESS			((unsigned int)0xF0000000)	/* bits[31-16] of AHB address */
#define MEM_SIZEOF_INTERNAL_MEMORY			(0x4000)
#define MOVE_AND (1<<21)
#define C3_HIF_MAAR_offset						((unsigned int)0x310)	/* memory access address register */
										/* AHB: *C3_HIF_MBAR_offset + *C3_HIF_MAAR_offset */
#define C3_HIF_MADR_offset					((unsigned int)0x314)	/* memory access data register */

static __inline void c3_write_register(unsigned long Offset, unsigned long Data)
{
	*(volatile unsigned long*)(C3_BASE_ADDRESS + Offset) = Data;
} /* c3_write_register */


__inline unsigned long MEM_INTERNAL_CHECK_RANGE(void* a)
{
	unsigned long addr = (unsigned long) a;

	if (((addr) >= (unsigned long) MEM_INTERNAL_MEMORY_BASE_ADDRESS) &&
		((addr) < ((unsigned long)MEM_INTERNAL_MEMORY_BASE_ADDRESS+MEM_SIZEOF_INTERNAL_MEMORY)))
		return 1;
	else
		return 0;
}

#define C3_MEMXLATE(addr) (MEM_INTERNAL_CHECK_RANGE((void *)addr)?(unsigned long) addr - MEM_INTERNAL_MEMORY_BASE_ADDRESS + C3_LOCAL_MEMORY_ADDRESS: (unsigned long)addr)

void c3_reset(void)
{
	/* reset the entire C3 */
	unsigned long x;
	volatile unsigned long *reg = (volatile unsigned long *) C3_PERIPHERAL_RESET_BASE_ADDRESS;

	x = *reg & ~(1<<C3_PERIPHERAL_RESET_BIT_POS);
	*reg = x | (1<<C3_PERIPHERAL_RESET_BIT_POS);
	*reg = x;
}

int c3_init(void)
{
	register int err;
	register int tries = 2;

	/* 1. Enable the clocks
	 * c3_rng_clock_enable();
	 */
	*(volatile unsigned long *) C3_CLOCK_ENABLE_BASE_ADDRESS |= C3_CLOCK_ENABLE_BIT;
	c3_reset();

	do {
	/* 2. Check ID */
		if ((*(volatile unsigned long *) C3_ID0_SCR)!= C3_ID0_DEFAULT_READY_VALUE) {
			/* Perform an asynchronous reset */
			*(volatile unsigned long *) C3_ID0_SCR = C3_ID0_SCR_RESET;
		}

		 /* 3. Check the move channel returns the right value */
		if ((*(volatile unsigned long *) C3_MOVE_CHANNEL_ID)== C3_MOVE_CHANNEL_ID_VAL) {
			return -1;
		 }

		 /* 4. Init C3 internal memory for C3 accesses */
		*(volatile unsigned long *) C3_HIF_MCR = C3_HIF_MCR_ENABLE_INTERNAL_MEM;
		*(volatile unsigned long *) (C3_BASE_ADDRESS + C3_HIF_offset + C3_HIF_MBAR_offset) = C3_LOCAL_MEMORY_ADDRESS;
    
		if (err) {
			c3_reset();
		}
	} while (err && tries--);

	return 0;
}


int c3_run(void * prog_start)
{
	*(volatile unsigned long *) C3_ID0_IP = (unsigned long *)C3_MEMXLATE((unsigned long) prog_start);

	while (((*(volatile unsigned long *) C3_ID0_SCR) & C3_ID0_STATE_MASK) == C3_ID0_STATE_RUN);

	if (((*(volatile unsigned long *) C3_ID0_SCR) & C3_ID0_STATE_MASK) != C3_ID0_STATE_IDLE) {
		/* If not back to idle an error occured */
		/* Need to return and error and reset the dispatcher: first blade */
		*(volatile unsigned long *) C3_ID0_SCR = C3_ID0_SCR_RESET;

		/* Second blade reset the whole enchilada */
		c3_reset();

		/* Make sure internal access is set in order to run c3 programs */
		*(volatile unsigned long *) C3_HIF_MCR = C3_HIF_MCR_ENABLE_INTERNAL_MEM;

		return -1;
	}

	return 0;
}

int c3_move(void *ram, void *dest, void *src, int cnt,  int optype, int opdata)
{
	unsigned long * my_c3_prog;
	int ret = 0;

	/* 3.b Prepare program */
	my_c3_prog = (unsigned long *)ram;

	/* 3.b.i. Mov init */
	my_c3_prog[0] = 0x06000000;
	my_c3_prog[1] = opdata;

	/* 3.b.ii. Mov data */
	my_c3_prog[2] = 0x0A800000+cnt+optype;
	my_c3_prog[3] = C3_MEMXLATE(src);
	my_c3_prog[4] = C3_MEMXLATE(dest);

	/* 3.b.iii. Stop */
	my_c3_prog[5] = 0x00000000;

	/* 4. Execute and wait */
	ret = c3_run(my_c3_prog);

	return ret;
}

unsigned long write_meml(void *addr, unsigned long val)
{
	unsigned long offset = ((unsigned long)addr) - MEM_INTERNAL_MEMORY_BASE_ADDRESS;

	/* set the address */
	c3_write_register(C3_HIF_offset +C3_HIF_MAAR_offset, offset);

	/* write the value */
	c3_write_register(C3_HIF_offset +C3_HIF_MADR_offset, val);

	return val;
} /* write_meml */

void c3_memset(void *ram, unsigned char *end, u32 val)
{
#define DATA_SIZE (1024*4)
	unsigned char *cur, *data;
	u32 size;

	data =  (char *)ram+0x100;

	/* zero the memory */
	write_meml(data, val);
	for (size = 4;size<DATA_SIZE;size<<=1)
		c3_move(ram, (unsigned char *)data+size,data, size, MOVE_AND, 0xffffffff);

	cur = 0;

	while (cur < end) {
		c3_move(ram, cur, data, DATA_SIZE, MOVE_AND, 0xffffffff);
		cur += DATA_SIZE;
	}
}

#endif
#endif

void start_armboot(void)
{
#if DDR_ECC_ENABLE
	/* u32 j, i = 0; */
#if CONFIG_DDR_MT41J64M16
	int end = 256*1024*1024; /* Initializing 256MB*/
#elif CONFIG_DDR_MT41J256M8
	int end = 1024*1024*1024; /* initializing 1GB */
#endif
#if USE_C3 == 0
	__armv7_mmu_cache_on();
	memset_long((u32 *)(0x000), 0, 96); /*intializing 96 bytes, 24 words*/
	mpmc_config_ecc(ECC_CORR_ON);
	asm("dmb"); /* data memory barrier */
	asm("dsb"); /* data synch barrier*/
	asm("isb"); /* isntn synch barrier*/
	memset_long((u32 *)(0x000), 0, end); /* Initializing */
	__v7_flush_dcache_all();
	__disable_dcache();
	icache_disable();
	__disable_mmu();
#else
	void *c3sram = (void *)MEM_INTERNAL_MEMORY_BASE_ADDRESS;
	c3_init();
	mpmc_config_ecc(ECC_CORR_ON);
	c3_memset(c3sram, end, 0);
#endif
#endif

	boot();
}

void hang(void)
{
	for (;;);
}

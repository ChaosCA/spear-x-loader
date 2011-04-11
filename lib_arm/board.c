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
#endif

ulong start_armboot(void)
{
#if DDR_ECC_ENABLE
	u32 j, i = 0;
	__armv7_mmu_cache_on();
	memset_long((u32 *)(0x000), 0, 96); /*intializing 96 bytes, 24 words*/
	mpmc_config_ecc(ECC_CORR_ON);
	asm("dmb"); /* data memory barrier */
	asm("dsb"); /* data synch barrier*/
	asm("isb"); /* isntn synch barrier*/
#if CONFIG_DDR_MT41J64M16
	memset_long((u32 *)(0x000), 0, (256*1024*1024)); /* Initializing 256MB*/
#elif CONFIG_DDR_MT41J256M8
	memset_long((u32 *)(0x000), 0, (1024*1024*1024)); /* initializing 1GB */
#endif
	__v7_flush_dcache_all();
	__disable_dcache();
	icache_disable();
	__disable_mmu();
#endif
	return boot();
}

void hang(void)
{
	for (;;);
}

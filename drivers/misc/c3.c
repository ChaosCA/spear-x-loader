/*
 * (C) Copyright 2011
 * ST Micoelectronics Pvt. Ltd.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/spr13xx_misc.h>
#include <c3.h>

static inline int mem_internal_check_range(void *addr)
{
	if (((ulong)addr < C3_INT_MEM_BASE_ADDR))
		return 0;

	if ((ulong)addr >= (C3_INT_MEM_BASE_ADDR + C3_INT_MEM_SIZE))
		return 0;

	return 1;
}

static inline unsigned long c3_mem_xlate(void *addr)
{

	if (mem_internal_check_range(addr)) {
		return (unsigned long)addr - C3_INT_MEM_BASE_ADDR +
			C3_LOCAL_MEM_ADDR;
	}

	return (ulong)addr;
}

void c3_reset(void)
{
	struct misc_regs *misc_p = (struct misc_regs *)CONFIG_SPEAR_MISCBASE;
	u32 val, i;

	val = readl(&misc_p->perip1_sw_rst);
	val = val & ~C3_RST_BIT;

	/* first put c3 to reset */
	writel(val | C3_RST_BIT, &misc_p->perip1_sw_rst);

	/* wait for some time */
	for (i = 0; i < 100000; i++)
		;

	/* bring c3 out of reset */
	writel(val & ~C3_RST_BIT, &misc_p->perip1_sw_rst);
}

int c3_init(void)
{
	u32 val;
	struct misc_regs *misc_p = (struct misc_regs *)CONFIG_SPEAR_MISCBASE;

	/*
	 * 1. Enable the clocks
	 * c3_rng_clock_enable();
	 */

	val = readl(&misc_p->perip1_clk_enb);
	val |= C3_CLK_ENB_BIT;
	writel(val, &misc_p->perip1_clk_enb);

	c3_reset();

	/* 2. Check ID */
	if (readl(C3_ID0_SCR) != C3_ID0_DEF_RDY_VAL) {
		/* Perform an asynchronous reset */
		writel(C3_ID0_SCR_RST, C3_ID0_SCR);
	}

	/* 3. Check the move channel returns the right value */
	if (readl(C3_MOVE_CHANNEL_ID) == C3_MOVE_CHANNEL_ID_VAL)
		return -1;

	/* 4. Init C3 internal memory for C3 accesses */
	writel(C3_HIF_MCR_ENB_INT_MEM, C3_HIF_MCR);
	writel(C3_LOCAL_MEM_ADDR, CONFIG_C3_BASE +
			C3_HIF_OFF + C3_HIF_MBAR_OFF);

	return 0;
}

int c3_run(void *prog_start)
{
	u32 val;

	writel(c3_mem_xlate(prog_start), C3_ID0_IP);

	while (1) {
		val = readl(C3_ID0_SCR);
		if ((val & C3_ID0_STATE_MASK) == C3_ID0_STATE_RUN)
			continue;
		else
			break;
	}

	val = readl(C3_ID0_SCR);
	if ((val & C3_ID0_STATE_MASK) != C3_ID0_STATE_IDLE) {
		/* If not back to idle an error occured */
		/* Need to return and error and reset the dispatcher: first blade */
		writel(C3_ID0_SCR_RST, C3_ID0_SCR);

		/* Second blade reset the whole enchilada */
		c3_reset();

		/* Make sure internal access is set in order to run c3 programs */
		writel(C3_HIF_MCR_ENB_INT_MEM, C3_HIF_MCR);

		return -1;
	}

	return 0;
}

int c3_move(void *ram, void *dest, void *src, int cnt, int optype, int opdata)
{
	unsigned long *my_c3_prog;
	int ret = 0;

	/* 3.b Prepare program */
	my_c3_prog = (unsigned long *)ram;

	/* 3.b.i. Mov init */
	my_c3_prog[0] = C3_CMD_MOVE_INIT;
	my_c3_prog[1] = opdata;

	/* 3.b.ii. Mov data */
	my_c3_prog[2] = C3_CMD_MOVE_DATA + cnt + optype;
	my_c3_prog[3] = c3_mem_xlate(src);
	my_c3_prog[4] = c3_mem_xlate(dest);

	/* 3.b.iii. Stop */
	my_c3_prog[5] = C3_CMD_STOP;

	/* 4. Execute and wait */
	ret = c3_run(my_c3_prog);

	return ret;
}

unsigned long write_meml(void *addr, u32 val)
{
	unsigned long offset = ((unsigned long)addr) - C3_INT_MEM_BASE_ADDR;

	/* set the address */
	writel(offset, CONFIG_C3_BASE + C3_HIF_OFF + C3_HIF_MAAR_OFF);

	/* write the value */
	writel(val, CONFIG_C3_BASE + C3_HIF_OFF + C3_HIF_MADR_OFF);

	return val;
}

void c3_memset(void *ram, void *end, u32 val)
{
#define DATA_SIZE (1024*4)
	void *cur, *data;
	u32 size;

	data = ram + 0x100;

	/* zero the memory */
	write_meml(data, val);
	for (size = 4; size < DATA_SIZE; size <<= 1)
		c3_move(ram, data + size, data, size, C3_MOVE_AND, 0xffffffff);

	cur = 0;

	while (cur < end) {
		c3_move(ram, cur, data, DATA_SIZE, C3_MOVE_AND, 0xffffffff);
		cur += DATA_SIZE;
	}
}

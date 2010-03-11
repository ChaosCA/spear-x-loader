/*
 * (C) Copyright 2009
 * Vipin Kumar, ST Micoelectronics, vipin.kumar@st.com.
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
#include <nand.h>
#include <table.h>
#include <asm/io.h>
#include <asm/errno.h>
#include <asm/arch/spr13xx_nand.h>

static func_getsoctype boot_getsoctype;
static func_getboottype boot_getboottype;
static func_nandread boot_nandread;
static nand_info_t *boot_nandinfo;

/**
 * nand_init:
 */
void nand_init(void)
{
	struct bootrom_table *romtb_p =
		(struct bootrom_table *)BOOTROM_TABLE_ADDRESS;

	if (BOOTROM_TABLE_VERSION_2_0 == romtb_p->table_version) {
		boot_getsoctype = romtb_p->table.table_2_0.get_soc_type;
		boot_getboottype = romtb_p->table.table_2_0.get_boot_type;
		boot_nandinfo = romtb_p->table.table_2_0.nand_info;
		boot_nandread = romtb_p->table.table_2_0.nand_read;
	} else {
		boot_getsoctype = NULL;
		boot_getboottype = NULL;
		boot_nandinfo = NULL;
		boot_nandread = NULL;
	}
}

/**
 * nand_read_skip_bad:
 *
 * Read image from NAND flash.
 * Blocks that are marked bad are skipped and the next block is readen
 * instead as long as the image is short enough to fit even after skipping the
 * bad blocks.
 *
 * @param block block number to start the read
 * @param offset offset in the block number
 * @param length buffer length, on return holds remaining bytes to read
 * @param buffer buffer to write to
 * @return 0 in case of success
 */
int nand_read_skip_bad(u32 block, size_t offset, size_t *length,
		       u_char *buffer)
{
	nand_info_t *nand = boot_nandinfo;
	ulong chip_off = (nand->erasesize * block) + offset;
	
	if (boot_nandread)
		return (*boot_nandread)(nand, chip_off, length, (void *)buffer);
	
	return -EFAULT;
}

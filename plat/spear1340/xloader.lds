/*
 * April 2011 - Changed to support SPEAr device
 * Copyright (c) 20011-2015 ST Microelectronics
 *
 * (C) Copyright 2011
 * Vipul Kumar Samar, ST Microelectronics, <vipulkumar.samar@st.com>
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

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

MEMORY
{
	xloader : ORIGIN = 0xB3801500 , LENGTH = 0x4000
	xloader_table : ORIGIN = 0xB3803FF0 , LENGTH = 0x16

	bootrom : ORIGIN = 0xffff0000, LENGTH = 0x5000
	bootrom_table : ORIGIN = 0xffff7f00, LENGTH = 0x100
}

SECTIONS
{
	. = 0xB3801500;

	.text :
	{
		cpu/arm_cortexa8/start.o	(.text)
		*(.text)
	} > xloader

	. = ALIGN(4);
	.rodata . :
	{ *(.rodata*); . = ALIGN(4); } > xloader

	. = ALIGN(4);
	.data . :
	{ *(.data) } > xloader

	. = ALIGN(4);
	.bss . :
	{ *(.bss) } > xloader

	. = ALIGN(4);
	.table : { __xloader_table = .; } > xloader_table

	. = 0xFFFF0000;
	.boot_table : { __bootrom_table = .; } > bootrom_table

}

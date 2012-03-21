/*
 * (C) Copyright 2000-2012
 * Vipin Kumar, ST Microelectronics, vipin.kumar@st.com
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
	xloader : ORIGIN = 0xD2800B00 , LENGTH = 0x14F0
	xloader_bss : ORIGIN = 0x00004000 , LENGTH = 0x1000
	xloader_table : ORIGIN = 0xD2801FF0 , LENGTH = 0x10
}

SECTIONS
{
	.text :
	{
		cpu/arm926ejs/start.o	(.text)
		*(.text); . = ALIGN(4);
	} > xloader

	.rodata : { *(.rodata*); . = ALIGN(4); } > xloader
	.data : { *(.data); . = ALIGN(4); } > xloader
	.bss : { *(.bss) } > xloader_bss
	.table : {*(.table); . = ALIGN(4); } > xloader_table
}

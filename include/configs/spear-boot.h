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

#ifndef __SPEAR_BOOT_H
#define __SPEAR_BOOT_H

/* U-Boot placement */
#define CONFIG_SNOR_BOOT_ADDR		(0xF8010000)
#define CONFIG_PNOR_BOOT_ADDR		(0x50020000)

#define CONFIG_NAND_BOOT_BLK		(1)
#define CONFIG_NAND_BOOT_BLKCNT		(10)

/* OS placement definitions */
#if defined(CONFIG_OS_BOOT)
#define CONFIG_SNOR_OSBOOT_ADDR		(0xF8050000)
#define CONFIG_PNOR_OSBOOT_ADDR		(0x50080000)
#define CONFIG_NAND_OSBOOT_BLK		(25)
#define CONFIG_NAND_OSBOOT_BLKCNT	(10)
#endif

#endif /* __SPEAR_BOOT_H */

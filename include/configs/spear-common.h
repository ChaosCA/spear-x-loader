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

#ifndef __SPEAR_COMMON_H
#define __SPEAR_COMMON_H

#define FALSE				0
#define TRUE				(!FALSE)

/*
 * Platform specific defines (SPEAr)
 */
#if defined(CONFIG_SPEAR3XX) || defined(CONFIG_SPEAR600)

#define CONFIG_SPEAR_SMIBASE		(0xFC000000)
#define CONFIG_SPEAR_SCBASE		(0xFCA00000)
#define CONFIG_SPEAR_MISCBASE		(0xFCA80000)
#define CONFIG_SPEAR_MPMCBASE		(0xFC600000)
#define CONFIG_SPEAR_SERIAL0		(0xD0000000)

#elif defined(CONFIG_SPEAR1300)

#define CONFIG_SPEAR_SMIBASE		(0xFC000000)
#define CONFIG_SPEAR_MISCBASE		(0xE0700000)
#define CONFIG_SPEAR_MPMCBASE		(0xEC000000)
#define CONFIG_SPEAR_SERIAL0		(0xD0000000)

#endif

#define PNOR_WIDTH_8			0
#define	PNOR_WIDTH_16			1
#define	PNOR_WIDTH_32			2
#define PNOR_WIDTH_NUM			3
#define PNOR_WIDTH_SEARCH		0xff

/*
 * Boot OS image by default if U-Boot is not found
 */
#define CONFIG_OS_BOOT

#if defined(CONFIG_OS_BOOT)

#define CONFIG_BOOT_PARAMS_ADDR		(0x00000100)
#define CONFIG_NR_DRAM_BANKS		(1)
#define CONFIG_MEM_START		(0x00000000)
#define CONFIG_MEM_SIZE			(0x04000000)

#define CONFIG_PL011_CLOCK		(48 * 1000 * 1000)
#define CONFIG_CONS_INDEX		0
#define CONFIG_BAUDRATE			115200
#define CONFIG_PL01x_PORTS		{ (void *)CONFIG_SPEAR_SERIAL0 }

#endif

#endif /* __SPEAR_COMMON_H */

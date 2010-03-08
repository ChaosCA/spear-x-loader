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

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * High Level Configuration Options
 * (easy to change)
 */
#define CONFIG_SPEAR600			1

/*
 * Please select/define only one of the following
 * Each definition corresponds to a supported DDR chip.
 * DDR configuration is based on the following selection
 */
#define CONFIG_DDR_MT47H64M16		0
#define CONFIG_DDR_MT47H32M16		1
#define CONFIG_DDR_MT47H128M8		0

/*
 * Synchronous/Asynchronous operation of DDR
 *
 * Select CONFIG_DDR_2HCLK for DDR clk = 333MHz, synchronous operation
 * Select CONFIG_DDR_HCLK for DDR clk = 166MHz, synchronous operation
 * Select CONFIG_DDR_PLL2 for DDR clk = PLL2, asynchronous operation
 */
#define CONFIG_DDR_2HCLK		1
#define CONFIG_DDR_HCLK			0
#define CONFIG_DDR_PLL2			0

/*
 * Machine type for SPEAr600
 */
#define CONFIG_MACH_TYPE		(2236)
#define CONFIG_BOOTARGS			\
		"console=ttyS0 mem=128M root=/dev/mtdblock8 rootfstype=jffs2"

/*
 * Platform specific defines (SPEAr)
 *
 * Strictly SPEAr300 specific. Do not modify any of the below definitins. All
 * these definitions are derived from SPEAr600 hardware support
 */
#define CONFIG_SPEAR_MPMCREGS		(100)

#define CONFIG_SPEAR_BOOTSTRAPCFG	(0xFCA80000)
#define CONFIG_SPEAR_BOOTSTRAPSHFT	(16)
#define CONFIG_SPEAR_BOOTSTRAPMASK	(0xB)
#define CONFIG_SPEAR_ONLYSNORBOOT	(0xA)
#define CONFIG_SPEAR_NORNANDBOOT	(0xB)
#define CONFIG_SPEAR_NORNAND8BOOT	(0x8)
#define CONFIG_SPEAR_NORNAND16BOOT	(0x9)
#define CONFIG_SPEAR_USBBOOT		(0x8)

/*
 * xxx_boot_supported macro defines whether a booting type is supported
 * or not. Modify/Add to only these macros to define new boot types
 */
#define boot_bypass_supported()		1
#define usb_boot_supported()		1
#define pcie_boot_supported()		0
#define snor_boot_supported()		1
#define nand_boot_supported()		1
#define pnor_boot_supported()		0
#define tftp_boot_supported()		0
#define uart_boot_supported()		0
#define spi_boot_supported()		0
#define i2c_boot_supported()		0

#include <configs/spear-common.h>
#include <configs/spear-boot.h>

#endif /* __CONFIG_H */

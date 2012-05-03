/*
 * (C) Copyright 2009
 * Vipin Kumar, ST Micoelectronics, vipin.kumar@st.com.
 * Vipul Kumar Samar, ST Micoelectronics, vipulkumar.samar@st.com.
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
#define CONFIG_SPEAR1310		1
#define CONFIG_SPEAR13XX		1

/*
 * Please select/define only one of the following
 * Each definition corresponds to a supported DDR chip.
 * DDR configuration is based on the following selection
 */
#define CONFIG_DDR_MT41J64M16		0
#define CONFIG_DDR_MT41J256M8		1

/*
 * DDR frequency selection
 *
 * Select CONFIG_DDR_PLL1 for synchronous operation
 * Select CONFIG_DDR_PLL4 for asynchronous operation
 */
#define CONFIG_DDR_PLL1			0
#define CONFIG_DDR_PLL4			1

/* Maximum supported DDR size */
#define PHYS_SDRAM_MAXSIZE		(1 * 1024 * 1024 * 1024)

/*
 * DDR frequency selection using PLL4
 * Select(define to 1) only one out of following list
 */
#define CONFIG_DDR_FREQ_400		0
#define CONFIG_DDR_FREQ_533		1

/*
 * DDR ECC Enable selection.
 * Select(define to 1).
 */
#define DDR_ECC_ENABLE			0
#define CONFIG_C3_DDR_INIT		0

/*
 * Machine type for SPEAr1310
 */
#define CONFIG_MACH_TYPE		(2881)
#define CONFIG_BOOTARGS			\
		"console=ttyAMA0,115200 mem=1G root=/dev/mtdblock3 " \
		"rootfstype=jffs2"

/*
 * Platform specific defines (SPEAr)
 *
 * Strictly SPEAr300 specific. Do not modify any of the below definitins. All
 * these definitions are derived from SPEAr300 hardware support
 */
#define CONFIG_SPEAR_MPMCBASE		(0xEC000000)
#define CONFIG_SPEAR_MPMCREGS		(208)

#define CONFIG_SPEAR_BOOTSTRAPCFG	(0xE0700004)
	#define CONFIG_SPEAR_BOOTSTRAPSHFT	(0)
	#define CONFIG_SPEAR_BOOTSTRAPMASK	(0xF)
	#define CONFIG_SPEAR_BOOTBYPASS		(0x0)
	#define CONFIG_SPEAR_SNORBOOT_DEFUSBD	(0x1)
	#define CONFIG_SPEAR_NANDBOOT_DEFUSBD	(0x2)
	#define CONFIG_SPEAR_PNOR8BOOT_DEFUSBD	(0x3)
	#define CONFIG_SPEAR_PNOR16BOOT_DEFUSBD	(0x4)
	#define CONFIG_SPEAR_UARTBOOT		(0x5)
	#define CONFIG_SPEAR_PCIEBOOT		(0x6)
	#define CONFIG_SPEAR_SATABOOT		(0x7)
	#define CONFIG_SPEAR_USBBOOT		(0x8)
	#define CONFIG_SPEAR_SNORBOOT_DEFUART	(0x9)
	#define CONFIG_SPEAR_NANDBOOT_DEFUART	(0xA)
	#define CONFIG_SPEAR_PNOR8BOOT_DEFUART	(0xB)
	#define CONFIG_SPEAR_PNOR16BOOT_DEFUART	(0xC)
	#define CONFIG_SPEAR_MMCBOOT		(0xD)
	/* not supported */
	#define CONFIG_SPEAR_TFTPBOOT		(0xF)
	#define CONFIG_SPEAR_I2CBOOT		(0xF)
	#define CONFIG_SPEAR_SPIBOOT		(0xF)

/*
 * xxx_boot_supported macro defines whether a booting type is supported
 * or not. Modify/Add to only these macros to define new boot types
 */
#define boot_bypass_supported()         1
#define usb_boot_supported()		1
#define pcie_boot_supported()           0
#define snor_boot_supported()           1
#define nand_boot_supported()           1
#define pnor_boot_supported()           1
#define tftp_boot_supported()           0
#define uart_boot_supported()           1
#define spi_boot_supported()            0
#define i2c_boot_supported()            0
#define mmc_boot_supported()            1

#include <configs/spear-common.h>
#include <configs/spear-boot.h>

#endif

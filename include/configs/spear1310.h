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
#define CONFIG_SPEAR1310		1
#define CONFIG_SPEAR13XX		1

/*
 * Please select/define only one of the following
 * Each definition corresponds to a supported DDR chip.
 * DDR configuration is based on the following selection
 */
#define CONFIG_DDR_MT41J64M16		1
#define CONFIG_DDR_MT41J256M8		0

/*
 * DDR frequency selection
 *
 * Select CONFIG_DDR_PLL1 for synchronous operation
 * Select CONFIG_DDR_PLL4 for asynchronous operation
 */
#define CONFIG_DDR_PLL1			0
#define CONFIG_DDR_PLL4			1

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
#define USE_C3				1	/* In this platform always use C3 to init memory */

/*
 * Machine type for SPEAr1310
 */
#define CONFIG_MACH_TYPE		(2881)
#define CONFIG_BOOTARGS			\
		"console=ttyAMA0,115200 mem=128M root=/dev/mtdblock3 " \
		"rootfstype=jffs2"

/*
 * Platform specific defines (SPEAr)
 *
 * Strictly SPEAr300 specific. Do not modify any of the below definitins. All
 * these definitions are derived from SPEAr300 hardware support
 */
#define CONFIG_SPEAR_MPMCBASE		(0xEC000000)
#define CONFIG_SPEAR_MPMCREGS		(201)

#define CONFIG_SPEAR_BOOTSTRAPCFG	(0xE0700004)
#define CONFIG_SPEAR_BOOTSTRAPSHFT	(0)
#define CONFIG_SPEAR_BOOTSTRAPMASK	(0xF)
#define CONFIG_SPEAR_BOOTBYPASS		(0x0)
#define CONFIG_SPEAR_SNORBOOT		(0x1)
#define CONFIG_SPEAR_NANDBOOT		(0x2)
#define CONFIG_SPEAR_PNORBOOT		(0x3)
#define CONFIG_SPEAR_I2CBOOT		(0x4)
#define CONFIG_SPEAR_UARTBOOT		(0x5)
#define CONFIG_SPEAR_PCIEBOOT		(0x6)
#define CONFIG_SPEAR_USBBOOT		(0x8)
#define CONFIG_SPEAR_TFTPBOOT		(0x9)
#define CONFIG_SPEAR_SPIBOOT		(0xA)

/*
 * xxx_boot_supported macro defines whether a booting type is supported
 * or not. Modify/Add to only these macros to define new boot types
 */
#define boot_bypass_supported()		1
#define usb_boot_supported()		1
#define pcie_boot_supported()		1
#define snor_boot_supported()		1
#define nand_boot_supported()		1
#define pnor_boot_supported()		1
#define tftp_boot_supported()		0
#define uart_boot_supported()		0
#define spi_boot_supported()		0
#define i2c_boot_supported()		0

#include <configs/spear-common.h>
#include <configs/spear-boot.h>

#endif

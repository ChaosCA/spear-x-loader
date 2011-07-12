/*
 * (C) Copyright 2000-2009
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/spr13xx_defs.h>
#include <asm/arch/spr13xx_misc.h>
#include <asm/arch/spr13xx_mpmc.h>

extern void snor_init(void);

/**
 * plat_ddr_init:
 */
void plat_ddr_init(void)
{
	struct misc_regs *misc_p = (struct misc_regs *)CONFIG_SPEAR_MISCBASE;

	writel(PAD_VREF | DATA_PROGB | DATA_PROGA | CLK_PROGB | CLK_PROGA |
		CTRL_PROGB | CTRL_PROGA, &misc_p->ddr_pad_cfg);

	lvl_write();
	lvl_gatetrn();
	lvl_read();
}

/**
 * soc_init:
 */
void soc_init(void)
{
	/* Bringing ARM2 out of the PHolding loop */
	cpu2_wake();

	/*
	 * Enabling the filtering at L2 cache controller level.
	 * This code limits the address range which can be accessed through the
	 * two ports coming out of ARM core.
	 *
	 * Port 0 access -> All - (Port 1 access range)
	 * Port 1 access -> 0x0 - 0x4000,0000 (1GB) all DDR space
	 * available on evaluation board
	 */

	/* L2 cache controller filtering registers */
	writel(CONFIG_SPEAR_FLTEND, CONFIG_SPEAR_L2CCBASE + 0xC04);
	writel(CONFIG_SPEAR_FLTSTART | 0x1, CONFIG_SPEAR_L2CCBASE + 0xC00);
}

/**
 * xxx_boot_selected:
 *
 * return TRUE if the particular booting option is selected
 * return FALSE otherwise
 */
static u32 read_bootstrap(void)
{
	return ((readl(CONFIG_SPEAR_BOOTSTRAPCFG) >> CONFIG_SPEAR_BOOTSTRAPSHFT)
			& CONFIG_SPEAR_BOOTSTRAPMASK);
}

int boot_bypass_selected(void)
{
	if (boot_bypass_supported()) {
		/* Check whether boot bypass is selected */
		if (CONFIG_SPEAR_BOOTBYPASS == read_bootstrap()) {
			return TRUE;
		}
	}
	return FALSE;
}

int snor_boot_selected(void)
{
	if (snor_boot_supported()) {
		/* Check whether SNOR boot is selected */
#ifdef CONFIG_SPEAR1340
		if ((CONFIG_SPEAR_SNORBOOT_DEFUART == read_bootstrap()) ||
			(CONFIG_SPEAR_SNORBOOT_DEFUSBD == read_bootstrap()))
#else
		if (CONFIG_SPEAR_SNORBOOT == read_bootstrap())
#endif
			return TRUE;
	}
	return FALSE;
}

int nand_boot_selected(void)
{
	if (nand_boot_supported()) {
		/* Check whether NAND boot is selected */
#ifdef CONFIG_SPEAR1340
		if ((CONFIG_SPEAR_NANDBOOT_DEFUART == read_bootstrap()) ||
			(CONFIG_SPEAR_NANDBOOT_DEFUSBD == read_bootstrap()))
#else
		if (CONFIG_SPEAR_NANDBOOT == read_bootstrap())
#endif
			return TRUE;
	}
	return FALSE;
}

int pnor_boot_selected(void)
{
	if (pnor_boot_supported()) {
		/* Check whether SNOR boot is selected */
#ifdef CONFIG_SPEAR1340
		if ((CONFIG_SPEAR_PNORBOOT_DEFUART == read_bootstrap()) ||
			(CONFIG_SPEAR_PNORBOOT_DEFUSBD == read_bootstrap()))
#else
		if (CONFIG_SPEAR_SNORBOOT == read_bootstrap())
#endif
			return TRUE;
	}
	return FALSE;
}

int usb_boot_selected(void)
{
	if (usb_boot_supported()) {
		/* Check whether SNOR boot is selected */
		if (CONFIG_SPEAR_USBBOOT == read_bootstrap())
			return TRUE;
	}
	return FALSE;
}

int mmc_boot_selected(void)
{
	if (mmc_boot_supported()) {
		/* Check whether MMC boot is selected */
#ifdef CONFIG_SPEAR1340
		if (CONFIG_SPEAR_MMCBOOT == read_bootstrap())
			return TRUE;
#endif
	}
	return FALSE;
}

int pcie_boot_selected(void)
{
	if (pcie_boot_supported()) {
		/* Check whether PCIE boot is selected */
		if (CONFIG_SPEAR_PCIEBOOT == read_bootstrap()) {
			return TRUE;
		}
	}
	return FALSE;
}

int tftp_boot_selected(void)
{
	if (tftp_boot_supported()) {
		/* Check whether SNOR boot is selected */
		if (CONFIG_SPEAR_TFTPBOOT == read_bootstrap()) {
			return TRUE;
		}
	}
	return FALSE;
}

int uart_boot_selected(void)
{
	if (uart_boot_supported()) {
		/* Check whether SNOR boot is selected */
		if (CONFIG_SPEAR_UARTBOOT == read_bootstrap()) {
			return TRUE;
		}
	}
	return FALSE;
}

int spi_boot_selected(void)
{
	if (spi_boot_supported()) {
		/* Check whether SNOR boot is selected */
		if (CONFIG_SPEAR_SPIBOOT == read_bootstrap()) {
			return TRUE;
		}
	}
	return FALSE;
}

int i2c_boot_selected(void)
{
	if (i2c_boot_supported()) {
		/* Check whether SNOR boot is selected */
		if (CONFIG_SPEAR_I2CBOOT == read_bootstrap()) {
			return TRUE;
		}
	}
	return FALSE;
}

/**
 * get_pnor_width:
 *
 * Get Parallel NOR flash width.
 * @return PNOR_WIDTH_{8|16|32|SEARCH}
 */
u32 get_pnor_width(void)
{
	return PNOR_WIDTH_SEARCH;
}

void plat_late_init(void)
{
}

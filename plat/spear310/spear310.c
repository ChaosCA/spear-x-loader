/*
 * (C) Copyright 2000-2009
 * Viresh Kumar, ST Microelectronics, viresh.kumar@st.com
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
#include <asm/arch/spr_misc.h>
#include <asm/arch/spr_defs.h>

/**
 * plat_ddr_init:
 */
void plat_ddr_init(void)
{
	struct misc_regs *misc_p = (struct misc_regs *)CONFIG_SPEAR_MISCBASE;
	u32 ddrpad;
	u32 core3v3, ddr1v8;

	/* DDR pad register configurations */
	ddrpad = readl(&misc_p->ddr_pad);
	ddrpad &= ~DDR_PAD_CNF_MSK;

#if (CONFIG_DDR_HCLK)
	ddrpad |= 0x7AAD;
#elif (CONFIG_DDR_2HCLK)
	ddrpad |= 0x7AA5;
#elif (CONFIG_DDR_PLL2)
	ddrpad |= 0x7AA5;
#endif
	writel(ddrpad, &misc_p->ddr_pad);

	/* Compensation register configurations */
	core3v3 = readl(&misc_p->core_3v3_compensation);
	core3v3 &= 0x02fffff0;
	core3v3 |= 0x78000008;
	writel(core3v3, &misc_p->core_3v3_compensation);

	ddr1v8 = readl(&misc_p->ddr_1v8_compensation);
	ddr1v8 &= 0x02fffff0;
	ddr1v8 |= 0x78000008;
	writel(ddr1v8, &misc_p->ddr_1v8_compensation);
}

/**
 * soc_init:
 */
void soc_init(void)
{
	struct misc_regs *misc_p = (struct misc_regs *)CONFIG_SPEAR_MISCBASE;

	writel(readl(&misc_p->amem_cfg_ctrl) | PORT_CLK_EN,
	       &misc_p->amem_cfg_ctrl);

	writel(0x2f7bc210, &misc_p->plgpio3_pad_prg);
	writel(0x017bdef6, &misc_p->plgpio4_pad_prg);
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

int snor_boot_selected(void)
{
	u32 bootstrap = read_bootstrap();

	/* Check whether SNOR boot is selected */
	if (snor_boot_supported() && (bootstrap == CONFIG_SPEAR_SNORBOOT))
		return TRUE;

	return FALSE;
}

int nand_boot_selected(void)
{
	u32 bootstrap = read_bootstrap();

	/* Check whether NAND boot is selected */
	if (nand_boot_supported() && (bootstrap == CONFIG_SPEAR_NANDBOOT))
		return TRUE;

	return FALSE;
}

int pnor_boot_selected(void)
{
	u32 bootstrap = read_bootstrap();

	/* Check whether Parallel NOR boot is selected */
	if (pnor_boot_supported() && (bootstrap == CONFIG_SPEAR_PNORBOOT))
		return TRUE;

	return FALSE;
}

int usb_boot_selected(void)
{
	u32 bootstrap = read_bootstrap();

	/* Check whether USB boot is selected */
	if (usb_boot_supported() && (bootstrap == CONFIG_SPEAR_USBBOOT))
		return TRUE;

	return FALSE;
}

int tftp_boot_selected(void)
{
	return FALSE;
}

int uart_boot_selected(void)
{
	return FALSE;
}

int spi_boot_selected(void)
{
	/* SPI boot is not selected in any SPEAr3xx revision */
	return FALSE;
}

int i2c_boot_selected(void)
{
	/* I2C boot is not selected in any SPEAr3xx revision */
	return FALSE;
}

int mmc_boot_selected(void)
{
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
	u32 pnorwidth = ((readl(CONFIG_SPEAR_BOOTSTRAPCFG) >> CONFIG_SPEAR_PNORSHFT)
			& CONFIG_SPEAR_PNORMASK);

	switch (pnorwidth) {
	case CONFIG_SPEAR_PNOR8BOOT:
		return PNOR_WIDTH_8;

	case CONFIG_SPEAR_PNOR16BOOT:
		return PNOR_WIDTH_16;

	case CONFIG_SPEAR_PNOR32BOOT:
		return PNOR_WIDTH_32;

	default:
		return PNOR_WIDTH_SEARCH;
	}
}

/**
 * plat_late_init:
 *
 * Platform related initialisations. Late initialisations are performed just
 * before passing the control to second level boot image
 */
void plat_late_init(void)
{
	spear_late_init();

	/*
	 * Bad fix : has to be removed later
	 * BootROM function flashdetectandinit initializes function enable
	 * register to incorrect value. Specific to SPEAr310
	 */
	writel(FUNCENBDEF, (u32 *)CONFIG_SPEAR_FUNCENB);
}

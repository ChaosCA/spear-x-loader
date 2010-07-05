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
#include <asm/io.h>
#include <asm/arch/spr13xx_misc.h>
#include <asm/arch/spr13xx_defs.h>

static void ddr_clock_init(void)
{
	struct misc_regs *misc_p = (struct misc_regs *)CONFIG_SPEAR_MISCBASE;
	u32 perip_clkcfg, perip2_clkenb, perip2_swrst;

	perip_clkcfg = readl(&misc_p->perip_clk_cfg);

#if (CONFIG_DDR_PLL4)
	perip_clkcfg |= MPMC_CLK_PLL4;
#elif (CONFIG_DDR_PLL1)
	perip_clkcfg &= ~MPMC_CLK_PLL4;
#endif
	writel(perip_clkcfg, &misc_p->perip_clk_cfg);

	perip2_clkenb = readl(&misc_p->perip2_clk_enb);
	perip2_clkenb |= DDR_CTRL_CLKEN | DDR_CORE_CLKEN;
	writel(perip2_clkenb, &misc_p->perip2_clk_enb);

	perip2_swrst = readl(&misc_p->perip2_sw_rst);
	perip2_swrst |= DDR_CTRL_CLKEN | DDR_CORE_CLKEN;
	writel(perip2_swrst, &misc_p->perip2_sw_rst);

	perip2_swrst = readl(&misc_p->perip2_sw_rst);
	perip2_swrst &= ~(DDR_CTRL_CLKEN | DDR_CORE_CLKEN);
	writel(perip2_swrst, &misc_p->perip2_sw_rst);
}

static void mpmc_init_values(void)
{
	u32 i;
	u32 *mpmc_reg_p = (u32 *)CONFIG_SPEAR_MPMCBASE;
	u32 *mpmc_val_p = &mpmc_conf_vals[0];

	for (i = 0; i < CONFIG_SPEAR_MPMCREGS; i++, mpmc_reg_p++, mpmc_val_p++)
		writel(*mpmc_val_p, mpmc_reg_p);

	mpmc_reg_p = (u32 *)CONFIG_SPEAR_MPMCBASE;
	/*
	 * MPMC register25 rewrite
	 * MPMC controller start
	 */
	writel(0x03030305, &mpmc_reg_p[25]);
	writel(0x01000101, &mpmc_reg_p[11]);

	while (!(readl(&mpmc_reg_p[105]) & 0x100))
		;
}

static void mpmc_init(void)
{
	/* Clock related settings for DDR */
	ddr_clock_init();

	/* Initialize mpmc register values */
	mpmc_init_values();

	/*
	 * DDR pad register bits are different for different SoCs
	 * Compensation values are also handled separately
	 */
	plat_ddr_init();

}

static void pll_init(void)
{
	struct misc_regs *misc_p = (struct misc_regs *)CONFIG_SPEAR_MISCBASE;
	u32 usbphycfg;

	/* Initialize PLLs */
	writel(FREQ_1000, &misc_p->pll1_frq);
	writel(readl(&misc_p->pll1_ctr) | PLLENABLE, &misc_p->pll1_ctr);

	writel(FREQ_125, &misc_p->pll2_frq);
	writel(readl(&misc_p->pll2_ctr) | PLLENABLE, &misc_p->pll2_ctr);

	writel(FREQ_332, &misc_p->pll3_frq);
	writel(readl(&misc_p->pll3_ctr) | PLLENABLE, &misc_p->pll3_ctr);

	writel(FREQ_400, &misc_p->pll4_frq);
	/* strobing required for pll4 */
	writel(0x60A, &misc_p->pll4_ctr);
	writel(0x60E, &misc_p->pll4_ctr);
	writel(0x606, &misc_p->pll4_ctr);
	writel(0x60E, &misc_p->pll4_ctr);

	usbphycfg = readl(&misc_p->usbphy_gen_cfg);
	usbphycfg &= ~(COMMON_PWDN | USBPHY_POR);
	usbphycfg |= USBPHY_RST;

	writel(usbphycfg, &misc_p->usbphy_gen_cfg);
	while (!(readl(&misc_p->usbphy_gen_cfg) & USB_PLL_LOCK))
		;
	writel(readl(&misc_p->usbphy_gen_cfg) |
		AUTOPPD_ON_OVRCURR | UTMI_XFER_RST0 | UTMI_XFER_RST1 |
		UTMI_XFER_RST0 | USB_BURST_INCR16,
		&misc_p->usbphy_gen_cfg);

	/* wait for pll locks */
	while (!(readl(&misc_p->pll1_ctr) & PLLLOCK))
		;
	while (!(readl(&misc_p->pll2_ctr) & PLLLOCK))
		;
	while (!(readl(&misc_p->pll3_ctr) & PLLLOCK))
		;
	while (!(readl(&misc_p->pll4_ctr) & PLLLOCK))
		;
}

static void mac_init(void)
{
}

static void sys_init(void)
{
	struct misc_regs *misc_p = (struct misc_regs *)CONFIG_SPEAR_MISCBASE;
	u32 sysclkctrl;

	/* Set system state to SLOW */
	sysclkctrl = readl(&misc_p->sys_clk_ctrl);
	sysclkctrl &= ~SYS_MODE_MASK;
	sysclkctrl |= XTAL_TIMEOUT_ENB | PLL_TIMEOUT_ENB | SYS_MODE_REQ_SLOW;
	writel(sysclkctrl, &misc_p->sys_clk_ctrl);

	writel(PLL_TIM, &misc_p->sys_clk_plltimer);
	writel(OSCI_TIM, &misc_p->sys_clk_oscitimer);
	
	/* Initialize PLLs */
	pll_init();

	mac_init();

	writel(0x0, &misc_p->perip_clk_cfg);

	/* Set system state to NORMAL */
	sysclkctrl = readl(&misc_p->sys_clk_ctrl);
	sysclkctrl &= ~SYS_MODE_MASK;
	sysclkctrl |= XTAL_TIMEOUT_ENB | PLL_TIMEOUT_ENB | SYS_MODE_REQ_NORMAL;
	writel(sysclkctrl, &misc_p->sys_clk_ctrl);

	/* Wait for system to switch to normal mode */
	while ((readl(&misc_p->sys_clk_ctrl) & SYS_STATE_MASK) !=
			SYS_STATE_NORMAL);
}

void lowlevel_init(void)
{
	struct misc_regs *misc_p = (struct misc_regs *)CONFIG_SPEAR_MISCBASE;

	/* Initialize PLLs */
	sys_init();

#if defined(CONFIG_OS_BOOT)
	writel(readl(&misc_p->perip1_clk_enb) | UART_CLKEN,
			&misc_p->perip1_clk_enb);
#endif

	/* Enable IPs (release reset) */
	writel(PERIPH1_RST_ALL, &misc_p->perip1_sw_rst);
	writel(PERIPH2_RST_ALL, &misc_p->perip2_sw_rst);

	/* Initialize MPMC */
	mpmc_init();

	/* SoC specific initialization */
	soc_init();
}

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
#include <pl061.h>
#include <ddrtest.h>

static void ddr_clock_init(void)
{
	struct misc_regs *misc_p = (struct misc_regs *)CONFIG_SPEAR_MISCBASE;
	u32 perip_clkcfg, perip2_clkenb, perip2_swrst;
#ifdef CONFIG_SPEAR1310
	u32 mpmc_cfg;
#endif

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

#ifdef CONFIG_SPEAR1310
	/*
	 * Following modifies write access as non-bufferable
	 * and read to happen word by word. Without this
	 * dependent write-read are happening out of order
	 * resulting in Linux crash.
	 */
	mpmc_cfg = readl(&misc_p->mpmc_cfg);
	mpmc_cfg |= 0x05000000;
	mpmc_cfg &= ~0xf0f;
	mpmc_cfg |= 0x101;
	writel(mpmc_cfg, &misc_p->mpmc_cfg);
#endif

	/*
	 * MISC compensation_ddr_cfg before mpmc reset
	 * disable automatic ddr pad compensation
	 * use fixed comzcp=0000 and comzcn=0000
	 */
#if (defined(CONFIG_SPEAR1340) || defined(CONFIG_SPEAR1310))
	u32 pad_pu_cfg_3, pad_pd_cfg_3;

	/* MISC 0x710 update=0, enb=0, encomzc=0 */
	writel(0x00000000, &misc_p->compensation_ddr_cfg);

	/*
	 * The code below modifies pad_pu_cfg_3 and pad_pd_cfg_3
	 * registers settings in order to add support for SPEAr1340
	 * DDR Board Modifications:
	 * - DDR_CLKEN (XGPIO 88: PullDown = 1, PullUp = 0)
	 * - DDR_RESET (XGPIO 89: PullDown = 1, PullUp = 0)
	 */
	pad_pu_cfg_3 = readl(&misc_p->pad_pu_cfg_3);
	pad_pu_cfg_3 |= (PAD_88_PU_CFG | PAD_89_PU_CFG);
	writel(pad_pu_cfg_3, &misc_p->pad_pu_cfg_3);

	pad_pd_cfg_3 = readl(&misc_p->pad_pd_cfg_3);
	pad_pd_cfg_3 &= PAD_88_PD_CFG;
	pad_pd_cfg_3 &= PAD_89_PD_CFG;
	writel(pad_pd_cfg_3, &misc_p->pad_pd_cfg_3);

#else

#define PLGPIO_2_3_DIR_SEL		(void *)(CONFIG_SPEAR_GPIO0_BASE + 0x400)
#define PLGPIO_2_3_RW_DATA		(void *)(CONFIG_SPEAR_GPIO0_BASE + 0x3FC)

	/* Enable the GPIO Clock Enable */
	writel(readl(&misc_p->perip1_clk_enb) | GPIO0_CLKEN,
			&misc_p->perip1_clk_enb);
	/*
	 * The code below modifies plgpio2 and plgpio3
	 * registers settings in order to add support for SPEAr13xx
	 * DDR Board Modifications:
	 * - DDR_CLKEN (PLGPIO 2: PullDown = 1, PullUp = 0)
	 * - DDR_RESET (PLGPIO 3: PullDown = 1, PullUp = 0)
	 */

	/* Set up the plgpio direction as output */
	writel(readl(PLGPIO_2_3_DIR_SEL) | 0xc, PLGPIO_2_3_DIR_SEL);
	/* Set up the Value for DDR_CLKEN and DDR_RESET */
	writel(readl(PLGPIO_2_3_RW_DATA) | (PLGPIO3_PU_CFG | PLGPIO2_PU_CFG) ,
			PLGPIO_2_3_RW_DATA);
	writel(readl(PLGPIO_2_3_RW_DATA) & (PLGPIO3_PD_CFG & PLGPIO2_PD_CFG),
			PLGPIO_2_3_RW_DATA);

#endif
	perip2_swrst = readl(&misc_p->perip2_sw_rst);
	perip2_swrst |= DDR_CTRL_CLKEN | DDR_CORE_CLKEN;
	writel(perip2_swrst, &misc_p->perip2_sw_rst);

	perip2_swrst = readl(&misc_p->perip2_sw_rst);
	perip2_swrst &= ~(DDR_CTRL_CLKEN | DDR_CORE_CLKEN);
	writel(perip2_swrst, &misc_p->perip2_sw_rst);

#if defined(CONFIG_SPEAR1310)
	/* enable MPMC ECC gasket for all AXI ports */
	writel(0x0, &misc_p->mpmc_ctr_sts);

	/* wait for turn-on */
	while ((readl(&misc_p->mpmc_ctr_sts) & 0xFFF))
		;

#endif

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
	writel(0x03070700, &mpmc_reg_p[25]);
	writel(0x01000101, &mpmc_reg_p[11]);
#if (defined(CONFIG_SPEAR1340) || defined(CONFIG_SPEAR1310))
	while (!(readl(&mpmc_reg_p[105]) & 0x200))
#else
	while (!(readl(&mpmc_reg_p[105]) & 0x100))
#endif
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
#if CONFIG_DDR_FREQ_400
	writel(FREQ_400, &misc_p->pll4_frq);
#elif CONFIG_DDR_FREQ_533
	writel(FREQ_533, &misc_p->pll4_frq);
#else
#error Define one of the valid DDR frequency(CONFIG_DDR_FREQ_[533/400])
#endif
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
	/* AHB Master Burst is not supported for SPEAr1340 */

	usbphycfg = readl(&misc_p->usbphy_gen_cfg);

	usbphycfg |= AUTOPPD_ON_OVRCURR | UTMI_XFER_RST0 | UTMI_XFER_RST1 |
		UTMI_XFER_RST2;
#if (!defined(CONFIG_SPEAR1340) && !defined(CONFIG_SPEAR1310))
	usbphycfg |= USB_BURST_INCR16;
#endif
	writel(usbphycfg, &misc_p->usbphy_gen_cfg);

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

#if (defined(CONFIG_SPEAR1340) || defined(CONFIG_SPEAR1310))
	u32 plgpio_enb_3;
	/*
	 * The code below modifies  plgpio_enb_3 register
	 * settings in order to add support for SPEAr1340
	 * rev AB DDR Board Modifications setting in output
	 * GPIOs 88 and 89 on GPIO controller.
	 */
	plgpio_enb_3 = readl(PLGPIO_ENB_3);
	plgpio_enb_3 &= ~(PLGPIO_88_CFG | PLGPIO_89_CFG);
	writel(plgpio_enb_3, PLGPIO_ENB_3);

	u32 pad_pu_cfg_1, pad_pd_cfg_1;

	/*
	 * The code below modifies pad_pu_cfg_1 and pad_pd_cfg_1
	 * registers settings in order to add support for SPEAr1340
	 * DDR Board Modifications:
	 * - DDR_PHY_1v2 (XGPIO 21: PullDown = 1, PullUp = 0)
	 * - DDR_PHY_1v5 (XGPIO 22: PullDown = 1, PullUp = 0)
	 */
	pad_pu_cfg_1 = readl(&misc_p->pad_pu_cfg_1);
	pad_pu_cfg_1 |= (PAD_21_PU_CFG | PAD_22_PU_CFG);
	writel(pad_pu_cfg_1, &misc_p->pad_pu_cfg_1);

	pad_pd_cfg_1 = readl(&misc_p->pad_pd_cfg_1);
	pad_pd_cfg_1 &= PAD_21_PD_CFG;
	pad_pd_cfg_1 &= PAD_22_PD_CFG;
	writel(pad_pd_cfg_1, &misc_p->pad_pd_cfg_1);
#endif

	/* Initialize PLLs */
	pll_init();

	mac_init();

	/* Set system state to NORMAL */
	sysclkctrl = readl(&misc_p->sys_clk_ctrl);
	sysclkctrl &= ~SYS_MODE_MASK;
	sysclkctrl |= XTAL_TIMEOUT_ENB | PLL_TIMEOUT_ENB | SYS_MODE_REQ_NORMAL;
	writel(sysclkctrl, &misc_p->sys_clk_ctrl);

	/* Wait for system to switch to normal mode */
	while ((readl(&misc_p->sys_clk_ctrl) & SYS_STATE_MASK) !=
			SYS_STATE_NORMAL);
}

#ifdef CONFIG_SPEAR900_LCAD
static void pl061_init(void)
{
	struct misc_regs *misc_p = (struct misc_regs *)CONFIG_SPEAR_MISCBASE;

	writel(readl(&misc_p->perip1_clk_enb) | GPIO1_CLKEN,
			&misc_p->perip1_clk_enb);
}

/*
 * SPEAr900 LCAD board requires a GPIO to be set in order to
 * power on...
 */
static void set_lcad_power_on(void)
{
	pl061_set_value(CONFIG_SPEAR_GPIO1_BASE, GPIO1_1, 1);
	pl061_set_out(CONFIG_SPEAR_GPIO1_BASE, GPIO1_1);
	pl061_set_value(CONFIG_SPEAR_GPIO1_BASE, GPIO1_1, 1);
}
#endif

#ifdef CONFIG_SPEAR1340_LCAD
static void pl061_init(void)
{
	struct misc_regs *misc_p = (struct misc_regs *)CONFIG_SPEAR_MISCBASE;

	writel(readl(&misc_p->perip1_clk_enb) | GPIO0_CLKEN,
			&misc_p->perip1_clk_enb);
}

/*
 * SPEAr1340 LCAD board requires a GPIO to be set in order to
 * power on...
 */
static void set_lcad_power_on(void)
{
	pl061_set_value(CONFIG_SPEAR_GPIO0_BASE, GPIO0_3, 1);
	pl061_set_out(CONFIG_SPEAR_GPIO0_BASE, GPIO0_3);
	pl061_set_value(CONFIG_SPEAR_GPIO0_BASE, GPIO0_3, 1);
}
#endif

void ddr_memory_test(void)
{
	unsigned long  start = 0x00100000;
	unsigned long  val = DDR_TEST_SIZE;
	unsigned long  result = 0;

	/* memory test trial */
	result = probememory(start, start + val);

	if (result) {
		while (1) /* loop infinitly on ddr test error */
		;
	}
}

void lowlevel_init(void)
{
	struct misc_regs *misc_p = (struct misc_regs *)CONFIG_SPEAR_MISCBASE;
#if defined(CONFIG_SPEAR1310)
	u32 pad_dir_sel1_reg;
#endif
	/* Initialize PLLs */
	sys_init();

#if defined(CONFIG_OS_BOOT)
	writel(readl(&misc_p->perip1_clk_enb) | UART_CLKEN,
			&misc_p->perip1_clk_enb);
#endif

	/* Enable IPs (release reset) */
	writel(PERIPH1_RST_ALL, &misc_p->perip1_sw_rst);
	writel(PERIPH2_RST_ALL, &misc_p->perip2_sw_rst);

#if defined(CONFIG_SPEAR900_LCAD) || defined(CONFIG_SPEAR1340_LCAD)
	pl061_init();

	/* Power on complete */
	set_lcad_power_on();
#endif

#if defined(CONFIG_SPEAR1340)
	writel(PERIPH3_RST_ALL, &misc_p->perip3_sw_rst);
#else
	writel(RAS_RST_ALL, &misc_p->ras_sw_rst);
#endif

#if defined(CONFIG_SPEAR1310)
	pad_dir_sel1_reg = readl(&misc_p->pad_dir_sel_1);
	pad_dir_sel1_reg |= PAD_DIR_SEL_1_UART;
	writel(pad_dir_sel1_reg, &misc_p->pad_dir_sel_1);
#endif

	/* Initialize MPMC */
	mpmc_init();

#if DDRTEST_EN
	ddr_memory_test(); /* ddr memory test */
#endif

	/* SoC specific initialization */
	soc_init();
}

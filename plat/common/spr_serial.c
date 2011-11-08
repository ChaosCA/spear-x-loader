/*
 * (C) Copyright 2009
 * Shiraz Hashim, ST Micoelectronics, shiraz.hashim@st.com.
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

#ifdef CONFIG_SPEAR13XX
#include <asm/arch/spr13xx_misc.h>
#else
#include <asm/arch/spr_misc.h>
#endif

void spear_serial_init(void)
{
	struct misc_regs *const misc_p =
		(struct misc_regs *)CONFIG_SPEAR_MISCBASE;
	void *perip_clk_cfg, *perip_clk_enb;
	u32 val, uart_clk_enb;

#ifdef CONFIG_SPEAR13XX
	perip_clk_cfg = &misc_p->perip_clk_cfg;
	perip_clk_enb = &misc_p->perip1_clk_enb;
	uart_clk_enb = UART_CLKEN;
#elif defined(CONFIG_SPEAR3XX) || defined(CONFIG_SPEAR600)
	perip_clk_cfg = &misc_p->periph_clk_cfg;
	perip_clk_enb = &misc_p->periph1_clken;
	uart_clk_enb = PERIPH_UART1;
#endif

	/* choose the clk src to USB PLL (48MHz) */
	val = readl(perip_clk_cfg);
	val &= ~UARTCLK_MASK;
	val |= UARTCLK_48MHZ;
	writel(val, perip_clk_cfg);

	/* enable UART clk */
	val = readl(perip_clk_enb);
	val |= uart_clk_enb;
	writel(val, perip_clk_enb);
}

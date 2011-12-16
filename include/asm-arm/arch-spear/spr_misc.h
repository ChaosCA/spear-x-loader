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

#ifndef __MISC_REGS_H
#define __MISC_REGS_H

struct misc_regs {
	unsigned int auto_cfg_reg;		/* 0x0 */
	unsigned int armdbg_ctr_reg;		/* 0x4 */
	unsigned int pll1_cntl;			/* 0x8 */
	unsigned int pll1_frq;			/* 0xc */
	unsigned int pll1_mod;			/* 0x10 */
	unsigned int pll2_cntl;			/* 0x14 */
	unsigned int pll2_frq;			/* 0x18 */
	unsigned int pll2_mod;			/* 0x1C */
	unsigned int pll_ctr_reg;		/* 0x20 */
	unsigned int amba_clk_cfg;		/* 0x24 */
	unsigned int periph_clk_cfg;		/* 0x28 */
	unsigned int periph1_clken;		/* 0x2C */
	unsigned int soc_core_id;		/* 0x30 */
	unsigned int ras_clken;			/* 0x34 */
	unsigned int periph1_rst;		/* 0x38 */
	unsigned int periph2_rst;		/* 0x3C */
	unsigned int ras_rst;			/* 0x40 */
	unsigned int prsc1_clk_cfg;		/* 0x44 */
	unsigned int prsc2_clk_cfg;		/* 0x48 */
	unsigned int prsc3_clk_cfg;		/* 0x4C */
	unsigned int amem_cfg_ctrl;		/* 0x50 */
	unsigned int port_cfg_ctrl;		/* 0x54 */
	unsigned int reserved_1;		/* 0x58 */
	unsigned int clcd_synth_clk;		/* 0x5C */
	unsigned int irda_synth_clk;		/* 0x60 */
	unsigned int uart_synth_clk;		/* 0x64 */
	unsigned int gmac_synth_clk;		/* 0x68 */
	unsigned int ras_synth1_clk;		/* 0x6C */
	unsigned int ras_synth2_clk;		/* 0x70 */
	unsigned int ras_synth3_clk;		/* 0x74 */
	unsigned int ras_synth4_clk;		/* 0x78 */
	unsigned int arb_icm_ml1;		/* 0x7C */
	unsigned int arb_icm_ml2;		/* 0x80 */
	unsigned int arb_icm_ml3;		/* 0x84 */
	unsigned int arb_icm_ml4;		/* 0x88 */
	unsigned int arb_icm_ml5;		/* 0x8C */
	unsigned int arb_icm_ml6;		/* 0x90 */
	unsigned int arb_icm_ml7;		/* 0x94 */
	unsigned int arb_icm_ml8;		/* 0x98 */
	unsigned int arb_icm_ml9;		/* 0x9C */
	unsigned int dma_src_sel;		/* 0xA0 */
	unsigned int uphy_ctr_reg;		/* 0xA4 */
	unsigned int gmac_ctr_reg;		/* 0xA8 */
	unsigned int port_bridge_ctrl;		/* 0xAC */
	unsigned int reserved_2[4];		/* 0xB0--0xBC */
	unsigned int prc1_ilck_ctrl_reg;	/* 0xC0 */
	unsigned int prc2_ilck_ctrl_reg;	/* 0xC4 */
	unsigned int prc3_ilck_ctrl_reg;	/* 0xC8 */
	unsigned int prc4_ilck_ctrl_reg;	/* 0xCC */
	unsigned int prc1_intr_ctrl_reg;	/* 0xD0 */
	unsigned int prc2_intr_ctrl_reg;	/* 0xD4 */
	unsigned int prc3_intr_ctrl_reg;	/* 0xD8 */
	unsigned int prc4_intr_ctrl_reg;	/* 0xDC */
	unsigned int powerdown_cfg_reg;		/* 0xE0 */
	unsigned int ddr_1v8_compensation;	/* 0xE4 */
	unsigned int ddr_2v5_compensation;	/* 0xE8 */
	unsigned int core_3v3_compensation;	/* 0xEC */
	unsigned int ddr_pad;			/* 0xF0 */
	unsigned int bist1_ctr_reg;		/* 0xF4 */
	unsigned int bist2_ctr_reg;		/* 0xF8 */
	unsigned int bist3_ctr_reg;		/* 0xFC */
	unsigned int bist4_ctr_reg;		/* 0x100 */
	unsigned int bist5_ctr_reg;		/* 0x104 */
	unsigned int bist1_rslt_reg;		/* 0x108 */
	unsigned int bist2_rslt_reg;		/* 0x10C */
	unsigned int bist3_rslt_reg;		/* 0x110 */
	unsigned int bist4_rslt_reg;		/* 0x114 */
	unsigned int bist5_rslt_reg;		/* 0x118 */
	unsigned int syst_error_reg;		/* 0x11C */
	unsigned char reserved_3[16];		/* 0x120--0x12C */
	unsigned int plgpio0_pad_prg;		/* 0x130 */
	unsigned int plgpio1_pad_prg;		/* 0x134 */
	unsigned int plgpio2_pad_prg;		/* 0x138 */
	unsigned int plgpio3_pad_prg;		/* 0x13C */
	unsigned int plgpio4_pad_prg;		/* 0x140 */
};

/* SYNTH_CLK value*/
#define SYNTH23			0x00020003

/* PLLx_FRQ value */
#if defined(CONFIG_SPEAR3XX)
#define FREQ_332		0xA600010C
#define FREQ_266		0x8500010C
#elif defined(CONFIG_SPEAR600)
#define FREQ_332		0xA600010F
#define FREQ_266		0x8500010F
#endif

/* AMBA_CLK_CFG value*/
#define CLK_RATIO_11		0x00000000    /* division factor 1:1 */
#define CLK_RATIO_12		0x00000001    /* division factor 1:2 */
#define CLK_RATIO_13		0x00000002    /* division factor 1:3 */
#define CLK_RATIO_14		0x00000003    /* division factor 1:4 */

/*  SYNTH. programming Mask  */
/*  Fout = (X/Y) Fin         */
#define SYNTH_XMASK		0XFFFF0000
#define SYNTH_YMASK		0X0000FFFF

/* AMEM_CFG_CTRL &  PORT_CFG_CTRL value  */
#define PORT_CLK_EN		0x00000001
#define PORT_CLK_HCLK		0x00000000
#define PORT_CLK_PLL1		0x00000002
#define PORT_CLK_PLL2		0x00000004
#define PORT_CLK_RAS		0x00000006
#define PORT_CLK_EXT		0x00000008
#define PORT_SYNT_EN		0x00000010
#define PORT_SW_RST		0x00008000

#define PORT_SYNX_MASK		0xFF000000
#define PORT_SYNY_MASK		0x00FF0000

#define PORT_RST_OUT_EN		0x00000200
#define PORT_CLK_OUT_EN		0x00000400

#define CLK_EXP_OUT_SEL		0x00000800

/* PLL_CTR_REG   */
#define MEM_CLK_SEL_MSK		0x70000000
#define MEM_CLK_HCLK		0x00000000
#define MEM_CLK_2HCLK		0x10000000
#define MEM_CLK_PLL2		0x30000000

/* GMAC_CTR_REG   */
#define MII_REVERSE_MODE	0x00000001
#define TXCLK_EXT		0x00000000
#define TXCLK_PLL2		0x00000004
#define TXCLK_SYNTH		0x00000008

/*   PERIPH_CLK_CFG   */
#define  XTALTIMEEN		0x00000001
#define  PLLTIMEEN		0x00000002
#define  CLCDCLK_SYNTH		0x00000000
#define  CLCDCLK_48MHZ		0x00000004
#define  CLCDCLK_EXT		0x00000008
#define  UARTCLK_MASK		(0x1 << 4)
#define  UARTCLK_48MHZ		0x00000000
#define  UARTCLK_SYNTH		0x00000010
#define  IRDACLK_48MHZ		0x00000000
#define  IRDACLK_SYNTH		0x00000020
#define  IRDACLK_EXT		0x00000040
#define  RTC_DISABLE		0x00000080
#define  GPT1CLK_48MHZ		0x00000000
#define  GPT1CLK_SYNTH		0x00000100
#define  GPT2CLK_48MHZ		0x00000000
#define  GPT2CLK_SYNTH		0x00000200
#define  GPT3CLK_48MHZ		0x00000000
#define  GPT3CLK_SYNTH		0x00000400
#define  GPT4CLK_48MHZ		0x00000000
#define  GPT4CLK_SYNTH		0x00000800
#define  GPT5CLK_48MHZ		0x00000000
#define  GPT5CLK_SYNTH		0x00001000
#define  GPT1_FREEZE		0x00002000
#define  GPT2_FREEZE		0x00004000
#define  GPT3_FREEZE		0x00008000
#define  GPT4_FREEZE		0x00010000
#define  GPT5_FREEZE		0x00020000

/*  PERIPH1_CLKEN bits  */
#define PERIPH_ARM1_WE		0x00000001
#define PERIPH_ARM1		0x00000002
#define PERIPH_ARM2		0x00000004
#define PERIPH_UART1		0x00000008
#define PERIPH_UART2		0x00000010
#define PERIPH_SSP1		0x00000020
#define PERIPH_SSP2		0x00000040
#define PERIPH_I2C		0x00000080
#define PERIPH_JPEG		0x00000100
#define PERIPH_FSMC		0x00000200
#define PERIPH_FIRDA		0x00000400
#define PERIPH_GPT4		0x00000800
#define PERIPH_GPT5		0x00001000
#define PERIPH_GPIO4		0x00002000
#define PERIPH_SSP3		0x00004000
#define PERIPH_ADC		0x00008000
#define PERIPH_GPT3		0x00010000
#define PERIPH_RTC		0x00020000
#define PERIPH_GPIO3		0x00040000
#define PERIPH_DMA		0x00080000
#define PERIPH_ROM		0x00100000
#define PERIPH_SMI		0x00200000
#define PERIPH_CLCD		0x00400000
#define PERIPH_GMAC		0x00800000
#define PERIPH_USBD		0x01000000
#define PERIPH_USBH1		0x02000000
#define PERIPH_USBH2		0x04000000
#define PERIPH_MPMC		0x08000000
#define PERIPH_RAMW		0x10000000
#define PERIPH_MPMC_EN		0x20000000
#define PERIPH_MPMC_WE		0x40000000
#define PERIPH_MPMCMSK		0x60000000

#define PERIPH_CLK_ALL		0x0FFFFFF8
#define PERIPH_RST_ALL		0x00000004

/* PORT_BRIDGE_CTRL  value    */
#define MEH2HM_SW_RST		0x00000010
#define MEH2HS_SW_RST		0x00000020
#define MEH2H_CLKEN		0x00000040
#define MEH2H_SYNTH_MASK	0x00000F00

#define SEH2HM_SW_RST		0x00010000
#define SEH2HS_SW_RST		0x00020000
#define SEH2H_CLKEN		0x00040000
#define SEH2H_SYNTH_MASK	0x00F00000

#define SH2HM_SW_RST		0x02000000
#define SH2HS_SW_RST		0x04000000
#define SH2H_CLKEN		0x08000000
#define SH2H_SYNTH_MASK		0xF0000000


/*  SYST_ERROR_REG */
#define ENA_SYSERR_INT		0x00000001
#define RST_SYSERR_INT		0x00000002

#define ENB_PLL_INT		0x00000010
#define ENB_CPU_INT		0x00000020
#define ENB_WDG_INT		0x00000040
#define ENB_EXP_INT		0x00000080
#define ENB_USB_INT		0x00000100
#define ENB_MEM_INT		0x00000200
#define ENB_DMA_INT		0x00000400

/* DDR_PAD values */
#define DDR_PAD_CNF_MSK		0x0000ffff
#define DDR_PAD_SW_CONF		0x00060000
#define DDR_PAD_SSTL_SEL	0x00000001
#define DDR_PAD_DRAM_TYPE	0x00008000

/* DDR_COMP values */
#define DDR_COMP_ACCURATE	0x00000010

#endif

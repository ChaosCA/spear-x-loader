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

#ifndef SPR_SNOR_H
#define SPR_SNOR_H

struct smi_regs {
	u32 smi_cr1;
	u32 smi_cr2;
	u32 smi_sr;
	u32 smi_tr;
	u32 smi_rr;
};

/* CONTROL REG 1 */
#define BANK_EN		0x0000000F /* Enables all banks */
#define DSEL_TIME	0x00000050 /* Deselect time 6+1 SMI_CK periods */
#define PRESCAL5	0x00000500 /* AHB_CK prescaling value */
#define PRESCALA	0x00000A00 /* AHB_CK prescaling value */
#define PRESCAL1	0x00000100 /* AHB_CK prescaling value */
#define PRESCAL2	0x00000200 /* AHB_CK prescaling value */
#define PRESCAL3	0x00000300 /* AHB_CK prescaling value */
#define PRESCAL4	0x00000400 /* AHB_CK prescaling value */
#define SW_MODE		0x10000000 /* Enables SW Mode */
#define WB_MODE		0x20000000 /* Write Burst Mode */
#define FAST_MODE	0x00008000 /* Fast Mode */
#define HOLD1		0x00010000 /* Hold */

#endif

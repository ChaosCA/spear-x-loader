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

#ifndef _SPR_SOCREV_H
#define _SPR_SOCREV_H

#define SOC_PRI_SHFT		16
#define SOC_SEC_SHFT		8

/* Revision definitions */
#define SOC_SPEAR_NA		0

/*
 * The definitons have started from
 * 101 for SPEAr6xx
 * 201 for SPEAr3xx
 * 301 for SPEAr13xx
 */
#define SOC_SPEAR600_AA		101
#define SOC_SPEAR600_AB		102
#define SOC_SPEAR600_BA		103
#define SOC_SPEAR600_BB		104
#define SOC_SPEAR600_BC		105
#define SOC_SPEAR600_BD		106

#define SOC_SPEAR300		201
#define SOC_SPEAR310		202
#define SOC_SPEAR320		203

extern int get_socrev(void);

#endif

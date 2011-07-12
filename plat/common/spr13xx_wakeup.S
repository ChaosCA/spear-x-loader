/*
 * (C) Copyright 2010
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

#include <config.h>

.global cleanup_caches

.global cpu2_wake
cpu2_wake:
	stmfd	r13!, {r0 - r1}
	ldr	r0,=CONFIG_SPEAR_ARM2PHOLD
	adr	r1,cpu2_start
	str	r1,[r0]
	dmb
	dsb
	ldmfd	r13!, {r0 - r1}
	bx	r14

/* CPU2 starts running from here */
cpu2_start:
	/* stack init, needed for calling C routine */
	ldr	r13,=CONFIG_SPEAR_ARM2STACK
	bl	cleanup_caches

	ldr	r0,=CONFIG_SPEAR_ARM2PHOLD
	ldr	r1,=0xdeadbeef
	str	r1,[r0]
	dmb
	dsb
1:
	wfe
	ldr	r1,[r0]
	tst	r1,#0x3
	bne	1b
	bx	r1

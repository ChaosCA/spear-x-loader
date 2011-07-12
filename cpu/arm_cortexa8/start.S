/*
 * armboot - Startup Code for OMAP3530/ARM Cortex CPU-core
 *
 * Copyright (c) 2004	Texas Instruments <r-woodruff2@ti.com>
 *
 * Copyright (c) 2001	Marius Gröger <mag@sysgo.de>
 * Copyright (c) 2002	Alex Züpke <azu@sysgo.de>
 * Copyright (c) 2002	Gary Jennejohn <garyj@denx.de>
 * Copyright (c) 2003	Richard Woodruff <r-woodruff2@ti.com>
 * Copyright (c) 2003	Kshitij <kshitij@ti.com>
 * Copyright (c) 2006-2008 Syed Mohammed Khasim <x0khasim@ti.com>
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

#include <config.h>

.globl _start
_start: b	reset
	ldr	pc, _undefined_instruction
	ldr	pc, _software_interrupt
	ldr	pc, _prefetch_abort
	ldr	pc, _data_abort
	ldr	pc, _not_used
	ldr	pc, _irq
	ldr	pc, _fiq

_undefined_instruction:
_software_interrupt:
_prefetch_abort:
_data_abort:
_not_used:
_irq:
_fiq:		.word infinite_loop
_pad:		.word 0x12345678 /* now 16*4=64 */

infinite_loop:
	b	infinite_loop

/*************************************************************************
 *
 * Startup Code (reset vector)
 *
 * Below are the critical initializations already taken place in BootROM.
 * So, these are not taken care in Xloader
 * 1. Relocation to RAM
 * 2. Initializing stacks
 *
 *************************************************************************/

/*
 * the actual reset code
 */

reset:
/*
 * Xloader has to return back to BootROM in a few cases.
 * eg. Ethernet boot, UART boot, USB boot
 * Saving registers for returning back
 */
	stmdb	sp!, {r1-r12,r14}
	bl	cpu_init_crit
/*
 * Clearing bss area is not done in Xloader. 
 * BSS area lies in the DDR location which is not yet initialized
 * bss is assumed to be uninitialized.
 */
	bl	start_armboot
	ldmia	sp!, {r1-r12,pc}

/*************************************************************************
 *
 * CPU_init_critical registers
 *
 * setup important registers
 * setup memory timing
 *
 *************************************************************************/
cpu_init_crit:
	/*
	 * Invalidate L1 I/D
	 */
	mov	r0, #0			@ set up for MCR
	mcr	p15, 0, r0, c8, c7, 0	@ invalidate TLBs
	mcr	p15, 0, r0, c7, c5, 0	@ invalidate icache

	/*
	 * disable MMU stuff and caches
	 */
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00000007	@ clear bits 2:0 (-CAM)
	orr	r0, r0, #0x00000002	@ set bit 1 (--A-) Align
	orr	r0, r0, #0x00000800	@ set bit 12 (Z---) BTB
	mcr	p15, 0, r0, c1, c0, 0

	/*
	 * Go setup Memory and board specific bits prior to relocation.
	 */
	stmdb	sp!, {lr}
	bl	lowlevel_init	/* go setup pll,mux,memory */
	ldmia	sp!, {pc}

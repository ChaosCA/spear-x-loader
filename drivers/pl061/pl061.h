/*
 * (C) Copyright 2012
 * ST Microelectronics
 * Armando Visconti <armando.visconti@st.com>
 * Vincenzo Frascino <vincenzo.frascino@st.com>
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
#ifndef SPR_PL061_H
#define SPR_PL061_H

#define GPIODIR 0x400

#define GPIO0_0		0
#define GPIO0_1		1
#define GPIO0_2		2
#define GPIO0_3		3
#define GPIO0_4		4
#define GPIO0_5		5
#define GPIO0_6		6
#define GPIO0_7		7

#define GPIO1_0		0
#define GPIO1_1		1
#define GPIO1_2		2
#define GPIO1_3		3
#define GPIO1_4		4
#define GPIO1_5		5
#define GPIO1_6		6
#define GPIO1_7		7

void pl061_set_value(unsigned int gpio_base, unsigned offset, int value);
int pl061_set_out(unsigned int gpio_base, unsigned offset);

#endif

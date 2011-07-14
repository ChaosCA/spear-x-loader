/*
 * (C) Copyright 2011
 * ST Microelectronics
 * Armando Visconti <armando.visconti@st.com>
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
#include <pl061.h>

int pl061_set_out(unsigned int gpio_base, unsigned offset)
{
	unsigned char gpiodir;

	gpiodir = readb(gpio_base + GPIODIR);
	gpiodir |= 1 << offset;
	writeb(gpiodir, gpio_base + GPIODIR);

	return 0;
}

void pl061_set_value(unsigned int gpio_base, unsigned offset, int value)
{
	writeb(value << offset, gpio_base + (1 << (offset + 2)));
}
/*
 * (C) Copyright 2009
 * Mirko Gardi, ST Micoelectronics, mirko.gardi@st.com.
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
#include <ddrtest.h>

#ifdef CONFIG_RAM_TEST
int probememory(unsigned long start_ddr, unsigned long end_ddr)
{
	u32	*addr, *start, *end;
	u32	readback;
	int	errs = 0;
	int	testpatt = 0;
	int	testpos = 0;

	u32 data_pattern[DATA_PATT_NUM][DATA_PAT_POS] = {
		{ 0xffffffff, 0x00000000, 0xffffffff, 0x00000000 },
		{ 0x00000000, 0xffffffff, 0x00000000, 0xffffffff },
		{ 0xffffffff, 0xffffffff, 0x00000000, 0x00000000 },
		{ 0x00000000, 0x00000000, 0xffffffff, 0xffffffff },
		{ 0x5a5a5a5a, 0xa5a5a5a5, 0xa5a5a5a5, 0x5a5a5a5a },
		{ 0xa5a5a5a5, 0x5a5a5a5a, 0x5a5a5a5a, 0xa5a5a5a5 },
		{ 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000 },
		{ 0x00000000, 0x00000000, 0x00000000, 0xffffffff },
		{ 0x55555555, 0x55555555, 0xaaaaaaaa, 0xaaaaaaaa },
		{ 0xaaaaaaaa, 0xaaaaaaaa, 0x55555555, 0x55555555 },
	};

	start = (u32 *)start_ddr;
	end = (u32 *)end_ddr;

	errs = 0;

	for (testpatt = 0; testpatt < DATA_PATT_NUM; testpatt++) {
		/* WRITE */
		for (addr = start; addr < end;) {
			for (testpos = 0; testpos < DATA_PAT_POS; testpos++) {
				*addr = data_pattern[testpatt][testpos];
				addr++;
			}
		}

		/* READ/CHECK */
		for (addr = start; addr < end;) {
			for (testpos = 0; testpos < DATA_PAT_POS; testpos++) {
				readback = *addr;
				addr++;
				if (readback != data_pattern[testpatt][testpos])
					errs++;
			}
		}
	}

	return errs;
}
#endif

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

#include <asm/arch/lvl_code.h>

#define MAX_DELAY_READ 11

int resp_array [16][16][2];
int slice;
int x,y;

void  mode_specific_setup_read(void)
{
	write_reg_field( 0xffff , 0xffff ,DENALI_CTL_182);
	write_reg_field(0x10<<16,0xff<<16,DENALI_CTL_64);
	write_reg_field(0x12<<8,0xff<<8,DENALI_CTL_59);
}

void mode_specific_delay_loop_read(int slice, int delayval)
{
	switch(slice)
	{
	case 0:
		write_reg_field( delayval<<16,RDLVL_DELAY_0_MASK ,DENALI_CTL_81);
		break;
	case 1:
		write_reg_field( delayval,RDLVL_DELAY_1_MASK ,DENALI_CTL_82);
		break;
	case 2:
		write_reg_field( delayval<<16,RDLVL_DELAY_2_MASK ,DENALI_CTL_82);
		break;
	case 3:
		write_reg_field( delayval,RDLVL_DELAY_3_MASK ,DENALI_CTL_83);
		break;
	case 4:
		write_reg_field( delayval<<16,RDLVL_DELAY_4_MASK ,DENALI_CTL_83);
		break;
	}
}

int sub_abs(int a, int b)
{
	if (a > b)
		return (a-b);
	else
		return (b-a);
}

void doReadLeveling8Phase(void)
{
	int resp = 0;
	int start_point_00[2];
	int end_point_00[2];
	int lvl_value;
	int seen_00;
	int seen_00_done;
	int pass;
	int i;

	write_reg_field (READ_LVL,SW_LEVELING_MODE_MASK,DENALI_CTL_20);
	mode_specific_setup_read();
	setbit(8,DENALI_CTL_13);
	waitForOpDone();
	for(slice=0; slice < NO_OF_DATA_SLICES ; slice++)
	{
		int delayval = 0;
#if 0
		setbit(24,DENALI_CTL_12);
		waitForOpDone();
#endif
		lvl_value = -1;
		for(pass=0; pass < 2; pass++)
		{
			write_reg_field (pass,1,DENALI_CTL_07);
			delayval = 4;
			while (delayval <= MAX_DELAY_READ)
			{
				mode_specific_delay_loop_read(slice, delayval);
				setbit(24,DENALI_CTL_12);
				waitForOpDone();
				switch(slice)
				{
				case 0:
					resp = read_reg_field( 16 , SWLVL_RESP_0_MASK  ,DENALI_CTL_57);
					break;
				case 1:
					resp = read_reg_field( 24,SWLVL_RESP_1_MASK ,DENALI_CTL_57);
					break;
				case 2:
					resp = read_reg_field( 0,SWLVL_RESP_2_MASK ,DENALI_CTL_58);
					break;
				case 3:
					resp = read_reg_field( 8,SWLVL_RESP_3_MASK ,DENALI_CTL_58);
					break;
				case 4:
					resp = read_reg_field( 16,SWLVL_RESP_4_MASK ,DENALI_CTL_58);
					break;
				}
				resp_array[slice][delayval][pass] = resp & 1;
				delayval = delayval + 1;
			}
			start_point_00[pass] = -1;
			end_point_00[pass] = -1;
			seen_00 = 0;
			seen_00_done = 0;
			for(i=4; i<=MAX_DELAY_READ; i++)
			{
				if (start_point_00[pass] < 0)
				{
					if (resp_array[slice][i][pass] == 0)
					{
						seen_00 = 1;
						start_point_00[pass] = i;
					}
				}
				else if ((resp_array[slice][i][pass] == 0) && (seen_00_done == 0))
				{
					end_point_00[pass] = i;
				}
				else if (resp_array[slice][i][pass] != 0)
				{
					seen_00_done = 1;
				}
			}
		}
		x = end_point_00[0] - start_point_00[0];
		y = end_point_00[1] - start_point_00[1];
		if ((x == 0) && (y == 0))
			if (start_point_00[0] != start_point_00[1])
				while(1);
			else
				lvl_value = start_point_00[0];
		else if ((x == 0) && (y != 0))
			if ((start_point_00[0] == start_point_00[1]) || (start_point_00[0] == (start_point_00[1]+1)))
				lvl_value = start_point_00[0];
			else
				while(1);
		else if ((x != 0) && (y == 0))
			if ((start_point_00[1] == start_point_00[0]) || (start_point_00[1] == (start_point_00[0]+1)))
				lvl_value = start_point_00[1];
			else
				while(1);
		else
			if (start_point_00[1] == start_point_00[0])
				lvl_value = start_point_00[1]+1;
			else if (sub_abs(start_point_00[0], start_point_00[1]) < 2)
			{
				if ((x == 1) && (y == 1))
				{
					if (start_point_00[1] == (start_point_00[0]+1))
						lvl_value = start_point_00[1];
					else if (start_point_00[0] == (start_point_00[1]+1))
						lvl_value = start_point_00[0];
					else
						while(1);
				}
				else if (x==y)
				{
					if (start_point_00[1] == (start_point_00[0]+1))
						lvl_value = start_point_00[1] + 1;
					else if (start_point_00[0] == (start_point_00[1] + 1))
						lvl_value = start_point_00[0] + 1;
					else
						while(1);
				}
				else if (x > y)
				{
					if (start_point_00[1] == (start_point_00[0]+1))
						lvl_value = start_point_00[0] + 2;
					else if (start_point_00[0] == (start_point_00[1] + 1))
					{
						if (y == 1)
							lvl_value = start_point_00[1] + 1;
						else
							lvl_value = start_point_00[1] + 2;
					}
					else
						while(1);
				}
				else
				{
					if (start_point_00[0] == (start_point_00[1]+1))
						lvl_value = start_point_00[1] + 2;
					else if (start_point_00[1] == (start_point_00[0] + 1))
					{
						if (x == 1)
							lvl_value = start_point_00[0] + 1;
						else
							lvl_value = start_point_00[0] + 2;
					}
					else
						while(1);
				}
			}
			else
				while(1);
		if (lvl_value != -1)
		{
			mode_specific_delay_loop_read(slice, lvl_value);
		}
		else
			while(1);
#if 0
		setbit(16,DENALI_CTL_12);
		waitForOpDone();
#endif
	}
#if 0
	setbit(8,DENALI_CTL_13);
	waitForOpDone();
#endif
	setbit(24,DENALI_CTL_12);
	waitForOpDone();
	setbit(16,DENALI_CTL_12);
	waitForOpDone();
}


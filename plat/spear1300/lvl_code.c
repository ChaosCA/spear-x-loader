/********************************************************************************/
/*										*/
/*       	        	 Author : RC & GS			    	*/
/*										*/
/********************************************************************************/

#include <asm/arch/lvl_code.h>

int m_max_delay;
int resp;

void waitForOpDone()
{
	volatile int pr_state;
	do{
		readl(pr_state, DENALI_CTL_13);
	}while( !(pr_state & SWLVL_OP_DONE) );
}

void write_reg_field(int value,int mask, volatile unsigned int *address)
{
	int temp_reg;
	temp_reg= *address & ~mask;
	*address= temp_reg | value;
}

int read_reg_field(int shift,int mask, volatile unsigned int *address)
{
	int temp_reg;
	temp_reg= *address & mask;
	return temp_reg >> shift;
}

void toggleSliceReset(int slice)
{
	int i;
	switch(slice)
	{
	case 0:
		setbit(0,DENALI_CTL_124);
		for(i=0; i< 100; i++);
		clearbit(0,DENALI_CTL_124);
		break;
	case 1:
		setbit(0,DENALI_CTL_125);
		for(i=0; i< 100; i++);
		clearbit(0,DENALI_CTL_125);
		break;
	case 2:
		setbit(0,DENALI_CTL_126);
		for(i=0; i< 100; i++);
		clearbit(0,DENALI_CTL_126);
		break;
	case 3:
		setbit(0,DENALI_CTL_127);
		for(i=0; i< 100; i++);
		clearbit(0,DENALI_CTL_127);
		break;
	case 4:
		setbit(0,DENALI_CTL_128);
		for(i=0; i< 100; i++);
		clearbit(0,DENALI_CTL_128);
		break;
	}
}

char add_clk_and_half(char coarse, char fine,char sel)
{
	char result;
	switch(coarse)
	{
	case	0: 
		switch (fine)
		{
		case 0:
			result = 5;
			break;
		case 1:
			result = 6;
			break;
		case 2:
			result = 9;
			break;
		}
		break;

	case	1:
		switch (fine)
		{
		case 1:
			result = 10;
			break;
		case 2:
			result = 13;
			break;
		}
		break;

	case	2: switch (fine)
		   {
		   case 1: result = 14;
			   break;
		   case 2: result = 17;
			   break;
		   }
		   break;
	case	3: switch (fine)
		   {
		   case 1: result = 18;
			   break;
		   case 2: result = 21;
			   break;
		   }
		   break;
	case	4: switch (fine)
		   {
		   case 1: result = 22;
			   break;
		   case 2: result = 25;
			   break;
		   }
		   break;
	case	5: switch (fine)
		   {
		   case 1: result = 26;
			   break;
		   case 2: result = 29;
			   break;
		   }
		   break;
	case	6: switch (fine)
		   {
		   case 1: result = 30;
			   break;
		   case 2: result = 1;
			   break;
		   }
		   break;
	case	7: switch (fine)
		   {
		   case 1: result = 2;
			   break;
		   case 2: result = 5;
			   break;
		   }
		   break;
	}
	if (sel)
		return (result & 3);
	else
		return (result >> 2);
}

void doLeveling()
{
	int resp_array [16][8];
	int slice;
	signed char wrlvl_start;
	signed char wrlvl_base;
	char wrlvl_base_offset_reg;
	char cal_clk_byte_patt;
	char cal_ref_byte_patt;
	char clk_source_sel_cal_pre;
	char ref_source_sel_cal_pre;
	char start_wrlvl_delay_mod;
	char start_wrlvl_delay_mod_array[16];
	char wr_dqs_gate_coarse_assert[1][15]   = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1};
	char wr_dqs_gate_fine_assert[1][15]     = {1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2};
	char wr_dqs_gate_coarse_deassert[1][15] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1};
	char wr_dqs_gate_fine_deassert[1][15]   = {1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2};
	signed char dq_a_timing_reg;
	char dq_a_timing_reg_offset;
	char dqs_byte_patt_mux_dqs;
	char dqs_byte_patt_mux_data;
	char dqs_byte_patt_mux_data1;
	char phy_clk_phase_match_pre;
	char dq_clk_phase_match_pre;
	char dq_clk_phase_match0_pre;
	char dq_clk_phase_match1_pre;
	signed char dqs_dq_clk_phase_match_delta;
	signed char wr_dqX_a_timing_adj;
	char wr_dqX_a_timing_adj_array[16];
	int found;
	int found_00s;
	int     table_index;
	int j;
	int i;
	int temp_reg;
	int temp_reg0;
	int temp_reg1;
	int temp_reg2;
	int temp_reg3;
	int temp_reg4;
	int temp_reg5;
	char selected_wr_dq_dqs_gate_coarse_assert;
	char selected_wr_dq_dqs_gate_fine_assert;
	char selected_wr_dq_dqs_gate_coarse_deassert;
	char selected_wr_dq_dqs_gate_fine_deassert;
	char deassert_fine;
	char wr_dq_dqs_gate_coarse_assert[3][15]  = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1}};
	char  wr_dq_dqs_gate_fine_assert[3][15]= {{0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2},
		{0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1},
		{0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1}};
	char  wr_dq_dqs_gate_coarse_deassert[3][15]  = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1}};
	char wr_dq_dqs_gate_fine_deassert[3][15]  = {{0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2},
		{0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1},
		{0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1}};
	write_reg_field (WRITE_LVL,SW_LEVELING_MODE_MASK,DENALI_CTL_20);
	dq_a_timing_reg_offset = 2;
	m_max_delay = 8;
	for(slice=0; slice < NO_OF_DATA_SLICES ; slice++) {
		int delayval = 0;
		setbit(SWLVL_START,DENALI_CTL_13);
		waitForOpDone();
		wrlvl_base_offset_reg = 2;
		write_reg_field (wrlvl_base_offset_reg<<20,WRLVL_BASE_OFFSET_REG_MASK,DENALI_CTL_129);
		cal_clk_byte_patt= read_reg_field(0,cal_clk_byte_patt_mask, DENALI_CTL_181);
		cal_ref_byte_patt= read_reg_field(8,cal_ref_byte_patt_mask, DENALI_CTL_181);
		if        ((cal_clk_byte_patt & 0xE1) == 0x60)  clk_source_sel_cal_pre = 1;
		else if ((cal_clk_byte_patt & 0xF0) == 0x30)  clk_source_sel_cal_pre = 2;
		else if ((cal_clk_byte_patt & 0x78) == 0x18)  clk_source_sel_cal_pre = 3;
		else if ((cal_clk_byte_patt & 0x3C) == 0x0C)  clk_source_sel_cal_pre = 4;
		else if ((cal_clk_byte_patt & 0x1E) == 0x06)  clk_source_sel_cal_pre = 5;
		else if ((cal_clk_byte_patt & 0x0F) == 0x03)  clk_source_sel_cal_pre = 6;
		else if ((cal_clk_byte_patt & 0x87) == 0x81)  clk_source_sel_cal_pre = 7;
		else  clk_source_sel_cal_pre = 0;
		if        ((cal_ref_byte_patt & 0xE1) == 0x60)  ref_source_sel_cal_pre = 1;
		else if ((cal_ref_byte_patt & 0xF0) == 0x30)  ref_source_sel_cal_pre = 2;
		else if ((cal_ref_byte_patt & 0x78) == 0x18)  ref_source_sel_cal_pre = 3;
		else if ((cal_ref_byte_patt & 0x3C) == 0x0C)  ref_source_sel_cal_pre = 4;
		else if ((cal_ref_byte_patt & 0x1E) == 0x06)  ref_source_sel_cal_pre = 5;
		else if ((cal_ref_byte_patt & 0x0F) == 0x03)  ref_source_sel_cal_pre = 6;
		else if ((cal_ref_byte_patt & 0x87) == 0x81)  ref_source_sel_cal_pre = 7;
		else  ref_source_sel_cal_pre = 0;
		wrlvl_base = ref_source_sel_cal_pre - clk_source_sel_cal_pre;
		if (wrlvl_base< 0)
			wrlvl_base = 8 + wrlvl_base;
		wrlvl_start = wrlvl_base - wrlvl_base_offset_reg;
		if (wrlvl_start<0)
			wrlvl_start = 8 + wrlvl_start;
		toggleSliceReset(slice);
		for(delayval = 0; delayval < m_max_delay; delayval++) {
			int  dfi_wrlvl_delay;
			dfi_wrlvl_delay = delayval;
			switch(slice)
			{
			case 0:
				write_reg_field( dfi_wrlvl_delay,WRLVL_DELAY_0_MASK , DENALI_CTL_100);
				break;
			case 1:
				write_reg_field( dfi_wrlvl_delay << 16,WRLVL_DELAY_1_MASK , DENALI_CTL_100);
				break;
			case 2:
				write_reg_field( dfi_wrlvl_delay,WRLVL_DELAY_2_MASK , DENALI_CTL_101);
				break;
			case 3:
				write_reg_field( dfi_wrlvl_delay << 16,WRLVL_DELAY_3_MASK , DENALI_CTL_101);
				break;
			case 4:
				write_reg_field( dfi_wrlvl_delay,WRLVL_DELAY_4_MASK , DENALI_CTL_102);
				break;
			default:
				break;
			}
			start_wrlvl_delay_mod = wrlvl_start + dfi_wrlvl_delay;

			if  (wr_dqs_gate_fine_assert[0][start_wrlvl_delay_mod] == 0)
				deassert_fine = 1;
			else if (wr_dqs_gate_fine_assert[0][start_wrlvl_delay_mod] == 1)
				deassert_fine = 2;
			else
				deassert_fine = 1;
			switch(slice)
			{
			case 0:
				write_reg_field(wr_dqs_gate_fine_assert[0][start_wrlvl_delay_mod]<<3,WR_DQS_GATE_FINE_ASSERT_MASK ,DENALI_CTL_130);
				write_reg_field( deassert_fine<<8,DEASSERT_FINE_MASK ,DENALI_CTL_130);
				break;
			case 1:
				write_reg_field(wr_dqs_gate_fine_assert[0][start_wrlvl_delay_mod]<<3,WR_DQS_GATE_FINE_ASSERT_MASK ,DENALI_CTL_131);
				write_reg_field( deassert_fine<<8,DEASSERT_FINE_MASK ,DENALI_CTL_131);
				break;
			case 2:
				write_reg_field(wr_dqs_gate_fine_assert[0][start_wrlvl_delay_mod]<<3,WR_DQS_GATE_FINE_ASSERT_MASK ,DENALI_CTL_132);
				write_reg_field( deassert_fine<<8,DEASSERT_FINE_MASK ,DENALI_CTL_132);
				break;
			case 3:
				write_reg_field(wr_dqs_gate_fine_assert[0][start_wrlvl_delay_mod]<<3,WR_DQS_GATE_FINE_ASSERT_MASK ,DENALI_CTL_133);
				write_reg_field( deassert_fine<<8,DEASSERT_FINE_MASK ,DENALI_CTL_133);
				break;
			case 4:
				write_reg_field(wr_dqs_gate_fine_assert[0][start_wrlvl_delay_mod]<<3,WR_DQS_GATE_FINE_ASSERT_MASK ,DENALI_CTL_134);
				write_reg_field( deassert_fine<<8,DEASSERT_FINE_MASK ,DENALI_CTL_134);
				break;
			}
			resp_array[slice][delayval] = 0;
			for(j=0; j<5; j++) {
				setbit(24,DENALI_CTL_12);
				waitForOpDone();
				if (j==0){}
				else{
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
					resp_array[slice][delayval]= resp_array[slice][delayval]+ resp;
				}
			}
		}
		found = 0;
		found_00s = 0;
		for(i=0; i<8; i++)
			if ((found_00s == 0) && (resp_array[slice][i] < 4)) {
				found_00s = 1;
			}
			else if ((found_00s == 1) && (found == 0) && (resp_array[slice][i] == 4)) {
				int best_wrlvl_delay;
				found = 1;
				best_wrlvl_delay = i -1;
				start_wrlvl_delay_mod = wrlvl_start + best_wrlvl_delay;
				switch(slice)
				{
				case 0:
					write_reg_field( best_wrlvl_delay,WRLVL_DELAY_0_MASK ,DENALI_CTL_100);
					break;
				case 1:
					write_reg_field( best_wrlvl_delay<<16,WRLVL_DELAY_1_MASK ,DENALI_CTL_100);
					break;
				case 2:
					write_reg_field( best_wrlvl_delay,WRLVL_DELAY_2_MASK ,DENALI_CTL_101);
					break;
				case 3:
					write_reg_field( best_wrlvl_delay<<16,WRLVL_DELAY_3_MASK ,DENALI_CTL_101);
					break;
				case 4:
					write_reg_field( best_wrlvl_delay,WRLVL_DELAY_4_MASK ,DENALI_CTL_102);
					break;
				}
				start_wrlvl_delay_mod = wrlvl_start + best_wrlvl_delay;
				if   (wr_dqs_gate_fine_assert[0][start_wrlvl_delay_mod] == 0)
					deassert_fine = 1;
				else if  (wr_dqs_gate_fine_assert[0][start_wrlvl_delay_mod] == 1)
					deassert_fine = 2;
				else
					deassert_fine = 1;
				switch(slice)
				{
				case 0:
					write_reg_field(wr_dqs_gate_fine_assert[0][start_wrlvl_delay_mod]<<3,WR_DQS_GATE_FINE_ASSERT_MASK ,DENALI_CTL_130);
					write_reg_field( deassert_fine<<8,DEASSERT_FINE_MASK ,DENALI_CTL_130);
					break;
				case 1:
					write_reg_field(wr_dqs_gate_fine_assert[0][start_wrlvl_delay_mod]<<3,WR_DQS_GATE_FINE_ASSERT_MASK ,DENALI_CTL_131);
					write_reg_field( deassert_fine<<8,DEASSERT_FINE_MASK ,DENALI_CTL_131);
					break;
				case 2:
					write_reg_field(wr_dqs_gate_fine_assert[0][start_wrlvl_delay_mod]<<3,WR_DQS_GATE_FINE_ASSERT_MASK ,DENALI_CTL_132);
					write_reg_field( deassert_fine<<8,DEASSERT_FINE_MASK ,DENALI_CTL_132);
					break;
				case 3:
					write_reg_field(wr_dqs_gate_fine_assert[0][start_wrlvl_delay_mod]<<3,WR_DQS_GATE_FINE_ASSERT_MASK ,DENALI_CTL_133);
					write_reg_field( deassert_fine<<8,DEASSERT_FINE_MASK ,DENALI_CTL_133);
					break;
				case 4:
					write_reg_field(wr_dqs_gate_fine_assert[0][start_wrlvl_delay_mod]<<3,WR_DQS_GATE_FINE_ASSERT_MASK ,DENALI_CTL_134);
					write_reg_field( deassert_fine<<8,DEASSERT_FINE_MASK ,DENALI_CTL_134);
					break;
				}
				setbit(24,DENALI_CTL_12);
				waitForOpDone();
			}
		setbit(16,DENALI_CTL_12);
		waitForOpDone();
		dq_a_timing_reg = start_wrlvl_delay_mod - dq_a_timing_reg_offset;
		if (dq_a_timing_reg < 0)
			dq_a_timing_reg= 8+dq_a_timing_reg;
		else if (dq_a_timing_reg > 7)
			dq_a_timing_reg=dq_a_timing_reg - 8;
		temp_reg = dq_a_timing_reg+(dq_a_timing_reg<<3)+(dq_a_timing_reg<<6)+(dq_a_timing_reg<<9)+(dq_a_timing_reg<<12)+(dq_a_timing_reg<<15)+(dq_a_timing_reg<<18)+(dq_a_timing_reg<<21)+(dq_a_timing_reg<<24);
		switch(slice)
		{
		case 0:
			write_reg_field(temp_reg ,0x07FFFFFF ,DENALI_CTL_145);
			break;
		case 1:
			write_reg_field(temp_reg, 0x07FFFFFF ,DENALI_CTL_146);
			break;
		case 2:
			write_reg_field(temp_reg, 0x07FFFFFF ,DENALI_CTL_147);
			break;
		case 3:
			write_reg_field(temp_reg, 0x07FFFFFF ,DENALI_CTL_148);
			break;
		case 4:
			write_reg_field(temp_reg, 0x07FFFFFF ,DENALI_CTL_149);
			break;
		}
		switch(slice)
		{
		case 0:
			dqs_byte_patt_mux_dqs = read_reg_field( 8 , PATT_MUX_DQS_MASK  ,DENALI_CTL_175);
			dqs_byte_patt_mux_data = read_reg_field( 16 , PATT_MUX_DATA_MASK  ,DENALI_CTL_175);
			dqs_byte_patt_mux_data1 = read_reg_field( 23 , PATT_MUX_DATA1_MASK  ,DENALI_CTL_170);
			break;
		case 1:
			dqs_byte_patt_mux_dqs = read_reg_field( 8 , PATT_MUX_DQS_MASK  ,DENALI_CTL_176);
			dqs_byte_patt_mux_data = read_reg_field( 16 , PATT_MUX_DATA_MASK  ,DENALI_CTL_176);
			dqs_byte_patt_mux_data1 = read_reg_field( 23 , PATT_MUX_DATA1_MASK  ,DENALI_CTL_171);
			break;
		case 2:
			dqs_byte_patt_mux_dqs = read_reg_field( 8 , PATT_MUX_DQS_MASK  ,DENALI_CTL_177);
			dqs_byte_patt_mux_data = read_reg_field( 16 , PATT_MUX_DATA_MASK  ,DENALI_CTL_177);
			dqs_byte_patt_mux_data1 = read_reg_field( 23 , PATT_MUX_DATA1_MASK  ,DENALI_CTL_172);
			break;
		case 3:
			dqs_byte_patt_mux_dqs = read_reg_field( 8 , PATT_MUX_DQS_MASK  ,DENALI_CTL_178);
			dqs_byte_patt_mux_data = read_reg_field( 16 , PATT_MUX_DATA_MASK  ,DENALI_CTL_178);
			dqs_byte_patt_mux_data1 = read_reg_field( 23 , PATT_MUX_DATA1_MASK  ,DENALI_CTL_173);
			break;
		case 4:
			dqs_byte_patt_mux_dqs = read_reg_field( 8 , PATT_MUX_DQS_MASK  ,DENALI_CTL_179);
			dqs_byte_patt_mux_data = read_reg_field( 16 , PATT_MUX_DATA_MASK  ,DENALI_CTL_179);
			dqs_byte_patt_mux_data1 = read_reg_field( 23 , PATT_MUX_DATA1_MASK  ,DENALI_CTL_174);
			break;
		};
		/*           `info($psprintf("dqs_byte_patt_dqs_%0d: %0x", slice, dqs_byte_patt_dqs)); */
		/*           `info($psprintf("dqs_byte_patt_mux_data_%0d: %0x", slice, dqs_byte_patt_mux_data)); */
		/*           `info($psprintf("dqs_byte_patt_mux_data1_%0d: %0x", slice, dqs_byte_patt_mux_data1)); */
		if        ((dqs_byte_patt_mux_dqs & 0xE1) == 0x60)  phy_clk_phase_match_pre = 1;
		else if ((dqs_byte_patt_mux_dqs & 0xF0) == 0x30)  phy_clk_phase_match_pre = 2;
		else if ((dqs_byte_patt_mux_dqs & 0x78) == 0x18)  phy_clk_phase_match_pre = 3;
		else if ((dqs_byte_patt_mux_dqs & 0x3C) == 0x0C)  phy_clk_phase_match_pre = 4;
		else if ((dqs_byte_patt_mux_dqs & 0x1E) == 0x06)  phy_clk_phase_match_pre = 5;
		else if ((dqs_byte_patt_mux_dqs & 0x0F) == 0x03)  phy_clk_phase_match_pre = 6;
		else if ((dqs_byte_patt_mux_dqs & 0x87) == 0x81)  phy_clk_phase_match_pre = 7;
		else  phy_clk_phase_match_pre = 0;
		if      ((dqs_byte_patt_mux_data & 0x1F) == 0x00)  dq_clk_phase_match0_pre = 8;
		else if ((dqs_byte_patt_mux_data & 0xF8) == 0xF8)  dq_clk_phase_match0_pre = 9;
		else if ((dqs_byte_patt_mux_data & 0xF8) == 0x78)  dq_clk_phase_match0_pre = 1;
		else if ((dqs_byte_patt_mux_data & 0xFC) == 0x3C)  dq_clk_phase_match0_pre = 2;
		else if ((dqs_byte_patt_mux_data & 0xFE) == 0x1E)  dq_clk_phase_match0_pre = 3;
		else if ((dqs_byte_patt_mux_data & 0xFF) == 0x0F)  dq_clk_phase_match0_pre = 4;
		else if ((dqs_byte_patt_mux_data & 0x7F) == 0x07)  dq_clk_phase_match0_pre = 5;
		else if ((dqs_byte_patt_mux_data & 0x3F) == 0x03)  dq_clk_phase_match0_pre = 6;
		else if ((dqs_byte_patt_mux_data & 0x1F) == 0x01)  dq_clk_phase_match0_pre = 7;
		else  dq_clk_phase_match0_pre = 0xF;
		if      ((dqs_byte_patt_mux_data1 & 0x1F) == 0x00)  dq_clk_phase_match1_pre = 8;
		else if ((dqs_byte_patt_mux_data1 & 0xF8) == 0xF8)  dq_clk_phase_match1_pre = 9;
		else if ((dqs_byte_patt_mux_data1 & 0xF8) == 0x78)  dq_clk_phase_match1_pre = 1;
		else if ((dqs_byte_patt_mux_data1 & 0xFC) == 0x3C)  dq_clk_phase_match1_pre = 2;
		else if ((dqs_byte_patt_mux_data1 & 0xFE) == 0x1E)  dq_clk_phase_match1_pre = 3;
		else if ((dqs_byte_patt_mux_data1 & 0xFF) == 0x0F)  dq_clk_phase_match1_pre = 4;
		else if ((dqs_byte_patt_mux_data1 & 0x7F) == 0x07)  dq_clk_phase_match1_pre = 5;
		else if ((dqs_byte_patt_mux_data1 & 0x3F) == 0x03)  dq_clk_phase_match1_pre = 6;
		else if ((dqs_byte_patt_mux_data1 & 0x1F) == 0x01)  dq_clk_phase_match1_pre = 7;
		else  dq_clk_phase_match1_pre = 0xF;
		if (((dq_clk_phase_match0_pre == 8) && (dq_clk_phase_match1_pre == 9)) ||
				((dq_clk_phase_match1_pre == 8) && (dq_clk_phase_match0_pre == 9)))
			dq_clk_phase_match_pre = 0;
		else if ((dq_clk_phase_match0_pre & 8) == 0)
			dq_clk_phase_match_pre = dq_clk_phase_match0_pre & 7;
		else
			dq_clk_phase_match_pre = dq_clk_phase_match1_pre & 7;
		dqs_dq_clk_phase_match_delta = dq_clk_phase_match_pre - phy_clk_phase_match_pre;
		if (dqs_dq_clk_phase_match_delta <0)
			dqs_dq_clk_phase_match_delta = 16 + dqs_dq_clk_phase_match_delta;
		wr_dqX_a_timing_adj = dq_a_timing_reg - dqs_dq_clk_phase_match_delta;
		if (-15<=wr_dqX_a_timing_adj<=-8)
			wr_dqX_a_timing_adj= 16+wr_dqX_a_timing_adj;
		else if (-7<=wr_dqX_a_timing_adj<0)
			wr_dqX_a_timing_adj = 8+wr_dqX_a_timing_adj;
		start_wrlvl_delay_mod_array[slice] = start_wrlvl_delay_mod & 15;
		wr_dqX_a_timing_adj_array[slice]   = wr_dqX_a_timing_adj & 7;
	}
	for(slice = 0; slice < NO_OF_DATA_SLICES; slice++){
		start_wrlvl_delay_mod = start_wrlvl_delay_mod_array[slice];
		wr_dqX_a_timing_adj   = wr_dqX_a_timing_adj_array[slice];
		dq_a_timing_reg       = start_wrlvl_delay_mod - dq_a_timing_reg_offset;
		if (dq_a_timing_reg < 0)
			dq_a_timing_reg= 8+dq_a_timing_reg;
		else if (dq_a_timing_reg > 7)
			dq_a_timing_reg=dq_a_timing_reg - 8;
		table_index = dqs_dq_clk_phase_match_delta;
		selected_wr_dq_dqs_gate_coarse_assert   = wr_dq_dqs_gate_coarse_assert[table_index][start_wrlvl_delay_mod];
		selected_wr_dq_dqs_gate_fine_assert     = wr_dq_dqs_gate_fine_assert[table_index][start_wrlvl_delay_mod];
		selected_wr_dq_dqs_gate_coarse_deassert = wr_dq_dqs_gate_coarse_deassert[table_index][start_wrlvl_delay_mod];
		selected_wr_dq_dqs_gate_fine_deassert   = wr_dq_dqs_gate_fine_deassert[table_index][start_wrlvl_delay_mod];
		temp_reg = selected_wr_dq_dqs_gate_coarse_assert +  (selected_wr_dq_dqs_gate_fine_assert << 3) +  (selected_wr_dq_dqs_gate_coarse_deassert <<5) +  (selected_wr_dq_dqs_gate_fine_deassert <<8);
		switch(slice)
		{
		case 0:
			write_reg_field( temp_reg , 0x000003FF ,DENALI_CTL_150);
			break;
		case 1:
			write_reg_field(temp_reg, 0x000003FF   ,DENALI_CTL_151);
			break;
		case 2:
			write_reg_field(temp_reg, 0x000003FF  ,DENALI_CTL_152);
			break;
		case 3:
			write_reg_field(temp_reg, 0x000003FF  ,DENALI_CTL_153);
			break;
		case 4:
			write_reg_field(temp_reg, 0x000003FF  ,DENALI_CTL_154);
			break;
		}
		temp_reg0 = wr_dqs_gate_coarse_assert[0][start_wrlvl_delay_mod];
		temp_reg1 =  wr_dqs_gate_fine_assert[0][start_wrlvl_delay_mod];
		temp_reg2 =  wr_dqs_gate_coarse_deassert[0][start_wrlvl_delay_mod];
		temp_reg3 = wr_dqs_gate_fine_deassert[0][start_wrlvl_delay_mod];
		temp_reg4 = add_clk_and_half(wr_dqs_gate_coarse_assert[0][start_wrlvl_delay_mod], wr_dqs_gate_fine_assert[0][start_wrlvl_delay_mod],0);
		temp_reg5 =  add_clk_and_half(wr_dqs_gate_coarse_assert[0][start_wrlvl_delay_mod], wr_dqs_gate_fine_assert[0][start_wrlvl_delay_mod],1);
		temp_reg = temp_reg0 + (temp_reg1 << 3) + (temp_reg2 << 5) + (temp_reg3 << 8) + (temp_reg0 << 10) + (temp_reg1 << 13) + (temp_reg4 << 15) + (temp_reg5 << 18) + (temp_reg0 << 20) + (temp_reg1 << 23) + (temp_reg4 << 25) + (temp_reg5 << 28);
		switch(slice)
		{
		case 0:
			write_reg_field( temp_reg , 0x3FFFFFFF ,DENALI_CTL_130);
			break;
		case 1:
			write_reg_field(temp_reg, 0x3FFFFFFF  ,DENALI_CTL_131);
			break;
		case 2:
			write_reg_field(temp_reg, 0x3FFFFFFF ,DENALI_CTL_132);
			break;
		case 3:
			write_reg_field(temp_reg, 0x3FFFFFFF ,DENALI_CTL_133);
			break;
		case 4:
			write_reg_field(temp_reg, 0x3FFFFFFF ,DENALI_CTL_134);
			break;
		}
	}
}

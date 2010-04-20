#include <asm/arch/lvl_code.h>
typedef enum { IDLE, INC2ZERO, INC2NZERO, INC2ONE, DEC2ZERO, DEC2ONE, LVLDONE, ERROR } SW_LEVELING_STATE_TYPE;
   SW_LEVELING_STATE_TYPE m_swlvl_state;
int m_max_delay_gtrn;
int j;
int k;

void mode_specific_setup()
{
	write_reg_field(TDFI_RDLVL_RR,TDFI_RDLVL_RR_MASK,DENALI_CTL_64);
	m_max_delay_gtrn = 63;
}
   void  mode_specific_delay_loop(int slice, int delayval)
{
	 	switch(slice)
	{
		case 0:
			write_reg_field((delayval & 7)<<16,RDLVL_GATE_DELAY_0_MASK,DENALI_CTL_86);
			write_reg_field(((delayval >> 3) & 7)<<20,7<<20,DENALI_CTL_124);
			break;
		case 1:
			write_reg_field(delayval & 7,RDLVL_GATE_DELAY_1_MASK,DENALI_CTL_87);
			write_reg_field(((delayval >> 3) & 7)<<20,7<<20,DENALI_CTL_125);
			break;
		case 2:
			write_reg_field((delayval & 7)<<16,RDLVL_GATE_DELAY_2_MASK,DENALI_CTL_87);
			write_reg_field(((delayval >> 3) & 7)<<20,7<<20,DENALI_CTL_126);
			break;
		case 3:
			write_reg_field(delayval & 7,RDLVL_GATE_DELAY_3_MASK,DENALI_CTL_88);
			write_reg_field(((delayval >> 3) & 7)<<20,7<<20,DENALI_CTL_127);
			break;
		case 4:
			write_reg_field((delayval & 7)<<16,RDLVL_GATE_DELAY_4_MASK,DENALI_CTL_88);
			write_reg_field(((delayval >> 3) & 7)<<20,7<<20,DENALI_CTL_128);
			break;
	}
   }
   void tweak(){
       int slice;
      for ( slice=0; slice<NO_OF_DATA_SLICES; slice++) {
         int coarse_setting;
         int tmp_reg;
	 int tmp_reg1;
	 	 	switch(slice)
	{
		case 0:
			tmp_reg1=read_reg_field(20,(7<<20),DENALI_CTL_124);
         		tmp_reg=read_reg_field(16,RDLVL_GATE_DELAY_0_MASK,DENALI_CTL_86);
       			if (tmp_reg == 7)
         		tmp_reg = 0;
			else{
			tmp_reg1-=1;
			tmp_reg = (8 + tmp_reg) - 7;
			}
			write_reg_field(tmp_reg<<16,RDLVL_GATE_DELAY_0_MASK,DENALI_CTL_86);
			write_reg_field(tmp_reg1<<20,7<<20,DENALI_CTL_124);
			break;
		case 1:
         		tmp_reg1=read_reg_field(20,(7<<20),DENALI_CTL_125);
         		tmp_reg=read_reg_field(0,RDLVL_GATE_DELAY_1_MASK,DENALI_CTL_87);
       			if (tmp_reg == 7)
         		tmp_reg = 0;
			else{
			tmp_reg1-=1;
			tmp_reg = (8 + tmp_reg) - 7;
			}
			write_reg_field(tmp_reg,RDLVL_GATE_DELAY_1_MASK,DENALI_CTL_87);
			write_reg_field(tmp_reg1<<20,7<<20,DENALI_CTL_125);
			break;
		case 2:
         		tmp_reg1=read_reg_field(20,(7<<20),DENALI_CTL_126);
         		tmp_reg=read_reg_field(16,RDLVL_GATE_DELAY_2_MASK,DENALI_CTL_87);
       			if (tmp_reg == 7)
         		tmp_reg = 0;
			else{
			tmp_reg1-=1;
			tmp_reg = (8 + tmp_reg) - 7;
			}
			write_reg_field(tmp_reg<<16,RDLVL_GATE_DELAY_2_MASK,DENALI_CTL_87);
			write_reg_field(tmp_reg1<<20,7<<20,DENALI_CTL_126);
			break;
		case 3:
         		tmp_reg1=read_reg_field(20,(7<<20),DENALI_CTL_127);
         		tmp_reg=read_reg_field(0,RDLVL_GATE_DELAY_3_MASK,DENALI_CTL_88);
       			if (tmp_reg == 7)
         		tmp_reg = 0;
			else{
			tmp_reg1-=1;
			tmp_reg = (8 + tmp_reg) - 7;
			}
			write_reg_field(tmp_reg,RDLVL_GATE_DELAY_3_MASK,DENALI_CTL_88);
			write_reg_field(tmp_reg1<<20,7<<20,DENALI_CTL_127);
			break;
		case 4:
         		tmp_reg1=read_reg_field(20,(7<<20),DENALI_CTL_128);
         		tmp_reg=read_reg_field(16,RDLVL_GATE_DELAY_4_MASK,DENALI_CTL_88);
       			if (tmp_reg == 7)
         		tmp_reg = 0;
			else{
			tmp_reg1-=1;
			tmp_reg = (8 + tmp_reg) - 7;
			}
			write_reg_field(tmp_reg<<16,RDLVL_GATE_DELAY_4_MASK,DENALI_CTL_88);
			write_reg_field(tmp_reg1<<20,7<<20,DENALI_CTL_128);
			break;
	}
    }
       setbit(24,DENALI_CTL_12);
      waitForOpDone();
   }
void doGateTrainingLeveling()
{
	int resp;
	int slice;
	for(slice=0; slice < NO_OF_DATA_SLICES; slice++) {
		int tmp_reg;
		switch(slice)
		{
		case 0:
			write_reg_field(2145<<8,4095<<8,DENALI_CTL_124);
			break;
		case 1:
			write_reg_field(2145<<8,4095<<8,DENALI_CTL_125);
			break;
		case 2:
			write_reg_field(2145<<8,4095<<8,DENALI_CTL_126);
			break;
		case 3:
			write_reg_field(2145<<8,4095<<8,DENALI_CTL_127);
			break;
		case 4:
			write_reg_field(2145<<8,4095<<8,DENALI_CTL_128);
			break;
		}
	}
	write_reg_field (GATE_TRN,SW_LEVELING_MODE_MASK,DENALI_CTL_20);
	mode_specific_setup();
	for(slice=0; slice < NO_OF_DATA_SLICES; slice++) {
		int delayval;
		setbit(SWLVL_START,DENALI_CTL_13);
		waitForOpDone();
#if 0
		switch(slice)
		{
		case 0:
			delayval=read_reg_field(16,RDLVL_GATE_DELAY_0_MASK,DENALI_CTL_86);
			break;
		case 1:
			delayval=read_reg_field(0,RDLVL_GATE_DELAY_1_MASK,DENALI_CTL_87);
			break;
		case 2:
			delayval=read_reg_field(16,RDLVL_GATE_DELAY_2_MASK,DENALI_CTL_87);
			break;
		case 3:
			delayval=read_reg_field(0,RDLVL_GATE_DELAY_3_MASK,DENALI_CTL_88);
			break;
		case 4:
			delayval=read_reg_field(16,RDLVL_GATE_DELAY_4_MASK,DENALI_CTL_88);
			break;
		}
		setbit(24,DENALI_CTL_12);
		waitForOpDone();
#endif
		m_swlvl_state = IDLE;
		delayval = 16;
		while((delayval <= m_max_delay_gtrn) && (m_swlvl_state != LVLDONE) && (m_swlvl_state != ERROR)) {
			mode_specific_delay_loop(slice, delayval);
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
			switch (m_swlvl_state)
			{
			case IDLE:     if ((resp & 1) == 1) m_swlvl_state= INC2ZERO; else m_swlvl_state=  INC2ONE; break;
			case INC2ZERO: if (delayval == m_max_delay_gtrn) m_swlvl_state=ERROR; else if ((resp & 1) == 1) m_swlvl_state=INC2ZERO; else m_swlvl_state=INC2ONE; break;
			case INC2ONE:  if (delayval == m_max_delay_gtrn) m_swlvl_state=ERROR; else if ((resp & 1) == 1) m_swlvl_state=LVLDONE;  else m_swlvl_state=INC2ONE; break;
#if 0
			case DEC2ZERO: if (delayval == m_max_delay_gtrn) m_swlvl_state=ERROR; else if ((resp & 1) == 1) m_swlvl_state=DEC2ZERO; else m_swlvl_state=INC2ONE; break;
#endif
			case LVLDONE:  m_swlvl_state = LVLDONE; break;
			case ERROR:    m_swlvl_state = ERROR; break;
			}
			if  (m_swlvl_state == INC2ONE) delayval = delayval+1;
			else if (m_swlvl_state == INC2ZERO) delayval = delayval+1;
			else if (m_swlvl_state == DEC2ZERO) delayval = delayval+1;
			else delayval = delayval;
		}
		if (m_swlvl_state == ERROR)
			while(1);
		else
		{}
		setbit(16,DENALI_CTL_12);
		waitForOpDone();
	}
	setbit(SWLVL_START,DENALI_CTL_13);
	waitForOpDone();
	tweak();
	setbit(16,DENALI_CTL_12);
	waitForOpDone();
}


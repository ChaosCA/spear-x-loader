#ifndef __LEVELLING_H
#define __LEVELLING_H
//#include <types.h>
//#include <hwconfig.h>
typedef unsigned int u32;
#define NO_OF_DATA_SLICES	4

#define readl( value, Address )\
		( value = (u32)(*Address) )
		
#define writel( value, Address )\
                    ( (*(u32*)Address = (u32)value)  )
		    
#define setbit( pos, Address )\
		( *Address |= (u32)(1<<pos) )
		
#define clearbit( pos, Address )\
		( (*Address) &= ~((u32)(1<<pos)) )
		    



void write_reg_field(int value,int mask, volatile unsigned int *address);
	
int read_reg_field(int shift,int mask, volatile unsigned int *address);

void waitForOpDone(void);	
#define MPMC0_B2_START 0xEC000000
#define MPMC_Base (MPMC0_B2_START)

#define DENALI_CTL_00	((volatile unsigned int*)(MPMC_Base + 0x00))		
#define DENALI_CTL_01	((volatile unsigned int*)(MPMC_Base + 0x04))
#define DENALI_CTL_02	((volatile unsigned int*)(MPMC_Base + 0x08))
#define DENALI_CTL_03	((volatile unsigned int*)(MPMC_Base + 0x0C))
#define DENALI_CTL_04	((volatile unsigned int*)(MPMC_Base + 0x10))
#define DENALI_CTL_05	((volatile unsigned int*)(MPMC_Base + 0x14))
#define DENALI_CTL_06	((volatile unsigned int*)(MPMC_Base + 0x18))
#define DENALI_CTL_07	((volatile unsigned int*)(MPMC_Base + 0x1C))
#define DENALI_CTL_08	((volatile unsigned int*)(MPMC_Base + 0x20))
#define DENALI_CTL_09	((volatile unsigned int*)(MPMC_Base + 0x24))
#define DENALI_CTL_10	((volatile unsigned int*)(MPMC_Base + 0x28))
#define DENALI_CTL_11	((volatile unsigned int*)(MPMC_Base + 0x2C))
#define DENALI_CTL_12	((volatile unsigned int*)(MPMC_Base + 0x30))
#define DENALI_CTL_13	((volatile unsigned int*)(MPMC_Base + 0x34))
	#define SWLVL_OP_DONE 1
	#define SWLVL_START 8
#define DENALI_CTL_14	((volatile unsigned int*)(MPMC_Base + 0x38))
#define DENALI_CTL_15	((volatile unsigned int*)(MPMC_Base + 0x3C))
#define DENALI_CTL_16	((volatile unsigned int*)(MPMC_Base + 0x40))
#define DENALI_CTL_17	((volatile unsigned int*)(MPMC_Base + 0x44))
#define DENALI_CTL_18	((volatile unsigned int*)(MPMC_Base + 0x48))
#define DENALI_CTL_19	((volatile unsigned int*)(MPMC_Base + 0x4C))		
#define DENALI_CTL_20	((volatile unsigned int*)(MPMC_Base + 0x50))
	#define SW_LEVELING_MODE_MASK 3<<8
	#define NO_LVL    0<<8	
	#define WRITE_LVL 1<<8
	#define READ_LVL  2<<8
	#define GATE_TRN  3<<8
		
#define DENALI_CTL_21	((volatile unsigned int*)(MPMC_Base + 0x54))
#define DENALI_CTL_22	((volatile unsigned int*)(MPMC_Base + 0x58))
#define DENALI_CTL_23	((volatile unsigned int*)(MPMC_Base + 0x5C))		
#define DENALI_CTL_24	((volatile unsigned int*)(MPMC_Base + 0x60))
#define DENALI_CTL_25	((volatile unsigned int*)(MPMC_Base + 0x64))
#define DENALI_CTL_26	((volatile unsigned int*)(MPMC_Base + 0x68))		
#define DENALI_CTL_27	((volatile unsigned int*)(MPMC_Base + 0x6C))
#define DENALI_CTL_28	((volatile unsigned int*)(MPMC_Base + 0x70))
#define DENALI_CTL_29	((volatile unsigned int*)(MPMC_Base + 0x74))		
#define DENALI_CTL_30	((volatile unsigned int*)(MPMC_Base + 0x78))		
#define DENALI_CTL_31	((volatile unsigned int*)(MPMC_Base + 0x7C))
#define DENALI_CTL_32	((volatile unsigned int*)(MPMC_Base + 0x80))
#define DENALI_CTL_33	((volatile unsigned int*)(MPMC_Base + 0x84))		
#define DENALI_CTL_34	((volatile unsigned int*)(MPMC_Base + 0x88))
#define DENALI_CTL_35	((volatile unsigned int*)(MPMC_Base + 0x8C))
#define DENALI_CTL_36	((volatile unsigned int*)(MPMC_Base + 0x90))		
#define DENALI_CTL_37	((volatile unsigned int*)(MPMC_Base + 0x94))
#define DENALI_CTL_38	((volatile unsigned int*)(MPMC_Base + 0x98))
#define DENALI_CTL_39	((volatile unsigned int*)(MPMC_Base + 0x9C))		
#define DENALI_CTL_40	((volatile unsigned int*)(MPMC_Base + 0xA0))		
#define DENALI_CTL_41	((volatile unsigned int*)(MPMC_Base + 0xA4))
#define DENALI_CTL_42	((volatile unsigned int*)(MPMC_Base + 0xA8))
#define DENALI_CTL_43	((volatile unsigned int*)(MPMC_Base + 0xAC))		
#define DENALI_CTL_44	((volatile unsigned int*)(MPMC_Base + 0xB0))
#define DENALI_CTL_45	((volatile unsigned int*)(MPMC_Base + 0xB4))
#define DENALI_CTL_46	((volatile unsigned int*)(MPMC_Base + 0xB8))		
#define DENALI_CTL_47	((volatile unsigned int*)(MPMC_Base + 0xBC))
#define DENALI_CTL_48	((volatile unsigned int*)(MPMC_Base + 0xC0))
#define DENALI_CTL_49	((volatile unsigned int*)(MPMC_Base + 0xC4))		
#define DENALI_CTL_50	((volatile unsigned int*)(MPMC_Base + 0xC8))		
#define DENALI_CTL_51	((volatile unsigned int*)(MPMC_Base + 0xCC))
#define DENALI_CTL_52	((volatile unsigned int*)(MPMC_Base + 0xD0))
#define DENALI_CTL_53	((volatile unsigned int*)(MPMC_Base + 0xD4))
        #define DLL_LOCK_MASK 127 <<  8	
#define DENALI_CTL_54	((volatile unsigned int*)(MPMC_Base + 0xD8))
#define DENALI_CTL_55	((volatile unsigned int*)(MPMC_Base + 0xDC))
#define DENALI_CTL_56	((volatile unsigned int*)(MPMC_Base + 0xE0))		
#define DENALI_CTL_57	((volatile unsigned int*)(MPMC_Base + 0xE4))
        #define SWLVL_RESP_0_MASK 255 << 16
        #define SWLVL_RESP_1_MASK 255 << 24
#define DENALI_CTL_58	((volatile unsigned int*)(MPMC_Base + 0xE8))
        #define SWLVL_RESP_2_MASK  255 << 0
        #define SWLVL_RESP_3_MASK  255 << 8
        #define SWLVL_RESP_4_MASK  255 << 16
#define DENALI_CTL_59	((volatile unsigned int*)(MPMC_Base + 0xEC))		
#define DENALI_CTL_60	((volatile unsigned int*)(MPMC_Base + 0xF0))		
#define DENALI_CTL_61	((volatile unsigned int*)(MPMC_Base + 0xF4))
#define DENALI_CTL_62	((volatile unsigned int*)(MPMC_Base + 0xF8))
#define DENALI_CTL_63	((volatile unsigned int*)(MPMC_Base + 0xFC))		
#define DENALI_CTL_64	((volatile unsigned int*)(MPMC_Base + 0x100))
        #define TDFI_RDLVL_RR_MASK 1023<<16
        #define TDFI_RDLVL_RR      16<<16
#define DENALI_CTL_65	((volatile unsigned int*)(MPMC_Base + 0x104))
#define DENALI_CTL_66	((volatile unsigned int*)(MPMC_Base + 0x108))		
#define DENALI_CTL_67	((volatile unsigned int*)(MPMC_Base + 0x10C))
#define DENALI_CTL_68	((volatile unsigned int*)(MPMC_Base + 0x110))
#define DENALI_CTL_69	((volatile unsigned int*)(MPMC_Base + 0x114))		
#define DENALI_CTL_70	((volatile unsigned int*)(MPMC_Base + 0x118))		
#define DENALI_CTL_71	((volatile unsigned int*)(MPMC_Base + 0x11C))
#define DENALI_CTL_72	((volatile unsigned int*)(MPMC_Base + 0x120))
#define DENALI_CTL_73	((volatile unsigned int*)(MPMC_Base + 0x124))		
#define DENALI_CTL_74	((volatile unsigned int*)(MPMC_Base + 0x128))
#define DENALI_CTL_75	((volatile unsigned int*)(MPMC_Base + 0x12C))
#define DENALI_CTL_76	((volatile unsigned int*)(MPMC_Base + 0x130))		
#define DENALI_CTL_77	((volatile unsigned int*)(MPMC_Base + 0x134))
#define DENALI_CTL_78	((volatile unsigned int*)(MPMC_Base + 0x138))
#define DENALI_CTL_79	((volatile unsigned int*)(MPMC_Base + 0x13C))		
#define DENALI_CTL_80	((volatile unsigned int*)(MPMC_Base + 0x140))		
#define DENALI_CTL_81	((volatile unsigned int*)(MPMC_Base + 0x144))
             #define RDLVL_DELAY_0_MASK 0xFFFF0000	
#define DENALI_CTL_82	((volatile unsigned int*)(MPMC_Base + 0x148))
	    #define RDLVL_DELAY_1_MASK 0x0000FFFF	
	    #define RDLVL_DELAY_2_MASK 0xFFFF0000	
#define DENALI_CTL_83	((volatile unsigned int*)(MPMC_Base + 0x14C))	
	    #define RDLVL_DELAY_3_MASK 0x0000FFFF	
	    #define RDLVL_DELAY_4_MASK 0xFFFF0000		
#define DENALI_CTL_84	((volatile unsigned int*)(MPMC_Base + 0x150))
#define DENALI_CTL_85	((volatile unsigned int*)(MPMC_Base + 0x154))
#define DENALI_CTL_86	((volatile unsigned int*)(MPMC_Base + 0x158))
	    #define RDLVL_GATE_DELAY_0_MASK 0xFFFF0000	
#define DENALI_CTL_87	((volatile unsigned int*)(MPMC_Base + 0x15C))
	    #define RDLVL_GATE_DELAY_1_MASK 0x0000FFFF	
	    #define RDLVL_GATE_DELAY_2_MASK 0xFFFF0000	
#define DENALI_CTL_88	((volatile unsigned int*)(MPMC_Base + 0x160))
	    #define RDLVL_GATE_DELAY_3_MASK 0x0000FFFF	
	    #define RDLVL_GATE_DELAY_4_MASK 0xFFFF0000	
#define DENALI_CTL_89	((volatile unsigned int*)(MPMC_Base + 0x164))		
#define DENALI_CTL_90	((volatile unsigned int*)(MPMC_Base + 0x168))		
#define DENALI_CTL_91	((volatile unsigned int*)(MPMC_Base + 0x16C))
#define DENALI_CTL_92	((volatile unsigned int*)(MPMC_Base + 0x170))
#define DENALI_CTL_93	((volatile unsigned int*)(MPMC_Base + 0x174))		
#define DENALI_CTL_94	((volatile unsigned int*)(MPMC_Base + 0x178))
#define DENALI_CTL_95	((volatile unsigned int*)(MPMC_Base + 0x17C))
#define DENALI_CTL_96	((volatile unsigned int*)(MPMC_Base + 0x180))		
#define DENALI_CTL_97	((volatile unsigned int*)(MPMC_Base + 0x184))
#define DENALI_CTL_98	((volatile unsigned int*)(MPMC_Base + 0x188))
#define DENALI_CTL_99	((volatile unsigned int*)(MPMC_Base + 0x18C))		
#define DENALI_CTL_100	((volatile unsigned int*)(MPMC_Base + 0x190))		
            #define WRLVL_DELAY_0_MASK 0xFFFF 
            #define WRLVL_DELAY_1_MASK 0xFFFF0000
#define DENALI_CTL_101	((volatile unsigned int*)(MPMC_Base + 0x194))
            #define WRLVL_DELAY_2_MASK 0xFFFF 
            #define WRLVL_DELAY_3_MASK 0xFFFF0000
#define DENALI_CTL_102	((volatile unsigned int*)(MPMC_Base + 0x198))
            #define WRLVL_DELAY_4_MASK 0xFFFF 
#define DENALI_CTL_103	((volatile unsigned int*)(MPMC_Base + 0x19C))		
#define DENALI_CTL_104	((volatile unsigned int*)(MPMC_Base + 0x1A0))
#define DENALI_CTL_105	((volatile unsigned int*)(MPMC_Base + 0x1A4))
#define DENALI_CTL_106	((volatile unsigned int*)(MPMC_Base + 0x1A8))		
#define DENALI_CTL_107	((volatile unsigned int*)(MPMC_Base + 0x1AC))
#define DENALI_CTL_108	((volatile unsigned int*)(MPMC_Base + 0x1B0))
#define DENALI_CTL_109	((volatile unsigned int*)(MPMC_Base + 0x1B4))		
#define DENALI_CTL_110	((volatile unsigned int*)(MPMC_Base + 0x1B8))		
#define DENALI_CTL_111	((volatile unsigned int*)(MPMC_Base + 0x1BC))
#define DENALI_CTL_112	((volatile unsigned int*)(MPMC_Base + 0x1C0))
#define DENALI_CTL_113	((volatile unsigned int*)(MPMC_Base + 0x1C4))		
#define DENALI_CTL_114	((volatile unsigned int*)(MPMC_Base + 0x1C8))
#define DENALI_CTL_115	((volatile unsigned int*)(MPMC_Base + 0x1CC))
#define DENALI_CTL_116	((volatile unsigned int*)(MPMC_Base + 0x1D0))		
#define DENALI_CTL_117	((volatile unsigned int*)(MPMC_Base + 0x1D4))
#define DENALI_CTL_118	((volatile unsigned int*)(MPMC_Base + 0x1D8))
#define DENALI_CTL_119	((volatile unsigned int*)(MPMC_Base + 0x1DC))		
#define DENALI_CTL_120	((volatile unsigned int*)(MPMC_Base + 0x1E0))		
#define DENALI_CTL_121	((volatile unsigned int*)(MPMC_Base + 0x1E4))
#define DENALI_CTL_122	((volatile unsigned int*)(MPMC_Base + 0x1E8))
#define DENALI_CTL_123	((volatile unsigned int*)(MPMC_Base + 0x1EC))		
#define DENALI_CTL_124	((volatile unsigned int*)(MPMC_Base + 0x1F0))
#define DENALI_CTL_125	((volatile unsigned int*)(MPMC_Base + 0x1F4))
#define DENALI_CTL_126	((volatile unsigned int*)(MPMC_Base + 0x1F8))		
#define DENALI_CTL_127	((volatile unsigned int*)(MPMC_Base + 0x1FC))
#define DENALI_CTL_128	((volatile unsigned int*)(MPMC_Base + 0x200))
#define DENALI_CTL_129	((volatile unsigned int*)(MPMC_Base + 0x204))
		#define WRLVL_BASE_OFFSET_REG_MASK 7<<20	
#define DENALI_CTL_130	((volatile unsigned int*)(MPMC_Base + 0x208))		
                #define WR_DQS_GATE_FINE_ASSERT_MASK 3 << 3
                #define DEASSERT_FINE_MASK 3 << 8
#define DENALI_CTL_131	((volatile unsigned int*)(MPMC_Base + 0x20C))
#define DENALI_CTL_132	((volatile unsigned int*)(MPMC_Base + 0x210))
#define DENALI_CTL_133	((volatile unsigned int*)(MPMC_Base + 0x214))		
#define DENALI_CTL_134	((volatile unsigned int*)(MPMC_Base + 0x218))
#define DENALI_CTL_135	((volatile unsigned int*)(MPMC_Base + 0x21C))
#define DENALI_CTL_136	((volatile unsigned int*)(MPMC_Base + 0x220))		
#define DENALI_CTL_137	((volatile unsigned int*)(MPMC_Base + 0x224))
#define DENALI_CTL_138	((volatile unsigned int*)(MPMC_Base + 0x228))
#define DENALI_CTL_139	((volatile unsigned int*)(MPMC_Base + 0x22C))		
#define DENALI_CTL_140	((volatile unsigned int*)(MPMC_Base + 0x230))		
#define DENALI_CTL_141	((volatile unsigned int*)(MPMC_Base + 0x234))
#define DENALI_CTL_142	((volatile unsigned int*)(MPMC_Base + 0x238))
#define DENALI_CTL_143	((volatile unsigned int*)(MPMC_Base + 0x23C))		
#define DENALI_CTL_144	((volatile unsigned int*)(MPMC_Base + 0x240))
#define DENALI_CTL_145	((volatile unsigned int*)(MPMC_Base + 0x244))
#define DENALI_CTL_146	((volatile unsigned int*)(MPMC_Base + 0x248))		
#define DENALI_CTL_147	((volatile unsigned int*)(MPMC_Base + 0x24C))
#define DENALI_CTL_148	((volatile unsigned int*)(MPMC_Base + 0x250))
#define DENALI_CTL_149	((volatile unsigned int*)(MPMC_Base + 0x254))		
#define DENALI_CTL_150	((volatile unsigned int*)(MPMC_Base + 0x258))		
#define DENALI_CTL_151	((volatile unsigned int*)(MPMC_Base + 0x25C))
#define DENALI_CTL_152	((volatile unsigned int*)(MPMC_Base + 0x260))
#define DENALI_CTL_153	((volatile unsigned int*)(MPMC_Base + 0x264))		
#define DENALI_CTL_154	((volatile unsigned int*)(MPMC_Base + 0x268))
#define DENALI_CTL_155	((volatile unsigned int*)(MPMC_Base + 0x26C))
#define DENALI_CTL_156	((volatile unsigned int*)(MPMC_Base + 0x270))		
#define DENALI_CTL_157	((volatile unsigned int*)(MPMC_Base + 0x274))
#define DENALI_CTL_158	((volatile unsigned int*)(MPMC_Base + 0x278))
#define DENALI_CTL_159	((volatile unsigned int*)(MPMC_Base + 0x27C))		
#define DENALI_CTL_160	((volatile unsigned int*)(MPMC_Base + 0x280))		
#define DENALI_CTL_161	((volatile unsigned int*)(MPMC_Base + 0x284))
#define DENALI_CTL_162	((volatile unsigned int*)(MPMC_Base + 0x288))
#define DENALI_CTL_163	((volatile unsigned int*)(MPMC_Base + 0x28C))		
#define DENALI_CTL_164	((volatile unsigned int*)(MPMC_Base + 0x290))
#define DENALI_CTL_165	((volatile unsigned int*)(MPMC_Base + 0x294))
#define DENALI_CTL_166	((volatile unsigned int*)(MPMC_Base + 0x298))		
#define DENALI_CTL_167	((volatile unsigned int*)(MPMC_Base + 0x29C))
#define DENALI_CTL_168	((volatile unsigned int*)(MPMC_Base + 0x2A0))
#define DENALI_CTL_169	((volatile unsigned int*)(MPMC_Base + 0x2A4))		
#define DENALI_CTL_170	((volatile unsigned int*)(MPMC_Base + 0x2A8))		
           #define PATT_MUX_DATA1_MASK 255 << 23
#define DENALI_CTL_171	((volatile unsigned int*)(MPMC_Base + 0x2AC))
#define DENALI_CTL_172	((volatile unsigned int*)(MPMC_Base + 0x2B0))
#define DENALI_CTL_173	((volatile unsigned int*)(MPMC_Base + 0x2B4))		
#define DENALI_CTL_174	((volatile unsigned int*)(MPMC_Base + 0x2B8))
#define DENALI_CTL_175	((volatile unsigned int*)(MPMC_Base + 0x2BC))
           #define PATT_DQS_MASK 255 << 0
           #define PATT_MUX_DQS_MASK 255 << 8
           #define PATT_MUX_DATA_MASK 255 << 16
#define DENALI_CTL_176	((volatile unsigned int*)(MPMC_Base + 0x2C0))		
#define DENALI_CTL_177	((volatile unsigned int*)(MPMC_Base + 0x2C4))
#define DENALI_CTL_178	((volatile unsigned int*)(MPMC_Base + 0x2C8))
#define DENALI_CTL_179	((volatile unsigned int*)(MPMC_Base + 0x2CC))		
#define DENALI_CTL_180	((volatile unsigned int*)(MPMC_Base + 0x2D0))		
#define DENALI_CTL_181	((volatile unsigned int*)(MPMC_Base + 0x2D4))
	#define cal_clk_byte_patt_mask 0xFF<<0
	#define cal_ref_byte_patt_mask 0xFF<<8
#define DENALI_CTL_182	((volatile unsigned int*)(MPMC_Base + 0x2D8))
#define DENALI_CTL_183	((volatile unsigned int*)(MPMC_Base + 0x2DC))		
#define DENALI_CTL_184	((volatile unsigned int*)(MPMC_Base + 0x2E0))
#define DENALI_CTL_185	((volatile unsigned int*)(MPMC_Base + 0x2E4))
#define DENALI_CTL_186	((volatile unsigned int*)(MPMC_Base + 0x2E8))		
#define DENALI_CTL_187	((volatile unsigned int*)(MPMC_Base + 0x2EC))
#define DENALI_CTL_188	((volatile unsigned int*)(MPMC_Base + 0x2F0))
#define DENALI_CTL_189	((volatile unsigned int*)(MPMC_Base + 0x2F4))		
#define DENALI_CTL_190	((volatile unsigned int*)(MPMC_Base + 0x2F8))		
#define DENALI_CTL_191	((volatile unsigned int*)(MPMC_Base + 0x3FC))
#define DENALI_CTL_192	((volatile unsigned int*)(MPMC_Base + 0x300))
#define DENALI_CTL_193	((volatile unsigned int*)(MPMC_Base + 0x304))		
#define DENALI_CTL_194	((volatile unsigned int*)(MPMC_Base + 0x308))
#define DENALI_CTL_195	((volatile unsigned int*)(MPMC_Base + 0x30C))
#define DENALI_CTL_196	((volatile unsigned int*)(MPMC_Base + 0x310))		
#define DENALI_CTL_197	((volatile unsigned int*)(MPMC_Base + 0x314))
#define DENALI_CTL_198	((volatile unsigned int*)(MPMC_Base + 0x318))
#define DENALI_CTL_199	((volatile unsigned int*)(MPMC_Base + 0x31C))		
#define DENALI_CTL_200	((volatile unsigned int*)(MPMC_Base + 0x320))		
#endif

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

#ifndef __TABLE_H__
#define __TABLE_H__

/*
 * Xloader Table Structures
 */
#define XLOADER_TABLE_VERSION_1_1	2
#define XLOADER_TABLE_VERSION_1_2	3

struct xloader_table_1_1 {
	unsigned short ddrfreq;
	unsigned char ddrsize;
	unsigned char ddrtype;
#define DDR2		2

	unsigned char soc_rev;
#define REV_DYNAMIC	0xFF

} __attribute__ ((packed));

struct xloader_table_1_2 {
	unsigned const char * version;

	unsigned short ddrfreq;
	unsigned char ddrsize;
	unsigned char ddrtype;
	
	unsigned char soc_rev;

} __attribute__ ((packed));

union xloader_ver_table {
	struct xloader_table_1_1 table_1_1;
	struct xloader_table_1_2 table_1_2;
};

struct xloader_table {
	unsigned char table_version;
	union xloader_ver_table table;
} __attribute__ ((packed));

/*
 * BootROM Table Structures
 */
#define BOOTROM_TABLE_ADDRESS		0xFFFF7F00

#define BOOTROM_TABLE_VERSION_1_0	1
#define BOOTROM_TABLE_VERSION_2_0	2

struct bootrom_table_1_0 {
	const void *boot_flashdetectandinit_ptr;
	const void *boot_flashread_ptr;
	const void *boot_nandsanitycheck_ptr;
	const void *boot_nandreadpage_ptr;
};

/* BootROM version 2 tables */
struct soc_type_s {
	u8	soc;
	u8	revision;
};

#define SOC_SPEAR1300 3
#define SOC_1300REVAA 1

struct bootrom_table_2_0 {
	struct soc_type_s * (*get_soc_type)(void);
	u8	(*get_boot_type)(void);
	nand_info_t *nand_info;
	int 	(*nand_read)(nand_info_t *nand, size_t offset, 
			size_t *length, u_char *buffer);
}__attribute__ ((packed));

union bootrom_ver_table {
	struct bootrom_table_1_0 table_1_0;
	struct bootrom_table_2_0 table_2_0;
};

struct bootrom_table {
	const unsigned int table_version;
	union bootrom_ver_table table;
};

#endif

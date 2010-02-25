/*
 * (C) Copyright 2008 Semihalf
 *
 * (C) Copyright 2000-2006
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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
#include <image.h>

uint32_t crc32(uint32_t, const unsigned char *, uint);

/*****************************************************************************/
/* Legacy format routines */
/*****************************************************************************/
int image_check_hcrc(image_header_t *hdr)
{
	ulong hcrc;
	ulong len = image_get_header_size();
	image_header_t header;

	/* Copy header so we can blank CRC field for re-calculation */
	lcopy((const ulong *)hdr, (ulong *)&header, len >> 2);
	image_set_hcrc(&header, 0);

	hcrc = crc32(0, (unsigned char *)&header, len);

	return (hcrc == image_get_hcrc(hdr));
}

int image_check_dcrc(image_header_t *hdr)
{
	ulong data = image_get_load(hdr);
	ulong len = image_get_data_size(hdr);
	ulong dcrc = crc32(0, (unsigned char *)data, len);

	return (dcrc == image_get_dcrc(hdr));
}

int image_check_header(image_header_t *hdr, const char *name)
{
	if (image_check_magic(hdr) &&
	    (!strncmp(image_get_name(hdr), name, strlen(name))) &&
	    image_check_hcrc(hdr)) {
		return TRUE;
	}
	return FALSE;
}

int image_check_data(image_header_t *hdr)
{
	if (image_check_dcrc(hdr))
		return TRUE;

	return FALSE;
}

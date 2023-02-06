/*
 * The NTFS attribute list attribute ($ATTRIBUTE_LIST) definition
 *
 * Copyright (C) 2010-2023, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined( _FSNTFS_MFT_ATTRIBUTE_LIST_H )
#define _FSNTFS_MFT_ATTRIBUTE_LIST_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsntfs_mft_attribute_list_entry_header fsntfs_mft_attribute_list_entry_header_t;

struct fsntfs_mft_attribute_list_entry_header
{
	/* The type
	 * Consists of 4 bytes
	 */
	uint8_t type[ 4 ];

	/* The size
	 * Consists of 2 bytes
	 */
	uint8_t size[ 2 ];

	/* The name size
	 * Consists of 1 byte
	 */
	uint8_t name_size;

	/* The name offset
	 * Consists of 1 byte
	 */
	uint8_t name_offset;

	/* The data first Virtual Cluster Number (VCN)
	 * Consists of 8 bytes
	 */
	uint8_t data_first_vcn[ 8 ];

	/* The file reference
	 * Consists of 8 bytes
	 */
	uint8_t file_reference[ 8 ];

	/* The identifier
	 * Consists of 2 bytes
	 */
	uint8_t identifier[ 2 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSNTFS_MFT_ATTRIBUTE_LIST_H ) */


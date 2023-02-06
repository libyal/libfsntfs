/*
 * The NTFS Master File Table (MFT) attribute definition
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

#if !defined( _FSNTFS_MFT_ATTRIBUTE_H )
#define _FSNTFS_MFT_ATTRIBUTE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsntfs_mft_attribute_header fsntfs_mft_attribute_header_t;

struct fsntfs_mft_attribute_header
{
	/* The type
	 * Consists of 4 bytes
	 */
	uint8_t type[ 4 ];

	/* The size
	 * Consists of 4 bytes
	 */
	uint8_t size[ 4 ];

	/* The non-resident flag
	 * Consists of 1 byte
	 */
	uint8_t non_resident_flag;

	/* The name size
	 * Consists of 1 byte
	 */
	uint8_t name_size;

	/* The name offset
	 * Consists of 2 bytes
	 */
	uint8_t name_offset[ 2 ];

	/* The data flags
	 * Consists of 2 bytes
	 */
	uint8_t data_flags[ 2 ];

	/* The identifier
	 * Consists of 2 bytes
	 */
	uint8_t identifier[ 2 ];
};

typedef struct fsntfs_mft_attribute_resident fsntfs_mft_attribute_resident_t;

struct fsntfs_mft_attribute_resident
{
	/* The data size
	 * Consists of 4 bytes
	 */
	uint8_t data_size[ 4 ];

	/* The data offset
	 * Consists of 2 bytes
	 */
	uint8_t data_offset[ 2 ];

	/* The indexed flag
	 * Consists of 1 byte
	 */
	uint8_t indexed_flag;

	/* Padding
	 * Consists of 1 byte
	 */
	uint8_t padding;
};

typedef struct fsntfs_mft_attribute_non_resident fsntfs_mft_attribute_non_resident_t;

struct fsntfs_mft_attribute_non_resident
{
	/* The data first virtual cluster number (VCN)
	 * Consists of 8 bytes
	 */
	uint8_t data_first_vcn[ 8 ];

	/* The data last virtual cluster number (VCN)
	 * Consists of 8 bytes
	 */
	uint8_t data_last_vcn[ 8 ];

	/* The data runs offset
	 * Consists of 2 bytes
	 */
	uint8_t data_runs_offset[ 2 ];

	/* The compression unit size
	 * Consists of 2 bytes
	 */
	uint8_t compression_unit_size[ 2 ];

	/* Padding
	 * Consists of 4 bytes
	 */
	uint8_t padding[ 4 ];

	/* The allocated data size
	 * Consists of 8 bytes
	 */
	uint8_t allocated_data_size[ 8 ];

	/* The data size
	 * Consists of 8 bytes
	 */
	uint8_t data_size[ 8 ];

	/* The valid data size
	 * Consists of 8 bytes
	 */
	uint8_t valid_data_size[ 8 ];
};

typedef struct fsntfs_mft_attribute_non_resident_compressed fsntfs_mft_attribute_non_resident_compressed_t;

struct fsntfs_mft_attribute_non_resident_compressed
{
	/* The data first virtual cluster number (VCN)
	 * Consists of 8 bytes
	 */
	uint8_t data_first_vcn[ 8 ];

	/* The data last virtual cluster number (VCN)
	 * Consists of 8 bytes
	 */
	uint8_t data_last_vcn[ 8 ];

	/* The data runs offset
	 * Consists of 2 bytes
	 */
	uint8_t data_runs_offset[ 2 ];

	/* The compression unit size
	 * Consists of 2 bytes
	 */
	uint8_t compression_unit_size[ 2 ];

	/* Padding
	 * Consists of 4 bytes
	 */
	uint8_t padding[ 4 ];

	/* The allocated data size
	 * Consists of 8 bytes
	 */
	uint8_t allocated_data_size[ 8 ];

	/* The data size
	 * Consists of 8 bytes
	 */
	uint8_t data_size[ 8 ];

	/* The valid data size
	 * Consists of 8 bytes
	 */
	uint8_t valid_data_size[ 8 ];

	/* The total data size
	 * Consists of 8 bytes
	 */
	uint8_t total_data_size[ 8 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSNTFS_MFT_ATTRIBUTE_H ) */


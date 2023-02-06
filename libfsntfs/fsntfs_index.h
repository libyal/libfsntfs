/*
 * The NTFS index definition
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

#if !defined( _FSNTFS_INDEX_H )
#define _FSNTFS_INDEX_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsntfs_index_root_header fsntfs_index_root_header_t;

struct fsntfs_index_root_header
{
	/* The attribute type
	 * Consists of 4 bytes
	 */
	uint8_t attribute_type[ 4 ];

	/* The collation type
	 * Consists of 4 bytes
	 */
	uint8_t collation_type[ 4 ];

	/* The index entry size
	 * Consists of 4 bytes
	 */
	uint8_t index_entry_size[ 4 ];

	/* The index entry number of cluster blocks
	 * Consists of 4 bytes
	 */
	uint8_t index_entry_number_of_cluster_blocks[ 4 ];
};

typedef struct fsntfs_index_entry_header fsntfs_index_entry_header_t;

struct fsntfs_index_entry_header
{
	/* The signature
	 * Consists of 4 bytes
	 * Contains: "INDX"
	 */
	uint8_t signature[ 4 ];

	/* The fixup values offset
	 * Consists of 2 bytes
	 */
	uint8_t fixup_values_offset[ 2 ];

	/* The number of fixup values
	 * Consists of 2 bytes
	 */
	uint8_t number_of_fixup_values[ 2 ];

	/* The journal sequence number
	 * Consists of 8 bytes
	 */
	uint8_t journal_sequence_number[ 8 ];

	/* The virtual cluster number (VCN)
	 * Consists of 8 bytes
	 */
	uint8_t vcn[ 8 ];
};

typedef struct fsntfs_index_node_header fsntfs_index_node_header_t;

struct fsntfs_index_node_header
{
	/* The index values offset
	 * Consists of 4 bytes
	 * Contains a value relative to the start of the index node header
	 */
	uint8_t index_values_offset[ 4 ];

	/* The size
	 * Consists of 4 bytes
	 */
	uint8_t size[ 4 ];

	/* The allocated size
	 * Consists of 4 bytes
	 */
	uint8_t allocated_size[ 4 ];

	/* The flags
	 * Consists of 4 bytes
	 */
	uint8_t flags[ 4 ];

	/* The index values
	 */
};

typedef struct fsntfs_index_value fsntfs_index_value_t;

struct fsntfs_index_value
{
	/* The file reference
	 * Consists of 8 bytes
	 */
	uint8_t file_reference[ 8 ];

	/* The index value size
	 * Consists of 2 bytes
	 */
	uint8_t size[ 2 ];

	/* The index value key data size
	 * Consists of 2 bytes
	 */
	uint8_t key_data_size[ 2 ];

	/* The index value flags
	 * Consists of 4 bytes
	 */
	uint8_t flags[ 4 ];

	/* The index value key and value data
	 */

	/* The sub node virtual cluster number (VCN)
	 * Consists of 8 bytes
	 */
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSNTFS_INDEX_H ) */


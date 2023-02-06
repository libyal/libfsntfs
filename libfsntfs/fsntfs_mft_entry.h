/*
 * The NTFS Master File Table (MFT) entry definition
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

#if !defined( _FSNTFS_MFT_ENTRY_H )
#define _FSNTFS_MFT_ENTRY_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsntfs_mft_entry_header fsntfs_mft_entry_header_t;

struct fsntfs_mft_entry_header
{
	/* The signature
	 * Consists of 4 bytes
	 * Contains: "FILE"
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

	/* The sequence (value)
	 * Consists of 2 bytes
	 */
	uint8_t sequence[ 2 ];

	/* The reference (link) count
	 * Consists of 2 bytes
	 */
	uint8_t reference_count[ 2 ];

	/* The attributes offset
	 * Consists of 2 bytes
	 */
	uint8_t attributes_offset[ 2 ];

	/* The flags
	 * Consists of 2 bytes
	 */
	uint8_t flags[ 2 ];

	/* The used entry size
	 * Consists of 4 bytes
	 */
	uint8_t used_entry_size[ 4 ];

	/* The total entry size
	 * Consists of 4 bytes
	 */
	uint8_t total_entry_size[ 4 ];

	/* The base record file reference
	 * Consists of 8 bytes
	 */
	uint8_t base_record_file_reference[ 8 ];

	/* The first available attribute identifier
	 * Consists of 2 bytes
	 */
	uint8_t first_available_attribute_identifier[ 2 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown1[ 2 ];

	/* The index
	 * Consists of 4 bytes
	 */
	uint8_t index[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSNTFS_MFT_ENTRY_H ) */


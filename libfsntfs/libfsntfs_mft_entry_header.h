/*
 * Master File Table (MFT) entry header functions
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

#if !defined( _LIBFSNTFS_MFT_ENTRY_HEADER_H )
#define _LIBFSNTFS_MFT_ENTRY_HEADER_H

#include <common.h>
#include <types.h>

#include "libfsntfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_mft_entry_header libfsntfs_mft_entry_header_t;

struct libfsntfs_mft_entry_header
{
	/* The fix-up values offset
	 */
	uint16_t fixup_values_offset;

	/* The number of fix-up values
	 */
	uint16_t number_of_fixup_values;

	/* The journal sequence number
	 */
	uint64_t journal_sequence_number;

	/* The sequence
	 */
	uint16_t sequence;

	/* The reference count
	 */
	uint16_t reference_count;

	/* The attributes offset
	 */
	uint16_t attributes_offset;

	/* The flags
	 */
	uint16_t flags;

	/* The used entry size 
	 */
	uint16_t used_entry_size;

	/* The total entry size 
	 */
	uint16_t total_entry_size;

	/* The base record file reference
	 */
	uint64_t base_record_file_reference;

	/* The index
	 */
	uint32_t index;

	/* Value to indicate if the MFT entry is bad
	 * The signature is set to "BAAD"
	 */
	uint8_t is_bad;
};

int libfsntfs_mft_entry_header_initialize(
     libfsntfs_mft_entry_header_t **mft_entry_header,
     libcerror_error_t **error );

int libfsntfs_mft_entry_header_free(
     libfsntfs_mft_entry_header_t **mft_entry_header,
     libcerror_error_t **error );

int libfsntfs_mft_entry_header_read_data(
     libfsntfs_mft_entry_header_t *mft_entry_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsntfs_mft_entry_header_get_fixup_values_offset(
     libfsntfs_mft_entry_header_t *mft_entry_header,
     uint16_t *fixup_values_offset,
     libcerror_error_t **error );

int libfsntfs_mft_entry_header_get_number_of_fixup_values(
     libfsntfs_mft_entry_header_t *mft_entry_header,
     uint16_t *number_of_fixup_values,
     libcerror_error_t **error );

int libfsntfs_mft_entry_header_get_journal_sequence_number(
     libfsntfs_mft_entry_header_t *mft_entry_header,
     uint64_t *journal_sequence_number,
     libcerror_error_t **error );

int libfsntfs_mft_entry_header_get_reference_count(
     libfsntfs_mft_entry_header_t *mft_entry_header,
     uint16_t *reference_count,
     libcerror_error_t **error );

int libfsntfs_mft_entry_header_get_attributes_offset(
     libfsntfs_mft_entry_header_t *mft_entry_header,
     uint16_t *attributes_offset,
     libcerror_error_t **error );

int libfsntfs_mft_entry_header_get_used_entry_size(
     libfsntfs_mft_entry_header_t *mft_entry_header,
     uint16_t *used_entry_size,
     libcerror_error_t **error );

int libfsntfs_mft_entry_header_get_total_entry_size(
     libfsntfs_mft_entry_header_t *mft_entry_header,
     uint16_t *total_entry_size,
     libcerror_error_t **error );

int libfsntfs_mft_entry_header_get_base_record_file_reference(
     libfsntfs_mft_entry_header_t *mft_entry_header,
     uint64_t *base_record_file_reference,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_MFT_ENTRY_HEADER_H ) */


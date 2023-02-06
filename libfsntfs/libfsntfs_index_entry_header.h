/*
 * The NTFS index entry header functions
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

#if !defined( _LIBFSNTFS_INDEX_ENTRY_HEADER_H )
#define _LIBFSNTFS_INDEX_ENTRY_HEADER_H

#include <common.h>
#include <types.h>

#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_index_entry_header libfsntfs_index_entry_header_t;

struct libfsntfs_index_entry_header
{
	/* The fix-up values offset
	 */
	uint16_t fixup_values_offset;

	/* The number of fix-up values
	 */
	uint16_t number_of_fixup_values;
};

int libfsntfs_index_entry_header_initialize(
     libfsntfs_index_entry_header_t **index_entry_header,
     libcerror_error_t **error );

int libfsntfs_index_entry_header_free(
     libfsntfs_index_entry_header_t **index_entry_header,
     libcerror_error_t **error );

int libfsntfs_index_entry_header_read_data(
     libfsntfs_index_entry_header_t *index_entry_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsntfs_index_entry_header_get_fixup_values_offset(
     libfsntfs_index_entry_header_t *index_entry_header,
     uint16_t *fixup_values_offset,
     libcerror_error_t **error );

int libfsntfs_index_entry_header_get_number_of_fixup_values(
     libfsntfs_index_entry_header_t *index_entry_header,
     uint16_t *number_of_fixup_values,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_INDEX_ENTRY_HEADER_H ) */


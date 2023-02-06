/*
 * The NTFS index root header functions
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

#if !defined( _LIBFSNTFS_INDEX_ROOT_HEADER_H )
#define _LIBFSNTFS_INDEX_ROOT_HEADER_H

#include <common.h>
#include <types.h>

#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_index_root_header libfsntfs_index_root_header_t;

struct libfsntfs_index_root_header
{
	/* The index entry size
	 */
	uint32_t index_entry_size;

	/* The attribute type
	 */
	uint32_t attribute_type;

	/* The collation type
	 */
	uint32_t collation_type;
};

int libfsntfs_index_root_header_initialize(
     libfsntfs_index_root_header_t **index_root_header,
     libcerror_error_t **error );

int libfsntfs_index_root_header_free(
     libfsntfs_index_root_header_t **index_root_header,
     libcerror_error_t **error );

int libfsntfs_index_root_header_read_data(
     libfsntfs_index_root_header_t *index_root_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsntfs_index_root_header_get_index_entry_size(
     libfsntfs_index_root_header_t *index_root_header,
     uint32_t *index_entry_size,
     libcerror_error_t **error );

int libfsntfs_index_root_header_get_attribute_type(
     libfsntfs_index_root_header_t *index_root_header,
     uint32_t *attribute_type,
     libcerror_error_t **error );

int libfsntfs_index_root_header_get_collation_type(
     libfsntfs_index_root_header_t *index_root_header,
     uint32_t *collation_type,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_INDEX_ROOT_HEADER_H ) */


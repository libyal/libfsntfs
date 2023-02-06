/*
 * Security descriptor index value functions
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

#if !defined( _LIBFSNTFS_SECURITY_DESCRIPTOR_INDEX_VALUE_H )
#define _LIBFSNTFS_SECURITY_DESCRIPTOR_INDEX_VALUE_H

#include <common.h>
#include <types.h>

#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_security_descriptor_index_value libfsntfs_security_descriptor_index_value_t;

struct libfsntfs_security_descriptor_index_value
{
	/* The hash
	 */
	uint32_t hash;

	/* The identifier
	 */
	uint32_t identifier;

	/* The data offset
	 */
	uint64_t data_offset;

	/* The data size
	 */
	uint32_t data_size;
};

int libfsntfs_security_descriptor_index_value_initialize(
     libfsntfs_security_descriptor_index_value_t **security_descriptor_index_value,
     libcerror_error_t **error );

int libfsntfs_security_descriptor_index_value_free(
     libfsntfs_security_descriptor_index_value_t **security_descriptor_index_value,
     libcerror_error_t **error );

int libfsntfs_security_descriptor_index_value_compare(
     libfsntfs_security_descriptor_index_value_t *first_security_descriptor_index_value,
     libfsntfs_security_descriptor_index_value_t *second_security_descriptor_index_value,
     libcerror_error_t **error );

int libfsntfs_security_descriptor_index_value_read_data(
     libfsntfs_security_descriptor_index_value_t *security_descriptor_index_value,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_SECURITY_DESCRIPTOR_INDEX_VALUE_H ) */


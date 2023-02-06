/*
 * Index value functions
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

#if !defined( _LIBFSNTFS_INDEX_VALUE_H )
#define _LIBFSNTFS_INDEX_VALUE_H

#include <common.h>
#include <types.h>

#include "libfsntfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_index_value libfsntfs_index_value_t;

struct libfsntfs_index_value
{
	/* The offset
	 */
	off64_t offset;

	/* The size
	 */
	uint32_t size;

	/* The key data
	 */
	uint8_t *key_data;

	/* The key data size
	 */
	uint16_t key_data_size;

	/* The value data
	 */
	uint8_t *value_data;

	/* The value data size
	 */
	uint32_t value_data_size;

	/* The file reference
	 */
	uint64_t file_reference;

	/* The flags
	 */
	uint32_t flags;

	/* The sub node virtual cluster number (VCN)
	 */
	uint64_t sub_node_vcn;
};

int libfsntfs_index_value_initialize(
     libfsntfs_index_value_t **index_value,
     libcerror_error_t **error );

int libfsntfs_index_value_free(
     libfsntfs_index_value_t **index_value,
     libcerror_error_t **error );

ssize_t libfsntfs_index_value_read(
         libfsntfs_index_value_t *index_value,
         off64_t index_value_vcn_offset,
         int *index_value_entry,
         const uint8_t *data,
         size_t data_size,
         size_t data_offset,
         libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_INDEX_VALUE_H ) */


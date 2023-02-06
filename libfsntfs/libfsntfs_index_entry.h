/*
 * Index entry functions
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

#if !defined( _LIBFSNTFS_INDEX_ENTRY_H )
#define _LIBFSNTFS_INDEX_ENTRY_H

#include <common.h>
#include <types.h>

#include "libfsntfs_index_node.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_index_entry libfsntfs_index_entry_t;

struct libfsntfs_index_entry
{
	/* The index node
	 */
	libfsntfs_index_node_t *node;
};

int libfsntfs_index_entry_initialize(
     libfsntfs_index_entry_t **index_entry,
     libcerror_error_t **error );

int libfsntfs_index_entry_free(
     libfsntfs_index_entry_t **index_entry,
     libcerror_error_t **error );

int libfsntfs_index_entry_read_file_io_handle(
     libfsntfs_index_entry_t *index_entry,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     uint32_t index_entry_size,
     uint32_t index_entry_index,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_INDEX_ENTRY_H ) */


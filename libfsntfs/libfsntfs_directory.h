/*
 * Directory functions
 *
 * Copyright (C) 2010-2018, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBFSNTFS_DIRECTORY_H )
#define _LIBFSNTFS_DIRECTORY_H

#include <common.h>
#include <types.h>

#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_mft_entry.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_directory libfsntfs_directory_t;

struct libfsntfs_directory
{
	/* The directory entries tree
	 */
	libcdata_btree_t *directory_entries_tree;
};

int libfsntfs_directory_initialize(
     libfsntfs_directory_t **directory,
     libcerror_error_t **error );

int libfsntfs_directory_free(
     libfsntfs_directory_t **directory,
     libcerror_error_t **error );

int libfsntfs_directory_read_file_io_handle(
     libfsntfs_directory_t *directory,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsntfs_mft_entry_t *mft_entry,
     uint8_t flags,
     libcerror_error_t **error );

int libfsntfs_directory_get_number_of_entries(
     libfsntfs_directory_t *directory,
     int *number_of_entries,
     libcerror_error_t **error );

int libfsntfs_directory_get_entry_by_index(
     libfsntfs_directory_t *directory,
     int entry_index,
     libfsntfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfsntfs_directory_get_entry_by_utf8_name(
     libfsntfs_directory_t *directory,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsntfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfsntfs_directory_get_entry_by_utf16_name(
     libfsntfs_directory_t *directory,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsntfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_DIRECTORY_H ) */


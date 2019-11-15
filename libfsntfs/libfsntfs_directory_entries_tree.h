/*
 * Directory entries tree functions
 *
 * Copyright (C) 2010-2019, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFSNTFS_DIRECTORY_ENTRIES_TREE_H )
#define _LIBFSNTFS_DIRECTORY_ENTRIES_TREE_H

#include <common.h>
#include <types.h>

#include "libfsntfs_directory_entry.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_mft_entry.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libfsntfs_directory_entries_tree_read_from_mft_entry(
     libcdata_btree_t *directory_entries_tree,
     libfsntfs_mft_entry_t *mft_entry,
     libbfio_handle_t *file_io_handle,
     uint8_t flags,
     libcerror_error_t **error );

int libfsntfs_directory_entries_tree_get_directory_entry_by_utf8_name(
     libcdata_btree_t *directory_entries_tree,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsntfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfsntfs_directory_entries_tree_get_directory_entry_by_utf16_name(
     libcdata_btree_t *directory_entries_tree,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsntfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_DIRECTORY_ENTRIES_TREE_H ) */


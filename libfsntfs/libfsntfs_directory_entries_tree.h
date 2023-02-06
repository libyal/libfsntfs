/*
 * Directory entries tree functions
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

#if !defined( _LIBFSNTFS_DIRECTORY_ENTRIES_TREE_H )
#define _LIBFSNTFS_DIRECTORY_ENTRIES_TREE_H

#include <common.h>
#include <types.h>

#include "libfsntfs_directory_entry.h"
#include "libfsntfs_index.h"
#include "libfsntfs_index_node.h"
#include "libfsntfs_index_value.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_mft_entry.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_directory_entries_tree libfsntfs_directory_entries_tree_t;

struct libfsntfs_directory_entries_tree
{
	/* The $I30 index
	 */
	libfsntfs_index_t *i30_index;

	/* The short names B-tree
	 */
	libcdata_btree_t *short_names_tree;

	/* The entries list
	 */
	libfdata_list_t *entries_list;

	/* The entries cache
	 */
	libfcache_cache_t *entries_cache;

	/* Value to indicate case folding should be used
	 */
	uint8_t use_case_folding;
};

int libfsntfs_directory_entries_tree_initialize(
     libfsntfs_directory_entries_tree_t **directory_entries_tree,
     libcerror_error_t **error );

int libfsntfs_directory_entries_tree_free(
     libfsntfs_directory_entries_tree_t **directory_entries_tree,
     libcerror_error_t **error );

int libfsntfs_directory_entries_tree_insert_index_value(
     libfsntfs_directory_entries_tree_t *directory_entries_tree,
     int index_value_entry,
     libfsntfs_index_value_t *index_value,
     uint32_t index_value_flags,
     libcerror_error_t **error );

int libfsntfs_directory_entries_tree_read_from_index_node(
     libfsntfs_directory_entries_tree_t *directory_entries_tree,
     libbfio_handle_t *file_io_handle,
     libfsntfs_index_node_t *index_node,
     int recursion_depth,
     libcerror_error_t **error );

int libfsntfs_directory_entries_tree_read_from_i30_index(
     libfsntfs_directory_entries_tree_t *directory_entries_tree,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsntfs_mft_entry_t *mft_entry,
     uint8_t flags,
     libcerror_error_t **error );

int libfsntfs_directory_entries_tree_get_number_of_entries(
     libfsntfs_directory_entries_tree_t *directory_entries_tree,
     int *number_of_entries,
     libcerror_error_t **error );

int libfsntfs_directory_entries_tree_get_entry_by_index(
     libfsntfs_directory_entries_tree_t *directory_entries_tree,
     libbfio_handle_t *file_io_handle,
     int entry_index,
     libfsntfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf8_name(
     libfsntfs_directory_entries_tree_t *directory_entries_tree,
     libbfio_handle_t *file_io_handle,
     libfsntfs_index_node_t *index_node,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsntfs_directory_entry_t **directory_entry,
     int recursion_depth,
     libcerror_error_t **error );

int libfsntfs_directory_entries_tree_get_entry_by_utf8_name(
     libfsntfs_directory_entries_tree_t *directory_entries_tree,
     libbfio_handle_t *file_io_handle,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsntfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfsntfs_directory_entries_tree_get_entry_from_index_node_by_utf16_name(
     libfsntfs_directory_entries_tree_t *directory_entries_tree,
     libbfio_handle_t *file_io_handle,
     libfsntfs_index_node_t *index_node,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsntfs_directory_entry_t **directory_entry,
     int recursion_depth,
     libcerror_error_t **error );

int libfsntfs_directory_entries_tree_get_entry_by_utf16_name(
     libfsntfs_directory_entries_tree_t *directory_entries_tree,
     libbfio_handle_t *file_io_handle,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsntfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfsntfs_directory_entries_tree_read_element_data(
     libfsntfs_directory_entries_tree_t *directory_entries_tree,
     libbfio_handle_t *file_io_handle,
     libfdata_list_element_t *element,
     libfdata_cache_t *cache,
     int element_file_index,
     off64_t index_value_offset,
     size64_t element_size,
     uint32_t index_value_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_DIRECTORY_ENTRIES_TREE_H ) */


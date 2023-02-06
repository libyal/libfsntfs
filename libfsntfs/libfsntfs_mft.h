/*
 * MFT functions
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

#if !defined( _LIBFSNTFS_MFT_H )
#define _LIBFSNTFS_MFT_H

#include <common.h>
#include <types.h>

#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_mft_entry.h"
#include "libfsntfs_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_mft libfsntfs_mft_t;

struct libfsntfs_mft
{
	/* The number of MFT entries
	 */
	uint64_t number_of_mft_entries;

	/* The MFT entry vector
	 */
	libfdata_vector_t *mft_entry_vector;

	/* The MFT entry cache
	 */
	libfcache_cache_t *mft_entry_cache;

	/* The single MFT entry cache
	 */
	libfcache_cache_t *single_mft_entry_cache;

	/* The attribute list B-tree
	 */
	libcdata_btree_t *attribute_list_tree;

	/* The IO handle
	 */
	libfsntfs_io_handle_t *io_handle;

	/* The flags
	 */
	uint8_t flags;
};

int libfsntfs_mft_initialize(
     libfsntfs_mft_t **mft,
     libfsntfs_io_handle_t *io_handle,
     size64_t mft_entry_size,
     uint8_t flags,
     libcerror_error_t **error );

int libfsntfs_mft_free(
     libfsntfs_mft_t **mft,
     libcerror_error_t **error );

int libfsntfs_mft_read_list_data_mft_entries(
     libfsntfs_mft_t *mft,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libfsntfs_mft_get_number_of_entries(
     libfsntfs_mft_t *mft,
     uint64_t *number_of_entries,
     libcerror_error_t **error );

int libfsntfs_mft_get_mft_entry_by_index(
     libfsntfs_mft_t *mft,
     libbfio_handle_t *file_io_handle,
     uint64_t mft_entry_index,
     libfsntfs_mft_entry_t **mft_entry,
     libcerror_error_t **error );

int libfsntfs_mft_get_mft_entry_by_index_no_cache(
     libfsntfs_mft_t *mft,
     libbfio_handle_t *file_io_handle,
     uint64_t mft_entry_index,
     libfsntfs_mft_entry_t **mft_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_MFT_H ) */


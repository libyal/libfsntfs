/*
 * File system functions
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

#if !defined( _LIBFSNTFS_FILE_SYSTEM_H )
#define _LIBFSNTFS_FILE_SYSTEM_H

#include <common.h>
#include <types.h>

#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcthreads.h"
#include "libfsntfs_mft.h"
#include "libfsntfs_path_hint.h"
#include "libfsntfs_security_descriptor_index.h"
#include "libfsntfs_security_descriptor_values.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_file_system libfsntfs_file_system_t;

struct libfsntfs_file_system
{
	/* The MFT
	 */
	libfsntfs_mft_t *mft;

	/* The security descriptor index
	 */
	libfsntfs_security_descriptor_index_t *security_descriptor_index;

	/* The list data MFT entry B-tree
	 */
	libcdata_btree_t *list_data_mft_entry_tree;

	/* The path hints B-tree
	 */
	libcdata_btree_t *path_hints_tree;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libfsntfs_file_system_initialize(
     libfsntfs_file_system_t **file_system,
     libcerror_error_t **error );

int libfsntfs_file_system_free(
     libfsntfs_file_system_t **file_system,
     libcerror_error_t **error );

int libfsntfs_file_system_read_mft(
     libfsntfs_file_system_t *file_system,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t mft_offset,
     uint8_t flags,
     libcerror_error_t **error );

int libfsntfs_file_system_read_bitmap(
     libfsntfs_file_system_t *file_system,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libfsntfs_file_system_read_security_descriptors(
     libfsntfs_file_system_t *file_system,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libfsntfs_file_system_get_number_of_mft_entries(
     libfsntfs_file_system_t *file_system,
     uint64_t *number_of_mft_entries,
     libcerror_error_t **error );

int libfsntfs_file_system_get_mft_entry_by_index(
     libfsntfs_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     uint64_t mft_entry_index,
     libfsntfs_mft_entry_t **mft_entry,
     libcerror_error_t **error );

int libfsntfs_file_system_get_mft_entry_by_index_no_cache(
     libfsntfs_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     uint64_t mft_entry_index,
     libfsntfs_mft_entry_t **mft_entry,
     libcerror_error_t **error );

int libfsntfs_file_system_get_security_descriptor_values_by_identifier(
     libfsntfs_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     uint32_t security_descriptor_identifier,
     libfsntfs_security_descriptor_values_t **security_descriptor_values,
     libcerror_error_t **error );

int libfsntfs_file_system_get_path_hint(
     libfsntfs_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     uint64_t file_reference,
     libfsntfs_path_hint_t **path_hint,
     int recursion_depth,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_FILE_SYSTEM_H ) */


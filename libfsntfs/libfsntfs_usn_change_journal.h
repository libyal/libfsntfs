/*
 * USN change journal functions
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

#if !defined( _LIBFSNTFS_INTERNAL_USN_CHANGE_JOURNAL_H )
#define _LIBFSNTFS_INTERNAL_USN_CHANGE_JOURNAL_H

#include <common.h>
#include <types.h>

#include "libfsntfs_directory_entry.h"
#include "libfsntfs_extern.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcthreads.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_mft_attribute.h"
#include "libfsntfs_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_internal_usn_change_journal libfsntfs_internal_usn_change_journal_t;

struct libfsntfs_internal_usn_change_journal
{
	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The directory entry
	 */
	libfsntfs_directory_entry_t *directory_entry;

	/* The $J data stream
	 */
	libfdata_stream_t *data_stream;

	/* The data offset
	 */
	off64_t data_offset;

	/* The data size
	 */
	size64_t data_size;

	/* The number of extents
	 */
	int number_of_extents;

	/* The extent index
	 */
	int extent_index;

	/* The extent start offset
	 */
	off64_t extent_start_offset;

	/* The extent offset
	 */
	off64_t extent_offset;

	/* The extent size
	 */
	size64_t extent_size;

	/* The extent flags
	 */
	uint32_t extent_flags;

	/* The journal block data
	 */
	uint8_t *journal_block_data;

	/* The journal block (data) offset
	 */
	size_t journal_block_offset;

	/* The journal block size
	 */
	size_t journal_block_size;
};

int libfsntfs_usn_change_journal_initialize(
     libfsntfs_usn_change_journal_t **usn_change_journal,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsntfs_directory_entry_t *directory_entry,
     libfsntfs_mft_attribute_t *data_attribute,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_usn_change_journal_free(
     libfsntfs_usn_change_journal_t **usn_change_journal,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_usn_change_journal_get_offset(
     libfsntfs_usn_change_journal_t *usn_change_journal,
     off64_t *offset,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
ssize_t libfsntfs_usn_change_journal_read_usn_record(
         libfsntfs_usn_change_journal_t *usn_change_journal,
         uint8_t *usn_record_data,
         size_t usn_record_data_size,
         libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_INTERNAL_USN_CHANGE_JOURNAL_H ) */


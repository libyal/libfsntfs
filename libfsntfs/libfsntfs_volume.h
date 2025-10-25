/*
 * Volume functions
 *
 * Copyright (C) 2010-2024, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFSNTFS_VOLUME_H )
#define _LIBFSNTFS_VOLUME_H

#include <common.h>
#include <types.h>

#include "libfsntfs_directory_entry.h"
#include "libfsntfs_extern.h"
#include "libfsntfs_file_system.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcthreads.h"
#include "libfsntfs_mft.h"
#include "libfsntfs_mft_entry.h"
#include "libfsntfs_types.h"
#include "libfsntfs_volume_header.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_internal_volume libfsntfs_internal_volume_t;

struct libfsntfs_internal_volume
{
	/* The IO handle
	 */
	libfsntfs_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* Value to indicate if the file IO handle was created inside the library
	 */
	uint8_t file_io_handle_created_in_library;

	/* Value to indicate if the file IO handle was opened inside the library
	 */
	uint8_t file_io_handle_opened_in_library;

	/* The volume serial number
	 */
	uint64_t volume_serial_number;

	/* The volume header
	 */
	libfsntfs_volume_header_t *volume_header;

	/* The MFT
	 */
	libfsntfs_mft_t *mft;

	/* The volume MFT entry
	 */
	libfsntfs_mft_entry_t *volume_mft_entry;

	/* The volume information ($VOLUME_INFORMATION) attribute
	 */
	libfsntfs_attribute_t *volume_information_attribute;

	/* The volume name ($VOLUME_NAME) attribute
	 */
	libfsntfs_attribute_t *volume_name_attribute;

	/* The file system
	 */
	libfsntfs_file_system_t *file_system;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

LIBFSNTFS_EXTERN \
int libfsntfs_volume_initialize(
     libfsntfs_volume_t **volume,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_volume_free(
     libfsntfs_volume_t **volume,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_volume_signal_abort(
     libfsntfs_volume_t *volume,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_volume_open(
     libfsntfs_volume_t *volume,
     const char *filename,
     int access_flags,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBFSNTFS_EXTERN \
int libfsntfs_volume_open_wide(
     libfsntfs_volume_t *volume,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBFSNTFS_EXTERN \
int libfsntfs_volume_open_file_io_handle(
     libfsntfs_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_volume_close(
     libfsntfs_volume_t *volume,
     libcerror_error_t **error );

int libfsntfs_internal_volume_open_read(
     libfsntfs_internal_volume_t *internal_volume,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libfsntfs_internal_volume_read_bitmap(
     libfsntfs_internal_volume_t *internal_volume,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_volume_has_bitlocker_drive_encryption(
     libfsntfs_volume_t *volume,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_volume_has_volume_shadow_snapshots(
     libfsntfs_volume_t *volume,
     libcerror_error_t **error );

int libfsntfs_internal_volume_get_volume_information_attribute(
     libfsntfs_internal_volume_t *internal_volume,
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error );

int libfsntfs_internal_volume_get_volume_name_attribute(
     libfsntfs_internal_volume_t *internal_volume,
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_volume_get_bytes_per_sector(
     libfsntfs_volume_t *volume,
     uint16_t *bytes_per_sector,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_volume_get_cluster_block_size(
     libfsntfs_volume_t *volume,
     size32_t *cluster_block_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_volume_get_size(
	 libfsntfs_volume_t *volume,
	 size64_t *volume_size,
	 libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_volume_get_mft_entry_size(
     libfsntfs_volume_t *volume,
     size32_t *mft_entry_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_volume_get_index_entry_size(
     libfsntfs_volume_t *volume,
     size32_t *index_entry_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_volume_get_utf8_name_size(
     libfsntfs_volume_t *volume,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_volume_get_utf8_name(
     libfsntfs_volume_t *volume,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_volume_get_utf16_name_size(
     libfsntfs_volume_t *volume,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_volume_get_utf16_name(
     libfsntfs_volume_t *volume,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_volume_get_version(
     libfsntfs_volume_t *volume,
     uint8_t *major_version,
     uint8_t *minor_version,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_volume_get_flags(
     libfsntfs_volume_t *volume,
     uint16_t *flags,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_volume_get_serial_number(
     libfsntfs_volume_t *volume,
     uint64_t *serial_number,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_volume_get_root_directory(
     libfsntfs_volume_t *volume,
     libfsntfs_file_entry_t **file_entry,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_volume_get_number_of_file_entries(
     libfsntfs_volume_t *volume,
     uint64_t *number_of_file_entries,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_volume_get_file_entry_by_index(
     libfsntfs_volume_t *volume,
     uint64_t mft_entry_index,
     libfsntfs_file_entry_t **file_entry,
     libcerror_error_t **error );

int libfsntfs_internal_volume_get_mft_and_directory_entry_by_utf8_path(
     libfsntfs_internal_volume_t *internal_volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsntfs_mft_entry_t **mft_entry,
     libfsntfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_volume_get_file_entry_by_utf8_path(
     libfsntfs_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsntfs_file_entry_t **file_entry,
     libcerror_error_t **error );

int libfsntfs_internal_volume_get_mft_and_directory_entry_by_utf16_path(
     libfsntfs_internal_volume_t *internal_volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsntfs_mft_entry_t **mft_entry,
     libfsntfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_volume_get_file_entry_by_utf16_path(
     libfsntfs_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsntfs_file_entry_t **file_entry,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_volume_get_usn_change_journal(
     libfsntfs_volume_t *volume,
     libfsntfs_usn_change_journal_t **usn_change_journal,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_VOLUME_H ) */


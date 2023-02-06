/*
 * $MFT metadata file functions
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

#if !defined( _LIBFSNTFS_MFT_METADATA_FILE_H )
#define _LIBFSNTFS_MFT_METADATA_FILE_H

#include <common.h>
#include <types.h>

#include "libfsntfs_extern.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcthreads.h"
#include "libfsntfs_file_system.h"
#include "libfsntfs_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_internal_mft_metadata_file libfsntfs_internal_mft_metadata_file_t;

struct libfsntfs_internal_mft_metadata_file
{
	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* Value to indicate if the file IO handle was created inside the library
	 */
	uint8_t file_io_handle_created_in_library;

	/* Value to indicate if the file IO handle was opened inside the library
	 */
	uint8_t file_io_handle_opened_in_library;

	/* The IO handle
	 */
	libfsntfs_io_handle_t *io_handle;

	/* The file system
	 */
	libfsntfs_file_system_t *file_system;

	/* The volume MFT entry
	 */
	libfsntfs_mft_entry_t *volume_mft_entry;

	/* The volume information ($VOLUME_INFORMATION) attribute
	 */
	libfsntfs_attribute_t *volume_information_attribute;

	/* The volume name ($VOLUME_NAME) attribute
	 */
	libfsntfs_attribute_t *volume_name_attribute;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

LIBFSNTFS_EXTERN \
int libfsntfs_mft_metadata_file_initialize(
     libfsntfs_mft_metadata_file_t **mft_metadata_file,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_mft_metadata_file_free(
     libfsntfs_mft_metadata_file_t **mft_metadata_file,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_mft_metadata_file_signal_abort(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_mft_metadata_file_open(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     const char *filename,
     int access_flags,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBFSNTFS_EXTERN \
int libfsntfs_mft_metadata_file_open_wide(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBFSNTFS_EXTERN \
int libfsntfs_mft_metadata_file_open_file_io_handle(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_mft_metadata_file_close(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     libcerror_error_t **error );

int libfsntfs_internal_mft_metadata_file_open_read(
     libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libfsntfs_internal_mft_metadata_file_get_volume_information_attribute(
     libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file,
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error );

int libfsntfs_internal_mft_metadata_file_get_volume_name_attribute(
     libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file,
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_mft_metadata_file_get_utf8_volume_name_size(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_mft_metadata_file_get_utf8_volume_name(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_mft_metadata_file_get_utf16_volume_name_size(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_mft_metadata_file_get_utf16_volume_name(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_mft_metadata_file_get_volume_version(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     uint8_t *major_version,
     uint8_t *minor_version,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_mft_metadata_file_get_volume_flags(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     uint16_t *flags,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_mft_metadata_file_get_number_of_file_entries(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     uint64_t *number_of_file_entries,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_mft_metadata_file_get_file_entry_by_index(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     uint64_t mft_entry_index,
     libfsntfs_file_entry_t **file_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_MFT_METADATA_FILE_H ) */


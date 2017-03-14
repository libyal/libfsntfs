/*
 * $MFT metadata file functions
 *
 * Copyright (C) 2010-2017, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFSNTFS_INTERNAL_MFT_METADATA_FILE_H )
#define _LIBFSNTFS_INTERNAL_MFT_METADATA_FILE_H

#include <common.h>
#include <types.h>

#include "libfsntfs_extern.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_mft.h"
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

	/* The MFT
	 */
	libfsntfs_mft_t *mft;
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

int libfsntfs_mft_metadata_file_open_read(
     libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_mft_metadata_file_get_utf8_volume_name_size(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     size_t *utf8_volume_name_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_mft_metadata_file_get_utf8_volume_name(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     uint8_t *utf8_volume_name,
     size_t utf8_volume_name_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_mft_metadata_file_get_utf16_volume_name_size(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     size_t *utf16_volume_name_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_mft_metadata_file_get_utf16_volume_name(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     uint16_t *utf16_volume_name,
     size_t utf16_volume_name_size,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_mft_metadata_file_get_volume_version(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     uint8_t *major_version,
     uint8_t *minor_version,
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

#endif /* !defined( _LIBFSNTFS_INTERNAL_MFT_METADATA_FILE_H ) */


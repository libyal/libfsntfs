/*
 * $MFT file functions
 *
 * Copyright (C) 2010-2014, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFSNTFS_INTERNAL_MFT_FILE_H )
#define _LIBFSNTFS_INTERNAL_MFT_FILE_H

#include <common.h>
#include <types.h>

#include "libfsntfs_extern.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_mft_entry.h"
#include "libfsntfs_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_internal_mft_file libfsntfs_internal_mft_file_t;

struct libfsntfs_internal_mft_file
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

	/* The MFT entry vector
	 */
	libfdata_vector_t *mft_entry_vector;

	/* The MFT entry cache
	 */
	libfcache_cache_t *mft_entry_cache;
};

LIBFSNTFS_EXTERN \
int libfsntfs_mft_file_initialize(
     libfsntfs_mft_file_t **mft_file,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_mft_file_free(
     libfsntfs_mft_file_t **mft_file,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_mft_file_signal_abort(
     libfsntfs_mft_file_t *mft_file,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_mft_file_open(
     libfsntfs_mft_file_t *mft_file,
     const char *filename,
     int access_flags,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBFSNTFS_EXTERN \
int libfsntfs_mft_file_open_wide(
     libfsntfs_mft_file_t *mft_file,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error );
#endif

LIBFSNTFS_EXTERN \
int libfsntfs_mft_file_open_file_io_handle(
     libfsntfs_mft_file_t *mft_file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error );

LIBFSNTFS_EXTERN \
int libfsntfs_mft_file_close(
     libfsntfs_mft_file_t *mft_file,
     libcerror_error_t **error );

int libfsntfs_mft_file_open_read(
     libfsntfs_internal_mft_file_t *internal_mft_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif


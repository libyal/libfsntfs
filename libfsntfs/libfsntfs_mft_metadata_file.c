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

#include <common.h>
#include <memory.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include "libfsntfs_attribute.h"
#include "libfsntfs_debug.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_file_entry.h"
#include "libfsntfs_file_name_values.h"
#include "libfsntfs_file_system.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libuna.h"
#include "libfsntfs_mft_entry.h"
#include "libfsntfs_mft_metadata_file.h"
#include "libfsntfs_types.h"
#include "libfsntfs_volume_information_attribute.h"
#include "libfsntfs_volume_name_attribute.h"

/* Creates a MFT metadata file
 * Make sure the value mft_metadata_file is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_metadata_file_initialize(
     libfsntfs_mft_metadata_file_t **mft_metadata_file,
     libcerror_error_t **error )
{
	libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file = NULL;
	static char *function                                              = "libfsntfs_mft_metadata_file_initialize";

	if( mft_metadata_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT metadata file.",
		 function );

		return( -1 );
	}
	if( *mft_metadata_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid MFT metadata file value already set.",
		 function );

		return( -1 );
	}
	internal_mft_metadata_file = memory_allocate_structure(
	                              libfsntfs_internal_mft_metadata_file_t );

	if( internal_mft_metadata_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create MFT metadata file.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_mft_metadata_file,
	     0,
	     sizeof( libfsntfs_internal_mft_metadata_file_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear MFT metadata file.",
		 function );

		memory_free(
		 internal_mft_metadata_file );

		return( -1 );
	}
	if( libfsntfs_io_handle_initialize(
	     &( internal_mft_metadata_file->io_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_mft_metadata_file->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	*mft_metadata_file = (libfsntfs_mft_metadata_file_t *) internal_mft_metadata_file;

	return( 1 );

on_error:
	if( internal_mft_metadata_file != NULL )
	{
		if( internal_mft_metadata_file->io_handle != NULL )
		{
			libfsntfs_io_handle_free(
			 &( internal_mft_metadata_file->io_handle ),
			 NULL );
		}
		memory_free(
		 internal_mft_metadata_file );
	}
	return( -1 );
}

/* Frees a MFT metadata file
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_metadata_file_free(
     libfsntfs_mft_metadata_file_t **mft_metadata_file,
     libcerror_error_t **error )
{
	libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file = NULL;
	static char *function                                              = "libfsntfs_mft_metadata_file_free";
	int result                                                         = 1;

	if( mft_metadata_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT metadata file.",
		 function );

		return( -1 );
	}
	if( *mft_metadata_file != NULL )
	{
		internal_mft_metadata_file = (libfsntfs_internal_mft_metadata_file_t *) *mft_metadata_file;

		if( internal_mft_metadata_file->file_io_handle != NULL )
		{
			if( libfsntfs_mft_metadata_file_close(
			     *mft_metadata_file,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close MFT metadata file.",
				 function );

				result = -1;
			}
		}
		*mft_metadata_file = NULL;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_mft_metadata_file->read_write_lock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read/write lock.",
			 function );

			result = -1;
		}
#endif
		if( libfsntfs_io_handle_free(
		     &( internal_mft_metadata_file->io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free IO handle.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_mft_metadata_file );
	}
	return( result );
}

/* Signals the mft_metadata_file to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_metadata_file_signal_abort(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     libcerror_error_t **error )
{
	libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file = NULL;
	static char *function                                              = "libfsntfs_mft_metadata_file_signal_abort";

	if( mft_metadata_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT metadata file.",
		 function );

		return( -1 );
	}
	internal_mft_metadata_file = (libfsntfs_internal_mft_metadata_file_t *) mft_metadata_file;

	if( internal_mft_metadata_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid MFT metadata file - missing IO handle.",
		 function );

		return( -1 );
	}
	internal_mft_metadata_file->io_handle->abort = 1;

	return( 1 );
}

/* Opens a MFT metadata file
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_metadata_file_open(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     const char *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle                                   = NULL;
	libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file = NULL;
	static char *function                                              = "libfsntfs_mft_metadata_file_open";

	if( mft_metadata_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT metadata file.",
		 function );

		return( -1 );
	}
	internal_mft_metadata_file = (libfsntfs_internal_mft_metadata_file_t *) mft_metadata_file;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBFSNTFS_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBFSNTFS_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBFSNTFS_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set track offsets read in file IO handle.",
		 function );

		goto on_error;
	}
#endif
	if( libbfio_file_set_name(
	     file_io_handle,
	     filename,
	     narrow_string_length(
	      filename ) + 1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set filename in file IO handle.",
		 function );

		goto on_error;
	}
	if( libfsntfs_mft_metadata_file_open_file_io_handle(
	     mft_metadata_file,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open MFT metadata file: %s.",
		 function,
		 filename );

		goto on_error;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_mft_metadata_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		goto on_error;
	}
#endif
	internal_mft_metadata_file->file_io_handle_created_in_library = 1;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_mft_metadata_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		internal_mft_metadata_file->file_io_handle_created_in_library = 0;

		goto on_error;
	}
#endif
	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Opens a MFT metadata file
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_metadata_file_open_wide(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle                                   = NULL;
	libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file = NULL;
	static char *function                                              = "libfsntfs_mft_metadata_file_open_wide";

	if( mft_metadata_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT metadata file.",
		 function );

		return( -1 );
	}
	internal_mft_metadata_file = (libfsntfs_internal_mft_metadata_file_t *) mft_metadata_file;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBFSNTFS_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBFSNTFS_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBFSNTFS_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set track offsets read in file IO handle.",
		 function );

		goto on_error;
	}
#endif
	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     filename,
	     wide_string_length(
	      filename ) + 1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set filename in file IO handle.",
		 function );

		goto on_error;
	}
	if( libfsntfs_mft_metadata_file_open_file_io_handle(
	     mft_metadata_file,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open MFT metadata file: %ls.",
		 function,
		 filename );

		goto on_error;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_mft_metadata_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		goto on_error;
	}
#endif
	internal_mft_metadata_file->file_io_handle_created_in_library = 1;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_mft_metadata_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		internal_mft_metadata_file->file_io_handle_created_in_library = 0;

		goto on_error;
	}
#endif
	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

#endif /* #if defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Opens a MFT metadata file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_metadata_file_open_file_io_handle(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error )
{
	libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file = NULL;
	static char *function                                              = "libfsntfs_volume_open_file_io_handle";
	int bfio_access_flags                                              = 0;
	int file_io_handle_is_open                                         = 0;
	uint8_t file_io_handle_opened_in_library                           = 0;

	if( mft_metadata_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT metadata file.",
		 function );

		return( -1 );
	}
	internal_mft_metadata_file = (libfsntfs_internal_mft_metadata_file_t *) mft_metadata_file;

	if( internal_mft_metadata_file->file_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid MFT metadata file - file IO handle already set.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBFSNTFS_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBFSNTFS_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBFSNTFS_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBFSNTFS_ACCESS_FLAG_READ ) != 0 )
	{
		bfio_access_flags = LIBBFIO_ACCESS_FLAG_READ;
	}
	file_io_handle_is_open = libbfio_handle_is_open(
	                          file_io_handle,
	                          error );

	if( file_io_handle_is_open == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open volume.",
		 function );

		goto on_error;
	}
	else if( file_io_handle_is_open == 0 )
	{
		if( libbfio_handle_open(
		     file_io_handle,
		     bfio_access_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open file IO handle.",
			 function );

			goto on_error;
		}
		file_io_handle_opened_in_library = 1;
	}
	if( libfsntfs_internal_mft_metadata_file_open_read(
	     internal_mft_metadata_file,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_mft_metadata_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		goto on_error;
	}
#endif
	internal_mft_metadata_file->file_io_handle                   = file_io_handle;
	internal_mft_metadata_file->file_io_handle_opened_in_library = file_io_handle_opened_in_library;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_mft_metadata_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		internal_mft_metadata_file->file_io_handle                   = NULL;
		internal_mft_metadata_file->file_io_handle_opened_in_library = 0;

		goto on_error;
	}
#endif
	return( 1 );

on_error:
	if( file_io_handle_opened_in_library != 0 )
	{
		libbfio_handle_close(
		 file_io_handle,
		 error );
	}
	return( -1 );
}

/* Closes a MFT metadata file
 * Returns 0 if successful or -1 on error
 */
int libfsntfs_mft_metadata_file_close(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     libcerror_error_t **error )
{
	libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file = NULL;
	static char *function                                              = "libfsntfs_mft_metadata_file_close";
	int result                                                         = 0;

	if( mft_metadata_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT metadata file.",
		 function );

		return( -1 );
	}
	internal_mft_metadata_file = (libfsntfs_internal_mft_metadata_file_t *) mft_metadata_file;

	if( internal_mft_metadata_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid MFT metadata file - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_mft_metadata_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( internal_mft_metadata_file->file_io_handle_created_in_library != 0 )
		{
			if( libfsntfs_debug_print_read_offsets(
			     internal_mft_metadata_file->file_io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print the read offsets.",
				 function );

				result = -1;
			}
		}
	}
#endif
	if( internal_mft_metadata_file->file_io_handle_opened_in_library != 0 )
	{
		if( libbfio_handle_close(
		     internal_mft_metadata_file->file_io_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close file IO handle.",
			 function );

			result = -1;
		}
		internal_mft_metadata_file->file_io_handle_opened_in_library = 0;
	}
	if( internal_mft_metadata_file->file_io_handle_created_in_library != 0 )
	{
		if( libbfio_handle_free(
		     &( internal_mft_metadata_file->file_io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file IO handle.",
			 function );

			result = -1;
		}
		internal_mft_metadata_file->file_io_handle_created_in_library = 0;
	}
	internal_mft_metadata_file->file_io_handle = NULL;

	if( libfsntfs_io_handle_clear(
	     internal_mft_metadata_file->io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		result = -1;
	}
	if( internal_mft_metadata_file->file_system != NULL )
	{
		if( libfsntfs_file_system_free(
		     &( internal_mft_metadata_file->file_system ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file system.",
			 function );

			result = -1;
		}
	}
	if( internal_mft_metadata_file->volume_mft_entry != NULL )
	{
		if( libfsntfs_mft_entry_free(
		     &( internal_mft_metadata_file->volume_mft_entry ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free volume MFT entry.",
			 function );

			result = -1;
		}
	}
	if( internal_mft_metadata_file->volume_information_attribute != NULL )
	{
		if( libfsntfs_internal_attribute_free(
		     (libfsntfs_internal_attribute_t **) &( internal_mft_metadata_file->volume_information_attribute ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free volume information attribute.",
			 function );

			result = -1;
		}
	}
	if( internal_mft_metadata_file->volume_name_attribute != NULL )
	{
		if( libfsntfs_internal_attribute_free(
		     (libfsntfs_internal_attribute_t **) &( internal_mft_metadata_file->volume_name_attribute ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free volume name attribute.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_mft_metadata_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Opens a MFT metadata file for reading
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_internal_mft_metadata_file_open_read(
     libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_internal_mft_metadata_file_open_read";

	if( internal_mft_metadata_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT metadata file.",
		 function );

		return( -1 );
	}
	if( internal_mft_metadata_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid MFT metadata file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_mft_metadata_file->file_system != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid MFT metadata file - file system value already set.",
		 function );

		return( -1 );
	}
/* TODO allow to set the values */
/* TODO scan for signature to determine MFT entry size */
	internal_mft_metadata_file->io_handle->bytes_per_sector   = 512;
/* TODO if not set FILE signature try scan? */
	internal_mft_metadata_file->io_handle->mft_entry_size     = 1024;
/* TODO if not set INDX signature try scan? */
	internal_mft_metadata_file->io_handle->index_entry_size   = 4096;
	internal_mft_metadata_file->io_handle->cluster_block_size = 4096;

	if( libfsntfs_file_system_initialize(
	     &( internal_mft_metadata_file->file_system ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file system.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading MFT entry: 0:\n" );
	}
#endif
	if( libfsntfs_file_system_read_mft(
	     internal_mft_metadata_file->file_system,
	     internal_mft_metadata_file->io_handle,
	     file_io_handle,
	     0,
	     LIBFSNTFS_FILE_ENTRY_FLAGS_MFT_ONLY,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read MFT (MFT entry: 0).",
		 function );

		goto on_error;
	}
	if( libfsntfs_mft_read_list_data_mft_entries(
	     internal_mft_metadata_file->file_system->mft,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read list data MFT entries.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( internal_mft_metadata_file->file_system != NULL )
	{
		libfsntfs_file_system_free(
		 &( internal_mft_metadata_file->file_system ),
		 NULL );
	}
	return( -1 );
}

/* Retrieves the $VOLUME_INFORMATION attribute from MFT entry 3
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_internal_mft_metadata_file_get_volume_information_attribute(
     libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file,
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error )
{
	libfsntfs_mft_attribute_t *mft_attribute = NULL;
	static char *function                    = "libfsntfs_internal_mft_metadata_file_get_volume_information_attribute";
	int result                               = 0;

	if( internal_mft_metadata_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT metadata file.",
		 function );

		return( -1 );
	}
	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	if( internal_mft_metadata_file->volume_mft_entry == NULL )
	{
		if( libfsntfs_file_system_get_mft_entry_by_index_no_cache(
		     internal_mft_metadata_file->file_system,
		     internal_mft_metadata_file->file_io_handle,
		     LIBFSNTFS_MFT_ENTRY_INDEX_VOLUME,
		     &( internal_mft_metadata_file->volume_mft_entry ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve MFT entry: %d.",
			 function,
			 LIBFSNTFS_MFT_ENTRY_INDEX_VOLUME );

			return( -1 );
		}
	}
	if( internal_mft_metadata_file->volume_information_attribute == NULL )
	{
		result = libfsntfs_mft_entry_get_volume_information_attribute(
		          internal_mft_metadata_file->volume_mft_entry,
		          &mft_attribute,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve $VOLUME_INFORMATION attribute.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		if( libfsntfs_attribute_initialize(
		     &( internal_mft_metadata_file->volume_information_attribute ),
		     mft_attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create $VOLUME_INFORMATION attribute.",
			 function );

			return( -1 );
		}
		if( libfsntfs_internal_attribute_read_value(
		     (libfsntfs_internal_attribute_t *) internal_mft_metadata_file->volume_information_attribute,
		     internal_mft_metadata_file->io_handle,
		     internal_mft_metadata_file->file_io_handle,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read value of $VOLUME_INFORMATION attribute.",
			 function );

			return( -1 );
		}
	}
	*attribute = internal_mft_metadata_file->volume_information_attribute;

	return( 1 );
}

/* Retrieves the $VOLUME_NAME attribute from MFT entry 3
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_internal_mft_metadata_file_get_volume_name_attribute(
     libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file,
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error )
{
	libfsntfs_mft_attribute_t *mft_attribute = NULL;
	static char *function                    = "libfsntfs_internal_mft_metadata_file_get_volume_name_attribute";
	int result                               = 0;

	if( internal_mft_metadata_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT metadata file.",
		 function );

		return( -1 );
	}
	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	if( internal_mft_metadata_file->volume_mft_entry == NULL )
	{
		if( libfsntfs_file_system_get_mft_entry_by_index_no_cache(
		     internal_mft_metadata_file->file_system,
		     internal_mft_metadata_file->file_io_handle,
		     LIBFSNTFS_MFT_ENTRY_INDEX_VOLUME,
		     &( internal_mft_metadata_file->volume_mft_entry ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve MFT entry: %d.",
			 function,
			 LIBFSNTFS_MFT_ENTRY_INDEX_VOLUME );

			return( -1 );
		}
	}
	if( internal_mft_metadata_file->volume_name_attribute == NULL )
	{
		result = libfsntfs_mft_entry_get_volume_name_attribute(
		          internal_mft_metadata_file->volume_mft_entry,
		          &mft_attribute,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve $VOLUME_NAME attribute.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		if( libfsntfs_attribute_initialize(
		     &( internal_mft_metadata_file->volume_name_attribute ),
		     mft_attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create $VOLUME_NAME attribute.",
			 function );

			return( -1 );
		}
		if( libfsntfs_internal_attribute_read_value(
		     (libfsntfs_internal_attribute_t *) internal_mft_metadata_file->volume_name_attribute,
		     internal_mft_metadata_file->io_handle,
		     internal_mft_metadata_file->file_io_handle,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read value of $VOLUME_NAME attribute.",
			 function );

			return( -1 );
		}
	}
	*attribute = internal_mft_metadata_file->volume_name_attribute;

	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * This value is retrieved from the $VOLUME_NAME attribute of the $Volume metadata file
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_mft_metadata_file_get_utf8_volume_name_size(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *volume_name_attribute                       = NULL;
	libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file = NULL;
	static char *function                                              = "libfsntfs_mft_metadata_file_get_utf8_volume_name_size";
	int result                                                         = 0;

	if( mft_metadata_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT metadata file.",
		 function );

		return( -1 );
	}
	internal_mft_metadata_file = (libfsntfs_internal_mft_metadata_file_t *) mft_metadata_file;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_mft_metadata_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libfsntfs_internal_mft_metadata_file_get_volume_name_attribute(
	          internal_mft_metadata_file,
	          &volume_name_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume name attribute.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		if( libfsntfs_volume_name_attribute_get_utf8_name_size(
		     volume_name_attribute,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size of UTF-8 name from volume name attribute.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_mft_metadata_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * This value is retrieved from the $VOLUME_NAME attribute of the $Volume metadata file
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_mft_metadata_file_get_utf8_volume_name(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *volume_name_attribute                       = NULL;
	libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file = NULL;
	static char *function                                              = "libfsntfs_mft_metadata_file_get_utf8_volume_name";
	int result                                                         = 0;

	if( mft_metadata_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT metadata file.",
		 function );

		return( -1 );
	}
	internal_mft_metadata_file = (libfsntfs_internal_mft_metadata_file_t *) mft_metadata_file;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_mft_metadata_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libfsntfs_internal_mft_metadata_file_get_volume_name_attribute(
	          internal_mft_metadata_file,
	          &volume_name_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume name attribute.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		if( libfsntfs_volume_name_attribute_get_utf8_name(
		     volume_name_attribute,
		     utf8_string,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 name from volume name attribute.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_mft_metadata_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * This value is retrieved from the $VOLUME_NAME attribute of the $Volume metadata file
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_mft_metadata_file_get_utf16_volume_name_size(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *volume_name_attribute                       = NULL;
	libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file = NULL;
	static char *function                                              = "libfsntfs_mft_metadata_file_get_utf16_volume_name_size";
	int result                                                         = 0;

	if( mft_metadata_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT metadata file.",
		 function );

		return( -1 );
	}
	internal_mft_metadata_file = (libfsntfs_internal_mft_metadata_file_t *) mft_metadata_file;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_mft_metadata_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libfsntfs_internal_mft_metadata_file_get_volume_name_attribute(
	          internal_mft_metadata_file,
	          &volume_name_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume name attribute.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		if( libfsntfs_volume_name_attribute_get_utf16_name_size(
		     volume_name_attribute,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size of UTF-16 name from volume name attribute.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_mft_metadata_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * This value is retrieved from the $VOLUME_NAME attribute of the $Volume metadata file
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_mft_metadata_file_get_utf16_volume_name(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *volume_name_attribute                       = NULL;
	libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file = NULL;
	static char *function                                              = "libfsntfs_mft_metadata_file_get_utf16_volume_name";
	int result                                                         = 0;

	if( mft_metadata_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT metadata file.",
		 function );

		return( -1 );
	}
	internal_mft_metadata_file = (libfsntfs_internal_mft_metadata_file_t *) mft_metadata_file;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_mft_metadata_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libfsntfs_internal_mft_metadata_file_get_volume_name_attribute(
	          internal_mft_metadata_file,
	          &volume_name_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume name attribute.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		if( libfsntfs_volume_name_attribute_get_utf16_name(
		     volume_name_attribute,
		     utf16_string,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 name from volume name attribute.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_mft_metadata_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the volume version
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_mft_metadata_file_get_volume_version(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     uint8_t *major_version,
     uint8_t *minor_version,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *volume_information_attribute                = NULL;
	libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file = NULL;
	static char *function                                              = "libfsntfs_mft_metadata_file_get_volume_version";
	int result                                                         = 0;

	if( mft_metadata_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT metadata file.",
		 function );

		return( -1 );
	}
	internal_mft_metadata_file = (libfsntfs_internal_mft_metadata_file_t *) mft_metadata_file;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_mft_metadata_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libfsntfs_internal_mft_metadata_file_get_volume_information_attribute(
	          internal_mft_metadata_file,
	          &volume_information_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume information attribute.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		if( libfsntfs_volume_information_attribute_get_version(
		     volume_information_attribute,
		     major_version,
		     minor_version,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve version from volume information attribute.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_mft_metadata_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the volume flags
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_mft_metadata_file_get_volume_flags(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     uint16_t *flags,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *volume_information_attribute                = NULL;
	libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file = NULL;
	static char *function                                              = "libfsntfs_mft_metadata_file_get_volume_flags";
	int result                                                         = 0;

	if( mft_metadata_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT metadata file.",
		 function );

		return( -1 );
	}
	internal_mft_metadata_file = (libfsntfs_internal_mft_metadata_file_t *) mft_metadata_file;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_mft_metadata_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libfsntfs_internal_mft_metadata_file_get_volume_information_attribute(
	          internal_mft_metadata_file,
	          &volume_information_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume information attribute.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		if( libfsntfs_volume_information_attribute_get_flags(
		     volume_information_attribute,
		     flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve flags from volume information attribute.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_mft_metadata_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the number of file entries (MFT entries)
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_metadata_file_get_number_of_file_entries(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     uint64_t *number_of_file_entries,
     libcerror_error_t **error )
{
	libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file = NULL;
	static char *function                                              = "libfsntfs_mft_metadata_file_get_number_of_file_entries";
	int result                                                         = 1;

	if( mft_metadata_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT metadata file.",
		 function );

		return( -1 );
	}
	internal_mft_metadata_file = (libfsntfs_internal_mft_metadata_file_t *) mft_metadata_file;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_mft_metadata_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsntfs_file_system_get_number_of_mft_entries(
	     internal_mft_metadata_file->file_system,
	     number_of_file_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of MFT entries.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_mft_metadata_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the file entry of a specific MFT entry index
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_metadata_file_get_file_entry_by_index(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     uint64_t mft_entry_index,
     libfsntfs_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file = NULL;
	static char *function                                              = "libfsntfs_mft_metadata_file_get_file_entry_by_index";
	int result                                                         = 1;

	if( mft_metadata_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT metadata file.",
		 function );

		return( -1 );
	}
	internal_mft_metadata_file = (libfsntfs_internal_mft_metadata_file_t *) mft_metadata_file;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( *file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file entry value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_mft_metadata_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libfsntfs_file_entry_initialize(
	     file_entry,
	     internal_mft_metadata_file->io_handle,
	     internal_mft_metadata_file->file_io_handle,
	     internal_mft_metadata_file->file_system,
	     mft_entry_index,
	     NULL,
	     LIBFSNTFS_FILE_ENTRY_FLAGS_MFT_ONLY,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file entry from MFT entry: %" PRIu64 ".",
		 function,
		 mft_entry_index );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_mft_metadata_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}


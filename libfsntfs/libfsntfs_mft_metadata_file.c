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

#include <common.h>
#include <memory.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include "libfsntfs_debug.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_file_entry.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libuna.h"
#include "libfsntfs_mft_entry.h"
#include "libfsntfs_mft_metadata_file.h"

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

		goto on_error;
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
	*mft_metadata_file = (libfsntfs_mft_metadata_file_t *) internal_mft_metadata_file;

	return( 1 );

on_error:
	if( internal_mft_metadata_file != NULL )
	{
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
	internal_mft_metadata_file->file_io_handle_created_in_library = 1;

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
	internal_mft_metadata_file->file_io_handle_created_in_library = 1;

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

#endif

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
	static char *function                                              = "libfsntfs_mft_metadata_file_open_file_io_handle";
	int bfio_access_flags                                              = 0;
	int file_io_handle_is_open                                         = 0;

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
		 "%s: unable to open file.",
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
		internal_mft_metadata_file->file_io_handle_opened_in_library = 1;
	}
	if( libfsntfs_mft_metadata_file_open_read(
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
	internal_mft_metadata_file->file_io_handle = file_io_handle;

	return( 1 );

on_error:
	if( ( file_io_handle_is_open == 0 )
	 && ( internal_mft_metadata_file->file_io_handle_opened_in_library != 0 ) )
	{
		libbfio_handle_close(
		 file_io_handle,
		 error );

		internal_mft_metadata_file->file_io_handle_opened_in_library = 0;
	}
	internal_mft_metadata_file->file_io_handle = NULL;

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
	if( libfsntfs_mft_free(
	     &( internal_mft_metadata_file->mft ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free MFT.",
		 function );

		result = -1;
	}
	return( result );
}

/* Opens a MFT metadata file for reading
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_metadata_file_open_read(
     libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libfsntfs_mft_entry_t *mft_entry = NULL;
	static char *function            = "libfsntfs_mft_metadata_file_open_read";
	size64_t file_size               = 0;

	if( internal_mft_metadata_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal MFT metadata file.",
		 function );

		return( -1 );
	}
	if( internal_mft_metadata_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal MFT metadata file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_mft_metadata_file->mft != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid internal MFT metadata file - MFT value already set.",
		 function );

		return( -1 );
	}
	if( libbfio_handle_get_size(
	     file_io_handle,
	     &file_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file size.",
		 function );

		goto on_error;
	}
/* TODO allow to set the values */
/* TODO scan for signature to determine MFT entry size */
	internal_mft_metadata_file->io_handle->bytes_per_sector   = 512;
	internal_mft_metadata_file->io_handle->mft_offset         = 0;
/* TODO if not set FILE signature try scan? */
	internal_mft_metadata_file->io_handle->mft_entry_size     = 1024;
/* TODO if not set INDX signature try scan? */
	internal_mft_metadata_file->io_handle->index_entry_size   = 4096;
	internal_mft_metadata_file->io_handle->cluster_block_size = 4096;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading MFT entry: 0:\n" );
	}
#endif
	if( libfsntfs_mft_initialize(
	     &( internal_mft_metadata_file->mft ),
	     internal_mft_metadata_file->io_handle,
	     internal_mft_metadata_file->io_handle->mft_offset,
	     file_size,
	     (size64_t) internal_mft_metadata_file->io_handle->mft_entry_size,
	     LIBFSNTFS_FILE_ENTRY_FLAGS_MFT_ONLY,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create MFT.",
		 function );

		goto on_error;
	}
	if( libfsntfs_mft_entry_initialize(
	     &mft_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create MFT entry.",
		 function );

		goto on_error;
	}
	if( libfsntfs_mft_read_mft_entry(
	     internal_mft_metadata_file->mft,
	     internal_mft_metadata_file->io_handle,
	     file_io_handle,
	     internal_mft_metadata_file->io_handle->mft_offset,
	     0,
	     mft_entry,
	     LIBFSNTFS_FILE_ENTRY_FLAGS_MFT_ONLY,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read MFT entry: 0.",
		 function );

		goto on_error;
	}
	if( mft_entry->data_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid MFT entry: 0 - missing data attribute.",
		 function );

		goto on_error;
	}
	internal_mft_metadata_file->mft->number_of_mft_entries = (uint64_t) ( file_size / internal_mft_metadata_file->io_handle->mft_entry_size );

	if( internal_mft_metadata_file->mft->number_of_mft_entries > (uint64_t) INT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of MFT entries value out of bounds.",
		 function );

		return( -1 );
	}
	if( libfsntfs_mft_entry_free(
	     &mft_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free MFT entry.",
		 function );

		goto on_error;
	}
	internal_mft_metadata_file->file_io_handle = file_io_handle;

	return( 1 );

on_error:
	if( internal_mft_metadata_file->mft != NULL )
	{
		libfsntfs_mft_free(
		 &( internal_mft_metadata_file->mft ),
		 NULL );
	}
	if( mft_entry != NULL )
	{
		libfsntfs_mft_entry_free(
		 &mft_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the size of the UTF-8 encoded volume name
 * The returned size includes the end of string character
 * This value is retrieved from the $VOLUME_NAME attribute of the $Volume metadata file
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_metadata_file_get_utf8_volume_name_size(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     size_t *utf8_volume_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file = NULL;
	static char *function                                              = "libfsntfs_mft_metadata_file_get_utf8_volume_name_size";

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

	if( libfsntfs_mft_get_utf8_volume_name_size(
	     internal_mft_metadata_file->mft,
	     internal_mft_metadata_file->file_io_handle,
	     utf8_volume_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-8 volume name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded volume name
 * The size should include the end of string character
 * This value is retrieved from the $VOLUME_NAME attribute of the $Volume metadata file
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_metadata_file_get_utf8_volume_name(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     uint8_t *utf8_volume_name,
     size_t utf8_volume_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file = NULL;
	static char *function                                              = "libfsntfs_mft_metadata_file_get_utf8_volume_name";

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

	if( libfsntfs_mft_get_utf8_volume_name(
	     internal_mft_metadata_file->mft,
	     internal_mft_metadata_file->file_io_handle,
	     utf8_volume_name,
	     utf8_volume_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 volume name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded volume name
 * The returned size includes the end of string character
 * This value is retrieved from the $VOLUME_NAME attribute of the $Volume metadata file
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_metadata_file_get_utf16_volume_name_size(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     size_t *utf16_volume_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file = NULL;
	static char *function                                              = "libfsntfs_mft_metadata_file_get_utf16_volume_name_size";

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

	if( libfsntfs_mft_get_utf16_volume_name_size(
	     internal_mft_metadata_file->mft,
	     internal_mft_metadata_file->file_io_handle,
	     utf16_volume_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-16 volume name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded volume name
 * The size should include the end of string character
 * This value is retrieved from the $VOLUME_NAME attribute of the $Volume metadata file
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_metadata_file_get_utf16_volume_name(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     uint16_t *utf16_volume_name,
     size_t utf16_volume_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file = NULL;
	static char *function                                              = "libfsntfs_mft_metadata_file_get_utf16_volume_name";

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

	if( libfsntfs_mft_get_utf16_volume_name(
	     internal_mft_metadata_file->mft,
	     internal_mft_metadata_file->file_io_handle,
	     utf16_volume_name,
	     utf16_volume_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 volume name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the volume version
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_metadata_file_get_volume_version(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     uint8_t *major_version,
     uint8_t *minor_version,
     libcerror_error_t **error )
{
	libfsntfs_internal_mft_metadata_file_t *internal_mft_metadata_file = NULL;
	static char *function                                              = "libfsntfs_mft_metadata_file_get_volume_version";

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

	if( libfsntfs_mft_get_volume_version(
	     internal_mft_metadata_file->mft,
	     internal_mft_metadata_file->file_io_handle,
	     major_version,
	     minor_version,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume version.",
		 function );

		return( -1 );
	}
	return( 1 );
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

	if( libfsntfs_mft_get_number_of_entries(
	     internal_mft_metadata_file->mft,
	     number_of_file_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of MFT entries.",
		 function );

		return( -1 );
	}
	return( 1 );
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
	libfsntfs_mft_entry_t *mft_entry                                   = NULL;
	static char *function                                              = "libfsntfs_mft_metadata_file_get_file_entry_by_index";

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
	if( libfsntfs_mft_get_mft_entry_by_index(
	     internal_mft_metadata_file->mft,
	     internal_mft_metadata_file->file_io_handle,
	     mft_entry_index,
	     &mft_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT entry: %" PRIu64 ".",
		 function,
		 mft_entry_index );

		return( -1 );
	}
	if( libfsntfs_file_entry_initialize(
	     file_entry,
	     internal_mft_metadata_file->io_handle,
	     internal_mft_metadata_file->file_io_handle,
	     internal_mft_metadata_file->mft,
	     NULL,
	     mft_entry,
	     NULL,
	     LIBFSNTFS_FILE_ENTRY_FLAGS_MFT_ONLY,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file entry.",
		 function );

		return( -1 );
	}
	return( 1 );
}


/*
 * MFT file functions
 *
 * Copyright (C) 2010-2015, Joachim Metz <joachim.metz@gmail.com>
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
#include <types.h>

#include "libfsntfs_debug.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libcstring.h"
#include "libfsntfs_libuna.h"
#include "libfsntfs_mft_entry.h"
#include "libfsntfs_mft_file.h"

/* Creates a MFT file
 * Make sure the value mft_file is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_file_initialize(
     libfsntfs_mft_file_t **mft_file,
     libcerror_error_t **error )
{
	libfsntfs_internal_mft_file_t *internal_mft_file = NULL;
	static char *function                            = "libfsntfs_mft_file_initialize";

	if( mft_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT file.",
		 function );

		return( -1 );
	}
	if( *mft_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid MFT file value already set.",
		 function );

		return( -1 );
	}
	internal_mft_file = memory_allocate_structure(
	                     libfsntfs_internal_mft_file_t );

	if( internal_mft_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create MFT file.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_mft_file,
	     0,
	     sizeof( libfsntfs_internal_mft_file_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear MFT file.",
		 function );

		goto on_error;
	}
	if( libfsntfs_io_handle_initialize(
	     &( internal_mft_file->io_handle ),
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
	*mft_file = (libfsntfs_mft_file_t *) internal_mft_file;

	return( 1 );

on_error:
	if( internal_mft_file != NULL )
	{
		memory_free(
		 internal_mft_file );
	}
	return( -1 );
}

/* Frees a MFT file
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_file_free(
     libfsntfs_mft_file_t **mft_file,
     libcerror_error_t **error )
{
	libfsntfs_internal_mft_file_t *internal_mft_file = NULL;
	static char *function                            = "libfsntfs_mft_file_free";
	int result                                       = 1;

	if( mft_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT file.",
		 function );

		return( -1 );
	}
	if( *mft_file != NULL )
	{
		internal_mft_file = (libfsntfs_internal_mft_file_t *) *mft_file;

		if( internal_mft_file->file_io_handle != NULL )
		{
			if( libfsntfs_mft_file_close(
			     *mft_file,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close MFT file.",
				 function );

				result = -1;
			}
		}
		*mft_file = NULL;

		if( libfsntfs_io_handle_free(
		     &( internal_mft_file->io_handle ),
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
		 internal_mft_file );
	}
	return( result );
}

/* Signals the mft_file to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_file_signal_abort(
     libfsntfs_mft_file_t *mft_file,
     libcerror_error_t **error )
{
	libfsntfs_internal_mft_file_t *internal_mft_file = NULL;
	static char *function                            = "libfsntfs_mft_file_signal_abort";

	if( mft_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT file.",
		 function );

		return( -1 );
	}
	internal_mft_file = (libfsntfs_internal_mft_file_t *) mft_file;

	if( internal_mft_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid MFT file - missing IO handle.",
		 function );

		return( -1 );
	}
	internal_mft_file->io_handle->abort = 1;

	return( 1 );
}

/* Opens a MFT file
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_file_open(
     libfsntfs_mft_file_t *mft_file,
     const char *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle                 = NULL;
	libfsntfs_internal_mft_file_t *internal_mft_file = NULL;
	static char *function                            = "libfsntfs_mft_file_open";

	if( mft_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT file.",
		 function );

		return( -1 );
	}
	internal_mft_file = (libfsntfs_internal_mft_file_t *) mft_file;

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
	     libcstring_narrow_string_length(
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
	if( libfsntfs_mft_file_open_file_io_handle(
	     mft_file,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open mft_file: %s.",
		 function,
		 filename );

		goto on_error;
	}
	internal_mft_file->file_io_handle_created_in_library = 1;

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

/* Opens a MFT file
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_file_open_wide(
     libfsntfs_mft_file_t *mft_file,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle                 = NULL;
	libfsntfs_internal_mft_file_t *internal_mft_file = NULL;
	static char *function                            = "libfsntfs_mft_file_open_wide";

	if( mft_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT file.",
		 function );

		return( -1 );
	}
	internal_mft_file = (libfsntfs_internal_mft_file_t *) mft_file;

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
	     libcstring_wide_string_length(
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
	if( libfsntfs_mft_file_open_file_io_handle(
	     mft_file,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open mft_file: %ls.",
		 function,
		 filename );

		goto on_error;
	}
	internal_mft_file->file_io_handle_created_in_library = 1;

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

/* Opens a MFT file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_file_open_file_io_handle(
     libfsntfs_mft_file_t *mft_file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error )
{
	libfsntfs_internal_mft_file_t *internal_mft_file = NULL;
	static char *function                            = "libfsntfs_mft_file_open_file_io_handle";
	int bfio_access_flags                            = 0;
	int file_io_handle_is_open                       = 0;

	if( mft_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT file.",
		 function );

		return( -1 );
	}
	internal_mft_file = (libfsntfs_internal_mft_file_t *) mft_file;

	if( internal_mft_file->file_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid MFT file - file IO handle already set.",
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
		internal_mft_file->file_io_handle_opened_in_library = 1;
	}
	if( libfsntfs_mft_file_open_read(
	     internal_mft_file,
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
	internal_mft_file->file_io_handle = file_io_handle;

	return( 1 );

on_error:
	if( ( file_io_handle_is_open == 0 )
	 && ( internal_mft_file->file_io_handle_opened_in_library != 0 ) )
	{
		libbfio_handle_close(
		 file_io_handle,
		 error );

		internal_mft_file->file_io_handle_opened_in_library = 0;
	}
	internal_mft_file->file_io_handle = NULL;

	return( -1 );
}

/* Closes a MFT file
 * Returns 0 if successful or -1 on error
 */
int libfsntfs_mft_file_close(
     libfsntfs_mft_file_t *mft_file,
     libcerror_error_t **error )
{
	libfsntfs_internal_mft_file_t *internal_mft_file = NULL;
	static char *function                            = "libfsntfs_mft_file_close";
	int result                                       = 0;

	if( mft_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT file.",
		 function );

		return( -1 );
	}
	internal_mft_file = (libfsntfs_internal_mft_file_t *) mft_file;

	if( internal_mft_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid MFT file - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( internal_mft_file->file_io_handle_created_in_library != 0 )
		{
			if( libfsntfs_debug_print_read_offsets(
			     internal_mft_file->file_io_handle,
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
	if( internal_mft_file->file_io_handle_opened_in_library != 0 )
	{
		if( libbfio_handle_close(
		     internal_mft_file->file_io_handle,
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
		internal_mft_file->file_io_handle_opened_in_library = 0;
	}
	if( internal_mft_file->file_io_handle_created_in_library != 0 )
	{
		if( libbfio_handle_free(
		     &( internal_mft_file->file_io_handle ),
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
		internal_mft_file->file_io_handle_created_in_library = 0;
	}
	internal_mft_file->file_io_handle = NULL;

	if( libfsntfs_io_handle_clear(
	     internal_mft_file->io_handle,
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
	if( libfdata_vector_free(
	     &( internal_mft_file->mft_entry_vector ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free MFT entry vector.",
		 function );

		result = -1;
	}
	if( libfcache_cache_free(
	     &( internal_mft_file->mft_entry_cache ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free MFT entry cache.",
		 function );

		result = -1;
	}
	return( result );
}

/* Opens a MFT file for reading
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_file_open_read(
     libfsntfs_internal_mft_file_t *internal_mft_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libfsntfs_mft_entry_t *mft_entry = NULL;
	static char *function            = "libfsntfs_mft_file_open_read";
	size64_t file_size               = 0;
	int data_run_index               = 0;
	int segment_index                = 0;

	if( internal_mft_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal MFT file.",
		 function );

		return( -1 );
	}
	if( internal_mft_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal MFT file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_mft_file->mft_entry_vector != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid internal MFT file - MFT entry vector value already set.",
		 function );

		return( -1 );
	}
	if( internal_mft_file->mft_entry_cache != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid internal MFT file - MFT entry cache value already set.",
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
	internal_mft_file->io_handle->mft_offset     = 0;
	internal_mft_file->io_handle->mft_entry_size = 1024;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading MFT entry: 0:\n" );
	}
#endif
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
	if( libfsntfs_mft_entry_read(
	     mft_entry,
	     internal_mft_file->io_handle,
	     internal_mft_file->file_io_handle,
	     NULL,
	     internal_mft_file->io_handle->mft_offset,
	     0,
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
	if( libfdata_vector_initialize(
	     &( internal_mft_file->mft_entry_vector ),
	     (size64_t) internal_mft_file->io_handle->mft_entry_size,
	     (intptr_t *) internal_mft_file->io_handle,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_vector_t *, libfcache_cache_t *, int, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsntfs_mft_entry_read_element_data,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create MFT entry vector.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_append_segment(
	     internal_mft_file->mft_entry_vector,
	     &segment_index,
	     0,
	     internal_mft_file->io_handle->mft_offset,
	     file_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append segment: %d to MFT entry vector.",
		 function,
		 data_run_index );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( internal_mft_file->mft_entry_cache ),
	     LIBFSNTFS_MAXIMUM_CACHE_ENTRIES_MFT_ENTRIES,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create MFT entry cache.",
		 function );

		goto on_error;
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
	return( 1 );

on_error:
	if( internal_mft_file->mft_entry_cache != NULL )
	{
		libfcache_cache_free(
		 &( internal_mft_file->mft_entry_cache ),
		 NULL );
	}
	if( internal_mft_file->mft_entry_vector != NULL )
	{
		libfdata_vector_free(
		 &( internal_mft_file->mft_entry_vector ),
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


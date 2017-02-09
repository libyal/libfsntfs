/*
 * Volume functions
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

#include "libfsntfs_attribute.h"
#include "libfsntfs_cluster_block.h"
#include "libfsntfs_cluster_block_vector.h"
#include "libfsntfs_data_run.h"
#include "libfsntfs_debug.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_directory_entries_tree.h"
#include "libfsntfs_directory_entry.h"
#include "libfsntfs_file_entry.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libuna.h"
#include "libfsntfs_mft_entry.h"
#include "libfsntfs_security_descriptor_index.h"
#include "libfsntfs_usn_change_journal.h"
#include "libfsntfs_volume.h"

/* Creates a volume
 * Make sure the value volume is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_initialize(
     libfsntfs_volume_t **volume,
     libcerror_error_t **error )
{
	libfsntfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsntfs_volume_initialize";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( *volume != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume value already set.",
		 function );

		return( -1 );
	}
	internal_volume = memory_allocate_structure(
	                   libfsntfs_internal_volume_t );

	if( internal_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_volume,
	     0,
	     sizeof( libfsntfs_internal_volume_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear volume.",
		 function );

		goto on_error;
	}
	if( libfsntfs_io_handle_initialize(
	     &( internal_volume->io_handle ),
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
	     &( internal_volume->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to intialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	*volume = (libfsntfs_volume_t *) internal_volume;

	return( 1 );

on_error:
	if( internal_volume != NULL )
	{
		memory_free(
		 internal_volume );
	}
	return( -1 );
}

/* Frees a volume
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_free(
     libfsntfs_volume_t **volume,
     libcerror_error_t **error )
{
	libfsntfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsntfs_volume_free";
	int result                                   = 1;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( *volume != NULL )
	{
		internal_volume = (libfsntfs_internal_volume_t *) *volume;

		if( internal_volume->file_io_handle != NULL )
		{
			if( libfsntfs_volume_close(
			     *volume,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close volume.",
				 function );

				result = -1;
			}
		}
		*volume = NULL;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_volume->read_write_lock ),
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
		     &( internal_volume->io_handle ),
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
		 internal_volume );
	}
	return( result );
}

/* Signals the volume to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_signal_abort(
     libfsntfs_volume_t *volume,
     libcerror_error_t **error )
{
	libfsntfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsntfs_volume_signal_abort";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsntfs_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	internal_volume->io_handle->abort = 1;

	return( 1 );
}

/* Opens a volume
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_open(
     libfsntfs_volume_t *volume,
     const char *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle             = NULL;
	libfsntfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsntfs_volume_open";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsntfs_internal_volume_t *) volume;

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
	if( libfsntfs_volume_open_file_io_handle(
	     volume,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open volume: %s.",
		 function,
		 filename );

		goto on_error;
	}
	internal_volume->file_io_handle_created_in_library = 1;

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

/* Opens a volume
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_open_wide(
     libfsntfs_volume_t *volume,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle             = NULL;
	libfsntfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsntfs_volume_open_wide";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsntfs_internal_volume_t *) volume;

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
	if( libfsntfs_volume_open_file_io_handle(
	     volume,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open volume: %ls.",
		 function,
		 filename );

		goto on_error;
	}
	internal_volume->file_io_handle_created_in_library = 1;

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

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Opens a volume using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_open_file_io_handle(
     libfsntfs_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error )
{
	libfsntfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsntfs_volume_open_file_io_handle";
	int bfio_access_flags                        = 0;
	int file_io_handle_is_open                   = 0;
	int file_io_handle_opened_in_library         = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsntfs_internal_volume_t *) volume;

	if( internal_volume->file_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - file IO handle already set.",
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
	if( libfsntfs_internal_volume_open_read(
	     internal_volume,
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
	internal_volume->file_io_handle                   = file_io_handle;
	internal_volume->file_io_handle_opened_in_library = file_io_handle_opened_in_library;

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

/* Closes a volume
 * Returns 0 if successful or -1 on error
 */
int libfsntfs_volume_close(
     libfsntfs_volume_t *volume,
     libcerror_error_t **error )
{
	libfsntfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsntfs_volume_close";
	int result                                   = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsntfs_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( internal_volume->file_io_handle_created_in_library != 0 )
		{
			if( libfsntfs_debug_print_read_offsets(
			     internal_volume->file_io_handle,
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
	if( internal_volume->file_io_handle_opened_in_library != 0 )
	{
		if( libbfio_handle_close(
		     internal_volume->file_io_handle,
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
		internal_volume->file_io_handle_opened_in_library = 0;
	}
	if( internal_volume->file_io_handle_created_in_library != 0 )
	{
		if( libbfio_handle_free(
		     &( internal_volume->file_io_handle ),
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
		internal_volume->file_io_handle_created_in_library = 0;
	}
	internal_volume->file_io_handle = NULL;

	if( libfsntfs_io_handle_clear(
	     internal_volume->io_handle,
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
	     &( internal_volume->mft ),
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
	if( internal_volume->security_descriptor_index != NULL )
	{
		if( libfsntfs_security_descriptor_index_free(
		     &( internal_volume->security_descriptor_index ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free security descriptor index.",
			 function );

			result = -1;
		}
	}
	return( result );
}

/* Opens a volume for reading
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_internal_volume_open_read(
     libfsntfs_internal_volume_t *internal_volume,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libfsntfs_mft_entry_t *mft_entry = NULL;
	static char *function            = "libfsntfs_internal_volume_open_read";
	size64_t mft_size                = 0;

	if( internal_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal volume.",
		 function );

		return( -1 );
	}
	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_volume->mft != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid internal volume - MFT value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading volume header:\n" );
	}
#endif
	if( libfsntfs_io_handle_read_volume_header(
	     internal_volume->io_handle,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header.",
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
	if( ( internal_volume->io_handle->mft_offset < 0 )
	 || ( (size64_t) internal_volume->io_handle->mft_offset >= internal_volume->io_handle->volume_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid MFT offset value out of bounds.",
		 function );

		goto on_error;
	}
	mft_size = internal_volume->io_handle->volume_size - internal_volume->io_handle->mft_offset;

	/* Since MFT entry 0 can contain an attribute list
	 * we define MFT entry vector before knowning all the data runs
	 */
	if( libfsntfs_mft_initialize(
	     &( internal_volume->mft ),
	     internal_volume->io_handle,
	     internal_volume->io_handle->mft_offset,
	     mft_size,
	     (size64_t) internal_volume->io_handle->mft_entry_size,
	     0,
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
	     internal_volume->mft,
	     internal_volume->io_handle,
	     file_io_handle,
	     internal_volume->io_handle->mft_offset,
	     0,
	     mft_entry,
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Initializing MFT entry vector:\n" );
	}
#endif
	if( libfsntfs_mft_set_data_runs(
	     internal_volume->mft,
	     mft_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set MFT data runs.",
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
/* TODO what about the mirror MFT ? */

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading MFT entry: 6 ($Bitmap):\n" );
	}
#endif
	if( libfsntfs_internal_volume_read_bitmap(
	     internal_volume,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read MFT entry: 6.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading MFT entry: 9 ($Secure):\n" );
	}
#endif
	if( libfsntfs_internal_volume_read_security_descriptors(
	     internal_volume,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read MFT entry: 9.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( internal_volume->mft != NULL )
	{
		libfsntfs_mft_free(
		 &( internal_volume->mft ),
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

/* Determines if the volume has BitLocker Drive Encryption (BDE)
 * Returns 1 if the volume has BitLocker Drive Encryption, 0 if not or -1 on error
 */
int libfsntfs_volume_has_bitlocker_drive_encryption(
     libfsntfs_volume_t *volume,
     libcerror_error_t **error )
{
	libfsntfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsntfs_volume_has_bitlocker_drive_encryption";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsntfs_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
/* TODO */
	return( 0 );
}

/* Determines if the volume has Volume Shadow Snapshots (VSS)
 * Returns 1 if the volume has Volume Shadow Snapshots, 0 if not or -1 on error
 */
int libfsntfs_volume_has_volume_shadow_snapshots(
     libfsntfs_volume_t *volume,
     libcerror_error_t **error )
{
	libfsntfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsntfs_volume_has_volume_shadow_snapshots";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsntfs_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
/* TODO */
	return( 0 );
}

/* Retrieves the cluster block size
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_get_cluster_block_size(
     libfsntfs_volume_t *volume,
     size_t *cluster_block_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsntfs_volume_get_cluster_block_size";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsntfs_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( cluster_block_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cluster block size.",
		 function );

		return( -1 );
	}
	*cluster_block_size = internal_volume->io_handle->cluster_block_size;

	return( 1 );
}

/* Retrieves the MFT entry size
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_get_mft_entry_size(
     libfsntfs_volume_t *volume,
     size32_t *mft_entry_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsntfs_volume_get_mft_entry_size";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsntfs_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( mft_entry_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT entry size.",
		 function );

		return( -1 );
	}
	*mft_entry_size = (size32_t) internal_volume->io_handle->mft_entry_size;

	return( 1 );
}

/* Retrieves the index entry size
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_get_index_entry_size(
     libfsntfs_volume_t *volume,
     size32_t *index_entry_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsntfs_volume_get_index_entry_size";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsntfs_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( index_entry_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index entry size.",
		 function );

		return( -1 );
	}
	*index_entry_size = (size32_t) internal_volume->io_handle->index_entry_size;

	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * This value is retrieved from the $VOLUME_NAME attribute of the $Volume metadata file
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_get_utf8_name_size(
     libfsntfs_volume_t *volume,
     size_t *utf8_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsntfs_volume_get_utf8_name_size";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsntfs_internal_volume_t *) volume;

	if( libfsntfs_mft_get_utf8_volume_name_size(
	     internal_volume->mft,
	     internal_volume->file_io_handle,
	     utf8_name_size,
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

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * This value is retrieved from the $VOLUME_NAME attribute of the $Volume metadata file
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_get_utf8_name(
     libfsntfs_volume_t *volume,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsntfs_volume_get_utf8_name";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsntfs_internal_volume_t *) volume;

	if( libfsntfs_mft_get_utf8_volume_name(
	     internal_volume->mft,
	     internal_volume->file_io_handle,
	     utf8_name,
	     utf8_name_size,
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

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * This value is retrieved from the $VOLUME_NAME attribute of the $Volume metadata file
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_get_utf16_name_size(
     libfsntfs_volume_t *volume,
     size_t *utf16_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsntfs_volume_get_utf16_name_size";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsntfs_internal_volume_t *) volume;

	if( libfsntfs_mft_get_utf16_volume_name_size(
	     internal_volume->mft,
	     internal_volume->file_io_handle,
	     utf16_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-16 volune name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * This value is retrieved from the $VOLUME_NAME attribute of the $Volume metadata file
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_get_utf16_name(
     libfsntfs_volume_t *volume,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsntfs_volume_get_utf16_name";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsntfs_internal_volume_t *) volume;

	if( libfsntfs_mft_get_utf16_volume_name(
	     internal_volume->mft,
	     internal_volume->file_io_handle,
	     utf16_name,
	     utf16_name_size,
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

/* Retrieves the version
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_get_version(
     libfsntfs_volume_t *volume,
     uint8_t *major_version,
     uint8_t *minor_version,
     libcerror_error_t **error )
{
	libfsntfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsntfs_volume_get_version";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsntfs_internal_volume_t *) volume;

	if( libfsntfs_mft_get_volume_version(
	     internal_volume->mft,
	     internal_volume->file_io_handle,
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

/* Retrieves the serial number
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_get_serial_number(
     libfsntfs_volume_t *volume,
     uint64_t *serial_number,
     libcerror_error_t **error )
{
	libfsntfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsntfs_volume_get_serial_number";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsntfs_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( serial_number == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid serial number.",
		 function );

		return( -1 );
	}
	*serial_number = internal_volume->io_handle->volume_serial_number;

	return( 1 );
}

/* Retrieves the number of file entries (MFT entries)
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_get_number_of_file_entries(
     libfsntfs_volume_t *volume,
     uint64_t *number_of_file_entries,
     libcerror_error_t **error )
{
	libfsntfs_internal_volume_t *internal_volume = NULL;
	static char *function                        = "libfsntfs_volume_get_number_of_file_entries";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsntfs_internal_volume_t *) volume;

	if( libfsntfs_mft_get_number_of_entries(
	     internal_volume->mft,
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
int libfsntfs_volume_get_file_entry_by_index(
     libfsntfs_volume_t *volume,
     uint64_t mft_entry_index,
     libfsntfs_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libfsntfs_internal_volume_t *internal_volume = NULL;
	libfsntfs_mft_entry_t *mft_entry             = NULL;
	static char *function                        = "libfsntfs_volume_get_file_entry_by_index";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsntfs_internal_volume_t *) volume;

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
	     internal_volume->mft,
	     internal_volume->file_io_handle,
	     mft_entry_index,
	     &mft_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT entry: %" PRIi64 ".",
		 function,
		 mft_entry_index );

		return( -1 );
	}
	if( libfsntfs_file_entry_initialize(
	     file_entry,
	     internal_volume->io_handle,
	     internal_volume->file_io_handle,
	     internal_volume->mft,
	     internal_volume->security_descriptor_index,
	     mft_entry,
	     NULL,
	     0,
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

/* Retrieves the root directory file entry
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_volume_get_root_directory(
     libfsntfs_volume_t *volume,
     libfsntfs_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libfsntfs_internal_volume_t *internal_volume = NULL;
	libfsntfs_mft_entry_t *mft_entry             = NULL;
	static char *function                        = "libfsntfs_volume_get_root_directory";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsntfs_internal_volume_t *) volume;

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
	     internal_volume->mft,
	     internal_volume->file_io_handle,
	     LIBFSNTFS_MFT_ENTRY_INDEX_ROOT_DIRECTORY,
	     &mft_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT entry: 5.",
		 function );

		return( -1 );
	}
	if( libfsntfs_file_entry_initialize(
	     file_entry,
	     internal_volume->io_handle,
	     internal_volume->file_io_handle,
	     internal_volume->mft,
	     internal_volume->security_descriptor_index,
	     mft_entry,
	     NULL,
	     0,
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

/* Retrieves the MFT entry for an UTF-8 encoded path
 * A new directory_entry is allocated if a match is found
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfsntfs_internal_volume_get_mft_and_directory_entry_by_utf8_path(
     libfsntfs_internal_volume_t *internal_volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsntfs_mft_entry_t **mft_entry,
     libfsntfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libcdata_btree_t *directory_entries_tree          = NULL;
	libfsntfs_directory_entry_t *safe_directory_entry = NULL;
	uint8_t *utf8_string_segment                      = NULL;
	static char *function                             = "libfsntfs_internal_volume_get_mft_and_directory_entry_by_utf8_path";
	libuna_unicode_character_t unicode_character      = 0;
	size_t utf8_string_index                          = 0;
	size_t utf8_string_segment_length                 = 0;
	uint64_t mft_entry_index                          = 0;
	int result                                        = 0;

	if( internal_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( mft_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT entry.",
		 function );

		return( -1 );
	}
	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( utf8_string_length > 0 )
	{
		/* Ignore a leading separator
		 */
		if( utf8_string[ utf8_string_index ] == (uint8_t) LIBFSNTFS_SEPARATOR )
		{
			utf8_string_index++;
		}
	}
	if( libfsntfs_mft_get_mft_entry_by_index(
	     internal_volume->mft,
	     internal_volume->file_io_handle,
	     LIBFSNTFS_MFT_ENTRY_INDEX_ROOT_DIRECTORY,
	     mft_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT entry: 5.",
		 function );

		goto on_error;
	}
	if( ( utf8_string_length == 0 )
	 || ( utf8_string_length == 1 ) )
	{
		result = 1;
	}
	else while( utf8_string_index < utf8_string_length )
	{
		if( directory_entries_tree != NULL )
		{
			if( libcdata_btree_free(
			     &directory_entries_tree,
			     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_directory_entry_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free directory entries tree.",
				 function );

				goto on_error;
			}
		}
		if( libcdata_btree_initialize(
		     &directory_entries_tree,
		     LIBFSNTFS_INDEX_TREE_MAXIMUM_NUMBER_OF_SUB_NODES,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create directory entries tree.",
			 function );

			goto on_error;
		}
		if( libfsntfs_mft_entry_read_directory_entries_tree(
		     *mft_entry,
		     internal_volume->io_handle,
		     internal_volume->file_io_handle,
		     directory_entries_tree,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read directory entries tree.",
			 function );

			goto on_error;
		}
		utf8_string_segment        = (uint8_t *) &( utf8_string[ utf8_string_index ] );
		utf8_string_segment_length = utf8_string_index;

		while( utf8_string_index < utf8_string_length )
		{
			if( libuna_unicode_character_copy_from_utf8(
			     &unicode_character,
			     utf8_string,
			     utf8_string_length,
			     &utf8_string_index,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-8 string to Unicode character.",
				 function );

				goto on_error;
			}
			if( ( unicode_character == (libuna_unicode_character_t) LIBFSNTFS_SEPARATOR )
			 || ( unicode_character == 0 ) )
			{
				utf8_string_segment_length += 1;

				break;
			}
		}
		utf8_string_segment_length = utf8_string_index - utf8_string_segment_length;

		if( utf8_string_segment_length == 0 )
		{
			result = 0;
		}
		else
		{
			result = libfsntfs_directory_entries_tree_get_directory_entry_by_utf8_name(
			          directory_entries_tree,
				  utf8_string_segment,
				  utf8_string_segment_length,
			          &safe_directory_entry,
			          error );
		}
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory entry by name.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			break;
		}
		if( libfsntfs_directory_entry_get_mft_entry_index(
		     safe_directory_entry,
		     &mft_entry_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve MFT entry index.",
			 function );

			goto on_error;
		}
		if( libfsntfs_mft_get_mft_entry_by_index(
		     internal_volume->mft,
		     internal_volume->file_io_handle,
		     mft_entry_index,
		     mft_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve MFT entry: %" PRIu64 ".",
			 function,
			 mft_entry_index );

			goto on_error;
		}
	}
	if( result != 0 )
	{
		if( libfsntfs_directory_entry_clone(
		     directory_entry,
		     safe_directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create directory entry.",
			 function );

			goto on_error;
		}
	}
	if( directory_entries_tree != NULL )
	{
		if( libcdata_btree_free(
		     &directory_entries_tree,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_directory_entry_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free directory entries tree.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( directory_entries_tree != NULL )
	{
		libcdata_btree_free(
		 &directory_entries_tree,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_directory_entry_free,
		 NULL );
	}
	if( *directory_entry != NULL )
	{
		libfsntfs_directory_entry_free(
		 directory_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the file entry for an UTF-8 encoded path
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfsntfs_volume_get_file_entry_by_utf8_path(
     libfsntfs_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsntfs_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libfsntfs_directory_entry_t *directory_entry = NULL;
	libfsntfs_internal_volume_t *internal_volume = NULL;
	libfsntfs_mft_entry_t *mft_entry             = NULL;
	static char *function                        = "libfsntfs_volume_get_file_entry_by_utf8_path";
	int result                                   = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsntfs_internal_volume_t *) volume;

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
	result = libfsntfs_internal_volume_get_mft_and_directory_entry_by_utf8_path(
	          internal_volume,
	          utf8_string,
	          utf8_string_length,
	          &mft_entry,
	          &directory_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT and directory entry by path.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		/* libfsntfs_file_entry_initialize takes over management of directory_entry
		 */
		if( libfsntfs_file_entry_initialize(
		     file_entry,
		     internal_volume->io_handle,
		     internal_volume->file_io_handle,
		     internal_volume->mft,
		     internal_volume->security_descriptor_index,
		     mft_entry,
		     directory_entry,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file entry.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( directory_entry != NULL )
	{
		libfsntfs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the MFT entry for an UTF-16 encoded path
 * A new directory_entry is allocated if a match is found
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfsntfs_internal_volume_get_mft_and_directory_entry_by_utf16_path(
     libfsntfs_internal_volume_t *internal_volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsntfs_mft_entry_t **mft_entry,
     libfsntfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libcdata_btree_t *directory_entries_tree          = NULL;
	libfsntfs_directory_entry_t *safe_directory_entry = NULL;
	uint16_t *utf16_string_segment                    = NULL;
	static char *function                             = "libfsntfs_internal_volume_get_mft_and_directory_entry_by_utf16_path";
	libuna_unicode_character_t unicode_character      = 0;
	size_t utf16_string_index                         = 0;
	size_t utf16_string_segment_length                = 0;
	uint64_t mft_entry_index                          = 0;
	int result                                        = 0;

	if( internal_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( mft_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT entry.",
		 function );

		return( -1 );
	}
	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( utf16_string_length > 0 )
	{
		/* Ignore a leading separator
		 */
		if( utf16_string[ utf16_string_index ] == (uint16_t) LIBFSNTFS_SEPARATOR )
		{
			utf16_string_index++;
		}
	}
	if( libfsntfs_mft_get_mft_entry_by_index(
	     internal_volume->mft,
	     internal_volume->file_io_handle,
	     LIBFSNTFS_MFT_ENTRY_INDEX_ROOT_DIRECTORY,
	     mft_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT entry: 5.",
		 function );

		goto on_error;
	}
	if( ( utf16_string_length == 0 )
	 || ( utf16_string_length == 1 ) )
	{
		result = 1;
	}
	else while( utf16_string_index < utf16_string_length )
	{
		if( directory_entries_tree != NULL )
		{
			if( libcdata_btree_free(
			     &directory_entries_tree,
			     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_directory_entry_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free directory entries tree.",
				 function );

				goto on_error;
			}
		}
		if( libcdata_btree_initialize(
		     &directory_entries_tree,
		     LIBFSNTFS_INDEX_TREE_MAXIMUM_NUMBER_OF_SUB_NODES,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create directory entries tree.",
			 function );

			goto on_error;
		}
		if( libfsntfs_mft_entry_read_directory_entries_tree(
		     *mft_entry,
		     internal_volume->io_handle,
		     internal_volume->file_io_handle,
		     directory_entries_tree,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read directory entries tree.",
			 function );

			goto on_error;
		}
		utf16_string_segment        = (uint16_t *) &( utf16_string[ utf16_string_index ] );
		utf16_string_segment_length = utf16_string_index;

		while( utf16_string_index < utf16_string_length )
		{
			if( libuna_unicode_character_copy_from_utf16(
			     &unicode_character,
			     utf16_string,
			     utf16_string_length,
			     &utf16_string_index,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-16 string to Unicode character.",
				 function );

				goto on_error;
			}
			if( ( unicode_character == (libuna_unicode_character_t) LIBFSNTFS_SEPARATOR )
			 || ( unicode_character == 0 ) )
			{
				utf16_string_segment_length += 1;

				break;
			}
		}
		utf16_string_segment_length = utf16_string_index - utf16_string_segment_length;

		if( utf16_string_segment_length == 0 )
		{
			result = 0;
		}
		else
		{
			result = libfsntfs_directory_entries_tree_get_directory_entry_by_utf16_name(
			          directory_entries_tree,
				  utf16_string_segment,
				  utf16_string_segment_length,
			          &safe_directory_entry,
			          error );
		}
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory entry by name.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			break;
		}
		if( libfsntfs_directory_entry_get_mft_entry_index(
		     safe_directory_entry,
		     &mft_entry_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve MFT entry index.",
			 function );

			goto on_error;
		}
		if( libfsntfs_mft_get_mft_entry_by_index(
		     internal_volume->mft,
		     internal_volume->file_io_handle,
		     mft_entry_index,
		     mft_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve MFT entry: %" PRIu64 ".",
			 function,
			 mft_entry_index );

			goto on_error;
		}
	}
	if( result != 0 )
	{
		if( libfsntfs_directory_entry_clone(
		     directory_entry,
		     safe_directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create directory entry.",
			 function );

			goto on_error;
		}
	}
	if( directory_entries_tree != NULL )
	{
		if( libcdata_btree_free(
		     &directory_entries_tree,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_directory_entry_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free directory entries tree.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( directory_entries_tree != NULL )
	{
		libcdata_btree_free(
		 &directory_entries_tree,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_directory_entry_free,
		 NULL );
	}
	if( *directory_entry != NULL )
	{
		libfsntfs_directory_entry_free(
		 directory_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the file entry for an UTF-16 encoded path
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfsntfs_volume_get_file_entry_by_utf16_path(
     libfsntfs_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsntfs_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libfsntfs_directory_entry_t *directory_entry = NULL;
	libfsntfs_internal_volume_t *internal_volume = NULL;
	libfsntfs_mft_entry_t *mft_entry             = NULL;
	static char *function                        = "libfsntfs_volume_get_file_entry_by_utf16_path";
	int result                                   = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsntfs_internal_volume_t *) volume;

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
	result = libfsntfs_internal_volume_get_mft_and_directory_entry_by_utf16_path(
	          internal_volume,
	          utf16_string,
	          utf16_string_length,
	          &mft_entry,
	          &directory_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT and directory entry by path.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		/* libfsntfs_file_entry_initialize takes over management of directory_entry
		 */
		if( libfsntfs_file_entry_initialize(
		     file_entry,
		     internal_volume->io_handle,
		     internal_volume->file_io_handle,
		     internal_volume->mft,
		     internal_volume->security_descriptor_index,
		     mft_entry,
		     directory_entry,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file entry.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( directory_entry != NULL )
	{
		libfsntfs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	return( -1 );
}

/* Reads the bitmap file entry
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_internal_volume_read_bitmap(
     libfsntfs_internal_volume_t *internal_volume,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libfcache_cache_t *cluster_block_cache   = NULL;
	libfdata_vector_t *cluster_block_vector  = NULL;
	libfsntfs_cluster_block_t *cluster_block = NULL;
	libfsntfs_mft_entry_t *mft_entry         = NULL;
	static char *function                    = "libfsntfs_internal_volume_read_bitmap";
	off64_t bitmap_offset                    = 0;
	off64_t start_offset                     = 0;
	size_t cluster_block_data_offset         = 0;
	uint32_t value_32bit                     = 0;
	uint8_t bit_index                        = 0;
	int cluster_block_index                  = 0;
	int number_of_cluster_blocks             = 0;

	if( internal_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal volume.",
		 function );

		return( -1 );
	}
	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( libfsntfs_mft_get_mft_entry_by_index(
	     internal_volume->mft,
	     file_io_handle,
	     LIBFSNTFS_MFT_ENTRY_INDEX_BITMAP,
	     &mft_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT entry: %d.",
		 function,
		 LIBFSNTFS_MFT_ENTRY_INDEX_BITMAP );

		goto on_error;
	}
	if( mft_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing MFT entry: %d.",
		 function,
		 LIBFSNTFS_MFT_ENTRY_INDEX_BITMAP );

		goto on_error;
	}
	if( mft_entry->data_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid MFT entry: %d - missing data attribute.",
		 function,
		 LIBFSNTFS_MFT_ENTRY_INDEX_BITMAP );

		goto on_error;
	}
	if( libfsntfs_cluster_block_vector_initialize(
	     &cluster_block_vector,
	     internal_volume->io_handle,
	     mft_entry->data_attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create cluster block vector.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &cluster_block_cache,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create cluster block cache.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_get_number_of_elements(
	     cluster_block_vector,
	     &number_of_cluster_blocks,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of cluster blocks.",
		 function );

		goto on_error;
	}
	for( cluster_block_index = 0;
	     cluster_block_index < number_of_cluster_blocks;
	     cluster_block_index++ )
	{
		if( libfdata_vector_get_element_value_by_index(
		     cluster_block_vector,
		     (intptr_t *) file_io_handle,
		     cluster_block_cache,
		     cluster_block_index,
		     (intptr_t **) &cluster_block,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve cluster block: %d from vector.",
			 function,
			 cluster_block_index );

			goto on_error;
		}
		if( cluster_block == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing cluster block: %d.",
			 function,
			 cluster_block_index );

			goto on_error;
		}
		if( cluster_block->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid cluster block: %d - missing data.",
			 function,
			 cluster_block_index );

			goto on_error;
		}
		if( ( ( cluster_block->data_size % 4 ) != 0 )
		 || ( cluster_block->data_size > (size_t) SSIZE_MAX ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: cluster block: %d data size value out of bounds.",
			 function,
			 cluster_block_index );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: bitmap segment: %d data:\n",
			 function,
			 cluster_block_index );
			libcnotify_print_data(
			 cluster_block->data,
			 cluster_block->data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
		cluster_block_data_offset = 0;
		start_offset              = -1;

		while( cluster_block_data_offset < cluster_block->data_size )
		{
			byte_stream_copy_to_uint32_little_endian(
			 &( cluster_block->data[ cluster_block_data_offset ] ),
			 value_32bit );

			for( bit_index = 0;
			     bit_index < 32;
			     bit_index++ )
			{
				if( ( value_32bit & 0x00000001UL ) == 0 )
				{
					if( start_offset >= 0 )
					{
#if defined( HAVE_DEBUG_OUTPUT )
						if( libcnotify_verbose != 0 )
						{
							libcnotify_printf(
							 "%s: offset range\t\t: 0x%08" PRIx64 " - 0x%08" PRIx64 " (0x%08" PRIx64 ")\n",
							 function,
							 start_offset,
							 bitmap_offset,
							 bitmap_offset - start_offset );
						}
#endif
/*
						if( libfsntfs_offset_list_append_offset(
						     offset_list,
						     start_offset,
						     bitmap_offset - start_offset,
						     1,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
							 "%s: unable to append offset range to offset list.",
							 function );

							goto on_error;
						}
*/
						start_offset = -1;
					}
				}
				else
				{
					if( start_offset < 0 )
					{
						start_offset = bitmap_offset;
					}
				}
				bitmap_offset += internal_volume->io_handle->cluster_block_size;

				value_32bit >>= 1;
			}
			cluster_block_data_offset += 4;
		}
		if( start_offset >= 0 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: offset range\t\t: 0x%08" PRIx64 " - 0x%08" PRIx64 " (0x%08" PRIx64 ")\n",
				 function,
				 start_offset,
				 bitmap_offset,
				 bitmap_offset - start_offset );
			}
#endif
/* TODO
			if( libfsntfs_offset_list_append_offset(
			     offset_list,
			     start_offset,
			     bitmap_offset - start_offset,
			     1,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append offset range to offset list.",
				 function );

				goto on_error;
			}
*/
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "\n" );
		}
#endif
	}
	if( libfdata_vector_free(
	     &cluster_block_vector,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free cluster block vector.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_free(
	     &cluster_block_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free cluster block cache.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( cluster_block_cache != NULL )
	{
		libfcache_cache_free(
		 &cluster_block_cache,
		 NULL );
	}
	if( cluster_block_vector != NULL )
	{
		libfdata_vector_free(
		 &cluster_block_vector,
		 NULL );
	}
	return( -1 );
}

/* Reads the security descriptors file entry
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_internal_volume_read_security_descriptors(
     libfsntfs_internal_volume_t *internal_volume,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *data_attribute = NULL;
	libfsntfs_mft_entry_t *mft_entry      = NULL;
	static char *function                 = "libfsntfs_internal_volume_read_security_descriptors";

	if( internal_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal volume.",
		 function );

		return( -1 );
	}
	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( libfsntfs_mft_get_mft_entry_by_index(
	     internal_volume->mft,
	     file_io_handle,
	     LIBFSNTFS_MFT_ENTRY_INDEX_SECURE,
	     &mft_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT entry: %d.",
		 function,
		 LIBFSNTFS_MFT_ENTRY_INDEX_SECURE );

		goto on_error;
	}
	if( libfsntfs_mft_entry_get_alternate_data_attribute_by_utf8_name(
	     mft_entry,
	     (uint8_t *) "$SDS",
	     4,
	     &data_attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve $SDS data attribute.",
		 function );

		goto on_error;
	}
	if( libfsntfs_security_descriptor_index_initialize(
	     &( internal_volume->security_descriptor_index ),
	     internal_volume->io_handle,
	     file_io_handle,
	     data_attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create security descriptor index.",
		 function );

		goto on_error;
	}
	if( libfsntfs_security_descriptor_index_read_sii_index(
	     internal_volume->security_descriptor_index,
	     internal_volume->io_handle,
	     file_io_handle,
	     mft_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read security descriptor identifier ($SII) index.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( internal_volume->security_descriptor_index != NULL )
	{
		libfsntfs_security_descriptor_index_free(
		 &( internal_volume->security_descriptor_index ),
		 NULL );
	}
	return( -1 );
}

/* Retrieves the USN change journal
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_volume_get_usn_change_journal(
     libfsntfs_volume_t *volume,
     libfsntfs_usn_change_journal_t **usn_change_journal,
     libcerror_error_t **error )
{
	libfsntfs_attribute_t *data_attribute        = NULL;
	libfsntfs_directory_entry_t *directory_entry = NULL;
	libfsntfs_internal_volume_t *internal_volume = NULL;
	libfsntfs_mft_entry_t *mft_entry             = NULL;
	static char *function                        = "libfsntfs_volume_get_usn_change_journal";
	int result                                   = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfsntfs_internal_volume_t *) volume;

	result = libfsntfs_internal_volume_get_mft_and_directory_entry_by_utf8_path(
	          internal_volume,
	          (uint8_t *) "\\$Extend\\$UsnJrnl",
	          17,
	          &mft_entry,
	          &directory_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve \\$Extend\\$UsnJrnl MFT and directory entry by path.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	result = libfsntfs_mft_entry_get_alternate_data_attribute_by_utf8_name(
	          mft_entry,
	          (uint8_t *) "$J",
	          2,
	          &data_attribute,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve $J data attribute.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		if( libfsntfs_directory_entry_free(
		     &directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free directory entry.",
			 function );

			goto on_error;
		}
		return( 0 );
	}
	/* libfsntfs_usn_change_journal_initialize takes over management of directory_entry
	 */
	if( libfsntfs_usn_change_journal_initialize(
	     usn_change_journal,
	     internal_volume->io_handle,
	     internal_volume->file_io_handle,
	     directory_entry,
	     data_attribute,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create USN change journal.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( directory_entry != NULL )
	{
		libfsntfs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	return( -1 );
}


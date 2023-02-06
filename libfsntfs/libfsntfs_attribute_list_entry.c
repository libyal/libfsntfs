/*
 * Attribute list entry functions
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
#include <types.h>

#include "libfsntfs_attribute_list_entry.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcthreads.h"
#include "libfsntfs_mft_attribute_list_entry.h"
#include "libfsntfs_types.h"

/* Creates an attribute list entry
 * Make sure the value attribute_list_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_list_entry_initialize(
     libfsntfs_attribute_list_entry_t **attribute_list_entry,
     libfsntfs_mft_attribute_list_entry_t *mft_attribute_list_entry,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_list_entry_t *internal_attribute_list_entry = NULL;
	static char *function                                                    = "libfsntfs_attribute_list_entry_initialize";

	if( attribute_list_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list entry.",
		 function );

		return( -1 );
	}
	if( *attribute_list_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attribute list entry value already set.",
		 function );

		return( -1 );
	}
	if( mft_attribute_list_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT attribute list entry.",
		 function );

		return( -1 );
	}
	internal_attribute_list_entry = memory_allocate_structure(
	                                 libfsntfs_internal_attribute_list_entry_t );

	if( internal_attribute_list_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create attribute list entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_attribute_list_entry,
	     0,
	     sizeof( libfsntfs_internal_attribute_list_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear attribute list entry.",
		 function );

		memory_free(
		 internal_attribute_list_entry );

		return( -1 );
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_attribute_list_entry->read_write_lock ),
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
	internal_attribute_list_entry->mft_attribute_list_entry = mft_attribute_list_entry;

	*attribute_list_entry = (libfsntfs_attribute_list_entry_t *) internal_attribute_list_entry;

	return( 1 );

on_error:
	if( internal_attribute_list_entry != NULL )
	{
		memory_free(
		 internal_attribute_list_entry );
	}
	return( -1 );
}

/* Frees an attribute list entry
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_list_entry_free(
     libfsntfs_attribute_list_entry_t **attribute_list_entry,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_list_entry_t *internal_attribute_list_entry = NULL;
	static char *function                                                    = "libfsntfs_attribute_list_entry_free";
	int result                                                               = 1;

	if( attribute_list_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list entry.",
		 function );

		return( -1 );
	}
	if( *attribute_list_entry != NULL )
	{
		internal_attribute_list_entry = (libfsntfs_internal_attribute_list_entry_t *) *attribute_list_entry;
		*attribute_list_entry         = NULL;

		/* The mft_attribute_list_entry references is freed elsewhere
		 */
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_attribute_list_entry->read_write_lock ),
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
		memory_free(
		 internal_attribute_list_entry );
	}
	return( result );
}

/* Retrieves the attribute type
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_list_entry_get_attribute_type(
     libfsntfs_attribute_list_entry_t *attribute_list_entry,
     uint32_t *attribute_type,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_list_entry_t *internal_attribute_list_entry = NULL;
	static char *function                                                    = "libfsntfs_attribute_list_entry_get_type";
	int result                                                               = 1;

	if( attribute_list_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list entry.",
		 function );

		return( -1 );
	}
	internal_attribute_list_entry = (libfsntfs_internal_attribute_list_entry_t *) attribute_list_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_attribute_list_entry->read_write_lock,
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
	if( libfsntfs_mft_attribute_list_entry_get_attribute_type(
	     internal_attribute_list_entry->mft_attribute_list_entry,
	     attribute_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute type.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_attribute_list_entry->read_write_lock,
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

/* Retrieves the file reference
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_list_entry_get_file_reference(
     libfsntfs_attribute_list_entry_t *attribute_list_entry,
     uint64_t *file_reference,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_list_entry_t *internal_attribute_list_entry = NULL;
	static char *function                                                    = "libfsntfs_attribute_list_entry_get_type";
	int result                                                               = 1;

	if( attribute_list_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list entry.",
		 function );

		return( -1 );
	}
	internal_attribute_list_entry = (libfsntfs_internal_attribute_list_entry_t *) attribute_list_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_attribute_list_entry->read_write_lock,
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
	if( libfsntfs_mft_attribute_list_entry_get_file_reference(
	     internal_attribute_list_entry->mft_attribute_list_entry,
	     file_reference,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file reference.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_attribute_list_entry->read_write_lock,
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

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_list_entry_get_utf8_name_size(
     libfsntfs_attribute_list_entry_t *attribute_list_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_list_entry_t *internal_attribute_list_entry = NULL;
	static char *function                                                    = "libfsntfs_attribute_list_entry_get_utf8_name_size";
	int result                                                               = 1;

	if( attribute_list_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list entry.",
		 function );

		return( -1 );
	}
	internal_attribute_list_entry = (libfsntfs_internal_attribute_list_entry_t *) attribute_list_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_attribute_list_entry->read_write_lock,
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
	if( libfsntfs_mft_attribute_list_entry_get_utf8_name_size(
	     internal_attribute_list_entry->mft_attribute_list_entry,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name size from attribute list entry.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_attribute_list_entry->read_write_lock,
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

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_list_entry_get_utf8_name(
     libfsntfs_attribute_list_entry_t *attribute_list_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_list_entry_t *internal_attribute_list_entry = NULL;
	static char *function                                                    = "libfsntfs_attribute_list_entry_get_utf8_name";
	int result                                                               = 1;

	if( attribute_list_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list entry.",
		 function );

		return( -1 );
	}
	internal_attribute_list_entry = (libfsntfs_internal_attribute_list_entry_t *) attribute_list_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_attribute_list_entry->read_write_lock,
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
	if( libfsntfs_mft_attribute_list_entry_get_utf8_name(
	     internal_attribute_list_entry->mft_attribute_list_entry,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name from attribute list entry.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_attribute_list_entry->read_write_lock,
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

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_list_entry_get_utf16_name_size(
     libfsntfs_attribute_list_entry_t *attribute_list_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_list_entry_t *internal_attribute_list_entry = NULL;
	static char *function                                                    = "libfsntfs_attribute_list_entry_get_utf16_name_size";
	int result                                                               = 1;

	if( attribute_list_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list entry.",
		 function );

		return( -1 );
	}
	internal_attribute_list_entry = (libfsntfs_internal_attribute_list_entry_t *) attribute_list_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_attribute_list_entry->read_write_lock,
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
	if( libfsntfs_mft_attribute_list_entry_get_utf16_name_size(
	     internal_attribute_list_entry->mft_attribute_list_entry,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 name size from attribute list entry.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_attribute_list_entry->read_write_lock,
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

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_list_entry_get_utf16_name(
     libfsntfs_attribute_list_entry_t *attribute_list_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_list_entry_t *internal_attribute_list_entry = NULL;
	static char *function                                                    = "libfsntfs_attribute_list_entry_get_utf16_name";
	int result                                                               = 1;

	if( attribute_list_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute list entry.",
		 function );

		return( -1 );
	}
	internal_attribute_list_entry = (libfsntfs_internal_attribute_list_entry_t *) attribute_list_entry;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_attribute_list_entry->read_write_lock,
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
	if( libfsntfs_mft_attribute_list_entry_get_utf16_name(
	     internal_attribute_list_entry->mft_attribute_list_entry,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 name from attribute list entry.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_attribute_list_entry->read_write_lock,
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


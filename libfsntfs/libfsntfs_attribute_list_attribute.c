/*
 * Attribute list attribute ($ATTRIBUTE_LIST) functions
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

#include "libfsntfs_attribute.h"
#include "libfsntfs_attribute_list_attribute.h"
#include "libfsntfs_attribute_list_entry.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcthreads.h"
#include "libfsntfs_mft_attribute_list.h"
#include "libfsntfs_mft_attribute_list_entry.h"
#include "libfsntfs_types.h"

/* Retrieves the number of attribute list entries
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_list_attribute_get_number_of_entries(
     libfsntfs_attribute_t *attribute,
     int *number_of_entries,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_list_attribute_get_number_of_entries";
	uint32_t attribute_type                            = 0;
	int result                                         = 1;
	int safe_number_of_entries                         = 0;

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
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

	if( number_of_entries == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of entries.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_attribute->read_write_lock,
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
	if( libfsntfs_internal_attribute_get_type(
	     internal_attribute,
	     &attribute_type,
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
	else if( attribute_type != LIBFSNTFS_ATTRIBUTE_TYPE_ATTRIBUTE_LIST )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		result = -1;
	}
	else if( internal_attribute->value != NULL )
	{
		if( libfsntfs_mft_attribute_list_get_number_of_entries(
		     (libfsntfs_mft_attribute_list_t *) internal_attribute->value,
		     &safe_number_of_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of attribute list entries.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_attribute->read_write_lock,
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
	if( result == 1 )
	{
		*number_of_entries = safe_number_of_entries;
	}
	return( result );
}

/* Retrieves a specific attribute list entry
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_list_attribute_get_entry_by_index(
     libfsntfs_attribute_t *attribute,
     int entry_index,
     libfsntfs_attribute_list_entry_t **attribute_list_entry,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute             = NULL;
	libfsntfs_mft_attribute_list_entry_t *mft_attribute_list_entry = NULL;
	static char *function                                          = "libfsntfs_attribute_list_attribute_get_entry_by_index";
	uint32_t attribute_type                                        = 0;
	int result                                                     = 1;

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
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_attribute->read_write_lock,
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
	if( libfsntfs_internal_attribute_get_type(
	     internal_attribute,
	     &attribute_type,
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
	else if( attribute_type != LIBFSNTFS_ATTRIBUTE_TYPE_ATTRIBUTE_LIST )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported attribute type.",
		 function );

		result = -1;
	}
	else if( internal_attribute->value != NULL )
	{
		if( libfsntfs_mft_attribute_list_get_entry_by_index(
		     (libfsntfs_mft_attribute_list_t *) internal_attribute->value,
		     entry_index,
		     &mft_attribute_list_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute list entry: %d.",
			 function,
			 entry_index );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_attribute->read_write_lock,
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
	if( result == 1 )
	{
		if( libfsntfs_attribute_list_entry_initialize(
		     attribute_list_entry,
		     mft_attribute_list_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create attribute list entry: %d.",
			 function,
			 entry_index );

			result = -1;
		}
	}
	return( result );
}


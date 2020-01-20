/*
 * Data extent functions
 *
 * Copyright (C) 2010-2020, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfsntfs_data_extent.h"
#include "libfsntfs_libcerror.h"

/* Creates a data extent
 * Make sure the value data_extent is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_data_extent_initialize(
     libfsntfs_data_extent_t **data_extent,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_data_extent_initialize";

	if( data_extent == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data extent.",
		 function );

		return( -1 );
	}
	if( *data_extent != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data extent value already set.",
		 function );

		return( -1 );
	}
	*data_extent = memory_allocate_structure(
	                libfsntfs_data_extent_t );

	if( *data_extent == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data extent.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *data_extent,
	     0,
	     sizeof( libfsntfs_data_extent_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data extent.",
		 function );

		memory_free(
		 *data_extent );

		*data_extent = NULL;

		return( -1 );
	}
	return( 1 );

on_error:
	if( *data_extent != NULL )
	{
		memory_free(
		 *data_extent );

		*data_extent = NULL;
	}
	return( -1 );
}

/* Frees a data extent
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_data_extent_free(
     libfsntfs_data_extent_t **data_extent,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_data_extent_free";

	if( data_extent == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data extent.",
		 function );

		return( -1 );
	}
	if( *data_extent != NULL )
	{
		memory_free(
		 *data_extent );

		*data_extent = NULL;
	}
	return( 1 );
}

/* Retrieves the extent values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_data_extent_get_values(
     libfsntfs_data_extent_t *data_extent,
     off64_t *extent_offset,
     size64_t *extent_size,
     uint32_t *extent_flags,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_data_extent_get_values";

	if( data_extent == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data extent.",
		 function );

		return( -1 );
	}
	if( extent_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent offset.",
		 function );

		return( -1 );
	}
	if( extent_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent size.",
		 function );

		return( -1 );
	}
	if( extent_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent flags.",
		 function );

		return( -1 );
	}
	*extent_offset = data_extent->start_offset;
	*extent_size   = data_extent->size;
	*extent_flags  = data_extent->range_flags;

	return( 1 );
}


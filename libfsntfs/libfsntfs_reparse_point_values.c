/*
 * Reparse point attribute ($REPARSE_POINT) functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_reparse_point_values.h"

#include "fsntfs_reparse_point.h"

/* Creates reparse point values
 * Make sure the value reparse_point_values is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_reparse_point_values_initialize(
     libfsntfs_reparse_point_values_t **reparse_point_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_reparse_point_values_initialize";

	if( reparse_point_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid reparse point values.",
		 function );

		return( -1 );
	}
	if( *reparse_point_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid reparse point values value already set.",
		 function );

		return( -1 );
	}
	*reparse_point_values = memory_allocate_structure(
	                         libfsntfs_reparse_point_values_t );

	if( *reparse_point_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create reparse point values.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *reparse_point_values,
	     0,
	     sizeof( libfsntfs_reparse_point_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear reparse point values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *reparse_point_values != NULL )
	{
		memory_free(
		 *reparse_point_values );

		*reparse_point_values = NULL;
	}
	return( -1 );
}

/* Frees reparse point values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_reparse_point_values_free(
     libfsntfs_reparse_point_values_t **reparse_point_values,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_reparse_point_values_free";

	if( reparse_point_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid reparse point values.",
		 function );

		return( -1 );
	}
	if( *reparse_point_values != NULL )
	{
		memory_free(
		 *reparse_point_values );

		*reparse_point_values = NULL;
	}
	return( 1 );
}

/* Reads the reparse point values
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_reparse_point_values_read(
     libfsntfs_reparse_point_values_t *reparse_point_values,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_reparse_point_values_read";

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit  = 0;
	uint16_t value_16bit  = 0;
#endif

	if( reparse_point_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid reparse point values.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reparse point data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (fsntfs_reparse_point_t *) data )->type,
		 value_32bit );
		libcnotify_printf(
		 "%s: type and flags\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsntfs_reparse_point_t *) data )->data_size,
		 value_16bit );
		libcnotify_printf(
		 "%s: data size\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}


/*
 * $SDS index value functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_sds_index_value.h"

#include "fsntfs_secure.h"

/* Creates $SDS index value
 * Make sure the value sds_index_value is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_sds_index_value_initialize(
     libfsntfs_sds_index_value_t **sds_index_value,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_sds_index_value_initialize";

	if( sds_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid $SDS index value.",
		 function );

		return( -1 );
	}
	if( *sds_index_value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid $SDS index value value already set.",
		 function );

		return( -1 );
	}
	*sds_index_value = memory_allocate_structure(
	                     libfsntfs_sds_index_value_t );

	if( *sds_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create $SDS index value.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *sds_index_value,
	     0,
	     sizeof( libfsntfs_sds_index_value_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear $SDS index value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *sds_index_value != NULL )
	{
		memory_free(
		 *sds_index_value );

		*sds_index_value = NULL;
	}
	return( -1 );
}

/* Frees $SDS index value
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_sds_index_value_free(
     libfsntfs_sds_index_value_t **sds_index_value,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_sds_index_value_free";

	if( sds_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid $SDS index value.",
		 function );

		return( -1 );
	}
	if( *sds_index_value != NULL )
	{
		memory_free(
		 *sds_index_value );

		*sds_index_value = NULL;
	}
	return( 1 );
}

/* Reads the $SDS index value
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_sds_index_value_read_data(
     libfsntfs_sds_index_value_t *sds_index_value,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_sds_index_value_read_data";

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit  = 0;
	uint32_t value_32bit  = 0;
#endif

	if( sds_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid $SDS index value.",
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
		 "%s: $SDS index value:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
	if( data_size != sizeof( fsntfs_secure_index_value_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported $SDS index value size: %" PRIzd "\n",
		 function,
		 data_size );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (fsntfs_secure_index_value_t *) data )->hash,
		 value_32bit );
		libcnotify_printf(
		 "%s: hash\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsntfs_secure_index_value_t *) data )->identifier,
		 value_32bit );
		libcnotify_printf(
		 "%s: identifier\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsntfs_secure_index_value_t *) data )->data_offset,
		 value_64bit );
		libcnotify_printf(
		 "%s: data offset\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsntfs_secure_index_value_t *) data )->data_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: data size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}


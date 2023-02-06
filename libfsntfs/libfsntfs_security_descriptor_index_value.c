/*
 * Security descriptor index value functions
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

#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_security_descriptor_index_value.h"

#include "fsntfs_secure.h"

/* Creates security descriptor index value
 * Make sure the value security_descriptor_index_value is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_security_descriptor_index_value_initialize(
     libfsntfs_security_descriptor_index_value_t **security_descriptor_index_value,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_security_descriptor_index_value_initialize";

	if( security_descriptor_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid security descriptor index value.",
		 function );

		return( -1 );
	}
	if( *security_descriptor_index_value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid security descriptor index value already set.",
		 function );

		return( -1 );
	}
	*security_descriptor_index_value = memory_allocate_structure(
	                                    libfsntfs_security_descriptor_index_value_t );

	if( *security_descriptor_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create security descriptor index value.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *security_descriptor_index_value,
	     0,
	     sizeof( libfsntfs_security_descriptor_index_value_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear security descriptor index value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *security_descriptor_index_value != NULL )
	{
		memory_free(
		 *security_descriptor_index_value );

		*security_descriptor_index_value = NULL;
	}
	return( -1 );
}

/* Frees security descriptor index value
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_security_descriptor_index_value_free(
     libfsntfs_security_descriptor_index_value_t **security_descriptor_index_value,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_security_descriptor_index_value_free";

	if( security_descriptor_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid security descriptor index value.",
		 function );

		return( -1 );
	}
	if( *security_descriptor_index_value != NULL )
	{
		memory_free(
		 *security_descriptor_index_value );

		*security_descriptor_index_value = NULL;
	}
	return( 1 );
}

/* Compares 2 security descriptor index value by the identifier
 * Returns LIBCDATA_COMPARE_LESS, LIBCDATA_COMPARE_EQUAL, LIBCDATA_COMPARE_GREATER if successful or -1 on error
 */
int libfsntfs_security_descriptor_index_value_compare(
     libfsntfs_security_descriptor_index_value_t *first_security_descriptor_index_value,
     libfsntfs_security_descriptor_index_value_t *second_security_descriptor_index_value,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_security_descriptor_index_value_compare";

	if( first_security_descriptor_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid first security descriptor index value.",
		 function );

		return( -1 );
	}
	if( second_security_descriptor_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid second security descriptor index value.",
		 function );

		return( -1 );
	}
	if( first_security_descriptor_index_value->identifier < second_security_descriptor_index_value->identifier )
	{
		return( LIBCDATA_COMPARE_LESS );
	}
	else if( first_security_descriptor_index_value->identifier > second_security_descriptor_index_value->identifier )
	{
		return( LIBCDATA_COMPARE_GREATER );
	}
	return( LIBCDATA_COMPARE_EQUAL );
}

/* Reads the security descriptor index value
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_security_descriptor_index_value_read_data(
     libfsntfs_security_descriptor_index_value_t *security_descriptor_index_value,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_security_descriptor_index_value_read_data";

	if( security_descriptor_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid security descriptor index value.",
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
		 "%s: security descriptor index value data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( data_size != sizeof( fsntfs_secure_index_value_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported security descriptor index value data size: %" PRIzd "\n",
		 function,
		 data_size );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsntfs_secure_index_value_t *) data )->hash,
	 security_descriptor_index_value->hash );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsntfs_secure_index_value_t *) data )->identifier,
	 security_descriptor_index_value->identifier );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fsntfs_secure_index_value_t *) data )->data_offset,
	 security_descriptor_index_value->data_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fsntfs_secure_index_value_t *) data )->data_size,
	 security_descriptor_index_value->data_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: hash\t\t: 0x%08" PRIx32 "\n",
		 function,
		 security_descriptor_index_value->hash );

		libcnotify_printf(
		 "%s: identifier\t\t\t: %" PRIu32 "\n",
		 function,
		 security_descriptor_index_value->identifier );

		libcnotify_printf(
		 "%s: data offset\t: 0x%08" PRIx64 "\n",
		 function,
		 security_descriptor_index_value->data_offset );

		libcnotify_printf(
		 "%s: data size\t\t\t: %" PRIu32 "\n",
		 function,
		 security_descriptor_index_value->data_size );

		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}


/*
 * Path hint functions
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

#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libuna.h"
#include "libfsntfs_path_hint.h"

/* Creates a path hint
 * Make sure the value path_hint is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_path_hint_initialize(
     libfsntfs_path_hint_t **path_hint,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_path_hint_initialize";

	if( path_hint == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path hint.",
		 function );

		return( -1 );
	}
	if( *path_hint != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid path hint value already set.",
		 function );

		return( -1 );
	}
	*path_hint = memory_allocate_structure(
	              libfsntfs_path_hint_t );

	if( *path_hint == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create path hint.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *path_hint,
	     0,
	     sizeof( libfsntfs_path_hint_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear path hint.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *path_hint != NULL )
	{
		memory_free(
		 *path_hint );

		*path_hint = NULL;
	}
	return( -1 );
}

/* Frees a path hint
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_path_hint_free(
     libfsntfs_path_hint_t **path_hint,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_path_hint_free";

	if( path_hint == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path hint.",
		 function );

		return( -1 );
	}
	if( *path_hint != NULL )
	{
		if( ( *path_hint )->path != NULL )
		{
			memory_free(
			 ( *path_hint )->path );
		}
		memory_free(
		 *path_hint );

		*path_hint = NULL;
	}
	return( 1 );
}

/* Compares 2 path hints by file reference
 * Returns LIBCDATA_COMPARE_LESS, LIBCDATA_COMPARE_EQUAL, LIBCDATA_COMPARE_GREATER if successful or -1 on error
 */
int libfsntfs_path_hint_compare_by_file_reference(
     libfsntfs_path_hint_t *first_path_hint,
     libfsntfs_path_hint_t *second_path_hint,
     libcerror_error_t **error )
{
	static char *function           = "libfsntfs_path_hint_compare_by_file_reference";
	uint64_t first_mft_entry_index  = 0;
	uint64_t second_mft_entry_index = 0;
	uint16_t first_sequence_number  = 0;
	uint16_t second_sequence_number = 0;

	if( first_path_hint == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid first path hint.",
		 function );

		return( -1 );
	}
	if( second_path_hint == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid second path hint.",
		 function );

		return( -1 );
	}
	first_mft_entry_index  = first_path_hint->file_reference & 0xffffffffffffUL;
	second_mft_entry_index = second_path_hint->file_reference & 0xffffffffffffUL;

	if( first_mft_entry_index < second_mft_entry_index )
	{
		return( LIBCDATA_COMPARE_LESS );
	}
	else if( first_mft_entry_index > second_mft_entry_index )
	{
		return( LIBCDATA_COMPARE_GREATER );
	}
	first_sequence_number  = (uint16_t) ( first_path_hint->file_reference >> 48 );
	second_sequence_number = (uint16_t) ( second_path_hint->file_reference >> 48 );

	if( first_sequence_number < second_sequence_number )
	{
		return( LIBCDATA_COMPARE_LESS );
	}
	else if( first_sequence_number > second_sequence_number )
	{
		return( LIBCDATA_COMPARE_GREATER );
	}
	return( LIBCDATA_COMPARE_EQUAL );
}

/* Retrieves the size of the UTF-8 encoded path
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_path_hint_get_utf8_path_size(
     libfsntfs_path_hint_t *path_hint,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_path_hint_get_utf8_path_size";

	if( path_hint == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file path values.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_size_from_utf8_stream(
	     path_hint->path,
	     path_hint->path_size,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded path
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_path_hint_get_utf8_path(
     libfsntfs_path_hint_t *path_hint,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_path_hint_get_utf8_path";

	if( path_hint == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file path values.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_copy_from_utf8_stream(
	     utf8_string,
	     utf8_string_size,
	     path_hint->path,
	     path_hint->path_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded path
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_path_hint_get_utf16_path_size(
     libfsntfs_path_hint_t *path_hint,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_path_hint_get_utf16_path_size";

	if( path_hint == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file path values.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_size_from_utf8_stream(
	     path_hint->path,
	     path_hint->path_size,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded path
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_path_hint_get_utf16_path(
     libfsntfs_path_hint_t *path_hint,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_path_hint_get_utf16_path";

	if( path_hint == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file path values.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_copy_from_utf8_stream(
	     utf16_string,
	     utf16_string_size,
	     path_hint->path,
	     path_hint->path_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}


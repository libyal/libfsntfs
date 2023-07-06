/*
 * Directory entry functions
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

#include "libfsntfs_directory_entry.h"
#include "libfsntfs_file_name_values.h"
#include "libfsntfs_index_value.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"

/* Creates a directory entry
 * Make sure the value directory_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_directory_entry_initialize(
     libfsntfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_directory_entry_initialize";

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
	if( *directory_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory entry value already set.",
		 function );

		return( -1 );
	}
	*directory_entry = memory_allocate_structure(
	                    libfsntfs_directory_entry_t );

	if( *directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create directory entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *directory_entry,
	     0,
	     sizeof( libfsntfs_directory_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear directory entry.",
		 function );

		memory_free(
		 *directory_entry );

		*directory_entry = NULL;

		return( -1 );
	}
	return( 1 );

on_error:
	if( *directory_entry != NULL )
	{
		memory_free(
		 *directory_entry );

		*directory_entry = NULL;
	}
	return( -1 );
}

/* Frees a directory entry
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_directory_entry_free(
     libfsntfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_directory_entry_free";
	int result            = 1;

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
	if( *directory_entry != NULL )
	{
		if( ( *directory_entry )->file_name_values != NULL )
		{
			if( libfsntfs_file_name_values_free(
			     &( ( *directory_entry )->file_name_values ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free file name values.",
				 function );

				result = -1;
			}
		}
		if( ( *directory_entry )->short_file_name_values != NULL )
		{
			if( libfsntfs_file_name_values_free(
			     &( ( *directory_entry )->short_file_name_values ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free short file name values.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *directory_entry );

		*directory_entry = NULL;
	}
	return( result );
}

/* Clones a directory entry
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_directory_entry_clone(
     libfsntfs_directory_entry_t **destination_directory_entry,
     libfsntfs_directory_entry_t *source_directory_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_directory_entry_clone";

	if( destination_directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( *destination_directory_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination directory entry value already set.",
		 function );

		return( -1 );
	}
	if( source_directory_entry == NULL )
	{
		*destination_directory_entry = source_directory_entry;

		return( 1 );
	}
	if( libfsntfs_directory_entry_initialize(
	     destination_directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination directory entry.",
		 function );

		goto on_error;
	}
	if( libfsntfs_file_name_values_clone(
	     &( ( *destination_directory_entry )->file_name_values ),
	     source_directory_entry->file_name_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination file name values.",
		 function );

		goto on_error;
	}
	if( libfsntfs_file_name_values_clone(
	     &( ( *destination_directory_entry )->short_file_name_values ),
	     source_directory_entry->short_file_name_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination short file name values.",
		 function );

		goto on_error;
	}
	( *destination_directory_entry )->file_reference = source_directory_entry->file_reference;

	return( 1 );

on_error:
	if( *destination_directory_entry != NULL )
	{
		libfsntfs_directory_entry_free(
		 destination_directory_entry,
		 NULL );
	}
	return( -1 );
}

/* Compares 2 directory entries by file reference
 * Returns LIBCDATA_COMPARE_LESS, LIBCDATA_COMPARE_EQUAL, LIBCDATA_COMPARE_GREATER if successful or -1 on error
 */
int libfsntfs_directory_entry_compare_by_file_reference(
     libfsntfs_directory_entry_t *first_directory_entry,
     libfsntfs_directory_entry_t *second_directory_entry,
     libcerror_error_t **error )
{
	static char *function           = "libfsntfs_directory_entry_compare_by_file_reference";
	uint64_t first_mft_entry_index  = 0;
	uint64_t second_mft_entry_index = 0;
	uint16_t first_sequence_number  = 0;
	uint16_t second_sequence_number = 0;

	if( first_directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid first directory entry.",
		 function );

		return( -1 );
	}
	if( second_directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid second directory entry.",
		 function );

		return( -1 );
	}
	first_mft_entry_index  = first_directory_entry->file_reference & 0xffffffffffffUL;
	second_mft_entry_index = second_directory_entry->file_reference & 0xffffffffffffUL;

	if( first_mft_entry_index < second_mft_entry_index )
	{
		return( LIBCDATA_COMPARE_LESS );
	}
	else if( first_mft_entry_index > second_mft_entry_index )
	{
		return( LIBCDATA_COMPARE_GREATER );
	}
	first_sequence_number  = (uint16_t) ( first_directory_entry->file_reference >> 48 );
	second_sequence_number = (uint16_t) ( second_directory_entry->file_reference >> 48 );

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

/* Retrieves the MFT entry index
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_directory_entry_get_mft_entry_index(
     libfsntfs_directory_entry_t *directory_entry,
     uint64_t *mft_entry_index,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_directory_entry_get_mft_entry_index";

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
	if( mft_entry_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT entry index.",
		 function );

		return( -1 );
	}
	if( ( directory_entry->file_reference & 0xffffffffffffUL ) > (uint64_t) INT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid MFT entry index value out of bounds.",
		 function );

		return( -1 );
	}
	*mft_entry_index = (int) ( directory_entry->file_reference & 0xffffffffffffUL );

	return( 1 );
}

/* Retrieves the file reference
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_directory_entry_get_file_reference(
     libfsntfs_directory_entry_t *directory_entry,
     uint64_t *file_reference,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_directory_entry_get_file_reference";

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
	if( file_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file reference.",
		 function );

		return( -1 );
	}
	*file_reference = directory_entry->file_reference;

	return( 1 );
}

/* Retrieves the parent file reference
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_directory_entry_get_parent_file_reference(
     libfsntfs_directory_entry_t *directory_entry,
     uint64_t *parent_file_reference,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_directory_entry_get_parent_file_reference";

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
	if( libfsntfs_file_name_values_get_parent_file_reference(
	     directory_entry->file_name_values,
	     parent_file_reference,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve parent reference from file name values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * This function uses UTF-8 RFC 2279 (or 6-byte UTF-8) to support characters outside Unicode
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_directory_entry_get_utf8_name_size(
     libfsntfs_directory_entry_t *directory_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_directory_entry_get_utf8_name_size";

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
	if( libfsntfs_file_name_values_get_utf8_name_size(
	     directory_entry->file_name_values,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-8 name from file name values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded name
 * This function uses UTF-8 RFC 2279 (or 6-byte UTF-8) to support characters outside Unicode
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_directory_entry_get_utf8_name(
     libfsntfs_directory_entry_t *directory_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_directory_entry_get_utf8_name";

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
	if( libfsntfs_file_name_values_get_utf8_name(
	     directory_entry->file_name_values,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name from file name values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded name
 * This function uses UCS-2 (with surrogates) to support characters outside Unicode
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_directory_entry_get_utf16_name_size(
     libfsntfs_directory_entry_t *directory_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_directory_entry_get_utf16_name_size";

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
	if( libfsntfs_file_name_values_get_utf16_name_size(
	     directory_entry->file_name_values,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-16 name from file name values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded name
 * This function uses UCS-2 (with surrogates) to support characters outside Unicode
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_directory_entry_get_utf16_name(
     libfsntfs_directory_entry_t *directory_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_directory_entry_get_utf16_name";

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
	if( libfsntfs_file_name_values_get_utf16_name(
	     directory_entry->file_name_values,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 name from file name values.",
		 function );

		return( -1 );
	}
	return( 1 );
}


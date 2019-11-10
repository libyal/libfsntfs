/*
 * Directory entries tree functions
 *
 * Copyright (C) 2010-2019, Joachim Metz <joachim.metz@gmail.com>
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
#include <types.h>

#if defined( HAVE_WCTYPE_H )
#include <wctype.h>
#endif

#include "libfsntfs_directory_entry.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libuna.h"
#include "libfsntfs_name.h"

/* Retrieves the directory entry for an UTF-8 encoded name
 * Returns 1 if successful, 0 if no such directory entry or -1 on error
 */
int libfsntfs_directory_entries_tree_get_directory_entry_by_utf8_name(
     libcdata_btree_t *directory_entries_tree,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsntfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	static char *function           = "libfsntfs_directory_entries_tree_get_directory_entry_by_utf8_name";
	int directory_entry_index       = 0;
	int number_of_directory_entries = 0;
	int result                      = 0;

	if( directory_entries_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entries tree.",
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
	if( libcdata_btree_get_number_of_values(
	     directory_entries_tree,
	     &number_of_directory_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of directory entries from tree.",
		 function );

		goto on_error;
	}
	for( directory_entry_index = 0;
	     directory_entry_index < number_of_directory_entries;
	     directory_entry_index++ )
	{
		if( libcdata_btree_get_value_by_index(
		     directory_entries_tree,
		     directory_entry_index,
		     (intptr_t **) directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory entry: %d from tree.",
			 function,
			 directory_entry_index );

			goto on_error;
		}
		if( *directory_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing directory entry.",
			 function );

			goto on_error;
		}
		if( ( *directory_entry )->file_name_values != NULL )
		{
			if( ( *directory_entry )->file_name_values->name == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid directory entry - invalid file name values - missing name.",
				 function );

				goto on_error;
			}
			result = libfsntfs_name_compare_with_utf8_string(
				  ( *directory_entry )->file_name_values->name,
				  ( *directory_entry )->file_name_values->name_size,
				  utf8_string,
				  utf8_string_length,
				  1,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to compare UTF-8 string with file name values.",
				 function );

				goto on_error;
			}
			else if( result == LIBUNA_COMPARE_EQUAL )
			{
				return( 1 );
			}
		}
		if( ( *directory_entry )->short_file_name_values != NULL )
		{
			if( ( *directory_entry )->short_file_name_values->name == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid directory entry - invalid short file name values - missing name.",
				 function );

				goto on_error;
			}
			result = libfsntfs_name_compare_with_utf8_string(
				  ( *directory_entry )->short_file_name_values->name,
				  ( *directory_entry )->short_file_name_values->name_size,
				  utf8_string,
				  utf8_string_length,
				  1,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to compare UTF-8 string with short file name values.",
				 function );

				goto on_error;
			}
			else if( result == LIBUNA_COMPARE_EQUAL )
			{
				return( 1 );
			}
		}
	}
	*directory_entry = NULL;

	return( 0 );

on_error:
	*directory_entry = NULL;

	return( -1 );
}

/* Retrieves the directory entry for an UTF-16 encoded name
 * Returns 1 if successful, 0 if no such directory entry or -1 on error
 */
int libfsntfs_directory_entries_tree_get_directory_entry_by_utf16_name(
     libcdata_btree_t *directory_entries_tree,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsntfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	static char *function           = "libfsntfs_directory_entries_tree_get_directory_entry_by_utf16_name";
	int directory_entry_index       = 0;
	int number_of_directory_entries = 0;
	int result                      = 0;

	if( directory_entries_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entries tree.",
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
	if( libcdata_btree_get_number_of_values(
	     directory_entries_tree,
	     &number_of_directory_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of directory entries from tree.",
		 function );

		goto on_error;
	}
	for( directory_entry_index = 0;
	     directory_entry_index < number_of_directory_entries;
	     directory_entry_index++ )
	{
		if( libcdata_btree_get_value_by_index(
		     directory_entries_tree,
		     directory_entry_index,
		     (intptr_t **) directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory entry: %d from tree.",
			 function,
			 directory_entry_index );

			goto on_error;
		}
		if( *directory_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing directory entry.",
			 function );

			goto on_error;
		}
		if( ( *directory_entry )->file_name_values != NULL )
		{
			if( ( *directory_entry )->file_name_values->name == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid directory entry - invalid file name values - missing name.",
				 function );

				goto on_error;
			}
			result = libfsntfs_name_compare_with_utf16_string(
				  ( *directory_entry )->file_name_values->name,
				  ( *directory_entry )->file_name_values->name_size,
				  utf16_string,
				  utf16_string_length,
				  1,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to compare UTF-16 string with file name values.",
				 function );

				goto on_error;
			}
			else if( result == LIBUNA_COMPARE_EQUAL )
			{
				return( 1 );
			}
		}
		if( ( *directory_entry )->short_file_name_values != NULL )
		{
			if( ( *directory_entry )->short_file_name_values->name == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid directory entry - invalid short file name values - missing name.",
				 function );

				goto on_error;
			}
			result = libfsntfs_name_compare_with_utf16_string(
				  ( *directory_entry )->short_file_name_values->name,
				  ( *directory_entry )->short_file_name_values->name_size,
				  utf16_string,
				  utf16_string_length,
				  1,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to compare UTF-16 string with short file name values.",
				 function );

				goto on_error;
			}
			else if( result == LIBUNA_COMPARE_EQUAL )
			{
				return( 1 );
			}
		}
	}
	*directory_entry = NULL;

	return( 0 );

on_error:
	*directory_entry = NULL;

	return( -1 );
}


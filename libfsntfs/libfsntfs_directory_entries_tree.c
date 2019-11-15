/*
 * Directory entries tree functions
 *
 * Copyright (C) 2010-2019, Joachim Metz <joachim.metz@gmail.com>
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
#include <types.h>

#include "libfsntfs_definitions.h"
#include "libfsntfs_directory_entry.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_mft_entry.h"
#include "libfsntfs_name.h"

/* Reads the MFT entry directory entry index if available
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_directory_entries_tree_read_from_mft_entry(
     libcdata_btree_t *directory_entries_tree,
     libfsntfs_mft_entry_t *mft_entry,
     libbfio_handle_t *file_io_handle,
     uint8_t flags,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *upper_node                      = NULL;
	libfsntfs_directory_entry_t *directory_entry          = NULL;
	libfsntfs_directory_entry_t *existing_directory_entry = NULL;
	libfsntfs_file_name_values_t *file_name_values        = NULL;
	libfsntfs_index_value_t *index_value                  = NULL;
	static char *function                                 = "libfsntfs_directory_entries_tree_read_from_mft_entry";
	uint8_t file_name_namespace                           = 0;
	int index_value_entry                                 = 0;
	int number_of_index_values                            = 0;
	int result                                            = 0;
	int value_index                                       = 0;

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
	if( ( flags & LIBFSNTFS_FILE_ENTRY_FLAGS_MFT_ONLY ) != 0 )
	{
		return( 1 );
	}
	if( mft_entry->i30_index == NULL )
	{
		return( 1 );
	}
	if( libfsntfs_index_read(
	     mft_entry->i30_index,
	     file_io_handle,
	     flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read $I30 index.",
		 function );

		goto on_error;
	}
	if( libfsntfs_index_get_number_of_index_values(
	     mft_entry->i30_index,
	     &number_of_index_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of $I30 index values.",
		 function );

		goto on_error;
	}
	for( index_value_entry = 0;
	     index_value_entry < number_of_index_values;
	     index_value_entry++ )
	{
		if( libfsntfs_index_get_index_value_by_index(
		     mft_entry->i30_index,
		     file_io_handle,
		     index_value_entry,
		     &index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve $I30 index value: %d.",
			 function,
			 index_value_entry );

			goto on_error;
		}
		if( libfsntfs_file_name_values_initialize(
		     &file_name_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file name values.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: index value: %03d file reference: MFT entry: %" PRIu64 ", sequence: %" PRIu64 "\n",
			 function,
			 index_value_entry,
			 index_value->file_reference & 0xffffffffffffUL,
			 index_value->file_reference >> 48 );
			libcnotify_printf(
			 "\n" );
		}
#endif
		if( libfsntfs_file_name_values_read_data(
		     file_name_values,
		     index_value->key_data,
		     (size_t) index_value->key_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read file name values.",
			 function );

			goto on_error;
		}
		if( file_name_values->name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid file name values - missing name.",
			 function );

			goto on_error;
		}
		/* Ignore the file name with the . as its name
		 */
		if( ( file_name_values->name_size == 2 )
		 && ( file_name_values->name[ 0 ] == 0x2e )
		 && ( file_name_values->name[ 1 ] == 0x00 ) )
		{
			if( libfsntfs_file_name_values_free(
			     &file_name_values,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free file name values.",
				 function );

				goto on_error;
			}
			continue;
		}
		file_name_namespace = file_name_values->name_namespace;

		if( libfsntfs_directory_entry_initialize(
		     &directory_entry,
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
		directory_entry->file_reference = index_value->file_reference;

		if( file_name_namespace == LIBFSNTFS_FILE_NAME_NAMESPACE_DOS )
		{
			directory_entry->short_file_name_values = file_name_values;
		}
		else
		{
			directory_entry->file_name_values = file_name_values;
		}
		file_name_values = NULL;

		result = libcdata_btree_insert_value(
			  directory_entries_tree,
			  &value_index,
			  (intptr_t *) directory_entry,
			  (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libfsntfs_directory_entry_compare,
			  &upper_node,
			  (intptr_t **) &existing_directory_entry,
			  error ) ;

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to insert directory entry into tree.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			if( existing_directory_entry == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing existing directory entry.",
				 function );

				goto on_error;
			}
			if( file_name_namespace == LIBFSNTFS_FILE_NAME_NAMESPACE_DOS )
			{
				if( existing_directory_entry->short_file_name_values == NULL )
				{
					existing_directory_entry->short_file_name_values = directory_entry->short_file_name_values;
					directory_entry->short_file_name_values          = NULL;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				else if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: short file name of existing directory entry already set.\n",
					 function );
				}
#endif
			}
			else
			{
				if( existing_directory_entry->file_name_values == NULL )
				{
					existing_directory_entry->file_name_values = directory_entry->file_name_values;
					directory_entry->file_name_values          = NULL;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				else if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: file name of existing directory entry already set.\n",
					 function );
				}
#endif
			}
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
		}
		directory_entry = NULL;
	}
	return( 1 );

on_error:
	if( directory_entry != NULL )
	{
		libfsntfs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	if( file_name_values != NULL )
	{
		libfsntfs_file_name_values_free(
		 &file_name_values,
		 NULL );
	}
	return( -1 );
}

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


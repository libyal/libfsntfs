/*
 * MFT functions
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

#include "libfsntfs_definitions.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_mft.h"
#include "libfsntfs_mft_attribute_list.h"
#include "libfsntfs_mft_entry.h"
#include "libfsntfs_types.h"

/* Creates a MFT
 * Make sure the value mft is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_initialize(
     libfsntfs_mft_t **mft,
     libfsntfs_io_handle_t *io_handle,
     size64_t mft_entry_size,
     uint8_t flags,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_initialize";

	if( mft == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT.",
		 function );

		return( -1 );
	}
	if( *mft != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid MFT value already set.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	*mft = memory_allocate_structure(
	        libfsntfs_mft_t );

	if( *mft == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create MFT.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *mft,
	     0,
	     sizeof( libfsntfs_mft_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear MFT.",
		 function );

		memory_free(
		 *mft );

		*mft = NULL;

		return( -1 );
	}
	if( libfdata_vector_initialize(
	     &( ( *mft )->mft_entry_vector ),
	     mft_entry_size,
	     (intptr_t *) io_handle,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_vector_t *, libfdata_cache_t *, int, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsntfs_mft_entry_read_element_data,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create MFT entry vector.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( ( *mft )->mft_entry_cache ),
	     LIBFSNTFS_MAXIMUM_CACHE_ENTRIES_MFT_ENTRIES,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create MFT entry cache.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( ( *mft )->single_mft_entry_cache ),
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create signle MFT entry cache.",
		 function );

		goto on_error;
	}
	( *mft )->io_handle = io_handle;
	( *mft )->flags     = flags;

	return( 1 );

on_error:
	if( *mft != NULL )
	{
		if( ( *mft )->mft_entry_cache != NULL )
		{
			libfcache_cache_free(
			 &( ( *mft )->mft_entry_cache ),
			 NULL );
		}
		if( ( *mft )->mft_entry_vector != NULL )
		{
			libfdata_vector_free(
			 &( ( *mft )->mft_entry_vector ),
			 NULL );
		}
		memory_free(
		 *mft );

		*mft = NULL;
	}
	return( -1 );
}

/* Frees a MFT
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_free(
     libfsntfs_mft_t **mft,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_free";
	int result            = 1;

	if( mft == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT.",
		 function );

		return( -1 );
	}
	if( *mft != NULL )
	{
		if( libfdata_vector_free(
		     &( ( *mft )->mft_entry_vector ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free MFT entry vector.",
			 function );

			result = -1;
		}
		if( libfcache_cache_free(
		     &( ( *mft )->mft_entry_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free MFT entry cache.",
			 function );

			result = -1;
		}
		if( libfcache_cache_free(
		     &( ( *mft )->single_mft_entry_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free single MFT entry cache.",
			 function );

			result = -1;
		}
		if( ( *mft )->attribute_list_tree != NULL )
		{
			if( libcdata_btree_free(
			     &( ( *mft )->attribute_list_tree ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_mft_attribute_list_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free attribute list tree.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *mft );

		*mft = NULL;
	}
	return( result );
}

/* Reads the attribute list data MFT entries
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_read_list_data_mft_entries(
     libfsntfs_mft_t *mft,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *upper_node                        = NULL;
	libfsntfs_mft_attribute_list_t *attribute_list          = NULL;
	libfsntfs_mft_attribute_list_t *existing_attribute_list = NULL;
	libfsntfs_mft_entry_t *mft_entry                        = NULL;
	static char *function                                   = "libfsntfs_mft_read_list_data_mft_entries";
	uint64_t base_record_file_reference                     = 0;
	uint64_t file_reference                                 = 0;
	uint64_t mft_entry_index                                = 0;
	int result                                              = 0;
	int value_index                                         = 0;

	if( mft == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT.",
		 function );

		return( -1 );
	}
	if( mft->attribute_list_tree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid MFT - attribute list tree value already set.",
		 function );

		return( -1 );
	}
	if( libcdata_btree_initialize(
	     &( mft->attribute_list_tree ),
	     LIBFSNTFS_INDEX_TREE_MAXIMUM_NUMBER_OF_SUB_NODES,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create attribute list B-tree.",
		 function );

		goto on_error;
	}
	for( mft_entry_index = 0;
	     mft_entry_index < mft->number_of_mft_entries;
	     mft_entry_index++ )
	{
		if( libfdata_vector_get_element_value_by_index(
		     mft->mft_entry_vector,
		     (intptr_t *) file_io_handle,
		     (libfdata_cache_t *) mft->mft_entry_cache,
		     (int) mft_entry_index,
		     (intptr_t **) &mft_entry,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve MFT entry: %" PRIu64 ".",
			 function,
			 mft_entry_index );

			goto on_error;
		}
		result = libfsntfs_mft_entry_get_base_record_file_reference(
		          mft_entry,
		          &base_record_file_reference,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve base record file reference from MFT entry: %" PRIu64 ".",
			 function,
			 mft_entry_index );

			goto on_error;
		}
		if( ( result == 0 )
		 || ( base_record_file_reference == 0 ) )
		{
			continue;
		}
		if( libfsntfs_mft_attribute_list_initialize(
		     &attribute_list,
		     base_record_file_reference,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create attribute list.",
			 function );

			goto on_error;
		}
		result = libcdata_btree_insert_value(
			  mft->attribute_list_tree,
			  &value_index,
			  (intptr_t *) attribute_list,
			  (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libfsntfs_mft_attribute_list_compare_by_base_record_file_reference,
			  &upper_node,
			  (intptr_t **) &existing_attribute_list,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to insert attribute list into tree.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			if( libfsntfs_mft_attribute_list_free(
			     &attribute_list,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free attribute list.",
				 function );

				goto on_error;
			}
		}
		else
		{
			existing_attribute_list = attribute_list;

			attribute_list = NULL;
		}
		if( libfsntfs_mft_entry_get_file_reference(
		     mft_entry,
		     &file_reference,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file reference from MFT entry: %" PRIu64 ".",
			 function,
			 mft_entry_index );

			goto on_error;
		}
		if( libfsntfs_mft_attribute_list_insert_file_reference(
		     existing_attribute_list,
		     file_reference,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to insert attribute list data file reference in attribute list.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( attribute_list != NULL )
	{
		libfsntfs_mft_attribute_list_free(
		 &attribute_list,
		 NULL );
	}
	if( mft->attribute_list_tree != NULL )
	{
		libcdata_btree_free(
		 &( mft->attribute_list_tree ),
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_mft_attribute_list_free,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the number of MFT entries
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_get_number_of_entries(
     libfsntfs_mft_t *mft,
     uint64_t *number_of_entries,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_mft_get_number_of_entries";

	if( mft == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT.",
		 function );

		return( -1 );
	}
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
	*number_of_entries = mft->number_of_mft_entries;

	return( 1 );
}

/* Retrieves the MFT entry for a specific index
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_get_mft_entry_by_index(
     libfsntfs_mft_t *mft,
     libbfio_handle_t *file_io_handle,
     uint64_t mft_entry_index,
     libfsntfs_mft_entry_t **mft_entry,
     libcerror_error_t **error )
{
	libfsntfs_mft_entry_t *safe_mft_entry = NULL;
	static char *function                 = "libfsntfs_mft_get_mft_entry_by_index";

	if( mft == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT.",
		 function );

		return( -1 );
	}
	if( mft_entry_index > mft->number_of_mft_entries )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid MFT entry index value out of bounds.",
		 function );

		return( -1 );
	}
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
	if( libfdata_vector_get_element_value_by_index(
	     mft->mft_entry_vector,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) mft->mft_entry_cache,
	     (int) mft_entry_index,
	     (intptr_t **) &safe_mft_entry,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT entry: %" PRIu64 ".",
		 function,
		 mft_entry_index );

		return( -1 );
	}
	if( libfsntfs_mft_entry_read_attributes(
	     safe_mft_entry,
	     mft->io_handle,
	     file_io_handle,
	     mft->mft_entry_vector,
	     mft->attribute_list_tree,
	     mft->flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read MFT entry: %d attributes.",
		 function,
		 mft_entry_index );

		return( -1 );
	}
	*mft_entry = safe_mft_entry;

	return( 1 );
}

/* Retrieves the MFT entry for a specific index
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_mft_get_mft_entry_by_index_no_cache(
     libfsntfs_mft_t *mft,
     libbfio_handle_t *file_io_handle,
     uint64_t mft_entry_index,
     libfsntfs_mft_entry_t **mft_entry,
     libcerror_error_t **error )
{
	libfsntfs_mft_entry_t *safe_mft_entry = NULL;
	static char *function                 = "libfsntfs_mft_get_mft_entry_by_index_no_cache";

	if( mft == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT.",
		 function );

		return( -1 );
	}
	if( mft_entry_index > mft->number_of_mft_entries )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid MFT entry index value out of bounds.",
		 function );

		return( -1 );
	}
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
	if( libfdata_vector_get_element_value_by_index(
	     mft->mft_entry_vector,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) mft->single_mft_entry_cache,
	     (int) mft_entry_index,
	     (intptr_t **) &safe_mft_entry,
	     LIBFDATA_READ_FLAG_IGNORE_CACHE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve MFT entry: %" PRIu64 ".",
		 function,
		 mft_entry_index );

		return( -1 );
	}
	if( libfsntfs_mft_entry_read_attributes(
	     safe_mft_entry,
	     mft->io_handle,
	     file_io_handle,
	     mft->mft_entry_vector,
	     mft->attribute_list_tree,
	     mft->flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read MFT entry: %d attributes.",
		 function,
		 mft_entry_index );

		return( -1 );
	}
	if( libfcache_cache_clear_value_by_index(
	     mft->single_mft_entry_cache,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to clear single MFT entry cache entry: 0.",
		 function );

		return( -1 );
	}
	*mft_entry = safe_mft_entry;

	return( 1 );
}

